// Example x86 (32-bit) page-fault handler in *one function*.
// Two variants are common on 32-bit:
//   A) 2-level paging (no PAE):   PD -> PT -> 4KiB pages (and optional 4MiB via PDE.PS)
//   B) PAE paging (CR4.PAE=1):    PDPT -> PD -> PT -> 4KiB pages (and optional 2MiB via PDE.PS)
//
// This example implements the SIMPLE/CLASSIC case: 2-level (no PAE), 4KiB pages.
// It walks:  CR3(PD) -> PDE -> PTE. If non-present, it allocates intermediate tables and maps a page.
//
// Assumptions (toy kernel):
//   - you have a direct-map phys->virt to access page tables:
//       void* PHYS_TO_VIRT(uint32_t pa);
//   - you can allocate a zeroed 4KiB physical page:
//       uint32_t alloc_phys_page_4k(void);  // returns phys addr (4KiB aligned) or 0 on OOM
//   - you can invalidate one TLB entry:
//       void invlpg(void* va);
//   - you can read CR2 (fault VA) and CR3 (page directory base):
//       uint32_t read_cr2(void), read_cr3(void);
//   - panic/logging primitive:
//       void panic(const char* msg);

void page_fault_handler_x86_32_example(uint32_t error_code)
{
    // ---- "helpers" as macros to keep ONE function ----
    #define PGSIZE        4096U
    #define PAGE_MASK     (~(PGSIZE - 1U))

    // 32-bit PDE/PTE flags (subset)
    #define PTE_P         (1U << 0)  // Present
    #define PTE_W         (1U << 1)  // Writable
    #define PTE_U         (1U << 2)  // User
    #define PTE_PWT       (1U << 3)
    #define PTE_PCD       (1U << 4)
    #define PTE_A         (1U << 5)
    #define PTE_D         (1U << 6)
    #define PTE_PS        (1U << 7)  // Page size (in PDE: 4MiB if CR4.PSE=1)
    #define PTE_G         (1U << 8)

    #define ADDR_MASK     0xFFFFF000U  // bits 12..31

    // Page-fault error code bits (architectural)
    #define PFEC_P        (1U << 0)  // 0=non-present, 1=protection violation
    #define PFEC_WR       (1U << 1)  // 1=write, 0=read
    #define PFEC_US       (1U << 2)  // 1=user, 0=supervisor
    #define PFEC_RSVD     (1U << 3)  // reserved bit violation
    // (PFEC_ID doesn't exist in classic 32-bit non-PAE paging)

    // Indices for 2-level paging
    #define PD_IDX(va)    (((va) >> 22) & 0x3FFU)  // top 10 bits
    #define PT_IDX(va)    (((va) >> 12) & 0x3FFU)  // next 10 bits

    extern uint32_t read_cr2(void);
    extern uint32_t read_cr3(void);
    extern void*    PHYS_TO_VIRT(uint32_t pa);
    extern uint32_t alloc_phys_page_4k(void);
    extern void     invlpg(void* va);
    extern void     panic(const char* msg);

    // ---- handler starts ----
    uint32_t fault_va = read_cr2();

    // Reserved-bit violation = page table corruption/wrong bits.
    if (error_code & PFEC_RSVD) {
        panic("page fault: RSVD bit violation");
    }

    // If it's a protection violation (present=1), this demo treats as fatal.
    // Real OS would handle COW, permissions, etc.
    if (error_code & PFEC_P) {
        panic("page fault: protection violation (present=1)");
    }

    // Non-present fault: do demand allocation of a 4KiB page.
    // Toy policy:
    //   - Map writable if the faulting access was a write (PFEC_WR)
    //   - Map user-accessible if the fault happened in user mode (PFEC_US)
    //
    // Note: In many kernels, you'd also check whether 'fault_va' is in a valid region
    // (e.g., user stack growth, heap, etc.). This demo simply maps it.

    uint32_t cr3 = read_cr3();
    uint32_t pd_pa = cr3 & ADDR_MASK;                     // CR3 holds PD base (aligned)
    uint32_t* pd = (uint32_t*)PHYS_TO_VIRT(pd_pa);

    uint32_t pde_idx = PD_IDX(fault_va);
    uint32_t pde = pd[pde_idx];

    // If PDE present but PS set, it's a 4MiB mapping. Demo doesn't handle splitting it.
    if ((pde & PTE_P) && (pde & PTE_PS)) {
        panic("page fault: hit 4MiB page mapping (demo doesn't handle)");
    }

    // Ensure PT exists
    if ((pde & PTE_P) == 0) {
        uint32_t new_pt_pa = alloc_phys_page_4k();
        if (!new_pt_pa) panic("OOM allocating PT");

        // Zero PT
        uint32_t* new_pt = (uint32_t*)PHYS_TO_VIRT(new_pt_pa);
        for (int i = 0; i < 1024; i++) new_pt[i] = 0;

        uint32_t flags = PTE_P | PTE_W;                   // make tables writable
        if (error_code & PFEC_US) flags |= PTE_U;         // user can traverse if needed

        pde = (new_pt_pa & ADDR_MASK) | flags;
        pd[pde_idx] = pde;
    }

    uint32_t pt_pa = pde & ADDR_MASK;
    uint32_t* pt = (uint32_t*)PHYS_TO_VIRT(pt_pa);

    uint32_t pte_idx = PT_IDX(fault_va);
    uint32_t pte = pt[pte_idx];

    // Map the final 4KiB page
    if ((pte & PTE_P) == 0) {
        uint32_t new_page_pa = alloc_phys_page_4k();
        if (!new_page_pa) panic("OOM allocating data page");

        // Optional: zero page (if allocator doesn't already)
        uint8_t* page = (uint8_t*)PHYS_TO_VIRT(new_page_pa);
        for (uint32_t i = 0; i < PGSIZE; i++) page[i] = 0;

        uint32_t flags = PTE_P;
        if (error_code & PFEC_WR) flags |= PTE_W;         // writable if fault was write
        if (error_code & PFEC_US) flags |= PTE_U;         // user page if user fault

        pt[pte_idx] = (new_page_pa & ADDR_MASK) | flags;

        invlpg((void*)(fault_va & PAGE_MASK));
        return;
    }

    // Inconsistent state: error says non-present but PTE is present.
    panic("page fault: inconsistent state (P=0 but PTE present)");
}


// Example x86-64 (4-level) page-fault handler in *one function*.
// Goal: show the complete walk PML4 -> PDPT -> PD -> PT, then either:
//   - handle non-present by allocating + mapping a 4KiB page (demand paging), OR
//   - treat protection violations as fatal.
//
// Assumptions (typical toy kernel):
//   - You have a direct-map (phys->virt) region so you can read/write page tables:
//       void* PHYS_TO_VIRT(uint64_t pa);
//   - You can allocate a zeroed 4KiB physical page:
//       uint64_t alloc_phys_page_4k(void);  // returns physical address, 4KiB aligned, or 0 on OOM
//   - You can invalidate a single TLB entry:
//       void invlpg(void* va);
//   - You can log/panic/kill-current-task as you like (here: panic()).
//
// NOTE: This is an *example*; production kernels also handle:
//   - huge pages (PS bit in PDPTE/PDE),
//   - NX, SMEP/SMAP, copy-on-write, stack growth policies,
//   - different address spaces (CR3 changes), etc.

void page_fault_handler_x86_64_example(uint64_t error_code)
{
    // ----- tiny "helpers" expressed as macros so we keep ONE function -----
    #define PGSIZE            4096ULL
    #define PAGE_MASK         (~(PGSIZE - 1ULL))

    // PTE bits (subset)
    #define PTE_P             (1ULL << 0)   // Present
    #define PTE_W             (1ULL << 1)   // Writable
    #define PTE_U             (1ULL << 2)   // User
    #define PTE_PWT           (1ULL << 3)
    #define PTE_PCD           (1ULL << 4)
    #define PTE_A             (1ULL << 5)
    #define PTE_D             (1ULL << 6)
    #define PTE_PS            (1ULL << 7)   // Page size (if set in PDE/PDPTE => huge page)
    #define PTE_G             (1ULL << 8)
    #define PTE_NX            (1ULL << 63)  // if supported/enabled via EFER.NXE

    #define ADDR_MASK         0x000FFFFFFFFFF000ULL  // bits 12..51

    // error_code bits (architectural)
    #define PFEC_P            (1ULL << 0)   // 0=non-present, 1=protection violation
    #define PFEC_WR           (1ULL << 1)   // 1=write, 0=read
    #define PFEC_US           (1ULL << 2)   // 1=user, 0=supervisor
    #define PFEC_RSVD         (1ULL << 3)   // reserved bit violation
    #define PFEC_ID           (1ULL << 4)   // instruction fetch (if supported)

    // 4-level indices
    #define IDX_PML4(va)      (((va) >> 39) & 0x1FFULL)
    #define IDX_PDPT(va)      (((va) >> 30) & 0x1FFULL)
    #define IDX_PD(va)        (((va) >> 21) & 0x1FFULL)
    #define IDX_PT(va)        (((va) >> 12) & 0x1FFULL)

    // Replace these with your kernel primitives:
    extern uint64_t read_cr2(void);
    extern uint64_t read_cr3(void);
    extern void*    PHYS_TO_VIRT(uint64_t pa);
    extern uint64_t alloc_phys_page_4k(void);
    extern void     invlpg(void* va);
    extern void     panic(const char* msg);

    // ----- start of real handler -----
    uint64_t fault_va = read_cr2();

    // Basic sanity reporting / early-fatal conditions:
    if (error_code & PFEC_RSVD) {
        // Reserved-bit violation usually means corrupted page tables or wrong bits set.
        panic("page fault: RSVD bit violation");
    }

    // If it's a protection violation (Present=1), this demo kernel just dies.
    // (Real kernels may handle COW, NX faults, SMAP/SMEP, etc.)
    if (error_code & PFEC_P) {
        panic("page fault: protection violation (present=1)");
    }

    // Non-present fault: do demand allocation of a 4KiB page at the fault address.
    // Policy decisions (toy):
    //  - allow user faults only if PFEC_US set (or allow both)
    //  - map writable if the fault was a write (PFEC_WR)
    //  - mark user-accessible if PFEC_US set

    uint64_t cr3 = read_cr3();
    uint64_t pml4_pa = cr3 & ADDR_MASK;                  // CR3 points to PML4 phys base
    uint64_t* pml4 = (uint64_t*)PHYS_TO_VIRT(pml4_pa);   // direct-map to access it

    uint64_t pml4e_idx = IDX_PML4(fault_va);
    uint64_t pml4e = pml4[pml4e_idx];

    // Ensure PDPT exists
    if ((pml4e & PTE_P) == 0) {
        uint64_t new_pdpt_pa = alloc_phys_page_4k();
        if (!new_pdpt_pa) panic("OOM allocating PDPT");
        // Zero the new table
        uint64_t* new_pdpt = (uint64_t*)PHYS_TO_VIRT(new_pdpt_pa);
        for (int i = 0; i < 512; i++) new_pdpt[i] = 0;

        // Present + writable; user bit if this is user fault.
        uint64_t flags = PTE_P | PTE_W;
        if (error_code & PFEC_US) flags |= PTE_U;

        pml4e = (new_pdpt_pa & ADDR_MASK) | flags;
        pml4[pml4e_idx] = pml4e;
    }

    uint64_t pdpt_pa = pml4e & ADDR_MASK;
    uint64_t* pdpt = (uint64_t*)PHYS_TO_VIRT(pdpt_pa);

    uint64_t pdpte_idx = IDX_PDPT(fault_va);
    uint64_t pdpte = pdpt[pdpte_idx];

    // Handle huge pages? (This demo chooses to reject; you can extend.)
    if ((pdpte & PTE_P) && (pdpte & PTE_PS)) {
        panic("page fault: hit 1GiB huge page mapping (demo doesn't handle)");
    }

    // Ensure PD exists
    if ((pdpte & PTE_P) == 0) {
        uint64_t new_pd_pa = alloc_phys_page_4k();
        if (!new_pd_pa) panic("OOM allocating PD");
        uint64_t* new_pd = (uint64_t*)PHYS_TO_VIRT(new_pd_pa);
        for (int i = 0; i < 512; i++) new_pd[i] = 0;

        uint64_t flags = PTE_P | PTE_W;
        if (error_code & PFEC_US) flags |= PTE_U;

        pdpte = (new_pd_pa & ADDR_MASK) | flags;
        pdpt[pdpte_idx] = pdpte;
    }

    uint64_t pd_pa = pdpte & ADDR_MASK;
    uint64_t* pd = (uint64_t*)PHYS_TO_VIRT(pd_pa);

    uint64_t pde_idx = IDX_PD(fault_va);
    uint64_t pde = pd[pde_idx];

    if ((pde & PTE_P) && (pde & PTE_PS)) {
        panic("page fault: hit 2MiB huge page mapping (demo doesn't handle)");
    }

    // Ensure PT exists
    if ((pde & PTE_P) == 0) {
        uint64_t new_pt_pa = alloc_phys_page_4k();
        if (!new_pt_pa) panic("OOM allocating PT");
        uint64_t* new_pt = (uint64_t*)PHYS_TO_VIRT(new_pt_pa);
        for (int i = 0; i < 512; i++) new_pt[i] = 0;

        uint64_t flags = PTE_P | PTE_W;
        if (error_code & PFEC_US) flags |= PTE_U;

        pde = (new_pt_pa & ADDR_MASK) | flags;
        pd[pde_idx] = pde;
    }

    uint64_t pt_pa = pde & ADDR_MASK;
    uint64_t* pt = (uint64_t*)PHYS_TO_VIRT(pt_pa);

    uint64_t pte_idx = IDX_PT(fault_va);
    uint64_t pte = pt[pte_idx];

    // Finally, map the faulting 4KiB page if not present.
    if ((pte & PTE_P) == 0) {
        uint64_t new_page_pa = alloc_phys_page_4k();
        if (!new_page_pa) panic("OOM allocating data page");

        // Optional: zero the new page (depends if alloc already returns zeroed)
        uint8_t* page = (uint8_t*)PHYS_TO_VIRT(new_page_pa);
        for (uint64_t i = 0; i < PGSIZE; i++) page[i] = 0;

        uint64_t flags = PTE_P;
        // allow write if the faulting access was a write (toy policy)
        if (error_code & PFEC_WR) flags |= PTE_W;
        // user page if fault came from user
        if (error_code & PFEC_US) flags |= PTE_U;

        // If you support NX, you could set NX based on region policy; demo leaves executable.
        // flags |= PTE_NX;

        pt[pte_idx] = (new_page_pa & ADDR_MASK) | flags;

        // Invalidate the TLB entry for that VA (safe even if not cached).
        invlpg((void*)(fault_va & PAGE_MASK));
        return;
    }

    // If we got here, it was "non-present" according to error_code, but PTE is present.
    // That usually indicates a race, stale TLB, or inconsistent tables in a real OS.
    // In a single-core toy kernel, treat as fatal to catch bugs.
    panic("page fault: inconsistent state (P=0 but PTE present)");
}


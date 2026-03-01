

/* XXX:
 *
 * There are no typos and compilation error
 * Because we can do: gcc -c va-to-pa.c
 */



#include <stdint.h>


#define PML4_INDEX (12+9+9+9)
#define PDPT_INDEX (12+9+9)
#define PD_INDEX (12+9)
#define PT_INDEX (12)

#define INDEX_MASK ((1<<9)-1)

#define PTE_P  (1<<0)
#define PTE_PS (1<<7) /* XXX: Huge Pages */

#define HP_1GB_INDEX ((1<<30)-1)
#define HP_2MB_INDEX ((2<<20)-1)

extern uint64_t P2V(uint64_t pa);
extern uint64_t read_cr3(void);

uint64_t va_to_pa(uint64_t va) {
    uint64_t cr3 = read_cr3();
    uint64_t pml4_pa = cr3 & ~0xFFF; /* XXX: It contains the PCID;
                                        PCID = Process Context Identifier
                                        It is a 12-bit tag stored in CR3 (when enabled) that 
                                            allows the CPU to avoid flushing the TLB on context switches

                                        So there can be 12bits = 4KB processes TLB at once
                                     */
    uint64_t * pml4 = (uint64_t*)P2V(pml4_pa);

    uint64_t pml4_e = pml4[(va >> PML4_INDEX) & INDEX_MASK];
    if (!(pml4_e & PTE_P)) return 0;
    uint64_t pdpt_pa = pml4_e & ~0xFFF;
    uint64_t * pdpt = (uint64_t*)P2V(pdpt_pa);

    uint64_t pdpt_e = pdpt[(va >> PDPT_INDEX) & INDEX_MASK];
    if (!(pdpt_e & PTE_P)) return 0;

    /*XXX: 1 GB Huge Page */
    if (pdpt_e & PTE_PS) {
        uint64_t page_base = pdpt_e & ~HP_1GB_INDEX;
        return page_base | (va & HP_1GB_INDEX);
    }

    uint64_t pd_pa = pdpt_e &  ~0xFFF;
    uint64_t *pd = (uint64_t *)P2V(pd_pa);
    
    uint64_t pd_e = pd[(va >> PD_INDEX) & INDEX_MASK];
    if (!(pd_e & PTE_P)) return 0;

    /*XXX: 2 MB Huge Page */
    if (pd_e & PTE_PS) {
        uint64_t page_base = pd_e & ~HP_2MB_INDEX;
        return page_base | (va & HP_2MB_INDEX);
    }

    uint64_t pt_pa = pd_e & ~0xFFF;
    uint64_t * pt =(uint64_t*)  P2V(pt_pa);

    uint64_t pte = pt[(va >> PT_INDEX) & INDEX_MASK];
    if (!(pte & PTE_P)) return 0;

    uint64_t page_base = pte & ~0xFFF;
    return page_base | (va & 0xFFF);
}





    

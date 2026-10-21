#include "../include/common.h"
#include <intrin.h>

typedef union _EPT_PTE {
    struct {
        unsigned __int64 Read : 1;
        unsigned __int64 Write : 1;
        unsigned __int64 Execute : 1;
        unsigned __int64 MemoryType : 3;
        unsigned __int64 IgnorePat : 1;
        unsigned __int64 LargePage : 1;
        unsigned __int64 Accessed : 1;
        unsigned __int64 Dirty : 1;
        unsigned __int64 UserModeExecute : 1;
        unsigned __int64 Reserved1 : 1;
        unsigned __int64 PhysicalAddress : 36;
        unsigned __int64 Reserved2 : 16;
        unsigned __int64 SuppressVe : 1;
    } Fields;
    unsigned __int64 All;
} EPT_PTE, *PEPT_PTE;

typedef union _EPT_PML4E {
    unsigned __int64 All;
} EPT_PML4E, *PEPT_PML4E;

typedef union _EPT_PDPTE {
    unsigned __int64 All;
} EPT_PDPTE, *PEPT_PDPTE;

void InveptSingleContext(unsigned __int64 eptp) {
    unsigned __int64 invept_type = 1; // Single Context
    unsigned __int64 descriptor[2];
    descriptor[0] = eptp;
    descriptor[1] = 0;
    _invept(invept_type, descriptor);
}

void SetupEptHook(unsigned __int64 eptp, unsigned __int64 target_phys, unsigned __int64 fake_phys) {
    EPT_PML4E* pml4 = (EPT_PML4E*)(eptp & 0xFFFFFFFFF000);
    if (!pml4) return;

    EPT_PDPTE* pdpt = (EPT_PDPTE*)(pml4[(target_phys >> 39) & 0x1FF].All & 0xFFFFFFFFF000);
    if (!pdpt) return;

    unsigned __int64* pd = (unsigned __int64*)(pdpt[(target_phys >> 30) & 0x1FF].All & 0xFFFFFFFFF000);
    if (!pd) return;

    unsigned __int64 pd_index = (target_phys >> 21) & 0x1FF;

    if (pd[pd_index] & (1 << 7)) { // Large Page (2MB)
        // I-allocate ang bagong 4KB Page Table
        EPT_PTE* new_pt = (EPT_PTE*)ExAllocatePoolWithTag(NonPagedPool, 4096, 'TPE');
        if (!new_pt) return;
        RtlZeroMemory(new_pt, 4096);

        unsigned __int64 base_2mb = target_phys & ~0x1FFFFF;

        // I-populate ang 4KB pages
        for (int i = 0; i < 512; i++) {
            new_pt[i].All = (pd[pd_index] & ~0x1FFFFF) + (i * 0x1000);
            new_pt[i].Fields.LargePage = 0;
        }

        // I-update ang PD entry para tumuro sa bagong PT, at i-clear ang LargePage bit
        pd[pd_index] = (unsigned __int64)MmGetPhysicalAddress(new_pt).QuadPart | 0x7; // RWX
        pd[pd_index] &= ~(1ULL << 7); 
    }

    // Ngayon, i-redirect ang target page papunta sa fake page
    EPT_PTE* pt = (EPT_PTE*)(pd[pd_index] & 0xFFFFFFFFF000);
    unsigned __int64 pt_index = (target_phys >> 12) & 0x1FF;
    pt[pt_index].Fields.PhysicalAddress = fake_phys >> 12;

    InveptSingleContext(eptp);
}

// language: C++, file: src/hv/svm_core.cpp
#include "../include/common.h"
#include "svm_defs.h"
#include <intrin.h>

extern "C" void AsmSvmLaunch(void* vmcb_pa);
extern "C" void SvmExitHandler_C(VMCB* vmcb) {
    unsigned __int64 exit_code = vmcb->ControlArea.ExitCode;

    if (exit_code == SVM_EXIT_CPUID) {
        unsigned int leaf = (unsigned int)vmcb->StateSaveArea.Rax;
        
        // Hide Hypervisor Presence
        if (leaf == 0x40000000) {
            vmcb->StateSaveArea.Rax = 0;
            vmcb->StateSaveArea.Rbx = 0;
            vmcb->StateSaveArea.Rcx = 0;
            vmcb->StateSaveArea.Rdx = 0;
        } else {
            int cpu_info[4];
            __cpuidex(cpu_info, leaf, (unsigned int)vmcb->StateSaveArea.Rcx);
            vmcb->StateSaveArea.Rax = cpu_info[0];
            vmcb->StateSaveArea.Rbx = cpu_info[1];
            vmcb->StateSaveArea.Rcx = cpu_info[2];
            vmcb->StateSaveArea.Rdx = cpu_info[3];
        }
    } 
    else if (exit_code == SVM_EXIT_MSR) {
        unsigned int msr_id = (unsigned int)vmcb->StateSaveArea.Rcx;
        
        // Handle RDMSR / WRMSR
        if (vmcb->ControlArea.ExitInfo1 == 0) { // Read
            unsigned __int64 val = __readmsr(msr_id);
            vmcb->StateSaveArea.Rax = val & 0xFFFFFFFF;
            vmcb->StateSaveArea.Rdx = val >> 32;
        } else { // Write
            unsigned __int64 val = (vmcb->StateSaveArea.Rdx << 32) | (vmcb->StateSaveArea.Rax & 0xFFFFFFFF);
            __writemsr(msr_id, val);
        }
    }

    // Advance RIP to prevent infinite loop
    vmcb->StateSaveArea.Rip += vmcb->ControlArea.NumBytes;
}

NTSTATUS StartAmdHypervisor() {
    // Check AMD SVM Support
    int cpu_info[4];
    __cpuid(cpu_info, 0x80000001);
    if (!(cpu_info[2] & (1 << 2))) return STATUS_NOT_SUPPORTED;

    // Enable SVME bit in EFER
    unsigned __int64 efer = __readmsr(0xC0000080);
    efer |= (1 << 12);
    __writemsr(0xC0000080, efer);

    // Allocate VMCB (Must be physically contiguous and 4KB aligned)
    PVMCB vmcb = (PVMCB)ExAllocatePoolWithTag(NonPagedPool, sizeof(VMCB), 'BCMV');
    if (!vmcb) return STATUS_INSUFFICIENT_RESOURCES;
    RtlZeroMemory(vmcb, sizeof(VMCB));

    // Setup Intercepts (CPUID and MSR)
    vmcb->ControlArea.InterceptCpuid = 1;
    vmcb->ControlArea.InterceptMsr = 1;

    // Set initial guest state
    vmcb->StateSaveArea.Rip = (unsigned __int64)AsmSvmLaunch; // Pwede ring default RIP ng caller
    vmcb->StateSaveArea.Rsp = __readrsp();
    vmcb->StateSaveArea.Rax = 0;

    // Launch SVM
    unsigned __int64 vmcb_pa = MmGetPhysicalAddress(vmcb).QuadPart;
    AsmSvmLaunch((void*)vmcb_pa);

    return STATUS_SUCCESS;
}

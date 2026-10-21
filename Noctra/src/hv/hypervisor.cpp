// language: C++, file: src/hv/hypervisor.cpp
#include "../include/common.h"
#include "svm_defs.h"
#include <intrin.h>

extern "C" void AsmVmxLaunch(void* vmcs_region);
extern "C" void AsmVmExitHandler();
extern NTSTATUS StartAmdHypervisor();

unsigned __int64 VmcsRead(unsigned long field) { unsigned __int64 v; __vmx_vmread(field, &v); return v; }
void VmcsWrite(unsigned long field, unsigned __int64 val) { __vmx_vmwrite(field, val); }

extern "C" void VmExitHandler_C(unsigned __int64* regs) {
    unsigned __int64 exit_reason = VmcsRead(0x4402);
    if (exit_reason == 10) { // CPUID
        int cpu_info[4];
        __cpuidex(cpu_info, (unsigned int)regs[0], (unsigned int)regs[2]);
        regs[0] = cpu_info[0]; regs[1] = cpu_info[1]; regs[2] = cpu_info[2]; regs[3] = cpu_info[3];
    } else if (exit_reason == 31) { // RDMSR
        unsigned __int64 val = __readmsr((unsigned int)regs[2]);
        regs[0] = val & 0xFFFFFFFF; regs[3] = val >> 32;
    }
    // Advance RIP
    VmcsWrite(0x681E, VmcsRead(0x681E) + VmcsRead(0x640C));
}

NTSTATUS StartIntelHypervisor() {
    int cpu_info[4];
    __cpuid(cpu_info, 1);
    if (!(cpu_info[2] & (1 << 5))) return STATUS_NOT_SUPPORTED;

    unsigned __int64 cr4 = __readcr4(); cr4 |= (1 << 13); __writecr4(cr4);

    void* vmxon_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'XONV');
    void* vmcs_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'CSMV');
    if (!vmxon_region || !vmcs_region) return STATUS_INSUFFICIENT_RESOURCES;

    unsigned __int64 vmx_msr = __readmsr(0x480);
    *(unsigned __int64*)vmxon_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;
    *(unsigned __int64*)vmcs_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;

    unsigned __int64 vmxon_pa = MmGetPhysicalAddress(vmxon_region).QuadPart;
    unsigned __int64 vmcs_pa = MmGetPhysicalAddress(vmcs_region).QuadPart;

    if (__vmx_on(&vmxon_pa) != 0) return STATUS_UNSUCCESSFUL;
    if (__vmx_vmptrld(&vmcs_pa) != 0) return STATUS_UNSUCCESSFUL;

    // Setup VMCS Execution Controls
    VmcsWrite(0x4002, (1 << 21) | (1 << 15)); // CPUID & RDMSR exits
    
    // Setup Guest State
    VmcsWrite(0x6800, __readcs());
    VmcsWrite(0x6802, __readds());
    VmcsWrite(0x6804, __reades());
    VmcsWrite(0x6806, __readfs());
    VmcsWrite(0x6808, __readgs());
    VmcsWrite(0x680A, __readss());
    VmcsWrite(0x681E, (unsigned __int64)AsmVmExitHandler); // Guest RIP
    VmcsWrite(0x6820, __readrsp());
    VmcsWrite(0x6822, __readrflags());

    AsmVmxLaunch(vmcs_region);
    return STATUS_SUCCESS;
}

NTSTATUS StartHypervisor() {
    int cpu_info[4];
    __cpuid(cpu_info, 0);
    
    // Check GenuineIntel
    if (cpu_info[1] == 0x756E6547 && cpu_info[3] == 0x49656E69 && cpu_info[2] == 0x6C65746E) {
        return StartIntelHypervisor();
    }
    // Check AuthenticAMD
    if (cpu_info[1] == 0x68747541 && cpu_info[3] == 0x69746E65 && cpu_info[2] == 0x444D4163) {
        return StartAmdHypervisor();
    }
    
    return STATUS_NOT_SUPPORTED;
}

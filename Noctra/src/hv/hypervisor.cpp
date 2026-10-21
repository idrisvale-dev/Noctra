#include "../include/common.h"
#include <intrin.h>

extern "C" void AsmVmxLaunch(void* vmcs_region);
extern "C" void AsmVmExitHandler();
extern void SetupEptHook(unsigned __int64 eptp, unsigned __int64 target_phys, unsigned __int64 fake_phys);

PVOID g_FakePage = NULL;

unsigned __int64 VmcsRead(unsigned long field) {
    unsigned __int64 val;
    __vmx_vmread(field, &val);
    return val;
}

void VmcsWrite(unsigned long field, unsigned __int64 val) {
    __vmx_vmwrite(field, val);
}

extern "C" void VmExitHandler_C(unsigned __int64* regs) {
    unsigned __int64 exit_reason = VmcsRead(0x4402);

    if (exit_reason == 10) { // CPUID
        unsigned int leaf = (unsigned int)regs[0]; 
        if (leaf == 0x40000000) {
            regs[0] = 0; regs[1] = 0; regs[2] = 0; regs[3] = 0; 
        } else {
            int cpu_info[4];
            __cpuidex(cpu_info, leaf, (unsigned int)regs[2]);
            regs[0] = cpu_info[0]; regs[1] = cpu_info[1];
            regs[2] = cpu_info[2]; regs[3] = cpu_info[3];
        }
    } else if (exit_reason == 31) { // RDMSR
        unsigned int msr_id = (unsigned int)regs[2];
        regs[0] = __readmsr(msr_id) & 0xFFFFFFFF;
        regs[3] = __readmsr(msr_id) >> 32;
    }

    unsigned __int64 guest_rip = VmcsRead(0x681E);
    unsigned __int64 instr_len = VmcsRead(0x640C);
    VmcsWrite(0x681E, guest_rip + instr_len);
}

NTSTATUS StartHypervisor() {
    int cpu_info[4];
    __cpuid(cpu_info, 1);
    if (!(cpu_info[2] & (1 << 5))) return STATUS_NOT_SUPPORTED;

    unsigned __int64 cr4 = __readcr4();
    cr4 |= (1 << 13); 
    __writecr4(cr4);

    void* vmxon_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'XONV');
    void* vmcs_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'CSMV');
    g_FakePage = ExAllocatePoolWithTag(NonPagedPool, 4096, 'KAFE'); // Fake page para sa EPT
    if (!vmxon_region || !vmcs_region || !g_FakePage) return STATUS_INSUFFICIENT_RESOURCES;

    unsigned __int64 vmx_msr = __readmsr(0x480);
    *(unsigned __int64*)vmxon_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;
    *(unsigned __int64*)vmcs_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;

    if (__vmx_on(&vmxon_region) != 0) return STATUS_UNSUCCESSFUL;
    if (__vmx_vmptrld(&vmcs_region) != 0) return STATUS_UNSUCCESSFUL;

    VmcsWrite(0x4002, (1 << 21) | (1 << 15)); // CPUID & RDMSR exits
    VmcsWrite(0x6800, __readcs());
    VmcsWrite(0x6802, __readds());
    VmcsWrite(0x6804, __reades());
    VmcsWrite(0x6806, __readfs());
    VmcsWrite(0x6808, __readgs());
    VmcsWrite(0x680A, __readss());
    VmcsWrite(0x681E, (unsigned __int64)AsmVmExitHandler);
    VmcsWrite(0x6820, __readrsp());
    VmcsWrite(0x6822, __readrflags());

    // EPT Setup (Halimbawa: i-hook ang isang address)
    // SetupEptHook(VmcsRead(0x2010), 0x12345000, MmGetPhysicalAddress(g_FakePage).QuadPart);

    AsmVmxLaunch(vmcs_region);
    return STATUS_SUCCESS;
}

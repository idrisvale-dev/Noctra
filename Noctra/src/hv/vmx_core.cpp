// VMX Core setup and VMCS management
#include <ntddk.h>
#include <intrin.h>

extern "C" void AsmVmxLaunch(void* vmcs_region);
extern "C" void AsmVmFunc(unsigned __int64 index);

// Forward declaration of C handler for assembly
extern "C" void VmExitHandler_C(void* regs) {
    // Handle CPUID and MSR exits here
}

unsigned __int64 VmcsRead(unsigned long field) {
    unsigned __int64 val;
    __vmx_vmread(field, &val);
    return val;
}

void VmcsWrite(unsigned long field, unsigned __int64 val) {
    __vmx_vmwrite(field, val);
}

bool InitializeHypervisor() {
    int cpu_info[4];
    __cpuid(cpu_info, 1);
    if (!(cpu_info[2] & (1 << 5))) return false;

    unsigned __int64 cr4 = __readcr4();
    cr4 |= (1 << 13);
    __writecr4(cr4);

    void* vmxon_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'XONV');
    void* vmcs_region = ExAllocatePoolWithTag(NonPagedPool, 4096, 'CSMV');
    if (!vmxon_region || !vmcs_region) return false;

    unsigned __int64 vmx_msr = __readmsr(0x480);
    *(unsigned __int64*)vmxon_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;
    *(unsigned __int64*)vmcs_region = vmx_msr & 0x7FFFFFFFFFFFFFFF;

    unsigned char status = __vmx_on(&vmxon_region);
    if (status != 0) return false;

    __vmx_vmptrld(&vmcs_region);
    VmcsWrite(0x4002, (1 << 21) | (1 << 15));
    
    AsmVmxLaunch(vmcs_region);
    return true;
}

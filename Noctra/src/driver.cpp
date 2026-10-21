// Noctra Main Driver Entry
#include <ntddk.h>

extern bool InitializeHypervisor();
extern void SpoofSMBIOS();
extern void BypassMultiInstance();
extern void PurgeSystemMemory();
extern VOID ProcessNotifyCallback(PEPROCESS, HANDLE, PPS_CREATE_NOTIFY_INFO);

extern "C" NTSTATUS DriverEntry(void* DriverObject, void* RegistryPath) {
    PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallback, FALSE);
    BypassMultiInstance();
    PurgeSystemMemory();
    SpoofSMBIOS();
    
    // Uncomment to launch hypervisor
    // InitializeHypervisor();

    return STATUS_SUCCESS;
}

extern "C" void DriverUnload(void* DriverObject) {
    PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallback, TRUE);
}

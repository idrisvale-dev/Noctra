// Blocks malicious scripts by process termination
#include <ntddk.h>

VOID ProcessNotifyCallback(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo) {
    if (CreateInfo != NULL && CreateInfo->ImageFileName != NULL) {
        if (wcsstr(CreateInfo->ImageFileName->Buffer, L"\\Downloads\\") || 
            wcsstr(CreateInfo->ImageFileName->Buffer, L"\\Temp\\")) {
            
            if (wcsstr(CreateInfo->ImageFileName->Buffer, L".exe") || 
                wcsstr(CreateInfo->ImageFileName->Buffer, L".scr")) {
                
                HANDLE hProcess;
                if (NT_SUCCESS(ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE, NULL, PROCESS_ALL_ACCESS, *PsProcessType, KernelMode, &hProcess))) {
                    ZwTerminateProcess(hProcess, STATUS_ACCESS_DENIED);
                    ZwClose(hProcess);
                }
            }
        }
    }
}

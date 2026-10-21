#include "../include/common.h"

VOID ProcessNotifyCallback(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo) {
    if (CreateInfo == NULL || CreateInfo->ImageFileName == NULL) return;

    PUNICODE_STRING path = CreateInfo->ImageFileName;
    
    if (wcsstr(path->Buffer, L"\\Downloads\\") || wcsstr(path->Buffer, L"\\Temp\\")) {
        if (wcsstr(path->Buffer, L".exe") || wcsstr(path->Buffer, L".scr") || wcsstr(path->Buffer, L".bat")) {
            HANDLE hProcess;
            if (NT_SUCCESS(ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE, NULL, PROCESS_ALL_ACCESS, *PsProcessType, KernelMode, &hProcess))) {
                ZwTerminateProcess(hProcess, STATUS_ACCESS_DENIED);
                ZwClose(hProcess);
            }
        }
    }
}

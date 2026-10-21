// Multi-instance bypass and RAM purge
#include <ntddk.h>

void BypassMultiInstance() {
    UNICODE_STRING mutexName;
    RtlInitUnicodeString(&mutexName, L"\\BaseNamedObjects\\ROBLOX_singletonMutex");
    HANDLE hMutex;
    OBJECT_ATTRIBUTES objAttr;
    InitializeObjectAttributes(&objAttr, &mutexName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    
    if (NT_SUCCESS(ZwOpenMutant(&hMutex, MUTANT_ALL_ACCESS, &objAttr))) {
        ZwClose(hMutex);
    }
}

void PurgeSystemMemory() {
    int command = 4; 
    ZwSetSystemInformation((SYSTEM_INFORMATION_CLASS)80, &command, sizeof(command));
}

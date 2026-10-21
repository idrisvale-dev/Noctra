#include "../include/common.h"

void SpoofHWID() {
    HANDLE hKey;
    UNICODE_STRING regPath;
    OBJECT_ATTRIBUTES objAttr;

    RtlInitUnicodeString(&regPath, L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Cryptography");
    InitializeObjectAttributes(&objAttr, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    if (NT_SUCCESS(ZwOpenKey(&hKey, KEY_WRITE, &objAttr))) {
        UNICODE_STRING valName, valData;
        RtlInitUnicodeString(&valName, L"MachineGuid");
        RtlInitUnicodeString(&valData, L"1337-1337-1337-1337-1337");
        ZwSetValueKey(hKey, &valName, 0, REG_SZ, valData.Buffer, valData.Length + sizeof(WCHAR));
        ZwClose(hKey);
    }

    RtlInitUnicodeString(&regPath, L"\\Registry\\Machine\\HARDWARE\\DESCRIPTION\\System\\BIOS");
    InitializeObjectAttributes(&objAttr, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    if (NT_SUCCESS(ZwOpenKey(&hKey, KEY_WRITE, &objAttr))) {
        UNICODE_STRING valName, valData;
        RtlInitUnicodeString(&valName, L"SystemManufacturer");
        RtlInitUnicodeString(&valData, L"ASUS");
        ZwSetValueKey(hKey, &valName, 0, REG_SZ, valData.Buffer, valData.Length + sizeof(WCHAR));
        ZwClose(hKey);
    }
}

void BypassRobloxMutex() {
    UNICODE_STRING mutexName;
    RtlInitUnicodeString(&mutexName, L"\\BaseNamedObjects\\ROBLOX_singletonMutex");
    HANDLE hMutex;
    OBJECT_ATTRIBUTES objAttr;
    InitializeObjectAttributes(&objAttr, &mutexName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    if (NT_SUCCESS(ZwOpenMutant(&hMutex, MUTANT_ALL_ACCESS, &objAttr))) {
        ZwClose(hMutex);
    }
}

void PurgeRAM() {
    int command = 4;
    ZwSetSystemInformation((SYSTEM_INFORMATION_CLASS)80, &command, sizeof(command));
}

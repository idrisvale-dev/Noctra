// Fake HWID (Motherboard/Disk)
#include <ntddk.h>

void SpoofSMBIOS() {
    HANDLE hKey;
    UNICODE_STRING regPath;
    RtlInitUnicodeString(&regPath, L"\\Registry\\Machine\\HARDWARE\\DESCRIPTION\\System\\BIOS");
    
    OBJECT_ATTRIBUTES objAttr;
    InitializeObjectAttributes(&objAttr, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    
    if (NT_SUCCESS(ZwOpenKey(&hKey, KEY_WRITE, &objAttr))) {
        UNICODE_STRING valName, valData;
        RtlInitUnicodeString(&valName, L"SystemManufacturer");
        RtlInitUnicodeString(&valData, L"ASUS");
        ZwSetValueKey(hKey, &valName, 0, REG_SZ, valData.Buffer, valData.Length + sizeof(WCHAR));
        ZwClose(hKey);
    }
}

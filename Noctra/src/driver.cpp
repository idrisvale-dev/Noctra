#include "include/common.h"

BOOLEAN g_WatchdogEnabled = FALSE;

extern void SpoofHWID();
extern void BypassRobloxMutex();
extern void PurgeRAM();
extern VOID ProcessNotifyCallback(PEPROCESS, HANDLE, PPS_CREATE_NOTIFY_INFO);
extern NTSTATUS StartHypervisor();

NTSTATUS DispatchCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;
    
    switch (stack->Parameters.DeviceIoControl.IoControlCode) {
        case IOCTL_NOCTRA_SPOOF_HWID:
            SpoofHWID();
            break;
        case IOCTL_NOCTRA_TOGGLE_WATCHDOG:
            if (!g_WatchdogEnabled) {
                PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallback, FALSE);
                g_WatchdogEnabled = TRUE;
            } else {
                PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallback, TRUE);
                g_WatchdogEnabled = FALSE;
            }
            break;
        case IOCTL_NOCTRA_BYPASS_MUTEX:
            BypassRobloxMutex();
            break;
        case IOCTL_NOCTRA_PURGE_RAM:
            PurgeRAM();
            break;
        case IOCTL_NOCTRA_START_HV:
            status = StartHypervisor();
            break;
        default:
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNICODE_STRING devName, symLink;
    RtlInitUnicodeString(&devName, L"\\Device\\NoctraCore");
    RtlInitUnicodeString(&symLink, L"\\DosDevices\\NoctraCore");

    PDEVICE_OBJECT devObj;
    NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &devObj);
    if (!NT_SUCCESS(status)) return status;

    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(devObj);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

    return STATUS_SUCCESS;
}

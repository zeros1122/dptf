/******************************************************************************
** Copyright (c) 2013 Intel Corporation All Rights Reserved
**
** Licensed under the Apache License, Version 2.0 (the "License"); you may not
** use this file except in compliance with the License.
**
** You may obtain a copy of the License at
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
** WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**
** See the License for the specific language governing permissions and
** limitations under the License.
**
******************************************************************************/

#ifndef _ESIF_CCB_SYSTEM_H_
#define _ESIF_CCB_SYSTEM_H_

#include "esif.h"

#ifdef ESIF_ATTR_USER

#ifdef ESIF_ATTR_OS_WINDOWS
#include "powrprof.h"
#endif

// Execute System Command. Encapsulate to avoid Linux warnings when using gcc -O9
static ESIF_INLINE void esif_ccb_system(const char *cmd)
{
	int rc = system(cmd);
	UNREFERENCED_PARAMETER(rc);
}


#define system(cmd) esif_ccb_system(cmd)

// Reboot
static ESIF_INLINE void esif_ccb_reboot()
{
#if defined(ESIF_ATTR_OS_WINDOWS)
	system("shutdown /r /t 0");
#else
	system("reboot");
#endif
}


#if defined(ESIF_ATTR_OS_WINDOWS)
//
// _THERMAL_EVENT and PowerReportThermalEvent have been defined in WinBlue WDK
//
#ifndef THERMAL_EVENT_VERSION

#define THERMAL_EVENT_VERSION 1
typedef struct _THERMAL_EVENT {
	ULONG   Version;
	ULONG   Size;
	ULONG   Type;
	ULONG   Temperature;
	ULONG   TripPointTemperature;
	LPWSTR  Initiator;
} THERMAL_EVENT, *PTHERMAL_EVENT;

#endif

typedef DWORD (WINAPI * PFNPOWERREPORTTHERMALEVENT)(
	PTHERMAL_EVENT Event);

#define THERMAL_EVENT_SHUTDOWN 0
#define THERMAL_EVENT_HIBERNATE 1
#define THERMAL_EVENT_UNSPECIFIED 0xffffffff

#endif

// Enter S0 Shutdown
static ESIF_INLINE void esif_ccb_shutdown(
	UInt32 temperature,
	UInt32 tripPointTemperature
	)
{
#if defined(ESIF_ATTR_OS_WINDOWS)

	/*
	** Report Thermal Event Before Shutdown With This UNDOCUMENTED API
	** Only Available in Windows 8.1/Blue.
	*/

	HMODULE hModule = LoadLibrary(L"powrprof.dll");
	if (NULL != hModule) {
		PFNPOWERREPORTTHERMALEVENT pfnPowerReportThermalEvent = (PFNPOWERREPORTTHERMALEVENT)GetProcAddress(
				hModule,
				"PowerReportThermalEvent");

		if (NULL != pfnPowerReportThermalEvent) {
			THERMAL_EVENT t_event = {0};
			t_event.Version     = THERMAL_EVENT_VERSION;
			t_event.Size        = sizeof(THERMAL_EVENT);
			t_event.Type        = THERMAL_EVENT_SHUTDOWN;
			t_event.Temperature = temperature;
			t_event.TripPointTemperature = tripPointTemperature;
			t_event.Initiator   = L"Intel(R) Dynamic Platform Thermal Framework";

			/* Best effort we are shutting down anyway */
			pfnPowerReportThermalEvent(&t_event);
		}
		FreeLibrary(hModule);
	}
	system("shutdown /s /f /t 0");
#elif defined(ESIF_ATTR_OS_CHROME)
	system("shutdown -P now");
#else
	system("shutdown -h now");
#endif
}


// Enter S4 Hibernation
static ESIF_INLINE void esif_ccb_hibernate()
{
#if defined(ESIF_ATTR_OS_WINDOWS)
	SetSuspendState(1, 1, 0);
#elif defined(ESIF_ATTR_OS_CHROME)
	/* NA */
#else
	system("pm-hibernate");
#endif
}


// Enter S3 or CS
static ESIF_INLINE void esif_ccb_suspend()
{
#if defined(ESIF_ATTR_OS_WINDOWS)
	SetSuspendState(0, 1, 0);
#elif defined(ESIF_ATTR_OS_CHROME)
	system("powerd_dbus_suspend");
#else
	system("pm-suspend");
#endif
}


#endif /* ESIF_ATTR_USER */
#endif /* _ESIF_CCB_SYSTEM_H_ */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

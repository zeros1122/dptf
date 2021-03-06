/******************************************************************************
** Copyright (c) 2014 Intel Corporation All Rights Reserved
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

#pragma once

#include "esif_uf_app_event_type.h"
#include "DptfExport.h"
#include "FrameworkEvent.h"

// This is the list of events that policies can subscribe to.

namespace PolicyEvent
{
    enum Type
    {
        Invalid,
        DptfConnectedStandbyEntry,
        DptfConnectedStandbyExit,
        DptfSuspend,
        DptfResume,
        ParticipantSpecificInfoChanged,
        DomainConfigTdpCapabilityChanged,
        DomainCoreControlCapabilityChanged,
        DomainDisplayControlCapabilityChanged,
        DomainDisplayStatusChanged,
        DomainPerformanceControlCapabilityChanged,
        DomainPerformanceControlsChanged,
        DomainPowerControlCapabilityChanged,
        DomainPriorityChanged,
        DomainRadioConnectionStatusChanged,
        DomainRfProfileChanged,
        DomainTemperatureThresholdCrossed,
        PolicyActiveRelationshipTableChanged,
        PolicyCoolingModeAcousticLimitChanged,
        PolicyCoolingModePolicyChanged,
        PolicyCoolingModePowerLimitChanged,
        PolicyForegroundApplicationChanged,
        PolicyInitiatedCallback,
        PolicyOperatingSystemConfigTdpLevelChanged,
        PolicyOperatingSystemLpmModeChanged,
        PolicyPassiveTableChanged,
        PolicyPlatformLpmModeChanged,
        PolicySensorOrientationChanged,
        PolicySensorProximityChanged,
        PolicySensorSpatialOrientationChanged,
        PolicyThermalRelationshipTableChanged,
        Max
    };

    FrameworkEvent::Type ToFrameworkEvent(PolicyEvent::Type policyEventType);
    Bool RequiresEsifEventRegistration(PolicyEvent::Type policyEventType);
}
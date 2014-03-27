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
#include "PolicyBase.h"
#include "ParticipantTracker.h"
#include "RadioDeviceList.h"
#include "PixelClockDeviceList.h"
#include "FivrDeviceList.h"

class dptf_export ActPolicy final : public PolicyBase
{

public:

    ActPolicy(void);
    virtual ~ActPolicy(void);

    virtual void onCreate(void) override;
    virtual void onDestroy(void) override;
    virtual void onEnable(void) override;
    virtual void onDisable(void) override;

    virtual void onConnectedStandbyEntry(void);
    virtual void onConnectedStandbyExit(void);

    virtual bool autoNotifyPlatformOscOnCreateDestroy() const override;
    virtual bool autoNotifyPlatformOscOnConnectedStandbyEntryExit() const override;
    virtual bool autoNotifyPlatformOscOnEnableDisable() const override;

    virtual Guid getGuid(void) const override;
    virtual std::string getName(void) const override;
    virtual std::string getStatusAsXml(void) const override;

    virtual void onBindParticipant(UIntN participantIndex) override;
    virtual void onUnbindParticipant(UIntN participantIndex) override;
    virtual void onBindDomain(UIntN participantIndex, UIntN domainIndex) override;
    virtual void onUnbindDomain(UIntN participantIndex, UIntN domainIndex) override;

    virtual void onDomainRfProfileChanged(UIntN participantIndex) override;
    virtual void onDomainRadioConnectionStatusChanged(UIntN participantIndex,
        RadioConnectionStatus::Type radioConnectionStatus) override;
    
private:

    RadioDeviceList m_radioDevices;
    PixelClockDeviceList m_pixelClockDevices;
    FivrDeviceList m_fivrDevices;
    Bool m_filterForGps;

    void adjustFivrAndPixelClockFrequencies();
    void setDomainFrequency(DomainProxy& sourceDomain, DomainProxy &targetDomain);
};


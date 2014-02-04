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
#include "Dptf.h"
#include "RfProfileData.h"
#include "DomainProxy.h"
#include "XmlNode.h"

class dptf_export RadioDevice
{
public:

    RadioDevice(DomainProxy& domain);
    ~RadioDevice();

    UIntN getParticipantIndex() const;
    UIntN getDomainIndex() const;
    DomainType::Type getDomainType() const;
    RfProfileData getRadioProfile() const;
    Bool isConnected() const;
    Frequency getLeftFrequency() const;
    Frequency getRightFrequency() const;
    void updateRadioFrequencyProfile();
    void updateRadioConnectionStatus(RadioConnectionStatus::Type radioConnectionStatus);
    Bool isWwanDevice() const;
    Bool isWlanDevice() const;
    Bool frequencySpreadIsWithin(const Frequency& low, const Frequency& high) const;
    void refresh();
    XmlNode* getXml() const;

    static Bool isRadioDevice(const DomainProxy& domain);

private:

    DomainProxy* m_domain;
    RfProfileData m_radioProfile;
    RadioConnectionStatus::Type m_connectionStatus;

    void throwIfNotRadioDevice();
};
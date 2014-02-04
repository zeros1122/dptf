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
#include "RadioDevice.h"
#include "RfProfileData.h"
#include "XmlNode.h"
#include <list>

class dptf_export RadioDeviceList
{
public:

    RadioDeviceList();
    ~RadioDeviceList();

    void add(const RadioDevice& radioDevice);
    void remove(const DomainProxy& domain);
    RadioDevice getHighestPriorityRadio() const;
    Bool hasConnectedDevices() const;
    void updateRadioFrequencyProfiles(UIntN participantIndex);
    void updateRadioConnectionStatus(UIntN participantIndex, RadioConnectionStatus::Type radioConnectionStatus);
    void refreshAll();
    XmlNode* getXml() const;
    
private:

    std::list<RadioDevice> m_devices;
};
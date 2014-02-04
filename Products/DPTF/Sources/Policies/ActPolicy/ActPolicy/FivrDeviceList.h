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
#include "DomainProxy.h"
#include "RadioDeviceList.h"
#include "FivrDevice.h"
#include "XmlNode.h"
#include <list>

class dptf_export FivrDeviceList
{
public:

    FivrDeviceList();
    ~FivrDeviceList();

    void add(const FivrDevice& fivrDevice);
    void remove(const DomainProxy& domain);
    void adjustFrequencies(const RadioDeviceList& radioDevices);
    XmlNode* getXml() const;

private:

    std::list<FivrDevice> m_devices;

    Frequency calculateFivrFrequencyForWwan() const;
    void setFrequencyForAllFivrDevices(const Frequency& newFrequency);
};
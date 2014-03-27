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

#include "FivrDeviceList.h"

FivrDeviceList::FivrDeviceList()
{

}

FivrDeviceList::~FivrDeviceList()
{

}

void FivrDeviceList::add(const FivrDevice& fivrDevice)
{
    m_devices.insert(m_devices.end(), fivrDevice);
}

void FivrDeviceList::remove(const DomainProxy& domain)
{
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        if ((device->getParticipantIndex() == domain.getParticipantIndex()) &&
            (device->getDomainIndex() == domain.getDomainIndex()))
        {
            device = m_devices.erase(device);
            if (device == m_devices.end())
            {
                break;
            }
        }
    }
}

const Frequency fivrClockHigh((UInt64)(147.0 * 1000 * 1000)); // 147.0 MHz -- alternative
const Frequency fivrClockLow((UInt64)(139.5 * 1000 * 1000)); // 139.5 MHz -- default
const Frequency minimumFrequency((UInt64)(1805 * 1000 * 1000)); // 1.805 GHz
const Frequency maximumFrequency((UInt64)(1995 * 1000 * 1000)); // 1.995 GHz

void FivrDeviceList::adjustFrequencies(const RadioDeviceList& radioDevices)
{
    Frequency newFrequency(fivrClockLow);
    if (radioDevices.hasConnectedDevices())
    {
        RadioDevice highestPriorityRadio = radioDevices.getHighestPriorityRadio();
        if (highestPriorityRadio.isWwanDevice() && 
            highestPriorityRadio.frequencySpreadIsWithin(minimumFrequency, maximumFrequency))
        {
            newFrequency = fivrClockHigh;
        }
    }
    setFrequencyForAllFivrDevices(newFrequency);
}

void FivrDeviceList::setFrequencyForAllFivrDevices(const Frequency& newFrequency)
{
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        device->setFrequency(newFrequency);
    }
}

XmlNode* FivrDeviceList::getXml() const
{
    XmlNode* deviceList = XmlNode::createWrapperElement("fivr_device_list");
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        deviceList->addChild(device->getXml());
    }
    return deviceList;
}

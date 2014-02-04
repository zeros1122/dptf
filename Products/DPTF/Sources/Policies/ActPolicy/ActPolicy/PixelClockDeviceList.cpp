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

#include "PixelClockDeviceList.h"

PixelClockDeviceList::PixelClockDeviceList()
{

}

PixelClockDeviceList::~PixelClockDeviceList()
{

}

void PixelClockDeviceList::add(const PixelClockDevice& pixelClockDevice)
{
    m_devices.insert(m_devices.end(), pixelClockDevice);
}

void PixelClockDeviceList::remove(const DomainProxy& domain)
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

void PixelClockDeviceList::adjustFrequencies(const RadioDeviceList& radioDevices)
{
    if (radioDevices.hasConnectedDevices())
    {
        for (auto device = m_devices.begin(); device != m_devices.end(); device++)
        {
            device->adjustFrequencies(radioDevices);
        }
    }
}

XmlNode* PixelClockDeviceList::getXml() const
{
    XmlNode* deviceList = XmlNode::createWrapperElement("pixel_clock_device_list");
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        deviceList->addChild(device->getXml());
    }
    return deviceList;
}
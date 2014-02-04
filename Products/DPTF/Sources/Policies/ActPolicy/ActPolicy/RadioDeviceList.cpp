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

#include "RadioDeviceList.h"

RadioDeviceList::RadioDeviceList()
{

}

RadioDeviceList::~RadioDeviceList()
{

}

bool byStatusThenByType(const RadioDevice& first, const RadioDevice& second)
{
    RadioConnectionStatus::Type firstConnectionStatus = 
        first.getRadioProfile().getSupplementalData().getRadioConnectionStatus();
    RadioConnectionStatus::Type secondConnectionStatus = 
        second.getRadioProfile().getSupplementalData().getRadioConnectionStatus();
    if (firstConnectionStatus == secondConnectionStatus)
    {
        if (first.getDomainType() == second.getDomainType())
        {
            return false;
        }
        else
        {
            if (first.getDomainType() == DomainType::Wireless)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        if (firstConnectionStatus == RadioConnectionStatus::Connected)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void RadioDeviceList::add(const RadioDevice& radioDevice)
{
    m_devices.insert(m_devices.end(), radioDevice);
    m_devices.sort(byStatusThenByType);
}

void RadioDeviceList::remove(const DomainProxy& domain)
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

Bool RadioDeviceList::hasConnectedDevices() const
{
    if (m_devices.size() > 0)
    {
        return m_devices.front().isConnected();
    }
    else
    {
        return false;
    }
}

RadioDevice RadioDeviceList::getHighestPriorityRadio() const
{
    if (hasConnectedDevices())
    {
        return m_devices.front();
    }
    else
    {
        throw dptf_exception("Could not retrieve first radio device from list.  Radio device list is empty.");
    }
}

void RadioDeviceList::updateRadioFrequencyProfiles(UIntN participantIndex)
{
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        if (device->getParticipantIndex() == participantIndex)
        {
            device->updateRadioFrequencyProfile();
        }
    }
}

void RadioDeviceList::updateRadioConnectionStatus(
    UIntN participantIndex, RadioConnectionStatus::Type radioConnectionStatus)
{
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        if (device->getParticipantIndex() == participantIndex)
        {
            device->updateRadioConnectionStatus(radioConnectionStatus);
        }
    }
}

void RadioDeviceList::refreshAll()
{
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        device->refresh();
    }
}

XmlNode* RadioDeviceList::getXml() const
{
    XmlNode* radioDeviceList = XmlNode::createWrapperElement("radio_device_list");
    for (auto device = m_devices.begin(); device != m_devices.end(); device++)
    {
        radioDeviceList->addChild(device->getXml());
    }

    return radioDeviceList;
}

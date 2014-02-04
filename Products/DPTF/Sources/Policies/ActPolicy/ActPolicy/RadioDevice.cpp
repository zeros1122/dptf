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

#include "RadioDevice.h"
#include "StatusFormat.h"
using namespace StatusFormat;

RadioDevice::RadioDevice(DomainProxy& domain)
    : m_domain(&domain), 
    m_radioProfile(0, 0, 0, RfProfileSupplementalData(0, 0, 0, 0, RadioConnectionStatus::NotConnected, 0)),
    m_connectionStatus(RadioConnectionStatus::NotConnected)
{
    throwIfNotRadioDevice();
    m_radioProfile = m_domain->getRadioFrequencyControl().getProfileData();
    m_connectionStatus = m_radioProfile.getSupplementalData().getRadioConnectionStatus();
}

RadioDevice::~RadioDevice()
{

}

UIntN RadioDevice::getParticipantIndex() const
{
    return m_domain->getParticipantIndex();
}

UIntN RadioDevice::getDomainIndex() const
{
    return m_domain->getDomainIndex();
}

DomainType::Type RadioDevice::getDomainType() const
{
    return m_domain->getDomainProperties().getDomainType();
}

RfProfileData RadioDevice::getRadioProfile() const
{
    return m_radioProfile;
}

Bool RadioDevice::isConnected() const
{
    return (m_connectionStatus == RadioConnectionStatus::Connected);
}

void RadioDevice::updateRadioFrequencyProfile()
{
    m_radioProfile = m_domain->getRadioFrequencyControl().getProfileData();
    m_connectionStatus = m_radioProfile.getSupplementalData().getRadioConnectionStatus();
}

void RadioDevice::updateRadioConnectionStatus(RadioConnectionStatus::Type radioConnectionStatus)
{
    m_connectionStatus = radioConnectionStatus;
}

Bool RadioDevice::isRadioDevice(const DomainProxy& domain)
{
    return domain.getRadioFrequencyControl().supportsStatus();
}

void RadioDevice::throwIfNotRadioDevice()
{
    if (isRadioDevice(*m_domain) == false)
    {
        throw dptf_exception("Domain does not support radio frequency status controls.");
    }
}

Bool RadioDevice::isWwanDevice() const
{
    return (getDomainType() == DomainType::WWan);
}

Bool RadioDevice::isWlanDevice() const
{
    return (getDomainType() == DomainType::Wireless);
}

Bool RadioDevice::frequencySpreadIsWithin(const Frequency& low, const Frequency& high) const
{
    return ((getLeftFrequency() >= low) && (getRightFrequency() <= high));
}

void RadioDevice::refresh()
{
    updateRadioFrequencyProfile();
}

Frequency RadioDevice::getLeftFrequency() const
{
    return m_radioProfile.getCenterFrequency() - (getRadioProfile().getLeftFrequencySpread());
}

Frequency RadioDevice::getRightFrequency() const
{
    return m_radioProfile.getCenterFrequency() + (getRadioProfile().getRightFrequencySpread());
}

XmlNode* RadioDevice::getXml() const
{
    XmlNode* device = XmlNode::createWrapperElement("radio_device");
    device->addChild(XmlNode::createDataElement("participant_index", friendlyValue(getParticipantIndex())));
    device->addChild(XmlNode::createDataElement("domain_index", friendlyValue(getDomainIndex())));
    device->addChild(m_radioProfile.getXml());
    device->addChild(XmlNode::createDataElement("last_known_connection_status", 
        RadioConnectionStatus::ToString(m_connectionStatus)));
    return device;
}

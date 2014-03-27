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

#include "FivrDevice.h"
#include "StatusFormat.h"
using namespace StatusFormat;

FivrDevice::FivrDevice(DomainProxy& domain)
    : m_domain(&domain)
{
    throwIfNotFivrDevice();
}

FivrDevice::~FivrDevice()
{

}

UIntN FivrDevice::getParticipantIndex() const
{
    return m_domain->getParticipantIndex();
}

UIntN FivrDevice::getDomainIndex() const
{
    return m_domain->getDomainIndex();
}

Bool FivrDevice::isFivrDevice(const DomainProxy& domain)
{
    return domain.getRadioFrequencyControl().supportsRfControls();
}

void FivrDevice::setFrequency(const Frequency& newFrequency)
{
    m_domain->getRadioFrequencyControl().setOperatingFrequency(newFrequency);
}

void FivrDevice::throwIfNotFivrDevice()
{
    if (isFivrDevice(*m_domain) == false)
    {
        throw dptf_exception("Domain does not support radio frequency controls.");
    }
}

XmlNode* FivrDevice::getXml() const
{
    XmlNode* device = XmlNode::createWrapperElement("fivr_device");
    device->addChild(XmlNode::createDataElement("participant_index", friendlyValue(getParticipantIndex())));
    device->addChild(XmlNode::createDataElement("participant_name", m_domain->getParticipantProperties().getName()));
    device->addChild(XmlNode::createDataElement("domain_index", friendlyValue(getDomainIndex())));
    device->addChild(XmlNode::createDataElement("domain_name", m_domain->getDomainProperties().getName()));
    device->addChild(m_domain->getRadioFrequencyControl().getXml());
    return device;
}

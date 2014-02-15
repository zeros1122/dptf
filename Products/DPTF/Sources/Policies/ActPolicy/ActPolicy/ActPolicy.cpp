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

#include "ActPolicy.h"
using namespace std;

static const Guid MyGuid(0xCE, 0xC4, 0x49, 0x18, 0x3A, 0x24, 0xF3, 0x49, 0xB8, 0xD5, 0xF9, 0x70, 0x02, 0xF3, 0x8E, 0x6A);                         
static const string MyName("ACT Policy");

ActPolicy::ActPolicy()
{
    
}

ActPolicy::~ActPolicy()
{
    
}

void ActPolicy::onCreate()
{
    getPolicyServices().policyEventRegistration->registerEvent(PolicyEvent::DomainRadioConnectionStatusChanged); // Connected / Not Connected status changed
    getPolicyServices().policyEventRegistration->registerEvent(PolicyEvent::DomainRfProfileChanged); // ACT Channel Band Changed
}

void ActPolicy::onDestroy()
{
    getPolicyServices().policyEventRegistration->unregisterEvent(PolicyEvent::DomainRadioConnectionStatusChanged);
    getPolicyServices().policyEventRegistration->unregisterEvent(PolicyEvent::DomainRfProfileChanged);
}

void ActPolicy::onEnable()
{
    m_radioDevices.refreshAll();
    adjustFivrAndPixelClockFrequencies();
}

void ActPolicy::onDisable()
{
    // Nothing to do
}

void ActPolicy::onConnectedStandbyEntry()
{
    // Nothing to do
}

void ActPolicy::onConnectedStandbyExit()
{
    m_radioDevices.refreshAll();
    adjustFivrAndPixelClockFrequencies();
}

bool ActPolicy::autoNotifyPlatformOscOnCreateDestroy() const 
{
    return true;
}

bool ActPolicy::autoNotifyPlatformOscOnConnectedStandbyEntryExit() const 
{
    return false;
}

bool ActPolicy::autoNotifyPlatformOscOnEnableDisable() const 
{
    return true;
}

Guid ActPolicy::getGuid(void) const 
{
    return MyGuid;
}

string ActPolicy::getName(void) const 
{
    return MyName;
}

void ActPolicy::onBindParticipant(UIntN participantIndex) 
{
    getParticipantTracker().remember(participantIndex);
}

void ActPolicy::onUnbindParticipant(UIntN participantIndex)
{
    getParticipantTracker().forget(participantIndex);
}

void ActPolicy::onBindDomain(UIntN participantIndex, UIntN domainIndex)
{
    if (getParticipantTracker().remembers(participantIndex))
    {
        DomainProxy& domain = getParticipantTracker()[participantIndex].bindDomain(domainIndex);
        if (PixelClockDevice::isPixelClockDevice(domain))
        {
            m_pixelClockDevices.add(PixelClockDevice(domain));
            m_pixelClockDevices.adjustFrequencies(m_radioDevices);
        }
        else if (FivrDevice::isFivrDevice(domain))
        {
            m_fivrDevices.add(FivrDevice(domain));
            m_fivrDevices.adjustFrequencies(m_radioDevices);
        }
        else if (RadioDevice::isRadioDevice(domain))
        {
            m_radioDevices.add(RadioDevice(domain));
            adjustFivrAndPixelClockFrequencies();
        }
    }
}

void ActPolicy::onUnbindDomain(UIntN participantIndex, UIntN domainIndex)
{
    if (getParticipantTracker().remembers(participantIndex))
    {
        DomainProxy& domain = getParticipantTracker()[participantIndex][domainIndex];
        if (PixelClockDevice::isPixelClockDevice(domain))
        {
            m_pixelClockDevices.remove(domain);
        }
        else if (FivrDevice::isFivrDevice(domain))
        {
            m_fivrDevices.remove(domain);
        }
        else if (RadioDevice::isRadioDevice(domain))
        {
            m_radioDevices.remove(domain);
            adjustFivrAndPixelClockFrequencies();
        }
        getParticipantTracker()[participantIndex].unbindDomain(domainIndex);
    }
}

void ActPolicy::onDomainRfProfileChanged(UIntN participantIndex)
{
    if (getParticipantTracker().remembers(participantIndex))
    {
        m_radioDevices.updateRadioFrequencyProfiles(participantIndex);
        adjustFivrAndPixelClockFrequencies();
    }
}

void ActPolicy::onDomainRadioConnectionStatusChanged(
    UIntN participantIndex, RadioConnectionStatus::Type radioConnectionStatus)
{
    if (getParticipantTracker().remembers(participantIndex))
    {
        m_radioDevices.updateRadioConnectionStatus(participantIndex, radioConnectionStatus);
        adjustFivrAndPixelClockFrequencies();
    }
}

string ActPolicy::getStatusAsXml(void) const 
{
    XmlNode* format = XmlNode::createComment("format_id=" + MyGuid.toString());
    XmlNode* status = XmlNode::createWrapperElement("act_policy_status");
    status->addChild(m_fivrDevices.getXml());
    status->addChild(m_radioDevices.getXml());
    status->addChild(m_pixelClockDevices.getXml());
    XmlNode* root = XmlNode::createRoot();
    root->addChild(format);
    root->addChild(status);
    string statusString = root->toString();
    delete root;
    return statusString;
}

void ActPolicy::adjustFivrAndPixelClockFrequencies()
{
    m_pixelClockDevices.adjustFrequencies(m_radioDevices);
    m_fivrDevices.adjustFrequencies(m_radioDevices);
}
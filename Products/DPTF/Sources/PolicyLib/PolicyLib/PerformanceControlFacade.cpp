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

#include "PerformanceControlFacade.h"
#include "StatusFormat.h"
using namespace std;
using namespace StatusFormat;

PerformanceControlFacade::PerformanceControlFacade(
    UIntN participantIndex,
    UIntN domainIndex,
    const DomainProperties& domainProperties,
    const PolicyServicesInterfaceContainer& policyServices)
    : m_participantIndex(participantIndex),
    m_domainIndex(domainIndex),
    m_domainProperties(domainProperties),
    m_policyServices(policyServices),
    m_performanceControlSetProperty(participantIndex, domainIndex, domainProperties, policyServices),
    m_performanceControlStatusProperty(participantIndex, domainIndex, domainProperties, policyServices),
    m_performanceControlCapabilitiesProperty(participantIndex, domainIndex, domainProperties, policyServices),
    m_controlsHaveBeenInitialized(false),
    m_lastIssuedPerformanceControlIndex(0)
{
}

PerformanceControlFacade::~PerformanceControlFacade()
{
}

Bool PerformanceControlFacade::supportsPerformanceControls()
{
    return m_domainProperties.implementsPerformanceControlInterface();
}

const PerformanceControlStatus& PerformanceControlFacade::getStatus()
{
    initializeControlsIfNeeded();
    return m_performanceControlStatusProperty.getStatus();
}

void PerformanceControlFacade::setControl(UIntN performanceControlIndex)
{
    if (supportsPerformanceControls())
    {
        m_lastIssuedPerformanceControlIndex = performanceControlIndex;
        const PerformanceControlDynamicCaps& caps = getDynamicCapabilities();
        m_isLimited = m_lastIssuedPerformanceControlIndex > caps.getCurrentUpperLimitIndex();
        m_policyServices.domainPerformanceControl->setPerformanceControl(
            m_participantIndex, m_domainIndex, performanceControlIndex);
        m_performanceControlStatusProperty.invalidate();
    }
    else
    {
        throw dptf_exception("Domain does not support the performance control interface.");
    }
}

const PerformanceControlSet& PerformanceControlFacade::getControls()
{
    return m_performanceControlSetProperty.getPerformanceControlSet();
}

void PerformanceControlFacade::refreshControls()
{
    m_performanceControlSetProperty.refresh();
    m_performanceControlStatusProperty.invalidate();
}

void PerformanceControlFacade::refreshCapabilities()
{
    m_performanceControlCapabilitiesProperty.refresh();
}

void PerformanceControlFacade::invalidateStatus()
{
    m_performanceControlStatusProperty.invalidate();
}

const PerformanceControlDynamicCaps& PerformanceControlFacade::getDynamicCapabilities()
{
    return m_performanceControlCapabilitiesProperty.getDynamicCaps();
}

void PerformanceControlFacade::initializeControlsIfNeeded()
{
    if (supportsPerformanceControls())
    {
        const PerformanceControlDynamicCaps& caps = getDynamicCapabilities();
        UIntN upperLimitIndex = caps.getCurrentUpperLimitIndex();
        UIntN lowerLimitIndex = caps.getCurrentLowerLimitIndex();
        if (m_controlsHaveBeenInitialized == false)
        {
            setControl(upperLimitIndex);
            m_controlsHaveBeenInitialized = true;
        }
        else
        {
            if (m_isLimited)
            {
                if (m_lastIssuedPerformanceControlIndex < upperLimitIndex)
                {
                    setControl(upperLimitIndex);
                }

                if (m_lastIssuedPerformanceControlIndex > lowerLimitIndex)
                {
                    setControl(lowerLimitIndex);
                }
            }
            else
            {
                if (m_lastIssuedPerformanceControlIndex != upperLimitIndex)
                {
                    setControl(upperLimitIndex);
                }
            }
        }
    }

}
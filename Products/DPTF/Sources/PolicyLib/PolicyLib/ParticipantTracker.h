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
#include "ParticipantProxy.h"
#include "PolicyServicesInterfaceContainer.h"
#include "TimeInterface.h"
#include <vector>
#include <memory>

// keeps track of participants as they come and go on the system.
class dptf_export ParticipantTracker
{
public:

    ParticipantTracker();
    ~ParticipantTracker();

    // participant tracking
    void remember(UIntN participantIndex);
    bool remembers(UIntN participantIndex);
    void forget(UIntN participantIndex);
    ParticipantProxy& operator[](UIntN participantIndex);
    std::vector<UIntN> getAllTrackedIndexes() const;

    // services
    void setPolicyServices(PolicyServicesInterfaceContainer policyServices);
    void setTimeServiceObject(std::shared_ptr<TimeInterface> time);

    // status
    XmlNode* getXmlForCriticalTripPoints();
    XmlNode* getXmlForActiveTripPoints();
    XmlNode* getXmlForPassiveTripPoints();
    XmlNode* getXmlForActiveCoolingControls();
    XmlNode* getXmlForPassiveControlKnobs();
    XmlNode* getXmlForTripPointStatistics();
    XmlNode* getXmlForConfigTdpLevels();

protected:

    std::map<UIntN, ParticipantProxy> m_trackedParticipants;
    PolicyServicesInterfaceContainer m_policyServices;
    std::shared_ptr<TimeInterface> m_time;
};
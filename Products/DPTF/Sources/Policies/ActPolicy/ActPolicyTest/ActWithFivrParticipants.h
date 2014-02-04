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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PolicyServicesMocks.h"
#include "PolicyServicesObjects.h"

#include "ActPolicy.h"
#include "ActWithNoParticipants.h"


class ActPolicyWithFivrParticipants : public ActPolicyWithNoParticipants
{
     
public:

    PolicyServicesObjects policyServicesObjects;
    MockParticipantProperties participantProperties;
    MockDomainRfProfileControlInterface rfControl;
    MockDomainRfProfileStatusInterface rfStatus;

    ActPolicyWithFivrParticipants::ActPolicyWithFivrParticipants(void)
        : ActPolicyWithNoParticipants()
    {

    }

    virtual void SetUp() override {
        ActPolicyWithNoParticipants::SetUp();
        policyServices.platformNotification = &platformNotification;
        policyServices.platformConfigurationData = &platformConfiguration;
        policyServices.policyEventRegistration = &eventRegistration;
        policyServices.participantProperties = &participantProperties;
        policyServices.domainRfProfileControl = &rfControl;
        policyServices.domainRfProfileStatus = &rfStatus;
    }

    virtual void TearDown() override {
        ActPolicyWithNoParticipants::TearDown();
    }

    DomainPropertiesSet createDomainPropertiesSet(
        DomainType::Type domainType, bool pixelClockControl, bool pixelClockStatus, bool rfControl, bool rfStatus)
    {
        DomainFunctionalityVersions domainFunctionalityVersions;
        domainFunctionalityVersions.pixelClockControlVersion = pixelClockControl ? 1 : 0;
        domainFunctionalityVersions.pixelClockStatusVersion = pixelClockStatus ? 1 : 0;
        domainFunctionalityVersions.rfProfileControlVersion = rfControl ? 1 : 0;
        domainFunctionalityVersions.rfProfileStatusVersion = rfStatus ? 1 : 0;
        DomainProperties domainProperties(
            Guid(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15),
            0,
            true,
            domainType,  
            std::string("Domain 0"),
            std::string("This is a domain description."),
            domainFunctionalityVersions);
        std::vector<DomainProperties> allProperties;
        allProperties.push_back(domainProperties);
        DomainPropertiesSet domainPropertiesSet(allProperties);
        return domainPropertiesSet;
    }


};


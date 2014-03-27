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

#include "ActPolicy.h"
#include "PolicyServicesMocks.h"


class ActPolicyWithNoParticipants : public ::testing::Test
{
     
public:

    PolicyServicesInterfaceContainer policyServices;
    ActPolicy actPolicy;
    PolicyBase* policy;
    MockPlatformNotification platformNotification;
    MockPlatformConfigurationDataInterface platformConfiguration;
    MockEventRegistration eventRegistration;
    MockMessageLoggingInterface messages;

    ActPolicyWithNoParticipants::ActPolicyWithNoParticipants(void)
    {

    }

    virtual void SetUp() override {
        ::testing::FLAGS_gmock_verbose = "error"; // needed to suppress erroneous warnings
        policy = (PolicyBase*)&actPolicy;
        policyServices.platformNotification = &platformNotification;
        policyServices.platformConfigurationData = &platformConfiguration;
        policyServices.policyEventRegistration = &eventRegistration;
        policyServices.messageLogging = &messages;
    }

    virtual void TearDown() override
    {
        ::testing::FLAGS_gmock_verbose = "warning"; // re-enable warnings
    }
};


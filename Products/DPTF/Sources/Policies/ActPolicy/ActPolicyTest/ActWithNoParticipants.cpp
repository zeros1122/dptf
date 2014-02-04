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

#include "ActWithNoParticipants.h"

static const Guid policyGuid(0x99, 0x8E, 0xFD, 0x5B, 0xE2, 0xBA, 0x47, 0x86, 0xA2, 0x99, 0x26, 0xAB, 0x5F, 0x70, 0xE5, 0x2B);                         
static const string policyName("ACT Policy");

TEST_F(ActPolicyWithNoParticipants, CanBeCreated)
{
    ASSERT_STREQ(policyName.c_str(), policy->getName().c_str());
    ASSERT_EQ(policyGuid, policy->getGuid());
    policy->create(true, policyServices, 0);
    ASSERT_STREQ(policyName.c_str(), policy->getName().c_str());
    ASSERT_EQ(policyGuid, policy->getGuid());
}

TEST_F(ActPolicyWithNoParticipants, CanBeDestroyed)
{
    policy->create(true, policyServices, 0);
    policy->destroy();
    ASSERT_STREQ(policyName.c_str(), policy->getName().c_str());
    ASSERT_EQ(policyGuid, policy->getGuid());
}

TEST_F(ActPolicyWithNoParticipants, TakesControlOfOscOnCreate)
{
    EXPECT_CALL(platformNotification, notifyPlatformPolicyTakeControl())
        .Times(1);
    ASSERT_TRUE(policy->autoNotifyPlatformOscOnCreateDestroy());
    policy->create(true, policyServices, 0);
}

TEST_F(ActPolicyWithNoParticipants, ReleasesControlOfOscOnDestroy)
{
    EXPECT_CALL(platformNotification, notifyPlatformPolicyReleaseControl())
        .Times(1);
    ASSERT_TRUE(policy->autoNotifyPlatformOscOnCreateDestroy());
    policy->create(true, policyServices, 0);
    policy->destroy();
}

TEST_F(ActPolicyWithNoParticipants, MaintainsControlOfOscDuringCs)
{
    policy->create(true, policyServices, 0);

    ASSERT_FALSE(policy->autoNotifyPlatformOscOnConnectedStandbyEntryExit());
    EXPECT_CALL(platformNotification, notifyPlatformPolicyTakeControl())
        .Times(0);
    EXPECT_CALL(platformNotification, notifyPlatformPolicyReleaseControl())
        .Times(0);
    policy->connectedStandbyEntry();
    policy->connectedStandbyExit();
}

TEST_F(ActPolicyWithNoParticipants, RegistersForRequiredEventsOnCreate)
{
    EXPECT_CALL(eventRegistration, registerEvent(PolicyEvent::DomainRadioConnectionStatusChanged))
        .Times(1);
    EXPECT_CALL(eventRegistration, registerEvent(PolicyEvent::DomainRfProfileChanged))
        .Times(1);
    policy->create(true, policyServices, 0);
}

TEST_F(ActPolicyWithNoParticipants, UnregistersForRequiredEventsOnDestroy)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(eventRegistration, unregisterEvent(PolicyEvent::DomainRadioConnectionStatusChanged))
        .Times(1);
    EXPECT_CALL(eventRegistration, unregisterEvent(PolicyEvent::DomainRfProfileChanged))
        .Times(1);
    policy->destroy();
}

TEST_F(ActPolicyWithNoParticipants, DoesNothingWhenDisabled)
{
    policy->create(true, policyServices, 0);
    policy->disable();
}

const char* ExpectedStatus = "\
<!-- format_id=99-8E-FD-5B-E2-BA-47-86-A2-99-26-AB-5F-70-E5-2B -->\n\
<act_policy_status>\n\
\t<fivr_device_list/>\n\
\t<radio_device_list/>\n\
\t<pixel_clock_device_list/>\n\
</act_policy_status>\n\
\n";

TEST_F(ActPolicyWithNoParticipants, ShowsExpectedStatus)
{
    policy->create(true, policyServices, 0);
    std::string status = policy->getStatusAsXml();
    ASSERT_STREQ(status.c_str(), ExpectedStatus);
}
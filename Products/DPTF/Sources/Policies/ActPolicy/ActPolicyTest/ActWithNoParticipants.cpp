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

static const Guid policyGuid(0x49, 0x18, 0xCE, 0xC4, 0x3A, 0x24, 0xF3, 0x49, 0xB8, 0xD5, 0xF9, 0x70, 0x02, 0xF3, 0x8E, 0x6A);                         
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
<!-- format_id=49-18-CE-C4-3A-24-F3-49-B8-D5-F9-70-02-F3-8E-6A -->\n\
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
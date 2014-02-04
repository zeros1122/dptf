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

#include "ActWithFivrParticipants.h"
using ::testing::Return;
using ::testing::Property;
using ::testing::_;

const Frequency fivrClockLow(1395 * 1000 * 1000); // 139.5 MHz
const Frequency fivrClockHigh(1470 * 1000 * 1000); // 147.0 MHz

TEST_F(ActPolicyWithFivrParticipants, CanBindParticipant)
{
    policy->create(true, policyServices, 0);
    policy->bindParticipant(1);
}

TEST_F(ActPolicyWithFivrParticipants, CanUnbindParticipant)
{
    policy->create(true, policyServices, 0);
    policy->bindParticipant(1);
    policy->unbindParticipant(1);
}

TEST_F(ActPolicyWithFivrParticipants, RespondsToEnable)
{
    policy->create(true, policyServices, 0);
    policy->bindParticipant(1);
    policy->disable();
    policy->enable();
}

TEST_F(ActPolicyWithFivrParticipants, CanAddFivrDevice)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency centerFrequency(1730 * 1000 * 1000);
    const Frequency spread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData supplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(centerFrequency, spread / 2, spread / 2, supplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);
}

TEST_F(ActPolicyWithFivrParticipants, CanAddFivrDeviceAndRadioDeviceWithinFreqSpread)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockHigh))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);
}

TEST_F(ActPolicyWithFivrParticipants, CanAddFivrDeviceAndRadioDeviceOutsideofFreqSpread)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000); // 173.0 MHz
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(2100 * 1000 * 1000); // 210.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);
}

TEST_F(ActPolicyWithFivrParticipants, CanAddFivrDeviceAndTwoRadioDeviceWithinFreqSpreadWlanFirst)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wlanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(3))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(3)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(3))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(3, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(3);
    policy->bindDomain(3, 0);
}

TEST_F(ActPolicyWithFivrParticipants, CanAddFivrDeviceAndTwoRadioDeviceWithinFreqSpreadWWanFirst)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(3))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(3)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(3))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(3, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockHigh))
        .Times(1);
    policy->bindParticipant(3);
    policy->bindDomain(3, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wlanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);
}

TEST_F(ActPolicyWithFivrParticipants, ReactsToRadioConnectionStatusDisconnected)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockHigh))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);

    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->domainRadioConnectionStatusChanged(2, RadioConnectionStatus::NotConnected);
}

TEST_F(ActPolicyWithFivrParticipants, ReactsToRadioConnectionStatusConnected)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::NotConnected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);

    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockHigh))
        .Times(1);
    policy->domainRadioConnectionStatusChanged(2, RadioConnectionStatus::Connected);
}

const char* ExpectedStatusValuesChanged = "\
<!-- format_id=99-8E-FD-5B-E2-BA-47-86-A2-99-26-AB-5F-70-E5-2B -->\n\
<act_policy_status>\n\
\t<fivr_device_list>\n\
\t\t<fivr_device>\n\
\t\t\t<participant_index>1</participant_index>\n\
\t\t\t<domain_index>0</domain_index>\n\
\t\t\t<radio_frequency_control>\n\
\t\t\t\t<supports_status_controls>true</supports_status_controls>\n\
\t\t\t\t<supports_set_controls>true</supports_set_controls>\n\
\t\t\t\t<radio_frequency_profile_data>\n\
\t\t\t\t\t<center_frequency>1730000000</center_frequency>\n\
\t\t\t\t\t<left_frequency_spread>15000000</left_frequency_spread>\n\
\t\t\t\t\t<right_frequency_spread>15000000</right_frequency_spread>\n\
\t\t\t\t\t<radio_frequency_supplemental_data>\n\
\t\t\t\t\t\t<channel_number>0</channel_number>\n\
\t\t\t\t\t\t<noise_power>0</noise_power>\n\
\t\t\t\t\t\t<signal_to_noise_ratio>0</signal_to_noise_ratio>\n\
\t\t\t\t\t\t<rssi>0</rssi>\n\
\t\t\t\t\t\t<radio_connection_status>Radio Connected</radio_connection_status>\n\
\t\t\t\t\t\t<bit_error>0</bit_error>\n\
\t\t\t\t\t</radio_frequency_supplemental_data>\n\
\t\t\t\t</radio_frequency_profile_data>\n\
\t\t\t</radio_frequency_control>\n\
\t\t</fivr_device>\n\
\t</fivr_device_list>\n\
\t<radio_device_list>\n\
\t\t<radio_device>\n\
\t\t\t<participant_index>2</participant_index>\n\
\t\t\t<domain_index>0</domain_index>\n\
\t\t\t<radio_frequency_profile_data>\n\
\t\t\t\t<center_frequency>1900000000</center_frequency>\n\
\t\t\t\t<left_frequency_spread>25000000</left_frequency_spread>\n\
\t\t\t\t<right_frequency_spread>25000000</right_frequency_spread>\n\
\t\t\t\t<radio_frequency_supplemental_data>\n\
\t\t\t\t\t<channel_number>0</channel_number>\n\
\t\t\t\t\t<noise_power>0</noise_power>\n\
\t\t\t\t\t<signal_to_noise_ratio>0</signal_to_noise_ratio>\n\
\t\t\t\t\t<rssi>0</rssi>\n\
\t\t\t\t\t<radio_connection_status>Radio Connected</radio_connection_status>\n\
\t\t\t\t\t<bit_error>0</bit_error>\n\
\t\t\t\t</radio_frequency_supplemental_data>\n\
\t\t\t</radio_frequency_profile_data>\n\
\t\t\t<last_known_connection_status>Radio Connected</last_known_connection_status>\n\
\t\t</radio_device>\n\
\t\t<radio_device>\n\
\t\t\t<participant_index>3</participant_index>\n\
\t\t\t<domain_index>0</domain_index>\n\
\t\t\t<radio_frequency_profile_data>\n\
\t\t\t\t<center_frequency>1900000000</center_frequency>\n\
\t\t\t\t<left_frequency_spread>25000000</left_frequency_spread>\n\
\t\t\t\t<right_frequency_spread>25000000</right_frequency_spread>\n\
\t\t\t\t<radio_frequency_supplemental_data>\n\
\t\t\t\t\t<channel_number>0</channel_number>\n\
\t\t\t\t\t<noise_power>0</noise_power>\n\
\t\t\t\t\t<signal_to_noise_ratio>0</signal_to_noise_ratio>\n\
\t\t\t\t\t<rssi>0</rssi>\n\
\t\t\t\t\t<radio_connection_status>Radio Connected</radio_connection_status>\n\
\t\t\t\t\t<bit_error>0</bit_error>\n\
\t\t\t\t</radio_frequency_supplemental_data>\n\
\t\t\t</radio_frequency_profile_data>\n\
\t\t\t<last_known_connection_status>Radio Connected</last_known_connection_status>\n\
\t\t</radio_device>\n\
\t</radio_device_list>\n\
\t<pixel_clock_device_list/>\n\
</act_policy_status>\n\
\n";

TEST_F(ActPolicyWithFivrParticipants, ShowsExpectedStatus)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Processor, false, false, true, true)));
    const Frequency fivrCenterFrequency(1730 * 1000 * 1000);
    const Frequency fivrSpread(30 * 1000 * 1000); // 30.0 MHz
    RfProfileSupplementalData fivrSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData fivrProfile(fivrCenterFrequency, fivrSpread / 2, fivrSpread / 2, 
        fivrSupplementalData);
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(3))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(3)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(3))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::WWan, false, false, false, true)));
    const Frequency wwanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wwanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wwanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wwanProfile(wwanCenterFrequency, wwanSpread / 2, wwanSpread / 2, 
        wwanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(3, 0))
        .WillOnce(Return(wwanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockHigh))
        .Times(1);
    policy->bindParticipant(3);
    policy->bindDomain(3, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency(1900 * 1000 * 1000); // 190.0 MHz
    const Frequency wlanSpread(50 * 1000 * 1000); // 50.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    EXPECT_CALL(rfControl, setRfProfileCenterFrequency(1, 0, fivrClockLow))
        .Times(1);
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);

    EXPECT_CALL(rfStatus, getRfProfileData(1, 0))
        .WillOnce(Return(fivrProfile));
    std::string status = policy->getStatusAsXml();
    ASSERT_STREQ(status.c_str(), ExpectedStatusValuesChanged);
}
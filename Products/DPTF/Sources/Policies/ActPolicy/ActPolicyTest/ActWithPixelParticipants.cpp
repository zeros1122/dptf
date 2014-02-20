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

#include "ActWithPixelParticipants.h"
using ::testing::Return;
using ::testing::Property;
using ::testing::_;

const Frequency fivrClockLow(1395 * 1000 * 1000); // 139.5 MHz
const Frequency fivrClockHigh(1470 * 1000 * 1000); // 147.0 MHz

TEST_F(ActPolicyWithPixelParticipants, CanAddPixelClockDevice)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Display, true, true, false, false)));
    PixelClockCapabilities pixelClockCapabilities(PixelClockPanelType::Edp, 2, true, true, 
        PixelClockChannelType::Single, true, PixelClockSpreadType::Up, .02);
    std::vector<PixelClockData> pixelClocks;
    pixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet pixelClockDataSet(pixelClocks);
    EXPECT_CALL(pixelClockStatus, getPixelClockCapabilities(1, 0))
        .WillOnce(Return(pixelClockCapabilities));
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(pixelClockDataSet));
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);
}

TEST_F(ActPolicyWithPixelParticipants, CanSetPixelClockDeviceFrequencySameAsCurrent)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Display, true, true, false, false)));
    PixelClockCapabilities pixelClockCapabilities(PixelClockPanelType::Edp, 2, true, true, 
        PixelClockChannelType::Single, true, PixelClockSpreadType::Up, .02);
    std::vector<PixelClockData> pixelClocks;
    pixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet pixelClockDataSet(pixelClocks);
    EXPECT_CALL(pixelClockStatus, getPixelClockCapabilities(1, 0))
        .WillOnce(Return(pixelClockCapabilities));
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(pixelClockDataSet));
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    std::vector<PixelClockData> newPixelClocks;
    newPixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet newPixelClockDataSet(newPixelClocks);
    EXPECT_CALL(pixelClockControl, setPixelClockControl(1, 0, newPixelClockDataSet))
        .Times(1);
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(newPixelClockDataSet));
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);
}

TEST_F(ActPolicyWithPixelParticipants, CanSetPixelClockDeviceFrequencyToDifferentValue)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Display, true, true, false, false)));
    PixelClockCapabilities pixelClockCapabilities(PixelClockPanelType::Edp, 2, true, true, 
        PixelClockChannelType::Single, true, PixelClockSpreadType::Up, .02);
    std::vector<PixelClockData> pixelClocks;
    pixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet pixelClockDataSet(pixelClocks);
    EXPECT_CALL(pixelClockStatus, getPixelClockCapabilities(1, 0))
        .WillOnce(Return(pixelClockCapabilities));
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(pixelClockDataSet));
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency((UInt64)2412 * (UInt64)1000 * (UInt64)1000); // 2.412 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    std::vector<PixelClockData> newPixelClocks;
    newPixelClocks.push_back(PixelClockData(138780000, 138780000, 139380000));
    PixelClockDataSet newPixelClockDataSet(newPixelClocks);
    EXPECT_CALL(pixelClockControl, setPixelClockControl(1, 0, newPixelClockDataSet))
        .Times(1);
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(newPixelClockDataSet));
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);
}

const char* ExpectedStatusPixelClocks = "\
<!-- format_id=CE-C4-49-18-3A-24-F3-49-B8-D5-F9-70-02-F3-8E-6A -->\n\
<act_policy_status>\n\
\t<fivr_device_list/>\n\
\t<radio_device_list>\n\
\t\t<radio_device>\n\
\t\t\t<participant_index>2</participant_index>\n\
\t\t\t<domain_index>0</domain_index>\n\
\t\t\t<radio_frequency_profile_data>\n\
\t\t\t\t<center_frequency>2407000000</center_frequency>\n\
\t\t\t\t<left_frequency_spread>11000000</left_frequency_spread>\n\
\t\t\t\t<right_frequency_spread>11000000</right_frequency_spread>\n\
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
\t<pixel_clock_device_list>\n\
\t\t<pixel_clock_device>\n\
\t\t\t<participant_index>1</participant_index>\n\
\t\t\t<domain_index>0</domain_index>\n\
\t\t\t<pixel_clock_capabilities>\n\
\t\t\t\t<panel_type>Edp</panel_type>\n\
\t\t\t\t<deviation>2</deviation>\n\
\t\t\t\t<upward_deviation>true</upward_deviation>\n\
\t\t\t\t<downward_deviation>true</downward_deviation>\n\
\t\t\t\t<channel_type>Single</channel_type>\n\
\t\t\t\t<ssc_enabled>true</ssc_enabled>\n\
\t\t\t\t<spread_type>Up</spread_type>\n\
\t\t\t\t<spread_percentage>2.000000</spread_percentage>\n\
\t\t\t</pixel_clock_capabilities>\n\
\t\t\t<pixel_clock_data_set>\n\
\t\t\t\t<pixel_clock_data>\n\
\t\t\t\t\t<panel_input_frequency>138780000</panel_input_frequency>\n\
\t\t\t\t\t<ssc_enabled_nudge_frequency>138780000</ssc_enabled_nudge_frequency>\n\
\t\t\t\t\t<ssc_disabled_nudge_frequency>138780000</ssc_disabled_nudge_frequency>\n\
\t\t\t\t</pixel_clock_data>\n\
\t\t\t</pixel_clock_data_set>\n\
\t\t</pixel_clock_device>\n\
\t</pixel_clock_device_list>\n\
</act_policy_status>\n\
\n";

TEST_F(ActPolicyWithPixelParticipants, ShowsExpectedStatus)
{
    policy->create(true, policyServices, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(1))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(1)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(1))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Display, true, true, false, false)));
    PixelClockCapabilities pixelClockCapabilities(PixelClockPanelType::Edp, 2, true, true, 
        PixelClockChannelType::Single, true, PixelClockSpreadType::Up, .02);
    std::vector<PixelClockData> pixelClocks;
    pixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet pixelClockDataSet(pixelClocks);
    EXPECT_CALL(pixelClockStatus, getPixelClockCapabilities(1, 0))
        .WillOnce(Return(pixelClockCapabilities));
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(pixelClockDataSet));
    policy->bindParticipant(1);
    policy->bindDomain(1, 0);

    EXPECT_CALL(participantProperties, getParticipantProperties(2))
        .WillOnce(Return(policyServicesObjects.participantProperties.at(2)));
    EXPECT_CALL(participantProperties, getDomainPropertiesSet(2))
        .WillOnce(Return(createDomainPropertiesSet(DomainType::Wireless, false, false, false, true)));
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    RfProfileSupplementalData wlanSupplementalData(0, 0, 0, 0, RadioConnectionStatus::Connected, 0);
    RfProfileData wlanProfile(wlanCenterFrequency, wlanSpread / 2, wlanSpread / 2, 
        wlanSupplementalData);
    EXPECT_CALL(rfStatus, getRfProfileData(2, 0))
        .WillOnce(Return(wlanProfile));
    std::vector<PixelClockData> newPixelClocks;
    newPixelClocks.push_back(PixelClockData(138780000, 138780000, 138780000));
    PixelClockDataSet newPixelClockDataSet(newPixelClocks);
    EXPECT_CALL(pixelClockControl, setPixelClockControl(1, 0, newPixelClockDataSet))
        .Times(1);
    EXPECT_CALL(pixelClockStatus, getPixelClockDataSet(1, 0))
        .WillOnce(Return(newPixelClockDataSet));
    policy->bindParticipant(2);
    policy->bindDomain(2, 0);

    std::string status = policy->getStatusAsXml();
    ASSERT_STREQ(status.c_str(), ExpectedStatusPixelClocks);
}
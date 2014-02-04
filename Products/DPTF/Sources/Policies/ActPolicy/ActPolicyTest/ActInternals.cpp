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

#include "ActInternals.h"
#include "CandidateFrequencyGenerator.h"
#include "../ActPolicy/GpsFilter.h"
#include "../ActPolicy/PixelClockFrequencyCostTable.h"
#include "../ActPolicy/RadioDevice.h"

const double DoubleFloatPrecision = 0.000001;

TEST_F(ActPolicyInternals, GeneratesSscDisabledFrequencyList_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscDisabledCandidates(
        capabilities_01, dataSet_01);
    ASSERT_EQ(1, table.size());
    ASSERT_EQ(11, table[0].size());
    ASSERT_EQ(Frequency(138780000), table[0][0]);
    ASSERT_EQ(Frequency(139380000), table[0][1]);
    ASSERT_EQ(Frequency(138180000), table[0][2]);
    ASSERT_EQ(Frequency(139980000), table[0][3]);
    ASSERT_EQ(Frequency(137580000), table[0][4]);
    ASSERT_EQ(Frequency(140580000), table[0][5]);
    ASSERT_EQ(Frequency(136980000), table[0][6]);
    ASSERT_EQ(Frequency(141180000), table[0][7]);
    ASSERT_EQ(Frequency(136380000), table[0][8]);
    ASSERT_EQ(Frequency(141780000), table[0][9]);
    ASSERT_EQ(Frequency(135780000), table[0][10]);
}

TEST_F(ActPolicyInternals, GeneratesSscEnabledFrequencyList_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscEnabledCandidates(
        capabilities_01, dataSet_01);
    ASSERT_EQ(1, table.size());
    ASSERT_EQ(11, table[0].size());
    ASSERT_EQ(Frequency(138780000), table[0][0]);
    ASSERT_EQ(Frequency(139380000), table[0][1]);
    ASSERT_EQ(Frequency(138180000), table[0][2]);
    ASSERT_EQ(Frequency(139980000), table[0][3]);
    ASSERT_EQ(Frequency(137580000), table[0][4]);
    ASSERT_EQ(Frequency(140580000), table[0][5]);
    ASSERT_EQ(Frequency(136980000), table[0][6]);
    ASSERT_EQ(Frequency(141180000), table[0][7]);
    ASSERT_EQ(Frequency(136380000), table[0][8]);
    ASSERT_EQ(Frequency(141780000), table[0][9]);
    ASSERT_EQ(Frequency(135780000), table[0][10]);
}

TEST_F(ActPolicyInternals, GeneratesSscDisabledFrequencyListGpsFiltered_01)
{
    PixelClockFrequencyTable table = GpsFilter().filter(CandidateFrequencyGenerator::generateSscDisabledCandidates(
        capabilities_01, dataSet_01));
    ASSERT_EQ(1, table.size());
    ASSERT_EQ(8, table[0].size());
    ASSERT_EQ(Frequency(138780000), table[0][0]);
    ASSERT_EQ(Frequency(139380000), table[0][1]);
    ASSERT_EQ(Frequency(138180000), table[0][2]);
    ASSERT_EQ(Frequency(139980000), table[0][3]);
    ASSERT_EQ(Frequency(140580000), table[0][4]);
    ASSERT_EQ(Frequency(141180000), table[0][5]);
    ASSERT_EQ(Frequency(141780000), table[0][6]);
    ASSERT_EQ(Frequency(135780000), table[0][7]);
}

TEST_F(ActPolicyInternals, GeneratesSscEnabledFrequencyListGpsFiltered_01)
{
    PixelClockFrequencyTable table = GpsFilter().filter(CandidateFrequencyGenerator::generateSscEnabledCandidates(
        capabilities_01, dataSet_01));
    ASSERT_EQ(1, table.size());
    ASSERT_EQ(7, table[0].size());
    ASSERT_EQ(Frequency(138780000), table[0][0]);
    ASSERT_EQ(Frequency(139380000), table[0][1]);
    ASSERT_EQ(Frequency(138180000), table[0][2]);
    ASSERT_EQ(Frequency(139980000), table[0][3]);
    ASSERT_EQ(Frequency(137580000), table[0][4]);
    ASSERT_EQ(Frequency(140580000), table[0][5]);
    ASSERT_EQ(Frequency(141180000), table[0][6]);
}

TEST_F(ActPolicyInternals, GeneratesSscDisabledFrequencyListCost_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscDisabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][0].cost);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.489662, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][3].cost);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_DOUBLE_EQ(1, costTable[0][4].cost);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_NEAR(0.071802, costTable[0][5].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.579956, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.781937, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][8].cost);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_DOUBLE_EQ(1, costTable[0][9].cost);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][10].cost);
}

TEST_F(ActPolicyInternals, GeneratesSscDisabledFrequencyListCost_02)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscDisabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2412 * (UInt64)1000 * (UInt64)1000); // 2.412 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_NEAR(0.112267, costTable[0][0].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.834278, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][3].cost);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_NEAR(0.960338, costTable[0][4].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][5].cost);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.232321, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.434724, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][8].cost);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_DOUBLE_EQ(1, costTable[0][9].cost);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][10].cost);
}

TEST_F(ActPolicyInternals, GeneratesSscEnabledFrequencyListCost_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscEnabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][0].cost);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.039815, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_NEAR(0.330316, costTable[0][3].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_NEAR(0.436254, costTable[0][4].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_NEAR(0.720983, costTable[0][5].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.834324, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.724983, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_NEAR(0.598398, costTable[0][8].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_NEAR(0.337388, costTable[0][9].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_NEAR(0.195105, costTable[0][10].cost, DoubleFloatPrecision);
}

TEST_F(ActPolicyInternals, GeneratesSscEnabledFrequencyListCost_02)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscEnabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2412 * (UInt64)1000 * (UInt64)1000); // 2.412 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][0].cost);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.227788, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_NEAR(0.139303, costTable[0][3].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_NEAR(0.625046, costTable[0][4].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_NEAR(0.530785, costTable[0][5].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.808539, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.833313, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_NEAR(0.407944, costTable[0][8].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_NEAR(0.525976, costTable[0][9].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_NEAR(0.003810, costTable[0][10].cost, DoubleFloatPrecision);
}

TEST_F(ActPolicyInternals, GeneratesSscDisabledFrequencyListCostGpsEnabled_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscDisabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][0].cost);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.489662, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][3].cost);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_DOUBLE_EQ(1, costTable[0][4].cost);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_NEAR(0.071802, costTable[0][5].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.579956, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.781937, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][8].cost);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_DOUBLE_EQ(1, costTable[0][9].cost);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][10].cost);
}

TEST_F(ActPolicyInternals, GeneratesSscEnabledFrequencyListCostGpsEnabled_01)
{
    PixelClockFrequencyTable table = CandidateFrequencyGenerator::generateSscEnabledCandidates(
        capabilities_01, dataSet_01);
    const Frequency wlanCenterFrequency((UInt64)2407 * (UInt64)1000 * (UInt64)1000); // 2.407 GHz
    const Frequency wlanSpread((UInt64)22 * (UInt64)1000 * (UInt64)1000); // 22.0 MHz
    const Frequency wlanLeftFrequency = wlanCenterFrequency - Frequency(wlanSpread / 2);
    const Frequency wlanRightFrequency = wlanCenterFrequency + Frequency(wlanSpread / 2);
    PixelClockFrequencyCostTable costTable(table, wlanLeftFrequency, wlanRightFrequency);

    ASSERT_EQ(1, costTable.size());
    ASSERT_EQ(11, costTable[0].size());
    ASSERT_EQ(Frequency(138780000), costTable[0][0].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][0].cost);
    ASSERT_EQ(Frequency(139380000), costTable[0][1].frequency);
    ASSERT_DOUBLE_EQ(0, costTable[0][1].cost);
    ASSERT_EQ(Frequency(138180000), costTable[0][2].frequency);
    ASSERT_NEAR(0.039815, costTable[0][2].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(139980000), costTable[0][3].frequency);
    ASSERT_NEAR(0.330316, costTable[0][3].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(137580000), costTable[0][4].frequency);
    ASSERT_NEAR(0.436254, costTable[0][4].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(140580000), costTable[0][5].frequency);
    ASSERT_NEAR(0.720983, costTable[0][5].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136980000), costTable[0][6].frequency);
    ASSERT_NEAR(0.834324, costTable[0][6].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141180000), costTable[0][7].frequency);
    ASSERT_NEAR(0.724983, costTable[0][7].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(136380000), costTable[0][8].frequency);
    ASSERT_NEAR(0.598398, costTable[0][8].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(141780000), costTable[0][9].frequency);
    ASSERT_NEAR(0.337388, costTable[0][9].cost, DoubleFloatPrecision);
    ASSERT_EQ(Frequency(135780000), costTable[0][10].frequency);
    ASSERT_NEAR(0.195105, costTable[0][10].cost, DoubleFloatPrecision);
}


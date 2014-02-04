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
#include "../../../SharedLib/SharedLib/PixelClockCapabilities.h"
#include "../../../SharedLib/SharedLib/PixelClockDataSet.h"

class ActPolicyInternals : public ::testing::Test
{
     
public:

    PixelClockCapabilities capabilities_01;
    PixelClockDataSet dataSet_01;

    ActPolicyInternals::ActPolicyInternals(void)
        : capabilities_01(PixelClockPanelType::Edp, 2, true, true, PixelClockChannelType::Single, true, 
        PixelClockSpreadType::Up, .02),
        dataSet_01(std::vector<PixelClockData>(1, PixelClockData(138780000, 138780000, 138780000)))
    {

    }

    virtual void SetUp() override {
        ::testing::FLAGS_gmock_verbose = "error"; // needed to suppress erroneous warnings
    }

    virtual void TearDown() override
    {
        ::testing::FLAGS_gmock_verbose = "warning"; // re-enable warnings
    }
};


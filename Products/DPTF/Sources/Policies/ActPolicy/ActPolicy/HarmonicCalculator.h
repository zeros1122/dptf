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
#include "Dptf.h"
#include "PixelClockSpreadType.h"

class dptf_export HarmonicCalculator
{
public:

    static UInt64 calculateFirstHarmonic(UInt64 clockFrequency, double spreadPercent, UInt64 exclusionFrequency);
    static UInt64 calculateLastHarmonic(UInt64 clockFrequency, double spreadPercent, UInt64 exclusionFrequency);
    static std::pair<double,double> calculateClockHarmonicEdges(
        double harmonicFrequency, PixelClockSpreadType::Type spreadType, double spreadPercent);
    static Bool radioBandsOverlap(std::pair<double,double> firstBand, std::pair<double,double> secondBand);
};
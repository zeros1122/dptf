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

#include "HarmonicCalculator.h"

UInt64 HarmonicCalculator::calculateFirstHarmonic(UInt64 clockFrequency, double spreadPercent, UInt64 exclusionFrequency)
{
    UInt64 initialFirstHarmonic = 
        (UInt64)std::floor(((double)exclusionFrequency / (double)clockFrequency) + 0.5);
    double lowerEdge = 
        (double)exclusionFrequency - 
        (double)(spreadPercent * (double)initialFirstHarmonic * (double)clockFrequency);
    UInt64 firstHarmonic = 
        (UInt64)std::floor(((double)lowerEdge / (double)clockFrequency) + (double)0.5);
    return firstHarmonic;
}

UInt64 HarmonicCalculator::calculateLastHarmonic(UInt64 clockFrequency, double spreadPercent, UInt64 exclusionFrequency)
{
    UInt64 initialLastHarmonic = 
        (UInt64)std::floor(((double)exclusionFrequency / (double)clockFrequency) + 0.5);
    double lowerEdge = 
        (double)exclusionFrequency + 
        (double)(spreadPercent * (double)initialLastHarmonic * (double)clockFrequency);
    UInt64 lastHarmonic = 
        (UInt64)std::floor(((double)lowerEdge / (double)clockFrequency) + (double)0.5);
    return lastHarmonic;
}

std::pair<double,double> HarmonicCalculator::calculateClockHarmonicEdges(
    double harmonicFrequency, PixelClockSpreadType::Type spreadType, double spreadPercent)
{
    std::pair<double,double> result;
    double variability = (double)1000 / (double)1000000;
    switch (spreadType)
    {
    case PixelClockSpreadType::None:
        result.first = (double)(harmonicFrequency * (1 - variability / 2));
        result.second = (double)(harmonicFrequency * (1 + variability / 2));
        break;
    case PixelClockSpreadType::Up:
        result.first = (double)(harmonicFrequency * (1 - variability / 2));
        result.second = (double)((1 + (spreadPercent + variability / 2)) * harmonicFrequency);
        break;
    case PixelClockSpreadType::Center:
        result.first = (double)(1 - (spreadPercent + variability) / 2) * harmonicFrequency;
        result.second = (double)(1 + (spreadPercent + variability) / 2) * harmonicFrequency;
        break;
    case PixelClockSpreadType::Down:
        result.first = (double)(1 - (spreadPercent + variability / 2)) * harmonicFrequency;
        result.second = (double)(harmonicFrequency * (1 + variability / 2));
        break;
    default:
        throw dptf_exception("Failed to calculate clock harmonic edges.  The given spread type is invalid.");
    }
    return result;
}

Bool HarmonicCalculator::radioBandsOverlap(std::pair<double,double> firstBand, std::pair<double,double> secondBand)
{
    return 
        ((((firstBand.first < secondBand.first) && (firstBand.second < secondBand.first)) || 
        ((firstBand.first > secondBand.second) && (firstBand.second > secondBand.second))) == false);
}

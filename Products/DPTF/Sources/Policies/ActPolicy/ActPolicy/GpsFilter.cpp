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

#include "GpsFilter.h"
#include "HarmonicCalculator.h"

const Frequency gpsStart = 1572000000;
const Frequency gpsEnd = 1578000000;
const UInt32 NumberOfSubharmonics = 2;

PixelClockFrequencyTable GpsFilter::filter(const PixelClockFrequencyTable& frequencyTable)
{
    PixelClockFrequencyTable newLists;
    newLists.setSpreadPercentage(frequencyTable.getSpreadPercentage());
    newLists.setSpreadType(frequencyTable.getSpreadType());
    for (UIntN listNumber = 0; listNumber < frequencyTable.size(); listNumber++)
    {
        newLists.add(filterForFrequencyRange(frequencyTable[listNumber], frequencyTable.getSpreadPercentage(), 
            frequencyTable.getSpreadType(), gpsStart, gpsEnd));
    }
    return newLists;
}

std::vector<Frequency> GpsFilter::filterForFrequencyRange(const std::vector<Frequency>& candidateFrequencies, 
    double spreadPercent, PixelClockSpreadType::Type spreadType, const Frequency& startFrequency, 
    const Frequency& endFrequency)
{
    std::vector<Frequency> filteredFrequencies;
    for (auto frequency = candidateFrequencies.begin(); frequency != candidateFrequencies.end(); frequency++)
    {
        UInt64 subharmonicFrequency = *frequency / NumberOfSubharmonics;
        UInt64 firstHarmonic = HarmonicCalculator::calculateFirstHarmonic(
            subharmonicFrequency, spreadPercent, startFrequency);
        UInt64 lastHarmonic = HarmonicCalculator::calculateLastHarmonic(
            subharmonicFrequency, spreadPercent, endFrequency);

        Bool frequencyInterferesWithRadioBand(false);
        for (UInt64 harmonic = firstHarmonic; harmonic <= lastHarmonic; harmonic++)
        {
            double harmonicFrequency = (double)harmonic * (double)subharmonicFrequency;
            std::pair<double,double> edges = HarmonicCalculator::calculateClockHarmonicEdges(harmonicFrequency, 
                spreadType, spreadPercent);
            std::pair<double,double> radioBand;
            radioBand.first = (double)startFrequency;
            radioBand.second = (double)endFrequency;
            if (HarmonicCalculator::radioBandsOverlap(edges, radioBand))
            {
                frequencyInterferesWithRadioBand = true;
                break;
            }
        }

        if (frequencyInterferesWithRadioBand == false)
        {
            filteredFrequencies.push_back(*frequency);
        }
    }
    return filteredFrequencies;
}
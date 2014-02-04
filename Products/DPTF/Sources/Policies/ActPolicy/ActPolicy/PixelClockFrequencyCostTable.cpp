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

#include "PixelClockFrequencyCostTable.h"
#include "HarmonicCalculator.h"

const UInt32 NumberOfSubharmonics = 2;

PixelClockFrequencyCostTable::PixelClockFrequencyCostTable(const PixelClockFrequencyTable& table, 
    const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency)
    : m_spreadType(PixelClockSpreadType::None), m_spreadPercentage(0.0)
{
    m_spreadType = table.getSpreadType();
    m_spreadPercentage = table.getSpreadPercentage();
    for (UIntN pixelClock = 0; pixelClock < table.size(); pixelClock++)
    {
        std::vector<FrequencyCost> costTable = calculateInterferenceCostTable(table[pixelClock], 
            radioLeftFrequency, radioRightFrequency);
        m_costs.push_back(costTable);
    }
}

PixelClockFrequencyCostTable::~PixelClockFrequencyCostTable()
{

}

const std::vector<FrequencyCost>& PixelClockFrequencyCostTable::operator[](UIntN pixelClock) const
{
    return m_costs.at(pixelClock);
}

UIntN PixelClockFrequencyCostTable::size() const
{
    return (UIntN) m_costs.size();
}

std::vector<class FrequencyCost> PixelClockFrequencyCostTable::calculateInterferenceCostTable(
    std::vector<Frequency> candidateFrequencies, 
    const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency)
{
    std::vector<FrequencyCost> costTable;
    for (auto frequency = candidateFrequencies.begin();
        frequency != candidateFrequencies.end();
        frequency++)
    {
        UInt64 subharmonicFrequency = *frequency / NumberOfSubharmonics;
        UInt64 firstHarmonic = HarmonicCalculator::calculateFirstHarmonic(
            subharmonicFrequency, m_spreadPercentage, radioLeftFrequency);
        UInt64 lastHarmonic = HarmonicCalculator::calculateLastHarmonic(
            subharmonicFrequency, m_spreadPercentage, radioRightFrequency);
        double costOfFrequency(0);
        for (UInt64 harmonic = firstHarmonic; harmonic <= lastHarmonic; harmonic++)
        {
            double harmonicFrequency = (double)harmonic * (double)subharmonicFrequency;
            std::pair<double,double> edges = HarmonicCalculator::calculateClockHarmonicEdges(
                harmonicFrequency, m_spreadType, m_spreadPercentage);
            costOfFrequency += calculateHarmonicCostInRadioChannel(edges, radioLeftFrequency, radioRightFrequency);
        }
        costTable.push_back(FrequencyCost(*frequency, costOfFrequency));
    }
    return costTable;
}

double PixelClockFrequencyCostTable::calculateHarmonicCostInRadioChannel(
    const std::pair<double,double>& edges, const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency)
{
    if (m_spreadType == PixelClockSpreadType::None)
    {
        if ((edges.first >= radioLeftFrequency) &&
            (edges.first <= radioRightFrequency))
        {
            // Harmonic in RF band
            return 1.0;
        }
        else
        {
            // no cost
            return 0.0;
        }
    }
    else
    {
        return calculateSpreadCost(edges, radioLeftFrequency, radioRightFrequency);
    }
}

namespace ChannelIntersection
{
    enum Type
    {
        Left,
        Right,
        FullyContained,
        FullyOverlapped,
        None
    };
}

ChannelIntersection::Type determineIntersection(const std::pair<double,double>& edges,
    const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency)
{
    if ((edges.first >= radioLeftFrequency) && 
        (edges.second <= radioRightFrequency))
    {
        return ChannelIntersection::FullyContained;
    }

    if ((edges.first < radioLeftFrequency) && 
        (edges.second > radioRightFrequency))
    {
        return ChannelIntersection::FullyOverlapped;
    }

    if ((edges.first < radioLeftFrequency) && 
        (edges.second <= radioRightFrequency) && 
        (edges.second > radioLeftFrequency))
    {
        return ChannelIntersection::Left;
    }

    if ((edges.first >= radioLeftFrequency) && 
        (edges.second > radioRightFrequency) &&
        (edges.first < radioRightFrequency))
    {
        return ChannelIntersection::Right;
    }

    return ChannelIntersection::None;
}

double PixelClockFrequencyCostTable::calculateSpreadCost(
    const std::pair<double,double>& edges, const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency)
{
    double clockSpreadProfile = 1.0 / (edges.second - edges.first);

    switch (determineIntersection(edges, radioLeftFrequency, radioRightFrequency))
    {
    case ChannelIntersection::FullyContained:
        return (edges.second - edges.first) * clockSpreadProfile;
    case ChannelIntersection::FullyOverlapped:
        return ((double)radioRightFrequency - (double)radioLeftFrequency) * clockSpreadProfile;
    case ChannelIntersection::Left:
        return (edges.second - (double)radioLeftFrequency) * clockSpreadProfile;
    case ChannelIntersection::Right:
        return ((double)radioRightFrequency - edges.first) * clockSpreadProfile;
    case ChannelIntersection::None:
    default:
        return 0.0;
    }
}
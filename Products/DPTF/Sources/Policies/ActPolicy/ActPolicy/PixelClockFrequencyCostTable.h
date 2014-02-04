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
#include "PixelClockFrequencyTable.h"

class dptf_export FrequencyCost
{
public:
    FrequencyCost(Frequency newfrequency, double newCost)
        : frequency(newfrequency), cost(newCost) {};
    Frequency frequency;
    double cost;
};

class dptf_export PixelClockFrequencyCostTable
{
public:

    PixelClockFrequencyCostTable(const PixelClockFrequencyTable& table, 
        const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency);
    ~PixelClockFrequencyCostTable();

    const std::vector<FrequencyCost>& operator[](UIntN pixelClock) const;
    UIntN size() const;

private:

    std::vector<std::vector<FrequencyCost>> m_costs;
    PixelClockSpreadType::Type m_spreadType;
    double m_spreadPercentage;

    std::vector<FrequencyCost> calculateInterferenceCostTable(std::vector<Frequency> candidateFrequencies, 
        const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency);
    double calculateHarmonicCostInRadioChannel(const std::pair<double,double>& edges, 
        const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency);
    double calculateSpreadCost(const std::pair<double,double>& edges, 
        const Frequency& radioLeftFrequency, const Frequency& radioRightFrequency);
};
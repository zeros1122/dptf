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

#include "CandidateFrequencyGenerator.h"

const UInt32 RoundingDigit = 100000;
const double SscDisabledSpread = 0.005;

PixelClockFrequencyTable CandidateFrequencyGenerator::generateSscDisabledCandidates(
    const PixelClockCapabilities& capabilities, const PixelClockDataSet& dataSet)
{
    PixelClockFrequencyTable candidateFrequenciesList;
    for (UIntN pixelClockIndex = 0; pixelClockIndex < dataSet.getCount(); pixelClockIndex++)
    {
        if (dataSet[pixelClockIndex].getPanelInputFrequencySpecification() > Frequency(0))
        {
            std::vector<Frequency> candidateFrequencies = generateCandidateFrequencies(
                dataSet[pixelClockIndex].getPanelInputFrequencySpecification(), capabilities);
            candidateFrequenciesList.add(candidateFrequencies);
        }
    }
    candidateFrequenciesList.setSpreadType(PixelClockSpreadType::Center);
    candidateFrequenciesList.setSpreadPercentage(SscDisabledSpread);
    return candidateFrequenciesList;
}

PixelClockFrequencyTable CandidateFrequencyGenerator::generateSscEnabledCandidates(
    const PixelClockCapabilities& capabilities, const PixelClockDataSet& dataSet)
{
    PixelClockFrequencyTable candidateFrequenciesList;
    for (UIntN pixelClockIndex = 0; pixelClockIndex < dataSet.getCount(); pixelClockIndex++)
    {
        if (dataSet[pixelClockIndex].getPanelInputFrequencySpecification() > Frequency(0))
        {
            std::vector<Frequency> candidateSscEnabledFrequencies = generateCandidateFrequencies(
                dataSet[pixelClockIndex].getPanelInputFrequencySpecification(), capabilities);
            candidateFrequenciesList.add(candidateSscEnabledFrequencies);
        }
    }
    candidateFrequenciesList.setSpreadType(capabilities.getSscSpreadDirection());
    double halfSpreadPercentage = capabilities.getSpreadPercentage() / (double)2;
    candidateFrequenciesList.setSpreadPercentage(halfSpreadPercentage);
    return candidateFrequenciesList;
}

std::vector<Frequency> CandidateFrequencyGenerator::generateCandidateFrequencies(const Frequency& baseFrequency,
    const PixelClockCapabilities& capabilities)
{
    Frequency stepping = calculateStepping(baseFrequency, capabilities);
    UInt32 numberOfFrequenciesToGenerate = calculateNumberOfFrequenciesToGenerate(baseFrequency, stepping, 
        capabilities);
    std::vector<Frequency> candidateFrequencies = calculateCandidateFrequencies(
        baseFrequency, stepping, numberOfFrequenciesToGenerate, capabilities);
    return candidateFrequencies;
}

Frequency CandidateFrequencyGenerator::calculateStepping(const Frequency& clockFrequency, 
    const PixelClockCapabilities& capabilities)
{
    if (capabilities.getClockDeviation() == 0)
    {
        throw dptf_exception("Cannot calculate stepping since clock deviation is 0.");
    }
    double clockDeviationInPercent = (double)capabilities.getClockDeviation() / (double)100;
    UInt64 panelFrequencyRange = (UInt64)((double)clockFrequency * clockDeviationInPercent);
    Frequency stepping = (UInt64)((double)0.2 * (double)panelFrequencyRange);
    return stepping;
}

UInt32 CandidateFrequencyGenerator::calculateNumberOfFrequenciesToGenerate(const Frequency& clockFrequency, 
    const Frequency& stepping, const PixelClockCapabilities& capabilities)
{
    double clockDeviationInPercent = (double)capabilities.getClockDeviation() / (double)100;
    UInt64 panelFrequencyRange = (UInt64)((double)(clockFrequency / 1000) * clockDeviationInPercent);
    UInt32 numberOfFrequenciesToGenerate = (UInt32)(panelFrequencyRange / (stepping / 1000));
    if (capabilities.isUpwardDeviation() && capabilities.isDownwardDeviation())
    {
        numberOfFrequenciesToGenerate = (numberOfFrequenciesToGenerate * 2) + 1;
    }
    return numberOfFrequenciesToGenerate;
}

std::vector<Frequency> CandidateFrequencyGenerator::calculateCandidateFrequencies(const Frequency& clockFrequency, 
    const Frequency& stepping, UInt32 numberOfFrequenciesToGenerate, const PixelClockCapabilities& capabilities)
{
    std::vector<Frequency> candidateFrequencies;
    Frequency roundedStepping = round(stepping, RoundingDigit);
    if ((capabilities.isUpwardDeviation() == true) && (capabilities.isDownwardDeviation() == false))
    {
        for (UInt32 frequencyNumber = 0; frequencyNumber < numberOfFrequenciesToGenerate; frequencyNumber++)
        {
            candidateFrequencies.push_back(clockFrequency + Frequency(frequencyNumber * roundedStepping));
        }
    }
    else if ((capabilities.isUpwardDeviation() == false) && (capabilities.isDownwardDeviation() == true))
    {
        for (UInt32 frequencyNumber = 0; frequencyNumber < numberOfFrequenciesToGenerate; frequencyNumber++)
        {
            candidateFrequencies.push_back(clockFrequency - Frequency(frequencyNumber * roundedStepping));
        }
    }
    else if ((capabilities.isUpwardDeviation() == true) && (capabilities.isDownwardDeviation() == true))
    {
        candidateFrequencies.push_back(clockFrequency);
        for (UInt32 frequencyNumber = 1; frequencyNumber <= (numberOfFrequenciesToGenerate / 2); frequencyNumber++)
        {
            candidateFrequencies.push_back(clockFrequency + Frequency(frequencyNumber * roundedStepping));
            candidateFrequencies.push_back(clockFrequency - Frequency(frequencyNumber * roundedStepping));
        }
    }
    return candidateFrequencies;
}

Frequency CandidateFrequencyGenerator::round(const Frequency& frequency, UInt32 amount)
{
    UInt64 remainder = frequency % amount;
    if (remainder < (amount / 2))
    {
        return frequency - Frequency(remainder);
    }
    else
    {
        return frequency + (Frequency(amount) - Frequency(remainder));
    }
}
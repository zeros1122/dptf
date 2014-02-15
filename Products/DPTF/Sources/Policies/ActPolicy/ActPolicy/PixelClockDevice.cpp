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

#include "PixelClockDevice.h"
#include "CandidateFrequencyGenerator.h"
#include "GpsFilter.h"
#include "PixelClockFrequencyCostTable.h"
#include "StatusFormat.h"
using namespace StatusFormat;

PixelClockDevice::PixelClockDevice(DomainProxy& domain)
    : m_domain(&domain), 
    m_capabilities(PixelClockPanelType::Edp, 0, false, false, PixelClockChannelType::Single, false, 
        PixelClockSpreadType::None, 0.0),
    m_dataSet(std::vector<PixelClockData>())
{
    throwIfNotPixelClockDevice();
    m_capabilities = m_domain->getPixelClockControl().getPixelClockCapabilities();
    m_dataSet = m_domain->getPixelClockControl().getPixelClockDataSet();
    m_sscDisabledCandidateFrequencyTable = GpsFilter().filter(
        CandidateFrequencyGenerator::generateSscDisabledCandidates(m_capabilities, m_dataSet));
    m_sscEnabledCandidateFrequencyTable = GpsFilter().filter(
        CandidateFrequencyGenerator::generateSscEnabledCandidates(m_capabilities, m_dataSet));
}

PixelClockDevice::~PixelClockDevice()
{

}

UIntN PixelClockDevice::getParticipantIndex() const
{
    return m_domain->getParticipantIndex();
}

UIntN PixelClockDevice::getDomainIndex() const
{
    return m_domain->getDomainIndex();
}

Bool PixelClockDevice::isPixelClockDevice(const DomainProxy& domain)
{
    return (domain.getPixelClockControl().supportsPixelClockControls() && 
        domain.getPixelClockControl().supportsStatus());
}

void PixelClockDevice::adjustFrequencies(const RadioDeviceList& radioDevices)
{
    RadioDevice radio = radioDevices.getHighestPriorityRadio();
    PixelClockFrequencyCostTable sscDisabledCostTable(m_sscDisabledCandidateFrequencyTable, 
        radio.getLeftFrequency(), radio.getRightFrequency());
    PixelClockFrequencyCostTable sscEnabledCostTable(m_sscEnabledCandidateFrequencyTable, 
        radio.getLeftFrequency(), radio.getRightFrequency());
    std::vector<PixelClockData> bestFrequencies = selectBestFrequenciesForEachPixelClock(
        sscDisabledCostTable, sscEnabledCostTable);
    m_domain->getPixelClockControl().sendPixelClockFrequencies(bestFrequencies);
    m_dataSet = m_domain->getPixelClockControl().getPixelClockDataSet();
}

std::vector<PixelClockData> PixelClockDevice::selectBestFrequenciesForEachPixelClock(
    const PixelClockFrequencyCostTable& sscDisabledCostTable, 
    const PixelClockFrequencyCostTable& sscEnabledCostTable)
{
    std::vector<PixelClockData> bestFrequencies;
    for (UIntN pixelClockIndex = 0; pixelClockIndex < m_dataSet.getCount(); pixelClockIndex++)
    {
        Frequency bestSscDisabledFrequency = selectBestFrequency(sscDisabledCostTable[pixelClockIndex], 
            m_dataSet[pixelClockIndex].getSscDisabledNudgeFrequency());
        Frequency bestSscEnabledFrequency(0);
        if (m_capabilities.isSscEnabled())
        {
            bestSscEnabledFrequency = selectBestFrequency(sscEnabledCostTable[pixelClockIndex], 
                m_dataSet[pixelClockIndex].getSscEnabledNudgeFrequency());
        }
        else
        {
            bestSscEnabledFrequency = m_dataSet[pixelClockIndex].getSscEnabledNudgeFrequency();
        }
        bestFrequencies.push_back(PixelClockData(m_dataSet[pixelClockIndex].getPanelInputFrequencySpecification(),
            bestSscEnabledFrequency, bestSscDisabledFrequency));
    }
    return bestFrequencies;
}

Frequency PixelClockDevice::selectBestFrequency(const std::vector<FrequencyCost>& frequencyCosts, 
    Frequency currentFrequency)
{
    try
    {
        std::vector<FrequencyCost> lowestCostFrequencies = getFrequenciesWithLowestCost(frequencyCosts);
        if (isFrequencyInList(currentFrequency, lowestCostFrequencies))
        {
            return currentFrequency;
        }
        else
        {
            return lowestCostFrequencies.at(0).frequency;
        }
    }
    catch (...)
    {
        return currentFrequency;
    }
}

std::vector<FrequencyCost> PixelClockDevice::getFrequenciesWithLowestCost(
    const std::vector<FrequencyCost>& costTable)
{
    double lowestCost = findLowestCost(costTable);
    std::vector<FrequencyCost> lowestCostFrequencies;
    for (auto costEntry = costTable.begin(); costEntry != costTable.end(); costEntry++)
    {
        if (costEntry->cost == lowestCost)
        {
            lowestCostFrequencies.push_back(*costEntry);
        }
    }
    return lowestCostFrequencies;
}

double PixelClockDevice::findLowestCost(const std::vector<FrequencyCost>& costTable)
{
    if (costTable.size() == 0)
    {
        throw dptf_exception("Cannot find lowest cost frequency because the cost table is empty.");
    }
    else
    {
        Bool bestFrequencyCostIsValid(false);
        double bestFrequencyCost(0);
        for (auto costEntry = costTable.begin(); costEntry != costTable.end(); costEntry++)
        {
            if (bestFrequencyCostIsValid == false)
            {
                bestFrequencyCost = costEntry->cost;
                bestFrequencyCostIsValid = true;
            }
            else
            {
                if (costEntry->cost < bestFrequencyCost)
                {
                    bestFrequencyCost = costEntry->cost;
                }
            }
        }
        return bestFrequencyCost;
    }
}

Bool PixelClockDevice::isFrequencyInList(Frequency frequency, const std::vector<FrequencyCost>& lowestCostFrequencies)
{
    for (auto candidateFrequency = lowestCostFrequencies.begin(); 
        candidateFrequency != lowestCostFrequencies.end(); 
        candidateFrequency++)
    {
        if (candidateFrequency->frequency == frequency)
        {
            return true;
        }
    }
    return false;
}

void PixelClockDevice::throwIfNotPixelClockDevice()
{
    if (isPixelClockDevice(*m_domain) == false)
    {
        throw dptf_exception("Domain does not support the Pixel Clock control interface.");
    }
}

XmlNode* PixelClockDevice::getXml() const
{
    XmlNode* device = XmlNode::createWrapperElement("pixel_clock_device");
    device->addChild(XmlNode::createDataElement("participant_index", friendlyValue(getParticipantIndex())));
    device->addChild(XmlNode::createDataElement("domain_index", friendlyValue(getDomainIndex())));
    device->addChild(m_capabilities.getXml());
    device->addChild(m_dataSet.getXml());
    return device;
}

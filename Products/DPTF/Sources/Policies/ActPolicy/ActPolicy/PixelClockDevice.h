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
#include "PixelClockCapabilities.h"
#include "PixelClockDataSet.h"
#include "DomainProxy.h"
#include "RadioDeviceList.h"
#include "XmlNode.h"
#include "PixelClockFrequencyTable.h"
#include "PixelClockFrequencyCostTable.h"

class dptf_export PixelClockDevice
{
public:

    PixelClockDevice(DomainProxy& domain, Bool filterForGps);
    ~PixelClockDevice();

    UIntN getParticipantIndex() const;
    UIntN getDomainIndex() const;
    void adjustFrequencies(const RadioDeviceList& radioDevices);
    XmlNode* getXml() const;
    static Bool isPixelClockDevice(const DomainProxy& domain);

private:

    PixelClockCapabilities m_capabilities;
    PixelClockDataSet m_dataSet;
    DomainProxy* m_domain;
    PixelClockFrequencyTable m_sscDisabledCandidateFrequencyTable;
    PixelClockFrequencyTable m_sscDisabledCandidateFrequencyTableFiltered;
    PixelClockFrequencyTable m_sscEnabledCandidateFrequencyTable;
    PixelClockFrequencyTable m_sscEnabledCandidateFrequencyTableFiltered;
    Bool m_filterForGps;

    std::vector<PixelClockData> selectBestFrequenciesForEachPixelClock(
        const PixelClockFrequencyCostTable& sscDisabledCostTable, 
        const PixelClockFrequencyCostTable& sscEnabledCostTable);
    Frequency selectBestFrequency(const std::vector<FrequencyCost>& frequencyCosts, Frequency currentFrequency);
    std::vector<class FrequencyCost> getFrequenciesWithLowestCost(const std::vector<class FrequencyCost>& costTable);
    double findLowestCost(const std::vector<class FrequencyCost>& costTable);
    Bool isFrequencyInList(Frequency frequency, const std::vector<FrequencyCost>& lowestCostFrequencies);
    void throwIfNotPixelClockDevice();
};
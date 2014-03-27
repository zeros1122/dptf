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
#include <vector>
#include "XmlNode.h"

class dptf_export PixelClockFrequencyTable
{
public:

    PixelClockFrequencyTable();
    ~PixelClockFrequencyTable();

    void add(const std::vector<Frequency>& frequencies);
    UIntN size() const;
    const std::vector<Frequency>& operator[](UIntN pixelClock) const;

    void setSpreadType(PixelClockSpreadType::Type spreadType);
    PixelClockSpreadType::Type getSpreadType() const;
    void setSpreadPercentage(double spreadPercentage);
    double getSpreadPercentage() const;
    XmlNode* getXml() const;
    
private:

    std::vector<std::vector<Frequency>> m_frequencyLists;
    PixelClockSpreadType::Type m_spreadType;
    double m_spreadPercentage;
};
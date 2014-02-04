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

#include "PixelClockFrequencyTable.h"


PixelClockFrequencyTable::PixelClockFrequencyTable()
    : m_spreadPercentage(0.0), m_spreadType(PixelClockSpreadType::None)
{

}

PixelClockFrequencyTable::~PixelClockFrequencyTable()
{

}

void PixelClockFrequencyTable::add(const std::vector<Frequency>& frequencies)
{
    m_frequencyLists.push_back(frequencies);
}

UIntN PixelClockFrequencyTable::size() const
{
    return (UIntN)m_frequencyLists.size();
}

const std::vector<Frequency>& PixelClockFrequencyTable::operator[](UIntN pixelClock) const
{
    return m_frequencyLists.at(pixelClock);
}

void PixelClockFrequencyTable::setSpreadType(PixelClockSpreadType::Type spreadType)
{
    m_spreadType = spreadType;
}

void PixelClockFrequencyTable::setSpreadPercentage(double spreadPercentage)
{
    m_spreadPercentage = spreadPercentage;
}

double PixelClockFrequencyTable::getSpreadPercentage() const
{
    return m_spreadPercentage;
}

PixelClockSpreadType::Type PixelClockFrequencyTable::getSpreadType() const
{
    return m_spreadType;
}

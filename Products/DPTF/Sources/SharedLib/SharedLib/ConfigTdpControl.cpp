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

#include "ConfigTdpControl.h"
#include "XmlNode.h"
#include "StatusFormat.h"
#include <iomanip>

ConfigTdpControl::ConfigTdpControl(UInt64 controlId, UInt64 tarRatio, UInt64 tdpPower, UInt64 tdpFrequency) :
    m_controlId(controlId), m_tdpPower(tdpPower), m_tdpFrequency(tdpFrequency),
    m_tdpRatio(tarRatio)
{
}

UInt64 ConfigTdpControl::getControlId(void) const
{
    return m_controlId;
}

UInt64 ConfigTdpControl::getTdpRatio(void) const
{
    return m_tdpRatio;
}

UInt64 ConfigTdpControl::getTdpPower(void) const
{
    return m_tdpPower;
}

UInt64 ConfigTdpControl::getTdpFrequency(void) const
{
    return m_tdpFrequency;
}

Bool ConfigTdpControl::operator==(const ConfigTdpControl& rhs) const
{
    return
        ((getControlId() == rhs.getControlId()) &&
         (getTdpPower() == rhs.getTdpPower()) &&
         (getTdpFrequency() == rhs.getTdpFrequency()) &&
         (getTdpRatio() == rhs.getTdpRatio()));
}

Bool ConfigTdpControl::operator!=(const ConfigTdpControl& rhs) const
{
    return !(*this == rhs);
}

std::string ConfigTdpControl::getNameListString() const
{
    double powerInWatts = (double)getTdpPower() / (double)1000;
    double frequencyInGHz = (double)getTdpFrequency() / (double)1000;
    std::stringstream stream;
    stream << std::setprecision(1) << std::fixed << powerInWatts << "W";
    stream << " @ ";
    stream << std::setprecision(1) << std::fixed << frequencyInGHz << "GHz";
    return stream.str();
}

XmlNode* ConfigTdpControl::getXml(void)
{
    XmlNode* root = XmlNode::createWrapperElement("config_tdp_control");

    root->addChild(XmlNode::createDataElement("control_id", StatusFormat::friendlyValue(m_controlId)));
    root->addChild(XmlNode::createDataElement("tdp_ratio", StatusFormat::friendlyValue(m_tdpRatio)));
    root->addChild(XmlNode::createDataElement("tdp_power", StatusFormat::friendlyValue(m_tdpPower)));
    root->addChild(XmlNode::createDataElement("tdp_frequency", StatusFormat::friendlyValue(m_tdpFrequency)));

    return root;
}
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

#include "DomainPriority_000.h"

DomainPriority_000::DomainPriority_000(ParticipantServicesInterface* participantServicesInterface)
{
    // Do nothing.  Not an error.
}

DomainPriority DomainPriority_000::getDomainPriority(UIntN participantIndex, UIntN domainIndex)
{
    return DomainPriority(0);
}

void DomainPriority_000::clearCachedData(void)
{
    // Do nothing.  Not an error.
}

XmlNode* DomainPriority_000::getXml(UIntN domainIndex)
{
    // Do nothing.  Not an error.
    throw not_implemented();
}
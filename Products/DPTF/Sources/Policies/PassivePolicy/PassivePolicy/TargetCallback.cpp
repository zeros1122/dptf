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

#include "TargetCallback.h"

using namespace std;

TargetCallback::TargetCallback()
    : m_timeDelta(Constants::Invalid),
    m_timeStamp(Constants::Invalid),
    m_callbackHandle(Constants::Invalid)
{
}

TargetCallback::TargetCallback(UInt64 timeDelta, UInt64 timeStamp, UInt64 callbackHandle)
    : m_timeDelta(timeDelta),
    m_timeStamp(timeStamp),
    m_callbackHandle(callbackHandle)
{
}

UInt64 TargetCallback::getTimeDelta() const
{
    return m_timeDelta;
}

UInt64 TargetCallback::getTimeStamp() const
{
    return m_timeStamp;
}

UInt64 TargetCallback::getCallbackHandle() const
{
    return m_callbackHandle;
}
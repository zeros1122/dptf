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

#include <string>
#include "esif_uf_ccb_file.h"

class EsifFileEnumerator
{
public:

    EsifFileEnumerator(std::string path, std::string pattern);
    ~EsifFileEnumerator(void);

    std::string getFirstFile(void);
    std::string getNextFile(void);

private:

    // hide the copy constructor and assignment operator.
    EsifFileEnumerator(const EsifFileEnumerator& rhs);
    EsifFileEnumerator& operator=(const EsifFileEnumerator& rhs);

    std::string m_path;
    std::string m_pattern;
    esif_ccb_file_find_handle m_fileHandle;
    esif_ccb_file m_file;
};
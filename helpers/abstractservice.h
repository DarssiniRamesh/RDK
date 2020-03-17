/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#pragma once

#include <string>


class AbstractService
{
public:
    AbstractService()
    {
        m_lastError = StatusOk;
    }

public:
    enum Status 
    {
        StatusOk,
        StatusFailure,
        StatusInvalidArgument,
        StatusInvalidState,
        StatusMethodNotFound
    };
    
    void setLastError(Status statusCode, const std::string& statusMessage = std::string())
    {
        m_lastError = statusCode;
        m_lastErrorDescription = statusMessage;
    }
    
    Status getLastError()
    {
        return m_lastError;
    }
    
    std::string getLastErrorDescription()
    {
        return m_lastErrorDescription;
    }

private:
    Status  m_lastError;
    std::string m_lastErrorDescription;
};

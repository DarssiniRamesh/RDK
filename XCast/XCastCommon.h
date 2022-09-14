/**
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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

#include <iostream>
#include <cstring>

#ifndef __XCAST_COMMON_H__
#define __XCAST_COMMON_H__
typedef struct _DynamicAppConfig {
    char appName[256];
    char prefixes[512];
    char cors[512];
    int  allowStop = 0; //Default allowStop value is false
    char query[2048];
    char payload[2048];

    public:
       _DynamicAppConfig(){
           memset (appName, '\0', sizeof(appName));
           memset (prefixes, '\0', sizeof(prefixes));
           memset (cors, '\0', sizeof(cors));
           memset (query, '\0', sizeof(query));
           memset (payload, '\0', sizeof(payload));
       }
}DynamicAppConfig;

#endif

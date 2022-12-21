/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2022 Sky UK
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
*/

#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <string>
#include "libIARM.h"
#include "libIBusDaemon.h"
#include "libIBus.h"
#include "iarmUtil.h"

#include <iostream>

#include <pthread.h>
#include "Module.h"

#include <sys/stat.h>
#include <vector>
#include "ControlSettingsTV.h"
#include "ControlSettingsSTB.h"

//Default ControlSettingsSTB
#ifndef DEVICE_TYPE
#define DEVICE_TYPE ControlSettingsSTB
#endif

namespace WPEFramework {
namespace Plugin {

    class ControlSettings : public DEVICE_TYPE {

    private:
        ControlSettings(const ControlSettings&) = delete;
        ControlSettings& operator=(const ControlSettings&) = delete;
   public : 
	DECLARE_JSON_RPC_METHOD(getAspectRatio)
	DECLARE_JSON_RPC_METHOD(getAspectRatio2)
        DECLARE_JSON_RPC_METHOD(setAspectRatio)
        DECLARE_JSON_RPC_METHOD(setAspectRatio2)
        DECLARE_JSON_RPC_METHOD(resetAspectRatio)
        DECLARE_JSON_RPC_METHOD(getVideoFormat)
        DECLARE_JSON_RPC_METHOD(getVideoFrameRate)
        DECLARE_JSON_RPC_METHOD(getVideoResolution)

   public:
        ControlSettings();
        ~ControlSettings();

    private:
        uint8_t _skipURL;
	void InitializeIARM();
        void DeinitializeIARM();
        bool isIARMConnected();
        bool IARMinit();

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        const std::string Initialize(PluginHost::IShell* service);
        void Deinitialize(PluginHost::IShell* service);
        virtual string Information() const override { return {}; }
	virtual void AddRef() const { }
	virtual uint32_t Release() const {return 0; }
        BEGIN_INTERFACE_MAP(ControlSettings)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        END_INTERFACE_MAP
   };
}
}
#endif

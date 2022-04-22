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

#include "libIBus.h"
#include "Module.h"
#include "utils.h"
#include "AbstractPlugin.h"
#include "dsTypes.h"

namespace WPEFramework {

    namespace Plugin {

        // This is a server for a JSONRPC communication channel. 
        // For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
        // By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
        // This realization of this interface implements, by default, the following methods on this plugin
        // - exists
        // - register
        // - unregister
        // Any other methood to be handled by this plugin  can be added can be added by using the
        // templated methods Register on the PluginHost::JSONRPC class.
        // As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
        // this class exposes a public method called, Notify(), using this methods, all subscribed clients
        // will receive a JSONRPC message as a notification, in case this method is called.
        class AVInput : public AbstractPlugin {
        private:

            // We do not allow this plugin to be copied !!
            AVInput(const AVInput&) = delete;
            AVInput& operator=(const AVInput&) = delete;

            void InitializeIARM();
            void DeinitializeIARM();

            //Begin methods
            uint32_t getInputDevicesWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t writeEDIDWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t readEDIDWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getRawSPDWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getSPDWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setEdidVersionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getEdidVersionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t startInput(const JsonObject& parameters, JsonObject& response);
            uint32_t stopInput(const JsonObject& parameters, JsonObject& response);
            uint32_t setVideoRectangleWrapper(const JsonObject& parameters, JsonObject& response);
            //End methods

            JsonArray getInputDevices(int iType);
            void writeEDID(int deviceId, std::string message);
            std::string readEDID(int iPort);
            std::string getRawSPD(int iPort);
            std::string getSPD(int iPort);
            int setEdidVersion(int iPort, int iEdidVer);
            int getEdidVersion(int iPort);
            bool setVideoRectangle(int x, int y, int width, int height, int type);
 
            void AVInputHotplug(int input , int connect, int type);
            static void dsAVEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);

            void AVInputSignalChange( int port , int signalStatus, int type);
            static void dsAVSignalStatusEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);

            void AVInputStatusChange( int port , bool isPresented, int type);
            static void dsAVStatusEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);
    
            void AVInputVideoModeUpdate( int port , dsVideoPortResolution_t resolution);
            static void dsAVVideoModeEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);

        public:
            AVInput();
            virtual ~AVInput();
            virtual void Deinitialize(PluginHost::IShell* service) override;

            void terminate();

        public:
            static AVInput* _instance;
        };
    } // namespace Plugin
} // namespace WPEFramework

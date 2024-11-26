/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
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
 */
 
#include "DeviceIdentification.h"
#include "IdentityProvider.h"
#include <interfaces/IConfiguration.h>
#include <com/com.h>
#include <core/core.h>
#include <plugins/plugins.h>
#include <interfaces/Ids.h>


#define API_VERSION_NUMBER_MAJOR 1
#define API_VERSION_NUMBER_MINOR 0
#define API_VERSION_NUMBER_PATCH 6

namespace WPEFramework {
namespace {
    static Plugin::Metadata<Plugin::DeviceIdentification> metadata(
        // Version (Major, Minor, Patch)
        API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH,
        // Preconditions
#ifdef DISABLE_DEVICEID_CONTROL
        { PluginHost::ISubSystem::IDENTIFIER },
#else
        {},
#endif
        // Terminations
        {},
        // Controls
#ifdef DISABLE_DEVICEID_CONTROL
        {}
#else
        { PluginHost::ISubSystem::IDENTIFIER }
#endif
    );
}

namespace Plugin {

    SERVICE_REGISTRATION(DeviceIdentification, API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH);

    /* virtual */ const string DeviceIdentification::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_identifier == nullptr);
        ASSERT(_connectionId == 0);

        _service = service;
        _service->AddRef();

         string message;

        // Register the Process::Notification stuff. The Remote process might die before we get a
        // change to "register" the sink for these events !!! So do it ahead of instantiation.
        service->Register(&_notification);

        _identifier = service->Root<PluginHost::ISubSystem::IIdentifier>(_connectionId, RPC::CommunicationTimeOut, _T("DeviceImplementation"));

        if (_identifier != nullptr) {

            Exchange::IConfiguration* configure = _identifier->QueryInterface<Exchange::IConfiguration>();
            if (configure != nullptr) {
                configure->Configure(service);
                configure->Release();
            }

            _deviceId = GetDeviceId();

            RegisterAll();

            if (_deviceId.empty() != true) {
#ifndef DISABLE_DEVICEID_CONTROL
                service->SubSystems()->Set(PluginHost::ISubSystem::IDENTIFIER, _identifier);
#endif
            }
            else {
                message = _T("DeviceIdentification plugin could not be instantiated. No DeviceID available");
            }
        }
        else {
            message = _T("DeviceIdentification plugin could not be instantiated.");
        }

        if (message.length() != 0) {
            Deinitialize(service);
        }

        return message;
    }

    /* virtual */ void DeviceIdentification::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(_service == service);

        _service->Unregister(&_notification);

        if (_deviceId.empty() != true) {
#ifndef DISABLE_DEVICEID_CONTROL
            service->SubSystems()->Set(PluginHost::ISubSystem::IDENTIFIER, nullptr);
#endif
            _deviceId.clear();
        }
        if(_identifier != nullptr) {

            UnregisterAll();

            // Stop processing:
            RPC::IRemoteConnection* connection = service->RemoteConnection(_connectionId);

            VARIABLE_IS_NOT_USED uint32_t result = _identifier->Release();
            _identifier = nullptr;

            // It should have been the last reference we are releasing,
            // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
            // are leaking...
            ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);

            // If this was running in a (container) process...
            if (connection != nullptr) {
                // Lets trigger the cleanup sequence for
                // out-of-process code. Which will guard
                // that unwilling processes, get shot if
                // not stopped friendly :-)
                connection->Terminate();
                connection->Release();
            }
         }

         _connectionId = 0;

        _service->Release();
        _service = nullptr;
    }

    /* virtual */ string DeviceIdentification::Information() const
    {
        // No additional info to report.
        return (string());
    }

    string DeviceIdentification::GetDeviceId() const
    {
        string result;
#ifndef DISABLE_DEVICEID_CONTROL
        ASSERT(_identifier != nullptr);

        if (_identifier != nullptr) {
            uint8_t myBuffer[64];

            myBuffer[0] = _identifier->Identifier(sizeof(myBuffer) - 1, &(myBuffer[1]));

            if (myBuffer[0] != 0) {
                result = Core::SystemInfo::Instance().Id(myBuffer, ~0);
            }

            else // if buffer is empty, try to get serial number from COM RPC
            {
                std::cout << "akshay COM RPC from main function" << std::endl;
                serial = RetrieveSerialNumberFromRPC(); // trying to get serial number from COM RPC
                std::cout << "akshay Serial number from RPC: '" << serial << std::endl;
            
            if (!serial.empty()) {
            // Convert the string into the required buffer format
            uint8_t ret = serial.length();
 
            if (ret > (sizeof(myBuffer) - 1))
            {
                ret = sizeof(myBuffer) - 1;
                std::cout << "akshay Serial number length to: '" << ret << std::endl;
            }
            myBuffer[0] = ret;
            ::memcpy(&(myBuffer[1]), serial.c_str(), ret);
            
            if(myBuffer[0] != 0){
                std::cout << "if case akshay DeviceId from RPC: '" << result << std::endl;
                result = Core::SystemInfo::Instance().Id(myBuffer, ~0);
                std::cout << "akshay DeviceId from RPC: '" << result << std::endl;
            }
            }
            }
        }
#else
        // extract DeviceId set by Thunder
        if (_service->SubSystems()->IsActive(PluginHost::ISubSystem::IDENTIFIER) == true) {

            const PluginHost::ISubSystem::IIdentifier* identifier(_service->SubSystems()->Get<PluginHost::ISubSystem::IIdentifier>());

            if (identifier != nullptr) {
                uint8_t myBuffer[64];

                if ((myBuffer[0] = identifier->Identifier(sizeof(myBuffer) - 1, &(myBuffer[1]))) != 0) {
                    result = Core::SystemInfo::Instance().Id(myBuffer, ~0);
                }
                identifier->Release();
            }
         }
#endif
        return result;
    }

    std::string DeviceIdentification::RetrieveSerialNumberFromRPC() const
    {
        Core::ProxyType<RPC::InvokeServerType<1, 0, 4>> _engine;
        Core::ProxyType<RPC::CommunicatorClient> _communicatorClient;
        PluginHost::IShell *_controller;
        Exchange::IDeviceInfo* _remotDeviceInfoObject;
        _engine = (Core::ProxyType<RPC::InvokeServerType<1, 0, 4>>::Create());
        _communicatorClient = (Core::ProxyType<RPC::CommunicatorClient>::Create(Core::NodeId("/tmp/communicator"), Core::ProxyType<Core::IIPCServer>(_engine)));
        _controller = (nullptr);
        _remotDeviceInfoObject = (nullptr);

        if (!_communicatorClient.IsValid())
        {
            std::cout << "akshay Communicator client is not valid\n";
        }
        #if ((THUNDER_VERSION == 2) || ((THUNDER_VERSION == 4) && (THUNDER_VERSION_MINOR == 2)))
        _engine->Announcements(_communicatorClient->Announcement());
        #endif

       std::cout<< "akshay Opening DeviceInfo Controller\n";

        _controller = _communicatorClient->Open<PluginHost::IShell>(_T("DeviceInfo"), ~0, 3000);

        if (_controller)
        {
            _remotDeviceInfoObject = _controller->QueryInterface<Exchange::IDeviceInfo>();
                 
            if(_remotDeviceInfoObject)
            {
                _remotDeviceInfoObject->AddRef();
            }
                 
        }
        else
            {
                std::cout << "akshay Failed to get DeviceInfo Controller\n";
            }
        
                std::string Number;
        if (_remotDeviceInfoObject)
            {
                std::cout << "akshay declared at start\n"; 
                std::cout << "akshay Getting serial number from DeviceInfo Controller\n";
                _remotDeviceInfoObject->SerialNumber(Number);
                std::cout << "akshay Serial number from DeviceInfo Controller: '" << Number << std::endl;
            }
        
        else
            {
                std::cout << "akshay Failed to get serial number from DeviceInfo Controller\n";
            }

        if (_controller)
            {
                _controller->Release();
                _controller = nullptr;
            }

        _communicatorClient->Close(RPC::CommunicationTimeOut);
            if (_communicatorClient.IsValid())
            {
                _communicatorClient.Release();
                std::cout << "akshay Communicator client released\n";
            }

        if(_engine.IsValid())
            {
                _engine.Release();
                std::cout << "akshay Engine released\n";
            }

        if(_remotDeviceInfoObject)
            {
                _remotDeviceInfoObject->Release();
                std::cout << "akshay DeviceInfo object released\n";
            }    
        return Number;


    }

    void DeviceIdentification::Info(JsonData::DeviceIdentification::DeviceidentificationData& deviceInfo) const
    {
        deviceInfo.Firmwareversion = _identifier->FirmwareVersion();
        deviceInfo.Chipset = _identifier->Chipset();

        if (_deviceId.empty() != true) {
            deviceInfo.Deviceid = _deviceId;
        }
    }

    void DeviceIdentification::Deactivated(RPC::IRemoteConnection* connection)
    {
        // This can potentially be called on a socket thread, so the deactivation (wich in turn kills this object) must be done
        // on a seperate thread. Also make sure this call-stack can be unwound before we are totally destructed.
        if (_connectionId == connection->Id()) {

            ASSERT(_service != nullptr);

            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service, PluginHost::IShell::DEACTIVATED, PluginHost::IShell::FAILURE));
        }
    }
} // namespace Plugin
} // namespace WPEFramework

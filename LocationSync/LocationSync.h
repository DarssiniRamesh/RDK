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
 
#ifndef LOCATIONSYNC_LOCATIONSYNC_H
#define LOCATIONSYNC_LOCATIONSYNC_H

#include "LocationService.h"
#include <interfaces/ITimeZone.h>
#include <interfaces/json/JsonData_LocationSync.h>
#include "Module.h"

namespace WPEFramework {
namespace Plugin {

    class LocationSync : public PluginHost::IPlugin, public PluginHost::IWeb, public PluginHost::JSONRPC {
    public:
        class Data : public Core::JSON::Container {
        public:
            Data(Data const& other) = delete;
            Data& operator=(Data const& other) = delete;

            Data()
                : Core::JSON::Container()
                , PublicIp()
                , TimeZone()
                , Region()
                , Country()
                , City()
            {
                Add(_T("ip"), &PublicIp);
                Add(_T("timezone"), &TimeZone);
                Add(_T("region"), &Region);
                Add(_T("country"), &Region);
                Add(_T("city"), &Region);
            }

            ~Data() override
            {
            }

        public:
            Core::JSON::String PublicIp;
            Core::JSON::String TimeZone;
            Core::JSON::String Region;
            Core::JSON::String Country;
            Core::JSON::String City;
        };

    private:
        class Notification : public Core::IDispatch {
        private:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
#ifdef __WINDOWS__
#pragma warning(disable : 4355)
#endif
            explicit Notification(LocationSync* parent)
                : _parent(*parent)
                , _source()
                , _interval()
                , _retries()
                , _locator(Core::Service<LocationService>::Create<LocationService>(this))
            {
                ASSERT(parent != nullptr);
            }
#ifdef __WINDOWS__
#pragma warning(default : 4355)
#endif
            ~Notification()
            {
                _locator->Release();
            }

        public:
            inline void Initialize(const string& source, const uint16_t interval, const uint8_t retries)
            {
                _source = source;
                _interval = interval;
                _retries = retries;

                Probe();
            }
            inline void Deinitialize()
            {
            }
            uint32_t Probe(const string& remoteNode, const uint32_t retries, const uint32_t retryTimeSpan)
            {
                _source = remoteNode;
                _interval = retryTimeSpan;
                _retries = retries;

                return (Probe());
            }

            inline PluginHost::ISubSystem::ILocation* Location()
            {
                return (_locator);
            }
            inline PluginHost::ISubSystem::IInternet* Network()
            {
                return (_locator);
            }

        private:
            inline uint32_t Probe()
            {

                ASSERT(_locator != nullptr);

                return (_locator != nullptr ? _locator->Probe(_source, _retries, _interval) : static_cast<uint32_t>(Core::ERROR_UNAVAILABLE));
            }

            void Dispatch() override
            {
                _parent.SyncedLocation();
            }

        private:
            LocationSync& _parent;
            string _source;
            uint16_t _interval;
            uint8_t _retries;
            LocationService* _locator;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            Config()
                : Interval(30)
                , Retries(8)
                , Source()
                , TimezoneFile()
            {
                Add(_T("interval"), &Interval);
                Add(_T("retries"), &Retries);
                Add(_T("source"), &Source);
                Add(_T("timezonefile"), &TimezoneFile);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::DecUInt16 Interval;
            Core::JSON::DecUInt8 Retries;
            Core::JSON::String Source;
            Core::JSON::String TimezoneFile;
        };

    private:
        class TZ {
        public:
            TZ() : _sync(true) {}

            static TZ& Instance()
            {
                static TZ& _instance = Core::SingletonType<TZ>::Instance();
                return (_instance);
            }

            void Set(const string &value, bool synced)
            {
                Core::SafeSyncType <Core::CriticalSection> lock(_adminLock);

                if ((value.empty() == false) && (!synced || _sync)) {
                    if (!synced && _sync) {

                        // time zone set explicitly, no need to sync

                        _sync = false;
                    }

                    Core::SystemInfo::SetEnvironment(_T("TZ"), value);
                }
            }

        private:
            bool _sync;
            Core::CriticalSection _adminLock;
        };

        class TimeZoneNotification : protected Exchange::ITimeZone::INotification {
        private:
            TimeZoneNotification(const TimeZoneNotification&) = delete;
            TimeZoneNotification& operator=(const TimeZoneNotification&) = delete;

        public:
            TimeZoneNotification() : _client(nullptr) { }
            ~TimeZoneNotification() = default;

        public:
            void Initialize(Exchange::ITimeZone* client)
            {
                ASSERT(_client == nullptr);
                ASSERT(client != nullptr);

                _client = client;
                _client->AddRef();
                _client->Register(this);
            }
            void Deinitialize()
            {
                ASSERT(_client != nullptr);

                if (_client != nullptr) {
                    _client->Unregister(this);
                    _client->Release();
                    _client = nullptr;
                }
            }

        public:
            // ITimeZone::INotification methods

            virtual void TimeZoneChanged(const string &timeZone) override
            {
                TZ::Instance().Set(timeZone, false);
            }

            BEGIN_INTERFACE_MAP(TimeZoneNotification)
            INTERFACE_ENTRY(Exchange::ITimeZone::INotification)
            END_INTERFACE_MAP

        private:
            Exchange::ITimeZone* _client;
        };

    private:
        LocationSync(const LocationSync&) = delete;
        LocationSync& operator=(const LocationSync&) = delete;

    public:
        LocationSync();
        ~LocationSync() override;

        // Build QueryInterface implementation, specifying all possible interfaces to be returned.
        BEGIN_INTERFACE_MAP(LocationSync)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        INTERFACE_AGGREGATE(Exchange::ITimeZone, _timeZone)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

        //   IWeb methods
        // -------------------------------------------------------------------------------------------------------
        void Inbound(Web::Request& request) override;
        Core::ProxyType<Web::Response> Process(const Web::Request& request) override;

    private:
        void RegisterAll();
        void UnregisterAll();
        uint32_t endpoint_sync();
        uint32_t get_location(JsonData::LocationSync::LocationData& response) const;
        void event_locationchange();

        void SyncedLocation();

    private:
        uint16_t _skipURL;
        string _source;
        Core::Sink<Notification> _sink;
        PluginHost::IShell* _service;
        Exchange::ITimeZone* _timeZone;
        Core::Sink<TimeZoneNotification> _timeZoneSink;
    };

} // namespace Plugin
} // namespace WPEFramework

#endif // LOCATIONSYNC_LOCATIONSYNC_H

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

#pragma once

#include <mutex>
#include "Module.h"
#include "utils.h"
#include <rdkshell/rdkshellevents.h>
#include <rdkshell/rdkshell.h>
#include <rdkshell/linuxkeys.h>
#include "AbstractPlugin.h"

namespace WPEFramework {

    namespace Plugin {

        class RDKShell;
        class EventTimer
        {
            private:
                EventTimer() = delete;
                EventTimer& operator=(const EventTimer& RHS) = delete;

            public:
                EventTimer(RDKShell* shell): mShell(shell) {}
                ~EventTimer() {}

            public:
                uint64_t Timed(const uint64_t scheduledTime);
                inline bool operator==(const EventTimer& RHS) const
                {
                    return(mShell == RHS.mShell);
                }


            private:
                RDKShell* mShell;
        };

        class RDKShell :  public AbstractPlugin {
        public:
            RDKShell();
            virtual ~RDKShell();
            virtual const string Initialize(PluginHost::IShell* service) override;
            virtual void Deinitialize(PluginHost::IShell* service) override;
            virtual string Information() const override;

        public/*members*/:
            static RDKShell* _instance;

        public /*constants*/:
            static const short API_VERSION_NUMBER_MAJOR;
            static const short API_VERSION_NUMBER_MINOR;
            static const string SERVICE_NAME;
            //methods
            static const string RDKSHELL_METHOD_MOVE_TO_FRONT;
            static const string RDKSHELL_METHOD_MOVE_TO_BACK;
            static const string RDKSHELL_METHOD_MOVE_BEHIND;
            static const string RDKSHELL_METHOD_SET_FOCUS;
            static const string RDKSHELL_METHOD_KILL;
            static const string RDKSHELL_METHOD_ADD_KEY_INTERCEPT;
            static const string RDKSHELL_METHOD_REMOVE_KEY_INTERCEPT;
            static const string RDKSHELL_METHOD_ADD_KEY_LISTENER;
            static const string RDKSHELL_METHOD_REMOVE_KEY_LISTENER;
            static const string RDKSHELL_METHOD_ADD_KEY_METADATA_LISTENER;
            static const string RDKSHELL_METHOD_REMOVE_KEY_METADATA_LISTENER;
            static const string RDKSHELL_METHOD_INJECT_KEY;
            static const string RDKSHELL_METHOD_GENERATE_KEYS;
            static const string RDKSHELL_METHOD_GET_SCREEN_RESOLUTION;
            static const string RDKSHELL_METHOD_SET_SCREEN_RESOLUTION;
            static const string RDKSHELL_METHOD_CREATE_DISPLAY;
            static const string RDKSHELL_METHOD_GET_CLIENTS;
            static const string RDKSHELL_METHOD_GET_Z_ORDER;
            static const string RDKSHELL_METHOD_GET_BOUNDS;
            static const string RDKSHELL_METHOD_SET_BOUNDS;
            static const string RDKSHELL_METHOD_GET_VISIBILITY;
            static const string RDKSHELL_METHOD_SET_VISIBILITY;
            static const string RDKSHELL_METHOD_GET_OPACITY;
            static const string RDKSHELL_METHOD_SET_OPACITY;
            static const string RDKSHELL_METHOD_GET_SCALE;
            static const string RDKSHELL_METHOD_SET_SCALE;
            static const string RDKSHELL_METHOD_GET_HOLE_PUNCH;
            static const string RDKSHELL_METHOD_SET_HOLE_PUNCH;
            static const string RDKSHELL_METHOD_GET_LOG_LEVEL;
            static const string RDKSHELL_METHOD_SET_LOG_LEVEL;
            static const string RDKSHELL_METHOD_HIDE_SPLASH_LOGO;
            static const string RDKSHELL_METHOD_ADD_ANIMATION;
            static const string RDKSHELL_METHOD_REMOVE_ANIMATION;
            static const string RDKSHELL_METHOD_ENABLE_INACTIVITY_REPORTING;
            static const string RDKSHELL_METHOD_SET_INACTIVITY_INTERVAL;
            static const string RDKSHELL_METHOD_SCALE_TO_FIT;
            static const string RDKSHELL_METHOD_LAUNCH;
            static const string RDKSHELL_METHOD_LAUNCH_APP;
            static const string RDKSHELL_METHOD_SUSPEND;
            static const string RDKSHELL_METHOD_SUSPEND_APP;
            static const string RDKSHELL_METHOD_RESUME_APP;
            static const string RDKSHELL_METHOD_DESTROY;
            static const string RDKSHELL_METHOD_GET_AVAILABLE_TYPES;
            static const string RDKSHELL_METHOD_GET_STATE;
            static const string RDKSHELL_METHOD_GET_SYSTEM_MEMORY;
            static const string RDKSHELL_METHOD_GET_SYSTEM_RESOURCE_INFO;
            static const string RDKSHELL_METHOD_SET_MEMORY_MONITOR;
            static const string RDKSHELL_METHOD_SHOW_WATERMARK;
            static const string RDKSHELL_METHOD_SHOW_FULL_SCREEN_IMAGE;
            static const string RDKSHELL_METHOD_HIDE_FULL_SCREEN_IMAGE;
            static const string RDKSHELL_METHOD_LAUNCH_RESIDENT_APP;
            static const string RDKSHELL_METHOD_GET_KEYREPEATS_ENABLED;
            static const string RDKSHELL_METHOD_ENABLE_KEYREPEATS;
            static const string RDKSHELL_METHOD_SET_TOPMOST;
            static const string RDKSHELL_METHOD_GET_VIRTUAL_RESOLUTION;
            static const string RDKSHELL_METHOD_SET_VIRTUAL_RESOLUTION;
            static const string RDKSHELL_METHOD_ENABLE_VIRTUAL_DISPLAY;
            static const string RDKSHELL_METHOD_GET_VIRTUAL_DISPLAY_ENABLED;

            // events
            static const string RDKSHELL_EVENT_ON_USER_INACTIVITY;
            static const string RDKSHELL_EVENT_ON_APP_LAUNCHED;
            static const string RDKSHELL_EVENT_ON_APP_CONNECTED;
            static const string RDKSHELL_EVENT_ON_APP_DISCONNECTED;
            static const string RDKSHELL_EVENT_ON_APP_TERMINATED;
            static const string RDKSHELL_EVENT_ON_APP_FIRST_FRAME;
            static const string RDKSHELL_EVENT_ON_APP_SUSPENDED;
            static const string RDKSHELL_EVENT_ON_APP_RESUMED;
            static const string RDKSHELL_EVENT_ON_APP_ACTIVATED;
            static const string RDKSHELL_EVENT_ON_LAUNCHED;
            static const string RDKSHELL_EVENT_ON_SUSPENDED;
            static const string RDKSHELL_EVENT_ON_DESTROYED;
            static const string RDKSHELL_EVENT_DEVICE_LOW_RAM_WARNING;
            static const string RDKSHELL_EVENT_DEVICE_CRITICALLY_LOW_RAM_WARNING;
            static const string RDKSHELL_EVENT_DEVICE_LOW_RAM_WARNING_CLEARED;
            static const string RDKSHELL_EVENT_DEVICE_CRITICALLY_LOW_RAM_WARNING_CLEARED;
            static const string RDKSHELL_EVENT_ON_EASTER_EGG;
            static const string RDKSHELL_EVENT_ON_WILL_DESTROY;

            void notify(const std::string& event, const JsonObject& parameters);
            void pluginEventHandler(const JsonObject& parameters);
            void startEventTimer();
            void stopEventTimer();
            void onEventTimer();

        private/*registered methods (wrappers)*/:

            //methods ("parameters" here is "params" from the curl request)
            uint32_t moveToFrontWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t moveToBackWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t moveBehindWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setFocusWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t killWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t addKeyInterceptWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t removeKeyInterceptWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t addKeyListenersWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t removeKeyListenersWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t addKeyMetadataListenerWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t removeKeyMetadataListenerWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t injectKeyWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t generateKeyWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getScreenResolutionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setScreenResolutionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t createDisplayWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getClientsWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getZOrderWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getBoundsWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setBoundsWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getVisibilityWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setVisibilityWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getOpacityWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setOpacityWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getScaleWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setScaleWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getHolePunchWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setHolePunchWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getLogLevelWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setLogLevelWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t hideSplashLogoWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t addAnimationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t removeAnimationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t enableInactivityReportingWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setInactivityIntervalWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t scaleToFitWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t launchWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t suspendWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t destroyWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t launchApplicationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t suspendApplicationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t resumeApplicationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t destroyApplicationWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getAvailableTypesWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getState(const JsonObject& parameters, JsonObject& response);
            uint32_t getSystemMemoryWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getSystemResourceInfoWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setMemoryMonitorWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t showWatermarkWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t showFullScreenImageWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t hideFullScreenImageWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t launchResidentAppWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getKeyRepeatsEnabledWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t enableKeyRepeatsWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setTopmostWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getVirtualResolutionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t setVirtualResolutionWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t enableVirtualDisplayWrapper(const JsonObject& parameters, JsonObject& response);
            uint32_t getVirtualDisplayEnabledWrapper(const JsonObject& parameters, JsonObject& response);

        private/*internal methods*/:
            RDKShell(const RDKShell&) = delete;
            RDKShell& operator=(const RDKShell&) = delete;

            bool moveToFront(const string& client);
            bool moveToBack(const string& client);
            bool moveBehind(const string& client, const string& target);
            bool setFocus(const string& client);
            bool kill(const string& client);
            bool addKeyIntercept(const uint32_t& keyCode, const JsonArray& modifiers, const string& client);
            bool removeKeyIntercept(const uint32_t& keyCode, const JsonArray& modifiers, const string& client);
            bool addKeyListeners(const string& client, const JsonArray& listeners);
            bool removeKeyListeners(const string& client, const JsonArray& listeners);
            bool addAnyKeyListener(const string& client, const JsonArray& listeners);
            bool injectKey(const uint32_t& keyCode, const JsonArray& modifiers);
            bool generateKey(const string& client, const JsonArray& keyInputs);
            bool getScreenResolution(JsonObject& out);
            bool setScreenResolution(const unsigned int w, const unsigned int h);
            bool setMimeType(const string& client, const string& mimeType);
            bool getMimeType(const string& client, string& mimeType);
            bool createDisplay(const string& client, const string& displayName, const uint32_t displayWidth = 0, const uint32_t displayHeight = 0,
                const bool virtualDisplay = false, const uint32_t virtualWidth = 0, const uint32_t virtualHeight = 0);
            bool getClients(JsonArray& clients);
            bool getZOrder(JsonArray& clients);
            bool getBounds(const string& client, JsonObject& bounds);
            bool setBounds(const string& client, const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h);
            bool getVisibility(const string& client, bool& visibility);
            bool setVisibility(const string& client, const bool visible);
            bool getOpacity(const string& client, unsigned int& opacity);
            bool setOpacity(const string& client, const unsigned int opacity);
            bool getScale(const string& client, double& scaleX, double& scaleY);
            bool setScale(const string& client, const double scaleX, const double scaleY);
            bool getHolePunch(const string& client, bool& holePunch);
            bool setHolePunch(const string& client, const bool holePunch);
            bool removeAnimation(const string& client);
            bool addAnimationList(const JsonArray& animations);
            bool enableInactivityReporting(const bool enable);
            bool setInactivityInterval(const uint32_t interval);
            void onLaunched(const std::string& client, const string& launchType);
            void onSuspended(const std::string& client);
            void onDestroyed(const std::string& client);
            bool systemMemory(uint32_t &freeKb, uint32_t & totalKb, uint32_t & usedSwapKb);
            bool pluginMemoryUsage(const string callsign, JsonArray& memoryInfo);
            bool showWatermark(const bool enable);
            bool showFullScreenImage(std::string& path);
            void killAllApps();
            bool enableKeyRepeats(const bool enable);
            bool getKeyRepeatsEnabled(bool& enable);
            bool setTopmost(const string& callsign, const bool topmost);
            bool getVirtualResolution(const std::string& client, uint32_t &virtualWidth, uint32_t &virtualHeight);
            bool setVirtualResolution(const std::string& client, const uint32_t virtualWidth, const uint32_t virtualHeight);
            bool enableVirtualDisplay(const std::string& client, const bool enable);
            bool getVirtualDisplayEnabled(const std::string& client, bool &enabled);
            void loadStartupConfig();
            void invokeStartupThunderApis();

            static std::shared_ptr<WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement> > getThunderControllerClient(std::string callsign="");
            static std::shared_ptr<WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement> > getPackagerPlugin();
            static std::shared_ptr<WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement> > getOCIContainerPlugin();

        private/*classes */:

            class RdkShellListener :  public RdkShell::RdkShellEventListener {

              public:
                RdkShellListener(RDKShell* shell)
                    : mShell(*shell)
                {
                }

                ~RdkShellListener()
                {
                }

                // rdkshell events listeners
                virtual void onApplicationLaunched(const std::string& client);
                virtual void onApplicationConnected(const std::string& client);
                virtual void onApplicationDisconnected(const std::string& client);
                virtual void onApplicationTerminated(const std::string& client);
                virtual void onApplicationFirstFrame(const std::string& client);
                virtual void onApplicationSuspended(const std::string& client);
                virtual void onApplicationResumed(const std::string& client);
                virtual void onApplicationActivated(const std::string& client);
                virtual void onUserInactive(const double minutes);
                virtual void onDeviceLowRamWarning(const int32_t freeKb);
                virtual void onDeviceCriticallyLowRamWarning(const int32_t freeKb);
                virtual void onDeviceLowRamWarningCleared(const int32_t freeKb);
                virtual void onDeviceCriticallyLowRamWarningCleared(const int32_t freeKb);
                virtual void onEasterEgg(const std::string& name, const std::string& actionJson);
                virtual void onPowerKey();

              private:
                  RDKShell& mShell;
            };

            class MonitorClients : public PluginHost::IPlugin::INotification {
              private:
                  MonitorClients() = delete;
                  MonitorClients(const MonitorClients&) = delete;
                  MonitorClients& operator=(const MonitorClients&) = delete;

              public:
                  MonitorClients(RDKShell* shell)
                      : mShell(*shell)
                  {
                      ASSERT(mShell != nullptr);
                  }
                  ~MonitorClients()
                  {
                  }

              public:
                  BEGIN_INTERFACE_MAP(MonitorClients)
                  INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                  END_INTERFACE_MAP

              private:
                  virtual void StateChange(PluginHost::IShell* shell);

              private:
                  RDKShell& mShell;
            };

        private/*members*/:
            bool mRemoteShell;
            bool mEnableUserInactivityNotification;
            MonitorClients* mClientsMonitor;
            std::shared_ptr<RdkShell::RdkShellEventListener> mEventListener;
            PluginHost::IShell* mCurrentService;
            EventTimer mEventTimer;
            //std::mutex m_callMutex;
        };

        struct PluginData
        {
          std::string mClassName;
        };

        class PluginStateChangeData
        {
           public:
                PluginStateChangeData(std::string callsign, std::shared_ptr<WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement>> pluginConnection, RDKShell* rdkshell);
                ~PluginStateChangeData();
                void onStateChangeEvent(const JsonObject& params);
                void enableLaunch(bool enable);

           private:
                std::string mCallSign;
                std::shared_ptr<WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement>> mPluginConnection;
                RDKShell& mRDKShell;
                bool mLaunchEnabled;
        };

    } // namespace Plugin
} // namespace WPEFramework

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Bluetooth.h"

#include "ServiceMock.h"
#include "FactoriesImplementation.h"

using namespace WPEFramework;

using ::testing::NiceMock;

class BluetoothTest : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::Bluetooth> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Connection connection;
    string response;

    BluetoothTest()
        : plugin(Core::ProxyType<Plugin::Bluetooth>::Create())
        , handler(*(plugin))
        , connection(1, 0)
    {
    }

    virtual ~BluetoothTest() override {}
};

class BluetoothInitializedTest : public BluetoothTest {
protected:
    FactoriesImplementation factoriesImplementation;
    PluginHost::IDispatcher* dispatcher;
    ServiceMock services;

    BluetoothInitializedTest()
        : BluetoothTest()
        , dispatcher(nullptr)
    {
        PluginHost::IFactories::Assign(&factoriesImplementation);
        dispatcher = static_cast<PluginHost::IDispatcher*>(
            plugin->QueryInterface(PluginHost::IDispatcher::ID));
        ASSERT_NE(dispatcher, nullptr);
        dispatcher->Activate(&services);
    }

    virtual ~BluetoothInitializedTest() override
    {
        dispatcher->Deactivate();
        dispatcher->Release();
        PluginHost::IFactories::Assign(nullptr);
    }
};

TEST_F(BluetoothTest, TestedAPIsShouldExist)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getApiVersionNumber")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("startScan")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("stopScan")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("isDiscoverable")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setDiscoverable")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getDiscoveredDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getPairedDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getConnectedDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("connect")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("disconnect")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setAudioStream")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("pair")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("unpair")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enable")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("disable")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getName")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setName")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("sendAudioPlaybackCommand")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("respondToEvent")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getDeviceInfo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getAudioInfo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getDeviceVolumeMuteInfo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setDeviceVolumeMuteInfo")));
}

TEST_F(BluetoothTest, GetApiVersionNumber)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getApiVersionNumber"), _T("{}"), response));
    // Response should contain version and success true
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"version\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, StartScan_RequiresParams)
{
    // Missing required params -> expect error
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("startScan"), _T("{}"), response));
}

TEST_F(BluetoothTest, StartScan_WithTimeoutOnly)
{
    // Provide only timeout, allowed by implementation (profile optional in this branch)
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startScan"), _T("{\"timeout\":5}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"status\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, StartScan_WithTimeoutAndProfile)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startScan"), _T("{\"timeout\":5, \"profile\":\"SMARTPHONE\"}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"status\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, StopScan)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("stopScan"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, IsDiscoverable)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("isDiscoverable"), _T("{}"), response));
    // discoverable is boolean, success true
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"discoverable\":(true|false)")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, SetDiscoverable_MissingParam)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setDiscoverable"), _T("{}"), response));
}

TEST_F(BluetoothTest, SetDiscoverable_WithParams)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setDiscoverable"), _T("{\"discoverable\":true,\"timeout\":10}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, GetDiscoveredDevices)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getDiscoveredDevices"), _T("{}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"discoveredDevices\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, GetPairedDevices)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getPairedDevices"), _T("{}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"pairedDevices\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, GetConnectedDevices)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getConnectedDevices"), _T("{}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"connectedDevices\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, Connect_Disconnect_RequireDeviceID)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("connect"), _T("{}"), response));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("disconnect"), _T("{}"), response));
}

TEST_F(BluetoothTest, Connect_WithDeviceID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("connect"), _T("{\"deviceID\":\"12345\",\"deviceType\":\"SMARTPHONE\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, Disconnect_WithDeviceID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("disconnect"), _T("{\"deviceID\":\"12345\",\"deviceType\":\"SMARTPHONE\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, SetAudioStream_RequiresParams)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setAudioStream"), _T("{}"), response));
}

TEST_F(BluetoothTest, SetAudioStream_WithParams)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setAudioStream"), _T("{\"deviceID\":\"12345\",\"audioStreamName\":\"PRIMARY\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, Pair_Unpair_RequireDeviceID)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("pair"), _T("{}"), response));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("unpair"), _T("{}"), response));
}

TEST_F(BluetoothTest, Pair_Unpair_Success)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("pair"), _T("{\"deviceID\":\"12345\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("unpair"), _T("{\"deviceID\":\"12345\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, Enable_Disable)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("enable"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("disable"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, GetName_SetName)
{
    // getName always succeeds and returns object with name
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getName"), _T("{}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"name\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));

    // setName with parameter
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setName"), _T("{\"name\":\"UnitTest BT\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, SendAudioPlaybackCommand_RequiresParams)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("sendAudioPlaybackCommand"), _T("{}"), response));
}

TEST_F(BluetoothTest, SendAudioPlaybackCommand_WithParams)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("sendAudioPlaybackCommand"), _T("{\"deviceID\":\"12345\",\"command\":\"PLAY\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, RespondToEvent_RequiresParams)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("respondToEvent"), _T("{}"), response));
}

TEST_F(BluetoothTest, RespondToEvent_WithParams)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("respondToEvent"), _T("{\"deviceID\":\"12345\",\"eventType\":\"onPairingRequest\",\"responseValue\":\"ACCEPTED\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothTest, GetDeviceInfo_RequiresDeviceID)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getDeviceInfo"), _T("{}"), response));
}

TEST_F(BluetoothTest, GetDeviceInfo_WithDeviceID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getDeviceInfo"), _T("{\"deviceID\":\"12345\"}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"deviceInfo\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, GetAudioInfo_RequiresDeviceID)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getAudioInfo"), _T("{}"), response));
}

TEST_F(BluetoothTest, GetAudioInfo_WithDeviceID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getAudioInfo"), _T("{\"deviceID\":\"12345\"}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"trackInfo\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
}

TEST_F(BluetoothTest, DeviceVolumeMuteInfo_RequireParams)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getDeviceVolumeMuteInfo"), _T("{}"), response));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setDeviceVolumeMuteInfo"), _T("{}"), response));
}

TEST_F(BluetoothTest, DeviceVolumeMuteInfo_WithParams)
{
    // get
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getDeviceVolumeMuteInfo"), _T("{\"deviceID\":\"12345\",\"deviceType\":\"HEADPHONES\"}"), response));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"volumeinfo\"")));
    EXPECT_THAT(response, ::testing::ContainsRegex(_T("\"success\":true")));
    // set
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setDeviceVolumeMuteInfo"), _T("{\"deviceID\":\"12345\",\"deviceType\":\"HEADPHONES\",\"volume\":200,\"mute\":0}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(BluetoothInitializedTest, EventNotificationsSmoke)
{
    // Subscribe a few events and ensure notify payloads serialize
    Core::JSONRPC::Message message;

    EXPECT_EQ(Core::ERROR_NONE, handler.Subscribe(0, _T("onStatusChanged"), _T("org.rdk.Bluetooth"), message));
    EXPECT_EQ(Core::ERROR_NONE, handler.Subscribe(0, _T("onPlaybackChange"), _T("org.rdk.Bluetooth"), message));
    EXPECT_EQ(Core::ERROR_NONE, handler.Subscribe(0, _T("onDiscoveredDevice"), _T("org.rdk.Bluetooth"), message));
    EXPECT_EQ(Core::ERROR_NONE, handler.Unsubscribe(0, _T("onStatusChanged"), _T("org.rdk.Bluetooth"), message));
    EXPECT_EQ(Core::ERROR_NONE, handler.Unsubscribe(0, _T("onPlaybackChange"), _T("org.rdk.Bluetooth"), message));
    EXPECT_EQ(Core::ERROR_NONE, handler.Unsubscribe(0, _T("onDiscoveredDevice"), _T("org.rdk.Bluetooth"), message));
}

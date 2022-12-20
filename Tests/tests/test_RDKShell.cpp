#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "RDKShell.h"
#include "rdkshell.h"
#include "rdkshellmock.h"
#include "ServiceMock.h"

using namespace WPEFramework;

class RDKShellTest : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::RDKShell> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Connection connection;
    Core::JSONRPC::Message message;
    RDKShellImplMock RDKShellmock;
    CompositorImplMock compositormock;
    RdkShellApiImplMock rdkshellapimock;
    string response;

    RDKShellTest()
        : plugin(Core::ProxyType<Plugin::RDKShell>::Create())
        , handler(*(plugin))
        , connection(1, 0)
        {
		RdkShell::CompositorController::getInstance().impl = &compositormock;
                RdkShell::RdkShellApi::getInstance().impl = &rdkshellapimock;
                RDKShell::getInstance().impl = &RDKShellmock;
        }
        virtual ~RDKShellTest()
	{
		RdkShell::CompositorController::getInstance().impl = nullptr;
                RdkShell::RdkShellApi::getInstance().impl = nullptr;
                RDKShell::getInstance().impl = nullptr;
	}
};
 

TEST_F(RDKShellTest, RegisteredMethods){
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addAnimation")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addKeyIntercept")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addKeyIntercepts")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addKeyListener")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addKeyMetadataListener")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("exitAgingMode")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("createDisplay")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("destroy")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableInactivityReporting")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableKeyRepeats")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableLogsFlushing")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableVirtualDisplay")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("generateKey")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getAvailableTypes")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getBounds")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getClients")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getCursorSize")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getHolePunch")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getKeyRepeatsEnabled")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getLastWakeupKey")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getLogsFlushingEnabled")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getOpacity")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getScale")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getScreenResolution")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getScreenshot")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getState")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getSystemMemory")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getSystemResourceInfo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getVirtualDisplayEnabled")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getVirtualResolution")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getVisibility")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getZOrder")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getGraphicsFrameRate")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("hideAllClients")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("hideCursor")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("hideFullScreenImage")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("hideSplashLogo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("ignoreKeyInputs")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("injectKey")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("kill")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("launch")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("launchApplication")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("launchResidentApp")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("moveBehind")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("moveToBack")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("moveToFront")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("removeAnimation")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("removeKeyIntercept")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("removeKeyListener")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("removeKeyMetadataListener")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("resetInactivityTime")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("resumeApplication")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("scaleToFit")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setBounds")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setCursorSize")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setFocus")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setHolePunch")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setInactivityInterval")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setLogLevel")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setMemoryMonitor")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setOpacity")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setScale")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setScreenResolution")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setTopmost")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setVirtualResolution")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setVisibility")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setGraphicsFrameRate")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("showCursor")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("showFullScreenImage")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("showSplashLogo")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("showWatermark")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("suspend")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("suspendApplication")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("keyRepeatConfig")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setAVBlocked")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getBlockedAVApplications")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("addEasterEggs")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("removeEasterEggs")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableEasterEggs")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getEasterEggs")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("launchFactoryApp")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("launchFactoryAppShortcut")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("enableInputEvents")));
    }
TEST_F(RDKShellTest, enableInputEvents)
{
   ON_CALL(compositormock, enableInputEvents(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, bool enable){
                      EXPECT_EQ(client, string("searchanddiscovery"));
                      EXPECT_EQ(enable, true);
                      return false;
                   }));

   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("enableInputEvents"), _T("{"
                                                                                "\"clients\": ["
                                                                                "    \"searchanddiscovery\""
                                                                                "],"
                                                                                "\"enable\": true"
                                                                                "}"), response));
   ON_CALL(compositormock, enableInputEvents(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, bool enable){
		      EXPECT_EQ(client, string("searchanddiscovery"));
		      EXPECT_EQ(enable, true);
                      return true;
                   }));

   EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("enableInputEvents"), _T("{"
                                                                                "\"clients\": ["
                                                                                "    \"searchanddiscovery\""
                                                                                "],"
                                                                                "\"enable\": true"
                                                                                "}"), response));
}

TEST_F(RDKShellTest, setMemoryMonitor)
{
    ON_CALL(amock, setMemoryMonitor(::testing::_))
                .WillByDefault(::testing::Invoke(
                [](std::map<std::string, RdkShell::RdkShellData> &configuration){
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setMemoryMonitor"), _T("{\"enable\": true,"
                                                                                             "\"interval\": 300,"
											     "\"lowRam\": 128,"
                                                                                             "\"criticallyLowRam\": 64}"), response));
}

TEST_F(RDKShellTest, getClients)
{
      ON_CALL(compositormock, getClients(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](std::vector<std::string>& clients){
                      clients.push_back("org.rdk.Netflix");
                      clients.push_back("org.rdk.RDKBrowser2");
                      clients.push_back("Test1");
		      clients.push_back("Test2");
                      return true;
                   }));

     EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getClients"), _T("{}"), response));
     EXPECT_EQ(response, string("{"
        "\"clients\":["
            "\"org.rdk.Netflix\","
	    "\"org.rdk.RDKBrowser2\","
	    "\"Test1\","
	    "\"Test2\""
        "],"
        "\"success\":true"
    "}"));	
}

TEST_F(RDKShellTest, getAvailableTypes)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getAvailableTypes"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"types\":[],\"success\":true}"));
}

TEST_F(RDKShellTest, getState)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getState"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"state\":[],\"success\":true}"));
}

TEST_F(RDKShellTest, keyRepeatConfig)
{
   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("keyRepeatConfig"), _T("{}"), response));
   ON_CALL(compositormock, setKeyRepeatConfig(::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](bool enabled, int32_t initialDelay, int32_t repeatInterval){
                      EXPECT_EQ(enabled, true);
		      EXPECT_EQ(initialDelay, 500);
		      EXPECT_EQ(repeatInterval, 250);
                      return true;
                }));


    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("keyRepeatConfig"), _T("{"
                                                                                      "\"enabled\":true,"
										      "\"initialDelay\":500,"
										      "\"repeatInterval\":250}"), response));
}

TEST_F(RDKShellTest, resetinactivity)
{
	ON_CALL(compositormock, resetInactivityTime())
            .WillByDefault(::testing::Invoke(
                [&](){
	          RdkShell::seconds();
                }));

        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("resetInactivityTime"), _T("{}"), response));
        EXPECT_EQ(response, _T("{\"success\":true}"));
}

TEST_F(RDKShellTest, launchApplication)
{
	XPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("launchApplication"), _T("{}"), response));
	ON_CALL(compositormock, launchApplication(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
		[](const std::string& client, const std::string& uri, const std::string& mimeType, bool topmost, bool focus){
                  EXPECT_EQ(client, string("testApp"));
		  EXPECT_EQ(uri, string("/usr/bin/westeros_test"));
		  EXPECT_EQ(mimeType, string("application/native"));
		  EXPECT_EQ(topmost, false);
		  EXPECT_EQ(focus, false);
		  return true;
		}));
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("launchApplication"), _T("{\"client\": \"testApp\","
                                                                                             "\"uri\": \"/usr/bin/westeros_test\","
                                                                                             "\"mimeType\": \"application/native\"}"), response));
	ON_CALL(compositormock, launchApplication(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
                .WillByDefault(::testing::Invoke(
                [](const std::string& client, const std::string& uri, const std::string& mimeType, bool topmost, bool focus){
                  return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("launchApplication"), _T("{\"client\": \"testApp\","
                                                                                             "\"uri\": \"/usr/bin/westeros_test\","
                                                                                             "\"mimeType\": \"application/native\"}"), response));
}

TEST_F(RDKShellTest, suspendApplication)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("suspendApplication"), _T("{}"), response));
	ON_CALL(compositormock, suspendApplication(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("HtmlApp"));
                      return true;
                }));
	ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
                      EXPECT_EQ(client, string("HtmlApp"));
		      EXPECT_EQ(visible, false);
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("suspendApplication"), _T("{\"client\": \"HtmlApp\"}"), response));
	ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
                      return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("suspendApplication"), _T("{\"client\": \"HtmlApp\"}"), response));
}

TEST_F(RDKShellTest, resumeApplication)
{
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("resumeApplication"), _T("{}"), response));
        ON_CALL(compositormock, resumeApplication(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("HtmlApp"));
                      return true;
                }));
        ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
                      EXPECT_EQ(client, string("HtmlApp"));
                      EXPECT_EQ(visible, true);
                      return true;
                }));
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("resumeApplication"), _T("{\"client\": \"HtmlApp\"}"), response));
        ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
                      return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("resumeApplication"), _T("{\"client\": \"HtmlApp\"}"), response));
}


TEST_F(RDKShellTest, GraphicsFrameRate)
{
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getGraphicsFrameRate"), _T("{}"), response));
	EXPECT_EQ(response, _T("{\"framerate\":40,\"success\":true}")); 

	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setGraphicsFrameRate"), _T("{}"), response));
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setGraphicsFrameRate"), _T("{\"framerate\":60}"), response));
        EXPECT_EQ(response, _T("{\"success\":true}"));

	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getGraphicsFrameRate"), _T("{}"), response));
	EXPECT_EQ(response, _T("{\"framerate\":60,\"success\":true}"));
}

TEST_F(RDKShellTest, FullScreenImage)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("showFullScreenImage"), _T("{}"), response));

	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("showFullScreenImage"), _T("{\"path\":\"tmp\netflix.png\"}"), response));
	EXPECT_EQ(response, _T("{\"success\":true}"));
       
        ON_CALL(compositormock, hideFullScreenImage())
        .WillByDefault(::testing::Return(true));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("hideFullScreenImage"), _T("{}"), response));
        EXPECT_EQ(response, _T("{\"success\":true}"));

	ON_CALL(compositormock, hideFullScreenImage())
        .WillByDefault(::testing::Return(false));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("hideFullScreenImage"), _T("{}"), response));
}


TEST_F(RDKShellTest, visibility)
{
    ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
	    .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
          		EXPECT_EQ(client, string("org.rdk.Netflix"));
			EXPECT_EQ(visible, true);
                      return true;
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setVisibility"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\","
                                                                                             "\"visible\": true}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(false));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setVisibility"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\","
                                                                                             "\"visible\": true}"), response));
    ON_CALL(compositormock, getVisibility(::testing::_, ::testing::_))
	    .WillByDefault(::testing::Invoke(
                [](const std::string& client, bool& visible){
                      bool x = true;
		      visible = x;
                      return true;
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getVisibility"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
    EXPECT_EQ(response, string("{\"visible\":true,\"success\":true}"));
}

TEST_F(RDKShellTest, getSystemResource)
{
   EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getSystemResourceInfo"), _T("{}"), response));
}
TEST_F(RDKShellTest, getSystemMemory)
{
    EXPECT_CALL(rdkshellapimock, systemRam(::testing::_, ::testing::_, ::testing::_, ::testing::_))
	    .Times(1)
            .WillOnce(::testing::Invoke(
                [&](uint32_t& freeKb, uint32_t& totalKb, uint32_t& availableKb, uint32_t& usedSwapKb) {
		freeKb = 994056;
		totalKb = 2830092;
		usedSwapKb = 0;
		availableKb = 764628;
                return true;
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getSystemMemory"), _T("{}"), response));
    EXPECT_EQ(response, string("{"
                                "\"freeRam\":994056,"
                                "\"swapRam\":0,"
                                "\"totalRam\":2830092,"
				"\"availablememory\":764628,"
                                "\"success\":true"
                        "}"));
}

TEST_F(RDKShellTest, showWaterMark)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("showWatermark"), _T("{\"show\":true}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
    ON_CALL(compositormock, hideWatermark())
            .WillByDefault(::testing::Invoke(
                [](){
                      return true;
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("showWatermark"), _T("{\"show\":false}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
}


TEST_F(RDKShellTest, Bounds)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setBounds"), _T("{}"), response));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getBounds"), _T("{}"), response));
    ON_CALL(compositormock, setBounds(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
	    .WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height){
		      EXPECT_EQ(client, string("org.rdk.Netflix"));
		      EXPECT_EQ(x, 0);
		      EXPECT_EQ(y, 0);
                      return true;
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setBounds"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\",\"x\":0,\"y\":0,\"w\":1920,\"h\":1080}"),response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
    ON_CALL(compositormock, getBounds(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height){
		      x = 0;
                      y = 0;
		      width = 1920;
		      height = 1080;
                      return true;
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getBounds"), _T("{\"client\": \"org.rdk.Netflix\","
				    "\"callsign\": \"org.rdk.Netflix\"}"), response));
    EXPECT_EQ(response, string("{"
                            "\"bounds\":{"
                                "\"x\":0,"
                                "\"y\":0,"
                                "\"w\":1920,"
                                "\"h\":1080"
                            "},"
                           "\"success\":true"
                        "}"));
}


TEST_F(RDKShellTest, CursorSize)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setCursorSize"), _T("{}"), response));
    ON_CALL(compositormock, setCursorSize(::testing::_, ::testing::_))
	    .WillByDefault(::testing::Invoke(
                [](uint32_t width, uint32_t height){
                      EXPECT_EQ(width, 255);
                      EXPECT_EQ(height, 255);
                      return true;
                }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setCursorSize"), _T("{\"width\":255,\"height\":255}"),response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    ON_CALL(compositormock, getCursorSize(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](uint32_t& width, uint32_t& height){
                      width = 255;
                      height = 255;
                      return true;
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getCursorSize"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"width\":255,\"height\":255,\"success\":true}"));
}

TEST_F(RDKShellTest, showandhideCursor)
{
    ON_CALL(compositormock, showCursor())
         .WillByDefault(::testing::Return(true));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("showCursor"), _T("{}"),response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
    ON_CALL(compositormock, hideCursor())
         .WillByDefault(::testing::Return(true));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("hideCursor"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
}

TEST_F(RDKShellTest, splashLogo)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("showSplashLogo"), _T("{}"),response));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("showSplashLogo"), _T("{\"displayTime\": 5}"),response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
    ON_CALL(compositormock, hideSplashScreen())
         .WillByDefault(::testing::Return(true));
    ON_CALL(RDKShellmock, logMilestone(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const char *msg_code){
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("hideSplashLogo"), _T("{}"),response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
    ON_CALL(compositormock, hideSplashScreen())
         .WillByDefault(::testing::Return(false));
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("hideSplashLogo"), _T("{}"),response));
}



TEST_F(RDKShellTest, LogLevel)
{
   ON_CALL(compositormock, setLogLevel(::testing::_))
	    .WillByDefault(::testing::Invoke(
                [](const std::string level){
		EXPECT_EQ(level, string("DEBUG"));
		return true;
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setLogLevel"), _T("{\"logLevel\": \"DEBUG\"}"), response));

    ON_CALL(compositormock, setLogLevel(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string level){
                EXPECT_EQ(level, string("DEBUG"));
                return false;
                }));

    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setLogLevel"), _T("{\"logLevel\": \"DEBUG\"}"), response));

    ON_CALL(compositormock, getLogLevel(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](std::string& level){
		      std::string a = "DEBUG";
		      level = a;
                      return true;
                }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getLogLevel"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"logLevel\":\"DEBUG\",\"success\":true}"));  
}



TEST_F(RDKShellTest, keyRepeat)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("enableKeyRepeats"), _T("{}"), response));
	ON_CALL(compositormock, enableKeyRepeats(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](bool enable){
                EXPECT_EQ(enable, true);
                return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("enableKeyRepeats"), _T("{\"enable\": \"true\"}"), response));
	EXPECT_EQ(response, _T("{\"success\":true}"));
	ON_CALL(compositormock, getKeyRepeatsEnabled(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](bool& enable){
                      enable = true;
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getKeyRepeatsEnabled"), _T("{}"), response));
        EXPECT_EQ(response, _T("{\"keyRepeat\":true,\"success\":true}"));
}

TEST_F(RDKShellTest, ScreenResolution)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setScreenResolution"), _T("{}"), response));
        ON_CALL(compositormock, setScreenResolution(::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const uint32_t width, const uint32_t height){
                      EXPECT_EQ(width, 1920);
                      EXPECT_EQ(height, 1080);
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setScreenResolution"), _T("{\"w\":1920,\"h\":1080}"), response));
	EXPECT_EQ(response, _T("{\"success\":true}"));
        ON_CALL(compositormock, getScreenResolution(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](uint32_t &width, uint32_t &height){
                      width = 1920;
                      height = 1080;
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getScreenResolution"), _T("{}"), response));
	EXPECT_EQ(response, _T("{\"w\":1920,\"h\":1080,\"success\":true}"));
}

TEST_F(RDKShellTest, VirtualResolution)
{
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getVirtualResolution"), _T("{}"), response));
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setVirtualResolution"), _T("{}"), response));
	ON_CALL(compositormock, setVirtualResolution(::testing::_, ::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t virtualWidth, const uint32_t virtualHeight){
		      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      EXPECT_EQ(virtualWidth, 1920);
                      EXPECT_EQ(virtualHeight, 1080);
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setVirtualResolution"), _T("{\"client\":\"org.rdk.Netflix\",\"width\":1920,\"height\":1080}"), response));
        EXPECT_EQ(response, _T("{\"success\":true}"));
        ON_CALL(compositormock, getVirtualResolution(::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, uint32_t &virtualWidth, uint32_t &virtualHeight){
                      virtualWidth = 1920;
                      virtualHeight = 1080;
                      return true;
                }));

        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getVirtualResolution"), _T("{\"client\":\"org.rdk.Netflix\"}"), response));
        EXPECT_EQ(response, _T("{\"width\":1920,\"height\":1080,\"success\":true}"));
}

TEST_F(RDKShellTest, getBlockedAVApplications)
{
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getBlockedAVApplications"), _T("{}"), response));
	EXPECT_EQ(response, _T("{\"message\":\"ERM not enabled\",\"success\":true}"));
	ServiceMock service;
	EXPECT_EQ(string(""), plugin->Initialize(&service));
	ON_CALL(compositormock, getBlockedAVApplications(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](std::vector<std::string>& apps){
                      apps.push_back("app1,");
                      return true;
                  }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getBlockedAVApplications"), _T("{}"), response));
	EXPECT_EQ(response, _T("{\"getBlockedAVApplications\":[\"app1\"],\"success\":true}"));
	plugin->Deinitialize(&service);
}


TEST_F(RDKShellTest, ScaleToFit)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("scaleToFit"), _T("{}"), response));
	ON_CALL(compositormock, scaleToFit(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const int32_t x, const int32_t y, const uint32_t width, const uint32_t height){
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
		    EXPECT_EQ(x, 0);
		    EXPECT_EQ(y, 0);
		    EXPECT_EQ(width, 1920);
		    EXPECT_EQ(height, 1080);
		    return true;
                }));
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("scaleToFit"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\","
                                                                                             "\"x\":0,\"y\":0,\"w\":1920,\"h\":1080}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

	ON_CALL(compositormock, scaleToFit(::testing::_, ::testing::_, ::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const int32_t x, const int32_t y, const uint32_t width, const uint32_t height){
		    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    EXPECT_EQ(x, 0);
                    EXPECT_EQ(y, 0);
                    EXPECT_EQ(width, 1920);
                    EXPECT_EQ(height, 1080);
                    return false;
                }));
	EXPECT_EQ(handler.Invoke(connection, _T("scaleToFit"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\","
                                                                                             "\"x\":0,\"y\":0,\"w\":1920,\"h\":1080}"), response), Core::ERROR_GENERAL);

}


TEST_F(RDKShellTest, hideAllClients)
{
	ON_CALL(compositormock, getClients(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](std::vector<std::string>& clients){
                      clients.push_back("org.rdk.Netflix");
                      clients.push_back("org.rdk.RDKBrowser2");
                      clients.push_back("Test1");
                      clients.push_back("Test2");
                      return true;
                   }));
	ON_CALL(compositormock, setVisibility(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, const bool visible){
                      EXPECT_EQ(visible, false);
                      return true;
                }));

	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("hideAllClients"), _T("{\"hide\":true}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(RDKShellTest, ignoreKeyInputs)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("ignoreKeyInputs"), _T("{}"), response));
        ON_CALL(compositormock, ignoreKeyInputs(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](bool ignore){
                      EXPECT_EQ(ignore, false);
                      return true;
                }));        
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("ignoreKeyInputs"), _T("{\"ignore\":false}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

        ON_CALL(compositormock, ignoreKeyInputs(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](bool ignore){
                      EXPECT_EQ(ignore, false);
                      return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("ignoreKeyInputs"), _T("{\"ignore\":false}"), response));
}


TEST_F(RDKShellTest, moveToFront)
{
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveToFront"), _T("{}"), response));

        ON_CALL(compositormock, moveToFront(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("moveToFront"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

	ON_CALL(compositormock, moveToFront(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveToFront"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
}

TEST_F(RDKShellTest, moveToBack)
{
      EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveToBack"), _T("{}"), response));

        ON_CALL(compositormock, moveToBack(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    return true;
                }));

        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("moveToBack"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

        ON_CALL(compositormock, moveToBack(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    return false;
                }));

        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveToBack"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
}

TEST_F(RDKShellTest, moveBehind)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveBehind"), _T("{}"), response));

	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveBehind"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));

	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveBehind"), _T("{\"target\": \"org.rdk.RDKBrowser2\"}"), response));


	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getClients"), _T("{}"), response));
	EXPECT_EQ(response, string("{"
        "\"clients\":["
            "\"org.rdk.Netflix\","
            "\"org.rdk.RDKBrowser2\","
            "\"Test2\""
        "],"
        "\"success\":true"
        "}"));


       	ON_CALL(compositormock, moveBehind(::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const std::string& target) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
		    EXPECT_EQ(target, string("org.rdk.RDKBrowser2"));
                    return true;
                }));
        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("moveBehind"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"target\": \"org.rdk.RDKBrowser2\"}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

        ON_CALL(compositormock, moveBehind(::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const std::string& target) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
                    EXPECT_EQ(target, string("org.rdk.RDKBrowser2"));
                    return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("moveBehind"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                         "\"target\": \"org.rdk.RDKBrowser2\"}"), response));

}

TEST_F(RDKShellTest, Opacity)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getOpacity"), _T("{}"), response));

	ON_CALL(compositormock, getOpacity(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, unsigned int& opacity) {
                    opacity = 100;
                    return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getOpacity"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));
	EXPECT_EQ(response, string("{\"opacity\":100,\"success\":true}"));

	ON_CALL(compositormock, getOpacity(::testing::_, ::testing::_))
                .WillByDefault(::testing::Return(false));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getOpacity"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));

	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setOpacity"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));

        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setOpacity"), _T("{\"opacity\": 100}"), response));

	ON_CALL(compositormock, setOpacity(::testing::_, ::testing::_))
               .WillByDefault(::testing::Invoke(
                [](const std::string& client, const unsigned int opacity) {
                    EXPECT_EQ(client, string("org.rdk.Netflix"));
		    EXPECT_EQ(opacity, 100);
                    return true;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setOpacity"), _T("{\"client\": \"org.rdk.Netflix\","
					                                             "\"opacity\": 100}"), response));

        ON_CALL(compositormock, setOpacity(::testing::_, ::testing::_))
                .WillByDefault(::testing::Return(false));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setOpacity"), _T("{\"client\": \"org.rdk.Netflix\","
					                                               "\"opacity\": 100}"), response));
}

TEST_F(RDKShellTest, kill)
{
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("kill"), _T("{}"), response));
}

TEST_F(RDKShellTest, setFocus)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setFocus"), _T("{}"), response));

        ON_CALL(compositormock, setFocus(::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      return true;
                }));


        EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setFocus"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));

	ON_CALL(compositormock, setFocus(::testing::_))
	      .WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      return false;
                }));

        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setFocus"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));
}

TEST_F(RDKShellTest, removeKeyIntercepts)
{
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyIntercept"), _T("{}"), response));
        ON_CALL(compositormock, removeKeyIntercept(::testing::_, ::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t& keyCode, const uint32_t& flags){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      EXPECT_EQ(keyCode, 10);
                      EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("removeKeyIntercept"), _T("{"
                                                                                "\"keyCode\": 10,"
                                                                                "\"modifiers\": ["
                                                                                "    \"shift\""
                                                                                "],"
                                                                                "\"client\": \"org.rdk.Netflix\","
                                                                                "\"callsign\": \"org.rdk.Netflix\""
                                                                                "}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));
        ON_CALL(compositormock, removeKeyIntercept(::testing::_, ::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t& keyCode, const uint32_t& flags){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      EXPECT_EQ(keyCode, 10);
                      EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                      return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyIntercept"), _T("{"
                                                                                "\"keyCode\": 10,"
                                                                                "\"modifiers\": ["
                                                                                "    \"shift\""
                                                                                "],"
                                                                                "\"client\": \"org.rdk.Netflix\","
                                                                                "\"callsign\": \"org.rdk.Netflix\""
                                                                                "}"), response));
}

TEST_F(RDKShellTest, removeKeyListeners)
{
	EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyListener"), _T("{}"), response));
        ON_CALL(compositormock, removeKeyListener(::testing::_, ::testing::_, ::testing::_))
		.WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t& keyCode, const uint32_t& flags){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
		      EXPECT_EQ(keyCode, 10);
		      EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                      return true;
                }));
	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("removeKeyListener"), _T("{"
                                                                                "\"client\": \"org.rdk.Netflix\","
                                                                                "\"callsign\": \"org.rdk.Netflix\","
                                                                                "\"keys\": ["
                                                                                     "{"
                                                                                           "\"keyCode\": 10,"
											   "\"nativekeyCode\": 10,"
                                                                                           "\"modifiers\": ["
                                                                                                "\"shift\""
                                                                                           "],"
											   "\"activate\": false,"
                                                                                           "\"propagate\": true"
                                                                                     "}"
                                                                                         "]"
                                                                                    "}"), response));
	EXPECT_EQ(response, string("{\"success\":true}"));
        ON_CALL(compositormock, removeKeyListener(::testing::_, ::testing::_, ::testing::_))
              .WillByDefault(::testing::Invoke(
                [](const std::string& client, const uint32_t& keyCode, const uint32_t& flags){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      EXPECT_EQ(keyCode, 10);
                      EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                      return false;
                }));
        EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyListener"), _T("{"
                                                                                "\"client\": \"org.rdk.Netflix\","
                                                                                "\"callsign\": \"org.rdk.Netflix\","
                                                                                "\"keys\": ["
                                                                                     "{"
                                                                                           "\"keyCode\": 10,"
                                                                                           "\"nativekeyCode\": 10,"
                                                                                           "\"modifiers\": ["
                                                                                                "\"shift\""
                                                                                           "],"
                                                                                           "\"activate\": false,"
                                                                                           "\"propagate\": true"
                                                                                     "}"
                                                                                         "]"
                                                                                    "}"), response));

}

TEST_F(RDKShellTest, removeKeyMetadataListener)
{
   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyMetadataListener"), _T("{}"), response));
   ON_CALL(compositormock, removeKeyMetadataListener(::testing::_))
           .WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      return true;
                }));

   EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("removeKeyMetadataListener"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));
   EXPECT_EQ(response, string("{\"success\":true}"));

   ON_CALL(compositormock, removeKeyMetadataListener(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client){
                      EXPECT_EQ(client, string("org.rdk.Netflix"));
                      return false;
                }));
   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("removeKeyMetadataListener"), _T("{\"client\": \"org.rdk.Netflix\"}"), response));

}

TEST_F(RDKShellTest, injectKey)
{
   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("injectKey"), _T("{}"), response));
   ON_CALL(compositormock, injectKey(::testing::_, ::testing::_))
                .WillByDefault(::testing::Invoke(
		[](const uint32_t& keyCode, const uint32_t& flags) {
                EXPECT_EQ(keyCode, 10);
                EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                return true;
            }));
   EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("injectKey"), _T("{\"keyCode\": 10, \"modifiers\": [\"shift\"]}"), response));

   ON_CALL(compositormock, injectKey(::testing::_, ::testing::_))
	   .WillByDefault(::testing::Invoke(
                [](const uint32_t& keyCode, const uint32_t& flags) {
                EXPECT_EQ(keyCode, 10);
                EXPECT_EQ(flags, RDKSHELL_FLAGS_SHIFT);
                return false;
		}));
   EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("injectKey"), _T("{\"keyCode\": 10, \"modifiers\": [\"shift\"]}"), response));   
}

TEST_F(RDKShellTest, getZOrder)
{
    ON_CALL(compositormock, getZOrder(::testing::_))
            .WillByDefault(::testing::Invoke(
                [](std::vector<std::string>& clients){
                      clients.push_back("org.rdk.Netflix");
                      clients.push_back("org.rdk.RDKBrowser2");
		      clients.push_back("Test1");
		      clients.push_back("Test2");
                      return true;
                  }));

	EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getZOrder"), _T("{}"), response));
        EXPECT_EQ(response, string("{"
        "\"clients\":["
            "\"org.rdk.Netflix\","
            "\"org.rdk.RDKBrowser2\","
	    "\"Test1\","
            "\"Test2\""
        "],"
        "\"success\":true"
    "}"));
}

TEST_F(RDKShellTest, HolePunch)
{
  ON_CALL(compositormock, setHolePunch(::testing::_, ::testing::_))
                .WillByDefault(::testing::Invoke(
		[](const std::string& client, const bool holePunch){
		      EXPECT_EQ(client, string("org.rdk.Netflix"));
		      EXPECT_EQ(holePunch, true);
		      return true;
		}));
  EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setHolePunch"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\","
                                                                                             "\"holePunch\": true}"), response));
  EXPECT_EQ(response, string("{\"success\":true}"));

  ON_CALL(compositormock, getHolePunch(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, bool& holePunch){
                      holePunch = true;
                      return true;
                }));

  EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHolePunch"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
  EXPECT_EQ(response, string("{\"holePunch\":true,\"success\":true}"));


}

TEST_F(RDKShellTest, Scale)
{
  ON_CALL(compositormock, getScale(::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [](const std::string& client, double &scaleX, double &scaleY){
                      scaleX = 1.0;
		      scaleY = 1.0;
                      return true;
                }));

  EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getScale"), _T("{\"client\": \"org.rdk.Netflix\","
                                                                                             "\"callsign\": \"org.rdk.Netflix\"}"), response));
}

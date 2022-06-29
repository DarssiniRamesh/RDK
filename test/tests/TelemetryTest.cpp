#include <gtest/gtest.h>

#include "Telemetry.h"
#include "ServiceMock.h"
#include "RfcApiMock.h"
#include "TelemetryMock.h"

#include "FactoriesImplementation.h"

using namespace WPEFramework;

class TelemetryTestFixture : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::Telemetry> plugin;
    Core::JSONRPC::Connection connection;
    RfcApiImplMock rfcApiImplMock;
    TelemetryApiImplMock telemetryApiImplMock;

    string response;
    ServiceMock service;
    Core::JSONRPC::Message message;
    FactoriesImplementation factoriesImplementation;

    TelemetryTestFixture()
        : connection(1, 0)
    {
        fprintf(stderr, "TelemetryTestFixture::TelemetryTestFixture()\n");
        PluginHost::IFactories::Assign(&factoriesImplementation);
    }
    virtual ~TelemetryTestFixture()
    {
        PluginHost::IFactories::Assign(nullptr);
    }

    virtual void SetUp()
    {
        RfcApi::getInstance().impl = &rfcApiImplMock;
        TelemetryApi::getInstance().impl = &telemetryApiImplMock;

        plugin = Core::ProxyType<Plugin::Telemetry>::Create();
    }

    virtual void TearDown()
    {
        RfcApi::getInstance().impl = nullptr;
        TelemetryApi::getInstance().impl = nullptr;
    }
};

TEST_F(TelemetryTestFixture, RegisteredMethods)
{
    Core::JSONRPC::Handler& handler(*(plugin));
    
    EXPECT_CALL(telemetryApiImplMock, t2_init(::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [](char *component) {
                return;
            }));

    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setReportProfileStatus")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("logApplicationEvent")));
}

TEST_F(TelemetryTestFixture, Plugin)
{
    fprintf(stderr, "TelemetryTestFixture Plugin start\n");
    
    Core::JSONRPC::Handler& handler(*(plugin));

    EXPECT_CALL(rfcApiImplMock, setRFCParameter(::testing::_, ::testing::_, ::testing::_, ::testing::_))
        .Times(2)
        .WillOnce(::testing::Invoke(
            [](char *pcCallerID, const char* pcParameterName, const char* pcParameterValue, DATA_TYPE eDataType) {

                return WDMP_SUCCESS;
            }))
        .WillOnce(::testing::Invoke(
            [](char *pcCallerID, const char* pcParameterName, const char* pcParameterValue, DATA_TYPE eDataType) {
                return WDMP_SUCCESS;
            }));

    EXPECT_CALL(telemetryApiImplMock, t2_init(::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [](char *component) {
                return;
            }));

    EXPECT_CALL(telemetryApiImplMock, t2_event_s(::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [](char* marker, char* value) {
                return T2ERROR_SUCCESS;
            }));

    // Initialize
    EXPECT_EQ(string(""), plugin->Initialize(nullptr));

    // JSON-RPC methods
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setReportProfileStatus"), _T("{\"status\":\"wrongvalue\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
    
    // JSON-RPC methods
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setReportProfileStatus"), _T("{\"status\":\"STARTED\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));


    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setReportProfileStatus"), _T("{\"status\":\"COMPLETE\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("logApplicationEvent"), _T("{\"eventName\":\"NAME\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("logApplicationEvent"), _T("{\"eventValue\":\"VALUE\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));

    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("logApplicationEvent"), _T("{\"eventName\":\"NAME\", \"eventValue\":\"VALUE\"}"), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    // Deinitialize
    plugin->Deinitialize(nullptr);
}

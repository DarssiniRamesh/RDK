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

#include <string>
#include "ControlSettings.h"

const char* PLUGIN_IARM_BUS_NAME = "Thunder_Plugins";

static char videoDescBuffer[VIDEO_DESCRIPTION_MAX*VIDEO_DESCRIPTION_NAME_SIZE] = {0};

#define registerMethod(...) Register(__VA_ARGS__);GetHandler(2)->Register<JsonObject, JsonObject>(__VA_ARGS__)

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(ControlSettings,1, 0);

    ControlSettings* ControlSettings::_instance = nullptr;

    static void tvVideoFormatChangeHandler(tvVideoHDRFormat_t format, void *userData)
    {
        printf("tvVideoFormatChangeHandler format:%d \n",format);
        ControlSettings *obj = (ControlSettings *)userData;
        if(obj)obj->NotifyVideoFormatChange(format);
    }

    static void tvVideoResolutionChangeHandler(tvResolutionParam_t resolution, void *userData)
    {
        printf("tvVideoResolutionChangeHandler resolution:%d\n",resolution.resolutionValue);
        ControlSettings *obj = (ControlSettings *)userData;
        if(obj)obj->NotifyVideoResolutionChange(resolution);
    }

    static void tvVideoFrameRateChangeHandler(tvVideoFrameRate_t frameRate, void *userData)
    {
        printf("tvVideoFrameRateChangeHandler format:%d \n",frameRate);
        ControlSettings *obj = (ControlSettings *)userData;
        if(obj)obj->NotifyVideoFrameRateChange(frameRate);
    }

    static JsonArray getSupportedVideoFormat(void)
    {
        JsonArray supportedHdrFormat;
        tvError_t ret = tvERROR_NONE;
        char buffer[EXPECTED_FILE_SIZE] = {0};
        unsigned short totalVideoFormats = 0;

        ret = ReadAllModeConfigfile(GENERIC_MODE_CONFIG_FILE,  buffer, "videoformat");
        if(ret == tvERROR_NONE){
           splitstringsfrombuffer(buffer, videoDescBuffer, &totalVideoFormats);

            for(int index=0; index<totalVideoFormats; index++){
                supportedHdrFormat.Add(videoDescBuffer+(index*VIDEO_DESCRIPTION_NAME_SIZE));
            }
        }else{
            printf("%s : Reading supported video format failed %d", __FUNCTION__, ret);
        }

        return supportedHdrFormat;
    }

    static std::string getVideoResolutionTypeToString(tvResolutionParam_t resolution)
    {
        std::string strValue = "NONE";
        std::string interlaceValue = (resolution.isInterlaced) ? "i" : "p";
        if ( resolution.resolutionValue != tvVideoResolution_NONE ) {
            strValue = std::to_string(resolution.frameWidth) + "*" + std::to_string(resolution.frameHeight) + interlaceValue;
        }
        printf("Video Resolution:[%s]\n", strValue.c_str());
        return strValue;
    }

    static JsonArray getSupportedVideoResolution(void)
    {
        JsonArray supportedResolution;
        tvError_t ret = tvERROR_NONE;
        char buffer[EXPECTED_FILE_SIZE] = {0};
        unsigned short totalVideoResolutions = 0;

        ret = ReadAllModeConfigfile(GENERIC_MODE_CONFIG_FILE,  buffer, "videoresolution");
        if(ret == tvERROR_NONE){
            splitstringsfrombuffer(buffer, videoDescBuffer, &totalVideoResolutions);

            for(int index=0; index < totalVideoResolutions; index++){
                supportedResolution.Add(videoDescBuffer+(index*VIDEO_DESCRIPTION_NAME_SIZE));
            }
        }else{
            printf("%s : Reading supported video resolutions failed %d", __FUNCTION__, ret);
        }

        return supportedResolution;
   }


    static const char *getVideoFormatTypeToString(tvVideoHDRFormat_t format)
    {
        const char *strValue = "NONE";
        switch(format) {
            case tvVideoHDRFormat_SDR:
                printf("Video Format: SDR\n");
                strValue = "SDR";
                break;
            case tvVideoHDRFormat_HDR10:
                printf("Video Format: HDR10\n");
                strValue = "HDR10";
                break;
            case tvVideoHDRFormat_HDR10PLUS:
                printf("Video Format: HDR10PLUS\n");
                strValue = "HDR10PLUS";
                break;
            case tvVideoHDRFormat_HLG:
                printf("Video Format: HLG\n");
                strValue = "HLG";
                break;
            case tvVideoHDRFormat_DV:
                printf("Video Format: DV\n");
                strValue = "DV";
                break;
            default:
                printf("Video Format:: NONE\n");
                strValue = "NONE";
                break;
        }
        return strValue;
   }

   static const char *getVideoFrameRateTypeToString(tvVideoFrameRate_t frameRate)
   {
       const char *strValue = "NONE";
       switch(frameRate) {
            case tvVideoFrameRate_24:
                printf("Video FrameRate: 24\n");
                strValue = "24";
                break;
            case tvVideoFrameRate_25:
                printf("Video FrameRate: 25\n");
                strValue = "25";
                break;
            case tvVideoFrameRate_30:
                printf("Video FrameRate: 30\n");
                strValue = "30";
                break;
            case tvVideoFrameRate_50:
                printf("Video FrameRate: 50\n");
                strValue = "50";
                break;
            case tvVideoFrameRate_60:
                printf("Video FrameRate: 60\n");
                strValue = "60";
                break;
            case tvVideoFrameRate_23dot98:
                printf("Video FrameRate: 23.98\n");
                strValue = "23.98";
                break;
            case tvVideoFrameRate_29dot97:
                printf("Video FrameRate: 29.97\n");
                strValue = "29.97";
                break;
            case tvVideoFrameRate_59dot94:
                printf("Video FrameRate: 59.94\n");
                strValue = "59.94";
                break;
            default:
                printf("Video FrameRate: NONE\n");
                strValue = "NONE";
                break;

        }
        return strValue;
    }

    static JsonArray getSupportedVideoFrameRate(void)
    {
        JsonArray supportedFrameRate;
        tvError_t ret = tvERROR_NONE;
        char buffer[EXPECTED_FILE_SIZE] = {0};
        unsigned short totalFrameRates = 0;

        ret = ReadAllModeConfigfile(GENERIC_MODE_CONFIG_FILE,  buffer, "videoframerate");
        if(ret == tvERROR_NONE){
            splitstringsfrombuffer(buffer, videoDescBuffer, &totalFrameRates);

            for(int index=0; index<totalFrameRates; index++){
                supportedFrameRate.Add(videoDescBuffer+(index*VIDEO_DESCRIPTION_NAME_SIZE));
            }
        }else{
            printf("%s : Reading supported video frame rate failed %d", __FUNCTION__, ret);
        }

        return supportedFrameRate;
    }


    ControlSettings::ControlSettings()
               : _skipURL(0)
               , m_currentHdmiInResoluton (dsVIDEO_PIXELRES_1920x1080)
               , m_videoZoomMode (tvDisplayMode_NORMAL)
               , m_isDisabledHdmiIn4KZoom (false)
    {
        LOGINFO("Entry\n");

	ControlSettings::_instance = this;
	InitializeIARM();
	CreateHandler({ 2 });

        //Common API Registration
	registerMethod("getAspectRatio", &ControlSettings::getAspectRatio, this);
        registerMethod("setAspectRatio", &ControlSettings::setAspectRatio, this);
        registerMethod("resetAspectRatio", &ControlSettings::resetAspectRatio, this);
        LOGINFO("Exit \n");
    }

    ControlSettings::~ControlSettings()
    {
        LOGINFO();
    }

    const std::string ControlSettings::Initialize(PluginHost::IShell* service)
    {
	LOGINFO("Entry\n");
        try {
            dsVideoPortResolution_t vidResolution;
            device::HdmiInput::getInstance().getCurrentVideoModeObj(vidResolution);
            m_currentHdmiInResoluton = vidResolution.pixelResolution;
        } catch (...){
            LOGWARN("ControlSettingsPlugins: getCurrentVideoModeObj failed");
        }
        LOGWARN("ControlSettingsPlugins: ControlSettings Initialize m_currentHdmiInResoluton:%d m_mod:%d", m_currentHdmiInResoluton, m_videoZoomMode);

        ASSERT(service != nullptr);
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());

        tvError_t ret = tvERROR_NONE;

	std::system("echo \"Testing dmesg [starts - before tvInit] - TVMgr::Initialize()\" > /dev/kmsg");
        ret = tvInit();
	std::system("echo \"Testing dmesg [starts - after tvInit] - TVMgr::Initialize()\" > /dev/kmsg");

	if(ret != tvERROR_NONE) {
            LOGERR("Platform Init failed, ret: %s \n", getErrorString(ret).c_str());

        }
        else{
            LOGINFO("Platform Init successful...\n");
	    std::system("echo \"Testing dmesg [starts..before tvSD3toCriSyncInit] - TVMgr::Initialize()\" > /dev/kmsg");
            ret = tvSD3toCriSyncInit();
	    std::system("echo \"Testing dmesg [starts..after tvSD3toCriSyncInit] - TVMgr::Initialize()\" > /dev/kmsg");
            if(ret != tvERROR_NONE) {
                LOGERR(" SD3 <->cri_data sync failed, ret: %s \n", getErrorString(ret).c_str());
            }
            else {
                LOGERR(" SD3 <->cri_data sync success, ret: %s \n", getErrorString(ret).c_str());
            }

        }
        tvVideoFormatCallbackData callbackData = {this,tvVideoFormatChangeHandler};
        RegisterVideoFormatChangeCB(callbackData);

        tvVideoResolutionCallbackData RescallbackData = {this,tvVideoResolutionChangeHandler};
        RegisterVideoResolutionChangeCB(RescallbackData);

        tvVideoFrameRateCallbackData FpscallbackData = {this,tvVideoFrameRateChangeHandler};
        RegisterVideoFrameRateChangeCB(FpscallbackData);

	instance->Initialize();

	LOGINFO("Exit\n");
        return (service != nullptr ? _T("") : _T("No service."));
    }

    void ControlSettings::Deinitialize(PluginHost::IShell* service)
    {
        LOGINFO();
        tvError_t ret = tvERROR_NONE;
        ret = tvTerm();

        if(ret != tvERROR_NONE) {
            LOGERR("Platform De-Init failed");
        }
        else{
            LOGINFO("Platform De-Init successful... \n");
        }
        ControlSettings::_instance = nullptr;
        DeinitializeIARM();

	instance->Deinitialize();
    }

    void ControlSettings::InitializeIARM()
    {
#if !defined (HDMIIN_4K_ZOOM)
        if (IARMinit())
        {
            IARM_Result_t res;
            IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_STATUS, dsHdmiStatusEventHandler) );
            IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_VIDEO_MODE_UPDATE, dsHdmiVideoModeEventHandler) );
            IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_HOTPLUG, dsHdmiEventHandler) );
        }
#endif
    }

    void ControlSettings::DeinitializeIARM()
    {
#if !defined (HDMIIN_4K_ZOOM)
        if (isIARMConnected())
        {
            IARM_Result_t res;
            IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_STATUS) );
            IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_VIDEO_MODE_UPDATE) );
            IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_HDMI_IN_HOTPLUG) );
        }
#endif
    }

    bool ControlSettings::IARMinit() {
        IARM_Result_t res;
        bool result = false;

        if (isIARMConnected()) {
            LOGINFO("ControlSettingsPlugin: IARM already connected");
            result = true;
        } else {
            res = IARM_Bus_Init(PLUGIN_IARM_BUS_NAME);
            LOGINFO("ControlSettingsPlugin: IARM_Bus_Init: %d", res);
            if (res == IARM_RESULT_SUCCESS ||
                res == IARM_RESULT_INVALID_STATE /* already inited or connected */) {

                res = IARM_Bus_Connect();
                LOGINFO("ControlSettingsPlugin: IARM_Bus_Connect: %d", res);
                if (res == IARM_RESULT_SUCCESS ||
                    res == IARM_RESULT_INVALID_STATE /* already connected or not inited */) {

                    result = isIARMConnected();
                } else {
                    LOGERR("ControlSettingsPlugin: IARM_Bus_Connect failure: %d", res);
                }
            } else {
                LOGERR("ControlSettingsPlugin: IARM_Bus_Init failure: %d", res);
            }
        }

        return result;
    }

    bool ControlSettings::isIARMConnected() {
        IARM_Result_t res;
        int isRegistered = 0;
        res = IARM_Bus_IsConnected(PLUGIN_IARM_BUS_NAME, &isRegistered);
        LOGINFO("ControlSettingsPlugin: IARM_Bus_IsConnected: %d (%d)", res, isRegistered);

        return (isRegistered == 1);
    }
   
    void ControlSettings::dsHdmiVideoModeEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
    {
        if(!ControlSettings::_instance)
            return;

        if (IARM_BUS_DSMGR_EVENT_HDMI_IN_VIDEO_MODE_UPDATE == eventId)
        {
            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
            int hdmi_in_port = eventData->data.hdmi_in_video_mode.port;
            dsVideoPortResolution_t resolution;
            ControlSettings::_instance->m_currentHdmiInResoluton = eventData->data.hdmi_in_video_mode.resolution.pixelResolution;
            resolution.pixelResolution =  eventData->data.hdmi_in_video_mode.resolution.pixelResolution;
            resolution.interlaced =  eventData->data.hdmi_in_video_mode.resolution.interlaced;
            resolution.frameRate =  eventData->data.hdmi_in_video_mode.resolution.frameRate;
            LOGWARN("ControlSettingsPlugins: Received IARM_BUS_DSMGR_EVENT_HDMI_IN_VIDEO_MODE_UPDATE  event  port: %d, pixelResolution: %d, interlaced : %d, frameRate: %d \n", hdmi_in_port,resolution.pixelResolution, resolution.interlaced, resolution.frameRate);
            if (ControlSettings::_instance->m_isDisabledHdmiIn4KZoom) {
                tvError_t ret = tvERROR_NONE;
                if (ControlSettings::_instance->m_currentHdmiInResoluton<dsVIDEO_PIXELRES_3840x2160 ||
                    (dsVIDEO_PIXELRES_MAX == ControlSettings::_instance->m_currentHdmiInResoluton)){
                    LOGWARN("ControlSettingsPlugins: Setting %d zoom mode for below 4K", ControlSettings::_instance->m_videoZoomMode);
                    ret = SetAspectRatio((tvDisplayMode_t)ControlSettings::_instance->m_videoZoomMode);
                }else {
                    LOGWARN("ControlSettingsPlugins: Setting auto zoom mode for 4K and above");
                    ret = SetAspectRatio(tvDisplayMode_AUTO);
                }
                if (ret != tvERROR_NONE) {
                    LOGWARN("SetAspectRatio set Failed");
                }
            } else {
                LOGWARN("ControlSettingsPlugins: %s: HdmiInput is not started yet. m_isDisabledHdmiIn4KZoom: %d", __FUNCTION__, ControlSettings::_instance->m_isDisabledHdmiIn4KZoom);
            }
        }
    }

    void ControlSettings::dsHdmiStatusEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
    {
        if(!ControlSettings::_instance)
            return;

        if (IARM_BUS_DSMGR_EVENT_HDMI_IN_STATUS == eventId)
        {
            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
            int hdmi_in_port = eventData->data.hdmi_in_status.port;
            bool hdmi_in_status = eventData->data.hdmi_in_status.isPresented;
            LOGWARN("ControlSettingsPlugins: Received IARM_BUS_DSMGR_EVENT_HDMI_IN_STATUS  event  port: %d, started: %d", hdmi_in_port,hdmi_in_status);
            if (!hdmi_in_status){
                tvError_t ret = tvERROR_NONE;
                ControlSettings::_instance->m_isDisabledHdmiIn4KZoom = false;
                LOGWARN("ControlSettingsPlugins: Hdmi streaming stopped here reapply the global zoom settings:%d here. m_isDisabledHdmiIn4KZoom: %d", ControlSettings::_instance->m_videoZoomMode, ControlSettings::_instance->m_isDisabledHdmiIn4KZoom);
                ret = SetAspectRatio((tvDisplayMode_t)ControlSettings::_instance->m_videoZoomMode);
                if (ret != tvERROR_NONE) {
                    LOGWARN("SetAspectRatio set Failed");
                }
            }else {
                ControlSettings::_instance->m_isDisabledHdmiIn4KZoom = true;
                LOGWARN("ControlSettingsPlugins: m_isDisabledHdmiIn4KZoom: %d", ControlSettings::_instance->m_isDisabledHdmiIn4KZoom);
            }
        }
    }
    
    void ControlSettings::NotifyVideoFormatChange(tvVideoHDRFormat_t format)
    {
        JsonObject response;
        response["currentVideoFormat"] = getVideoFormatTypeToString(format);
        response["supportedVideoFormat"] = getSupportedVideoFormat();
        sendNotify("videoFormatChanged", response);
    }

    uint32_t ControlSettings::getVideoFormat(const JsonObject& parameters, JsonObject& response)
    {
        LOGINFO();
        //PLUGIN_Lock(Lock);
        tvVideoHDRFormat_t videoFormat;
        tvError_t ret = GetCurrentVideoFormat(&videoFormat);
        response["supportedVideoFormat"] = getSupportedVideoFormat();
        if(ret != tvERROR_NONE) {
            response["currentVideoFormat"] = "NONE";
            returnResponse(false, getErrorString(ret).c_str());
        }
        else {
            response["currentVideoFormat"] = getVideoFormatTypeToString(videoFormat);
            LOGINFO(" getVideoFormat :%d   success \n",videoFormat);
            returnResponse(true, "success");
        }
    }

    void ControlSettings::NotifyVideoResolutionChange(tvResolutionParam_t resolution)
    {
        JsonObject response;
        response["currentVideoResolution"] = getVideoResolutionTypeToString(resolution);
        response["supportedVideoResolution"] = getSupportedVideoResolution();
        sendNotify("videoResolutionChanged", response);
    }

    uint32_t ControlSettings::getVideoResolution(const JsonObject& parameters, JsonObject& response)
    {
        LOGINFO();
        //PLUGIN_Lock(Lock);
        tvResolutionParam_t videoResolution;
        tvError_t ret = GetVideoResolution(&videoResolution);
        response["supportedVideoResolution"] = getSupportedVideoResolution();
        if(ret != tvERROR_NONE) {
            response["currentVideoResolution"] = "NONE";
            returnResponse(false, getErrorString(ret));
        }
        else {
            response["currentVideoResolution"] = getVideoResolutionTypeToString(videoResolution);
            LOGINFO(" getVideoResolution :%d   success \n",videoResolution.resolutionValue);
            returnResponse(true, "success");
        }
    }

     void ControlSettings::NotifyVideoFrameRateChange(tvVideoFrameRate_t frameRate)
    {
        JsonObject response;
        response["currentVideoFrameRate"] = getVideoFrameRateTypeToString(frameRate);
        response["supportedVideoFrameRate"] = getSupportedVideoFrameRate();
        sendNotify("videoFrameRateChanged", response);
    }

    uint32_t ControlSettings::getVideoFrameRate(const JsonObject& parameters, JsonObject& response)
    {
        LOGINFO();
        //PLUGIN_Lock(Lock);
        tvVideoFrameRate_t videoFramerate;
        tvError_t ret = GetVideoFrameRate(&videoFramerate);
        response["supportedFrameRate"] = getSupportedVideoFrameRate();
        if(ret != tvERROR_NONE) {
            response["currentVideoFrameRate"] = "NONE";
            returnResponse(false, getErrorString(ret));
        }
        else {
            response["currentVideoFrameRate"] = getVideoFrameRateTypeToString(videoFramerate);
            LOGINFO(" videoFramerate :%d   success \n",videoFramerate);
            returnResponse(true, "success");
        }
    }

    //API
    uint32_t ControlSettings::setAspectRatio(const JsonObject& parameters, JsonObject& response)
    {

        LOGINFO("Entry %s\n",__FUNCTION__);
        tvDisplayMode_t mode = tvDisplayMode_16x9;
	std::string value;
        std::string pqmode;
        std::string source;
        std::string format;


        value = parameters.HasLabel("aspectRatio") ? parameters["aspectRatio"].String() : "";
        returnIfParamNotFound(value);

	pqmode = parameters.HasLabel("pictureMode") ? parameters["pictureMode"].String() : "";
        if(pqmode.empty())
            pqmode = "current";

        source = parameters.HasLabel("source") ? parameters["source"].String() : "";
        if(source.empty())
            source = "current";

        format = parameters.HasLabel("format") ? parameters["format"].String() : "";
        if(format.empty())
            format = "current";

        if(!value.compare("TV 16X9 STRETCH")) {
            mode = tvDisplayMode_16x9;
        }
        else if (!value.compare("TV 4X3 PILLARBOX")){
            mode = tvDisplayMode_4x3;
        }
        else if (!value.compare("TV NORMAL")){
            mode = tvDisplayMode_NORMAL;
        }
        else if (!value.compare("TV DIRECT")){
            mode = tvDisplayMode_DIRECT;
        }
        else if (!value.compare("TV AUTO")){
            mode = tvDisplayMode_AUTO;
        }
        else if (!value.compare("TV ZOOM")){
            mode = tvDisplayMode_ZOOM;
        }
        else {
            returnResponse(false, getErrorString(tvERROR_INVALID_PARAM));
        }

	m_videoZoomMode = mode;
        tvError_t ret = setAspectRatioZoomSettings (mode);

        if(ret != tvERROR_NONE) {
            returnResponse(false, getErrorString(ret).c_str());
        }
        else {
            //Save DisplayMode to localstore and ssm_data
            int params[3]={0};
            params[0]=mode;
            int retval=UpdatePQParamsToCache("set","AspectRatio",pqmode.c_str(),source.c_str(),format.c_str(),PQ_PARAM_ASPECT_RATIO,params);

            if(retval != 0) {
                LOGWARN("Failed to Save DisplayMode to ssm_data\n");
            }

            tr181ErrorCode_t err = setLocalParam(rfc_caller_id, TVSETTINGS_ASPECTRATIO_RFC_PARAM, value.c_str());
            if ( err != tr181Success ) {
                LOGWARN("setLocalParam for %s Failed : %s\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM, getTR181ErrorString(err));
            }
            else {
                LOGINFO("setLocalParam for %s Successful, Value: %s\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM, value.c_str());
            }
            LOGINFO("Exit : SetAspectRatio2() value : %s\n",value.c_str());
            returnResponse(true, "success");
        }
    }

    uint32_t ControlSettings::getAspectRatio(const JsonObject& parameters, JsonObject& response)
    {

        LOGINFO("Entry%s\n",__FUNCTION__);
	tvDisplayMode_t mode;
        JsonObject dispModeObj;
        JsonArray dispOptions ;
        dispOptions.Add("TV AUTO"); dispOptions.Add("TV DIRECT"); dispOptions.Add("TV NORMAL");
        dispOptions.Add("TV 16X9 STRETCH"); dispOptions.Add("TV 4X3 PILLARBOX"); dispOptions.Add("TV ZOOM");
        dispModeObj["Selected"] = "TV AUTO";
        dispModeObj["Options"] = dispOptions;

        tvError_t ret = getUserSelectedAspectRatio (&mode);

        if(ret != tvERROR_NONE) {
            returnResponse(false, getErrorString(ret).c_str());
        }
        else {
            switch(mode) {

                case tvDisplayMode_16x9:
                    LOGINFO("Aspect Ratio: TV 16X9 STRETCH\n");
                    dispModeObj["Selected"] = "TV 16X9 STRETCH";
                    break;

                case tvDisplayMode_4x3:
                    LOGINFO("Aspect Ratio: TV 4X3 PILLARBOX\n");
                    dispModeObj["Selected"] = "TV 4X3 PILLARBOX";
                    break;

                case tvDisplayMode_NORMAL:
                    LOGINFO("Aspect Ratio: TV Normal\n");
                    dispModeObj["Selected"] = "TV NORMAL";
                    break;

                case tvDisplayMode_AUTO:
                    LOGINFO("Aspect Ratio: TV AUTO\n");
                    dispModeObj["Selected"] = "TV AUTO";
                    break;

                case tvDisplayMode_DIRECT:
                    LOGINFO("Aspect Ratio: TV DIRECT\n");
                    dispModeObj["Selected"] = "TV DIRECT";
                    break;

                case tvDisplayMode_ZOOM:
                    LOGINFO("Aspect Ratio: TV ZOOM\n");
                    dispModeObj["Selected"] = "TV ZOOM";
                    break;

                default:
                    LOGINFO("Aspect Ratio: TV AUTO\n");
                    break;
            }
	    response["AspectRatio"] = dispModeObj;
            returnResponse(true, "success");
        }
    }

    tvError_t ControlSettings::setAspectRatioZoomSettings(tvDisplayMode_t mode)
    {
        tvError_t ret = tvERROR_GENERAL;
        LOGERR("tvmgrplugin: %s mode selected is: %d", __FUNCTION__, m_videoZoomMode);
#if !defined (HDMIIN_4K_ZOOM)
        if (ControlSettings::_instance->m_isDisabledHdmiIn4KZoom) {
            if (m_currentHdmiInResoluton<dsVIDEO_PIXELRES_3840x2160 ||
                (dsVIDEO_PIXELRES_MAX == m_currentHdmiInResoluton)){
                LOGWARN("tvmgrplugin: %s: Setting %d zoom mode for below 4K", __FUNCTION__, m_videoZoomMode);
#endif
                ret = SetAspectRatio(mode);
#if !defined (HDMIIN_4K_ZOOM)
            }else {
                LOGWARN("tvmgrplugin: %s: Setting auto zoom mode for 4K and above", __FUNCTION__);
                ret = SetAspectRatio(tvDisplayMode_AUTO);
            }
        } else {
            LOGWARN("tvmgrplugin: %s: HdmiInput is not started yet. m_isDisabledHdmiIn4KZoom: %d", __FUNCTION__, ControlSettings::_instance->m_isDisabledHdmiIn4KZoom);
            ret = SetAspectRatio((tvDisplayMode_t)m_videoZoomMode);
        }
#endif
        return ret;
    }

    tvError_t ControlSettings::getUserSelectedAspectRatio (tvDisplayMode_t* mode)
    {
        tvError_t ret = tvERROR_GENERAL;
#if !defined (HDMIIN_4K_ZOOM)
        LOGERR("controlsettingsplugin: %s mode selected is: %d", __FUNCTION__, m_videoZoomMode);
        if (m_isDisabledHdmiIn4KZoom) {
            if (!(m_currentHdmiInResoluton<dsVIDEO_PIXELRES_3840x2160 ||
               (dsVIDEO_PIXELRES_MAX == m_currentHdmiInResoluton))){
                *mode = (tvDisplayMode_t)m_videoZoomMode;
                LOGWARN("tvmgrplugin: %s: Getting zoom mode %d for display, for 4K and above", __FUNCTION__, *mode);
                return tvERROR_NONE;
            }
        }
#endif
        ret = GetAspectRatio(mode);
        return ret;
    }

    uint32_t ControlSettings::resetAspectRatio(const JsonObject& parameters, JsonObject& response)
    {
        LOGINFO("Entry\n");
	std::string pqmode;
        std::string source;
        std::string format;
        tvError_t ret = tvERROR_NONE;

	pqmode = parameters.HasLabel("pictureMode") ? parameters["pictureMode"].String() : "";
        if(pqmode.empty())
            pqmode = "current";

        source = parameters.HasLabel("source") ? parameters["source"].String() : "";
        if(source.empty())
            source = "current";

        format = parameters.HasLabel("format") ? parameters["format"].String() : "";
        if(format.empty())
            format = "current";

        tr181ErrorCode_t err = clearLocalParam(rfc_caller_id,TVSETTINGS_ASPECTRATIO_RFC_PARAM);
        if ( err != tr181Success ) {
            LOGWARN("clearLocalParam for %s Failed : %s\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM, getTR181ErrorString(err));
            ret  = tvERROR_GENERAL;
        }
        else {
            LOGINFO("clearLocalParam for %s Successful\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM);
            ret = setDefaultAspectRatio(pqmode,source,format);
        }
        if(ret != tvERROR_NONE)
        {
            returnResponse(false, getErrorString(ret));
        }
        else
        {
            LOGINFO("Exit : resetDefaultAspectRatio()\n");
            returnResponse(true, "success");
        }
    }

    tvError_t ControlSettings::setDefaultAspectRatio(std::string pqmode,std::string  format,std::string source)
    {
        tvDisplayMode_t mode = tvDisplayMode_MAX;
        TR181_ParamData_t param;
        tvError_t ret = tvERROR_NONE;

        memset(&param, 0, sizeof(param));
        tr181ErrorCode_t err = getLocalParam(rfc_caller_id, TVSETTINGS_ASPECTRATIO_RFC_PARAM, &param);
        if ( tr181Success == err )
        {
            LOGINFO("getLocalParam for %s is %s\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM, param.value);

            if(!std::string(param.value).compare("16:9")) {
                mode = tvDisplayMode_16x9;
            }
            else if (!std::string(param.value).compare("4:3")){
                mode = tvDisplayMode_4x3;
            }
            else if (!std::string(param.value).compare("Full")){
                mode = tvDisplayMode_FULL;
            }
            else if (!std::string(param.value).compare("Normal")){
                mode = tvDisplayMode_NORMAL;
            }
            else if (!std::string(param.value).compare("TV AUTO")){
                mode = tvDisplayMode_AUTO;
            }
            else if (!std::string(param.value).compare("TV DIRECT")){
                mode = tvDisplayMode_DIRECT;
            }
            else if (!std::string(param.value).compare("TV NORMAL")){
                mode = tvDisplayMode_NORMAL;
            }
            else if (!std::string(param.value).compare("TV ZOOM")){
                mode = tvDisplayMode_ZOOM;
            }
            else if (!std::string(param.value).compare("TV 16X9 STRETCH")){
                mode = tvDisplayMode_16x9;
            }
            else if (!std::string(param.value).compare("TV 4X3 PILLARBOX")){
                mode = tvDisplayMode_4x3;
            }
            else {
                mode = tvDisplayMode_AUTO;
            }

            m_videoZoomMode = mode;
            tvError_t ret = setAspectRatioZoomSettings (mode);

            if(ret != tvERROR_NONE) {
                LOGWARN("AspectRatio  set failed: %s\n",getErrorString(ret).c_str());
            }
            else {
                //Save DisplayMode to ssm_data
                int params[3]={0};
                params[0]=mode;
                int retval=UpdatePQParamsToCache("set","AspectRatio",pqmode.c_str(),source.c_str(),format.c_str(),PQ_PARAM_ASPECT_RATIO,params);

                if(retval != 0) {
                    LOGWARN("Failed to Save DisplayMode to ssm_data\n");
                }
                LOGINFO("Aspect Ratio initialized successfully, value: %s\n", param.value);
            }

        }
        else
        {
            LOGWARN("getLocalParam for %s Failed : %s\n", TVSETTINGS_ASPECTRATIO_RFC_PARAM, getTR181ErrorString(err));
            ret = tvERROR_GENERAL;
        }
		 return ret;
    }

} //namespace WPEFramework

} //namespace Plugin

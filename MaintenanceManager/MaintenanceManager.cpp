/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2021 RDK Management
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

/**
 * @file  MaintenanceManager.cpp
 * @author Livin Sunny
 * @brief Thunder Plugin based Implementation for RDK MaintenanceManager service API's.
 * @reference RDK-29959.
 */
#include <stdlib.h>
#include <errno.h>
#include <cstdio>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <bits/stdc++.h>
#include <algorithm>

#include "MaintenanceManager.h"
#include "utils.h"

#if defined(USE_IARMBUS) || defined(USE_IARM_BUS)
#include "libIARM.h"

#endif /* USE_IARMBUS || USE_IARM_BUS */

#if defined(HAS_API_SYSTEM) && defined(HAS_API_POWERSTATE)
#include "powerstate.h"
#endif /* HAS_API_SYSTEM && HAS_API_POWERSTATE */

#ifdef ENABLE_DEVICE_MANUFACTURER_INFO
#include "mfrMgr.h"
#endif

#ifdef ENABLE_DEEP_SLEEP
#include "deepSleepMgr.h"
#endif

#include "maintenanceMGR.h"

using namespace std;

#define API_VERSION_NUMBER_MAJOR 1
#define API_VERSION_NUMBER_MINOR 0

#define TR181_AUTOREBOOT_ENABLE "Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AutoReboot.Enable"

string notifyStatusToString(Maint_notify_status_t &status)
{
    string ret_status="";
    switch(status){
        case MAINTENANCE_IDLE:
            ret_status="MAINTENANCE_IDLE";
            break;
        case MAINTENANCE_STARTED:
            ret_status="MAINTENANCE_STARTED";
            break;
        case MAINTENANCE_ERROR:
            ret_status="MAINTENANCE_ERROR";
            break;
        case MAINTENANCE_COMPLETE:
            ret_status="MAINTENANCE_COMPLETE";
            break;
        case MAINTENANCE_INCOMPLETE:
            ret_status="MAINTENANCE_INCOMPLETE";
            break;
        default:
            ret_status="MAINTENANCE_ERROR";
    }
    return ret_status;
}

string moduleStatusToString(IARM_Maint_module_status_t &status)
{
    string ret_status="";
    switch(status){
        case MAINT_DCM_COMPLETE:
            ret_status="MAINTENANCE_DCM_COMPLETE";
            break;
        case MAINT_DCM_ERROR:
            ret_status="MAINTENANCE_DCM_ERROR";
            break;
        case MAINT_RFC_COMPLETE:
            ret_status="MAINTENANCE_RFC_COMPLETE";
            break;
        case MAINT_RFC_ERROR:
            ret_status="MAINTENANCE_RFC_ERROR";
            break;
        case MAINT_LOGUPLOAD_COMPLETE:
            ret_status="MAINTENANCE_LOGUPLOAD_COMPLETE";
            break;
        case MAINT_LOGUPLOAD_ERROR:
            ret_status="MAINTENANCE_LOGUPLOAD_ERROR";
            break;
        case MAINT_PINGTELEMETRY_COMPLETE:
            ret_status="MAINTENANCE_PINGTELEMETRY_COMPLETE";
            break;
        case MAINT_PINGTELEMETRY_ERROR:
            ret_status="MAINTENANCE_PINGTELEMETRY_ERROR";
            break;
        case MAINT_FWDOWNLOAD_COMPLETE:
            ret_status="MAINTENANCE_FWDONLOAD_COMPLETE";
            break;
        case MAINT_FWDOWNLOAD_ERROR:
            ret_status="MAINTENANCE_FWDONLOAD_ERROR";
            break;
        case MAINT_REBOOT_REQUIRED:
            ret_status="MAINTENANCE_REBOOT_REQUIRED";
            break;
        case MAINT_FWDOWNLOAD_ABORTED:
            ret_status="MAINTENANCE_FWDOWNLOAD_ABORTED";
            break;
        case MAINT_CRITICAL_UPDATE:
            ret_status="MAINTENANCE_CRITICAL_UPDATE";
            break;
        default:
            ret_status="MAINTENANCE_EMPTY";
    }
    return ret_status;
}
/**
 * @brief WPEFramework class for Maintenance Manager
 */
namespace WPEFramework {
    namespace Plugin {
        //Prototypes
        SERVICE_REGISTRATION(MaintenanceManager,API_VERSION_NUMBER_MAJOR,API_VERSION_NUMBER_MINOR);
        /* Global time variable */
        MaintenanceManager* MaintenanceManager::_instance = nullptr;

        cSettings MaintenanceManager::m_setting(MAINTENANCE_MGR_RECORD_FILE);
        //TODO  this need to moved to a seperate class and vector based.

        string task_names_foreground[]={
            "/lib/rdk/RFCbase.sh",
            "/lib/rdk/swupdate_utility.sh >> /opt/logs/swupdate.log",
            "/lib/rdk/Start_uploadSTBLogs.sh"
        };



        /**
         * Register MaintenanceManager module as wpeframework plugin
         */
        MaintenanceManager::MaintenanceManager()
            :AbstractPlugin()
        {
            MaintenanceManager::_instance = this;

            /**
             * @brief Invoking Plugin API register to WPEFRAMEWORK.
             */
#ifdef DEBUG
            registerMethod("sampleMaintenanceManagerAPI", &MaintenanceManager::sampleAPI, this);
#endif /* DEBUG */
            registerMethod("getMaintenanceActivityStatus", &MaintenanceManager::getMaintenanceActivityStatus,this);
            registerMethod("getMaintenanceStartTime", &MaintenanceManager::getMaintenanceStartTime,this);
            registerMethod("setMaintenanceMode", &MaintenanceManager::setMaintenanceMode,this);
            registerMethod("startMaintenance", &MaintenanceManager::startMaintenance,this);


            MaintenanceManager::m_task_map["/lib/rdk/StartDCM_maintaince.sh"]=false;
            MaintenanceManager::m_task_map[task_names_foreground[0].c_str()]=false;
            MaintenanceManager::m_task_map[task_names_foreground[1].c_str()]=false;
            MaintenanceManager::m_task_map[task_names_foreground[2].c_str()]=false;


         }


        string MaintenanceManager::getLastRebootReason(){

            char rebootInfo[1024] = {'\0'};
            bool retAPIStatus = false;
            string reboot_reason="";
            string reason="";

            if (Utils::fileExists(SYSTEM_SERVICE_PREVIOUS_REBOOT_INFO_FILE)) {
                retAPIStatus = getFileContentToCharBuffer(SYSTEM_SERVICE_PREVIOUS_REBOOT_INFO_FILE, rebootInfo );
            }

            if (retAPIStatus && strlen(rebootInfo)) {
                string dataBuf(rebootInfo);
                JsonObject rebootInfoJson;
                rebootInfoJson.FromString(rebootInfo);
                reason = rebootInfoJson["reason"].String();
            }

            reboot_reason = reason;
            LOGINFO("Previous Reboot Reason: %s", reason.c_str());
            return reboot_reason;
        }

        bool MaintenanceManager::checkAutoRebootFlag(){
            bool ret=false;
            RFC_ParamData_t param;
            WDMP_STATUS wdmpStatus = getRFCParameter(const_cast<char *>("MaintenanceManager"),TR181_AUTOREBOOT_ENABLE, &param);
            if (wdmpStatus == WDMP_SUCCESS || wdmpStatus == WDMP_ERR_DEFAULT_VALUE){
                if( param.type == WDMP_BOOLEAN ){
                    if(strncasecmp(param.value,"true",4) == 0 ){
                        ret=true;
                    }
                }
            }
            LOGINFO(" AutoReboot.Enable = %s , call value %d ", (ret == true)?"true":"false", wdmpStatus);
            return ret;
        }

        void MaintenanceManager::requestSystemReboot(){
            bool result = false;

            string rebootCommand="";

            if (Utils::fileExists("/lib/rdk/AutoReboot.sh")) {
                rebootCommand = "/lib/rdk/AutoReboot.sh &";
            } else {
                LOGINFO("AutoReboot is not present \n");
            }

           LOGINFO("Requesting SystemReboot !!");

            system(rebootCommand.c_str());

        }
        void MaintenanceManager::task_execution_thread(){
            LOGINFO("INSIDE thread task execution");
            int task_count=3;
            int i=0;

            /* Check if the last reboot was MAITENANCE REBOOT */
            string reboot_reason=getLastRebootReason();
            if (!reboot_reason.compare("MAINTENANCE_REBOOT")){
                g_is_reboot_pending="false";
            }

            LOGINFO("Reboot_Pending :%s",g_is_reboot_pending.c_str());

            string cmd="";

            MaintenanceManager::_instance->onMaintenanceStatusChange(MAINTENANCE_STARTED);
            /*  In an unsolicited maintenance we make sure only after
             *  after DCM task activities are started.
             */

            /* we add the task in a loop */
            std::unique_lock<std::mutex> lck(m_callMutex);
            if (UNSOLICITED_MAINTENANCE == g_maintenance_type){
                LOGINFO("UNSOLICITED_MAINTENANCE");
                for ( i=0;i< task_count ;i++ ){
                    task_thread.wait(lck);
                    cmd=task_names_foreground[i].c_str();
                    cmd+=" &";
                    cmd+="\0";
                    m_task_map[task_names_foreground[i].c_str()]=true;
                    LOGINFO("Starting Script (USM) :  %s \n", cmd.c_str());
                    system(cmd.c_str());
                }
                LOGINFO("Worker Thread Completed");
            }
            /* Here in Solicited we start with RFC so no
             * need to wait for any DCM events */
            else if( SOLICITED_MAINTENANCE == g_maintenance_type ){
                    LOGINFO("SOLICITED_MAINTENANCE");
                    cmd=task_names_foreground[0].c_str();
                    cmd+=" &";
                    cmd+="\0";
                    m_task_map[task_names_foreground[0].c_str()]=true;
                    LOGINFO("Starting Script (SM) :  %s \n", cmd.c_str());
                    system(cmd.c_str());
                    cmd="";
                    for (i=1;i<task_count;i++){
                        task_thread.wait(lck);
                        cmd=task_names_foreground[i].c_str();
                        cmd+=" &";
                        cmd+="\0";
                        m_task_map[task_names_foreground[i].c_str()]=true;
                        LOGINFO("Starting Script (SM) :  %s \n", cmd.c_str());
                        system(cmd.c_str());
                    }
            }
        }

        MaintenanceManager::~MaintenanceManager()
        {
            MaintenanceManager::_instance = nullptr;
        }

        const string MaintenanceManager::Initialize(PluginHost::IShell*)
        {
#if defined(USE_IARMBUS) || defined(USE_IARM_BUS)
            InitializeIARM();
#endif /* defined(USE_IARMBUS) || defined(USE_IARM_BUS) */
            /* On Success; return empty to indicate no error text. */
            return (string());
        }

        void MaintenanceManager::Deinitialize(PluginHost::IShell*)
        {
#if defined(USE_IARMBUS) || defined(USE_IARM_BUS)
            DeinitializeIARM();
#endif /* defined(USE_IARMBUS) || defined(USE_IARM_BUS) */
        }

#if defined(USE_IARMBUS) || defined(USE_IARM_BUS)
        void MaintenanceManager::InitializeIARM()
        {
            if (Utils::IARM::init()) {
                IARM_Result_t res;
                // Register for the Maintenance Notification Events
                IARM_CHECK(IARM_Bus_RegisterEventHandler(IARM_BUS_MAINTENANCE_MGR_NAME, IARM_BUS_MAINTENANCEMGR_EVENT_UPDATE, _MaintenanceMgrEventHandler));
                //Register for setMaintenanceStartTime
                IARM_CHECK(IARM_Bus_RegisterEventHandler(IARM_BUS_MAINTENANCE_MGR_NAME, IARM_BUS_DCM_NEW_START_TIME_EVENT,_MaintenanceMgrEventHandler));

                maintenanceManagerOnBootup();
            }
        }

        void MaintenanceManager::maintenanceManagerOnBootup() {
            /* on boot up we set these things */
            MaintenanceManager::g_currentMode = FOREGROUND_MODE;

            MaintenanceManager::g_notify_status=MAINTENANCE_IDLE;
            MaintenanceManager::g_epoch_time="";

            /* to know the maintenance is solicited or unsolicited */
            g_maintenance_type=UNSOLICITED_MAINTENANCE;

            MaintenanceManager::g_is_critical_maintenance="false";
            MaintenanceManager::g_is_reboot_pending="false";
            MaintenanceManager::g_lastSuccessful_maint_time="";
            MaintenanceManager::g_task_status=0;

            /* we post just to tell that we are in idle at this moment */
            MaintenanceManager::_instance->onMaintenanceStatusChange(g_notify_status);

            int32_t exec_status=E_NOK;

            /* we call dcmscript to get the new start time */
            if (Utils::fileExists("/lib/rdk/StartDCM_maintaince.sh")) {
                exec_status = system("/lib/rdk/StartDCM_maintaince.sh &");
                if ( E_OK == exec_status ){
                    LOGINFO("DBG:Succesfully executed StartDCM_maintaince.sh \n");
                    m_task_map["/lib/rdk/StartDCM_maintaince.sh"]=true;
                }
                else {
                    LOGINFO("DBG:Failed to execute StartDCM_maintaince.sh !! \n");
                }
            }
            else {
                LOGINFO("DBG: Unable to find StartDCM_maintaince.sh \n");
            }

            /* we moved every thing to a thread */
            /* only when dcm is getting a DCM_SUCCESS/DCM_ERROR we say
             * Maintenance is started until then we say MAITENANCE_IDLE */
            m_thread = std::thread(&MaintenanceManager::task_execution_thread, _instance);
        }

        void MaintenanceManager::_MaintenanceMgrEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
        {
            if (MaintenanceManager::_instance){
                LOGWARN("IARM event Received with %d !", eventId);
                MaintenanceManager::_instance->iarmEventHandler(owner, eventId, data, len);
            }
            else
                LOGWARN("WARNING - cannot handle IARM events without MaintenanceManager plugin instance!");
        }

        void MaintenanceManager::iarmEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
        {
            Maint_notify_status_t m_notify_status=MAINTENANCE_STARTED;
            IARM_Bus_MaintMGR_EventData_t *module_event_data=(IARM_Bus_MaintMGR_EventData_t*)data;
            IARM_Maint_module_status_t module_status;
            time_t successfulTime;
            string str_successfulTime="";
            auto task_status_DCM=m_task_map.find("/lib/rdk/StartDCM_maintaince.sh");
            auto task_status_RFC=m_task_map.find(task_names_foreground[0].c_str());
            auto task_status_FWDLD=m_task_map.find(task_names_foreground[1].c_str());
            auto task_status_LOGUPLD=m_task_map.find(task_names_foreground[2].c_str());

            LOGINFO("Event-ID = %d \n",eventId);
            IARM_Bus_MaintMGR_EventId_t event = (IARM_Bus_MaintMGR_EventId_t)eventId;
            LOGINFO("Maintenance Event= %d \n",event);

            if (!strcmp(owner, IARM_BUS_MAINTENANCE_MGR_NAME)) {
                if ( IARM_BUS_DCM_NEW_START_TIME_EVENT == eventId ) {
                    /* we got a new start time from DCM script */
                    string l_time(module_event_data->data.startTimeUpdate.start_time);
                    LOGINFO("DCM_NEW_START_TIME_EVENT Start Time %s \n", l_time.c_str());
                    /* Store it in a Global structure */
                    g_epoch_time=l_time;
                }
                else if ( IARM_BUS_MAINTENANCEMGR_EVENT_UPDATE == eventId ) {
                    module_status = module_event_data->data.maintenance_module_status.status;
                    LOGINFO("MaintMGR Status %d \n",module_status);
                    string status_string=moduleStatusToString(module_status);
                    LOGINFO("MaintMGR Status %s \n", status_string.c_str());
                    switch (module_status) {
                        case MAINT_RFC_COMPLETE :
                            if(task_status_RFC->second != true) {
                                 LOGINFO("Ignoring Event RFC_COMPLETE");
                            }
                            else {
                                 SET_STATUS(g_task_status,RFC_SUCCESS);
                                 SET_STATUS(g_task_status,RFC_COMPLETE);
                                 task_thread.notify_one();
                                 m_task_map[task_names_foreground[0].c_str()]=false;
                            }
                            break;
                        case MAINT_DCM_COMPLETE :
                            if(task_status_DCM->second != true) {
                                 LOGINFO("Ignoring Event DCM_COMPLETE");
                            }
                            else {
                                SET_STATUS(g_task_status,DCM_SUCCESS);
                                SET_STATUS(g_task_status,DCM_COMPLETE);
                                task_thread.notify_one();
                                m_task_map["/lib/rdk/StartDCM_maintaince.sh"]=false;
                            }
                            break;
                        case MAINT_FWDOWNLOAD_COMPLETE :
                            if(task_status_FWDLD->second != true) {
                                 LOGINFO("Ignoring Event MAINT_FWDOWNLOAD_COMPLETE");
                            }
                            else {
                                SET_STATUS(g_task_status,DIFD_SUCCESS);
                                SET_STATUS(g_task_status,DIFD_COMPLETE);
                                task_thread.notify_one();
                                m_task_map[task_names_foreground[1].c_str()]=false;
                            }
                            break;
                       case MAINT_LOGUPLOAD_COMPLETE :
                            if(task_status_LOGUPLD->second != true) {
                                 LOGINFO("Ignoring Event MAINT_LOGUPLOAD_COMPLETE");
                            }
                            else {
                                SET_STATUS(g_task_status,LOGUPLOAD_SUCCESS);
                                SET_STATUS(g_task_status,LOGUPLOAD_COMPLETE);
                                m_task_map[task_names_foreground[2].c_str()]=false;
                            }

                            break;
                        case MAINT_REBOOT_REQUIRED :
                            SET_STATUS(g_task_status,REBOOT_REQUIRED);
                            g_is_reboot_pending="true";
                            break;
                        case MAINT_CRITICAL_UPDATE:
                            g_is_critical_maintenance="true";
                            break;
                        case MAINT_FWDOWNLOAD_ABORTED:
                            SET_STATUS(g_task_status,TASK_SKIPPED);
                            break;
                        case MAINT_DCM_ERROR:
                            if(task_status_DCM->second != true) {
                                 LOGINFO("Ignoring Event DCM_ERROR");
                            }
                            else {
                                SET_STATUS(g_task_status,DCM_COMPLETE);
                                task_thread.notify_one();
                                LOGINFO("Error encountered in DCM script task \n");
                                m_task_map["/lib/rdk/StartDCM_maintaince.sh"]=false;
                            }
                            break;
                        case MAINT_RFC_ERROR:
                            if(task_status_RFC->second != true) {
                                 LOGINFO("Ignoring Event RFC_ERROR");
                            }
                            else {
                                 SET_STATUS(g_task_status,RFC_COMPLETE);
                                 task_thread.notify_one();
                                 LOGINFO("Error encountered in RFC script task \n");
                                 m_task_map[task_names_foreground[0].c_str()]=false;
                            }

                            break;
                        case MAINT_LOGUPLOAD_ERROR:
                            if(task_status_LOGUPLD->second != true) {
                                  LOGINFO("Ignoring Event MAINT_LOGUPLOAD_ERROR");
                            }
                            else {
                                SET_STATUS(g_task_status,LOGUPLOAD_COMPLETE);
                                LOGINFO("Error encountered in LOGUPLOAD script task \n");
                                m_task_map[task_names_foreground[2].c_str()]=false;
                            }

                            break;
                       case MAINT_FWDOWNLOAD_ERROR:
                            if(task_status_FWDLD->second != true) {
                                 LOGINFO("Ignoring Event MAINT_FWDOWNLOAD_ERROR");
                            }
                            else {
                                SET_STATUS(g_task_status,DIFD_COMPLETE);
                                task_thread.notify_one();
                                LOGINFO("Error encountered in SWUPDATE script task \n");
                                m_task_map[task_names_foreground[1].c_str()]=false;
                            }
                            break;
                       case MAINT_DCM_INPROGRESS:
                            m_task_map["/lib/rdk/StartDCM_maintaince.sh"]=true;
                            /*will be set to false once COMEPLETE/ERROR received for DCM*/
                            LOGINFO(" DCM already IN PROGRESS -> setting m_task_map of DCM to true \n");
                            break;
                       case MAINT_RFC_INPROGRESS:
                            m_task_map[task_names_foreground[0].c_str()]=true;
                            /*will be set to false once COMEPLETE/ERROR received for RFC*/
                            LOGINFO(" RFC already IN PROGRESS -> setting m_task_map of RFC to true \n");
                            break;
                       case MAINT_FWDOWNLOAD_INPROGRESS:
                            m_task_map[task_names_foreground[1].c_str()]=true;
                            /*will be set to false once COMEPLETE/ERROR received for FWDOWNLOAD*/
                            LOGINFO(" FWDOWNLOAD already IN PROGRESS -> setting m_task_map of FWDOWNLOAD to true \n");
                            break;
                       case MAINT_LOGUPLOAD_INPROGRESS:
                            m_task_map[task_names_foreground[2].c_str()]=true;
                            /*will be set to false once COMEPLETE/ERROR received for LOGUPLOAD*/
                            LOGINFO(" LOGUPLOAD already IN PROGRESS -> setting m_task_map of LOGUPLOAD to true \n");
                            break;
                    }
                }
                else{
                    LOGINFO("Unknown Maintenance Status!!");
                }

                LOGINFO(" BITFIELD Status : %x",g_task_status);
                /* Send the updated status only if all task completes execution
                 * until that we say maintenance started */
                if ( (g_task_status & TASKS_COMPLETED ) == TASKS_COMPLETED ){
                    if ( (g_task_status & ALL_TASKS_SUCCESS) == ALL_TASKS_SUCCESS ){ // all tasks success
                        LOGINFO("DBG:Maintenance Successfully Completed!!");
                        m_notify_status=MAINTENANCE_COMPLETE;
                        /*  we store the time in persistant location */
                        successfulTime=time(nullptr);
                        tm ltime=*localtime(&successfulTime);
                        time_t epoch_time=mktime(&ltime);
                        str_successfulTime=to_string(epoch_time);
                        LOGINFO("last succesful time is :%s", str_successfulTime.c_str());
                        /* Remove any old completion time */
                        m_setting.remove("LastSuccessfulCompletionTime");
                        m_setting.setValue("LastSuccessfulCompletionTime",str_successfulTime);

                        MaintenanceManager::_instance->onMaintenanceStatusChange(m_notify_status);
                    }
                    /* Check other than all success case which means we have errors */
                    else if ((g_task_status & ALL_TASKS_SUCCESS)!= ALL_TASKS_SUCCESS) {
                        if ((g_task_status & MAINTENANCE_TASK_SKIPPED ) == MAINTENANCE_TASK_SKIPPED ){
                            LOGINFO("DBG:There are Skipped Task. Incomplete");
                            m_notify_status=MAINTENANCE_INCOMPLETE;
                            /*Check if there any chance to reboot
                             * say we receive a reboot required from rfc */
                        }
                        else {
                            LOGINFO("DBG:There are Errors");
                            m_notify_status=MAINTENANCE_ERROR;
                        }

                        MaintenanceManager::_instance->onMaintenanceStatusChange(m_notify_status);
                    }
                    LOGINFO("ENDING MAINTENANCE CYCLE"); 
                    if(m_thread.joinable()){
                        m_thread.join();
                    }
                }
                else {
                    LOGINFO("Still tasks are not completed!!!!");

                }
            }
            else {
                LOGWARN("Ignoring unexpected event - owner: %s, eventId: %d!!", owner, eventId);
            }
        }
        void MaintenanceManager::DeinitializeIARM()
        {
            if (Utils::IARM::isConnected()){
                IARM_Result_t res;
                IARM_CHECK(IARM_Bus_UnRegisterEventHandler(IARM_BUS_MAINTENANCE_MGR_NAME, IARM_BUS_MAINTENANCEMGR_EVENT_UPDATE));
                IARM_CHECK(IARM_Bus_UnRegisterEventHandler(IARM_BUS_MAINTENANCE_MGR_NAME, IARM_BUS_DCM_NEW_START_TIME_EVENT));
                MaintenanceManager::_instance = nullptr;
            }

            if(m_thread.joinable()){
                m_thread.join();
            }
        }
#endif /* defined(USE_IARMBUS) || defined(USE_IARM_BUS) */

#ifdef DEBUG
        /**
         * @brief : sampleAPI
         */
        uint32_t MaintenanceManager::sampleAPI(const JsonObject& parameters,
                JsonObject& response)
        {
            response["sampleAPI"] = "Success";
            /* Kept for debug purpose/future reference. */
            sendNotify(EVT_ONMAINTMGRSAMPLEEVENT, parameters);
            returnResponse(true);
        }
#endif /* DEBUG */

        /*
         * @brief This function returns the status of the current
         * or previous maintenance activity.
         * @param1[in]: {"jsonrpc":"2.0","id":"3","method":"org.rdk.MaintenanceManager.2.GetMaintenanceActivityStatus",
         *                  "params":{}}''
         * @param2[out]: {"jsonrpc":"2.0","id":3,"result":{"status":MAINTENANCE_IDLE,"LastSuccessfulCompletionTime":
                              -1,"isCriticalMaintenance":true,"isRebootPending":true,}}
         * @return: Core::<StatusCode>
         */

        uint32_t MaintenanceManager::getMaintenanceActivityStatus(const JsonObject& parameters,
                JsonObject& response)
                {
                    bool result = false;
                    string isCriticalMaintenance = "false";
                    string isRebootPending = "false";
                    string LastSuccessfulCompletionTime = "NA"; /* TODO : check max size to hold this */
                    string getMaintenanceStatusString = "\0";
                    bool b_criticalMaintenace=false;
                    bool b_rebootPending=false;

                    std::lock_guard<std::mutex> guard(m_callMutex);

                    /* Check if we have a critical maintenance */
                    if (!g_is_critical_maintenance.empty()){
                        isCriticalMaintenance=g_is_critical_maintenance;
                    }

                    if (!g_is_reboot_pending.empty()){
                        isRebootPending=g_is_reboot_pending;
                    }

                    /* Get the last SuccessfulCompletion time from Persistant location */
                    if (m_setting.contains("LastSuccessfulCompletionTime")){
                        LastSuccessfulCompletionTime=m_setting.getValue("LastSuccessfulCompletionTime").String();
                    }

                    if (!isCriticalMaintenance.compare("true")){
                        b_criticalMaintenace=true;
                    }

                    if(!isRebootPending.compare("true")){
                        b_rebootPending=true;
                    }

                    response["maintenanceStatus"] = notifyStatusToString(g_notify_status);
                    if(strcmp("NA",LastSuccessfulCompletionTime.c_str())==0)
                    {
                       response["LastSuccessfulCompletionTime"] = 0;  // stoi is not able handle "NA"
                    }
                    else
                    {
                       try{
                               response["LastSuccessfulCompletionTime"] = stoi(LastSuccessfulCompletionTime.c_str());
                          }
                       catch(exception &err)
                          {
                              //exception caught with stoi -- So making "LastSuccessfulCompletionTime" as 0
                              response["LastSuccessfulCompletionTime"] = 0;
                          }
                    }
                    response["isCriticalMaintenance"] = b_criticalMaintenace;
                    response["isRebootPending"] = b_rebootPending;
                    result = true;

                    returnResponse(result);
                }
        /*
         * @brief This function returns the start time of the maintenance activity.
         * @param1[in]: {"jsonrpc":"2.0","id":"3","method":"org.rdk.MaintenanceManager.2.GetMaintenanceStartTime","params":{}}''
         * @param2[out]: {"jsonrpc":"2.0","id":3,"result":{"time":03:45","success":true}}
         * @return: Core::<StatusCode>
         */
        uint32_t MaintenanceManager::getMaintenanceStartTime (const JsonObject& parameters,
                JsonObject& response)
        {
            bool result = false;
            string starttime="";
            unsigned long int start_time=0;
            if(!g_epoch_time.empty()) {

                response["maintenanceStartTime"] = stoi(g_epoch_time.c_str());
                result=true;
            }
            else {
                string starttime = Utils::cRunScript("/lib/rdk/getMaintenanceStartTime.sh &");
                if (!starttime.empty()){
                    response["maintenanceStartTime"]=stoi(starttime.c_str());
                    result=true;
                }
            }

            returnResponse(result);
        }

        /*
         * @brief This function returns the current status of the current
         * or previous maintenance activity.
         * @param1[in]: {"jsonrpc":"2.0","id":"3","method":"org.rdk.MaintenanceManager.2.SetMaintenanceMode",
         *                  "params":{"Mode":FOREGROUND}}''
         * @param2[out]: {"jsonrpc":"2.0","id":3,"result":{"success":<bool>}}
         * @return: Core::<StatusCode>
         */

        uint32_t MaintenanceManager::setMaintenanceMode(const JsonObject& parameters,
                JsonObject& response)
        {
            bool result = false;
            string new_mode = "";
            string old_mode = g_currentMode;
            string bg_flag = "false";

            /*  we set the default value to FG */
            if ( parameters.HasLabel("maintenanceMode") ){
                /* Get the value */
                new_mode = parameters["maintenanceMode"].String();

                LOGINFO("SetMaintenanceMode new_mode = %s\n",new_mode.c_str());
                std::lock_guard<std::mutex> guard(m_callMutex);
                /* check if maintenance is on progress or not */
                /* if in progress restrict the same */
                if ( MAINTENANCE_STARTED != g_notify_status ){
                    if ( BACKGROUND_MODE != new_mode && FOREGROUND_MODE != new_mode )  {
                        LOGERR("value of new mode is incorrect, therefore \
                                current mode '%s' not changed.\n", old_mode.c_str());
                        returnResponse(false);
                    }
                    if ( BACKGROUND_MODE == new_mode ) {
                        g_currentMode = new_mode;
                        bg_flag="true";
                        m_setting.setValue("background_flag",bg_flag);
                    }
                    else {
                        /* foreground */
                        g_currentMode =new_mode;
                        m_setting.remove("background_flag");
                        bg_flag="false";
                        m_setting.setValue("background_flag",bg_flag);
                    }
                    result = true;

                }
                else{
                    LOGERR("Maintenance is in Progress, Mode change not allowed");
                    result =false;
                }
            }
            else {
                /* havent got the correct label */
                LOGERR("SetMaintenanceMode Missing Key Values\n");
                populateResponseWithError(SysSrv_MissingKeyValues,response);
            }

            returnResponse(result);
        }

        /*
         * @brief This function starts the maintenance activity.
         * @param1[in]: {"jsonrpc":"2.0","id":"3","method":"org.rdk.MaintenanceManager.2.StartMaintenance",
         *                  "params":{}}''
         * @param2[out]:{"jsonrpc":"2.0","id":3,"result":{"success":<bool>}}
         * @return: Core::<StatusCode>
         */

        uint32_t MaintenanceManager::startMaintenance(const JsonObject& parameters,
                JsonObject& response)
                {
                    bool result = false;
                    int32_t exec_status=E_NOK;
                    Maint_notify_status_t notify_status = MAINTENANCE_IDLE;
                    /* check what mode we currently have */
                    string current_mode="";
                    bool skip_task=false;
                    string abort_flag="";

                    /* only one maintenance at a time */
                    if ( MAINTENANCE_STARTED != g_notify_status  ){

                        /*reset the status to 0*/
                        g_task_status=0;
                        g_maintenance_type=SOLICITED_MAINTENANCE;

                        /* we dont touch the dcm so
                         * we say DCM is success and complete */
                        SET_STATUS(g_task_status,DCM_SUCCESS);
                        SET_STATUS(g_task_status,DCM_COMPLETE);

                        /* isRebootPending will be set to true
                         * irrespective of XConf configuration */
                        g_is_reboot_pending="true";

                        /* we set this to false */
                        g_is_critical_maintenance="false";

                        m_thread = std::thread(&MaintenanceManager::task_execution_thread, _instance);

                        result=true;
                    }
                    else {
                        LOGINFO("Already a maintenance is in Progress. Please wait for it to complete !!");
                    }
                    returnResponse(result);
                }

        void MaintenanceManager::onMaintenanceStatusChange(Maint_notify_status_t status) {
            JsonObject params;
            /* we store the updated value as well */
            g_notify_status=status;
            params["maintenanceStatus"]=notifyStatusToString(status);
            sendNotify(EVT_ONMAINTENANCSTATUSCHANGE, params);
        }

   } /* namespace Plugin */
} /* namespace WPEFramework */

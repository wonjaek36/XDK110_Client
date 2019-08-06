/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development.
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/
/**
 * @ingroup APPS_LIST
 *
 * @defgroup SEND_DATA_OVER_UDP UdpExample
 * @{
 *
 * @brief Send Data Over UDP
 *
 * @details This module is intended to send UDP data to a server.
 * Here the device connects to an access point and obtains an IP address via DHCP.
 * Thereafter it triggers a task for sending data every pre-defined time interval (#APP_UDP_TX_DELAY) ,
 * and sends it over WIFI as a UDP broadcast to a pre-defined server listening on
 * a pre-defined port.
 *
 * @file
 **/
/* module includes ********************************************************** */

/* own header files */
#include "XdkAppInfo.h"
#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_APP_CONTROLLER

/* own header files */
#include "XDK_UDP.h"
#include "AppController.h"

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "XDK_WLAN.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"

/* constant definitions ***************************************************** */

#define APP_CONTROLLER_TX_BUFFER_SIZE        UINT8_C(4)

/* local variables ********************************************************** */
static WLAN_Setup_T WLANSetupInfo =
        {
                .IsEnterprise = false,
                .IsHostPgmEnabled = false,
                .SSID = "Z1-207-2.4G",
                .Password = "Hongik-apl",
                .IsStatic = false
        };/**< WLAN setup parameters */

static uint8_t AppControllerTxBuffer[APP_CONTROLLER_TX_BUFFER_SIZE];/**< This buffer holds the data to be sent to server via UDP */

static CmdProcessor_T * AppCmdProcessor;/**< Handle to store the main Command processor handle to be used by run-time event driven threads */

static xTaskHandle AppControllerHandle = NULL;/**< OS thread handle for Application controller to be used by run-time blocking threads */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief Responsible for controlling the UDP Example application control flow.
 *
 * -Creates a socket for communication
 * -Sends the data to the remote server.
 * -Closes the socket after successful communication.
 *
 * @param[in] pvParameters
 * Unused
 */
static void AppControllerFire(void* pvParameters)
{
    BCDS_UNUSED(pvParameters);

    Retcode_T retcode = RETCODE_OK;
    int16_t handle = 0;
    memset (AppControllerTxBuffer, 0x00, sizeof(AppControllerTxBuffer));
    char ip_address[20];
    retcode = WLAN_ConvertIPAddressToString(1325443264, ip_address);
    if(retcode == RETCODE_OK) {
    	printf("Server ip is: %s\n", ip_address);
    } else {
    	printf("Fail to Convert IP Address To String");
    	assert(0);
    }

    while (1)
    {
        retcode = UDP_Open(&handle);
        if (retcode == RETCODE_OK)
        {
            retcode = UDP_Send(handle, 1325443264, 6666, &AppControllerTxBuffer[0], sizeof(AppControllerTxBuffer));
        }
        if (retcode == RETCODE_OK)
        {
            printf("UDP sending successful\r\n");
            AppControllerTxBuffer[0] += 1;
        }
        else
        {
            Retcode_RaiseError(retcode);
        }
        retcode = UDP_Close(handle);
        if (retcode == RETCODE_OK)
        {
            printf("UDP close successful\r\n");
        }
        else
        {
            Retcode_RaiseError(retcode);
        }
        vTaskDelay(pdMS_TO_TICKS(APP_UDP_TX_DELAY));
    }
}

/**
 * @brief To enable the necessary modules for the application
 * - WLAN
 * - UDP
 *
 * @param[in] param1
 * Unused
 *
 * @param[in] param2
 * Unused
 */
static void AppControllerEnable(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = WLAN_Enable();
    if (RETCODE_OK == retcode)
    {
        retcode = UDP_Enable();
    }
    if (RETCODE_OK == retcode)
    {
        if (pdPASS != xTaskCreate(AppControllerFire, (const char * const ) "AppController", TASK_STACK_SIZE_APP_CONTROLLER, NULL, TASK_PRIO_APP_CONTROLLER, &AppControllerHandle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerEnable : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}

/**
 * @brief To setup the necessary modules for the application
 * - WLAN
 * - UDP
 *
 * @param[in] param1
 * Unused
 *
 * @param[in] param2
 * Unused
 */
static void AppControllerSetup(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);
    Retcode_T retcode = WLAN_Setup(&WLANSetupInfo);
    if (RETCODE_OK == retcode)
    {
        retcode = UDP_Setup(UDP_SETUP_USE_CC31XX_LAYER);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerEnable, NULL, UINT32_C(0));
    }
    if (RETCODE_OK != retcode)
    {
        printf("AppControllerSetup : Failed \r\n");
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}

/* global functions ********************************************************* */

/** Refer interface header for description */
void AppController_Init(void * cmdProcessorHandle, uint32_t param2)
{
    BCDS_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    if (cmdProcessorHandle == NULL)
    {
        printf("AppController_Init : Command processor handle is NULL \r\n");
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        AppCmdProcessor = (CmdProcessor_T *) cmdProcessorHandle;
        retcode = CmdProcessor_Enqueue(AppCmdProcessor, AppControllerSetup, NULL, UINT32_C(0));
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
        assert(0); /* To provide LED indication for the user */
    }
}

/**@} */
/** ************************************************************************* */

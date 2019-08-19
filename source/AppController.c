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
#include "XDK_WLAN.h"
#include "XDK_Sensor.h"
#include "AppController.h"
#include "Json.h"

/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "XDK_WLAN.h"
#include "BCDS_CmdProcessor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Json.h"

/* constant definitions ***************************************************** */

#define APP_TEMPERATURE_OFFSET_CORRECTION    (-3459)    /**< Macro for static temperature offset correction. Self heating, temperature correction factor */
#define APP_CONTROLLER_TX_BUFFER_SIZE        UINT8_C(4)

/* local variables ********************************************************** */
static WLAN_Setup_T WLANSetupInfo =
        {
                .IsEnterprise = false,
                .IsHostPgmEnabled = false,
                .SSID = WLAN_SSID,
                .Password = WLAN_PSK,
                .IsStatic = false
        };/**< WLAN setup parameters */

static uint8_t AppControllerTxBuffer[APP_CONTROLLER_TX_BUFFER_SIZE];/**< This buffer holds the data to be sent to server via UDP */

static CmdProcessor_T * AppCmdProcessor;/**< Handle to store the main Command processor handle to be used by run-time event driven threads */

static xTaskHandle AppControllerHandle = NULL;/**< OS thread handle for Application controller to be used by run-time blocking threads */

static Sensor_Setup_T SensorSetup =
{
	.CmdProcessorHandle = NULL,
	.Enable = {
		.Accel = true,
		.Mag = true,
		.Gyro = true,
		.Humidity = true,
		.Temp = true,
		.Pressure = true,
		.Light = true,
		.Noise = true,
	},
	.Config = {
		.Accel = {
			.Type = SENSOR_ACCEL_BMA280,
			.IsRawData = false,
			.IsInteruptEnabled = false,
			.Callback = NULL,
		},
		.Gyro = {
			.Type = SENSOR_GYRO_BMG160,
			.IsRawData = false,
		},
		.Mag = {
			.IsRawData = false
		},
		.Light =
		{
			.IsInteruptEnabled = false,
			.Callback = NULL,
		},
		.Temp =
		{
			.OffsetCorrection = APP_TEMPERATURE_OFFSET_CORRECTION,
		},
	},
};/**< Sensor setup parameters */

/* global variables ********************************************************* */
#define NUMBER_OF_SENSOR_VAR 15
#define MAX_LENGTH_NAME 10
#define MAX_LENGTH_INT_32 13
#define MAX_LENGTH_FLOAT 13
#define MAX_LENGTH_DOUBLE 20

/* inline functions ********************************************************* */

/* local functions ********************************************************** */
void extractSensorValueAsString(Sensor_Value_T sensorValue, char** sensors);
void allocateMemoryOnSensors(char*** sensors);
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
    Retcode_T retcodeSensor = RETCODE_OK;
    int16_t handle = 0;
    Sensor_Value_T sensorValue;
	char* json = NULL;
	int length_json = 0;
	char** sensors = NULL;

    memset (AppControllerTxBuffer, 0x00, sizeof(AppControllerTxBuffer));
    char ip_address[20];
    retcode = WLAN_ConvertIPAddressToString(DEST_SERVER_IP, ip_address);

    if(retcode == RETCODE_OK) {
    	printf("Server ip is: %s\n", ip_address);
    } else {
    	printf("Fail to Convert IP Address To String");
    	assert(0);
    }

    while (1)
    {
    	json = NULL;
    	sensors = NULL;
    	retcodeSensor = Sensor_GetData(&sensorValue);
    	printf("Return code: %d\n", retcodeSensor);
    	printf("Result: %d\n", RETCODE_OK == retcodeSensor);

    	if (RETCODE_OK == retcodeSensor) {

    		allocateMemoryOnSensors(&sensors);
    		extractSensorValueAsString(sensorValue, sensors);

    		length_json = createJson(&json, sensors, NUMBER_OF_SENSOR_VAR*2);
    	}

    	if (RETCODE_OK == retcodeSensor) {

			retcode = UDP_Open(&handle);
			if (retcode == RETCODE_OK)
			{
				retcode = UDP_Send(handle, DEST_SERVER_IP, DEST_SERVER_PORT, &(json[0]), sizeof(char)*length_json);
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

    	if (sensors != NULL) {
    		for(int i=0;i<NUMBER_OF_SENSOR_VAR*2;i++) {
    			free(sensors[i]);
    		}
    		free(sensors);
    	}
    	if (json != NULL) {
    		free(json);
    	}
    }
}

void extractSensorValueAsString(Sensor_Value_T sensorValue, char** sensors) {

	sprintf(sensors[0], "Accel_X");
	sprintf(sensors[1], "%ld", sensorValue.Accel.X);
	sprintf(sensors[2], "Accel_Y");
	sprintf(sensors[3], "%ld", sensorValue.Accel.Y);
	sprintf(sensors[4], "Accel_Z");
	sprintf(sensors[5], "%ld", sensorValue.Accel.Z);

	sprintf(sensors[6], "Mag_X");
	sprintf(sensors[7], "%ld", sensorValue.Mag.X);
	sprintf(sensors[8], "Mag_Y");
	sprintf(sensors[9], "%ld", sensorValue.Mag.Y);
	sprintf(sensors[10], "Mag_Z");
	sprintf(sensors[11], "%ld", sensorValue.Mag.Z);
	sprintf(sensors[12], "Mag_R");
	sprintf(sensors[13], "%ld", sensorValue.Mag.R);

	sprintf(sensors[14], "Gyro_X");
	sprintf(sensors[15], "%ld", sensorValue.Gyro.X);
	sprintf(sensors[16], "Gyro_Y");
	sprintf(sensors[17], "%ld", sensorValue.Gyro.Y);
	sprintf(sensors[18], "Gyro_Z");
	sprintf(sensors[19], "%ld", sensorValue.Gyro.Z);

	sprintf(sensors[20], "RH");
	sprintf(sensors[21], "%lu", sensorValue.RH);
	sprintf(sensors[22], "Pressure");
	sprintf(sensors[23], "%lu", sensorValue.Pressure);
	sprintf(sensors[24], "Light");
	sprintf(sensors[25], "%lu", sensorValue.Light);

	sprintf(sensors[26], "Noise");
	sprintf(sensors[27], "%g", sensorValue.Noise);
	sprintf(sensors[28], "Temp");
	sprintf(sensors[29], "%g", sensorValue.Temp);
}

void allocateMemoryOnSensors(char*** sensors) {

	(*sensors) = (char**)malloc(sizeof(char*)*(NUMBER_OF_SENSOR_VAR*2));
	int i = 0;
	for(;i<(NUMBER_OF_SENSOR_VAR-2)*2;) {
		*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
		*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_INT_32+2);
	}
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_FLOAT+2);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_DOUBLE+2);
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
		retcode = Sensor_Enable();
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
        SensorSetup.CmdProcessorHandle = AppCmdProcessor;
        retcode = Sensor_Setup(&SensorSetup);
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

/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 *  @file
 *
 *  @brief Configuration header for the AppController.c file.
 *
 */

/* header definition ******************************************************** */
#ifndef APPCONTROLLER_H_
#define APPCONTROLLER_H_

/* local interface declaration ********************************************** */
#include "XDK_Utils.h"

/* local type and macro definitions */

/* local module global variable declarations */

/* local inline function definitions */

/* local type and macro definitions */

//#warning Please configure your WLAN below and remove this line.

/**
 * WLAN_CONNECT_WPA_SSID is the SSID of the WIFI network you want to connect to.
 */
#define WLAN_SSID                        "Z1-207-2.4G"
/**
 * WLAN_CONNECT_WPA_PASS is the WPA/WPA2 passphrase (pre-shared key) of your WIFI network.
 */
#define WLAN_PSK                         "Hongik-apl"

/**
 * DEST_SERVER_IP is the destination server IP address of the web server we will send UDP payloads.
 * If you want to test this example without setting up your own server, you can use publicly available services.
 */
#define DEST_SERVER_IP                  XDK_NETWORK_IPV4(192, 168, 0, 79)

/**
 * DEST_SERVER_PORT is the UDP port to which we will send UDP payloads.
 */
#define DEST_SERVER_PORT                UINT16_C(6666)

/**
 * APP_UDP_TX_DELAY is the UDP packet transmit frequency in milli second.
 */
#define APP_UDP_TX_DELAY                UINT32_C(1000)

/* local function prototype declarations */

/**
 * @brief Gives control to the Application controller.
 *
 * @param[in] cmdProcessorHandle
 * Handle of the main command processor which shall be used based on the application needs
 *
 * @param[in] param2
 * Unused
 */
void AppController_Init(void * cmdProcessorHandle, uint32_t param2);

#endif /* APPCONTROLLER_H_ */

/** ************************************************************************* */

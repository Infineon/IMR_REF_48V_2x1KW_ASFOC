/******************************************************************************
 * Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product").
 * By including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing so
 * agrees to indemnify Cypress against all liability.
******************************************************************************/

/******************************************************************************
 * This library is specifically developed for Infineon Mobile Robot (IMR). The
 * LED boards are based on DEMO_IMR_LED_ISHAPE_V1 and DEMO_IMR_LED_USHAPE_V1.
 * The PCB design data can be found here: 
 * https://www.infineon.com/applications/robotics/development-platform#documents
******************************************************************************/

#ifndef LIBRARIES_IMR_LED_PATTERN_H_
#define LIBRARIES_IMR_LED_PATTERN_H_

#include <stdint.h>
#include "IMR_CAN.h"
#include "IMR_CAN_GLOBAL.h"
#include "cy_syslib.h"

#define BARGRAPH_ENABLED 	0
#define BARGRAPH_CONFIG		2	// 0: full IMR front only
								// 1: full IMR front and back
								// 2: mini IMR front only

typedef enum LED_COLOR_t {
    CYAN,		// similar to OCEAN
    MAGENTA,	// similar to BERRY
    WHITE,		// similar to GREY
    LAWN,
} LED_COLOR_t;

void robotLED2boardLED(bool robotLED[3][56], bool boardLEDs[12][23]);
void boardLED2CANmask(bool boardLED[23], bool preserve_old, uint8_t CANmask[3]);
void setLEDColor(LED_COLOR_t color, uint8_t rgb_color[3]);

CAN_STATUS_t patternCANTX(bool boardLED[23], uint8_t rgb_color[3],
		IMR_CAN_MESSAGE_IDS_t ID, uint32_t offset);

void barGraph(int8_t left_speed, int8_t right_speed);
void ledSnake_shortBoard(int8_t left_speed, int8_t right_speed);

#endif /* LIBRARIES_IMR_LED_PATTERN_H_ */

/*******************************************************************************
* Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
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
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


#pragma once

#include "MCU.h"  // microcontroller
#include "ParamConfig.h" // for fast-loop frequency


//------ Smart Gate Driver ------------
#ifndef USING_SGD
#define USING_SGD
#endif
//------ Position angle sensor --------
#ifndef USING_TLI_5012B
#define USING_TLI_5012B
#endif
//----- CAN communication -------------
#ifndef USING_CAN
#define USING_CAN
#endif
//-------------------------------------

#if defined(USING_SGD)
#include "6EDL_gateway.h"
#endif
#if defined(USING_TLI_5012B)
#include "TLI_5012B.h"
#endif
#if defined(USING_CAN)
#include "IMR_CAN.h"
#endif


#define NOT_SPECIFIED				0	// any unknown types

//----------------------------------------------------------------------------
//--- Power switch types
//----------------------------------------------------------------------------
#define SI_MOSFET					1
#define GAN_FET						2
//----------------------------------------------------------------------------
//--- Power board types
//----------------------------------------------------------------------------
#define BOARD_MOSFET_DUAL			1	// Si_MOSFET
#define BOARD_GAN_DUAL				2	// GaN-FET
//----------------------------------------------------------------------------
//--- Motor types
//----------------------------------------------------------------------------
#define GL60_KV25					1	// IMR motor
#define GM7008L_KV26				2	// R48 motor; TODO: to verify motor parameters


//---------------------------------------------------------------------------
//--- Select power board and motor type
//---------------------------------------------------------------------------
#define MOTOR_TYPE		GL60_KV25	//GM7008L_KV26 //NOT_SPECIFIED
#define BOARD_TYPE		BOARD_MOSFET_DUAL //BOARD_GAN_DUAL //NOT_SPECIFIED
//---------------------------------------------------------------------------
//--- Set status of the position sensors offset calibration
//---------------------------------------------------------------------------
#if defined(USING_TLI_5012B)
#define OFFSET_CAL_DONE		1 	// 0 = offset calibration is not done for both sensors
								// 1 = offset calibration done for both sensors
#endif


//---------------------------------------------------------------------------
//--- Power switch type determination
//---------------------------------------------------------------------------
#if (BOARD_TYPE == BOARD_MOSFET_DUAL)
#define SWITCHING_DEVICE_TYPE	SI_MOSFET
#elif (BOARD_TYPE == BOARD_GAN_DUAL)
#define SWITCHING_DEVICE_TYPE	GAN_FET
#else
#define SWITCHING_DEVICE_TYPE	SI_MOSFET
#endif
//---------------------------------------------------------------------------


void HW_IFACE_Init(uint8_t motor_id);
void HW_IFACE_ConnectFcnPointers(void);

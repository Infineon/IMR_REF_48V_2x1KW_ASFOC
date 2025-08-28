/*******************************************************************************
* Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
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

#include <string.h>

#include "cybsp.h"
#include "cy_pdl.h"
#include "IMR_CAN_GLOBAL.h"
#include "IMRLedPattern.h"

/*******************************************************************************
* Definitions
*******************************************************************************/
#define GUI_CONTROL				0		// 0: speed given by IMR CAN command
										// 1: speed given by Motor Suite GUI
#define CAN_DATA_REFRESH_TIME	100   	// refresh time in msec
#define CAN_SPEED_DIVIDER		32760.0	// divisor for CAN speed as CAN speed
										// 16380 is max. ADC value in pot.
										// for IMR set to 32760
#define DLM_BRD_POSITION		0		// 0: front position 0x407 bar-graph
										// 1: back position 0x406 bar-graph

// Settings for Infineon Mobile Robot (IMR)
#define R_WHEEL 				0.05	// [m]
#define VMAX_ROBOT				3.0		// [m/s]
// theoretical maximum value for the wheel speed
#define SMAX_WHEEL				(VMAX_ROBOT/R_WHEEL) // [rad/s]
#define RADPS2_15BIT			(32768.0/SMAX_WHEEL) // 546.133

/*******************************************************************************
* CAN Macros
*******************************************************************************/
/* CAN-FD message identifier 1*/
#define CANFD_NODE_1            1
/* CAN-FD message identifier 2 (use different for 2nd device) */
#define CANFD_NODE_2            2
/* message Identifier used for this code */
#define USE_CANFD_NODE          CANFD_NODE_1
/* CAN-FD channel number used */

#if defined (CY_DEVICE_PSC3)
#define CANFD_HW_CHANNEL        1
#else
#define CANFD_HW_CHANNEL        0
#endif
/* CAN-FD data buffer index to send data from */
#define CANFD_BUFFER_INDEX      0
/* Maximum incoming data length supported */
#define CANFD_DLC               8

#if defined (CY_DEVICE_PSC3)
#define CANFD_INTERRUPT         canfd_0_interrupts0_1_IRQn
#else
#define CANFD_INTERRUPT         canfd_0_interrupts0_0_IRQn
#endif

#define GPIO_INTERRUPT_PRIORITY (7u)

/* Maximum size of the CAN data to be analyzed */
#define CAN_MAX_DATA_LENGTH		8

/* IRQ Event Source Name */
#define CAN_IRQ_RX_MESSAGE_HANDLER	canfd_rx_callback

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* initialize CAN peripheral */
void can_fd_init(void);
//void set_canfd_filter(uint32_t sfid1, uint32_t sfid2);

void CAN_IRQ_RX_MESSAGE_HANDLER (bool  msg_valid, uint8_t msg_buf_fifo_num,
		cy_stc_canfd_rx_buffer_t* canfd_rx_buf);

void can_rx_speed_cmd(float speed_frac[2]);
void manage_extComm_with_CAN(void);
void init_Timer(void);
void manage_timer(void);

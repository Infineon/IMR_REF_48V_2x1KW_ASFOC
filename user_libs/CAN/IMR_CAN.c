/******************************************************************************
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
******************************************************************************/

#include <stdint.h>
#include <math.h>
#include "IMR_CAN.h"
#include "IMR_CAN_GLOBAL.h"
#include "CtrlVars.h"
#include "Controller.h"
#include "ParamConfig.h"
#include "TLI_5012B.h"

/******************************************************************************
* Definition and Global Variables
******************************************************************************/

/* This is a shared context structure, unique for each can-fd channel */
static cy_stc_canfd_context_t canfd_context;

/* motor speed input with [0] refers to left & [1] refers to right */
int16_t rx_speed[2]; // value received from CAN
float frac_speed[2]; // converted received speed to max. 12-bit resolution
uint8_t speed_percent[2];	// speed in % value to control LED bar graph

volatile extern uint32_t Task100msCnt; // timer counter for CAN transmit
float SPEED_SCALE_4LED = 2.0;	// multiplier to increase number of LED
								// that lights up based on speed value

extern CTRL_VARS_t vars[MOTOR_CTRL_NO_OF_MOTOR];
extern TLI_5012B_ABS_POS_t TLI_5012B_ABS_POS;
extern TLI_5012B_ABS_POS_t TLI_5012B_ABS_POS_M1;

int16_t EncSpdL, EncSpdR; 	// Encoder speed

/******************************************************************************
* Function Prototype
******************************************************************************/
/* can-fd interrupt handler */
static void isr_canfd (void);

/******************************************************************************
* The interrupt handler function for the can-fd interrupt
******************************************************************************/
static void isr_canfd(void) {
    /* Just call the IRQ handler with the current channel number and context */
    Cy_CANFD_IrqHandler(CANFD_HW, CANFD_HW_CHANNEL, &canfd_context);
}

/*******************************************************************************
* Initializes the CAN FD Node
*******************************************************************************/
void can_fd_init(void) {
    cy_en_canfd_status_t status;

    /* Populate the configuration structure for CAN-FD Interrupt */
    cy_stc_sysint_t canfd_irq_cfg =
    {
        /* Source of interrupt signal */
        .intrSrc = CANFD_INTERRUPT,
        /* Interrupt priority */
        .intrPriority = 1U,
    };

    /* Hook the interrupt service routine */
    (void) Cy_SysInt_Init(&canfd_irq_cfg, &isr_canfd);
    /* enable the CAN-FD interrupt */
    NVIC_EnableIRQ(CANFD_INTERRUPT);

    /* Initialize CAN-FD Channel */
    status = Cy_CANFD_Init(CANFD_HW, CANFD_HW_CHANNEL, &CANFD_config,
                           &canfd_context);

    if (status != CY_CANFD_SUCCESS) {
        CY_ASSERT(0);
    }
    /* Setting Node(message) Identifier to global setting of "USE_CANFD_NODE" */
    CANFD_T0RegisterBuffer_0.id = USE_CANFD_NODE;
}

/*******************************************************************************
* Transmit message via CAN bus
* Parameters:
*  CAN_ID                   CAN message identifier (see IMR_CAN_GLOBAL.h)
*  Target_Data              CAN data / the message content in each byte
*  Target_Data_Length       Length of the CAN data in byte
*
* Return:
*  0 for transmit success (CAN_SUCCESS)
*  1 for transmit failure (CAN_ERROR)
*******************************************************************************/
CAN_STATUS_t can_tx_request(uint32_t CAN_ID, uint8_t* Target_Data,
		uint8_t Target_Data_Length)
{
	cy_stc_canfd_t0_t t0 = {
			.id  = CAN_ID,
	        .rtr = CY_CANFD_RTR_DATA_FRAME,
	        .xtd = CY_CANFD_XTD_STANDARD_ID,
	        .esi = CY_CANFD_ESI_ERROR_ACTIVE //CY_CANFD_ESI_ERROR_PASSIVE
	};
	/*cy_stc_canfd_t1_t t1 = {
			.dlc = 15U, //Target_Data_Length,
	        .brs = true, //false, 
	        .fdf = CY_CANFD_FDF_CAN_FD_FRAME, //CY_CANFD_FDF_STANDARD_FRAME,
	        .efc = false,
	        .mm  = 0UL
	};*/
	uint32_t data[16] = {0};
	for (uint8_t i = 0; i < Target_Data_Length; i++) {
		((uint8_t*)data)[i] = Target_Data[i];
	}		
	cy_stc_canfd_tx_buffer_t msg_buffer = {
			.t0_f = &t0,
	        .t1_f = &CANFD_T1RegisterBuffer_0, //&t1,
	        .data_area_f = data
	};
	cy_en_canfd_status_t status =
			Cy_CANFD_UpdateAndTransmitMsgBuffer(CANFD_HW, CANFD_HW_CHANNEL,
					&msg_buffer, CANFD_BUFFER_INDEX, &canfd_context);
	if (status != CY_CANFD_SUCCESS) {
		return CAN_ERROR;
	}
	return CAN_SUCCESS;
}

/*******************************************************************************
* The callback function for can-fd reception
* Parameters:
*    msg_valid                     Message received properly or not
*    msg_buf_fifo_num              RxFIFO number of the received message
*    canfd_rx_buf                  Message buffer
*******************************************************************************/
void CAN_IRQ_RX_MESSAGE_HANDLER(bool  msg_valid, uint8_t msg_buf_fifo_num,
                        cy_stc_canfd_rx_buffer_t* canfd_rx_buf) {
    /* Array to hold the data bytes of the CAN-FD frame */
    uint8_t canfd_data_buffer[CANFD_DLC];
    /* Variable to hold the data length code of the CAN-FD frame */
    uint32_t canfd_dlc;
    /* Variable to hold the Identifier of the CAN-FD frame */
    uint32_t canfd_id;

    if (true == msg_valid) {
        /* Checking whether the frame received is a data frame */
        if(CY_CANFD_RTR_DATA_FRAME == canfd_rx_buf->r0_f->rtr) {
            /* Get message ID and length */
        	canfd_dlc = canfd_rx_buf->r1_f->dlc;
            canfd_id  = canfd_rx_buf->r0_f->id;
            /* Copy data from CAN FIFO to buffer */
            memcpy(canfd_data_buffer,canfd_rx_buf->data_area_f,canfd_dlc);

            /* Using definition to define front (0) and back (1) board */
            /* Negate the received speed due to opposite direction 
               between DLM PSOC C3 code and IMR_MAINCTRL code */
            if (DLM_BRD_POSITION == 0) { // front board
                if (canfd_id == MOT_FL_SPEED_COMMAND) {
            		rx_speed[0] = (int16_t)
            				((uint16_t)canfd_data_buffer[0]) << 8 |
							((uint16_t)canfd_data_buffer[1]);
            		frac_speed[0] = -(float)rx_speed[0] / CAN_SPEED_DIVIDER;
                }
                else if (canfd_id == MOT_FR_SPEED_COMMAND) {
                	rx_speed[1] = (int16_t)
            				((uint16_t)canfd_data_buffer[0]) << 8 |
							((uint16_t)canfd_data_buffer[1]);
                	frac_speed[1] = -(float)rx_speed[1] / CAN_SPEED_DIVIDER;
                }
            }
            else if (DLM_BRD_POSITION == 1) { // back board
                if (canfd_id == MOT_BL_SPEED_COMMAND) {
            		rx_speed[0] = (int16_t)
            				((uint16_t)canfd_data_buffer[0]) << 8 |
							((uint16_t)canfd_data_buffer[1]);
            		frac_speed[0] = -(float)rx_speed[0] / CAN_SPEED_DIVIDER;
                }
                else if (canfd_id == MOT_BR_SPEED_COMMAND) {
                	rx_speed[1] = (int16_t)
            				((uint16_t)canfd_data_buffer[0]) << 8 |
							((uint16_t)canfd_data_buffer[1]);
                	frac_speed[1] = -(float)rx_speed[1] / CAN_SPEED_DIVIDER;
                }
            }
#if (!GUI_CONTROL)
            can_rx_speed_cmd(frac_speed);
#endif
        }
    }
}

/*******************************************************************************
* Function to assign the received speed from CAN to the motor control variables
* Parameter:
*  speed_frac - received speed in fraction (-1.0 to 1.0)
*******************************************************************************/
void can_rx_speed_cmd(float speed_frac[2]) {

    /* Using definition to define front (0) and back (1) board */
    if (DLM_BRD_POSITION == 0) { // front board
        for (int i = 0; i < MOTOR_CTRL_NO_OF_MOTOR; i++) {
            vars[i].cmd_ext = fabs(speed_frac[i]);
            if (speed_frac[i] < 0) vars[i].dir = -1.0;
            else vars[i].dir = 1.0;
            speed_percent[i] = (uint8_t)(int8_t)(speed_frac[i] * 100.0);
        }
	}
	else if (DLM_BRD_POSITION == 1) { // back board
        for (int i = 0; i < MOTOR_CTRL_NO_OF_MOTOR; i++) {
            vars[i].cmd_ext = fabs(speed_frac[MOTOR_CTRL_NO_OF_MOTOR-1-i]);
            if (speed_frac[MOTOR_CTRL_NO_OF_MOTOR-1-i] < 0)
            	vars[i].dir  = -1.0;
            else vars[i].dir = 1.0;
            speed_percent[i] = (uint8_t)(int8_t)
            		(speed_frac[MOTOR_CTRL_NO_OF_MOTOR-1-i] * 100.0);
        }
	}
	if(Task100msCnt >= CAN_DATA_REFRESH_TIME) {
		Task100msCnt = 0;
//Bargraph Pattern for EW & PCIM 2025 demo
#if (BARGRAPH_ENABLED)
#if (BARGRAPH_CONFIG != 2)
	    /* Using definition to define front (0) and back (1) board */
	    if (DLM_BRD_POSITION == 1) { // back board
			barGraph(speed_percent[1] * SPEED_SCALE_4LED,
					 speed_percent[0] * SPEED_SCALE_4LED);
		}
#endif
#if (BARGRAPH_CONFIG == 1)
	    if (DLM_BRD_POSITION == 0) { // front board
			barGraph(speed_percent[0] * SPEED_SCALE_4LED,
					 speed_percent[1] * SPEED_SCALE_4LED);
		}
#endif
#if (BARGRAPH_CONFIG == 2)
		ledSnake_shortBoard(speed_percent[0] * SPEED_SCALE_4LED,
							speed_percent[1] * SPEED_SCALE_4LED);
#endif
#endif
	}
}

/******************************************************************************
* Function Name: manage_extComm_with_CAN
*******************************************************************************
* managing the communication with external subsystems
* currently set with every 100 msec refresh time (10 Hz)
* dependent on IMR_CAN.h (ensure to include) and other files in Libraries
******************************************************************************/
void manage_extComm_with_CAN(void)
{
	CAN_STATUS_t status;

	if(Task100msCnt >= CAN_DATA_REFRESH_TIME) {
		Task100msCnt = 0;
//Bargraph Pattern for EW & PCIM 2025 demo
#if (BARGRAPH_ENABLED)
		uint8_t targetData[CAN_MAX_DATA_LENGTH] = {0};
		for(int i = 0; i < MOTOR_CTRL_NO_OF_MOTOR; i++)
		{
			if (i == CAN_MAX_DATA_LENGTH)
				break;
			targetData[i] = (uint8_t)
				(int8_t)(vars[i].cmd_final * 100.0 * vars[i].dir);
		}
#if (BARGRAPH_CONFIG != 2)
	    if (DLM_BRD_POSITION == 0) { // back board
			barGraph(targetData[1] * SPEED_SCALE_4LED,
					 targetData[0] * SPEED_SCALE_4LED);
		}
#endif
#if (BARGRAPH_CONFIG == 1)
	    /* Using definition to define front (0) and back (1) board */
	    if (DLM_BRD_POSITION == 0) { // front board
			barGraph(targetData[0] * SPEED_SCALE_4LED,
					 targetData[1] * SPEED_SCALE_4LED);
		}
#endif
#if (BARGRAPH_CONFIG == 2)
		ledSnake_shortBoard(targetData[0] * SPEED_SCALE_4LED,
							targetData[1] * SPEED_SCALE_4LED);
#endif
#endif

		uint32_t CAN_MSG_ID;
		uint8_t data_length;

		data_length = 64;
		uint8_t *data = (uint8_t *) calloc(data_length, sizeof(uint8_t));

		/* Using definition to define front (0) and back (1) board */
		if (DLM_BRD_POSITION == 0) { // front board
			CAN_MSG_ID = MOT_FL_ENCODER_DATA;
			/* Negate the transmitted speed due to opposite direction 
               between DLM PSOC C3 code and MAIN_CONTROL code */
			EncSpdL = (int16_t)(-TLI_5012B_ABS_POS.PLL.Omega_flt / 
								(MOTOR_POLE / 2.0) * RADPS2_15BIT);
			data[0] = EncSpdL >> 8 & 0xF;
			data[1] = EncSpdL & 0xFF;
			data[2] = TLI_5012B_ABS_POS.Theta_MechRaw_U16 >> 8 & 0xFF;
			data[3] = TLI_5012B_ABS_POS.Theta_MechRaw_U16 & 0xFF;
			status = can_tx_request(CAN_MSG_ID, data, data_length);
			if (status == CAN_SUCCESS)
				Cy_GPIO_Set(LED_STATUS_PORT, LED_STATUS_PIN);
			else Cy_GPIO_Clr(LED_STATUS_PORT, LED_STATUS_PIN);

			Cy_SysLib_Delay(1);

			CAN_MSG_ID = MOT_FR_ENCODER_DATA;
			EncSpdR = (int16_t)(-TLI_5012B_ABS_POS_M1.PLL.Omega_flt / 
								(MOTOR_POLE_M1 / 2.0) * RADPS2_15BIT);
			data[0] = EncSpdR >> 8 & 0xFF;
			data[1] = EncSpdR & 0xFF;
			data[2] = TLI_5012B_ABS_POS_M1.Theta_MechRaw_U16 >> 8 & 0xFF;
			data[3] = TLI_5012B_ABS_POS_M1.Theta_MechRaw_U16 & 0xFF;
			status = can_tx_request(CAN_MSG_ID, data, data_length);
			if (status == CAN_SUCCESS)
				Cy_GPIO_Set(LED_STATUS_PORT, LED_STATUS_PIN);
			else Cy_GPIO_Clr(LED_STATUS_PORT, LED_STATUS_PIN);
		}
		else if (DLM_BRD_POSITION == 1) { // back board
			CAN_MSG_ID = MOT_BL_ENCODER_DATA;
			EncSpdL = (int16_t)(-TLI_5012B_ABS_POS_M1.PLL.Omega_flt / 
								(MOTOR_POLE_M1 / 2.0) * RADPS2_15BIT);
			data[0] = EncSpdL >> 8 & 0xF;
			data[1] = EncSpdL & 0xFF;
			data[2] = TLI_5012B_ABS_POS_M1.Theta_MechRaw_U16 >> 8 & 0xFF;
			data[3] = TLI_5012B_ABS_POS_M1.Theta_MechRaw_U16 & 0xFF;
			status = can_tx_request(CAN_MSG_ID, data, data_length);
			if (status == CAN_SUCCESS)
				Cy_GPIO_Set(LED_STATUS_PORT, LED_STATUS_PIN);
			else Cy_GPIO_Clr(LED_STATUS_PORT, LED_STATUS_PIN);

			Cy_SysLib_Delay(1);

			CAN_MSG_ID = MOT_BR_ENCODER_DATA;
			EncSpdR = (int16_t)(-TLI_5012B_ABS_POS.PLL.Omega_flt / 
								(MOTOR_POLE / 2.0) * RADPS2_15BIT);
			data[0] = EncSpdR >> 8 & 0xFF;
			data[1] = EncSpdR & 0xFF;
			data[2] = TLI_5012B_ABS_POS.Theta_MechRaw_U16 >> 8 & 0xFF;
			data[3] = TLI_5012B_ABS_POS.Theta_MechRaw_U16 & 0xFF;
			status = can_tx_request(CAN_MSG_ID, data, data_length);
			if (status == CAN_SUCCESS)
				Cy_GPIO_Set(LED_STATUS_PORT, LED_STATUS_PIN);
			else Cy_GPIO_Clr(LED_STATUS_PORT, LED_STATUS_PIN);
		}
		free(data);
	}
}

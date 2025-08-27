/******************************************************************************
* File Name: 6EDL_gateway.h
*
* Description: 6EDL7141 gateway functions header file
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
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


#ifndef PMSM_FOC_MCUINIT_6EDL_GATEWAY_H_
#define PMSM_FOC_MCUINIT_6EDL_GATEWAY_H_


/**
 * @addtogroup PMSM_FOC
 * @{
 */

/**
 * @addtogroup MCUInit
 * @{
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "../6EDL71x1/6EDL_spi.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define Edl7141_1_ParameterBlock_Addr 	(0x10020600)
#define Edl7141_2_ParameterBlock_Addr 	(0x10020C00)

#if 0
/* For 6EDL7141 evaluation */
#define FW_VER                   	0x02   /* for FW version storage */

#define UART_BAUD_RATE            	(115200)
#define XMC_UART_CH_OVERSAMPLING  	(16UL)

/* GUI_6EDL7141_INTEGRATION UART pin assignment */
#define UART_PIN_TX_TO_PC         	P4_4 /* USIC1_CH0.DOUT0, ALT6 */
#define UART_PIN_RX_FROM_PC       	P4_5 /* USIC1_CH0.DX0D */
#define UART_COM_CH               	XMC_UART1_CH0

#define LED_TO_BLINK              	XMC_GPIO_PORT0, 11

extern uint8_t GUIwrReg_6EDL7141_1_addr;
extern uint8_t WriteReg_6EDL7141_1_addr;
extern uint16_t GUIwrReg_6EDL7141_1_data;
extern uint16_t WriteReg_6EDL7141_1_data;
extern uint8_t GuiMonitor_6EDL7141_1_addr;
extern uint16_t GuiMonitor_6EDL7141_1_value;
#endif

#define EDL1	1
#define EDL2	2

typedef struct
{
	uint16_t en_drv_level:1;
	uint16_t nbrake_level:1;
} EDL_IO_CONTROL_t;

typedef struct {
	uint16_t StatusRegister_count;
	uint16_t ConfigRegister_count;
	uint16_t GUIwrReg_6EDL7141_addr;
	uint16_t WriteReg_6EDL7141_addr;
	uint16_t GUIwrReg_6EDL7141_data;
	uint16_t WriteReg_6EDL7141_data;
	uint16_t GuiMonitor_6EDL7141_addr;
	uint16_t GuiMonitor_6EDL7141_value;
} EDL_UPDATE_t;

volatile extern  edl7141_register_t *Edl1ParamFlashBlock_Ptr;//	(edl7141_register_t *)Edl7141_1_ParameterBlock_Addr
volatile extern  edl7141_register_t *Edl2ParamFlashBlock_Ptr;//	(edl7141_register_t *)Edl7141_2_ParameterBlock_Addr

volatile extern edl7141_register_t Edl7141Reg1;
volatile extern edl7141_register_t Edl7141Reg2;

volatile extern EDL_IO_CONTROL_t EdlIo;				//
volatile extern EDL_UPDATE_t EdlUpdate;				//

volatile extern EDL_IO_CONTROL_t EdlIo_M1;			//
volatile extern EDL_UPDATE_t EdlUpdate_M1;			//



/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/
/**
 * @brief Default 6EDL7141 register setting for testing purpose if macro PROGRAM_DEFAULT_PARAM set to 1U
 *
 * @param None
 *
 * @retval None
 */
void EDL7141_MOTOR_PARAM_set_default(edl7141_register_t *ptr);


/**
 * @brief If macro PROGRAM_DEFAULT_PARAM is 1U, it copy default 6EDL7141 register values to FLASH,
 * else it read the chip version from Flash address, if match, it will upload the 6EDL7141 register value from FLASH to RAM,
 * if chip version read back does not match, it will stay in IDLE state.
 *
 * @param None
 *
 * @retval None
 */
uint32_t EDL7141_FLASH_parameter_load(edl7141_register_t* const reg, const uint32_t ParameterBlock_Addr);


/**
 * @brief Slave select, enable SPI, write configure register value into device, read back all register values
 *
 * @param None
 *
 * @retval None
 */
void EDL7141_Config_init(edl7141_register_t* const reg, const uint32_t edl_no);


/**
 * @brief To communicate with PC GUI 6EDL Configurator
 *
 * @param None
 *
 * @retval None
 */
uint32_t EDL7141_Update(EDL_UPDATE_t* const edl_update, EDL_IO_CONTROL_t* const EdlIo, edl7141_register_t* const reg, const uint32_t edl_no, uint32_t error_status);



/**
 * @}
 */

/**
 * @}
 */

#endif /* PMSM_FOC_MCUINIT_6EDL_GATEWAY_H_ */

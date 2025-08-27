/******************************************************************************
* File Name: 6EDL_spi.h
*
* Description: SPI communication API with 6EDL7141 registers header file
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

#ifndef PMSM_FOC_MCUINIT_6EDL_SPI_H_
#define PMSM_FOC_MCUINIT_6EDL_SPI_H_


/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "../6EDL71x1/pmsm_foc_6EDL7141_config.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* SPI configuration */
/* Register write and read definition */
#if (GATE_DRIVER == IFX_6EDL7141)
#define REG_WRITE        0x80
#define REG_READ         0x7F
#elif (GATE_DRIVER == IFX_NN)
#define REG_WRITE        0x7F
#define REG_READ         0x80
#endif

#define BIT_RIGHT_SHIFT  (8)
#define WORD_AND_MASK    (0x0FF)

extern volatile uint32_t EDL_Task1Timer;

/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/
/**
 * @brief Initialize SPI for 6EDL communication
 *
 * @param None
 *
 * @retval None
 */
void SCB_SPI_init_6EDL(void);

/**
 * @brief To send the read command to the SPI channel.
 * The RegAddr is the address of register to read.
 * Data parameter is a pointer to store the two bytes of data read from the register
 *
 * @param None
 *
 * @retval 16 bit unsigned integer
 */
uint16_t read_word_16b(uint8_t RegAddr);

/**
 * @brief To write data to register via SPI channel.
 * The RegAddr is the address of register to write to.
 * Data parameter is a pointer which stored the two bytes of data written to the register
 *
 * @param None
 *
 * @retval None
 */
void write_word_16b(uint8_t RegAddr, uint16_t data);

/**
 * @}
 */

/**
 * @}
 */

#endif /* PMSM_FOC_MCUINIT_6EDL_SPI_H_ */

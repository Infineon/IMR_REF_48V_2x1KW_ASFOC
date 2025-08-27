/******************************************************************************
* File Name: 6EDL_spi.c
*
* Description: SPI communication API with 6EDL7141 registers
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


/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cycfg.h"
#include "6EDL_spi.h"

/*********************************************************************************************************************
 * DATA STRUCTURE
 ********************************************************************************************************************/
volatile uint32_t error_SPI_RX = 0;
volatile uint32_t error_SPI_TX = 0;
volatile uint32_t EDL_Task1Timer = 0;

/*********************************************************************************************************************
 * LOCAL ROUTINE
 ********************************************************************************************************************/


/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/**
 * @brief API to initialize USIC SPI peripherals for 6EDL
 *
 * @param None
 *
 * @retval None
 */

void SCB_SPI_init_6EDL(void)
{
	Cy_SCB_SPI_Init(SPI_6EDL_HW,&SPI_6EDL_config,NULL);
}

/**
 * @brief To send the read command to the SPI channel.
 * The RegAddr is the address of register to read.
 * Data parameter is a pointer to store the two bytes of data read from the register
 *
 * @param None
 *
 * @retval 16 bit unsigned integer
 */
uint16_t read_word_16b(uint8_t RegAddr)
{
  uint16_t read_data = 0;

  //volatile uint8_t addr = RegAddr;
  Cy_SCB_SPI_ClearTxFifo(SPI_6EDL_HW);
  SPI_6EDL_HW->TX_FIFO_WR = RegAddr & REG_READ;
  SPI_6EDL_HW->TX_FIFO_WR = 0x00;
  SPI_6EDL_HW->TX_FIFO_WR = 0x00;

  EDL_Task1Timer = 0;
  while(Cy_SCB_SPI_GetNumInRxFifo(SPI_6EDL_HW) != 3)
  {
	  if (EDL_Task1Timer > 1)
	  {
		  error_SPI_RX++;
	      break;
	  }
  };
  Cy_SCB_SPI_ClearRxFifoStatus(SPI_6EDL_HW,CY_SCB_SPI_RX_NOT_EMPTY);

  uint8_t data_high, data_low;
  SPI_6EDL_HW->RX_FIFO_RD;
  data_high = SPI_6EDL_HW->RX_FIFO_RD; //data read of MSB
  data_low = SPI_6EDL_HW->RX_FIFO_RD;  //data read of LSB
  read_data = ((data_high << 8) + data_low);
  return read_data;
}

/**
 * @brief To write data to register via SPI channel.
 * The RegAddr is the address of register to write to.
 * Data parameter is a pointer which stored the two bytes of data written to the register
 *
 * @param None
 *
 * @retval None
 */
uint32_t fifo_val;
uint32_t rx_val;
void write_word_16b(uint8_t RegAddr, uint16_t data)
{
	Cy_SCB_SPI_ClearTxFifo(SPI_6EDL_HW);
	SPI_6EDL_HW->TX_FIFO_WR = RegAddr | REG_WRITE;
	SPI_6EDL_HW->TX_FIFO_WR = (uint8_t)(data >> 8);
	SPI_6EDL_HW->TX_FIFO_WR = (uint8_t)data;

	EDL_Task1Timer = 0;
	while(Cy_SCB_SPI_GetNumInRxFifo(SPI_6EDL_HW) != 3)
	{
		if (EDL_Task1Timer > 1)
		{
			error_SPI_TX++;
			break;
		}
	};
	Cy_SCB_SPI_ClearRxFifoStatus(SPI_6EDL_HW,CY_SCB_SPI_RX_NOT_EMPTY);
	rx_val = SPI_6EDL_HW->RX_FIFO_RD;
	rx_val = SPI_6EDL_HW->RX_FIFO_RD;
	rx_val = SPI_6EDL_HW->RX_FIFO_RD;
}

/* --- End of File ------------------------------------------------ */

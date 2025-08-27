/******************************************************************************
* File Name: TLI_5012_config.h
*
* Description: Define the TLI_5012 register's address and register's field
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
#ifndef PMSM_FOC_TLI_5012_H_
#define PMSM_FOC_TLI_5012_H_

#include "cy_pdl.h"
#include "cycfg.h"
#include "stdbool.h"
#include "stdint.h"


/**
 * @addtogroup PMSM_FOC
 * @{
 */

/**
 * @addtogroup Configuration
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define MASTER_ERROR_MASK  (CY_SCB_SPI_SLAVE_TRANSFER_ERR  | CY_SCB_SPI_TRANSFER_OVERFLOW | CY_SCB_SPI_TRANSFER_UNDERFLOW)

/* Initialization status */
#define INIT_SUCCESS            (0)
#define INIT_FAILURE            (1)

/************************************************
 * TLI_5012 register address (Unchangeable)
 ************************************************/

//--- TLI-5012B Registers ---
#define ADDR_STAT		0x00	// Status Register
#define ADDR_ACSTAT     0x01    // Activation Status Register
#define ADDR_AVAL       0x02    // Angle Value Register
#define ADDR_ASPD       0x03    // Angle SPeed Register
#define ADDR_AREV       0x04    // Angle Revolution Register
#define ADDR_FSYNC      0x05    // Frame Synchronization Register
#define ADDR_MOD_1      0x06    // Interface Mode 1 Register
#define ADDR_SIL        0x07    // SIL Register
#define ADDR_MOD_2      0x08    // Interface Mode 2 Register
#define ADDR_MOD_3      0x09    // Interface Mode 3 Register
#define ADDR_OFFX       0x0A    // Offset X Register
#define ADDR_OFFY       0x0B    // Offset Y Register
#define ADDR_SYNCH      0x0C    // Synchronicity Register
#define ADDR_IFAB       0x0D    // IFAB Register
#define ADDR_MOD_4      0x0E    // Interface Mode 4 Register
#define ADDR_TCO_Y      0x0F    // Temperature Coefficient Register
#define ADDR_ADC_X      0x10    // X-Raw Value Register
#define ADDR_ADC_Y		0x11    // Y-Raw Value Register
#define ADDR_D_MAG		0x14    // D_MAG Register
#define ADDR_T_RAW		0x15    // T_RAW Register
#define ADDR_IIF_CNT	0x20    // IIF Counter Value Register
#define ADDR_T250      	0x30    // Temperature 25degC Offset Register

#define READ_CMD_Msk	0x8001	// Command to read a value from TLI_5012
#define WRITE_CMD_Msk	0x0401	// Command to write a value to TLI_5012

typedef struct
{
	uint16_t CMD;			// the command word transmitted to the slave
	uint16_t POS;			// the data word received from the slave
	uint16_t SPD;			// the data word received from the slave
	uint16_t SAFETY_WORD;	// the safety word received from the slave
} TLI_5012_SSC_RD_POS_SPD_t;

typedef struct
{
	uint16_t CMD;			// the command word transmitted to the slave
	uint16_t POS;			// the data word received from the slave
	uint16_t SAFETY_WORD;	// the safety word received from the slave
} TLI_5012_SSC_RD_POS_t;

typedef struct
{
	uint16_t CMD;			// the command word transmitted to the slave
	uint16_t DATA1;			// the data word transmitted to the slave
	uint16_t SAFETY_WORD;	// the safety word received from the slave
} TLI_5012_SSC_WRT_t;

typedef union
{
	uint16_t all;
	struct {
		uint16_t ND:   4;		// Number of Data Words
		uint16_t ADDR: 6;		// 6-bit address of the internal register
		uint16_t UPD:  1;		// Update-Register Access: 0 (Access to current values), 1(Access to values in update buffer)
		uint16_t LOCK: 4;		// 4-bit Lock Value: 0000 (Default Operating Access for Addresses 0x00 - 0x04), 1010 (Configuration Access for Addresses 0x05 - 0x11)
		uint16_t RW:   1;		// 1(Read)/0(Write)
	} bit;
} TLI_5012_CMD_WORD_t;


/******************************************************************************
  * @brief TLI_5012 registers
  * Access to register members:
  ****************************************************************************/
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t S_RST:     1;
		uint16_t S_WD:      1;
		uint16_t S_VR:      1;
		uint16_t S_FUSE:    1;
		uint16_t S_DSPU:    1;
		uint16_t S_OV:      1;
		uint16_t S_XYOL:    1;
		uint16_t S_MAGOL:   1;
		uint16_t RES0:      1;
		uint16_t S_ADCT:    1;
		uint16_t S_ROM:     1;
		uint16_t NO_GMR_XY: 1;
		uint16_t NO_GMR_A:  1;
		uint16_t S_NR:      2;
		uint16_t RD_ST:     1;
	} bit;
} TLI_5012_STAT_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t AS_RST:     1;
		uint16_t AS_WD:      1;
		uint16_t AS_VR:      1;
		uint16_t AS_FUSE:    1;
		uint16_t AS_DSPU:    1;
		uint16_t AS_OV:      1;
		uint16_t AS_VEC_XY:  1;
		uint16_t AS_VEC_MAC: 1;
		uint16_t RES0:       1;
		uint16_t AS_ADCT:    1;
		uint16_t AS_FRST:    1;
		uint16_t RES1:       5;
	} bit;
} TLI_5012_ACSTAT_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t ANG_VAL: 15;
		uint16_t RD_AV:    1;
	} bit;
} TLI_5012_AVAL_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t ANG_SPD: 15;
		uint16_t RD_AS:    1;
	} bit;
} TLI_5012_ASPD_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t REVOL:  9;
		uint16_t FCNT:   6;
		uint16_t RD_REV: 1;
	} bit;
} TLI_5012_AREV_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t TEMPER:  9;
		uint16_t FSYNC:   7;
	} bit;
} TLI_5012_FSYNC_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t IIF_MD:    2;
		uint16_t DSPU_HOLD: 1;
		uint16_t RES0:      1;
		uint16_t CLK_SEL:   1;
		uint16_t RES1:      9;
		uint16_t FIR_MD:    2;
	} bit;
} TLI_5012_MOD_1_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t ADCTV_X:   3;
		uint16_t ADCTV_Y:   3;
		uint16_t ADCTV_EN:  1;
		uint16_t RES0:      3;
		uint16_t FUSE_REL:  1;
		uint16_t RES1:      3;
		uint16_t FILT_INV:  1;
		uint16_t FILT_PAR:  1;
	} bit;
} TLI_5012_SIL_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t AUTOCAL:     2;
		uint16_t PREDICT:     1;
		uint16_t ANG_DIR:     1;
		uint16_t ANG_RANVGE: 11;
		uint16_t RES0:        1;
	} bit;
} TLI_5012_MOD_2_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t PAD_DRV:   2;
		uint16_t SSC_OD:    1;
		uint16_t SPIKEF:    1;
		uint16_t ANG_BASE: 12;
	} bit;
} TLI_5012_MOD_3_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t RES0:      4;
		uint16_t X_OFFSET: 12;
	} bit;
} TLI_5012_OFFX_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t RES0:      4;
		uint16_t Y_OFFSET: 12;
	} bit;
} TLI_5012_OFFY_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t RES0:   4;
		uint16_t SYNCH: 12;
	} bit;
} TLI_5012_SYNCH_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t IFAB_HYST: 2;
		uint16_t IFAB_OD:   1;
		uint16_t FIR_UDR:   1;
		uint16_t ORTHO:    12;
	} bit;
} TLI_5012_IFAB_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t IF_MD:    2;
		uint16_t RES0:     1;
		uint16_t IFAB_RES: 2;
		uint16_t HSM_PLP:  4;
		uint16_t TCO_X_T:  7;
	} bit;
} TLI_5012_MOD_4_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t CRC_PAR: 8;
		uint16_t SBIST:   1;
		uint16_t TCO_Y_T: 7;
	} bit;
} TLI_5012_TCO_Y_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t ADC_X: 16;
	} bit;
} TLI_5012_ADC_X_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t ADC_Y: 16;
	} bit;
} TLI_5012_ADC_Y_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t MAG: 10;
		uint16_t RES0: 6;
	} bit;
} TLI_5012_D_MAG_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t T_TRW: 10;
		uint16_t RES0:   5;
		uint16_t T_TGL:  1;
	} bit;
} TLI_5012_T_TRW_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t IIF_CNT: 14;
		uint16_t RES0:     2;
	} bit;
} TLI_5012_IIF_CNT_t;
//---------------------------------------
typedef union
{
	uint16_t all;
	struct
	{
		uint16_t RES0: 9;
		uint16_t T25O: 7;
	} bit;
} TLI_5012_T_25O_t;

typedef uint16_t TLI_5012_RES_t;

typedef union
{
	uint16_t    table[0x30 + 2];									// 64[byte]
	struct
	{
		TLI_5012_STAT_t	   STAT;		// 0x00
		TLI_5012_ACSTAT_t  ACSTAT;		// 0x01
		TLI_5012_AVAL_t    AVAL;		// 0x02
		TLI_5012_ASPD_t    ASPD;		// 0x03
		TLI_5012_AREV_t    AREV;		// 0x04
		TLI_5012_FSYNC_t   FSYNC;		// 0x05
		TLI_5012_MOD_1_t   MOD_1;		// 0x06
		TLI_5012_SIL_t     SIL;			// 0x07
		TLI_5012_MOD_2_t   MOD_2;		// 0x08
		TLI_5012_MOD_3_t   MOD_3;		// 0x09
		TLI_5012_OFFX_t    OFFX;		// 0x0A
		TLI_5012_OFFY_t    OFFY;		// 0x0B
		TLI_5012_SYNCH_t   SYNCH;		// 0x0C
		TLI_5012_IFAB_t    IFAB;		// 0x0D
		TLI_5012_MOD_4_t   MOD_4;		// 0x0E
		TLI_5012_TCO_Y_t   TCO_Y;		// 0x0F

		TLI_5012_ADC_X_t   ADC_X;		// 0x10
		TLI_5012_ADC_Y_t   ADC_Y;		// 0x11
		TLI_5012_RES_t	   RES1;		// 0x12
		TLI_5012_RES_t	   RES2;		// 0x13
		TLI_5012_D_MAG_t   D_MAG;		// 0x14
		TLI_5012_T_TRW_t   T_TRW;		// 0x15
		TLI_5012_RES_t	   RES3;		// 0x16
		TLI_5012_RES_t	   RES4;		// 0x17
		TLI_5012_RES_t	   RES5;		// 0x18
		TLI_5012_RES_t	   RES6;		// 0x19
		TLI_5012_RES_t	   RES7;		// 0x1A
		TLI_5012_RES_t	   RES8;		// 0x1B
		TLI_5012_RES_t	   RES9;		// 0x1C
		TLI_5012_RES_t	   RES10;		// 0x1D
		TLI_5012_RES_t	   RES11;		// 0x1E
		TLI_5012_RES_t	   RES12;		// 0x1F

		TLI_5012_IIF_CNT_t IIF_CNT;		// 0x20
		TLI_5012_RES_t	   RES13;		// 0x21
		TLI_5012_RES_t	   RES14;		// 0x22
		TLI_5012_RES_t	   RES15;		// 0x23
		TLI_5012_RES_t	   RES16;		// 0x24
		TLI_5012_RES_t	   RES17;		// 0x25
		TLI_5012_RES_t	   RES18;		// 0x26
		TLI_5012_RES_t	   RES19;		// 0x27
		TLI_5012_RES_t	   RES20;		// 0x28
		TLI_5012_RES_t	   RES21;		// 0x29
		TLI_5012_RES_t	   RES22;		// 0x2A
		TLI_5012_RES_t	   RES23;		// 0x2B
		TLI_5012_RES_t	   RES24;		// 0x2C
		TLI_5012_RES_t	   RES25;		// 0x2D
		TLI_5012_RES_t	   RES26;		// 0x2E
		TLI_5012_RES_t	   RES27;		// 0x2F

		TLI_5012_T_25O_t   T_25O;		// 0x30
	};
} TLI_5012B_REG_t;


#include "../TLx_5012B/PIPLL_Qxx.h"
typedef struct {
	uint16_t Theta_MechRaw_U16;			// mechanical angle value
	uint16_t Theta_TLI_5012B_U16;		//
	int16_t  _ENC_SENSOR_OFST_S16;		// [cnt] TLI-5012 Position Sensor Offset: -32768 ~ 32767[cnt] corresponding to -180.0 ~ 180.0[deg]
	int16_t  Dir_TLI_5012B;				//
	float    Theta_TLI_5012B_flt;		//

	uint16_t PLLSmplCnt;
	PIPLL_t PLL;
} TLI_5012B_ABS_POS_t;


/******************************************************************************
 * Extern Variables                                                           *
******************************************************************************/
extern volatile uint32_t SPI_TLI_5012B_Rx_Buf[3];

extern TLI_5012B_ABS_POS_t TLI_5012B_ABS_POS;
extern TLI_5012B_ABS_POS_t TLI_5012B_ABS_POS_M1;

/******************************************************************************
 * Function Prototypes                                                        *
******************************************************************************/
uint32_t Config_SPI_TLI_5012B_TxDMA(void);	// Configure/initialize DMA for SPI Tx Interface
uint32_t Config_SPI_TLI_5012B_RxDMA(void);  // Configure/initialize DMA for SPI Rx Interface
uint32_t Init_SPI_TLI_5012B(void);			// Initialize the SPI to interface with TLI-5012B

static inline __attribute__((always_inline)) void Rqst_TLI_5012B_POS(cy_en_scb_spi_slave_select_t TLI_5012B_SLAVE_ID)	// Request the TLI-5012B Position Feedback every Task0 control loop
{
	Cy_SCB_SPI_SetActiveSlaveSelect(SPI_TLI_HW, TLI_5012B_SLAVE_ID);			// Set active slave select to line 0
	Cy_DMA_Channel_Enable(TLI_5012_RX_DMA_HW, TLI_5012_RX_DMA_CHANNEL);			// Enable DMA channel to transfer 12 bytes of data from sSPI RX-FIFO to rxBuffer.
	Cy_DMA_Channel_Enable(TLI_5012_TX_DMA_HW, TLI_5012_TX_DMA_CHANNEL);			// Enable DMA channel to transfer 12 bytes of data from txBuffer into mSPI TX-FIFO
	Cy_DMA_Channel_SetSWTrigger(TLI_5012_TX_DMA_HW, TLI_5012_TX_DMA_CHANNEL);	// SW Based Trigger for SPI Tx DMA to collect the TLI-5012B Position and Speed
}

#define TLI_5012B_POS	((uint16_t)((SPI_TLI_5012B_Rx_Buf[1] & 0x7FFF) << 1))	// The position information out of TLI_5012B via SPI + RxDMA every Task0 control loop


static inline __attribute__((always_inline)) void Get_Pos_TLI_5012B(TLI_5012B_ABS_POS_t *pos, int32_t motor_id, float motor_pole_nums)
{
	uint16_t TLI_5012B_Raw_U16 = TLI_5012B_POS;
	Rqst_TLI_5012B_POS(motor_id);
	pos->Theta_MechRaw_U16 = -(TLI_5012B_Raw_U16); //to have positive value in CCW direction
	pos->Theta_TLI_5012B_U16 = ( (((int16_t)TLI_5012B_Raw_U16)*((int16_t)(motor_pole_nums*0.5f + 0.5f))) + pos->_ENC_SENSOR_OFST_S16 ) * pos->Dir_TLI_5012B;
	pos->Theta_TLI_5012B_flt = PU_TO_FLOAT(((q_t)(int16_t)pos->Theta_TLI_5012B_U16), THETA_BASE, Qxx_MAX);

	#if 1
	if (pos->PLLSmplCnt >= 10)
	{
		PIPLLMain(&pos->PLL, pos->Theta_TLI_5012B_U16);
	}
	else
	{
		PresetPIPLL(&pos->PLL, 0, 0, pos->Theta_TLI_5012B_U16);
		pos->PLLSmplCnt++;
	}
	#endif
}



//-----------------------------------------------------------------------------
//-- Power up/down the TLI-5012B
//-----------------------------------------------------------------------------
extern void PwrUp_Enc1(void);
extern void PwrUp_Enc2(void);
extern void PwrDn_Enc1(void);
extern void PwrDn_Enc2(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* PMSM_FOC_CONFIGURATION_PMSM_FOC_TLI_5012_CONFIG_H_ */

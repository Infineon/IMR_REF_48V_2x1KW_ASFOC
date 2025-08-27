/******************************************************************************
* File Name: 6EDL_gateway.c
*
* Description: 6EDL7141 gateway functions
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
//#include "pmsm_foc_user_input_config.h"
//#include "pmsm_foc_error_handling.h"

#include "stdint.h"
#include "cycfg.h"
#include "HardwareIface.h"
#include "6EDL_gateway.h"

#define SPI_6EDL7141_1_CS_LOW()		Cy_GPIO_Write(SPI_SS_6EDL_1_PORT, SPI_SS_6EDL_1_PIN, GPIO_OUTPUT_LEVEL_LOW);
#define SPI_6EDL7141_1_CS_HIGH()	Cy_GPIO_Write(SPI_SS_6EDL_1_PORT, SPI_SS_6EDL_1_PIN, GPIO_OUTPUT_LEVEL_HIGH);
#define SPI_6EDL7141_2_CS_LOW()		Cy_GPIO_Write(SPI_SS_6EDL_2_PORT, SPI_SS_6EDL_2_PIN, GPIO_OUTPUT_LEVEL_LOW);
#define SPI_6EDL7141_2_CS_HIGH()	Cy_GPIO_Write(SPI_SS_6EDL_2_PORT, SPI_SS_6EDL_2_PIN, GPIO_OUTPUT_LEVEL_HIGH);

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/
volatile extern uint32_t error_SPI_RX;
volatile extern uint32_t error_SPI_TX;
volatile edl7141_register_t *Edl1ParamFlashBlock_Ptr;//	(edl7141_register_t *)Edl7141_1_ParameterBlock_Addr
volatile edl7141_register_t *Edl2ParamFlashBlock_Ptr;//	(edl7141_register_t *)Edl7141_2_ParameterBlock_Addr

/* 6EDL EN_DRV pin */
#if defined (__ICCARM__)
#pragma diag_suppress=Pe188
#endif

#if defined (__ICCARM__)
#pragma diag_default=Pe188
#endif

volatile EDL_IO_CONTROL_t EdlIo;				//
volatile EDL_UPDATE_t EdlUpdate;				//

volatile EDL_IO_CONTROL_t EdlIo_M1;			//
volatile EDL_UPDATE_t EdlUpdate_M1;			//

volatile edl7141_register_t Edl7141Reg1 =
{
	#if (EDL7141_CHIP_VERSION == 21)
	.FAULTS_CLR =
	0 << FAULTS_CLR_CLR_FLTS_Pos |
	0 << FAULTS_CLR_CLR_LATCH_Pos,			// 16
	#if (INVTR_TYPE == SI_MOSFET_INVTR)
	.SUPPLY_CFG =
	PVCC_12V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
	#else
	.SUPPLY_CFG =
	PVCC_7V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
	#endif
	VREF_DVDD_1_2 << SUPPLY_CFG_CS_REF_CFG_Pos |
	OCP_THR_450 << SUPPLY_CFG_DVDD_OCP_CFG_Pos |
	DVDD_SFTSTRT_100us << SUPPLY_CFG_DVDD_SFTSTRT_Pos |
	DVDD_3_3 << SUPPLY_CFG_DVDD_SETPT_Pos |
	BK_FREQ_500K << SUPPLY_CFG_BK_FREQ_Pos |
	DVDD_TON_DELAY_800us << SUPPLY_CFG_DVDD_TON_DELAY_Pos |
	CP_PRECHAR_DIS << SUPPLY_CFG_CP_PRECHAR_EN_Pos,

	.ADC_CFG =
	NO_ADC_REQ << ADC_CFG_ADC_OD_REQ_Pos |
	ADC_IN_IDIGITAL << ADC_CFG_ADC_OD_INSEL_Pos |
	ADC_DIS_FILTER << ADC_CFG_ADC_EN_FILT_Pos |
	ADC_FILT_SAMP_8 << ADC_CFG_ADC_FILT_CFG_Pos |
	ADC_FILT_PSAMP_32 << ADC_CFG_ADC_FILT_CFG_PVDD_Pos,

	.PWM_CFG =
	PWM_COMM_MODE_6 << PWM_CFG_PWM_MODE_Pos |
	DIODE_FREEW << PWM_CFG_PWM_FREEW_CFG_Pos |
	BRAKE_LOW << PWM_CFG_BRAKE_CFG_Pos |
	BRAKE_RECIRC_DIS << PWM_CFG_PWM_RECIRC_Pos,

	.SENSOR_CFG =
	HALL_DEGLITCH_640ns << SENSOR_CFG_HALL_DEGLITCH_Pos |
	OTEMP_PROT_DIS << SENSOR_CFG_OTS_DIS_Pos |
	CS_ACTIVE_GLx_H << SENSOR_CFG_CS_TMODE_Pos,

	.WD_CFG =
	WATCHDOG_CLOCK << WD_CFG_WD_EN_Pos |
	WDIN_DRV << WD_CFG_WD_INSEL_Pos |
	WDOUT_STATUS << WD_CFG_WD_FLTCFG_Pos |
	WD_PERIOD_100us << WD_CFG_WD_TIMER_T_Pos,

	.WD_CFG2 =
	WD_REACT_NFAULT << WD_CFG2_WD_BRAKE_Pos |
	WD_FLT_LATCH_DIS << WD_CFG2_WD_EN_LATCH_Pos |
	RSTRT_ATT0 << WD_CFG2_WD_DVDD_RSTRT_ATT_Pos |
	DVDD_RSTRT_DLY_0ms5 << WD_CFG2_WD_DVDD_RSTRT_DLY_Pos |
	RLOCK_DIS << WD_CFG2_WD_RLOCK_EN_Pos |
	RLOCK_WD_TMOUT_1s << WD_CFG2_WD_RLOCK_T_Pos |
	BUCK_EN << WD_CFG2_WD_BK_DIS_Pos,

	.IDRIVE_CFG =
	IDRIVE_20mA << IDRIVE_CFG_IHS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_IHS_SINK_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SINK_Pos,

	.IDRIVE_PRE_CFG =
	IDRIVE_PRE_50mA << IDRIVE_PRE_CFG_I_PRE_SRC_Pos |
	IDRIVE_PRE_100mA << IDRIVE_PRE_CFG_I_PRE_SINK_Pos |
	PRECHAR_MODE_SRC_EN << IDRIVE_PRE_CFG_I_PRE_SRC_EN_Pos |
	PRECHAR_MODE_SNK_EN << IDRIVE_PRE_CFG_I_PRE_SNK_EN_Pos,

	.TDRIVE_SRC_CFG =
	TDRIVE1_70ns << TDRIVE_SRC_CFG_TDRIVE1_Pos |
	TDRIVE2_260ns << TDRIVE_SRC_CFG_TDRIVE2_Pos,

	.TDRIVE_SINK_CFG =
	TDRIVE3_70ns << TDRIVE_SINK_CFG_TDRIVE3_Pos |
	TDRIVE4_510ns << TDRIVE_SINK_CFG_TDRIVE4_Pos,

	.DT_CFG =
	DT_760ns << DT_CFG_DT_RISE_Pos |
	DT_760ns << DT_CFG_DT_FALL_Pos,

	.CP_CFG =
	CP_CLK_781_25kHz << CP_CFG_CP_CLK_CFG_Pos |
	CP_CLK_SS_EN << CP_CFG_CP_CLK_SS_DIS_Pos,

	.CSAMP_CFG =
	CS_GAIN_12V << CSAMP_CFG_CS_GAIN_Pos |
	CS_GAIN_PROG_DIG << CSAMP_CFG_CS_GAIN_ANA_Pos |
	CS_A_EN_B_EN_C_EN << CSAMP_CFG_CS_EN_Pos |
	CS_BLANK_0ns << CSAMP_CFG_CS_BLANK_Pos |
	CS_CALIB_DIS << CSAMP_CFG_CS_EN_DCCAL_Pos |
	CS_DEGLITCH_8us << CSAMP_CFG_CS_OCP_DEGLITCH_Pos |
	OCP_FLT_TRIG_8 << CSAMP_CFG_CS_OCPFLT_CFG_Pos,

	.CSAMP_CFG2 =
	OCP_POS_THR_300mV << CSAMP_CFG2_CS_OCP_PTHR_Pos |
	OCP_NEG_THR_300mV << CSAMP_CFG2_CS_OCP_NTHR_Pos |
	OCP_FLT_LATCH_DIS << CSAMP_CFG2_CS_OCP_LATCH_Pos |
	CS_SENSE_SHUNT_RES << CSAMP_CFG2_CS_MODE_Pos |
	OCP_FLT_BRAKE_DIS << CSAMP_CFG2_CS_OCP_BRAKE_Pos |
	OCP_PWM_TRUNC_DIS << CSAMP_CFG2_CS_TRUNC_DIS_Pos | /* OCP_PWM_TRUNC_EN_POS: Enable cycle-by-cycle current limit */
	// Nitin: CS_VREF_INT bit is not present in 6EDL7151
	//                        CS_VREF_INT << CSAMP_CFG2_VREF_INSEL_Pos |
	OCP_NEG_EN << CSAMP_CFG2_CS_NEG_OCP_DIS_Pos |
	/* external setting is used in flash memory setting*/
	CS_AUTOZERO_EN << CSAMP_CFG2_CS_AZ_CFG_Pos,

	.OTP_PROG =
	0 << OTP_PROG_OTP_PROG_Pos |
	0x0 << OTP_PROG_USER_ID_Pos, /* write a value different to the value in OTP, this helps to detect unexpected IC reset */

	#endif //(EDL7141_CHIP_VERSION == 21)
	.chip_version = EDL7141_CHIP_VERSION,
};

volatile edl7141_register_t Edl7141Reg2 =
{
	#if (EDL7141_CHIP_VERSION == 21)
	.FAULTS_CLR =
	0 << FAULTS_CLR_CLR_FLTS_Pos |
	0 << FAULTS_CLR_CLR_LATCH_Pos,			// 16
	#if (INVTR_TYPE == SI_MOSFET_INVTR)
	.SUPPLY_CFG =
	PVCC_12V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
	#else
	.SUPPLY_CFG =
	PVCC_7V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
	#endif
	VREF_DVDD_1_2 << SUPPLY_CFG_CS_REF_CFG_Pos |
	OCP_THR_450 << SUPPLY_CFG_DVDD_OCP_CFG_Pos |
	DVDD_SFTSTRT_100us << SUPPLY_CFG_DVDD_SFTSTRT_Pos |
	DVDD_3_3 << SUPPLY_CFG_DVDD_SETPT_Pos |
	BK_FREQ_500K << SUPPLY_CFG_BK_FREQ_Pos |
	DVDD_TON_DELAY_800us << SUPPLY_CFG_DVDD_TON_DELAY_Pos |
	CP_PRECHAR_DIS << SUPPLY_CFG_CP_PRECHAR_EN_Pos,

	.ADC_CFG =
	NO_ADC_REQ << ADC_CFG_ADC_OD_REQ_Pos |
	ADC_IN_IDIGITAL << ADC_CFG_ADC_OD_INSEL_Pos |
	ADC_DIS_FILTER << ADC_CFG_ADC_EN_FILT_Pos |
	ADC_FILT_SAMP_8 << ADC_CFG_ADC_FILT_CFG_Pos |
	ADC_FILT_PSAMP_32 << ADC_CFG_ADC_FILT_CFG_PVDD_Pos,

	.PWM_CFG =
	PWM_COMM_MODE_6 << PWM_CFG_PWM_MODE_Pos |
	DIODE_FREEW << PWM_CFG_PWM_FREEW_CFG_Pos |
	BRAKE_LOW << PWM_CFG_BRAKE_CFG_Pos |
	BRAKE_RECIRC_DIS << PWM_CFG_PWM_RECIRC_Pos,

	.SENSOR_CFG =
	HALL_DEGLITCH_640ns << SENSOR_CFG_HALL_DEGLITCH_Pos |
	OTEMP_PROT_DIS << SENSOR_CFG_OTS_DIS_Pos |
	CS_ACTIVE_GLx_H << SENSOR_CFG_CS_TMODE_Pos,

	.WD_CFG =
	WATCHDOG_CLOCK << WD_CFG_WD_EN_Pos |
	WDIN_DRV << WD_CFG_WD_INSEL_Pos |
	WDOUT_STATUS << WD_CFG_WD_FLTCFG_Pos |
	WD_PERIOD_100us << WD_CFG_WD_TIMER_T_Pos,

	.WD_CFG2 =
	WD_REACT_NFAULT << WD_CFG2_WD_BRAKE_Pos |
	WD_FLT_LATCH_DIS << WD_CFG2_WD_EN_LATCH_Pos |
	RSTRT_ATT0 << WD_CFG2_WD_DVDD_RSTRT_ATT_Pos |
	DVDD_RSTRT_DLY_0ms5 << WD_CFG2_WD_DVDD_RSTRT_DLY_Pos |
	RLOCK_DIS << WD_CFG2_WD_RLOCK_EN_Pos |
	RLOCK_WD_TMOUT_1s << WD_CFG2_WD_RLOCK_T_Pos |
	BUCK_EN << WD_CFG2_WD_BK_DIS_Pos,

	.IDRIVE_CFG =
	IDRIVE_20mA << IDRIVE_CFG_IHS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_IHS_SINK_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SINK_Pos,

	.IDRIVE_PRE_CFG =
	IDRIVE_PRE_50mA << IDRIVE_PRE_CFG_I_PRE_SRC_Pos |
	IDRIVE_PRE_100mA << IDRIVE_PRE_CFG_I_PRE_SINK_Pos |
	PRECHAR_MODE_SRC_EN << IDRIVE_PRE_CFG_I_PRE_SRC_EN_Pos |
	PRECHAR_MODE_SNK_EN << IDRIVE_PRE_CFG_I_PRE_SNK_EN_Pos,

	.TDRIVE_SRC_CFG =
	TDRIVE1_70ns << TDRIVE_SRC_CFG_TDRIVE1_Pos |
	TDRIVE2_260ns << TDRIVE_SRC_CFG_TDRIVE2_Pos,

	.TDRIVE_SINK_CFG =
	TDRIVE3_70ns << TDRIVE_SINK_CFG_TDRIVE3_Pos |
	TDRIVE4_510ns << TDRIVE_SINK_CFG_TDRIVE4_Pos,

	.DT_CFG =
	DT_760ns << DT_CFG_DT_RISE_Pos |
	DT_760ns << DT_CFG_DT_FALL_Pos,

	.CP_CFG =
	CP_CLK_781_25kHz << CP_CFG_CP_CLK_CFG_Pos |
	CP_CLK_SS_EN << CP_CFG_CP_CLK_SS_DIS_Pos,

	.CSAMP_CFG =
	CS_GAIN_12V << CSAMP_CFG_CS_GAIN_Pos |
	CS_GAIN_PROG_DIG << CSAMP_CFG_CS_GAIN_ANA_Pos |
	CS_A_EN_B_EN_C_EN << CSAMP_CFG_CS_EN_Pos |
	CS_BLANK_0ns << CSAMP_CFG_CS_BLANK_Pos |
	CS_CALIB_DIS << CSAMP_CFG_CS_EN_DCCAL_Pos |
	CS_DEGLITCH_8us << CSAMP_CFG_CS_OCP_DEGLITCH_Pos |
	OCP_FLT_TRIG_8 << CSAMP_CFG_CS_OCPFLT_CFG_Pos,

	.CSAMP_CFG2 =
	OCP_POS_THR_300mV << CSAMP_CFG2_CS_OCP_PTHR_Pos |
	OCP_NEG_THR_300mV << CSAMP_CFG2_CS_OCP_NTHR_Pos |
	OCP_FLT_LATCH_DIS << CSAMP_CFG2_CS_OCP_LATCH_Pos |
	CS_SENSE_SHUNT_RES << CSAMP_CFG2_CS_MODE_Pos |
	OCP_FLT_BRAKE_DIS << CSAMP_CFG2_CS_OCP_BRAKE_Pos |
	OCP_PWM_TRUNC_DIS << CSAMP_CFG2_CS_TRUNC_DIS_Pos | /* OCP_PWM_TRUNC_EN_POS: Enable cycle-by-cycle current limit */
	// Nitin: CS_VREF_INT bit is not present in 6EDL7151
	//                        CS_VREF_INT << CSAMP_CFG2_VREF_INSEL_Pos |
	OCP_NEG_EN << CSAMP_CFG2_CS_NEG_OCP_DIS_Pos |
	/* external setting is used in flash memory setting*/
	CS_AUTOZERO_EN << CSAMP_CFG2_CS_AZ_CFG_Pos,

	.OTP_PROG =
	0 << OTP_PROG_OTP_PROG_Pos |
	0x0 << OTP_PROG_USER_ID_Pos, /* write a value different to the value in OTP, this helps to detect unexpected IC reset */

	#endif //(EDL7141_CHIP_VERSION == 21)
	.chip_version = EDL7141_CHIP_VERSION,
};


/**
 * @brief Default 6EDL7141 register setting for testing purpose if macro PROGRAM_DEFAULT_PARAM set to 1U
 *
 * @param None
 *
 * @retval None
 */
void EDL7141_MOTOR_PARAM_set_default(edl7141_register_t *prm)
{
	#if (EDL7141_CHIP_VERSION == 21)
	prm->FAULTS_CLR =
	0 << FAULTS_CLR_CLR_FLTS_Pos |
	0 << FAULTS_CLR_CLR_LATCH_Pos;			// 16
	#if (INVTR_TYPE == SI_MOSFET_INVTR)
	prm->SUPPLY_CFG =
	PVCC_12V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
  	#else
	prm->SUPPLY_CFG =
	PVCC_7V << SUPPLY_CFG_PVCC_SETPT_Pos |	// 17
	#endif
	VREF_DVDD_1_2 << SUPPLY_CFG_CS_REF_CFG_Pos |
	OCP_THR_450 << SUPPLY_CFG_DVDD_OCP_CFG_Pos |
	DVDD_SFTSTRT_100us << SUPPLY_CFG_DVDD_SFTSTRT_Pos |
	DVDD_3_3 << SUPPLY_CFG_DVDD_SETPT_Pos |
	BK_FREQ_500K << SUPPLY_CFG_BK_FREQ_Pos |
	DVDD_TON_DELAY_800us << SUPPLY_CFG_DVDD_TON_DELAY_Pos |
	CP_PRECHAR_DIS << SUPPLY_CFG_CP_PRECHAR_EN_Pos;

	prm->ADC_CFG =
	NO_ADC_REQ << ADC_CFG_ADC_OD_REQ_Pos |
	ADC_IN_IDIGITAL << ADC_CFG_ADC_OD_INSEL_Pos |
	ADC_DIS_FILTER << ADC_CFG_ADC_EN_FILT_Pos |
	ADC_FILT_SAMP_8 << ADC_CFG_ADC_FILT_CFG_Pos |
	ADC_FILT_PSAMP_32 << ADC_CFG_ADC_FILT_CFG_PVDD_Pos;

	prm->PWM_CFG =
	PWM_COMM_MODE_6 << PWM_CFG_PWM_MODE_Pos |
	DIODE_FREEW << PWM_CFG_PWM_FREEW_CFG_Pos |
	BRAKE_LOW << PWM_CFG_BRAKE_CFG_Pos |
	BRAKE_RECIRC_DIS << PWM_CFG_PWM_RECIRC_Pos;

	prm->SENSOR_CFG =
	HALL_DEGLITCH_640ns << SENSOR_CFG_HALL_DEGLITCH_Pos |
	OTEMP_PROT_DIS << SENSOR_CFG_OTS_DIS_Pos |
	CS_ACTIVE_GLx_H << SENSOR_CFG_CS_TMODE_Pos;

	prm->WD_CFG =
	WATCHDOG_CLOCK << WD_CFG_WD_EN_Pos |
	WDIN_DRV << WD_CFG_WD_INSEL_Pos |
	WDOUT_STATUS << WD_CFG_WD_FLTCFG_Pos |
	WD_PERIOD_100us << WD_CFG_WD_TIMER_T_Pos;

	prm->WD_CFG2 =
	WD_REACT_NFAULT << WD_CFG2_WD_BRAKE_Pos |
	WD_FLT_LATCH_DIS << WD_CFG2_WD_EN_LATCH_Pos |
	RSTRT_ATT0 << WD_CFG2_WD_DVDD_RSTRT_ATT_Pos |
	DVDD_RSTRT_DLY_0ms5 << WD_CFG2_WD_DVDD_RSTRT_DLY_Pos |
	RLOCK_DIS << WD_CFG2_WD_RLOCK_EN_Pos |
	RLOCK_WD_TMOUT_1s << WD_CFG2_WD_RLOCK_T_Pos |
	BUCK_EN << WD_CFG2_WD_BK_DIS_Pos;

	prm->IDRIVE_CFG =
	IDRIVE_20mA << IDRIVE_CFG_IHS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_IHS_SINK_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SRC_Pos |
	IDRIVE_20mA << IDRIVE_CFG_ILS_SINK_Pos;

	prm->IDRIVE_PRE_CFG =
	IDRIVE_PRE_50mA << IDRIVE_PRE_CFG_I_PRE_SRC_Pos |
	IDRIVE_PRE_100mA << IDRIVE_PRE_CFG_I_PRE_SINK_Pos |
	PRECHAR_MODE_SRC_EN << IDRIVE_PRE_CFG_I_PRE_SRC_EN_Pos |
	PRECHAR_MODE_SNK_EN << IDRIVE_PRE_CFG_I_PRE_SNK_EN_Pos;

	prm->TDRIVE_SRC_CFG =
	TDRIVE1_70ns << TDRIVE_SRC_CFG_TDRIVE1_Pos |
	TDRIVE2_260ns << TDRIVE_SRC_CFG_TDRIVE2_Pos;

	prm->TDRIVE_SINK_CFG =
	TDRIVE3_70ns << TDRIVE_SINK_CFG_TDRIVE3_Pos |
	TDRIVE4_510ns << TDRIVE_SINK_CFG_TDRIVE4_Pos;

	prm->DT_CFG =
	DT_760ns << DT_CFG_DT_RISE_Pos |
	DT_760ns << DT_CFG_DT_FALL_Pos;

	prm->CP_CFG =
	CP_CLK_781_25kHz << CP_CFG_CP_CLK_CFG_Pos |
	CP_CLK_SS_EN << CP_CFG_CP_CLK_SS_DIS_Pos;

	prm->CSAMP_CFG =
	CS_GAIN_12V << CSAMP_CFG_CS_GAIN_Pos |
	CS_GAIN_PROG_DIG << CSAMP_CFG_CS_GAIN_ANA_Pos |
	CS_A_EN_B_EN_C_EN << CSAMP_CFG_CS_EN_Pos |
	CS_BLANK_0ns << CSAMP_CFG_CS_BLANK_Pos |
	CS_CALIB_DIS << CSAMP_CFG_CS_EN_DCCAL_Pos |
	CS_DEGLITCH_8us << CSAMP_CFG_CS_OCP_DEGLITCH_Pos |
	OCP_FLT_TRIG_8 << CSAMP_CFG_CS_OCPFLT_CFG_Pos;

	prm->CSAMP_CFG2 =
	OCP_POS_THR_300mV << CSAMP_CFG2_CS_OCP_PTHR_Pos |
	OCP_NEG_THR_300mV << CSAMP_CFG2_CS_OCP_NTHR_Pos |
	OCP_FLT_LATCH_DIS << CSAMP_CFG2_CS_OCP_LATCH_Pos |
	CS_SENSE_SHUNT_RES << CSAMP_CFG2_CS_MODE_Pos |
	OCP_FLT_BRAKE_DIS << CSAMP_CFG2_CS_OCP_BRAKE_Pos |
	OCP_PWM_TRUNC_DIS << CSAMP_CFG2_CS_TRUNC_DIS_Pos | /* OCP_PWM_TRUNC_EN_POS: Enable cycle-by-cycle current limit */
	// Nitin: CS_VREF_INT bit is not present in 6EDL7151
	//                        CS_VREF_INT << CSAMP_CFG2_VREF_INSEL_Pos |
	OCP_NEG_EN << CSAMP_CFG2_CS_NEG_OCP_DIS_Pos |
	/* external setting is used in flash memory setting*/
	CS_AUTOZERO_EN << CSAMP_CFG2_CS_AZ_CFG_Pos;

	prm->OTP_PROG =
	0 << OTP_PROG_OTP_PROG_Pos |
	0x0 << OTP_PROG_USER_ID_Pos; /* write a value different to the value in OTP, this helps to detect unexpected IC reset */

	#endif //(EDL7141_CHIP_VERSION == 21)
	prm->chip_version = EDL7141_CHIP_VERSION;
}

/**
 * @brief If macro PROGRAM_DEFAULT_PARAM is 1U, it copy default 6EDL7141 register values to FLASH,
 * else it read the chip version from Flash address, if match, it will upload the 6EDL7141 register value from FLASH to RAM,
 * if chip version read back does not match, it will stay in IDLE state.
 *
 * @param None
 *
 * @retval None
 */
#if 0
uint32_t EDL7141_FLASH_parameter_load(edl7141_register_t* const reg, const uint32_t ParameterBlock_Addr)
{
	uint32_t Edl7141Configured;

	/* MC_INFO structure contains expected parameter version for the firmware. Head of parameter block should contain
	 * same value, otherwise parameter block won't be loaded
	 */
	#if (PROGRAM_DEFAULT_PARAM == 1)
	/* For test only, program the parameter block with default value */
	/* Configure with default parameter */
	EDL7141_MOTOR_PARAM_set_default(reg);

	//--- Override the current feedback OP-Amp gain for the standalone version ----
	uint16_t CSAMP_CFG = reg->CSAMP_CFG & (~CSAMP_CFG_CS_GAIN_Msk);
	#if (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_4)
	CSAMP_CFG |= (CS_GAIN_4V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_8)
	CSAMP_CFG |= (CS_GAIN_8V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_12)
	CSAMP_CFG |= (CS_GAIN_12V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_16)
	CSAMP_CFG |= (CS_GAIN_16V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_20)
	CSAMP_CFG |= (CS_GAIN_20V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_24)
	CSAMP_CFG |= (CS_GAIN_24V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_32)
	CSAMP_CFG |= (CS_GAIN_32V << CSAMP_CFG_CS_GAIN_Pos);
	#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_64)
	CSAMP_CFG |= (CS_GAIN_64V << CSAMP_CFG_CS_GAIN_Pos);
	#else
	CSAMP_CFG |= (CS_GAIN_16V << CSAMP_CFG_CS_GAIN_Pos);
	#endif
	reg->CSAMP_CFG = CSAMP_CFG;

	/* Write default parameter into parameter block */
	Cy_Flash_WriteRow(ParameterBlock_Addr, (uint32_t*) reg);
	Edl7141Configured = TRUE;

	#else
	reg->chip_version = ((edl7141_register_t*)ParameterBlock_Addr)->chip_version;
	if (reg->chip_version != EDL7141_CHIP_VERSION)
	{
		/* If there is no valid parameter block */
		/* Do nothing, state machine will stay at IDLE state, until all parameters (including chip_version) are configured by master control */
		Edl7141Configured = FALSE;
	}
	else
	{
		/* Found parameter block with matching version, copy parameter block data into Mtr1.Edl7141Reg structure */
		edl7141_register_t *parameter_block = (edl7141_register_t*)ParameterBlock_Addr;
		for (int32_t i = 0; i < CFG_ADDR_MAX; i++)
		{
			reg->table[i] = parameter_block->table[i];
		}

		#if USER_UCPROBE_GUI == 0
		//--- Override the current feedback OP-Amp gain for the standalone version ----
		uint16_t CSAMP_CFG = reg->CSAMP_CFG & (~CSAMP_CFG_CS_GAIN_Msk);
		#if (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_4)
		CSAMP_CFG |= (CS_GAIN_4V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_8)
		CSAMP_CFG |= (CS_GAIN_8V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_12)
		CSAMP_CFG |= (CS_GAIN_12V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_16)
		CSAMP_CFG |= (CS_GAIN_16V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_20)
		CSAMP_CFG |= (CS_GAIN_20V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_24)
		CSAMP_CFG |= (CS_GAIN_24V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_32)
		CSAMP_CFG |= (CS_GAIN_32V << CSAMP_CFG_CS_GAIN_Pos);
		#elif (OP_AMP_GAIN_DFLT == OP_AMP_GAIN_64)
		CSAMP_CFG |= (CS_GAIN_64V << CSAMP_CFG_CS_GAIN_Pos);
		#else
		CSAMP_CFG |= (CS_GAIN_16V << CSAMP_CFG_CS_GAIN_Pos);
		#endif
		reg->CSAMP_CFG = CSAMP_CFG;
		#endif

		Edl7141Configured = TRUE;
	}
	#endif //PROGRAM_DEFAULT_PARAM

	return Edl7141Configured;
}
#endif

/**
 * @brief To read all the 6EDL7141 registers when startup
 *
 * @param None
 *
 * @retval None
 */
void SPI_read_6EDL7141_registers(edl7141_register_t* const reg)
{
  /* Read status registers */
  for (uint32_t addr = ST_ADDR_MIN; addr < ST_ADDR_MAX; addr++)
  {
    reg->table[addr] = read_word_16b(addr);
  }
  /* Read parameter registers */
  for (uint32_t addr = CFG_ADDR_MIN; addr < CFG_ADDR_MAX; addr++)
  {
    reg->table[addr] = read_word_16b(addr);
  }
}

/**
 * @brief To write the 6EDL7141 configuration registers to default values
 *
 * @param None
 *
 * @retval None
 */
void SPI_write_6EDL7141_registers(edl7141_register_t* const reg)
{
  /* Write default parameters, exclude: FAULTS_CLR & OTP_PROG */
  for (uint32_t addr = CFG_ADDR_MIN; addr < CFG_ADDR_MAX; addr++)
  {
    write_word_16b(addr, reg->table[addr]);
  }
}

/**
 * @brief API to initialize USIC SPI peripherals
 *
 * @param None
 *
 * @retval None
 */
void SPI_master_init_6EDL(cy_en_scb_spi_slave_select_t slave_select)
{
	/* SlaveSelect should be done before the Cy_SCB_SPI_Enable */
	/* Set active slave select to line 0 */
	Cy_SCB_SPI_SetActiveSlaveSelect(SPI_6EDL_HW, slave_select);

	Cy_SCB_SPI_Enable(SPI_6EDL_HW);
}

/**
 * @brief Initialize SPI for 6EDL communication, write configure register value into device, read back all register values
 *
 * @param None
 *
 * @retval None
 */
void EDL7141_Config_init(edl7141_register_t* const reg, const uint32_t edl_no)
{
	/* Initialize SPI Master module for slave 0 */
	if (edl_no == 1)
		 SPI_master_init_6EDL(CY_SCB_SPI_SLAVE_SELECT0);	// Motor1 EDL smart gate driver
	else SPI_master_init_6EDL(CY_SCB_SPI_SLAVE_SELECT1);	// Motor2 EDL smart gate driver

	/* To write the 6EDL7141 configuration registers to default values */
	SPI_write_6EDL7141_registers(reg);
	/* read all the 6EDL7141 registers to Mtr1.Edl7141Reg table */
	SPI_read_6EDL7141_registers(reg);

	Cy_SCB_SPI_Disable(SPI_6EDL_HW, NULL);
}

/**
 * @brief To communicate with PC GUI 6EDL Configurator
 *
 * @param None
 *
 * @retval None
 */
uint32_t EDL7141_Update(EDL_UPDATE_t* const edl_update, EDL_IO_CONTROL_t* const EdlIo, edl7141_register_t* const reg, const uint32_t edl_no, uint32_t error_status)
{
	static uint16_t pre_en_drv_level1 = 0;
	static uint16_t pre_en_drv_level2 = 0;
	#ifdef USING_N_BRAKE
	static uint16_t pre_nbrake_level1 = 0;
	static uint16_t pre_nbrake_level2 = 0;
	#endif

	uint16_t *pre_en_drv_level_ptr;
	#ifdef USING_N_BRAKE
	uint16_t *pre_nbrake_level_ptr;
	#endif

	uint16_t data;
	uint8_t addr;

	/* SlaveSelect should be done before the Cy_SCB_SPI_Enable */
	/* Set active slave select to line 0 */
	if (edl_no == EDL1)
		 Cy_SCB_SPI_SetActiveSlaveSelect(SPI_6EDL_HW, CY_SCB_SPI_SLAVE_SELECT0);
	else Cy_SCB_SPI_SetActiveSlaveSelect(SPI_6EDL_HW, CY_SCB_SPI_SLAVE_SELECT1);

	Cy_SCB_SPI_Enable(SPI_6EDL_HW);

	if (edl_update->GUIwrReg_6EDL7141_addr <= CFG_ADDR_MAX) 		/* Handle write request from GUI */
	{
		addr = edl_update->GUIwrReg_6EDL7141_addr;
		write_word_16b(addr, edl_update->GUIwrReg_6EDL7141_data); 	/* Write to 6EDL7141 */
		data = read_word_16b(addr); 								/* Read back from 6EDL7141 */
		reg->table[addr] = data; 									/* Update register table */
		edl_update->GUIwrReg_6EDL7141_addr = 0x55; 					/* Indicate write operation is done */
	}
	if (edl_update->WriteReg_6EDL7141_addr <= CFG_ADDR_MAX) 		/* Handle write request from control */
	{
		addr = edl_update->WriteReg_6EDL7141_addr;
		write_word_16b(addr, edl_update->WriteReg_6EDL7141_data); 	/* Write to 6EDL7141 */
		data = read_word_16b(addr); 								/* Read back from 6EDL7141 */
		reg->table[addr] = data; 									/* Update register table */
		edl_update->WriteReg_6EDL7141_addr = 0x55; 					/* Indicate write operation is done */
	}

	/* Every 2ms, always read fault status register */
	reg->table[ADDR_FAULT_ST] = read_word_16b(ADDR_FAULT_ST);

	if (edl_update->StatusRegister_count <= 6)
	{
		/* Every 2ms, read one of remaining 7 status register from address 1-7 */
		edl_update->StatusRegister_count++;
		reg->table[edl_update->StatusRegister_count] = read_word_16b(edl_update->StatusRegister_count);
	}
	else
	{
		/* After read all the status registers, read one of configure registers */
		edl_update->StatusRegister_count = 0;
		addr = edl_update->ConfigRegister_count + CFG_ADDR_MIN;
		reg->table[addr] = read_word_16b(addr); /* Read one of configure registers */

		if (edl_update->ConfigRegister_count >= CFG_ADDR_MAX - CFG_ADDR_MIN)
		{
			edl_update->ConfigRegister_count = 0;
		}
		else
		{
			edl_update->ConfigRegister_count++;
		}

		/* Update 6EDL GUI monitor value */
		edl_update->GuiMonitor_6EDL7141_value = reg->table[edl_update->GuiMonitor_6EDL7141_addr];
	}

	/* fault handling */
	if (error_SPI_RX)
	{
		error_status = 1	;//|= 1 << PMSM_FOC_EID_SPI_FAULT_POS;
//		error_SPI = 0;
	}


	//--------------------------------------------------------------
	//--- EDL EN_DRV and nBRAKE
	//--------------------------------------------------------------
	if (edl_no == EDL1)
	{
		pre_en_drv_level_ptr = &pre_en_drv_level1;
		#ifdef USING_N_BRAKE
		pre_nbrake_level_ptr = &pre_nbrake_level1;
		#endif
	}
	else
	{
		pre_en_drv_level_ptr = &pre_en_drv_level2;
		#ifdef USING_N_BRAKE
		pre_nbrake_level_ptr = &pre_nbrake_level2;
		#endif
	}

	/* Control EN_DRV_1 pin output */
	if (EdlIo->en_drv_level == 0)
	{
		if (edl_no == EDL1)
			 Cy_GPIO_Clr(EN_DRV_SGD_PORT, 	   EN_DRV_SGD_PIN);
		else Cy_GPIO_Clr(EN_DRV_SGD_M1_PORT, EN_DRV_SGD_M1_PIN);
	}
	else
	{
		if (edl_no == EDL1)
			 Cy_GPIO_Set(EN_DRV_SGD_PORT,    EN_DRV_SGD_PIN);
		else Cy_GPIO_Set(EN_DRV_SGD_M1_PORT, EN_DRV_SGD_M1_PIN);

		if (*pre_en_drv_level_ptr == 0) /* rising edge of EdlIo_1.en_drv_level */
		{
			/* As there is no state defined when EN_DRV_1 is disabled, there is a need to reset the PI output and integrator values
			 * when the en_drv_level becomes 1 */
//    		PMSM_FOC_SPEED_PI_Reset(&pmsm_foc_speed_pi_motor_1);
//        	PMSM_FOC_PI_Reset(&pmsm_foc_flux_pi_motor_1);
		}
	}
	*pre_en_drv_level_ptr = EdlIo->en_drv_level;

	#ifdef USING_N_BRAKE
	if (EdlIo->nbrake_level == 0)
	{
		if (edl_no == EDL1)
			 Cy_GPIO_Clr(nBRAKE_1_PORT, nBRAKE_1_PIN);
		else Cy_GPIO_Clr(nBRAKE_2_PORT, nBRAKE_2_PIN);
	}
	else
	{
		if (error_status != 0)
		{
			if (edl_no == EDL1)
				 Cy_GPIO_Clr(nBRAKE_1_PORT, nBRAKE_1_PIN);
			else Cy_GPIO_Clr(nBRAKE_2_PORT, nBRAKE_2_PIN);
		}
		else
		{
			if (edl_no == EDL1)
				 Cy_GPIO_Set(nBRAKE_1_PORT, nBRAKE_1_PIN);
			else Cy_GPIO_Set(nBRAKE_2_PORT, nBRAKE_2_PIN);

			if (*pre_nbrake_level_ptr == 0) /* rising edge of EdlIo.nbrake_level */
			{
				/* As there is no state defined when nbrake is disabled, there is a need to reset the PI output and integrator values
				 * when the nbrake_level becomes 1 */
//				PMSM_FOC_SPEED_PI_Reset(&pmsm_foc_speed_pi_motor_1);
//				PMSM_FOC_PI_Reset(&pmsm_foc_flux_pi_motor_1);
			}
		}
	}
	*pre_nbrake_level_ptr = EdlIo->nbrake_level;
	#endif

	Cy_SCB_SPI_Disable(SPI_6EDL_HW, NULL);

	return error_status;
}
/* --- End of File ------------------------------------------------ */

/******************************************************************************
* File Name:   main.c
*
* Description: This code example demonstrates the implementation of PMSM sensorless
* field-oriented control (FOC) for 2 motors using the Infineon's MCUs.
*
* Related Document: See README.md
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "General.h"
#include "HardwareIface.h"
#include "cybsp.h"
#include "Controller.h"
#include "Params.h"
#include "MotorCtrlHWConfig.h"
#include "6EDL_gateway.h"
#include "TLI_5012B.h"
#include "CtrlVars.h"

/*******************************************************************************
* Global variable
********************************************************************************/
/* XMC7x - GCC_ARM: EEPROM storage */
#if defined(COMPONENT_CAT1C)
uint8_t Em_Eeprom_Storage[srss_0_eeprom_0_PHYSICAL_SIZE] __attribute__ ((section(".cy_em_eeprom")));
#endif
/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
//----------- Smart gate driver -----------------
volatile extern uint32_t Task10msCnt;
volatile uint32_t EDL7141_UpdateFg = 0;
//-----------------------------------------------

int main(void)
{
    cy_rslt_t result;
    
    #if defined(COMPONENT_CAT1C)// Disabled the D-CACHE for XMC7200 device. 
    SCB_DisableDCache();
    #endif
    
    result = cybsp_init();                 /* Initialize the device and board peripherals */
    CY_ASSERT(result == CY_RSLT_SUCCESS);  /* Board init failed. Stop program execution   */
   
    // Initialise controller
    HW_IFACE_ConnectFcnPointers();         /* must be called before STATE_MACHINE_Init()  */

    //---------------------------------------------------------
    //--- Initialise the SPI for 6EDL smart gate driver
    //---------------------------------------------------------
    #ifdef USING_SGD
    SCB_SPI_init_6EDL();		// Initialize SCB for 6EDL interface
    EDL7141_MOTOR_PARAM_set_default((edl7141_register_t *)&Edl7141Reg1);
	EDL7141_MOTOR_PARAM_set_default((edl7141_register_t *)&Edl7141Reg2);
	EDL7141_Config_init((edl7141_register_t *)&Edl7141Reg1, EDL1);
	EDL7141_Config_init((edl7141_register_t *)&Edl7141Reg2, EDL2);
	Cy_GPIO_Set(EN_DRV_SGD_PORT,    EN_DRV_SGD_PIN   );	EdlIo.en_drv_level    = 1;	// Enable the 6EDL driver 1
	Cy_GPIO_Set(EN_DRV_SGD_M1_PORT, EN_DRV_SGD_M1_PIN);	EdlIo_M1.en_drv_level = 1;	// Enable the 6EDL driver 2
    #endif
    //---------------------------------------------------------

    //---------------------------------------------------------
    //--- Initialise the SPI for TLI-5012 sensor interface
    //---------------------------------------------------------
	#ifdef USING_TLI_5012B
    uint32_t status = Init_SPI_TLI_5012B();
    if (INIT_FAILURE == status)
    {
    	CY_ASSERT(0);
    }

    status = Config_SPI_TLI_5012B_TxDMA();
    if (INIT_FAILURE == status)
    {
    	CY_ASSERT(0);
    }

    status = Config_SPI_TLI_5012B_RxDMA();
    if (INIT_FAILURE == status)
    {
    	CY_ASSERT(0);
    }
    PwrUp_Enc1();
    PwrUp_Enc2();
    
    // Add delay 1sec before initialising state machine
    // to ensure 2 boards can operate at the same time without faults
    Cy_SysLib_Delay(1000);
    STATE_MACHINE_Init();

    #if OFFSET_CAL_DONE
    motor[0].params_ptr->sys.fb.mode = Direct; //AqB_Enc;
    motor[1].params_ptr->sys.fb.mode = Direct; //AqB_Enc;
    motor[0].params_ptr->ctrl.mode   = Speed_Mode_FOC_Encoder_Align_Startup;
    motor[1].params_ptr->ctrl.mode   = Speed_Mode_FOC_Encoder_Align_Startup;
	#endif
	
    InitPIPLL(&TLI_5012B_ABS_POS.PLL,    0, TLI_5012B_POS);
    InitPIPLL(&TLI_5012B_ABS_POS_M1.PLL, 0, TLI_5012B_POS);
    #endif
    //---------------------------------------------------------

   	/* Initialize CAN peripheral */
   	#ifdef USING_CAN
	can_fd_init();
	#endif

    //---------------------------------------------------------
    //--- setting for boards in Infineon Mobile Robot (IMR)
    //---------------------------------------------------------
	if (DLM_BRD_POSITION == 0) { // front board
		vars[0].dir =  1;
		vars[1].dir = -1;
	}
	else if (DLM_BRD_POSITION == 1) { // back board
		vars[0].dir = -1;
		vars[1].dir =  1;
	}
	//---------------------------------------------------------

    // Enable global interrupts
    __enable_irq();

    //(void) (result);
    for (;;)
    {
		#ifdef USING_CAN
		/* transmit speed & position via CAN regularly */
    	manage_extComm_with_CAN();
    	#endif
    	
        //-----------------------------------------------------------
    	//--- Update the 6EDL internal state and its registers
        //-----------------------------------------------------------
        #ifdef USING_SGD
    	if (Task10msCnt > 9)
    	{
    		EDL7141_UpdateFg ^= 1;

    		if (EDL7141_UpdateFg)
    			 EDL7141_Update((EDL_UPDATE_t*)&EdlUpdate,    (EDL_IO_CONTROL_t *)&EdlIo,    (edl7141_register_t *)&Edl7141Reg1, EDL1, 0);
    		else EDL7141_Update((EDL_UPDATE_t*)&EdlUpdate_M1, (EDL_IO_CONTROL_t *)&EdlIo_M1, (edl7141_register_t *)&Edl7141Reg2, EDL2, 0);
    		Task10msCnt = 0;
    	}
    	#endif
        //---------------------------------------------------------
    }
}

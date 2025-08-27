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
/*
 * PIPLL_Qxx.c
 *
 *  Created on: May 2, 2025
 *      Author: leeyoung
 */

#include "PIPLL_Qxx.h"

void PresetPIPLL(PIPLL_t *ptr, const int16_t theta_err, const q_t omega, const int16_t theta)
{
	ptr->ThetaU16  = theta;
	ptr->omega1    = 0;
	ptr->omega     = ptr->omega2 = omega;

	ptr->Theta_flt = PU_TO_FLOAT(ptr->ThetaU16, THETA_BASE   , Qxx_MAX);
	ptr->Omega_flt = PU_TO_FLOAT(ptr->omega   , OMEGA_RE_BASE, Qxx_MAX);
}

void ResetPIPLL(PIPLL_t *ptr, const q_t omega, const uint16_t theta)
{
	ptr->ThetaU16  = theta;
	ptr->omega1    = 0;
	ptr->omega2    = omega;
	ptr->omega     = omega;
	ptr->Theta_flt = PU_TO_FLOAT(ptr->ThetaU16, THETA_BASE   , Qxx_MAX);
	ptr->Omega_flt = PU_TO_FLOAT(ptr->omega   , OMEGA_RE_BASE, Qxx_MAX);
}


void SetPIPLLGain(PIPLL_t *ptr, q_t k1, q_t k2, q_t k_omega_2_dtheta_pu)
{
	ptr->Kp = k1;
	ptr->Ki = k2;
	ptr->k_Omega2dTheta = k_omega_2_dtheta_pu;
}

void InitPIPLL(PIPLL_t *ptr, const q_t omega, const uint16_t theta)
{
	//--- PI based PLL (Position Observer) ---
	float Wc_PLL  = (TWO_PI*Fc_PLL);
	float Wz_PLL  = (TWO_PI*Fz_PLL);
	float K1_PLL  = (Wc_PLL + Wz_PLL);
	float K2_PLL  = (Wc_PLL * Wz_PLL);
	q_t K1_PLL_PU = IFX_Q( K1_PLL*(THETA_BASE*ONE_OVER_OMEGA_RE_BASE));
	q_t K2_PLL_PU = IFX_Q((K2_PLL*(THETA_BASE*ONE_OVER_OMEGA_RE_BASE))*Ts);
	q_t K_OMEGA_2_dTHETA_PU = IFX_Q(Ts*(OMEGA_RE_BASE*ONE_OVER_THETA_BASE));

	ResetPIPLL(ptr, omega, theta);
	SetPIPLLGain(ptr, K1_PLL_PU, K2_PLL_PU, K_OMEGA_2_dTHETA_PU);
}

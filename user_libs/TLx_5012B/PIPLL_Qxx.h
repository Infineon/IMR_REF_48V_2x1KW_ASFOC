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
 * PIPLL.h
 *
 *  Created on: May 2, 2025
 *      Author: leeyoung
 */

#ifndef TLI_5012_PIPLL_H_
#define TLI_5012_PIPLL_H_

#include "stdint.h"

#define q_t					int32_t

#define Qxx					15
#define Qxx_MAX				((q_t)(1 << Qxx))

#define IFX_Q_mul(x, y)		((q_t)(((x)*(y)) >> Qxx))
#define IFX_Q(x)			((q_t)(x*Qxx_MAX))

#define Fs_Hz					15000.0f							// [Hz] Sampling frequency
#define Ts						(1.0f / Fs_Hz)						// [s]  Sampling period
#define PI						(3.141592653589793f)
#define TWO_PI					(6.283185307179586f)
#define Fc_PLL					250.0f
#define Fz_PLL					25.0f
#define THETA_BASE				PI
#define RPM_BASE				1000.0f								// [rpm] base speed mechanical
#define POLE_PAIR				14									// [pole pair]

#define ONE_OVER_THETA_BASE		(1.0f / THETA_BASE)
#define OMEGA_RE_BASE			((RPM_BASE*POLE_PAIR*TWO_PI)/60.0f)	// [rad/s] base angular speed electrical
#define ONE_OVER_OMEGA_RE_BASE	(1.0f / OMEGA_RE_BASE)					// [1/rad/s]

typedef struct {
	q_t Kp;
	q_t Ki;
	q_t k_Omega2dTheta;
	q_t omega1;
	q_t omega2;
	q_t omega;

	uint16_t ThetaU16;
	float Theta_flt;
	float Omega_flt;
} PIPLL_t;

#define PU_TO_FLOAT(pu, base, qxx_max)	(((float)((q_t)pu))*(base/(float)qxx_max))

static inline __attribute__((always_inline)) void PIPLLMain(PIPLL_t *ptr, const uint16_t theta_u16)
{
	q_t theta_err     = (q_t)(int16_t)(theta_u16 - ptr->ThetaU16);
	ptr->omega1        = IFX_Q_mul(theta_err, ptr->Kp);
	ptr->omega2       += IFX_Q_mul(theta_err, ptr->Ki);
	ptr->omega         = ptr->omega1 + ptr->omega2;
	ptr->ThetaU16     += IFX_Q_mul(ptr->omega, ptr->k_Omega2dTheta);						// Q15: apply PLL factor to the estimated theta
	ptr->Theta_flt     = PU_TO_FLOAT(((q_t)(int16_t)ptr->ThetaU16), THETA_BASE , Qxx_MAX);	//((float)((int16_t)ptr->ThetaU16))*(THETA_BASE   /(float)Qxx_MAX);
	ptr->Omega_flt     = PU_TO_FLOAT(ptr->omega , OMEGA_RE_BASE, Qxx_MAX);					//((float)((int16_t)ptr->omega   ))*(OMEGA_RE_BASE/(float)Qxx_MAX);
}

//--- PI based PLL ---
extern void InitPIPLL(PIPLL_t *ptr, const q_t omega, const uint16_t theta);
extern void ResetPIPLL(PIPLL_t *ptr, const q_t omega, const uint16_t theta);
extern void SetPIPLLGain(PIPLL_t *ptr, q_t k1, q_t k2, q_t k_omega_2_dtheta_pu);
extern void PresetPIPLL(PIPLL_t *ptr, const int16_t theta_err, const q_t omega, const int16_t theta);



#endif /* TLI_5012_PIPLL_H_ */

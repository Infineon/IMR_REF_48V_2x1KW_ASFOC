
/******************************************************************************
 * Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
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
 * significant property damage, injury or death ("High Risk Product").
 * By including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing so
 * agrees to indemnify Cypress against all liability.
******************************************************************************/

/******************************************************************************
 * This library is specifically developed for Infineon Mobile Robot (IMR). The
 * LED boards are based on DEMO_IMR_LED_ISHAPE_V1 and DEMO_IMR_LED_USHAPE_V1.
 * The PCB design data can be found here: 
 * https://www.infineon.com/applications/robotics/development-platform#documents
******************************************************************************/

#include "IMRLedPattern.h"

/******************************************************************************
 * Definition
 *****************************************************************************/
#define N_LEDS_ISHAPE	5
#define N_LEDS_USHAPE	23

#if (BARGRAPH_CONFIG == 0)
// 11 front U_SHAPE + 5 side I_SHAPE + 11 back U_SHAPE
#define N_LEDS_FULL_BAR	27
#elif (BARGRAPH_CONFIG == 1)
#define N_LEDS_FULL_BAR	13 // 11 front or back U_SHAPE + 2 side I_SHAPE
#elif (BARGRAPH_CONFIG == 2)
#define N_LEDS_FULL_BAR	15 // 3 * 5 side I_SHAPE
#endif

/******************************************************************************
 * Global variables
 *****************************************************************************/
uint8_t color_left[3]  = {0x70, 0x15, 0x45}; // MAGENTA color
uint8_t color_right[3] = {0x15, 0x70, 0x45}; // CYAN color
uint8_t color_off[3]   = {0};

/******************************************************************************
 * Converts the robot LED pattern to the board LED patterns.
 *
 * This function maps the robot LED pattern to the board LED patterns.
 * The robot LED pattern is a 3x56 array,
 * and the LED patterns for the 12 boards is a 12x23 array.
 *
 * robotLED: A 3x56 array representing the robot LED pattern.
 * boardLEDs: A 12x23 array representing the board LED patterns.
 *****************************************************************************/
void robotLED2boardLED(bool robotLED[3][56], bool boardLEDs[12][23])
{
	for (int i = 0; i < 3; i++) { // layers
		uint8_t i_led = 0; // index of LED in layer (robot_led indexing)
		for (int j = 0; j < 4; j++) { // boards
			uint8_t n_leds = j%2 == 0 ?	23 : 5;	// number of LEDs on board
			for (int k = 0; k < n_leds; k++) {	// LEDs
				boardLEDs[i*4+j][k] = robotLED[i][k + i_led];
			}
			i_led += n_leds;
		}
	}
}

/******************************************************************************
 * Converts the board LED pattern to a CAN mask.
 *
 * This function converts the board LED pattern to a CAN mask,
 * which is used for communication.
 * The board LED pattern is a 23-element array,
 * and the CAN mask is a 3-element array.
 *
 * boardLED		: A 23-element array representing the board LED pattern.
 * preserve_old	: A boolean flag indicating whether to preserve
 * 				  the state of the masked leds or turn them off.
 * CANmask		: A 3-element array representing the CAN mask.
 *****************************************************************************/
void boardLED2CANmask(bool boardLED[23], bool preserve_old, uint8_t CANmask[3])
{
	for (int i = 0; i < 23; i++) {
		int byte_index = 2 - (i / 8);
		int bit_index = i % 8;
		if (boardLED[i]) {
			CANmask[byte_index] |= (1 << bit_index);
		} else {
			CANmask[byte_index] &= ~(1 << bit_index);
		}
	}
	// Invert CANmask
	for (int i = 0; i < 3; i++) {
		CANmask[i] = ~CANmask[i];
	}
	if (preserve_old) {
		// set bit 7 to 1 to keep masked LEDs in old status
		CANmask[0] |= 0x80;
	}
}

/******************************************************************************
 * Set LED color from appearance to hex values
 * color		: color interpretation (see enumeration list)
 * rgb_color	: hex values of the RGB
 *****************************************************************************/
void setLEDColor(LED_COLOR_t color, uint8_t rgb_color[3])
{
	switch (color){
	case CYAN: // OCEAN and CYAN (lighter) is similar here
		rgb_color[0] = 0x15;  rgb_color[1] = 0x70; rgb_color[2] = 0x45;
		break;
	case MAGENTA: // BERRY (lighter) and MAGENTA is similar here
		rgb_color[0] = 0x70;  rgb_color[1] = 0x15; rgb_color[2] = 0x45;
		break;
	case WHITE: // GREY and WHITE is similar here
		rgb_color[0] = 0xB9;  rgb_color[1] = 0xB9; rgb_color[2] = 0xB9;
		break;
	case LAWN: // GREEN and LAWN is similar here
		rgb_color[0] = 0x9B;  rgb_color[1] = 0xBA; rgb_color[2] = 0x15;
		break;
	}
}

/******************************************************************************
 * Transmit the desired LED pattern via CAN with fixed LED mode: steady
 *
 * boardLED		: status of LED IC in each LED board up to 23 LED pieces
 * rgb_color	: LED RGB values to get to the desired color
 * ID			: CAN message ID to start with the transmission
 * offset		: shift in the CAN message ID
 *****************************************************************************/
CAN_STATUS_t patternCANTX(bool boardLED[23], uint8_t rgb_color[3],
		IMR_CAN_MESSAGE_IDS_t ID, uint32_t offset)
{
	uint8_t can_data[8];
	uint8_t CANmask[3] = {0};
	CAN_STATUS_t status;

	can_data[0] = LED_MODE_STEADY; //this mode does not use CAN timing
	can_data[4] = 30; // dummy value for CAN timing
	for (int i = 0; i < 3; i++) {
		can_data[i+1] = rgb_color[i];
	}
	boardLED2CANmask(boardLED, true, CANmask);
	for (int j = 0; j < 3; j++)
		can_data[5 + j] = CANmask[j];
	status = can_tx_request(ID + offset, can_data, 8);

	return status;
}

/******************************************************************************
 * Controls the LED bar graph.
 *
 * This function controls the LED bar graph based on the given parameters.
 * The function serves as a sample for implementing custom LED patterns
 * on the IMR. The motor speed is in the range of 0 to 100 (%)
 *
 * left_speed	: The left motor speed to be represented by the no. of LEDs
 * right_speed	: The right motor speed to be represented by the no. of LEDs
 *****************************************************************************/
void barGraph(int8_t left_speed, int8_t right_speed)
{
	CAN_STATUS_t status;

	int8_t n_leds_left = left_speed * N_LEDS_FULL_BAR / 100;
	int8_t n_leds_right = right_speed * N_LEDS_FULL_BAR / 100;

	if (n_leds_left  < 0) { // forward motion with n_leds_right(+)
		n_leds_left  = -n_leds_left;
		setLEDColor(CYAN, color_left);
	} else setLEDColor(MAGENTA, color_left); // backward motion

	if (n_leds_right < 0) { // backward motion with n_leds_left(+)
		n_leds_right = -n_leds_right;
		setLEDColor(MAGENTA, color_right);
	} else setLEDColor(CYAN, color_right); // forward motion

	bool robotLEDlayer[56] = {0}; 	  // 1 layer with total 56 LEDs to be ON
	bool robotLEDlayer_off[56] = {0}; // 1 layer with total 56 LEDs to be OFF
	bool robotLED[3][56] = {{0}}; 	  // all 3 layers in IMR to be ON
	bool robotLED_off[3][56] = {{0}}; // all 3 layers in IMR to be OFF

	// IMR LED boards: 3 layers * 4 positions (front, left, back, right)
	// and up to 23 LEDs in each board (USHAPE)
	bool boardLEDs[12][N_LEDS_USHAPE] = {{0}};
	bool boardLEDs_off[12][N_LEDS_USHAPE] = {{0}};

	// bitwise representation of all LEDs (horizontally) on the left;
	// -1 is to shift whole bar one bit to the right to set robotLEDlayer
	// to true so LEDs light up at the right positions
	uint32_t bar_left = (n_leds_left == 0 ? 0 :(1 << n_leds_left)-1);
	uint32_t bar_right = (n_leds_right == 0 ? 0 : (1 << n_leds_right)-1);

	// left side
	for (int i = 0; i < N_LEDS_FULL_BAR; i++) {
		robotLEDlayer[i + 12] = (bar_left >> i) & 1;
		robotLEDlayer_off[i + 12] = !((bar_left >> i) & 1);
	}
#if (BARGRAPH_CONFIG == 1)
	for (int i = 0; i < N_LEDS_FULL_BAR; i++) {
		robotLEDlayer[38 - i] = (bar_left >> i) & 1;
		robotLEDlayer_off[38 - i] = !((bar_left >> i) & 1);
	}
#endif
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 56; j++) {
			robotLED[i][j] = robotLEDlayer[j];
			robotLED_off[i][j] = robotLEDlayer_off[j];
		}
	}
	robotLED2boardLED(robotLED, boardLEDs);
	for (int i = 0; i < 12; i++) { // start CAN transmit for all boards
		status = patternCANTX(boardLEDs[i], color_left, LED_LAYER_1_FRONT, i);
		Cy_SysLib_Delay(1);
	}
	// right side: reset arrays after use with left side
	// robotLEDlayer_off does not need to be reset at this stage as
	// it should hold the data from left side
	memset(robotLEDlayer, 0, sizeof(robotLEDlayer));
	memset(robotLED, 0, sizeof(robotLED));
	memset(boardLEDs, 0, sizeof(boardLEDs));

	for (int i = 0; i < N_LEDS_FULL_BAR; i++) {
		robotLEDlayer[(10 - i + 56) % 56] = (bar_right >> i) & 1;
		robotLEDlayer_off[(10 - i + 56) % 56] = !((bar_right >> i) & 1);
	}
#if (BARGRAPH_CONFIG == 1)
	for (int i = 40; i < (40 + N_LEDS_FULL_BAR); i++) {
		robotLEDlayer[i%56] = (bar_right >> (i-40)) & 1;
		robotLEDlayer_off[i%56] = !((bar_right >> (i-40)) & 1);
	}
#endif
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 56; j++) {
			robotLED[i][j] = robotLEDlayer[j];
			robotLED_off[i][j] = robotLEDlayer_off[j];
		}
	}
	robotLED2boardLED(robotLED, boardLEDs);
	for (int i = 0; i < 12; i++) { // start CAN transmit for all boards
		status = patternCANTX(boardLEDs[i], color_right, LED_LAYER_1_FRONT, i);
		Cy_SysLib_Delay(1);
}
	// send set LEDs within bargraph that are supposed to be turned off
	// to RGB=[000] manually. Note that using LED_MODE_OFF would not work
	// as it does not respect the bitmask
	robotLED2boardLED(robotLED_off, boardLEDs_off);
	for (int i = 0; i < 12; i++) { // start CAN transmit for all boards
		status = patternCANTX(boardLEDs_off[i], color_off, LED_LAYER_1_FRONT, i);
		Cy_SysLib_Delay(1);
	}
	if (status == CAN_SUCCESS)
		Cy_GPIO_Inv(LED_STATUS_PORT, LED_STATUS_PIN);
}

#if (BARGRAPH_CONFIG == 2)
/******************************************************************************
 * Controls the LEDs as speed indicator in mini IMR configuration:
 * 3 IMR I_SHAPE on the left & 3 I_SHAPE on the right (no IMR U_SHAPE LED)
 * The motor speed is in the range of 0 to 100 (%)
 *
 * IMR forward motion: (front) left_speed (-) & right_speed (+): CYAN
 * color and bottom LED starts from the back to the front
 *
 * IMR backward motion: (front) left_speed (+) & right_speed (-): MAGENTA
 * color and bottom LED starts from the front to the back
 *
 * left_speed	: The left motor speed to be represented by the no. of LEDs
 * right_speed	: The right motor speed to be represented by the no. of LEDs
 *****************************************************************************/
void ledSnake_shortBoard(int8_t left_speed, int8_t right_speed)
{
	CAN_STATUS_t status;

	int8_t n_leds_left = left_speed * N_LEDS_FULL_BAR / 100;
	int8_t n_leds_right = right_speed * N_LEDS_FULL_BAR / 100;

	if (n_leds_left  < 0) { // forward motion with n_leds_right(+)
		n_leds_left  = -n_leds_left;
		setLEDColor(CYAN, color_left);
	} else setLEDColor(MAGENTA, color_left); // backward motion

	if (n_leds_right < 0) { // backward motion with n_leds_left(+)
		n_leds_right = -n_leds_right;
		setLEDColor(MAGENTA, color_right);
	} else setLEDColor(CYAN, color_right); // forward motion

	uint32_t bar_left  = (n_leds_left == 0 ? 0 :(1 << n_leds_left)-1);
	uint32_t bar_right = (n_leds_right == 0 ? 0 : (1 << n_leds_right)-1);

	// left side
	bool leftLED_on[N_LEDS_FULL_BAR]   = {0}; // each LED to be ON
	bool leftLED_off[N_LEDS_FULL_BAR]  = {0}; // each LED to be OFF
	bool leftBOT_on[3][N_LEDS_USHAPE]  = {{0}}; //split into 3 boards
	bool leftBOT_off[3][N_LEDS_USHAPE] = {{0}}; //split into 3 boards

	for (int i = 0; i < N_LEDS_FULL_BAR; i++)
	{
		leftLED_on[i]  =  (bar_left >> i) & 1;
		leftLED_off[i] = !(bar_left >> i) & 1;
	}
	if (left_speed < 0) { // forward motion: bottom LED board starts from back
		for (int j = 0; j < N_LEDS_ISHAPE; j++)
		{
			leftBOT_on[0][j]  = leftLED_on[N_LEDS_ISHAPE-1-j]; // bottom board
			leftBOT_off[0][j] = leftLED_off[N_LEDS_ISHAPE-1-j];

			leftBOT_on[1][j]  = leftLED_on[N_LEDS_ISHAPE+j]; // mid board
			leftBOT_off[1][j] = leftLED_off[N_LEDS_ISHAPE+j];

			leftBOT_on[2][j]  = leftLED_on[N_LEDS_ISHAPE*3-1-j]; // top board
			leftBOT_off[2][j] = leftLED_off[N_LEDS_ISHAPE*3-1-j];
		}
	}
	else { // backward motion: bottom LED board starts from front
		for (int j = 0; j < N_LEDS_ISHAPE; j++)
		{
			leftBOT_on[0][j]  = leftLED_on[j]; // bottom board
			leftBOT_off[0][j] = leftLED_off[j];

			leftBOT_on[1][j]  = leftLED_on[N_LEDS_ISHAPE*2-1-j]; // mid board
			leftBOT_off[1][j] = leftLED_off[N_LEDS_ISHAPE*2-1-j];

			leftBOT_on[2][j]  = leftLED_on[N_LEDS_ISHAPE*2+j]; // top board
			leftBOT_off[2][j] = leftLED_off[N_LEDS_ISHAPE*2+j];
		}
	}
	for (int i = 0; i < 3; i++) { // start CAN transmit for 3 short boards
		status = patternCANTX(leftBOT_on[i], color_left, LED_LAYER_1_LEFT, i*4);
		Cy_SysLib_Delay(1);
	}
	// right side
	bool rightLED_on[N_LEDS_FULL_BAR]   = {0}; // each LED to be ON
	bool rightLED_off[N_LEDS_FULL_BAR]  = {0}; // each LED to be OFF
	bool rightBOT_on[3][N_LEDS_USHAPE]  = {{0}}; //split into 3 boards
	bool rightBOT_off[3][N_LEDS_USHAPE] = {{0}}; //split into 3 boards

	for (int i = 0; i < N_LEDS_FULL_BAR; i++)
	{
		rightLED_on[i]  =  (bar_right >> i) & 1;
		rightLED_off[i] = !(bar_right >> i) & 1;
	}
	if (right_speed < 0) { // backward motion: bottom LED starts from front
		for (int j = 0; j < N_LEDS_ISHAPE; j++)
		{
			rightBOT_on[0][j]  = rightLED_on[N_LEDS_ISHAPE-1-j]; // bottom board
			rightBOT_off[0][j] = rightLED_off[N_LEDS_ISHAPE-1-j];

			rightBOT_on[1][j]  = rightLED_on[N_LEDS_ISHAPE+j]; // mid board
			rightBOT_off[1][j] = rightLED_off[N_LEDS_ISHAPE+j];

			rightBOT_on[2][j]  = rightLED_on[N_LEDS_ISHAPE*3-1-j]; // top board
			rightBOT_off[2][j] = rightLED_off[N_LEDS_ISHAPE*3-1-j];
		}
	}
	else { // forward motion: bottom LED board starts from back
		for (int j = 0; j < N_LEDS_ISHAPE; j++)
		{
			rightBOT_on[0][j]  = rightLED_on[j]; // bottom board
			rightBOT_off[0][j] = rightLED_off[j];

			rightBOT_on[1][j]  = rightLED_on[N_LEDS_ISHAPE*2-1-j]; // mid board
			rightBOT_off[1][j] = rightLED_off[N_LEDS_ISHAPE*2-1-j];

			rightBOT_on[2][j]  = rightLED_on[N_LEDS_ISHAPE*2+j]; // top board
			rightBOT_off[2][j] = rightLED_off[N_LEDS_ISHAPE*2+j];
		}
	}
	for (int i = 0; i < 3; i++) { // start CAN transmit for 3 short boards
		status = patternCANTX(rightBOT_on[i], color_right, LED_LAYER_1_RIGHT, i*4);
		Cy_SysLib_Delay(1);
	}
	// send set LEDs within bargraph that are supposed to be turned off
	for (int i = 0; i < 3; i++) { // start CAN transmit for 3 short boards
		status = patternCANTX(leftBOT_off[i], color_off, LED_LAYER_1_LEFT, i*4);
		Cy_SysLib_Delay(1);
	}
	for (int i = 0; i < 3; i++) { // start CAN transmit for 3 short boards
		status = patternCANTX(rightBOT_off[i], color_off, LED_LAYER_1_RIGHT, i*4);
		Cy_SysLib_Delay(1);
	}
	if (status == CAN_SUCCESS)
		Cy_GPIO_Inv(LED_STATUS_PORT, LED_STATUS_PIN);
}
#endif

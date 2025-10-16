# Dual-motor control with sensored FOC code example 

<a href="https://www.infineon.com">
<img src="./images/Logo.svg" align="right" alt="Infineon logo">
</a>
<br>
<br>

## Overview

This code example (written in C programming language) demonstrates the position angle sensor-based dual motor control using the Infineon's PSOC™ Control C3 MCU and XENSIV™ TLx 5012B E1000. This code example includes the following solutions based on Rotor Field Oriented (RFO) control: 
- Sensorless PMSM FOC with 3-shunt
- GMR-based position angle sensor-based PMSM FOC<br><br>

Additionally this code demonstrates CAN communication for: 
- transmitting message containing the speed and mechanical angle detected by the position angle sensor
- receiving message containing the required speed

The CAN message IDs have been specifically written for [Infineon Mobile Robot (IMR)](https://www.infineon.com/cms/en/applications/robotics/development-platform/)<br><br>

[View this README on GitHub.](https://github.com/Infineon/IMR_REF_48V_2x1KW_ASFOC)


## Features

- 3.3 V 32-bit microcontroller Arm® Cortex®-M33 180 MHz PSOC™ Control C3 with 256 kB Flash and 64 kB RAM
- CAN bus communication with onboard CAN-FD transceiver
- SPI and IIF interface for two position angle sensors 
- Two motor drive power stages in one board, each providing up to 1 kW power
- 3-phase smart gate drivers, each with extensive protections (including VDS sensing), 3 current sense amplifiers, and voltage regulators 
- Disconnect switches providing Safe Torque-Off (STO) feature
- GMR-based position angle sensors for 2 motors
- Potentiometer board to provide speed input
- Wide operating battery voltage from 5S to 13S configuration (nominal 18 - 48 VDC) 


### Featured Infineon Products 
Following products are featured by the reference hardware:<br>

<table style="width:100%">
  <tr>
    <th>Product</th>
    <th>Description</th>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/PSC3M5FDS2AFQ1">PSC3M5FDS2AFQ1</a></td>
    <td>High performance microcontroller tailored for motor control</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/TLE9250VSJ">TLE9250VSJ</a></td>
    <td>High speed CAN transceiver for CAN and CAN FD</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/TLI5012B-E1000">TLI5012B E1000</a></td>
    <td>XENSIV™ GMR-based angle sensor with SPI and incremental interface</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/6EDL7151">6EDL7151</a></td>
    <td>MOTIX™ 70 V 3-phase smart gate driver</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/1EDL8011">1EDL8011</a></td>
    <td>125 V high-side gate driver with single output in DSO-8 package for battery-powered applications</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/ISC025N08NM5LF2">ISC025N08NM5LF2</a></td>
    <td>OptiMOS™ 5 Single N-Channel Linear FET 80 V, 2.55 mΩ, 199 A in a SuperSO8 (5x6) package</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/TLI4971-A050T5-E0001">TLI4971-A050T5-E0001</a></td>
    <td>XENSIV™ high precision coreless current sensor with analog interface and dual fast over-current detection outputs</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/ISC031N08NM6">ISC031N08NM6</a></td>
    <td>OptiMOS™ 6 n-channel power MOSFET 80 V, 3.1 mΩ, 145 A in SuperSO8</td>
  </tr>
  <tr>
    <td><a href="https://www.infineon.com/part/TLE4264-2G">TLE4264-2G</a></td>
    <td>Monolithic integrated low-drop fixed voltage regulator which can supply loads up to 150 mA</td>
  </tr>
</table>
</p>
<br>


## Hardware requirements and setup

- Motor control reference design board: [REF_48V_2x1KW_ASFOC](https://www.infineon.com/evaluation-board/REF-48V-2X1KW-ASFOC).
- Angle sensor boards supplied together with the above reference design or separate board e.g. [DEMO_IMR_ANGLE_SENS_V1](https://www.infineon.com/evaluation-board/DEMO-IMR-ANGLE-SENS-V1).
- Diametrical magnet suitable for the above sensor e.g. [SM-10x05-N-D](https://www.magnet-shop.com/neodymium/discmagnets/discmagnet-10.0-x-5.0-mm-n45-nickel-diametral-magnetized). Simulation tool to get the right design for the magnet and sensor is available [here](https://design.infineon.com/sensor/anglesim/index.html).
- Associated motor, of which motor parameters are used in this code example: [T-MOTOR GL60 KV25](https://store.cubemars.com/products/gl60-kv25?srsltid=AfmBOoo43LFEcBgF-joehwDfbFuc71DmEuLf7Sb7Uw1IsPBBfwHe3DWM) or motor suggested by the user manual of the above reference design board. However please note that the motor parameters have to be adjusted accordingly. 
- Fixture to mount the magnet in the center of the motor and angle sensor. For example, for the above hardware, below drawing can be used.<br><br>
        <img src="./images/Fixture.jpg"><br><br>
- Programmer and Debugger for the board e.g. [XMC™ Link](https://www.infineon.com/evaluation-board/KIT-XMC-LINK-SEGGER-V1).
- For testing CAN communication, [PCAN-USB FD](https://www.peak-system.com/PCAN-USB-FD.365.0.html?&L=1) will come in handy.

See the respective kit quick start guide for the hardware setup information. For details, see the [User Manual](https://www.infineon.com/assets/row/public/documents/24/44/infineon-reference-board-ref-48v-2x1kw-asfoc-usermanual-en.pdf)  of the above reference design board. For testing CAN communication, ensure the CAN-H and CAN-L lines of the board are connected to the respective lines of the PCAN USB device. 


## Software requirements and setup

- [ModusToolbox™ Setup](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.modustoolboxsetup) to allow the download of the following packages; See the [ModusToolbox™ tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.
    - ModusToolbox™ Tools Package version 3.5.0 (tested)
    - Eclipse IDE for ModusToolbox™ version 2025.4.0 (tested)<br>
    <em>Note that the code build of this repository is not tested for any other versions (earlier or later) of both the Tools Package and Eclipse IDE.</em>

- [ModusToolbox™ Motor Suite](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.ifxmotorsolutions) to evaluate the GUI features.
- Additionally, [Infineon Developer Center Launcher](https://softwaretools.infineon.com/tools/com.ifx.tb.launcher2) is useful to know which Infineon programs have been installed and to find other Infineon programs to be installed.<br><br>
    <img src="./images/MTB_Software_1.png"><br><br>
    <img src="./images/MTB_Software_2.png"><br><br>
    <img src="./images/MTB_Software_3.png"><br><br>
    <img src="./images/MTB_Software_4.png"><br><br>
- [J-Link Software](https://www.segger.com/downloads/jlink/) to allow the use of XMC™ Link to program the board and debug the software. 
- For testing CAN communication, install [PCAN-View](https://www.peak-system.com/PCAN-View.242.0.html?&L=1) to view, transmit, and record the CAN data traffic in Windows-based PC. 


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v11.3.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- IAR C/C++ Compiler v9.50.2 (`IAR`)


## Important notes

To avoid any uncertainty in code build process, please update the ModusToolbox™ Manifest DB value to be equal to https://gitlab.intra.infineon.com/rsg/mtb-super-manifest/-/raw/multi_inst_motor_lib/mtb-super-manifest-fv2.xml
- This setting can either be changed in ModusToolbox™ Library Manager (see [Step 7](#step7) to get into it), and go to Settings and select 'ModusToolbox Settings...'<br><br>
    <img src="./images/MTB_ManifestDB_1.png"><br><br>
    <img src="./images/MTB_ManifestDB_2.png"><br><br>
- If the field is locked as it is shown in above image, change the setting in the Windows OS environment variable. Go to Control Panel -> System -> Advanced system settings -> Environment Variables. Click on System variable CyRemoteManifestOverride to edit its value to the link given above.<br><br>
    <img src="./images/MTB_ManifestDB_3.png"><br><br>
    <img src="./images/MTB_ManifestDB_4.png"><br><br>

Please watch out for the boards version:
- Power stage Si board V1.1 is equipped with S1 and S2 switches. Old version is labelled as V1.0 where S2 switch might not be present. 
    - To provide the right 3.3V for the control card, ensure S1 toggle is either in position 1 for V1.1, or in position ON for V1.0.<br><br>
        <img src="./images/PwrBoard_V1p1_S1_3V3.jpg"><br><br>
    - Below picture is to indicate that the power board receives input voltage but is not fully switched ON yet (i.e. S2 switch is still in OFF position).<br><br>
        <img src="./images/PwrBoard_V1p1_S2_OFF.jpg"><br><br>
    - To fully switch on the power board and control card, S2 switch should be toggled ON as indicated in the picture below.<br><br>
        <img src="./images/PwrBoard_V1p1_S2_ON.jpg"><br><br>

- PSC3 control card V1.0 is considered to be the new control card version (the card on the right in the picture below). Old version has no label.<br><br>
        <img src="./images/CCard_Diff.jpg"><br><br>
    - For the new control card, we discovered a small mistake in the PCB layout, which prevented the board to be used for lower input voltage < 30 VDC. To ensure proper board operation, please short pin 145 and pin 147 of the SO-DIMM header as shown in the picture below.<br><br>
        <img src="./images/CCard_Rework.png"><br><br>
    - If old control card version is in use, ensure the pin configuration is correctly done accordingly (see [Step 11](#step11)).<br><br> 

User definitions and offset values to be set correctly in the code: 
- BOARD_TYPE, MOTOR_TYPE and OFFSET_CAL_DONE in .../configuration/hw-Config/HardwareIface.h
    - By default, the BOARD_TYPE is set to be BOARD_MOSFET_DUAL in such a way the SWITCHING_DEVICE_TYPE is set to SI_MOSFET
    - By default, the MOTOR_TYPE is set with the tested motor in this project i.e. GL60_KV25 but for other unknown motor, it can be set to NOT_SPECIFIED and the motor parameters can be adjusted manually by users. 
- TLI_5012B_ABS_POS._ENC_SENSOR_OFST_S16 in .../user_libs/TLx_5012B/TLI_5012B.c
- TLI_5012B_ABS_POS_M1._ENC_SENSOR_OFST_S16 in .../user_libs/TLx_5012B/TLI_5012B.c
- GUI_CONTROL in .../user_libs/CAN/IMR_CAN.h
<br><br>

This code example works with three different sources of speed input:
- ModusToolbox™ Motor Suite i.e. set Potentiometer Control in the GUI to OFF
    - GUI_CONTROL in .../user_libs/CAN/IMR_CAN.h is set to 1, or
	- GUI_CONTROL in .../user_libs/CAN/IMR_CAN.h is set to 0, as long as no other subsystems or program controlling the speed input via CAN
- Coming from [CAN communication](#step27)
    - GUI_CONTROL in .../user_libs/CAN/IMR_CAN.h is set to 0
- Coming from the [potentiometer board](#step31) (supplied together with control card) 
    - if Potentiometer Control in the ModusToolbox™ Motor Suite GUI is set to ON, or
    - MOTOR_CTRL_COMMAND_SOURCE and MOTOR_CTRL_COMMAND_SOURCE_M1 value in .../configuration/motor-ctrl-lib-config/ParamConfig.h are set to Internal
<br><br>

The later version of ModusToolbox™ Motor Suite (i.e. 2.7.0) allows number of channels in its Oscilloscope to be increased to 10 to allow sufficient variables to be monitored for dual-motor control including variables to determine the offset value for the position angle sensor.<br>
To set for this increase, 2 files need to be updated: 
- Makefile : look for the field DEFINES+=MOTOR_CTRL_NO_OF_SCOPE_CHANNELS and change the value from 0x8 to 0x0A <br><br>
    <img src="./images/MSO_Setting_1.png"><br><br>
- probe_scope.c located in library mtb_shared/motor-ctrl-lib/release-v3.0.0/ThirdPartyLib/probescope/ : replace the content of this file with the content of <a href="./images/probe_scope.c">this</a>.<br><br>
    <img src="./images/MSO_Setting_2.png"><br><br>
- In ModusToolbox™ Motor Suite Oscilloscope (see [Step 17](#step17)), the 10 channels can be set, for example, as follows:<br><br>
    <img src="./images/MSO_Setting_3.png"><br><br>
	
		'vars[0].i_uvw_fb.u',
		'vars[0].i_uvw_fb.v',
		'vars[0].i_uvw_fb.w',
		'TLI_5012B_ABS_POS.Theta_TLI_5012B_flt',
		'vars[0].th_r_final.elec',
		'vars[1].i_uvw_fb.u',
		'vars[1].i_uvw_fb.v',
		'vars[1].i_uvw_fb.w',
		'TLI_5012B_ABS_POS_M1.Theta_TLI_5012B_flt',
		'vars[1].th_r_final.elec'
	

#### User LEDs indicator

On the power stage board: 
- Red LED1 indicates the board receives 48V input voltage.
- Red LED3 (only in V1.1) indicates the board S2 switch is toggled ON to provide power to the control card. 

On the control card:
- Green LED indicates the board receives 3.3V input voltage.
- Orange LED indicates the CAN communication with other subsystems is established.
- Red LED indicates any faults detected by the board.


## Using the code example

<ol>
<li id="step1"> Clone the project repository into the local drive.<br><br>
        <img src="./images/MTB_Import_0.png"><br><br>
</li>
<li id="step2"> Open the ModusToolbox™ IDE (e.g. Eclipse for ModusToolbox™ 2025.4) and import the project with the import wizard by pressing 'File' – 'Import…'. <br><br>
        <img src="./images/MTB_Import_1.png"><br><br>
</li>
<li id="step3"> Select 'ModusToolbox™' – 'Import Existing Application In-Place' and press 'Next'. <br><br>
        <img src="./images/MTB_Import_2.png"><br><br>
</li>
<li id="step4"> Find the Project Location by pressing 'Browse…', and select the project folder accordingly and press 'Finish'. <br><br>
        <img src="./images/MTB_Import_3.png" ><br><br>
</li>
<li id="step5"> Wait until the project is fully imported. <br><br>
        <img src="./images/MTB_Import_4.png"><br><br>
</li>
<li id="step6"> Notice that additional folder 'mtb_shared' should be created (if there was none) in addition to the project folder itself, when the import is completed. This motor control project relies on the motor control library (current release is v3.0.0) provided in the ModusToolbox™ as shown inside the mtb_shared folder.<br><br>
        <img src="./images/MTB_Import_5.png"><br><br>
</li>
<li id="step7"> Right click the project folder and select 'ModusToolbox™' followed by 'Library Manager 2...'. <br><br>
        <img src="./images/MTB_Import_6.png"><br><br>
</li>
<li id="step8"> Press the 'Update' button <br><br>
        <img src="./images/MTB_Import_7.png"><br><br>
<br><br>
</li>
<li id="step9"> When the Update is completed the sucessful messages should be displayed. If the update failed, try it again by repressing the 'Update' button. If this also fails, try to clean the project before trying it again. <br><br>
        <img src="./images/MTB_Import_8.png"><br><br>
</li>
<li id="step10"> If the motor assembly is tested without any knowledge of the position sensor offset value, ensure in .../configuration/hw-Config/HardwareIface.h, OFFSET_CAL_DONE is 0. This will set the motor control mode into Speed Mode FOC Sensorless and allow user to calibrate the position sensor offset.<br>
Also set the MOTOR_TYPE and BOARD_TYPE accordingly based on your own hardware. Set to NOT_SPECIFIED if your hardware type is not supported by the code default setting, and set the necessary parameters manually.<br><br>
        <img src="./images/MTB_Import_9.png"><br><br>
</li>
<li id="step11"> If PSC3 control card is V1.0, the following steps should be ignored and go directly to clean the project (see Step 12). If the PSC3 control card in use is the old version, one pin configuration has to be modified by:
</li>
<ul>
<li> Right clicking on the project, select 'ModusToolbox™' followed by 'Device Configurator 5...'.<br><br>
        <img src="./images/MTB_Import_10.png"><br><br>
</li>
<li> Go to 'Pins' tab and right click on the Resource P2[0] to copy its configuration and paste it to P5[2].<br><br>
        <img src="./images/MTB_Import_11.png"><br><br>
        <img src="./images/MTB_Import_12.png"><br><br>
</li>
<li> Additionally, cut the pin Name EN_DRV_SGD_M1 from P2[0] and paste it to P5[2] overwriting its previous Name.<br><br>
        <img src="./images/MTB_Import_13.png"><br><br>
        <img src="./images/MTB_Import_14.png"><br><br>
</li>
<li> Disable P2[0] and P5[3] by unticking them. Next go to 'Peripherals' tab and also disable Serial Communication Block (SCB) 3 by unticking it.<br><br>
        <img src="./images/MTB_Import_15.png"><br><br>
        <img src="./images/MTB_Import_16.png"><br><br>
</li>
<li> Lastly save the new pinouts and peripherals configuration before closing the Device Configurator.<br><br>
        <img src="./images/MTB_Import_17.png"><br><br>
</li>
</ul>
<li id="step12"> Before building the project it is recommended to clean it by pressing 'Clean Project'. If error is reported (e.g. Directory is not empty during the removal step), repress the 'Clean Project' one more time.<br><br>
        <img src="./images/MTB_Import_18.png"><br><br>
</li>
<li id="step13"> The project can now be compiled by pressing 'Build Project'. <br><br>
        <img src="./images/MTB_Import_19.png"><br><br>
        <img src="./images/MTB_Import_20.png"><br><br>
</li>
<li id="step14"> To flash the board with the compiled code, first ensure that XMC™ Link is connected to the PC and its ribbon cable is connected to the programming connector on the board, and the board is powered with e.g. 48 VDC nominal.<br><br>
        <img src="./images/MTB_Import_21.jpg"><br><br>
</li>
<li id="step15"> One method to flash the board can be via ModusToolbox™ Motor Suite application, which can be opened directly from ModusToolbox™ IDE. Select the Dual Motor Control project with RFO configuration and ensure the hex and elf files location are correct.<br>
Additionally, ensure that XMC™ Link programmer is detected and connected to the ModusToolbox™ Motor Suite application.<br><br>
        <img src="./images/MTB_Import_21.png"><br><br>
</li>
<li id="step16"> Press the 'Flash Firmware' button to flash the compiled code into the PSOC™ Control C3 microcontroller. Locate the hex file correctly e.g. by selecting the 'Last Selected' option. Subsequently, press the next button 'Select ELF File' to allow the GUI to access the right variables for the project. Select the associated ELF file in the project.<br><br>
        <img src="./images/MTB_Import_22.png"><br><br>
        <img src="./images/MTB_Import_23.png"><br><br>
        <img src="./images/MTB_Import_24.png"><br><br>
        <img src="./images/MTB_Import_25.png"><br><br>
</li>
<li id="step17"> Go to the Test Bench GUI to start to test the board for turning the motors. Each motor can be controlled individually by selecting the motor of interest from the drop-down list on right top corner of the Control Panel.<br>
Additionally, a digital Oscilloscope can also be launched by pressing the second button (orange color) on the right top corner of the GUI. This oscilloscope will help to monitor up to 8 signals of interest e.g. position angle sensor readout from both motors.<br>
For easy viewing of all signals, 'Auto Fit All Traces' can be pressed to see all signals without the overlap. 'Sampling Clock' - 'Divider' value can also be increased to allow wider time range within the screen. Lastly, modify the variable of one of the channel for M0, e.g. from vars[0].i_uwb_fb.w to TLI_5012B_ABS_POS.Theta_TLI_5012B_flt for later step. Likewise for M1, e.g. from vars[1].i_uwb_fb.w to TLI_5012B_ABS_POS_M1.Theta_TLI_5012B_flt.<br><br>
        <img src="./images/MTB_Import_26.png"><br><br>
        <img src="./images/MTB_Import_27.png"><br><br>
        <img src="./images/MTB_Import_28.png"><br><br>
</li>
<li id="step18"> To turn each motor, go to Command Panel, ensure that Drive is ENABLED and Potentiometer Control is OFF, and drag the slider or use the (+) button to start increasing the motor speed to desired value e.g. 50%.<br>
Press 'Set Multiple Scales' in the oscilloscope, and notice that TLI_5012B_ABS_POS.Theta_TLI_5012B_flt may not well coincide with vars[0].th_r_final.elec. This means the position sensor offset value is not yet correct.<br><br>
        <img src="./images/MTB_Import_29.png"><br><br>
        <img src="./images/MTB_Import_30.png"><br><br>
</li>
<li id="step19"> To find the right offset value for Motor_0's position angle sensor, return to the GUI Test Bench window, and launch a GUI Builder by pressing the first button (green button) on the right top corner of the GUI.<br>
Press the open button ('Open Project') and navigate to the project folder to open the Enc_Sensor_Offsets.mcws project file. The project will automatically go into running mode once opened.<br>
Should you need to edit this custom GUI, press the pencil button ('Switch to Edit Mode') on top left corner to go into the edit mode.<br><br>
        <img src="./images/MTB_Import_31.png"><br><br>
        <img src="./images/MTB_Import_32.png"><br><br>
        <img src="./images/MTB_Import_33.png"><br><br>
Alternatively, instead of opening a project, a new project can also be created in the GUI builder by adding an input box (drag and drop) to contain the value of TLI_5012B_ABS_POS._ENC_SENSOR_OFST_S16 and remove the check for the 'Readonly', and set the minimum and maximum value to -32768 and 32767 accordingly. Add another input box for the second position sensor TLI_5012B_ABS_POS_M1._ENC_SENSOR_OFST_S16.<br><br>
Press the play button ('Switch to Run Mode') once it is ready to deploy in real time.\br><br>
        <img src="./images/MTB_Import_34.png"><br><br>
</li>
<li id="step20"> Change the offset value (type in the value and press ENTER) in real-time until the 2 signals of TLI_5012B_ABS_POS.Theta_TLI_5012B_flt and vars[0].th_r_final.elec fully overlap.<br><br>
        <img src="./images/MTB_Import_35.png"><br><br>
</li>
<li id="step21"> Repeat this activity for Motor_1. Two variables to compare are TLI_5012B_ABS_POS_M1.Theta_TLI_5012B_flt and vars[1].th_r_final.elec.<br><br>
        <img src="./images/MTB_Import_36.png"><br><br>
        <img src="./images/MTB_Import_37.png"><br><br>
        <img src="./images/MTB_Import_38.png"><br><br>
</li>
<li id="step22"> Once both offset values are found, update .../user_libs/TLx_5102B/TLI_5012B.c with the values and set OFFSET_CAL_DONE to 1 in .../configuration/hw-Config/HardwareIface.h to allow the motor control mode to be Speed Mode FOC Encoder.<br><br>
        <img src="./images/MTB_Import_39.png"><br><br>
        <img src="./images/MTB_Import_40.png"><br><br>
</li>
<li id="step23"> Rebuild the project in ModusToolbox™ IDE, and return to the ModusToolbox™ Motor Suite . It will notify to Reload ELF File due to new modification, but select Cancel and go to 'Configurator' Panel, and reflash the microcontroller with the new compiled code, followed by reuploading the new ELF file.<br><br>
        <img src="./images/MTB_Import_41.png"><br><br>
        <img src="./images/MTB_Import_42.png"><br><br>
        <img src="./images/MTB_Import_43.png"><br><br>
        <img src="./images/MTB_Import_44.png"><br><br>
</li>
<li id="step24"> Retest the board for the Speed Mode FOC Encoder control mode. At very low speed e.g. 5 RPM command, the Sampling Clock - Divider in the Motor Suite Oscilloscope can be set to 80. For speed command of e.g. 100 RPM, the Divider can be set to 8.
<br><br>
        <img src="./images/MTB_Import_45.png"><br><br>
        <img src="./images/MTB_Import_46.png"><br><br>
        <img src="./images/MTB_Import_47.png"><br><br>
        <img src="./images/MTB_Import_48.png"><br><br>
</li>
<li id="step25"> To check for the CAN-FD communication, ensure the GUI_CONTROL is set to 0 in the .../user_libs/CAN/IMR_CAN.h file. Rebuild the project, and reflash the compiled code into the microcontroller. Another method to reflash the board is by using ModusToolbox™ directly in the Quick Panel - Launches, select for <project_name> Program.  <br><br>
        <img src="./images/MTB_Import_49.png"><br><br>
</li>
<li id="step26"> Plug in the PCAN-USB FD device into the PC. Open PCAN-View software, and select for ISO CAN FD, Clock Frequency 80 MHz, and set for Bit Rate Preset SAE J2284-4 (500k/2M). Once the PCAN-USB FD hardware is connected, observe that the orange LED on control card light up. <br>
Also observe in the Receive section of the PCAN-View, CAN-ID 400h (from Motor_0) and 401h (from Motor_1) messages are regularly received at 100ms period. These IDs contain the motor speed and mechanical angle position for odometry information. Note that the CAN transmit from the control card has been set to CAN-FD with 64-byte message payload.<br><br>
        <img src="./images/MTB_Import_50.png"><br><br>
        <img src="./images/MTB_Import_51.png"><br><br>
        <img src="./images/MTB_Import_51.jpg"><br><br>
</li>
<li id="step27"> To test the CAN-FD communication, use the Transmit section of PCAN-View, right click on it to create 'New Message'. A CAN-ID 380 with 2 bytes data length for a speed command to Motor_0 can set accordingly, and enable the CAN-FD and Bit Rate Switch option.<br>
Press SPACE key to send the message.<br>
Similarly, a CAN-ID 381 can be used to transmit a speed command to Motor_1.<br>
Similar messages can be added to set the 2-byte data to 00 00 to stop the motors from turning, or simply edit each CAN-ID (right click and select 'Edit Message') to set the 2-byte data to 00 00.<br><br>
<table style="width:50%">
  <tr>
    <th align="left">Data (hex)</th>
    <th align="left">Speed (RPM)</th>
    <th align="left">Data (hex)</th>
    <th align="left">Speed (RPM)</th>
  </tr>
  <tr>
    <td>02 3C</td>
    <td>10</td>
    <td>FD C3</td>
    <td>-10</td>
  </tr>
  <tr>
    <td>04 78</td>
    <td>20</td>
    <td>FB 87</td>
    <td>-20</td>
  </tr>
  <tr>
    <td>0C CB</td>
    <td>57.3</td>
    <td>F3 34</td>
    <td>-57.3</td>
  </tr>
  <tr>
    <td>00 00</td>
    <td colspan="3">0 (to stop the motor from turning)</td>
  </tr>
</table>
<br><br>
        <img src="./images/MTB_Import_52.png"><br><br>
        <img src="./images/MTB_Import_53.png"><br><br>
        <img src="./images/MTB_Import_54.png"><br><br>
        <img src="./images/MTB_Import_55.png"><br><br>
</li>
<li id="step28"> To test for the classic CAN communication, firstly disconnect the CAN connection in PCAN-View.<br><br>
        <img src="./images/MTB_Import_56.png"><br><br>
Subsequently, the code has to be modified by:
</li>
<ul>
<li> right clicking on the project, select 'ModusToolbox™' followed by 'Device Configurator 5...'.<br><br>
        <img src="./images/MTB_Import_57.png">
</li>
<li> Go to 'Peripherals' tab and select Channel 1 under Communication -> Controller Area Network FD (CAN FD) 0. This code repository is by default set to the CAN-FD mode, and notice that the Fast Bitrate Setting is present, with Data Bit Rate set to 2 Mbps and Nominal Bit Rate set to 500 kbps.<br>
For CAN-FD to be successful, it is important to set the same Sampling Point in the BitRate Setting.<br><br>
        <img src="./images/MTB_Import_58.png"><br><br>
        <img src="./images/MTB_Import_59.png"><br><br>
        <img src="./images/MTB_Import_60.png"><br><br>
</li>
<li> Disable CAN FD Mode by unticking the CAN FD Mode, in order to be in Classic CAN mode, with Nominal Bit Rate set to 1 Mbps. Notice that afterwards the Fast Bitrate Setting disappears.<br><br>
        <img src="./images/MTB_Import_61.png"><br><br>
        <img src="./images/MTB_Import_62.png"><br><br>
</li>
<li> Lastly save the new peripherals configuration before closing the Device Configurator, rebuild the code, and reflash the MCU with this new version of software.<br><br>
        <img src="./images/MTB_Import_63.png"><br><br>
</li>
</ul>
<li id="step29"> Reconnect the CAN connection in PCAN-View with setting of nominal bit rate 1 Mbps as shown. Redo the speed command test for both motors.<br><br>
        <img src="./images/MTB_Import_64.png"><br><br>
        <img src="./images/MTB_Import_65.png"><br><br>
        <img src="./images/MTB_Import_66.png"><br><br>
</li>
<li id="step30"> Modify the transmitted message to remove the CAN FD setting, by right click on each CAN ID, select 'Edit Message', and untick the CAN FD in Message Type. Do for both CAN IDs.<br><br>
        <img src="./images/MTB_Import_67.png"><br><br>
        <img src="./images/MTB_Import_68.png"><br><br>
        <img src="./images/MTB_Import_69.png"><br><br>
        <img src="./images/MTB_Import_70.png"><br><br>
</li>
<li id="step31"> To provide motor speed input easily, a potentiometer board is included in the hardware kit. Ensure that the ribbon cable is connected between the potentiometer board and control card.<br>
Set the Potentiometer Control in the Motor Suite GUI to ON for each motor and test it by turning the potentiometer. The rotation direction can be controlled by using the switch on potentiometer board.<br><br>
        <img src="./images/MTB_Import_71.jpg"><br><br>
        <img src="./images/MTB_Import_71.png"><br><br>
        <img src="./images/MTB_Import_72.png"><br><br>
</li>
<li id="step32"> Alternatively, the code can be modified to have potentiometer control directly. Set MOTOR_CTRL_COMMAND_SOURCE and MOTOR_CTRL_COMMAND_SOURCE_M1 to Internal in .../configuration/motor-ctrl-lib-config/ParamConfig.h.<br>
Rebuild the code, and reflash the MCU with this new version of software. The potentiometer can be used to control the speed of the motors without any need of Motor Suite or CAN connection.<br><br>
        <img src="./images/MTB_Import_73.png"><br><br>
        <img src="./images/MTB_Import_74.png"><br><br>
        <img src="./images/MTB_Import_74.jpg"><br><br>
</li>
</ol>
   
   
## Debugging

You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (JLink)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox™ user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).
</details>

<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>


## Related resources

<table style="width:100%">
  <tr>
    <th>Type</th>
    <th>Links</th>
	<th>Description</th>
  </tr>
  <tr>
    <td>Application note</td>
	<td><a href="https://www.infineon.com/assets/row/public/documents/30/42/infineon-an238329-getting-started-psoc-control-c3-modustoolbox-applicationnotes-en.pdf?fileId=8ac78c8c92bcf0b0019393f072d813b5">AN238329</a></td>
    <td>Getting started with PSOC™ Control C3 MCU on ModusToolbox™ software</td>
  </tr>
  <tr>
    <td>Application note</td>
	<td><a href="https://www.infineon.com/assets/row/public/documents/30/42/infineon-an239646-pmsm-foc-psoc-control-c3-mcu-applicationnotes-en.pdf?fileId=8ac78c8c93956f5001939d61c8af4b9a">AN239646</a></td>
    <td>PMSM FOC using PSOC™ Control C3 MCU</td>
  </tr>
  <tr>
    <td>Code example</td>
	<td><a href="https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software">Using ModusToolbox™</a></td>
    <td>on Github</td>
  </tr>
  <tr>
    <td>Device documentation</td>
	<td><a href="https://www.infineon.com/products/microcontroller/32-bit-psoc-arm-cortex/32-bit-psoc-control-arm-cortex-m33-mcu#documents">PSOC™ Control C3</a></td>
    <td>MCU datasheets and reference manuals</td>
  </tr>
  <tr>
    <td>Development kits</td>
	<td><a href="https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board">Evaluation board finder</a></td>
    <td>Selecting the appropriate kits</td>
  </tr>
  <tr>
    <td>Library on Github</td>
	<td><a href="https://github.com/Infineon/mtb-pdl-cat1">mtb-pdl-cat1</a></td>
    <td>Peripheral Driver Library (PDL)</td>
  </tr>
  <tr>
    <td>Library on Github</td>
	<td><a href="https://github.com/Infineon/mtb-hal-cat1">mtb-hal-cat1</a></td>
    <td>Hardware Abstraction Layer (HAL) library (XMC7200 only)</td>
  </tr>
  <tr>
    <td>Library on Github</td>
	<td><a href="https://github.com/Infineon/retarget-io">retarget-io</a></td>
    <td>Utility library to retarget STDIO messages to a UART port</td>
  </tr>
  <tr>
    <td>Tools</td>
	<td><a href="https://www.infineon.com/modustoolbox">ModusToolbox™</a></td>
    <td>ModusToolbox™ software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSOC™ Industrial/IoT MCUs, AIROC™ Wi-Fi and Bluetooth® connectivity devices, XMC™ Industrial MCUs, and EZ-USB™/EZ-PD™ wired connectivity controllers. ModusToolbox™ incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development</td>
  </tr>
  <tr>
    <td>Tools</td>
	<td><a href="https://design.infineon.com/sensor/anglesim/index.html">Magnetic Design Tool: Angle Sensor</a></td>
    <td>Tool to measure the valid air-gap (distances from magnet surface to sensor) given a certain magnet size and remanence (diametrical magnetization), as well as to measure the worst case angle error caused by tilts and eccentricites of magnet and sensor elements against the axis of rotation. The worst case angle error ist taken at the worst rotational position for the maximum assembly tolerances combination. Simulation results based on worst case scenario.</td>
  </tr>
</table>
</p>
<br>


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.


All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth® word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

PSOC™, formerly known as PSoC™, is a trademark of Infineon Technologies. Any references to PSoC™ in this document or others shall be deemed to refer to PSOC™.

---------------------------------------------------------

© Cypress Semiconductor Corporation, 2025. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSoC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.

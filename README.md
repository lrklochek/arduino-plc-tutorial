## Getting Started

* Download [Arduino Desktop IDE](https://www.arduino.cc/en/Guide)

* Install necessary libraries
  * IRremote
  * LiquidCrystal_I2C
  * DS1307RTC
  
* Clone this repository into the Arduino directory

## Introduction

### Hardware
The PLC Mega 328 is a kit from Canaduino requiring soldering . Assembly instructions can be found [here](https://universal-solder.ca/downloads/CANADUINO_DIY_PLC_MEGA328_100-12_Assembling.pdf).

The PLC Mega 328 uses the Arduino Nano microcontroller and DS1327 Real Time Clock as well as the Infra Red Receiver, and a 16x2 LCD display.

There are 4 opto-isolated digital inputs, operating on an input voltage of 3V to 12V DC. The 6 digital outputs use the OMRON G5NB-1A-E relay which has a 10ms operate and release time, and 5A contacts.

Analog inputs and outputs are 0-10V.

Analog outputs are short circuit protected by limiting the maximum output current.

Analog inputs are protected against over voltage of max. 28V.

The NANO’s I2C bus is used to connect to the on-board RTC and it can also be accessed through the 4-pin female header besides the analog inputs section.

Since there are necessary pull-up resistors for the RTC chip already on board, the external IR Receiver that uses the I2C bus is connected without additional pullup resistors. The supply voltage for the PLC is 12V DC with a maximum current drawn of less than 500mA. Screw terminals fit wires 22-14 AWG (1.5mm max.) 
### Source Code
Five C source files are listed.
There is a file for the PLC / Infra Red Remote System. In addition there are three smaller files with no IR  interface or LCD displays and one file with LCD display only.  The smaller files run On Delay and Off Delay timers and are configured to include hours as well as minutes and seconds for timing purposes. A debouncing function for the digital inputs has been added. In addition a source file, created by ArduinoGetStarted.com, has been included to set the Real Time Clock for the file PLC_Date_Time.ino program, a year/month/day/hour/minute timer described below.
#### PLC_2_IR.ino
Source file for Infra Red Remote operation and display with i2CLCD1602. PLC functions can be selected by commenting or uncommenting functions in the looping code. These functions include On Delay timer, Off Delay timer, And/Or/Xor boolean logic, Sequencer, PID Controller, and 1st/2nd order simulator blocks.
#### PLC_4_OffDelay_Timers.ino
Source file for 6 Off Delay timers. Initiation of all timers is via a debounced Digital Input 3. No infra red operation or LCD display. Timers can be set in code from 1 second to 23 Hours 59 minutes and 59 seconds.
#### PLC_4_OnDelay_Timers.ino
Source file for 6 On Delay timers. Initiation of all timers is via a debounced Digital Input 3. No infra red operation or LCD display. Timers can be set in code from 1 second to 23 Hours 59 minutes and 59 seconds.
#### PLC_4_On_Off_Timers.ino
Source file for 2 On Delay and 2 Off Delay timers.No infra red operation or LCD display. Timers can be set in code from 1 second to 23 Hours 59 minutes and 59 seconds.
#### Set_Time_Date_Real_Time_Clock.ino (Created by ArduinoGetStarted.com - in the public domain)
Source file to set the Real Time Clock. Need to do this before uploading source file for PLC_Date_Time.ino. 
#### PLC_Date_Time.ino
Source file for timers. Relays 1 to 5 can each be set to turn on and off at prescribed year, month, day, hour, minute. Relay 6 can turn on or off at any of the same times entered for relays 1 through 5. Real Time Clock is battery backed up and set by uploading souce file Set_Time_Date_Real_Time_Clock.ino first.
#### PLC_Date_Time_ver_2.ino
This source file's Relay 1 uses a continous timing operation set in minutes, with an adjustable duty cycle. As for source file PLC_Date_Time.ino, Relays 2,3,4, and 5 can each be set to turn on and off at prescribed year, month, day, hour, minute. By performing logical operations such as ANDing and ORing, Relay 6, which has no inherent timing operation associated with it, can be made to incorporate all or any of the timing operations associated with Relays 1,2,3,4,or 5.

### PLC / Infra Red Remote System
<img alt="Image of the PLC setup" src="images/IR PLC.jpg">
The PLC/Infra Red (IR) Remote System consists of the PLCMega 328, an I2C LC1602 display, an IR Receiver module with and I2C bus interface, an external 5 VDC power supply for the IR Receiver (the 5 VDC could be supplied by the PLC), and an IR Remote. The choice of the IR Remote will dictate the coding of the IR Receive function.

### Purpose of the Project and Documentation
The project could be adapted for a variety of academic lab experiences such as electrical, computer, and mechatronic engineering, as well as computer technician/technology training. The intent of the project is to develop the following skills:
* C Programming
* Industrial automation
* Process Control
* Microcontroller applications
* Assembly/soldering

The labs could be organized to include/exclude the following:
1. Assemble the PLC
2. Develop Boolean code functions
3. Develop On Delay Timer Function
4. Develop Off Delay Timer Function
5. Develop Sequencer
6. Develop an industrial automation operation such as a pick and place robot
7. Develop and/or use the PID and internal Simulator
8. Develop a control operation using the PID controller
9. Develop LCD Interface
10. Develop IR Interface

### Software
The code is written in C and developed on the Arduino Integrated Develeopment Environment (IDE). The block diagram on the next page offers a simplified description of the organization of the code.

The intent was to create functions that implement in a similar manner, the ladder logic tools used on PLCs in the field of Industrial Automation. The following functions were developed:

* Boolean functions AND, OR, and XOR (exclusive Or).
* On Delay Timer
* Off Delay Timer
* Up Counter
* Down Counter
* Sequencer
* PID Controller

The PID controller output, controlled variable and set point are normalized 0 to 1.0. Inputting /Outputing will require mapping the normalized variables to the analog input/outputs.
Two first order lag simulator functions are included to experiment with the PID Controller. The lags can be cascaded to simulate a second order system. The simulator process variables are nomalized 0 to 1.0.

The infra red (IR) receiver function allows interaction in real time with the On Delay timer, Off Delay timer, sequencer, and analog output. The IR receiver also allows starting and stopping scanning.

Timing settings are displayed on the LCD1602. In addition, the On and Off delay timers can start and reset the timers via the IR remote.

<img alt="Image of the IR Remote" src="images/IR_Remote.jpg">

  * **Red Power Button** – tuns Scan On/Off
  * **FUNC STOP** On Delay Start/Stop
  * **Vol+** On Delay Increase Seconds
  * **Vol-** On Delay Decrease Seconds
  * **^** On Delay Increase Minutes
  * **V** On Delay Decrease Minutes
  * **ST/RPT** Off Delay Start/Stop
  * **|>|>|** Off Delay Increase Seconds
  * **|<|<|** Off Delay Decrease Seconds
  * **|>||** Off Delay Increase Minutes
  * **EQ** Off Delay Decrease Minutes
  * **0** Analog Out Count Increase
  * **9** Analog Out Count Decrease 

The settings for all the functions can be set directly in the code rather than using the IR Remote. However, the IR Remote allows the settings to be changed at run time. As a DS1307 real time clock is used for timing, the On and Off Delay timer implementation does not require the delay (ms) function. That is the scan operates without delay. The On and Off Delay Timers can be set from 1 sec to 59 minutes and 59 seconds. If the PID controller is used there will be a necessary 20 msec delay due to the integration and differentiation required when implementing PID.
### PLC Program Structure
<img alt="PLC Program Structure Scaled" src="images/PLC Program Structure Scaled.jpg">

### Ladder Logic Representation Of PLC Functions Used


####      On Delay and Off Delay Timer
<img alt="Timers" src="images/Timers Scaled.jpg">
____________________________________________________________________________________________________________

####     Up Counter and Down Counter

<img alt="Counters" src="images/Counters Scaled 3.jpg">
____________________________________________________________________________________________________________

<img alt="Boolean Logic" src="images/Boolean Logic Scaled 2.jpg">
____________________________________________________________________________________________________________

####   Sequencer
<img alt="Sequencer" src="images/Sequencer Scaled.jpg">
____________________________________________________________________________________________________________

####   PID Controller
<img alt="PID Controller" src="images/PID Function Scaled.jpg">

## PLC Timer
### Source File: PLC_Date_Time_ver_2.ino
The 6 relays of the PLC Timer operate differently with respect to timing capabilities. However by performing logical operations such as ANDing and ORing the Relay 6 which has no inherent timing operation associated with it, can be made to incorporate all or any of the timing operations associated with Relays 1,2,3,4,or 5.

The diagram on the next page describes the parameters that can be set to perform the various timing functions.

The description below illustrates an example of the possibilities for setting the timer.

* Set Relay 2: September 24 at 13 hours and 20 minutes to start, 
* Set Relay 2: September 25 to 10 hours 15 minutes to end

* Set Relay 1 to 20 minutes to start
* Set Relay 1 to 30 minutes to stop.
* Set Relay 1 delta 45 minutes 

* Relay 6 Trigger By Logically ANDing Relay 1 and Relay 2 Triggers 
* Result with have Relay 6 turning on at September 24, 13, hours and 20 minutes.
* Relay 6 will stay on for 10 minutes, then stop.
* Relay 6 will turn on again at September 24 14 hours and 5 minutes (65-60)
* Relay 6 will turn off again at September 24 14 hours and 15 minutes (75-60)

This pattern will continue until September 25 at 10 hours and 15 minutes

* Start September 24, 13 hours, 20 minutes
* 9/24/13/20 Relay 6 ON
* 9/24/13/30 Relay 6 OFF
* 9/24/14/05 Relay 6 ON
* 9/24/14/15 Relay 6 OFF
  * etc
  * etc
  * etc
* 9/25/10/15 Relay 6 OFF 
__________________________________________________________________
<img alt="plc-timer-operation" src="images/plc-timer-operation Scaled.jpg">
_________________________________________________________________
<img alt="timing-formula" src="images/timing-formula Scaled.jpg">


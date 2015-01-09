/**
  ******************************************************************************
  * @file    rgb_functions.h
  * @authors  JP Gal
  * @version V0.0.1
  * @date    16-December-2014
  * @brief   User Application File Header
  ******************************************************************************
  Copyright (c) 2014 TAOLight.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
  */

#include "rgb_functions.h"
#include <spark_wiring.h>

// Default LED Pins
int GCtrl_def_rLed   = A4;
int GCtrl_def_gLed   = A1;
int GCtrl_def_bLed   = A0;

int GCtrl_def_wwLed  = D0;
int GCtrl_def_cwLed  = D1;

GradientControls GCtrl = {
		{GCtrl_def_rLed, 0, 1, 0, 0, 255, 0, 0},
		{GCtrl_def_gLed, 0, 1, 0, 0, 255, 0, 0},
		{GCtrl_def_bLed, 0, 1, 0, 0, 255, 0, 0},

		{GCtrl_def_wwLed, 0, 1, 0, 0, 255, 0, 0},
		{GCtrl_def_cwLed, 0, 1, 0, 0, 255, 0, 0}};

/*
 *

// This is what we use to define a program
typedef struct {
	uint8_t val_curr;
	int8_t  val_incr;
	uint8_t val_min;
	uint8_t val_max;
	uint8_t delay;
	 int8_t loop_number;
} GradientParamStruct;

// This is what lihnk a program to a LED
typedef struct {
	uint8_t led_pin;
	uint8_t val_curr;
	 int8_t val_incr;
	uint8_t val_min;
	uint8_t val_max;
	uint8_t delay;
	uint8_t loop_current;
	 int8_t loop_number;
} GradientControlStruct;

// this is the list of 5 on going programs
typedef struct {
	GradientControl r;
	GradientControl g;
	GradientControl b;

	GradientControl ww;
	GradientControl cw;
} GradientControls;

*/

GradientControlStruct _resetProgram(GradientControlStruct programStructure){
	programStructure.val_curr 	  = programStructure.val_min;
	programStructure.loop_current = 0;
	return programStructure;
}

GradientControlStruct _stopProgram(GradientControlStruct programStructure){
	programStructure.val_incr	  = 0;
	return programStructure;
}

GradientControlStruct _removeProgram(GradientControlStruct programStructure){
	programStructure.val_curr 	  = 0;
	programStructure.val_incr	  = 0;
	programStructure.val_min	  = 0;
	programStructure.val_max	  = 0;
	programStructure.delay		  =	0;
	programStructure.loop_current = 0;
	programStructure.loop_number  = 0;
	return programStructure;
}

GradientControlStruct _applyProgram(GradientParamStruct programSettings, GradientControlStruct programStructure){
	programStructure.val_curr 	  = programSettings.val_curr;
	programStructure.val_incr	  = programSettings.val_incr;
	programStructure.val_min	  = programSettings.val_min;
	programStructure.val_max	  = programSettings.val_max;
	programStructure.delay		  =	programSettings.delay;
	programStructure.loop_current = 0;
	programStructure.loop_number  = programSettings.val_curr;
	return programStructure;
}

GradientControlStruct _iterateProgram(GradientControlStruct programStructure){
	int8_t newVal = -1;

	if (programStructure.val_incr != 0 and (programStructure.loop_number == -1 or programStructure.loop_current < programStructure.loop_number)){
		newVal = programStructure.val_curr + programStructure.val_incr;
		if (newVal >= programStructure.val_min and newVal <= programStructure.val_max){
			programStructure.val_curr = newVal;
			programStructure.loop_current += 1;
		}
	}
	if (programStructure.loop_current >= programStructure.loop_number){
		programStructure.val_incr = 0;
	}

	return programStructure;
}

GradientControlStruct _iterateAndApplyProgram(GradientControlStruct programStructure){

	if (programStructure.val_incr == 0){
		return programStructure;
	}
	programStructure = _iterateProgram(programStructure);
	analogWrite(programStructure.led_pin, programStructure.val_curr);

	return programStructure;
}

int GCtrl_set(int pinNumber, GradientParamStruct params)
{
	Serial.println(String("GCtrl_set(") + pinNumber + ")");
	int setPinNumber = 0;

	if (pinNumber == GCtrl.r.led_pin){ GCtrl.r   = _applyProgram(params,GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.g.led_pin){ GCtrl.g   = _applyProgram(params,GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.b.led_pin){ GCtrl.b   = _applyProgram(params,GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.ww.led_pin){ GCtrl.ww = _applyProgram(params,GCtrl.ww);  setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.cw.led_pin){ GCtrl.cw = _applyProgram(params,GCtrl.cw);  setPinNumber = pinNumber;}

	Serial.println(String("GCtrl_set(") + pinNumber + ")"+ setPinNumber);
	return setPinNumber;
}

int GCtrl_reset(int pinNumber)
{
	Serial.println(String("GCtrl_reset(") + pinNumber + ")");
	int setPinNumber = 0;

	if (pinNumber == GCtrl.r.led_pin){ GCtrl.r   = _resetProgram(GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.g.led_pin){ GCtrl.g   = _resetProgram(GCtrl.g);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.b.led_pin){ GCtrl.b   = _resetProgram(GCtrl.b);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.ww.led_pin){ GCtrl.ww = _resetProgram(GCtrl.ww);  setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.cw.led_pin){ GCtrl.cw = _resetProgram(GCtrl.cw);  setPinNumber = pinNumber;}

	Serial.println(String("GCtrl_set(") + pinNumber + ")"+ setPinNumber);
	return pinNumber;
}

int GCtrl_stop(int pinNumber)
{
	Serial.println(String("GCtrl_stop(") + pinNumber + ")");
	int setPinNumber = 0;

	if (pinNumber == GCtrl.r.led_pin){ GCtrl.r   = _stopProgram(GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.g.led_pin){ GCtrl.g   = _stopProgram(GCtrl.g);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.b.led_pin){ GCtrl.b   = _stopProgram(GCtrl.b);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.ww.led_pin){ GCtrl.ww = _stopProgram(GCtrl.ww);  setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.cw.led_pin){ GCtrl.cw = _stopProgram(GCtrl.cw);  setPinNumber = pinNumber;}

	Serial.println(String("GCtrl_stop(") + pinNumber + ")"+ setPinNumber);
	return pinNumber;
}

int GCtrl_remove(int pinNumber)
{
	Serial.println(String("GCtrl_remove(") + pinNumber + ")");
	int setPinNumber = 0;

	if (pinNumber == GCtrl.r.led_pin){ GCtrl.r   = _resetProgram(GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.g.led_pin){ GCtrl.g   = _resetProgram(GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.b.led_pin){ GCtrl.b   = _resetProgram(GCtrl.r);   setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.ww.led_pin){ GCtrl.ww = _resetProgram(GCtrl.ww);  setPinNumber = pinNumber;}
	if (pinNumber == GCtrl.cw.led_pin){ GCtrl.cw = _resetProgram(GCtrl.cw);  setPinNumber = pinNumber;}

	Serial.println(String("GCtrl_remove(") + pinNumber + ")"+ setPinNumber);
	return pinNumber;
}

int GCtrl_resetAll()
{
	Serial.println("GCtrl_resetAll()");
	return 0;
}


int GCtrl_iterate()
{
	Serial.println("GCtrl_iterate()");
	GCtrl.r    = _iterateAndApplyProgram(GCtrl.r);
	Serial.println("GCtrl_iterate() R  done");
	GCtrl.g    = _iterateAndApplyProgram(GCtrl.g);
	Serial.println("GCtrl_iterate() G  done");
	GCtrl.b    = _iterateAndApplyProgram(GCtrl.b);
	Serial.println("GCtrl_iterate() B  done");
	GCtrl.ww   = _iterateAndApplyProgram(GCtrl.ww);
	Serial.println("GCtrl_iterate() WW  done");
	GCtrl.cw   = _iterateAndApplyProgram(GCtrl.cw);
	Serial.println("GCtrl_iterate() CW  done");

	Serial.println("GCtrl_iterate() ALL Done");
	return 0;
}

void GCtrl_setup(){
	pinMode(GCtrl_def_rLed,  OUTPUT);
	pinMode(GCtrl_def_gLed,  OUTPUT);
	pinMode(GCtrl_def_bLed,  OUTPUT);
	pinMode(GCtrl_def_wwLed, OUTPUT);
	pinMode(GCtrl_def_cwLed, OUTPUT);
}













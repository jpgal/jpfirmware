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
 * typedef struct {
	uint8_t val_curr;
	uint8_t val_incr;
	uint8_t val_min;
	uint8_t val_max;
	uint8_t delay;
} GradientParamStruct;

typedef struct {
	uint8_t led_pin;
	uint8_t val_curr;
	uint8_t val_incr;
	uint8_t val_min;
	uint8_t val_max;
	uint8_t delay;
	uint8_t loop_current;
	uint8_t loop_number;
} GradientControlStruct;

typedef struct {
	GradientControl r;
	GradientControl g;
	GradientControl b;

	GradientControl ww;
	GradientControl cw;
} GradientControls;

*/

int GCtrl_set(int pinNumber, GradientParamStruct params)
{
	Serial.println(String("GCtrl_set(") + pinNumber + ")");
	return pinNumber;
}

int GCtrl_reset(int pinNumber)
{
	Serial.println(String("GCtrl_reset(") + pinNumber + ")");
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
	return 0;
}


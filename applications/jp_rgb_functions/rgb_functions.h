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

#ifndef RGB_FUNCTIONS_H_
#define RGB_FUNCTIONS_H_

#include "spark_wiring.h"
#include "spark_wiring_interrupts.h"
#include "spark_wiring_string.h"
#include "spark_wiring_print.h"
#include "spark_wiring_usartserial.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_spi.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_servo.h"
#include "spark_wiring_wifi.h"
#include "spark_wiring_network.h"
#include "spark_wiring_client.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_tcpserver.h"
#include "spark_wiring_udp.h"
#include "spark_wiring_time.h"
#include "spark_wiring_tone.h"
#include "spark_wiring_eeprom.h"


// Structures to control one gradient
typedef struct {
	int val_curr;
	int val_incr;
	int val_min;
	int val_max;
	int delay;
} GradientParamStruct;

typedef struct {
	int led_pin;
	int val_curr;
	int val_incr;
	int val_min;
	int val_max;
	int delay;
	int loop_current;
	int loop_number;
} GradientControlStruct;

// The list of gradients to control the 5 LEDs
typedef struct {
	GradientControlStruct r;
	GradientControlStruct g;
	GradientControlStruct b;

	GradientControlStruct ww;
	GradientControlStruct cw;
} GradientControls;

extern GradientControls GCtrl;

// The functions
extern int GCtrl_set(int pinNumber, GradientParamStruct params);	// creates a new Gradient
extern int GCtrl_reset(int pinNumber);								// remove an existing Gradients
extern int GCtrl_resetAll();										// remove all Gradients (set all LED to LOW)
extern int GCtrl_iterate();											// iterate the gradients


#endif /* RGB_FUNCTIONS_H_ */

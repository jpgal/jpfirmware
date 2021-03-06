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

#ifndef IDUAL_PROGRAMS_H_
#define IDUAL_PROGRAMS_H_

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


extern int PCtrl_def_rLed;
extern int PCtrl_def_gLed;
extern int PCtrl_def_bLed;

extern int PCtrl_def_wwLed;
extern int PCtrl_def_cwLed;

// Structures to control one program step
typedef struct {
	int r;
	int g;
	int b;
	int ww;
	int cw;
	int delay;
} ProgramStepStruct;

// The functions
extern void PCtrl_set(ProgramStepStruct *steps, int size);	// Apply a program
extern void PCtrl_reset();									// Restart from the beginning of the program
extern int  PCtrl_iterate(int smooth);						// Iterate through the program steps
															// (smooth is to sub-divize each steps in smaller steps)
extern void PCtrl_setup();

#endif /* IDUAL_PROGRAMS_H_ */

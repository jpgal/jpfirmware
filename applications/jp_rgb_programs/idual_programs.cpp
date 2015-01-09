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
int PCtrl_def_rLed   = A4;
int PCtrl_def_gLed   = A1;
int PCtrl_def_bLed   = A0;

int PCtrl_def_wwLed  = D0;
int PCtrl_def_cwLed  = D1;


// Structures to control one program step
typedef struct {
	int r;
	int g;
	int b;
	int ww;
	int cw;
	int delay;
} ProgramStepStruct;

ProgramStepStruct *currentProgram;
ProgramStepStruct  cs;
int                currentProgramStep;

// The functions
int PCtrl_set(ProgramStepStruct *steps){
	currentProgram     = steps;
	currentProgramStep = 0;
}
int PCtrl_reset(){
	currentProgramStep = 0;
}
int PCtrl_iterate(){
	if (currentProgram != NULL){
		if (currentProgram[currentProgramStep] != NULL){
			cs = currentProgram[currentProgramStep];
			Serial.println(String("P(")+ String(currentProgramStep) +") ( " + cs.r + ", "+ cs.g + ", "+ cs.b + ", "+ cs.ww + ", "+ cs.cw +")");
			currentProgramStep += 1;
		} else {
			currentProgramStep = 0;
			Serial.println(String("P(")+ String(currentProgramStep) +") LOOP ");
		}
	}
}
void PCtrl_setup(){
	currentProgram = NULL;
	pinMode(PCtrl_def_rLed,  OUTPUT);
	pinMode(PCtrl_def_gLed,  OUTPUT);
	pinMode(PCtrl_def_bLed,  OUTPUT);
	pinMode(PCtrl_def_wwLed, OUTPUT);
	pinMode(PCtrl_def_cwLed, OUTPUT);
	currentProgramStep = 0;
}









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
int                currentProgramSize = 0;
ProgramStepStruct  cs;
ProgramStepStruct  cs_prev;

int                currentProgramStep;
int                currentProgramHalfStep;
int halfstepnumbers = 3;


// The functions
void PCtrl_set(ProgramStepStruct *steps, int size){
	currentProgram     = steps;
	currentProgramSize = size;
	currentProgramStep = 0;
	currentProgramHalfStep = 0;
}

void PCtrl_reset(){
	currentProgramStep = 0;
	currentProgramHalfStep = 0;
}

int PCtrl_iterate(int smooth){
	int r  = 0;
	int g  = 0;
	int b  = 0;
	int cw = 0;
	int ww = 0;

	int return_delay  = 100;

	if (currentProgram != NULL){
		if (smooth == 1){
			return_delay  = (int)(cs.delay / halfstepnumbers);
		} else {
			return_delay  = cs.delay;
		}

		if (currentProgramStep < currentProgramSize){
			cs = currentProgram[currentProgramStep];
			return_delay  = cs.delay;

			if ((currentProgramStep > 1) && (currentProgramHalfStep > 0)){
				cs_prev = currentProgram[currentProgramStep-1];
				r  = (cs.r *(currentProgramHalfStep)     + cs_prev.r *(halfstepnumbers-currentProgramHalfStep))/halfstepnumbers;
				g  = (cs.g *(currentProgramHalfStep)     + cs_prev.g *(halfstepnumbers-currentProgramHalfStep))/halfstepnumbers;
				b  = (cs.b *(currentProgramHalfStep)     + cs_prev.b *(halfstepnumbers-currentProgramHalfStep))/halfstepnumbers;
				cw = (cs.cw*(currentProgramHalfStep)     + cs_prev.cw*(halfstepnumbers-currentProgramHalfStep))/halfstepnumbers;
				ww = (cs.ww*(currentProgramHalfStep)     + cs_prev.ww*(halfstepnumbers-currentProgramHalfStep))/halfstepnumbers;

				//Serial.println(String("P(")+ String(currentProgramStep) +") "+ currentProgramHalfStep +" ( " + cs.r + ", "+ cs.g + ", "+ cs.b + ", "+ cs.ww + ", "+ cs.cw +") ( " + r + ", "+ g + ", "+ b + ", "+ ww + ", "+ cw +")" );
				analogWrite(PCtrl_def_rLed,  r);
				analogWrite(PCtrl_def_gLed,  g);
				analogWrite(PCtrl_def_bLed,  b);
				analogWrite(PCtrl_def_wwLed, ww);
				analogWrite(PCtrl_def_cwLed, cw);

				currentProgramHalfStep += 1;
				if (currentProgramHalfStep == halfstepnumbers+1){
					currentProgramHalfStep = 0;
				}
				return_delay  = (int)(cs.delay / halfstepnumbers);

			} else {
				Serial.println(String("P(")+ String(currentProgramStep) +") --- ( " + cs.r + ", "+ cs.g + ", "+ cs.b + ", "+ cs.ww + ", "+ cs.cw +")");

				analogWrite(PCtrl_def_rLed,  cs.r);
				analogWrite(PCtrl_def_gLed,  cs.g);
				analogWrite(PCtrl_def_bLed,  cs.b);
				analogWrite(PCtrl_def_wwLed, cs.ww);
				analogWrite(PCtrl_def_cwLed, cs.cw);
				currentProgramStep += 1;

				if (cs.delay % 2 == 1){
					currentProgramHalfStep = 1;
					return_delay  = (int)(cs.delay / halfstepnumbers);
				}
			}
		} else {
			currentProgramStep = 0;
			currentProgramHalfStep = 0;
			Serial.println(String("P(")+ String(currentProgramStep) +") LOOP ");
		}
		return return_delay;
	}
	return -1;
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









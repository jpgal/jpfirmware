/**
 ******************************************************************************
  * @file    application.cpp
  * @authors  JP Gal
  * @version V0.0.1
  * @date    15-December-2014
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

/* Includes ------------------------------------------------------------------*/  
#include "application.h"
#include "idual_programs.h"

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);


int ledBI = D7;  // This one is the built-in tiny one to the right of the USB jack

GradientParamStruct simpleGradientPer10 = {0, 10, 0, 255, 0, 3};


ProgramStepStruct *myPgram =
	{{  0,   0, 200,   0,   0,   10},
	 { 10,   0, 180,   0, 100,   10},
	 { 20,   0, 160,   0, 100,   10},
	 { 30,   0, 140,   0,   0,   10},
	 { 40,   0, 120,   0,   0,   10},
	 { 50,   0, 100,   0, 100,   10},
	 { 60,   0,  80,   0, 100,   10},
	 { 70,   0,  60,   0,   0,   10},
	 { 80,   0,  40,   0,   0,   10},
	 { 90,   0,  20,   0, 100,   10},
	 {100,   0,   0,   0, 100,   10},
	 {110,   0,   0,   0,   0,   10},
	 {120,   0,   0,   0,   0,   10},
     {130,   0,   0,   0, 100,   10}};

/* This function is called once at start up ----------------------------------*/
void setup()
{
    // Connecting
	pinMode(ledBI, OUTPUT);
	GCtrl_setup();

	Serial.begin(9600);
	int waiting_serial_count = 0;
	while(!Serial.available() and (waiting_serial_count < 100)){ // wait for 10 seconds
   		SPARK_WLAN_Loop();

   		digitalWrite(ledBI, HIGH);
		delay(65);

		digitalWrite(ledBI, LOW);
   		delay(25);

   		waiting_serial_count += 1;
   	}

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println(WiFi.localIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.SSID());

	Serial.println("Falshing GCtrl()...");
	delay(5000);
	digitalWrite(ledBI, HIGH);
	digitalWrite(PCtrl_def_rLed,  HIGH);
	digitalWrite(PCtrl_def_gLed,  HIGH);
	digitalWrite(PCtrl_def_bLed,  HIGH);
	digitalWrite(GCtrl_def_wwLed, HIGH);
	digitalWrite(GCtrl_def_cwLed, HIGH);
	delay(5000);
	digitalWrite(GCtrl_def_rLed,  LOW);
	digitalWrite(GCtrl_def_gLed,  LOW);
	digitalWrite(GCtrl_def_bLed,  LOW);
	digitalWrite(GCtrl_def_wwLed, LOW);
	digitalWrite(GCtrl_def_cwLed, LOW);
	digitalWrite(ledBI, LOW);

	//tcpClient.connect("192.168.11.90", 80);

	//Register all the Tinker functions
    Serial.println("Declaring Spark functions...");
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);

	Serial.println("Setup done...");
}

int counter = 0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	if ((counter % 60000) == 0){ // every 60 seconds
		digitalWrite(ledBI, HIGH);
		Serial.println("-------------- Gradient Starting ----------- ");
		delay(1000);
		digitalWrite(ledBI, LOW);
		GCtrl_set(GCtrl_def_wwLed, simpleGradientPer10);	// creates a new Gradient
	} else 	if ((counter % 10000) == 0){ // every 10 seconds
		digitalWrite(ledBI, HIGH);
		Serial.println("-------------- Big Flash ----------- ");
		delay(1000);
		digitalWrite(ledBI, LOW);
	} else if ((counter%1000) == 0){ // every 1 second
		digitalWrite(ledBI, HIGH);
		delay(50);
		digitalWrite(ledBI, LOW);
	}

	delay(10);
	counter = counter + 10;	// wait 10 ms
}



/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT);
		return analogRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT);
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}

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

/* For manual set colors------------------------------------------------------*/
int ledCW  = D0;
int ledWW  = D1;

int ledR  = A4;
int ledG  = A1;
int ledB  = A0;

int usingProgram = 1;

int counter    = 0;
int ledBI = D7;  // This one is the built-in tiny one to the right of the USB jack

ProgramStepStruct myPgram[118] = {
{ 128,   0,   0,  43,  64, 1000},
{  59,   0,   0,  49,  58, 1000},
{  12,   0,   0,  55,  53, 1000},
{   1,   0,   0,  62,  48, 1000},
{  30,   0,   0,  69,  45, 1000},
{  89,   0,   0,  76,  43, 1000},
{ 160,   0,   0,  82,  43, 1000},
{ 222,   0,   0,  86,  44, 1000},
{ 254,   0,   0,  89,  48, 1000},
{ 247,   0,   0,  90,  52, 1000},
{ 202,   0,   0,  88,  57, 1000},
{ 135,   0,   0,  85,  63, 1000},
{  65,   0,   0,  80,  69, 1000},
{  15,   0,   0,  74,  75, 1000},
{   0,   0,   0,  67,  80, 1000},
{  25,   0,   0,  59,  83, 1000},
{  83,   0,   0,  53,  85, 1000},
{ 154,   0,   0,  47,  85, 1000},
{ 217,   0,   0,  42,  84, 1000},
{ 252,   0,   0,  39,  81, 1000},
{ 249,   0,   0,  38,  76, 1000},
{ 208,   0,   0,  40,  71, 1000},
{ 142,   0,   0,  43,  65, 1000},
{  71,   0,   0,  48,  59, 1000},
{  18,   0,   0,  54,  54, 1000},
{   0,   0,   0,  61,  49, 1000},
{  22,   0,   0,  68,  45, 1000},
{   0,   0,   0,  75,  43, 1000},
{   0, 147,   0,  81,  43, 1000},
{   0, 212,   0,  86,  44, 1000},
{   0, 251,   0,  89,  47, 1000},
{   0, 251,   0,  90,  51, 1000},
{   0, 213,   0,  89,  56, 1000},
{   0, 148,   0,  86,  62, 1000},
{   0,  77,   0,  81,  68, 1000},
{   0,  22,   0,  75,  74, 1000},
{   0,   0,   0,  68,  79, 1000},
{   0,  18,   0,  61,  83, 1000},
{   0,  70,   0,  54,  85, 1000},
{   0, 140,   0,  48,  85, 1000},
{   0, 207,   0,  43,  84, 1000},
{   0, 249,   0,  40,  81, 1000},
{   0, 253,   0,  38,  77, 1000},
{   0, 218,   0,  39,  72, 1000},
{   0, 155,   0,  42,  66, 1000},
{   0,  84,   0,  47,  60, 1000},
{   0,  26,   0,  52,  55, 1000},
{   0,   0,   0,  59,  50, 1000},
{   0,  15,   0,  66,  46, 1000},
{   0,  64,   0,  74,  43, 1000},
{   0, 134,   0,  80,  43, 1000},
{   0, 201,   0,  85,  44, 1000},
{   0, 246,   0,  88,  46, 1000},
{   0, 254,   0,  90,  50, 1000},
{   0, 223,   0,  89,  55, 1000},
{   0, 162,   0,  86,  61, 1000},
{   0,  90,   0,  82,  67, 1000},
{   0,  30,   0,  76,  73, 1000},
{   0,   1, 147,  69,  78, 1000},
{   0,  12, 212,  62,  82, 1000},
{   0,   0, 251,  55,  84, 1000},
{   0,   0, 251,  49,  85, 1000},
{   0,   0, 213,  44,  85, 1000},
{   0,   0, 148,  40,  82, 1000},
{   0,   0,  77,  38,  78, 1000},
{   0,   0,  22,  39,  73, 1000},
{   0,   0,   0,  41,  67, 1000},
{   0,   0,  18,  46,  61, 1000},
{   0,   0,  70,  51,  56, 1000},
{   0,   0, 140,  58,  50, 1000},
{   0,   0, 207,  65,  46, 1000},
{   0,   0, 249,  72,  44, 1000},
{   0,   0, 253,  79,  43, 1000},
{   0,   0, 218,  84,  43, 1000},
{   0,   0, 155,  88,  46, 1000},
{   0,   0,  84,  89,  49, 1000},
{   0,   0,  26,  89,  54, 1000},
{   0,   0,   0,  87,  60, 1000},
{   0,   0,  15,  83,  66, 1000},
{   0,   0,  64,  77,  72, 1000},
{   0,   0, 134,  71,  77, 1000},
{   0,   0, 201,  64,  81, 1000},
{   0,   0, 246,  56,  84, 1000},
{   0,   0, 254,  50,  85, 1000},
{   0,   0, 223,  44,  85, 1000},
{   0,   0, 162,  41,  83, 1000},
{   0,   0,  90,  39,  79, 1000},
{   0,   0,  30,  39,  74, 1000},
{   0,   0,   1,  41,  68, 1000},
{   0,   0,  12,  45,  62, 1000},
{   0,   0,   0,  50,  57, 1000},
{   0,   0,   0,  57,  51, 1000},
{   0,   0,   0,  64,  47, 1000},
{   0,   0,   0,  71,  44, 1000},
{   0,   0,   0,  78,  43, 1000},
{   0,   0,   0,  83,  43, 1000},
{   0,   0,   0,  87,  45, 1000},
{   0,   0,   0,  89,  49, 1000},
{   0,   0,   0,  89,  53, 1000},
{   0,   0,   0,  88,  59, 1000},
{   0,   0,   0,  84,  65, 1000},
{   0,   0,   0,  78,  71, 1000},
{   0,   0,   0,  72,  76, 1000},
{   0,   0,   0,  65,  81, 1000},
{   0,   0,   0,  58,  84, 1000},
{   0,   0,   0,  51,  85, 1000},
{   0,   0,   0,  45,  85, 1000},
{   0,   0,   0,  41,  83, 1000},
{   0,   0,   0,  39,  80, 1000},
{   0,   0,   0,  35,  75, 1000},
{   0,   0,   0,  32,  70, 1000},
{   0,   0,   0,  30,  64, 1000},
{   0,   0,   0,  20,  50, 1000},
{   0,   0,   0,  15,  40, 1000},
{   0,   0,   0,  10,  30, 1000},
{   0,   0,   0,   5,  20, 1000},
{   0,   0,   0,   0,  10, 1000},
{   0,   0,   0,   0,   2, 5000},
};






int receiveRGBNotification(String command) {
    int loc1 = 0;
    int loc2 = 0;
    int loc3 = 0;

    int RValue = 0;
    int GValue = 0;
    int BValue = 0;

    digitalWrite(ledBI, HIGH);

	Serial.println(String("RGB Received ")+ command);
    loc1 = command.indexOf(",");
    RValue = command.substring(0,loc1).toInt();

    loc2 = command.indexOf(",",loc1+1);
    GValue = command.substring(loc1+1,loc2).toInt();

    loc3 = command.indexOf(",",loc2+1);
    BValue = command.substring(loc2+1,loc3).toInt();

    analogWrite(PCtrl_def_rLed,  RValue);
    analogWrite(PCtrl_def_gLed,  GValue);
    analogWrite(PCtrl_def_bLed,  BValue);

    digitalWrite(ledBI, LOW);

    counter = 0;
    return (RValue+GValue*256+BValue*256*256);
}

int receiveWhiteNotification(String command) {
    int loc1 = 0;
    int loc2 = 0;

    int CWValue = 0;
    int WWValue = 0;

    digitalWrite(ledBI, HIGH);

	Serial.println(String("White Received ")+ command);

	loc1 = command.indexOf(",");
    CWValue = command.substring(0,loc1).toInt();

    loc2 = command.indexOf(",",loc1+1);
    WWValue = command.substring(loc1+1,loc2).toInt();

    analogWrite(PCtrl_def_cwLed, CWValue);
    analogWrite(PCtrl_def_wwLed, WWValue);

    digitalWrite(ledBI, LOW);

    counter = 0;
    return (CWValue+WWValue*256);
}

int receiveProgramNotification(String command) {
    int progValue = 0;

    digitalWrite(ledBI, HIGH);

    progValue = command.substring(0,1).toInt();
	Serial.println(String("Prog Received ")+ progValue);
    if (progValue == 0){
    	usingProgram = 0;
    } else {
    	usingProgram = 1;
    }

    digitalWrite(ledBI, LOW);

    return (usingProgram);
}

/* This function is called once at start up ----------------------------------*/
void setup()
{
    // Connecting
	pinMode(ledBI, OUTPUT);

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
	delay(500);
	digitalWrite(ledBI, LOW);
	delay(100);

	Serial.println(WiFi.localIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.SSID());

	digitalWrite(ledBI, HIGH);
	delay(500);
	digitalWrite(ledBI, LOW);
	delay(100);
	PCtrl_setup();
	PCtrl_set(myPgram, 118);

	//tcpClient.connect("192.168.11.90", 80);

	//Register all the Tinker functions
    Serial.println("Declaring Spark functions...");
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);

	Spark.function("idual_rgb",  receiveRGBNotification);
	Spark.function("idual_prog", receiveProgramNotification);
	Spark.function("idual_ww",   receiveWhiteNotification);

	Spark.variable("testvalue", &counter, INT);

	Serial.println("Setup done...");
}

int step_delay = 0;
int smooth = 0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	if (usingProgram == 1){
		step_delay = PCtrl_iterate(smooth);
		delay(step_delay);
	} else {
		counter += 1;
	}
	if (counter % 118 == 0){
		smooth = 1 - smooth;
	}
	digitalWrite(ledBI, HIGH);
	delay(10);
	digitalWrite(ledBI, LOW);
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

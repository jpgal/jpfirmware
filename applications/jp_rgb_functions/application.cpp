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
#include "rgb_functions.h"
#include <spark_wiring.h>

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);


// Define the pins we're going to call pinMode on
int ledA0 = A0;  // You'll need to wire an LED to this one to see it blink.
int ledBI = D7;  // This one is the built-in tiny one to the right of the USB jack

int ledC = A7;
int ledG = A6;
int ledY = A5;
int ledR = A4;

int ledT = A1;	// Temperature Activity

IPAddress remoteTAOUpIP(192, 168,   8,  21);
IPAddress localNASIP   (192, 168,  11,  17);

boolean localNASConnected    = true;
boolean remoteTAOUpConnected = true;

//TCPClient tcpClient = TCPClient();
#define HIGH_A  150
#define LOW_A     0

#include "spark-dallas-temperature.h"

DallasTemperature dallas(new OneWire(D2));
DeviceAddress insideThermometer;

char szInfo[64];
char tmpBuf[64];

SYSTEM_MODE(AUTOMATIC);

void write4LEDs(int cValue, int gValue, int yValue, int rValue){
	analogWrite(ledC, cValue);
	analogWrite(ledG, gValue);
	analogWrite(ledY, yValue);
	analogWrite(ledR, rValue);
}

int displayTemperature(float celsius){
	int temperatureDisplayed = 0;

	if (celsius < 1){	  write4LEDs(  0,   0,   0,   0); }
	else if(celsius < 16){write4LEDs(255,  80,   0,   0);       temperatureDisplayed =  1;}

	if (celsius > 16){
		 if(celsius < 17){write4LEDs( 50,   0,   0,   0);	    temperatureDisplayed =  2;}
	else if(celsius < 18){write4LEDs(200,   0,   0,   0);	    temperatureDisplayed =  3;}
	else if(celsius < 21){write4LEDs(  0,  50,   0,   0);	    temperatureDisplayed =  4;}
	else if(celsius < 22){write4LEDs(  0, 200,   0,   0);	    temperatureDisplayed =  5;}
	else if(celsius < 23){write4LEDs(  0,   0,  50,   0);	    temperatureDisplayed =  6;}
	else if(celsius < 24){write4LEDs(  0,   0, 200,   0);	    temperatureDisplayed =  7;}
	else if(celsius < 25){write4LEDs(  0,   0,   0,  50);	    temperatureDisplayed =  8;}
	else if(celsius < 26){write4LEDs(  0,   0,   0, 200);	    temperatureDisplayed =  9;}
	else                 {write4LEDs(  0,   0,  80, 255);		temperatureDisplayed = 10;}
	}

	return temperatureDisplayed;
}



/* This function is called once at start up ----------------------------------*/
void setup()
{
    // Connecting
	pinMode(ledBI, OUTPUT);
	pinMode(ledA0, OUTPUT);

	pinMode(ledT, OUTPUT);

	pinMode(ledC, OUTPUT);
	pinMode(ledG, OUTPUT);
	pinMode(ledY, OUTPUT);
	pinMode(ledR, OUTPUT);

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.begin(9600);
	int waiting_serial_count = 0;
	while(!Serial.available() and (waiting_serial_count < 150)){ // wait for 15 seconds
   		SPARK_WLAN_Loop();
   		digitalWrite(ledBI, HIGH);
   		if (waiting_serial_count > 120){
   	   		analogWrite(ledA0, HIGH_A);
			delay(60);
			digitalWrite(ledBI, LOW);
			analogWrite(ledA0, LOW_A);
   		} else {
			delay(80);
			digitalWrite(ledBI, LOW);
   		}

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

	Serial.println("Setup...");

	//tcpClient.connect("192.168.11.90", 80);




	//Register all the Tinker functions
    Serial.println("Declaring Spark functions...");
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);


	analogWrite(ledA0, HIGH_A);
	digitalWrite(ledBI, HIGH);
	delay(500);
	analogWrite(ledA0, LOW_A);
	digitalWrite(ledBI, LOW);
	delay(100);

	analogWrite(ledA0, HIGH_A);
	digitalWrite(ledBI, HIGH);
	delay(500);
	analogWrite(ledA0, LOW_A);
	digitalWrite(ledBI, LOW);
	delay(100);

	Serial.println("Starting Sensor...");
	dallas.begin();
	Serial.println("Setting resolution to 12...");
	if (!dallas.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
	dallas.setResolution(insideThermometer, 12);
	Serial.println("Started  Sensor...");

	Serial.println("Publishing tempr variable on  the cloud...");
	Spark.variable("tempr", &szInfo, STRING);

  	delay(500);
	Serial.println("Flashing RGB...");
  	delay(1000);
  	analogWrite(ledC, HIGH_A);	delay(250);
  	analogWrite(ledG, HIGH_A);	delay(120);
  	analogWrite(ledY, HIGH_A);	delay(120);
  	analogWrite(ledR, HIGH_A);	delay(120);
  	delay(250);
  	analogWrite(ledC, LOW_A);	delay(120);
  	analogWrite(ledG, LOW_A);	delay(120);
  	analogWrite(ledY, LOW_A);	delay(120);
  	analogWrite(ledR, LOW_A);	delay(120);
  	delay(1000);

	Serial.println("Flashing Temperature Gradient...");
  	delay(1000);
  	for (float temp=15.0; temp < 32.0; temp += 1.0){
  		Serial.print("Temperature:");
  		Serial.println(temp);
  		displayTemperature(temp);
  		delay(1000);
  	}
	displayTemperature(0.0);

	Serial.println("Setup done...");
}

int counter = 0;
float current_temperature         = 0;
float current_temperature_display = 0;
int numberOfReceivedPackage = 0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	float celsius = 0.0;
	int   temperatureDisplayed = 0;

	if (counter > 5000){
		Serial.println("");
		/*
		Serial.println("------------ Flashing LEDs ----------------------------------");
		for (int i = 0; i < 1; i++){
			digitalWrite(ledBI, HIGH);
			delay(200);
			digitalWrite(ledBI, LOW);
			delay(50);
		}
		*/
		counter = 0;
		digitalWrite(ledBI, HIGH);

		Serial.print("-------------- Temperature ----------- ");
		Serial.flush();
		dallas.requestTemperatures(); // Send the command to get temperatures

		celsius  = dallas.getTempCByIndex(0);
		sprintf(tmpBuf, "%2.2fC", celsius);

		Serial.print(tmpBuf);
		Serial.flush();

		strcpy(szInfo, tmpBuf);
		Serial.println("------------");

		temperatureDisplayed = displayTemperature(celsius);

		if (temperatureDisplayed != current_temperature_display){
			// Changed the temperature displayed
			// We flash the main LED a few times
			current_temperature_display = temperatureDisplayed;
			for (int i = 0; i < 5; i++){
				digitalWrite(ledT, HIGH_A);
				delay(250);
				digitalWrite(ledT, LOW_A);
				delay(50);
			}
		} else if ((celsius - current_temperature > 0.5) or (current_temperature - celsius> 0.5)){
			// The temperature has changed
			// We flash once
			analogWrite(ledT, 100);
   		  	delay(250);
   		  	current_temperature = celsius;
   		  	analogWrite(ledT, LOW_A);
		}

		digitalWrite(ledBI, LOW);
		delay(20);
		digitalWrite(ledBI, HIGH);

		Serial.print("-------------- Pinging Local --------- ");
		Serial.flush();
		numberOfReceivedPackage = WiFi.ping(localNASIP);
		Serial.print(numberOfReceivedPackage);
		Serial.println(" ---");
		localNASConnected    = (numberOfReceivedPackage == 5);

		digitalWrite(ledBI, LOW);
		delay(20);
		digitalWrite(ledBI, HIGH);
		Serial.print("-------------- Pinging Remote -------- ");
		Serial.flush();
		numberOfReceivedPackage = WiFi.ping(remoteTAOUpIP);
		Serial.print(numberOfReceivedPackage);
		Serial.println(" ---");
		remoteTAOUpConnected = (numberOfReceivedPackage == 5);

	} else {
		delay(10);
		counter = counter + 10;	// wait 10 ms
		if ((counter%1000) == 0){
			Serial.print(counter/1000);
			Serial.print(" ");
			Serial.flush();

			if (not localNASConnected){
				for (int i = 0; i < 2; i++){
					digitalWrite(ledA0, HIGH);
					delay(200);
					digitalWrite(ledA0, LOW);
					delay(50);
				}
			} else if (not remoteTAOUpConnected){
				for (int i = 0; i < 1; i++){
					digitalWrite(ledA0, HIGH);
					delay(200);
					digitalWrite(ledA0, LOW);
					delay(50);
				}
			}
		}
	}

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

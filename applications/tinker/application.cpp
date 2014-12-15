/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    05-November-2013
 * @brief   Tinker application
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);


// Define the pins we're going to call pinMode on
int led  = D0;  // You'll need to wire an LED to this one to see it blink.
int led2 = D7;  // This one is the built-in tiny one to the right of the USB jack

//TCPClient tcpClient = TCPClient();

SYSTEM_MODE(AUTOMATIC);

/* This function is called once at start up ----------------------------------*/
void setup()
{
    // Connecting
	pinMode(led2, OUTPUT);

	digitalWrite(led2, HIGH);
	delay(1000);
	digitalWrite(led2, LOW);
	delay(200);

	Serial.begin(9600);
	int waiting_serial_count = 0;
	while(!Serial.available() and (waiting_serial_count < 150)){ // wait for 15 seconds
   		SPARK_WLAN_Loop();
   		digitalWrite(led2, HIGH);
   		if (waiting_serial_count > 120){
   	   		digitalWrite(led, HIGH);
   		}
   		delay(75);
   		digitalWrite(led2, LOW);
   		if (waiting_serial_count > 120){
   	   		digitalWrite(led, LOW);
   		}

   		delay(25);
   		waiting_serial_count += 1;
   	}

	digitalWrite(led2, HIGH);
	delay(1000);
	digitalWrite(led2, LOW);
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


      digitalWrite(led, HIGH);
	  digitalWrite(led2, HIGH);
	  delay(500);
	  digitalWrite(led, LOW);
	  digitalWrite(led2, LOW);
	  delay(100);

	  digitalWrite(led, HIGH);
	  digitalWrite(led2, HIGH);
	  delay(500);
	  digitalWrite(led, LOW);
	  digitalWrite(led2, LOW);
	  delay(100);

	  Serial.println("Setup done...");
}

int counter = 0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	if (counter > 5000){
		  Serial.println("------------ Flashing LEDs ----------------------------------");
		  for (int i = 0; i < 5; i++){
			  digitalWrite(led, HIGH);
			  digitalWrite(led2, HIGH);
			  delay(300);
			  digitalWrite(led, LOW);
			  digitalWrite(led2, LOW);
			  delay(100);
		  }
		  counter = 0;
	} else {
		  delay(10);
		  counter = counter + 10;	// wait 10 ms
		  if ((counter%1000) == 0){
			  Serial.print(counter/1000);
			  Serial.print(" ");
			  Serial.flush();
		  }
	}
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

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
#include <spark_wiring.h>
#include "application.h"
#include "liquid-crystal-spi.h"
#include "IRLib.h"

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);


// Define the pins we're going to call pinMode on
int ledD0 = D0;  // You'll need to wire an LED to this one to see it blink.
int ledBI = D7;  // This one is the built-in tiny one to the right of the USB jack

//LiquidCrystal lcd(A2);
LiquidCrystal lcd(D2, D3, D4);

const char *blank = "                ";
char tmpBuf[64];
char lcdBuf[16];

int RECV_PIN = D6;

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;
unsigned int Buffer[RAWBUF];


SYSTEM_MODE(AUTOMATIC);

/* This function is called once at start up ----------------------------------*/
void setup()
{
    // Connecting
	pinMode(ledBI, OUTPUT);
	pinMode(ledD0, OUTPUT);

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	// initialize the SPI ( Must call this before begin()! )
	lcd.initSPI();
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);	lcd.print("Boot....");
	delay(100);

	lcd.setCursor(0, 1);	lcd.print("Wait for Serial  ");
	delay(100);
	Serial.begin(9600);
	int waiting_serial_count = 0;
	while(!Serial.available() and (waiting_serial_count < 100)){ // wait for 10 seconds
   		SPARK_WLAN_Loop();

   		digitalWrite(ledBI, HIGH);
   		analogWrite(ledD0, (waiting_serial_count/25) * 60);
		delay(65);

		analogWrite(ledD0,  LOW);
		digitalWrite(ledBI, LOW);
   		delay(35);

   		waiting_serial_count += 1;
   	}
	if (!Serial.available()){
		lcd.setCursor(0, 1);	lcd.print("Serial Skipped  ");
	} else {
		lcd.setCursor(0, 1);	lcd.print("Serial Ok       ");
	}

	digitalWrite(ledBI, HIGH);
	delay(2000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("display Wifi param");
	Serial.println(WiFi.localIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.SSID());
	lcd.clear();
	lcd.setCursor(0, 0);	lcd.print(String("Wifi: ")+ WiFi.SSID());
	lcd.setCursor(0, 1);	lcd.print(WiFi.localIP());

	//Register all the Tinker functions
	lcd.setCursor(0, 1);	lcd.print("Cloud link      ");
    Serial.println("Declaring Spark functions...");
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);

	lcd.setCursor(0, 1);	lcd.print("Cloud link done ");

	digitalWrite(ledBI, HIGH);
	delay(2000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("Enabling IR");

	lcd.setCursor(0, 0);	lcd.print("Enabling IR    ");
	lcd.setCursor(0, 1);	lcd.print(blank);
	My_Receiver.enableIRIn(); // Start the receiver
	My_Decoder.UseExtnBuf(Buffer);

	lcd.setCursor(0, 0);	lcd.print("Ready           ");
	lcd.setCursor(0, 1);	lcd.print(blank);
	delay(2000);
	Serial.println("Setup done...");
}

int counter = -1;
int signals =  0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	/*
	if (My_Receiver->GetResults(&My_Decoder)) {
		signals += 1;
		digitalWrite(ledBI, HIGH);
		//Restart the receiver so it can be capturing another code
		//while we are working on decoding this one.
		My_Receiver->resume();
		My_Decoder.decode();
		My_Decoder.DumpResults();
		lcd.setCursor(0, 1);	lcd.print(signals);
		digitalWrite(ledBI, LOW);
	}
	*/
  if (My_Receiver.GetResults(&My_Decoder)) {
	//Restart the receiver so it can be capturing another code
	//while we are working on decoding this one.
	My_Receiver.resume();
	My_Decoder.decode();
	My_Decoder.DumpResults();
  }

  counter += 1;
	if (counter%(1000) == 0){
		digitalWrite(ledBI, HIGH);
		Serial.print("loop ");
		Serial.println(counter);
		lcd.setCursor(0, 1);	lcd.print(counter/500);
		delay(10);
		digitalWrite(ledBI, LOW);
		digitalWrite(ledD0, LOW);
	}

	/*
	 *
	 if (counter == 1500){
		digitalWrite(ledBI, HIGH);
		digitalWrite(ledD0, HIGH);
		Serial.println("loop Enabling IR");
		delay(500);
		My_Receiver.enableIRIn(); // Start the receiver
		Serial.println("loop Enabling Buffer");
		My_Decoder.UseExtnBuf(Buffer);
		Serial.println("loop Enabling IR Done");
		delay(500);
		digitalWrite(ledBI, LOW);
		digitalWrite(ledD0, LOW);
	}
	*/

	if ((counter % 20000) == 0){
		digitalWrite(ledBI, HIGH);
		digitalWrite(ledD0, HIGH);
		Serial.println("loop Blink");
		delay(50);
		digitalWrite(ledBI, LOW);
		digitalWrite(ledD0, LOW);
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

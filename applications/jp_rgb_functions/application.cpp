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

IPAddress remoteTAOUpIP(192, 168,   8,  21);
IPAddress localNASIP   (192, 168,  11,  17);

int localNASConnected    = 5;
int remoteTAOUpConnected = 5;

int localNASwasConnected    = 5;
int remoteTAOUpwasConnected = 5;

char connInfo[64];

#include "spark-dallas-temperature.h"

DallasTemperature dallas(new OneWire(D1));
DeviceAddress insideThermometer;

char szInfo[64];
char tmpBuf[64];

SYSTEM_MODE(AUTOMATIC);


int displayTemperature(float celsius){
	int temperatureDisplayed = 0;

	return temperatureDisplayed;
}



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

	Serial.begin(9600);
	int waiting_serial_count = 0;
	while(!Serial.available() and (waiting_serial_count < 100)){ // wait for 10 seconds
   		SPARK_WLAN_Loop();

   		digitalWrite(ledBI, HIGH);
   		analogWrite(ledD0, (waiting_serial_count * 60) / 25);
		delay(65);

		analogWrite(ledD0,  LOW);
		digitalWrite(ledBI, LOW);
   		delay(25);

   		waiting_serial_count += 1;
   	}

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	// initialize the SPI ( Must call this before begin()! )
	Serial.println("init SPI");
	lcd.initSPI();
	// set up the LCD's number of columns and rows:
	Serial.println("begin");
	lcd.begin(16, 2);
	// Print a message to the LCD.
	Serial.println("display Message");
	lcd.print("Hello, Sparky!");
	delay(1000);

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("flash blacklight");
	for (int bIndex = 0; bIndex < 5; bIndex++){
		lcd.backlight();
		delay(200);
		lcd.noBacklight();
		delay(200);
	}

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("display characters");
	for (int l = 0; l < 2; l++){
		Serial.println(String("Line: ")+ l );
		for (int c = 0; c < 16; c++){
			lcd.setCursor(c, l);
			// print the number of seconds since reset:
			Serial.print(String(c)+ " ");
			Serial.flush();
			lcd.print("H");
			delay(500);
		}
	}
	Serial.println("");

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("display Wifi param");
	Serial.println(WiFi.localIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.SSID());

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	Serial.println("Starting Temperature Sensor...");
	dallas.begin();
	Serial.println("Setting resolution to 12...");
	if (!dallas.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
	dallas.setResolution(insideThermometer, 12);
	Serial.println("Started  Sensor...");

	digitalWrite(ledBI, HIGH);
	delay(1000);
	digitalWrite(ledBI, LOW);
	delay(200);

	//Register all the Tinker functions
    Serial.println("Declaring Spark functions...");
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);

	Spark.variable("tempr", &szInfo, STRING);
	Spark.variable("conn", &connInfo, STRING);

	/*
	 *
	 Serial.println("Flashing Temperature Gradient...");
  	delay(100);
  	for (float temp=15.0; temp < 32.0; temp += 1.0){
  		Serial.print("Temperature:");
  		Serial.println(temp);
  		displayTemperature(temp);
  		delay(50);
  	}
	displayTemperature(0.0);
	*/

	Serial.println("Setup done...");
}

int counter = 0;
float current_temperature         = 0;
float current_temperature_display = 0;

void displayConnectivityStatus()
{
	if (remoteTAOUpConnected + remoteTAOUpwasConnected + localNASConnected + localNASwasConnected < 15) {
		for (int i = 0; i < 3; i++){
			digitalWrite(ledD0, HIGH);
			delay(100);
			digitalWrite(ledD0, LOW);
			delay(50);
		}
	}

	if (localNASConnected < 2 and localNASwasConnected < 2){
		Serial.println("Flashing Local");
		digitalWrite(ledD0, 120);
	} else if (remoteTAOUpConnected < 2 and remoteTAOUpwasConnected < 2){
		Serial.println("Flashing Remote");
		digitalWrite(ledD0, HIGH);
	} else {
		digitalWrite(ledD0,   0);
	}

	sprintf(tmpBuf, "Local %d/%d, Remote %d/%d", localNASConnected, localNASwasConnected, remoteTAOUpConnected, remoteTAOUpwasConnected);
	//Serial.println(tmpBuf);
	strcpy(connInfo, tmpBuf);
}

int displayS = 0;

/* This function loops forever --------------------------------------------*/
void loop()
{
	float celsius = 0.0;
	int   temperatureDisplayed = 0;

	if (counter > 10000){ // every 10 seconds
		Serial.println("");
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
			for (int i = 0; i < 2; i++){
				digitalWrite(ledD0, HIGH);
				delay(250);
				digitalWrite(ledD0, LOW);
				delay(50);
			}
		}

		digitalWrite(ledBI, LOW);
		delay(20);
		digitalWrite(ledBI, HIGH);

		Serial.print("-------------- Pinging Local --------- ");
		Serial.flush();
		localNASConnected = WiFi.ping(localNASIP);
		Serial.print(localNASConnected);
		Serial.println(" ---");

		digitalWrite(ledBI, LOW);
		delay(20);
		digitalWrite(ledBI, HIGH);
		Serial.print("-------------- Pinging Remote -------- ");
		Serial.flush();
		remoteTAOUpConnected = WiFi.ping(remoteTAOUpIP);
		Serial.print(remoteTAOUpConnected);
		Serial.println(" ---");

		displayConnectivityStatus();

		Serial.println("-------------- LCD.clear() -------- ");
		lcd.clear();

		remoteTAOUpwasConnected = remoteTAOUpConnected;
		localNASwasConnected    = localNASConnected;
	} else {
		delay(10);
		counter = counter + 10;	// wait 10 ms
		if ((counter%2500) == 0){ // every 2.5 second
			Serial.print(counter/1000);
			Serial.print("s. Ping (");
			Serial.print(localNASConnected);
			Serial.print(", ");
			Serial.print(remoteTAOUpConnected);
			Serial.println(") ");

			displayConnectivityStatus();

			displayS = 1 - displayS;

			for (int l = 0; l < 2; l++){
				Serial.print("--");
				for (int c = 0; c < 16; c++){
					lcd.setCursor(c, l);
					// print the number of seconds since reset:
					if (displayS > 0){
						lcd.print("L");
						Serial.print("L");
					} else {
						lcd.print("H");
						Serial.print("H");
					}
					Serial.flush();
					delay(200);
				}
			}
			Serial.println("");
			Serial.println("--- Done ---");
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

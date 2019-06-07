/*********************************************************************
This is an example of a solution which employs Arduino along with the 

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <Wire.h>
// two libraries inserted into the 'libraries' folder
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// dht library
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define OLED_RESET 4 
Adafruit_SSD1306 display(OLED_RESET); //Resets the OLED

//#define DHTTYPE DHT22
#define DHTTYPE    DHT11     // DHT 11 

//Checks if the display is of the correct size
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Declares DHT pin and type
DHT dht(DHTPIN, DHTTYPE);

//Set hot and cold limits
const int hot = 82.0; //set hot parameter
const int cold = 75.0; //set cold parameter

void setup()   {                
//Define pin inputs and outputs  
  pinMode(A2, INPUT); //sensor
  pinMode(3, OUTPUT); //blue
  pinMode(4, OUTPUT); //green
  pinMode(5, OUTPUT); //red
  //Begin Serial port for displaying on data in the Serial Monitor
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display(); // show splashscreen
  delay(2000); 
  display.clearDisplay();   // clears the screen and buffer
  
  dht.begin();
  delay(2000);
}

void loop() {
  //int sensor = analogRead(A2);
  //float voltage = (sensor / 1024.0) * 5.0;
  //float tempC = (voltage - .5) * 100;
  //float tempF = (tempC * 1.8) + 32;
  //Serial.print(F("temp: "));
  //Serial.print(tempF);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    //display.println(F("Inside Tmp"));
   
  float h = dht.readHumidity();
  // Read temperature as Celsius (isCelsius = true)
  float t = dht.readTemperature(true);
  // Read temperature as Fahrenheit (the default)
  float f = dht.readTemperature();

  // Check if any reads failed.
  if (isnan(h) || isnan(t) || isnan(f)) {
    delay(2000);
  } else{
    // routine for converting temp/hum floats to char arrays
    char temp_buff[5]; char hum_buff[5];
    char temp_disp_buff[11] = "Temp:";
    char hum_disp_buff[11] = "Hum:";
    
    // appending temp/hum to buffers
    dtostrf(t,2,1,temp_buff);
    strcat(temp_disp_buff,temp_buff);
    dtostrf(h,2,1,hum_buff);
    strcat(hum_disp_buff,hum_buff);

    if (dht.readTemperature(true) < cold) { //cold
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    display.println(F("Too Cold"));
    //Serial.println(F(" Too Cold."));
    Serial.println(dht.readTemperature(true));
  }
  else if (dht.readTemperature(true) >= hot) { //hot
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    display.println(F("Too Hot"));
    //Serial.println(F(" Too Hot."));
    Serial.println(f = dht.readTemperature(true));
  }
  else { //fine
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    display.println(F("Ideal"));
    //Serial.println(F(" It's Fine."));
    Serial.println(f = dht.readTemperature(true));
  }
    
    // routine for displaying text for temp/hum readout
    display.setCursor(0,17);
    display.println(temp_disp_buff);
    display.println(hum_disp_buff);
    display.display();
    delay(2000);
  }
}
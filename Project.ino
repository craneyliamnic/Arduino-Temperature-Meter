#include <OneWire.h>
#include <DallasTemperature.h>
#include <FastLED_NeoPixel.h>

#define ONE_WIRE_BUS 2
#define DATA_PIN 6
#define NUM_LEDS 60
#define BRIGHTNESS 15

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;  

unsigned long currentmillis;
unsigned long startmillis;
unsigned long value = 1000;

float old_tempC;
int checktemp;

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  float tempC = sensors.getTempCByIndex(0);
  old_tempC = tempC;
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  sensors.begin();
  strip.begin();
	strip.setBrightness(BRIGHTNESS);
  strip.clear();
  startmillis = millis();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{
  currentmillis = millis();
  if (currentmillis - startmillis >= value) {
    Serial.println(old_tempC);
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");

    float tempC = sensors.getTempCByIndex(0);
    int tempC_int = tempC;
    Serial.println(tempC);
    for( int i = 0; i<tempC_int; i++){
      strip.setPixelColor(i-1, strip.Color(0, 191, 255));
      strip.setPixelColor(i, strip.Color(0, 255, 0));
	    strip.show();
    } 
    for( int ii = tempC_int; ii<NUM_LEDS; ii++){
      strip.setPixelColor(ii, strip.Color(255, 0, 0));
	    strip.show();
    } 

    // Check if reading was successful
    if(tempC != DEVICE_DISCONNECTED_C) 
    {
      Serial.print("Temperature for the device 1 (index 0) is: ");
      Serial.println(tempC);

    } 
    else
    {
      Serial.println("Error: Could not read temperature data");
    }



    if (checktemp == 5) {
      if(old_tempC > tempC) {
      Serial.println("Temperatur ist gesunken!");
    }
    if(old_tempC < tempC) {
      Serial.println("Temperatur ist gestiegen!");
    }
    if(old_tempC == tempC) {
      Serial.println("Temperatur ist konstant!");
    }
    old_tempC = tempC;
    checktemp = 0;
    }
  
    checktemp ++;
    Serial.println(checktemp);
    startmillis = currentmillis;
  }
}
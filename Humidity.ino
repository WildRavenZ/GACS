/*DOCUMENTATION
*Title: Humidity.ino
*Author: Fernando Franco
*Version: 1.0
*Date: 03/03/2023
*Description: Arduino program to read and return humidity and temperature on the environment
*/
//Libraries:
#include "DHT.h"
//Global variables:
#define DHTPIN 2
#define DHTTYPE DHT11
int led = 13;

DHT dht(DHTPIN, DHTTYPE);
//Setup:
void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    Serial.println("START");
    dht.begin();
}
//Loop:
void loop() { 
    //Variables:
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    //Verify function:
    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000);
    return;
    }
    //Heat index:
    float hic = dht.computeHeatIndex(t, h, false);
    //Serial print of results:
    Serial.println("-----------------");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%\n");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("°C\n");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print("°C\n");
    Serial.println("-----------------");
    digitalWrite(led, HIGH);
    delay(30);
    digitalWrite(led, LOW);
    delay(5000);
}
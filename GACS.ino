/*DOCUMENTATION
  *Title: GACS.ino
  *Authors:
    - Fernando Franco
    - María F. Alvarado
    - Lauro J. Pérez
    - Carlos A. Ruiz
  *Version: 2.5
  *Initial date: 03/03/2023
  *Version date: 09/5/2023
  *Description: Arduino program to read environmental variables
  for a plant and care it (watering or light).
*/

//Libraries:
#include "DHT.h" //DHT sensors library.
#include <SD.h> //Archive communication library.
//Define DHT method:
DHT dht(10, DHT11);

/*FUNCTION
  *Name: Settings()
  *Description: Initializing IN Sensors, OUT actuators and serial console.
  *Return value: NONE.
*/
void Settings() {
    dht.begin(); //Humidity and temperature sensors.
    pinMode(A0, INPUT); //Light sensor.
    pinMode(13, OUTPUT); //Light actuator.
    pinMode(8, OUTPUT); //Water actuator.
    Serial.begin(9600); //Serial console.
    Serial.println("START"); //Show serial console.
}

/*FUNCTION
  *Name: Print_Results()
  *Description: Prints the results of the lectures.
  *Return value: NONE.
*/
void Print_Results(float h, float t, float hi, int li, int c) {
    Serial.print("----Lecture-----------------");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%\n");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("°C\n");
    Serial.print("Heat index: ");
    Serial.print(hi);
    Serial.print("°C\n");
    Serial.print("Light intensity: ");
    Serial.println(li);
    Serial.print("Times the plant was cared: ");
    Serial.println(c);
    Serial.println("----------------------------");
}

/*FUNCTION
  *Name: Automatic_Care()
  *Description: Reads the sensors and applies the automatic care algorithm, then
  it prints the results obtained.
  *Return value: NONE.
*/
void Automatic_Care(float il_temperature, int il_light_intensity) {
    //Define variables:
    float humidity; //Humidity percentage.
    float temperature; //Temperature in C°.
    float heat_index; //Heat index in C°.
    int light_intensity; //Light intensity (From MIN:0 to MAX:1024).
    //Plant automatic care algorithm:
    int cared = 0; //Tracks how many times the plant needed to be cared.
    int caring = 1; //Finishes when plant does´nt need to be cared.
    while (caring == 1) {
        caring = 0;
        //Read IN sensors:  
        humidity = dht.readHumidity(); //Read humidity.
        temperature = dht.readTemperature(); //Read temperature.
        light_intensity = analogRead(A1); //Read light intensity.
        //Verify if values are not NULL:
        if (isnan(humidity) || isnan(temperature) || isnan(light_intensity)) {
            Serial.println("Failed to read from DHT sensor! Please check sensors");
            delay(5000);
        }
        heat_index = dht.computeHeatIndex(temperature, humidity, false); //Compute heat index.
        //If the plant needs water:
        if (heat_index < il_temperature) { //Waters the plant.
            Serial.println("Plant needs water");
            delay(2500);
            Serial.println("Watering...");
            digitalWrite(8, LOW);
            delay(10000);
            digitalWrite(8, HIGH);
            caring = 1;
            cared += 1;
        }
        //If the plant needs light:
        if (light_intensity < il_light_intensity) { //Provides light to the plant.
            Serial.println("Plant needs light");
            delay(2500);
            Serial.println("Providing light...");     
            digitalWrite(13, HIGH);
            delay(10000);
            digitalWrite(13, LOW);
            caring = 1;
            cared += 1;
        }
        delay(10000); //Wait 10 seconds for getting new results.
    }
    Print_Results(humidity, temperature, heat_index, light_intensity, cared); //Print results when ended the algorithm.
}

/*FUNCTION
  *Name: setup()
  *Description: Executes the Settings() and Autmaitc_Care() functions and stab
  and applies values usign a configuration file.
  *Return value: NONE.
*/
void setup() {
  digitalWrite(8, HIGH); //Turns off the water actuator.
  Settings(); //Apply settings.
  //Opening and reading GACS_config.txt file:
  File config;
  if (!SD.begin(10)) {
    Serial.println("Couldn't start de SD");
    return;
  }
  config = SD.open("GACS_config.txt");
  float ilt; //Inferior limit of temperature.
  float ill; //Inferior limit of light intensity.
  if (config.available()) {
      ilt = config.read(); //Read the temperature limit configuration.
      ill = config.read(); //Read the light intensity limit configuration.
  }
  config.close(); //Closes the configuration file.
  Automatic_Care(ilt, ill); //Set limits for water and light sensors.
}

void loop () {
  //Nothing here.
}
//programa simple para probar un solo bme 280
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <OneWire.h>        
#include <DallasTemperature.h>
OneWire ourWire(2); //Se establece la comunicación OneWire 
                     //para los sensores DS18B20 en el pin 2
DallasTemperature sensors(&ourWire);//Se llama a la librería DallasTemperature
float t0,t1,t2;



#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");
  
  sensors.requestTemperatures(); // Prepara los sensores DS para medir temperatura
  t0=sensors.getTempCByIndex(0); //Se lee la temperatura del interior
  t1=sensors.getTempCByIndex(1); // "     "     "     "     " exterior
  
 Serial.println((String) "la temperatura 1 es: " + t0 + "°C " + "la temperatura 2 es:" + t1 + "°C ");

  delay(1000);
}

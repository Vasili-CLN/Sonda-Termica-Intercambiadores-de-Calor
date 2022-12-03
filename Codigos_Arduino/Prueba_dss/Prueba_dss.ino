//Programa simple para probar 3 sensores DS18B20

//Se importan las librerías para los sensores DS18B20
#include <OneWire.h>        
#include <DallasTemperature.h>
OneWire ourWire(2); //Se establece la comunicación OneWire 
                     //para los sensores DS18B20 en el pin 17
DallasTemperature sensors(&ourWire);//Se llama a la librería DallasTemperature
float t1,t2,t3;

void setup() {
  Serial.begin(9600);    //Inicializamos la comunicación con el puerto serial
    // put your setup code here, to run once:

}

void loop() {
  sensors.requestTemperatures(); // Prepara los sensores DS para medir temperatura
  t1=sensors.getTempCByIndex(0); //Se lee la temperatura del interior
  t2=sensors.getTempCByIndex(1); // "     "     "     "     " exterior
  t3=sensors.getTempCByIndex(2); //Se lee la temperatura del interior
  
 Serial.println((String) "t1: " + t1 + " °C ");
 Serial.println((String) "t2: " + t2 + " °C ");
 Serial.println((String) "t3: " + t3 + " °C ");
 Serial.println("");
 Serial.println("");
 Serial.println("");
delay(1000);
}

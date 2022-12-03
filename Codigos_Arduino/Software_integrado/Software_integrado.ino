//este software utilizó todos los sensores que se pensaban implementar originalmente en el prototipo
/* 
 Codigo para la elaboración de la tesis de Licenciatura de Emilio Carmona Flores 
 a continuación se presentan todos las leyendas anexas de los codigos en los cuales me
 base para el funcionamiento de los diversos sensores usados:
 
 */
 
/***************************************************************************
  Example for BME280 Weather Station using up to four Sensors with SPI
  written by Thiago Barros for BlueDot UG (haftungsbeschränkt)
  BSD License
  This sketch was written for the Bosch Sensor BME280.
  The BME280 is a MEMS device for measuring temperature, humidity and atmospheric pressure.
  For more technical information on the BME280, please go to ------> http://www.bluedot.space
 ***************************************************************************/
/*
*/
//bibliotecas para sensores bme
#include <Wire.h>
#include "BlueDot_BME280.h"
//

BlueDot_BME280 bme1;                                     //Object for Sensor 1
BlueDot_BME280 bme2;                                     //Object for Sensor 2
BlueDot_BME280 bme3;                                     //Object for Sensor 3

int bme1Detected = 0;                                    //Checks if Sensor 1 is available
int bme2Detected = 0;                                    //Checks if Sensor 2 is available
int bme3Detected = 0;                                    //Checks if Sensor 3 is available

//librerias para los sensores ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// grupo 1 de sensores para primer tubo
uint8_t sensor1[8] = { 0x28, 0xEE, 0x38, 0x19, 0x2E, 0x16, 0x01, 0x4D};
uint8_t sensor2[8] = { 0x28, 0xEE, 0xC4, 0xE5, 0x1C, 0x16, 0x01, 0x38};
uint8_t sensor3[8] = { 0x28, 0xEE, 0x43, 0xEA, 0x1C, 0x16, 0x01, 0x38};
// grupo 2 de sensores para segundo tubo
uint8_t sensor4[8] = { 0x28, 0xEE, 0xB8, 0x1B, 0x2E, 0x16, 0x01, 0x81};
uint8_t sensor5[8] = { 0x28, 0xEE, 0x91, 0x18, 0x2E, 0x16, 0x01, 0x02};
uint8_t sensor6[8] = { 0x28, 0xEE, 0x25, 0x6D, 0x2C, 0x16, 0x02, 0xD6};
// grupo 3 de sensores para un tercer tubo o una profundidad
//se comportan poco precisos e inexactos
uint8_t sensor7[8] = { 0x28, 0xEE, 0xC3, 0xE1, 0x1C, 0x16, 0x01, 0x67};
uint8_t sensor8[8] = { 0x28, 0xEE, 0xB3, 0x10, 0x1A, 0x16, 0x02, 0xC7};
uint8_t sensor9[8] = { 0x28, 0xEE, 0x8B, 0xE2, 0x2C, 0x16, 0x02, 0x04};
// grupo 4 de sensores para un tercer tubo o una profundidad 
//se comportan muy precisos bien!!!
uint8_t sensor10[8] = { 0x28, 0xEE, 0x59, 0x10, 0x1A, 0x16, 0x02, 0xA0};
uint8_t sensor11[8] = { 0x28, 0xEE, 0x87, 0xA3, 0x2C, 0x16, 0x02, 0x4A};
uint8_t sensor12[8] = { 0x28, 0xEE, 0x37, 0x7A, 0x2E, 0x16, 0x01, 0x3A};
// grupo 5 de sensores para un tercer tubo o una profundidad
//se comportan muy similares al grupo 4
uint8_t sensor13[8] = { 0x28, 0xEE, 0xAC, 0x78, 0x2C, 0x16, 0x02, 0x57};
uint8_t sensor14[8] = { 0x28, 0xEE, 0x83, 0xE1, 0x2C, 0x16, 0x02, 0xB2};
uint8_t sensor15[8] = { 0x28, 0xEE, 0xA3, 0x05, 0x1A, 0x16, 0x02, 0x02};

//Librerias y definiciones necesarias para el modulo wifi
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include <ThingsBoard.h>
//definiciones necesarias para wifi
//#define WIFI_AP "XT1064 6189"
//#define WIFI_PASSWORD "5487219636aA"
//#define WIFI_AP "Valar Morghulis"
//#define WIFI_PASSWORD "valardohaeris16"
#define WIFI_AP "ALUMNOS"
#define WIFI_PASSWORD "alumn05ier"
//#define WIFI_AP "Carmona"
//#define WIFI_PASSWORD "Carmona222"

#define TOKEN "SzzD371JYP1jeogt7l9u"
char thingsboardServer[] = "iot.ier.unam.mx";

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;



/*Variables necesarias*/
float Tbme1,Hum1,Pres1,Tds11,Tds12,Tds13,TdsProm1,DesvTds1,DifTemps1;
float Tbme2,Hum2,Pres2,Tds21,Tds22,Tds23,TdsProm2,DesvTds2,DifTemps2;
float Tbme3,Hum3,Pres3,Tds31,Tds32,Tds33,TdsProm3,DesvTds3,DifTemps3;
float TbmeP,HumP,PresP,TdsP1,TdsP2,TdsP3,TdsPromP,DesvTdsP,DifTempsP;
float v1,v2,v3,Vref=4.98;

int imploc;
void setup() {
  delay(1000);
  //Serial1 es la comunicación wifi
  Serial.println("Iniciando todo el programa");
  Serial1.begin(9600);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;

  Serial.begin(9600);
  sensors.begin(); //inicia los sensores de temperatura ds18b20
  Serial.println("Sensores iniciados");
  pinMode(A15,INPUT); // Sensor 1 de viento 
  pinMode(A1,INPUT);  // Sensor 2 de viento
  pinMode(A7,INPUT);  // Sensor 3 de viento


    /*Se configuran todos los parametros para los 3 sensores bme280 que son usados como sensores de humedad*/
    bme1.parameter.communication = 1;                    //Hardware SPI for Sensor 1 (bme1)
    bme2.parameter.communication = 1;                    //Hardware SPI for Sensor 2 (bme2)
    bme3.parameter.communication = 1;                    //Hardware SPI for Sensor 3 (bme3)
      
    bme1.parameter.SPI_cs = 7;                          //Chip Select Pin for Sensor 1
    bme2.parameter.SPI_cs = 6;                          //Chip Select Pin for Sensor 2
    bme3.parameter.SPI_cs = 5;                          //Chip Select Pin for Sensor 3

    bme1.parameter.SPI_miso = 11;                       //The MISO/SDO signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_miso = 11;                       //The MISO/SDO signal must be connected to the same pin for all sensors
    bme3.parameter.SPI_miso = 11;                       //The MISO/SDO signal must be connected to the same pin for all sensors

    bme1.parameter.SPI_sck = 12;                       //The SCK/SCL signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_sck = 12;                       //The SCK/SCL signal must be connected to the same pin for all sensors
    bme3.parameter.SPI_sck = 12;                       //The SCK/SCL signal must be connected to the same pin for all sensors
  
    bme1.parameter.SPI_mosi = 13;                       //The MOSI/SDA signal must be connected to the same pin for all sensors
    bme2.parameter.SPI_mosi = 13;                       //The MOSI/SDA signal must be connected to the same pin for all sensors
    bme3.parameter.SPI_mosi = 13;                       //The MOSI/SDA signal must be connected to the same pin for all sensors
  
    //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
   
  //Now choose on which mode your device will run
  //On doubt, just leave on normal mode, that's the default value

  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  
    bme1.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 1
    bme2.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 2
    bme3.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 3
 
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Great! Now set up the internal IIR Filter
  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default

  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)

    bme1.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 1
    bme2.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 2
    bme3.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 3
    
   //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero
  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 1
    bme2.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 2
    bme3.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 3

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements
  
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 1
    bme2.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 2
    bme3.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 3
    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default
  
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  

    bme1.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 1
    bme2.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 2
    bme3.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 3
       
  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);
  
    bme1.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)
    bme2.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 2)
    bme3.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 3)

  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C);
  
    bme1.parameter.tempOutsideCelsius = 15;               //default value of 15°C
    bme2.parameter.tempOutsideCelsius = 15;               //default value of 15°C
    bme3.parameter.tempOutsideCelsius = 15;               //default value of 15°C
    
    bme1.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F
    bme2.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F
    bme3.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F

  
  //*********************************************************************
  //*************ADVANCED SETUP IS OVER - LET'S CHECK THE CHIP ID!*******
  
  // Initialize Sensor 1
  if (bme1.init() != 0x60)
  {    
    Serial.println(F("Ops! First BME280 Sensor not found!"));
    bme1Detected = 0;
  }
  else
  {
    Serial.println(F("First BME280 Sensor detected!"));
    bme1Detected = 1;
  }

  
  // Initialize Sensor 2
  if (bme2.init() != 0x60)
  {    
    Serial.println(F("Ops! Second BME280 Sensor not found!"));
    bme2Detected = 0;
  }
  else
  {
    Serial.println(F("Second BME280 Sensor detected!"));
    bme2Detected = 1;
  }

  
  // Initialize Sensor 3
  if (bme3.init() != 0x60)
  {    
    Serial.println(F("Ops! Third BME280 Sensor not found!"));
    bme3Detected = 0;
  }
  else
  {
    Serial.println(F("Third BME280 Sensor detected!"));
    bme3Detected = 1;
  }
   
  Serial.println();
  Serial.println();

}

void loop() {
   //revisión del modulo wifi
   status = WiFi.status();
  if ( status != WL_CONNECTED) {
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("Connected to AP");
  }

  if ( !client.connected() ) {
    reconnect();
  }

//Lectura de todos los sensores:

  imploc=1; // 0 para no imprimir por el serial de la pc, 1 para imprimir. Esto es para hacer depuración del codigo en caso de necesitarse.
  humedad_1(imploc);
  humedad_2(imploc);
  humedad_3(imploc);
  Serial.println();
  Serial.println();
  v1=analogRead(A0);
  v2=analogRead(A1);
  v3=analogRead(A2);
  /*
  Serial.println((String) "v1: "+ v1 + " cuentas" );
  Serial.println((String) "v2: "+ v2 + " cuentas" ); 
  Serial.println((String) "v3: "+ v3 + " cuentas" );
  */
  /*
  float Tbme1,Hum1,Pres1,Tds11,Tds12,Tds13,TdsProm1,DesvTds1,DifTemps1;
  float v1,v2,v3,Vref=4.98;
  */
  temperatura_1();
  temperatura_2();
  temperatura_3();
  //temperatura_prueba();


  //Se envian todas las variables medidas con su respectiva etiqueta al servidor Thingsboard!
  
  tb.sendTelemetryFloat("T1_tub_1", TdsProm1);
  tb.sendTelemetryFloat("DesT1_tub_1", DesvTds1);
  tb.sendTelemetryFloat("T2_tub_1", Tbme1);  
  tb.sendTelemetryFloat("Hum_tub_1", Hum1);
  tb.sendTelemetryFloat("Pres_tub_1", Pres1);
  tb.sendTelemetryFloat("V_tub_1", v1);
  
  tb.sendTelemetryFloat("T1_tub_2", TdsProm2);
  tb.sendTelemetryFloat("DesT2_tub_2", DesvTds2);
  tb.sendTelemetryFloat("T2_tub_2", Tbme2);
  tb.sendTelemetryFloat("Hum_tub_2", Hum2);
  tb.sendTelemetryFloat("Pres_tub_2", Pres2);
  tb.sendTelemetryFloat("V_tub_2", v2);
    
  tb.sendTelemetryFloat("T1_tub_3", TdsProm3);
  tb.sendTelemetryFloat("DesT1_tub_3", DesvTds3);
  tb.sendTelemetryFloat("T2_tub_3", Tbme3);
  tb.sendTelemetryFloat("Hum_tub_3", Hum3);
  tb.sendTelemetryFloat("Pres_tub_3", Pres3);
  tb.sendTelemetryFloat("V_tub_3", v3);
  Serial.println("Se enviaron todos los datos...");
  delay(3000);
  
} //Aqui termina el ciclo infinito del arduino



void humedad_1(int x)
{
  if (bme1Detected)
  {
   Tbme1=bme1.readTempC();
   Hum1=bme1.readHumidity(); 
   Pres1=bme1.readPressure();
   if (x==1)
   {
    Serial.println((String) "T1: "+ Tbme1 + " °C Hum1: " + Hum1 + "% P1: " + Pres1 + "hPa"  );
   }
  }
  else
  {
    Serial.println("El sensor 1 esta desconectado, o tiene un problema de conexión"); 
  }
}

void humedad_2(int x)
{
  if (bme2Detected)
  {
   Tbme2=bme2.readTempC();
   Hum2=bme2.readHumidity(); 
   Pres2=bme2.readPressure();
   if (x==1)
   {
   Serial.println((String) "T2: "+ Tbme2 + " °C Hum2: " + Hum2 + "% P2: " + Pres2 + "hPa"  );
   }
  }
  
  else
  {
    Serial.println("El sensor 2 esta desconectado, o tiene un problema de conexión"); 
  }
}

void humedad_3(int x)
{
  if (bme3Detected)
  {
   Tbme3=bme3.readTempC();
   Hum3=bme3.readHumidity(); 
   Pres3=bme3.readPressure();
   if (x==1)
   {
   Serial.println((String) "T3: "+ Tbme3 + " °C Hum3: " + Hum3 + "% P3: " + Pres3 + "hPa"  );
   }
  }
  else
  {
    Serial.println("El sensor 3 esta desconectado, o tiene un problema de conexión"); 
  }
}

void InitWiFi()
{
  // initialize serial for ESP module
//  soft.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("99f4cc00-4587-11ea-9ffe-35550336f914", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}


void temperatura_1()
{
 sensors.requestTemperatures();
 Tds11=sensors.getTempC(sensor4);
 Tds12=sensors.getTempC(sensor5);
 Tds13=sensors.getTempC(sensor6);
 TdsProm1=(Tds11+Tds12+Tds13)/3;
 DesvTds1=(pow((Tds11-TdsProm1),2)+pow((Tds12-TdsProm1),2)+pow((Tds13-TdsProm1),2))/3;

 Serial.println((String) "T1=" + Tds11 + "°C");
 Serial.println((String) "T2=" + Tds12 + "°C");
 Serial.println((String) "T3=" + Tds13 + "°C");
 Serial.println((String) "TP1=" + TdsProm1 + "°C");
 Serial.println((String) "Std Deviation1=+- " + DesvTds1 + "°C");

}

//float Tbme1,Hum1,Pres1,Tds11,Tds12,Tds13,TdsProm1,DesvTds1,DifTemps1;

void temperatura_2()
{
 sensors.requestTemperatures();
 Tds21=sensors.getTempC(sensor7);
 Tds22=sensors.getTempC(sensor8);
 Tds23=sensors.getTempC(sensor9);
 
 TdsProm2=(Tds21+Tds22+Tds23)/3;
 DesvTds2=(pow((Tds21-TdsProm2),2)+pow((Tds22-TdsProm2),2)+pow((Tds23-TdsProm2),2))/3;
 
 Serial.println((String) "T4=" + Tds21 + "°C");
 Serial.println((String) "T5=" + Tds22 + "°C");
 Serial.println((String) "T6=" + Tds23 + "°C");
 Serial.println((String) "TP2=" + TdsProm2 + "°C");
 Serial.println((String) "Std Deviation2=+- " + DesvTds2 + "°C");

 
}


void temperatura_3()
{
 sensors.requestTemperatures();
 Tds31=sensors.getTempC(sensor10);
 Tds32=sensors.getTempC(sensor11);
 Tds33=sensors.getTempC(sensor12);

 TdsProm3=(Tds31+Tds32+Tds33)/3;
 DesvTds3=(pow((Tds31-TdsProm3),2)+pow((Tds32-TdsProm3),2)+pow((Tds33-TdsProm3),2))/3;

 Serial.println((String) "T7=" + Tds31 + "°C");
 Serial.println((String) "T8=" + Tds32 + "°C");
 Serial.println((String) "T9=" + Tds33 + "°C");
 Serial.println((String) "TP3=" + TdsProm3 + "°C");
 Serial.println((String) "Std Deviation3=+- " + DesvTds3 + "°C");
 
}


void temperatura_prueba()
{
 sensors.requestTemperatures();
 TdsP1=sensors.getTempC(sensor13);
 TdsP2=sensors.getTempC(sensor14);
 TdsP3=sensors.getTempC(sensor15);

 TdsPromP=(TdsP1+TdsP2+TdsP3)/3;
 DesvTdsP=(pow((TdsP1-TdsPromP),2)+pow((TdsP2-TdsPromP),2)+pow((TdsP3-TdsPromP),2))/3;

 Serial.println((String) "T10=" + TdsP1 + "°C");
 Serial.println((String) "T11=" + TdsP2 + "°C");
 Serial.println((String) "T12=" + TdsP3 + "°C");
 Serial.println((String) "TP3=" + TdsPromP + "°C");
 Serial.println((String) "Std Deviation3=+- " + DesvTdsP + "°C");
 
}

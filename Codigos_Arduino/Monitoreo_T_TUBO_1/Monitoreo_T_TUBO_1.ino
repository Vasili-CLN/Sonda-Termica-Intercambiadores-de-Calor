
/* 
 Codigo para la elaboración de la tesis de Licenciatura de Emilio Carmona Flores 
 a continuación se presentan todos las leyendas anexas de los codigos en los cuales me
 base para el funcionamiento de los diversos sensores usados:

*/

/*Este codigo en particular se pensaba utilizar para monitorear una tuberia en particular de un intercambiador de calor*/
//librerias para los sensores ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

uint8_t sensor1[8] = { 0x28, 0xEE, 0x59, 0x10, 0x1A, 0x16, 0x02, 0xA0};
uint8_t sensor2[8] = { 0x28, 0xEE, 0x83, 0xE1, 0x2C, 0x16, 0x02, 0xB2};
uint8_t sensor3[8] = { 0x28, 0xEE, 0xAC, 0x78, 0x2C, 0x16, 0x02, 0x57};
uint8_t sensor4[8] = { 0x28, 0xEE, 0xA3, 0x05, 0x1A, 0x16, 0x02, 0x02};
uint8_t sensor5[8] = { 0x28, 0xEE, 0xB3, 0x10, 0x1A, 0x16, 0x02, 0xC7};
uint8_t sensor6[8] = { 0x28, 0xEE, 0xC3, 0xE1, 0x1C, 0x16, 0x01, 0x67};
uint8_t sensor7[8] = { 0x28, 0xEE, 0x8B, 0xE2, 0x2C, 0x16, 0x02, 0x04};
//Librerias y definiciones necesarias para el modulo wifi
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include <ThingsBoard.h>

//definiciones necesarias para wifi
#define WIFI_AP "tu red wifi"
#define WIFI_PASSWORD "tu contraseña"

#define TOKEN "TU TOKEN" 
char thingsboardServer[] = "TU SERVIDOR THINGSBOARD";

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;



/*Variables necesarias*/
float T,T1,T2,T3,T4,T5,T6,T7,T8,T9;
int imploc;
int inicio;
void setup() {
  Serial.begin(9600);
  //Serial1 es la comunicación wifi
  Serial.println("Iniciando todo el programa");
  Serial1.begin(9600);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
  //Inicia la comunicación Serial
  
  sensors.begin(); //inicia los sensores de temperatura ds18b20
  Serial.println("Sensores iniciados");
  inicio=0;
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
  
  if (inicio==0)
  {
  tb.sendTelemetryFloat("T1", 100);
  tb.sendTelemetryFloat("T2", 100);
  tb.sendTelemetryFloat("T3", 100);
  tb.sendTelemetryFloat("T4", 100);
  tb.sendTelemetryFloat("T5", 100);
  tb.sendTelemetryFloat("T6", 100);
  tb.sendTelemetryFloat("T7", 100);
  Serial.println("Se envio la señal para iniciar");
  }
  if(inicio==1)
  {
  temperatura_1();  
  tb.sendTelemetryFloat("T1", T1);
  tb.sendTelemetryFloat("T2", T2);
  tb.sendTelemetryFloat("T3", T3);
  tb.sendTelemetryFloat("T4", T4);
  tb.sendTelemetryFloat("T5", T5);
  tb.sendTelemetryFloat("T6", T6);
  tb.sendTelemetryFloat("T7", T7);
  Serial.println("Se enviaron todos los datos...");
  }
  inicio=1;
  delay(60000);
  
} //Aqui termina el ciclo infinito del arduino




void InitWiFi()
{
  // initialize serial for ESP module
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
    if ( client.connect("tu device ID", TOKEN, NULL) ) {
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
  T1=sensors.getTempC(sensor1);
  T2=sensors.getTempC(sensor2);
  T3=sensors.getTempC(sensor3);
  T4=sensors.getTempC(sensor4);
  T5=sensors.getTempC(sensor5);
  T6=sensors.getTempC(sensor6);
  T7=sensors.getTempC(sensor7);
 Serial.println((String) "T1=" + T1 + "°C");
 Serial.println((String) "T2=" + T2 + "°C");
 Serial.println((String) "T3=" + T3 + "°C");
 Serial.println((String) "T4=" + T4 + "°C");
 Serial.println((String) "T5=" + T5 + "°C");
 Serial.println((String) "T6=" + T6 + "°C");
 Serial.println((String) "T7=" + T7 + "°C");

}

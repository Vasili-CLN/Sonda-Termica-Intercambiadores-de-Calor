//Librerias para modulo wifi
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
//definiciones necesarias para wifi

#define WIFI_AP "Tu red"
#define WIFI_PASSWORD "tu contraseña"

#define TOKEN "tu token"
char thingsboardServer[] = "tu servidor thingsboard";

// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;

uint8_t TC0_CS  = 7;
uint8_t TC1_CS  = 8;
uint8_t TC2_CS  = 9;
uint8_t TC3_CS  = 10;

uint8_t TC0_FAULT = 2;                     // not used in this example, but needed for config setup
uint8_t TC0_DRDY  = 2;                     // not used in this example, but needed for config setup
void setup()
{
  delay(1000);                            // give chip a chance to stabilize
  Serial.begin(9600);                   // set baudrate of serial port
  //Serial1 es la comunicación wifi
  Serial1.begin(9600);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
  
}

void loop()
{
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
  
  double tmp,t0,t1,t2,t3;
  double v0,v1,v2,v3;
  struct var_max31856 *tc_ptr;
  
    // Mensaje que envia las temperaturas al puerto serial 
  Serial.print( "Sending temperature from TC : [" );
  Serial.print((String) "t0:"+ t0 + "...t3:"+ t3 + "..."); Serial.print( "," );
  Serial.println( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"P_CH_0\":"; payload +=String(t0); payload += ",";
  payload += "\"P_CH_1\":"; payload +=String(t1); payload += ",";
  payload += "\"P_CH_2\":"; payload +=String(t2); payload += ",";
  payload += "\"P_CH_3\":"; payload +=String(t3);//
  payload += "}";

  // Send payload
  char attributes[500];
  payload.toCharArray( attributes, 500 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );

   client.loop();
   // 500ms delay... can be as fast as ~100ms in continuous mode, 1 samp avg
   delay(1000);
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
    if ( client.connect("tu device id", TOKEN, NULL) ) {
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

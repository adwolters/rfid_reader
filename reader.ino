#include <Wiegand.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* mqtt_server = "IP_MQTT_SERVER";
const char* topic = "wiegand";    // rhis is the [root topic]

String code = "";

WIEGAND wg;
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  setupWifi();
  setupReader();
  setupMqtt();
}

void loop() {
  readCode();
}

/**
 * Reads the rfidReader. When the # is pressed the code will be send over MQTT
 */
void readCode() {
  if(wg.available())
  {
    if (wg.getCode() == 13) {
      sendCode();      
    }
    else {
      code += wg.getCode();
    }
  } 
}

/**
 * sent code over mqtt
 */
void sendCode() {
  // confirm still connected to mqtt server
  if (!client.connected()) {
    reconnect();
  }

  String payload = "{\"code\":\"";
  payload += code;
  payload += "\"}";

  String pubTopic;
  pubTopic += topic ;
  pubTopic += "/code/out";

  client.publish( (char*) pubTopic.c_str() , (char*) payload.c_str(), true );
  
  code = "";
}

/**
 * Setup the wifi connection to the given ssid with given password. 
 */
void setupWifi() {
  delay(10);
  // Connect WiFi
  WiFi.hostname("WiegandModule");
  WiFi.begin(ssid, password);
}

/**  
 * Initiliaze rfid reader
 * Wemos D2 and D3 are GPIO4 and GPIO0
 */
void setupReader() {
  wg.begin(4, 0);

}

/**
 * Setup MQTT connection
 */
void setupMqtt() {
  client.setServer(mqtt_server, 1883);
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

String composeClientID() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String clientId;
  clientId += "esp-";
  clientId += macToStr(mac);
  return clientId;
}

/**
 * 
 */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    String clientId = composeClientID() ;
    clientId += "-";
    clientId += String(micros() & 0xff, 16); // to randomise. sort of

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      // Once connected, publish an announcement...
      client.publish(topic, ("connected " + composeClientID()).c_str() , true );
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

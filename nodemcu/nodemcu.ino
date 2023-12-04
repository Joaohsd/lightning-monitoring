#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define START 0x1A

// Wifi
const char* ssid = "WLL-Inatel";        //Your Network SSID
const char* password = "inatelsemfio";  //Your Network Password

// Mqtt
const char *ID = "656a82102936bb00103b65fa";        // Name of our device, must be unique
const char* BROKER_MQTT = "mqtt.tago.io";           // MQTT Cloud Broker URL
unsigned int PORT = 1883;                           // Default port for mqtt
const char *TOKEN ="badafeb5-0702-4ee4-bf0d-df6414a3c809";
const char *USER = "inatelc115";                    // User for subscribe and publish

WiFiClient client;
PubSubClient MQTTclient(client);

uint16_t _current;
uint8_t _percentage;

// Serial Communication
SoftwareSerial mySerial (D1,D2);
uint8_t buffer[4];
size_t length = 4;

// Insert here topics that the device will publish to broker
const char *TopicsToPublish[]                 = { 
                                                "inatel/c115/data",
                                                "info"
                                              };

/*+--------------------------------------------------------------------------------------+
 *| Reconnect to MQTT client                                                             |
 *+--------------------------------------------------------------------------------------+ */
 
void MQTTconnect() {

  if(!MQTTclient.connected()) {                               // Check if MQTT client is connected
  
    Serial.println();
    Serial.println("MQTT Client   : [ not connected ]");

    MQTTclient.setServer(BROKER_MQTT, PORT);                    // MQTT broker info
    MQTTclient.setBufferSize(1024);
                      
    
    Serial.println("MQTT Client   : [ trying connection ]");
    
    if (MQTTclient.connect(ID,USER,TOKEN)) {
      Serial.println("MQTT Client   : [ broker connected ]");

      for(int i=0; i<=((sizeof(TopicsToPublish) / sizeof(TopicsToPublish[0]))-1); i++){

        Serial.print("MQTT Client   : [ publishing to ");
        Serial.print(TopicsToPublish[i]);
        Serial.println(" ]");
        
      }
    } else {
      Serial.print("MQTT Client   : [ failed, rc= ");
      Serial.print(MQTTclient.state());
      Serial.println(" ]");
      delay(1000);
    }
  }
}


/*+--------------------------------------------------------------------------------------+
 *| Serialize JSON and publish MQTT                                                      |
 *+--------------------------------------------------------------------------------------+ */

void SerializeAndPublish() {

  if (!MQTTclient.connected())                            // Reconnect if connection to MQTT is lost 
  {    MQTTconnect();      }

  char buffer[1024];                                      // JSON serialization 
   
  StaticJsonDocument<512> doc;

  JsonObject doc_0 = doc.createNestedObject();
  doc_0["variable"] = "current";
  doc_0["value"] = _current;
  doc_0["unit"] = "uA";

  JsonObject doc_1 = doc.createNestedObject();
  doc_1["variable"] = "percentage";
  doc_1["value"] = _percentage;
  doc_1["unit"] = "%";

  serializeJson(doc, buffer);

  Serial.printf("\nJSON Payload:");
    Serial.printf("\n");
  
  serializeJsonPretty(doc, Serial);                 // Print JSON payload on Serial port        
    Serial.printf("\n");
    Serial.println("MQTT Client   : [ Sending message to MQTT topic ]"); 
    Serial.println("");         

  MQTTclient.publish(TopicsToPublish[0], buffer);    // Publish data to MQTT Broker 
       
}

void setup()

{

  Serial.begin(9600);
  mySerial.begin(115200);
  
  delay(10);

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // MQTT Setup
  MQTTconnect();        // Connect to MQTT Broker

}

void loop()
{
  MQTTclient.loop();        // Needs to be in the loop to keep client connection alive

  while(mySerial.available() < length){}
  
  for(uint8_t it = 0; it < length; it++){
    buffer[it] = mySerial.read();
  }
  Serial.println("Received packet...");
  if(buffer[0] == START){
    _current = (buffer[1] << 8) | (buffer[2] & 0xFF);
    _percentage = buffer[3] & 0xFF;
    Serial.print("Current: ");
    Serial.print((int)_current);
    Serial.println(" uA");
    Serial.print("Percentage: ");
    Serial.print((int)_percentage);
    Serial.println(" %");
    SerializeAndPublish();
  }
  mySerial.flush();
}

#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ThingSpeak.h>

#include <SoftwareSerial.h>

#define START 0x1A

const char* ssid = "WLL-Inatel"; //Your Network SSID

const char* password = "inatelsemfio"; //Your Network Password

int i = 0;

bool flag = false;

WiFiClient client;

SoftwareSerial mySerial (D1,D2);

unsigned long myChannelNumber = 2364744; //Your Channel Number (Without Brackets)

const char * myWriteAPIKey = "B8E42ZCWUJAINXD6"; //Your Write API Key

uint8_t buffer[3];

size_t length = 3;

void setup()

{

  Serial.begin(9600);
  mySerial.begin(115200);
  
  delay(10);

  // Connect to WiFi network

  WiFi.begin(ssid, password);

  ThingSpeak.begin(client);

}

void loop()
{
  while(mySerial.available() < length){}
  
  for(uint8_t it = 0; it < length; it++){
    buffer[it] = mySerial.read();
  }
  Serial.println("Received packet...");
  if(buffer[0] == START){
    uint16_t current = (buffer[1] << 8) | (buffer[2] & 0xFF);
    Serial.print("Current: ");
    Serial.print((int)current);
    Serial.println(" uA");
    ThingSpeak.writeField(myChannelNumber, 1, current , myWriteAPIKey); //Update in ThingSpeak
  }
  mySerial.flush();
}

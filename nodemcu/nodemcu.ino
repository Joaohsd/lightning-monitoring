#include <ESP8266WiFi.h>;

#include <WiFiClient.h>;

#include <ThingSpeak.h>;

#include <SoftwareSerial.h>

#define START 0x1A

const char* ssid = "JOAO"; //Your Network SSID

const char* password = "11131429jj"; //Your Network Password

int i = 0;

bool flag = false;

WiFiClient client;

SoftwareSerial mySerial (D1,D2);

unsigned long myChannelNumber = 1117777; //Your Channel Number (Without Brackets)

const char * myWriteAPIKey = "N2CG4QO6PH53LJL9"; //Your Write API Key

uint8_t buffer[3];

void setup()

{

  Serial.begin(115200);
  mySerial.begin(9600);
  
  delay(10);

  // Connect to WiFi network

  WiFi.begin(ssid, password);

  ThingSpeak.begin(client);

}

void loop()
{
  while(mySerial.available() < 3){}
  
  for(uint8_t it = 0; it < 3; it++){
    buffer[it] = mySerial.read();
  }

  if(buffer[0] == START){
    uint16_t current = (buffer[1] << 8) | (buffer[2] & 0xFF);
    Serial.print(current);
    Serial.println(" uA");
    ThingSpeak.writeField(myChannelNumber, 1, current , myWriteAPIKey); //Update in ThingSpeak
  }
}

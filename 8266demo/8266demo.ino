#include "Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial espSerial(10,11);

String IO_USERNAME = ""; //paste username here
String IO_KEY = ""; //paste key here
String WIFI_SSID = "UD Devices"; //Only need to change if using other network, eduroam won't work
String WIFI_PASS = ""; //Blank for open network

int num = 1; //Counts up to show upload working

boolean debug = true; //Whether or not to print responses from the ESP in the serial monitor

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  Serial.println("setting up");
  
  String resp = espData("get_macaddr",2000,debug);
  
  resp = espData("wifi_ssid="+WIFI_SSID,2000,debug);
  
  resp = espData("wifi_pass="+WIFI_PASS,2000,debug);
  
  resp = espData("io_user="+IO_USERNAME,2000,debug);
  
  resp = espData("io_key="+IO_KEY,2000,debug);
  
  resp = espData("setup_io",15000,debug);
  
  if(resp.indexOf("connected") < 0){
    Serial.println("\nAdafruit IO Connection Failed");
    while(1);
  }
  
  resp = espData("setup_feed=1,test feed",2000,debug);
  Serial.println("------ Setup Complete ----------");
}

void loop() {
  // Wait a few seconds between uploads
  // Adafruit IO only allows 30 uploads/minute, it discards everything else
  delay(5000);

  Serial.print("Num is: ");
  Serial.println(num);

  String resp = espData("send_data=1,"+String(num),2000,debug);

  num++;
}

String espData(String command, const int timeout, boolean debug)
{ 
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
      //Serial.print(c);
    }
  }
  if (debug)
  {
    Serial.println("Resp: "+response);
  }
  response.trim();
  return response;
}

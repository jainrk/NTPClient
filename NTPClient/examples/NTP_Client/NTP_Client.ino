/*  
	March 20 2017 Raj Jain
	The NTPClient class is created to have one class do the work: 
	- to connect an ESP8266 to a NIST Time server
	- obtain the time over UDP
	- do the necessary computation to construct the time in a 00:00:00 format string	
	It removes the spaghetti of code from you main sketch and encapsulates it into reusable class. 
	Parts of this code have come from various sources floating around in various resources
	This project is licensed under the terms of the MIT license.
*/

#include <WiFiConnect.h> // download - https://github.com/jainrk/WiFiConnect
#include <NTPClient.h>

#define aSSID  "yourRouterSSID"
#define aPASSWORD "yourRouterPASSWORD"

WiFiConnect* wifiCon = WiFiConnect::Instance(aSSID, aPASSWORD);
/*
   200=time NTP server is given to respond. If the clock acts goofy
   or too many successive "Packet Lost" errors, then increase it by
   intervals of 100
*/
NTPClient* ntpClient = NTPClient::Instance(200);

void setup() {
  Serial.begin(115200);
  wifiCon->Connect();
}

void loop() {
  static String localUNIXTimeString;
  unsigned long localUNIXTime = ntpClient->getTime(localUNIXTimeString, -5);
  if (localUNIXTime) {
    Serial.println (localUNIXTime, DEC);
    Serial.println (localUNIXTimeString);
  } else Serial.println ("Packet lost");
  delay(800);
}
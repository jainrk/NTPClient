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

#include "NTPClient.h"
#include <WiFiConnect.h>

NTPClient* NTPClient::_instance = 0;
const char* ntpServerName = "time.nist.gov";

NTPClient::NTPClient() {
  UDPPort = 2390;
  NTPPort = 123;
  seventyYears = 2208988800UL;
  timeAcquired = false;
  WiFi.hostByName(ntpServerName, timeServerIP);
  Udp.begin(UDPPort);
}

NTPClient* NTPClient::Instance(unsigned int udpDelay = 200) {
  if (_instance == 0)
    _instance = new NTPClient;
	_instance->UDPDelay = udpDelay;
  return _instance;
}

unsigned long NTPClient::sendNTPPacket() { 
  memset(NTPPacket, 0, NTP_PACKET_SIZE); // set all bytes in the buffer to 0
  NTPPacket[0] = 0b11100011;   // LI, Version, Mode
  NTPPacket[1] = 0;     // Stratum, or type of clock
  NTPPacket[2] = 6;     // Polling Interval
  NTPPacket[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  NTPPacket[12]  = 49;
  NTPPacket[13]  = 0x4E;
  NTPPacket[14]  = 49;
  NTPPacket[15]  = 52;  
  
  Udp.beginPacket(timeServerIP, NTPPort);
  Udp.write(NTPPacket, NTP_PACKET_SIZE);
  Udp.endPacket();
}

unsigned long NTPClient::getUNIXTime() { //UNIX Time = SecondsSince1970;
  sendNTPPacket(); // send an NTP packet to a time server
  delay(UDPDelay);

  if (Udp.parsePacket()) {
    Udp.read(NTPPacket, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(NTPPacket[40], NTPPacket[41]);
    unsigned long lowWord = word(NTPPacket[42], NTPPacket[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    return (unsigned long)(secsSince1900 - seventyYears);
  } else {
    return 0;
  }
}

String NTPClient::getUTCString(unsigned long UNIXTime) {
  String timeString = "";
  timeString += String((UNIXTime  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  timeString += String(':');
  if (((UNIXTime % 3600) / 60) < 10) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    timeString += String('0');
  }
  timeString += String((UNIXTime  % 3600) / 60); // print the minute (3600 equals secs per minute)
  timeString += String(':');
  if ((UNIXTime % 60) < 10) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    timeString += String('0');
  }
  timeString += String(UNIXTime % 60); // print the second
  return timeString;
}

unsigned long NTPClient::getTime(String& timeString, float timezone = 0) {
  unsigned long UNIXTime = 0;
  unsigned long adjustedTime = 0;
  //String UTCTimeString = String("");
  for (int i = 0; i < 5; i++) {
    UNIXTime = getUNIXTime();    
    if (UNIXTime) {
	  adjustedTime = UNIXTime + (long)(3600 * timezone);
      timeString = getUTCString(adjustedTime);//UNIXTime + (long)(3600 * timezone));
      return adjustedTime;
    } else {
      Serial.print(".");
    }
  }
  return 0;
}

String NTPClient::convertUNIXTimeToString(unsigned long UNIXTime){
	return getUTCString(UNIXTime);
}

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


#ifndef __NTPCLIENT__
#define __NTPCLIENT__ 1

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define NTP_PACKET_SIZE 48

class NTPClient {
  protected:
    NTPClient();
  private:
    unsigned long sendNTPPacket();
    String getUTCString(unsigned long UNIXTime);
    unsigned long getUNIXTime();
    unsigned long seventyYears;
    bool timeAcquired;
    unsigned int NTPPort;    //NTP requests are to port 123
	unsigned int UDPDelay; 
	IPAddress timeServerIP; // time.nist.gov NTP server
    byte NTPPacket[NTP_PACKET_SIZE];
    static NTPClient* _instance;
  public:
    static NTPClient* Instance(unsigned int udpDelay);
    unsigned long getTime(String& timeString, float timezone);
	String convertUNIXTimeToString(unsigned long UNIXTime);
    WiFiUDP Udp;
    unsigned int UDPPort;    // Local port to listen for UDP packets
};

#endif //__NTPCLIENT__

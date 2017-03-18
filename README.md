# NTPClient
	
	The NTPClient class is created to have one class do the work: 
	- to connect an ESP8266 to a NIST Time server
	- obtain the time over UDP
	- do the necessary computation to construct the time in a 00:00:00 format string	
	It removes the spaghetti of code from you main sketch and encapsulates it into reusable class. 
	Parts of this code have come from various sources floating around in various resources
	This project is licensed under the terms of the MIT license.

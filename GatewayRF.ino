// References:
// https://1technophile.blogspot.com/2016/08/low-cost-low-power-6ua-garden-433mhz.html
// https://github.com/esp8266/Arduino/blob/master/doc/reference.rst
// https://arduino.stackexchange.com/questions/44531/arduino-esp8266-direct-fast-control-of-the-digital-pins
// https://www.instructables.com/id/Using-an-ESP8266-to-Control-Mains-Sockets-Using-43/
// http://nerdralph.blogspot.com/2015/04/a-4mbps-shiftout-for-esp8266arduino.html

// Note(1): Watt OK. max pinout watt of Nodemcu 1.0 10mW, Cheap transmitter 433mhz 10 10mW
// Note(2): digitalwrite() Esp8286 function runs to 160Khz (6,25 μs): it is enough for this program
// TODO: objective: integrate in ESPEASY. See _P112_RFTX.ino plugin




#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library


#include <EX_RCSwitch.h> // UTILIZZA LA LIBRERIA https://github.com/perivar/rc-switch
			 // https://github.com/perivar/ST_Anything_NodeMCU
			 // https://community.smartthings.com/t/st-anything-433mhz-rf-devices-arduino-thingshield/19084/36
			 //https://github.com/sui77/rc-switch/pull/170 ---transmittimingsarray---

#define SERVER_PORT 80
const int pulse = 360; //μs
#define UP6_SIZE 67
byte up6[] = {1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
#define pin 4  //GPIO4 = 2
#define NUM_ATTEMPTS 3
#define TRACE 1  // 0= trace off 1 = trace on Do we want to see trace for debugging purposes



unsigned int data[]   = { 0};


void trc(String msg);              // function prototypes 
void transmit_code(char code[]);





ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void setup(void){
  pinMode(pin,OUTPUT);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("MTT_2.4", "999999999");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/", HTTP_GET, []() { transmit_code(up6); 
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> ");  
                                });
  server.onNotFound([]() { server.send(404, "text/plain", "404: Not Found"); });
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
	
	
mySwitch.enableTransmit(pin);
mySwitch.setPulseLength(pulse);
mySwitch.setRepeatTransmit(3);
	
	
  
}


void loop(void){
  server.handleClient();  // Listen for HTTP requests from clients 	
}


// trace function
void trc(String msg){if (TRACE) { Serial.println(msg); } }

void transmit_code(byte code[]){
  mySwitch.send(data);
}
 
 trc("transmit preamble");
}
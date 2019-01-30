// References:
// https://1technophile.blogspot.com/2016/08/low-cost-low-power-6ua-garden-433mhz.html
// https://github.com/esp8266/Arduino/blob/master/doc/reference.rst
// https://arduino.stackexchange.com/questions/44531/arduino-esp8266-direct-fast-control-of-the-digital-pins
// https://www.instructables.com/id/Using-an-ESP8266-to-Control-Mains-Sockets-Using-43/
// http://nerdralph.blogspot.com/2015/04/a-4mbps-shiftout-for-esp8266arduino.html

// Note(1): Watt OK. max pinout watt of Nodemcu 1.0 10mW, Cheap transmitter 433mhz 10 10mW
// Note(2): The segnal of blinks is not rcswitch compliant, so I have write a program without this library
// Note(3): digitalwrite() Esp8286 function runs to 160Khz (6,25 μs): it is enough for this program


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

#define SERVER_PORT 80
const int pulse = 360; //μs

//# ------- gate ---------
const int short_gate = 760; //μs
const int long_delay = 1520;
const int extended_delay = 0.5;
canc = '01111010010000';
//# ----------------------

up0 = '110011000000100100000000000000000001100101010001101000100000000000';
st0 = '110011000000011000000000000000000001100101010001101000100000000000';
do0 = '110011000000001000000000000000000001100101010001101000100000000000';
up1 = '?';
st1 = '?';
do1 = '?';
up2 = '110011000000100100000000000000001001100100010001101000100000000000';
st2 = '110011000000011000000000000000001001100100010001101000100000000000';
do2 = '110011000000001000000000000000001001100100010001101000100000000000';
up3 = '110011000000100100000000000000000101100101100001101000100000000000';
st3 = '110011000000011000000000000000000101100101100001101000100000000000';
do3 = '110011000000001000000000000000000101100101100001101000100000000000';
up4 = '110011000000100100000000000000001101100100100001101000100000000000';
st4 = '110011000000011000000000000000001101100100100001101000100000000000';
do4 = '110011000000001000000000000000001101100100100001101000100000000000';
up5 = '110011000000100100000000000000000011100101000001101000100000000000';
st5 = '110011000000011000000000000000000011100101000001101000100000000000';
do5 = '110011000000001000000000000000000011100101000001101000100000000000';
up6 = '110011000000100100000000000000001011100100000001101000100000000000';
st6 = '110011000000011000000000000000001011100100000001101000100000000000';
do6 = '110011000000001000000000000000001011100100000001101000100000000000';
up7 = '110011000000100100000000000000000111100101111110101000100000000000';
st7 = '110011000000011000000000000000000111100101111110101000100000000000';
do7 = '110011000000001000000000000000000111100101111110101000100000000000';
up8 = '110011000000100100000000000000001111100100111110101000100000000000';
st8 = '110011000000011000000000000000001111100100111110101000100000000000';
do8 = '110011000000001000000000000000001111100100111110101000100000000000';
up9 = '110011000000100100000000000000000000010101011110101000100000000000';
st9 = '110011000000011000000000000000000000010101011110101000100000000000';
do9 = '110011000000001000000000000000000000010101011110101000100000000000';
up88 ='010101010000100100000000000000001111100100111110101000100000000000';
st88 ='010101010000011000000000000000001111100100111110101000100000000000';
do88 ='010101010000001000000000000000001111100100111110101000100000000000';
up99 ='010101010000100100000000000000000000010101011110101000100000000000';
st99 ='010101010000011000000000000000000000010101011110101000100000000000';
do99 ='010101010000001000000000000000000000010101011110101000100000000000';
  
#define pin D2  //GPIO4
#define NUM_ATTEMPTS 3
#define TRACE 1  // 0= trace off 1 = trace on Do we want to see trace for debugging purposes
void trc(String msg);              // function prototypes 
void transmit_code(String code);

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void setup(void){
  pinMode(pin,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);   
  
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
  server.on("/", HTTP_GET, []() {  
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> READY ");  
                                });
  server.on("/up6", HTTP_GET, []() { transmit_code( server.uri() ); 
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> ");  
                                });
  server.on("/do6", HTTP_GET, []() { transmit_code(do6); 
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> ");  
                                });
  server.on("/st6", HTTP_GET, []() { transmit_code(st6); 
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> ");  
                                });
                                
  server.on("/canc", HTTP_GET, []() { transmit_gate_code(canc); 
                                  server.send(200, "text/html", "<h1> Gateway Rf </h1> <p>"+server.uri()+"</p> ");  
                                });
                                
  server.onNotFound([]() { server.send(404, "text/plain", "404: Not Found"); });
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started"); 
}


void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

// trace function
void trc(String msg){if (TRACE) { Serial.println(msg); } }


void transmit_code(String code){
  int len = code.length();
  trc(code);
  for (int i = 0; i < NUM_ATTEMPTS; i++) 
  {        
      // ----------------------- Preamble ----------------------
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(pin, LOW);
      delay(50);  // sleep for 0,3 seconds
      for (int i = 0; i < 12; ++i) 
      {
        digitalWrite(pin, HIGH); 
        delayMicroseconds(pulse);
        digitalWrite(pin, LOW); 
        delayMicroseconds(pulse);  
      }
      // ---------------------- End Preamble --------------------
      // -----------------------Segnal --------------------------
      //trc("transmit segnal");
      digitalWrite(pin, LOW);
      delayMicroseconds(3500); // added 3,5 millis
      for (int j=0;j<len;++j) 
      {
         char ch = code.charAt(j);        
         if (ch == '1')
         {
           //trc("1");
           digitalWrite(pin, HIGH);         
           delayMicroseconds(pulse);
           digitalWrite(pin, LOW);          
           delayMicroseconds(pulse*2);
         } 
         else 
         { 
           //trc("0");
           digitalWrite(pin, HIGH); 
           delayMicroseconds(pulse*2);
           digitalWrite(pin, LOW); 
           delayMicroseconds(pulse);
         }
      }
      digitalWrite(pin, LOW);
      delayMicroseconds(2000); // added 2 millis
      digitalWrite(LED_BUILTIN, LOW);         
    // ---------------------End Segnal --------------------------   
    }
    trc("Segnal sended ");
}


void transmit_gate_code(String code){
  int len = code.length();
  trc(code);
  for (int i = 0; i < NUM_ATTEMPTS; i++) 
  {        
      //trc("transmit segnal");
      digitalWrite(pin, LOW);
      for (int j=0;j<len;++j) 
      {
         char ch = code.charAt(j);        
         if (ch == '1')
         {
           digitalWrite(pin, HIGH);         
           delayMicroseconds(short_pulse);
           digitalWrite(pin, LOW);          
           delayMicroseconds(long_gate);
         } 
         else 
         {            
           digitalWrite(pin, HIGH); 
           delayMicroseconds(long_gate);
           digitalWrite(pin, LOW); 
           delayMicroseconds(short_delay);
         }
      }
      digitalWrite(pin, LOW);
      delayMicroseconds(extended_delay); // added 2 millis
      delay(extended_delay);
      digitalWrite(LED_BUILTIN, LOW);         
    }
    trc("Segnal sended ");
}

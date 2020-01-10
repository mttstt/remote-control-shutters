#include "esphome.h"

const txPin_112 = ??; GPIO
int Plugin_112_Repeat = 10;

//# ------- gate ---------
const int short_delay =    760; //μs
const int long_delay =    1520; //μs
const int extended_delay = 0.3;
const uint16_t canc = 0b0111101001000000; //-00


//# -----Shutters------------
const int pulse = 360; //μs
const uint64_t up0 = 0b0101010100001001000000000000000000011001010100011010001000000000; //+00
const uint64_t st0 = 0b0101010100000110000000000000000000011001010100011010001000000000;
const uint64_t do0 = 0b0101010100000010000000000000000000011001010100011010001000000000;
const uint64_t up1 = 0b0;
const uint64_t st1 = 0b0;
const uint64_t do1 = 0b0;
const uint64_t up2 = 0b0101010100001001000000000000000010011001000100011010001000000000;
const uint64_t st2 = 0b0101010100000110000000000000000010011001000100011010001000000000;
const uint64_t do2 = 0b0101010100000010000000000000000010011001000100011010001000000000;
const uint64_t up3 = 0b0101010100001001000000000000000001011001011000011010001000000000;
const uint64_t st3 = 0b0101010100000110000000000000000001011001011000011010001000000000;
const uint64_t do3 = 0b0101010100000010000000000000000001011001011000011010001000000000;
const uint64_t up4 = 0b0101010100001001000000000000000011011001001000011010001000000000;
const uint64_t st4 = 0b0101010100000110000000000000000011011001001000011010001000000000;
const uint64_t do4 = 0b0101010100000010000000000000000011011001001000011010001000000000;
const uint64_t up5 = 0b0101010100001001000000000000000000111001010000011010001000000000;
const uint64_t st5 = 0b0101010100000110000000000000000000111001010000011010001000000000;
const uint64_t do5 = 0b0101010100000010000000000000000000111001010000011010001000000000;
const uint64_t up6 = 0b0101010100001001000000000000000010111001000000011010001000000000;
const uint64_t st6 = 0b0101010100000110000000000000000010111001000000011010001000000000;
const uint64_t do6 = 0b0101010100000010000000000000000010111001000000011010001000000000;
const uint64_t up7 = 0b0101010100001001000000000000000001111001011111101010001000000000;
const uint64_t st7 = 0b0101010100000110000000000000000001111001011111101010001000000000;
const uint64_t do7 = 0b0101010100000010000000000000000001111001011111101010001000000000;
const uint64_t up8 = 0b0101010100001001000000000000000011111001001111101010001000000000;
const uint64_t st8 = 0b0101010100000110000000000000000011111001001111101010001000000000;
const uint64_t do8 = 0b0101010100000010000000000000000011111001001111101010001000000000;
const uint64_t up9 = 0b0101010100001001000000000000000000000101010111101010001000000000;
const uint64_t st9 = 0b0101010100000110000000000000000000000101010111101010001000000000;
const uint64_t do9 = 0b0101010100000010000000000000000000000101010111101010001000000000;



class MyCustomComponent : public Component, public CustomAPIDevice {
 public:
  void setup() override {
    // This will be called once to set up the component
    pinMode(Pin_112, OUTPUT);

    // Declare a service "hello_world"
    //  - Service will be called "esphome.<NODE_NAME>_hello_world" in Home Assistant.
    //  - The service has no arguments
    //  - The function on_hello_world declared below will attached to the service.
    register_service(&MyCustomComponent::on_hello_world, "hello_world");
   
    // Declare a second service "start_shutter"
    //  - Service will be called "esphome.<NODE_NAME>_start_shutter" in Home Assistant.
    //  - The service has three arguments (type inferred from method definition):
    //     - cycle_duration: integer
    //     - silent: boolean
    //     - string_argument: string
    //  - The function on_start_shutter declared below will attached to the service.
    register_service(&MyCustomComponent::on_start_shutter, "start_shutter", {"shutter"});
  }
 
  void on_hello_world() {
    ESP_LOGD("custom", "Hello World!");
    if (is_connected()) {
      // Example check to see if a client is connected
    }
  }

 
  void on_start_shutter(std::string shutter) {
    ESP_LOGD("custom", "Starting shutter command!");
              if (shutter.equalsIgnoreCase(F("canc"))) {sendRFCode_canc(canc); }
              if (shutter.equalsIgnoreCase(F("up0")))  {sendRFCode(up0); };
              if (shutter.equalsIgnoreCase(F("st0")))  {sendRFCode(st0); };
              if (shutter.equalsIgnoreCase(F("do0")))  {sendRFCode(do0); };
              if (shutter.equalsIgnoreCase(F("up1")))  {sendRFCode(up1); };
              if (shutter.equalsIgnoreCase(F("st1")))  {sendRFCode(st1); };
              if (shutter.equalsIgnoreCase(F("do1")))  {sendRFCode(do1); };
              if (shutter.equalsIgnoreCase(F("up2")))  {sendRFCode(up2); };
              if (shutter.equalsIgnoreCase(F("st2")))  {sendRFCode(st2); };
              if (shutter.equalsIgnoreCase(F("do2")))  {sendRFCode(do2); };
              if (shutter.equalsIgnoreCase(F("up3")))  {sendRFCode(up3); };
              if (shutter.equalsIgnoreCase(F("st3")))  {sendRFCode(st3); };
              if (shutter.equalsIgnoreCase(F("do3")))  {sendRFCode(do3); };
              if (shutter.equalsIgnoreCase(F("up4")))  {sendRFCode(up4); };
              if (shutter.equalsIgnoreCase(F("st4")))  {sendRFCode(st4); };
              if (shutter.equalsIgnoreCase(F("do4")))  {sendRFCode(do4); };
              if (shutter.equalsIgnoreCase(F("up5")))  {sendRFCode(up5); };
              if (shutter.equalsIgnoreCase(F("st5")))  {sendRFCode(st5); };
              if (shutter.equalsIgnoreCase(F("do5")))  {sendRFCode(do5); };
              if (shutter.equalsIgnoreCase(F("up6")))  {sendRFCode(up6); };
              if (shutter.equalsIgnoreCase(F("st6")))  {sendRFCode(st6); };
              if (shutter.equalsIgnoreCase(F("do6")))  {sendRFCode(do6); };
              if (shutter.equalsIgnoreCase(F("up7")))  {sendRFCode(up7); };
              if (shutter.equalsIgnoreCase(F("st7")))  {sendRFCode(st7); };
              if (shutter.equalsIgnoreCase(F("do7")))  {sendRFCode(do7); };
              if (shutter.equalsIgnoreCase(F("up8")))  {sendRFCode(up8); };
              if (shutter.equalsIgnoreCase(F("st8")))  {sendRFCode(st8); };
              if (shutter.equalsIgnoreCase(F("do8")))  {sendRFCode(do8); };
              if (shutter.equalsIgnoreCase(F("up9")))  {sendRFCode(up9); };
              if (shutter.equalsIgnoreCase(F("st9")))  {sendRFCode(st9); };
              if (shutter.equalsIgnoreCase(F("do9")))  {sendRFCode(do9); };
              if (shutter.equalsIgnoreCase(F("upZg"))) {sendRFCode(up9); delay(1500);
                                                        sendRFCode(up8); delay(1500);
                                                        sendRFCode(up7); delay(1500);
                                                        sendRFCode(up6);
                                                       };
              if (shutter.equalsIgnoreCase(F("stZg"))) {sendRFCode(st9); delay(1500);
                                                        sendRFCode(st8); delay(1500);
                                                        sendRFCode(st7); delay(1500);
                                                        sendRFCode(st6);
                                                       };
              if (shutter.equalsIgnoreCase(F("doZg"))) {sendRFCode(do9); delay(1500);
                                                        sendRFCode(do8); delay(1500);
                                                        sendRFCode(do7); delay(1500);
                                                        sendRFCode(do6);
                                                       };
    // Call a homeassistant service
    call_homeassistant_service("homeassistant.service");
  }


void sendRFCode_canc(uint16_t code){
  for (int i = 0; i < Plugin_112_Repeat; i++)
  {
      for (int bits = 15; bits > 1 ; bits-- )
      {
         if (code & (1U << bits) )
         {       
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(short_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(long_delay);
         }
         else
         {           
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(long_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(short_delay);
         }
      }
      delayMicroseconds(3000);
    }
}



void sendRFCode(uint64_t code){
  for (int i = 0; i < Plugin_112_Repeat; i++)
    {
      // ----------------------- Preamble ----------------------
      for (int y = 0; y < 12; ++y)
      {
        digitalWrite(txPin_112, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(txPin_112, LOW);
        delayMicroseconds(pulse);
      }
      // -------------------End Preamble--Segnal ---------------
      digitalWrite(txPin_112, LOW);
      delayMicroseconds(3500); // added 3,5 millis

      for (int bits = 63; bits > -1 ; --bits )
      {
        if (code & (1ULL << bits) ) {
            digitalWrite(txPin_112, HIGH);
            delayMicroseconds(pulse);
            digitalWrite(txPin_112, LOW);
            delayMicroseconds(pulse*2);
          }
        else {
            digitalWrite( txPin_112, HIGH );
            delayMicroseconds( pulse * 2 );
            digitalWrite( txPin_112, LOW );
            delayMicroseconds( pulse );
        }
      }
      // -------------------Close--Segnal:00 ---------------
      digitalWrite( txPin_112, HIGH );
      delayMicroseconds( pulse * 2 );
      digitalWrite( txPin_112, LOW );
      delayMicroseconds( pulse );
      //
      digitalWrite( txPin_112, HIGH );
      delayMicroseconds( pulse * 2 );
      digitalWrite( txPin_112, LOW );
      delayMicroseconds( pulse );
     // -------------------Close--Segnal:00 ---------------
      digitalWrite(txPin_112, LOW);
      delayMicroseconds(5000); // added 2 millis
    // ---------------------End Segnal --------------------------
    }
}


};

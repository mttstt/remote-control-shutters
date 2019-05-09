#ifdef USES_P112

//#######################################################################################################
//########################### Plugin 112: Output 433 MHZ - RF                 ###########################
//#######################################################################################################
/*
   Version: 2.0
   Description: use this script to send RF with a cheap FS1000A alike sender.
                This plugin differs from the original one as it allows you to send custom messages

   Example of usage:
   http://192.168.1.124/control?cmd=rftx,up6
   http://192.168.1.124/control?cmd=rftx,do6

   Learn codes via _P112_RFTX_NORCSwitch.ino plugin!
   Needs: EspEasy
   Tested on GPIO:14
   Author: mtt
   Copyright: (c) 2019-2020 mttstt
   License: MIT
   License URI: http://en.wikipedia.org/wiki/MIT_License
   Status : "Proof of concept"

   PIN Nodemcu successuful tested: GPIO-5(D1)
                                   GPIO-4(D2)
                                   GPIO-2(D4)
 */

//#ifdef PLUGIN_BUILD_TESTING

#define PLUGIN_112
#define PLUGIN_ID_112         112
#define PLUGIN_NAME_112       "RF Custom Transmit - FS1000A alike sender "
#define PLUGIN_112_DEBUG  true      //set to true for extra log info in the debug

int Plugin_112_Repeat;
int txPin_112;


//# ------- gate ---------
const int short_delay =    760; //μs
const int long_delay =    1520; //μs
const int extended_delay = 0.5;
//String canc = "01111010010000";
const uint16_t canc = "0b0111101001000000"; //-00

//# -----Blinds------------
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

boolean Plugin_112(byte function, struct EventStruct *event, String& string)
{
        boolean success = false;
        switch (function)
        {
        case PLUGIN_DEVICE_ADD:
        {
                Device[++deviceCount].Number = PLUGIN_ID_112;
                Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
                Device[deviceCount].SendDataOption = false;
                Device[deviceCount].Ports = 0;
                Device[deviceCount].VType = SENSOR_TYPE_SWITCH;
                Device[deviceCount].PullUpOption = false;
                Device[deviceCount].InverseLogicOption = false;
                Device[deviceCount].FormulaOption = false;
                Device[deviceCount].ValueCount = 0;
                Device[deviceCount].TimerOption = false;
                Device[deviceCount].TimerOptional = false;
                break;
        }

        case PLUGIN_GET_DEVICENAME:
        {
                string = F(PLUGIN_NAME_112);
                break;
        }

        case PLUGIN_GET_DEVICEVALUENAMES:
        {
                break;
        }

        case PLUGIN_WEBFORM_LOAD:
        {
                Plugin_112_Repeat = ExtraTaskSettings.TaskDevicePluginConfigLong[0];
                addFormNumericBox(F("Repeat (default=1)"), F("plugin_112_repeat"), Plugin_112_Repeat, 1, 20);
                success = true;
                break;
        }


        case PLUGIN_WEBFORM_SAVE:
        {
                Plugin_112_Repeat = getFormItemInt(F("plugin_112_repeat"));
                if (Plugin_112_Repeat > 20) Plugin_112_Repeat = 1;
                ExtraTaskSettings.TaskDevicePluginConfigLong[0] = Plugin_112_Repeat;
                SaveTaskSettings(event->TaskIndex);
                success = true;
                break;
        }

        case PLUGIN_WEBFORM_SHOW_CONFIG:
        {
                string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[0]);
                success = true;
                break;
        }


        case PLUGIN_INIT:
        {
                LoadTaskSettings(event->TaskIndex);
                Plugin_112_Repeat = ExtraTaskSettings.TaskDevicePluginConfigLong[0];
                txPin_112 = Settings.TaskDevicePin1[event->TaskIndex];
              //  Serial.println("txPin_112: "); Serial.println(txPin_112);
                if (txPin_112 != -1)
                {
                    addLog(LOG_LEVEL_INFO, F("INIT: RF433 TX created!"));
                    pinMode(txPin_112,OUTPUT);
                }
                if (txPin_112 == -1)
                {
                    addLog(LOG_LEVEL_INFO, F("INIT: RF433 TX REMOVED!"));
                    pinMode(txPin_112,INPUT);
                }
                success = true;
                break;
        }


        case PLUGIN_WRITE:
        {
          String command;
          String shutter;
          success = false;
          command = parseString(string, 1);
          shutter = parseString(string, 2);
          if (command == F("rftx")) {
              addLog(LOG_LEVEL_INFO, F("command: ")); Serial.println(F("command: "));
              addLog(LOG_LEVEL_INFO, command); Serial.println(command);
              addLog(LOG_LEVEL_INFO, shutter); Serial.println(shutter);

              if (shutter.equalsIgnoreCase(F("canc"))) {sendRFCode_canc(canc); success = true; }                             
              if (shutter.equalsIgnoreCase(F("up0")))  {sendRFCode(up0); success = true;};
              if (shutter.equalsIgnoreCase(F("st0")))  {sendRFCode(st0); success = true;};
              if (shutter.equalsIgnoreCase(F("do0")))  {sendRFCode(do0); success = true;};
              if (shutter.equalsIgnoreCase(F("up1")))  {sendRFCode(up1); success = true;};
              if (shutter.equalsIgnoreCase(F("st1")))  {sendRFCode(st1); success = true;};
              if (shutter.equalsIgnoreCase(F("do1")))  {sendRFCode(do1); success = true;};
              if (shutter.equalsIgnoreCase(F("up2")))  {sendRFCode(up2); success = true;};
              if (shutter.equalsIgnoreCase(F("st2")))  {sendRFCode(st2); success = true;};
              if (shutter.equalsIgnoreCase(F("do2")))  {sendRFCode(do2); success = true;};
              if (shutter.equalsIgnoreCase(F("up3")))  {sendRFCode(up3); success = true;};
              if (shutter.equalsIgnoreCase(F("st3")))  {sendRFCode(st3); success = true;};
              if (shutter.equalsIgnoreCase(F("do3")))  {sendRFCode(do3); success = true;};
              if (shutter.equalsIgnoreCase(F("up4")))  {sendRFCode(up4); success = true;};
              if (shutter.equalsIgnoreCase(F("st4")))  {sendRFCode(st4); success = true;};
              if (shutter.equalsIgnoreCase(F("do4")))  {sendRFCode(do4); success = true;};
              if (shutter.equalsIgnoreCase(F("up5")))  {sendRFCode(up5); success = true;};
              if (shutter.equalsIgnoreCase(F("st5")))  {sendRFCode(st5); success = true;};
              if (shutter.equalsIgnoreCase(F("do5")))  {sendRFCode(do5); success = true;};
              if (shutter.equalsIgnoreCase(F("up6")))  {sendRFCode(up6); success = true;};
              if (shutter.equalsIgnoreCase(F("st6")))  {sendRFCode(st6); success = true;};
              if (shutter.equalsIgnoreCase(F("do6")))  {sendRFCode(do6); success = true;};
              if (shutter.equalsIgnoreCase(F("up7")))  {sendRFCode(up7); success = true;};
              if (shutter.equalsIgnoreCase(F("st7")))  {sendRFCode(st7); success = true;};
              if (shutter.equalsIgnoreCase(F("do7")))  {sendRFCode(do7); success = true;};
              if (shutter.equalsIgnoreCase(F("up8")))  {sendRFCode(up8); success = true;};
              if (shutter.equalsIgnoreCase(F("st8")))  {sendRFCode(st8); success = true;};
              if (shutter.equalsIgnoreCase(F("do8")))  {sendRFCode(do8); success = true;};
              if (shutter.equalsIgnoreCase(F("up9")))  {sendRFCode(up9); success = true;};
              if (shutter.equalsIgnoreCase(F("st9")))  {sendRFCode(st9); success = true;};
              if (shutter.equalsIgnoreCase(F("do9")))  {sendRFCode(do9); success = true;};
              if (shutter.equalsIgnoreCase(F("upZg"))) {sendRFCode(up9); delay(1500);
                                                        sendRFCode(up8); delay(1500);
                                                        sendRFCode(up7); delay(1500);
                                                        sendRFCode(up6); success = true;
                                                       };
              if (shutter.equalsIgnoreCase(F("stZg"))) {sendRFCode(st9); delay(1500);
                                                        sendRFCode(st8); delay(1500);
                                                        sendRFCode(st7); delay(1500);
                                                        sendRFCode(st6); success = true;
                                                       };
              if (shutter.equalsIgnoreCase(F("doZg"))) {sendRFCode(do9); delay(1000);
                                                        sendRFCode(do8); delay(1000);
                                                        sendRFCode(do7); delay(1000);
                                                        sendRFCode(do6); success = true;
                                                       };                           
            }

            if (success) {
                            String url = String(WiFi.localIP().toString()) + "/control?cmd=" + string;
                            addLog(LOG_LEVEL_INFO, F("To send this command again, "));
                            addLog(LOG_LEVEL_INFO, "use this: <a href=\"http://" + url + "\">URL</a>");
                            if (printToWeb)
                              {
                                printWebString += F("RFTX Gateway <BR>");
                                printWebString += F("URL: <a href=\"http://"); printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=rftx,up6");
                                printWebString += F("\">http://");printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=rftx,up6"); printWebString += F("</a><BR>");

                               }
                          }
            break;
        }
     }
     return success;
}


void sendRFCode(uint64_t code){
  addLog(LOG_LEVEL_INFO, F("trasmitting")); Serial.println(F("trasmitting"));
  addLog(LOG_LEVEL_INFO, uint64ToString(code) ); Serial.println( uint64ToString(code) );

  for (int i = 0; i < Plugin_112_Repeat; i++)
    {
      // ----------------------- Preamble ----------------------
      for (int y = 0; y < 12; ++y)
      {
        //Serial.println(y);
        digitalWrite(txPin_112, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(txPin_112, LOW);
        delayMicroseconds(pulse);
      }
      // -------------------End Preamble--Segnal ---------------
      digitalWrite(txPin_112, LOW);
      delayMicroseconds(3500); // added 3,5 millis

      for (int bits = 0; bits < 64 ; ++bits )
      {
        if (code & (1ULL >> bits) ) {
            Serial.print(F("1"));
            digitalWrite(txPin_112, HIGH);
            delayMicroseconds(pulse);
            digitalWrite(txPin_112, LOW);
            delayMicroseconds(pulse*2);
          }
        else {
            Serial.print(F("0"));
            digitalWrite( txPin_112, HIGH );
            delayMicroseconds( pulse * 2 );
            digitalWrite( txPin_112, LOW );
            delayMicroseconds( pulse );
        }
      }
      // -------------------Close--Segnal:00 ---------------
      addLog(LOG_LEVEL_INFO, F("00")); Serial.println(F("00"));
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


void sendRFCode_canc(uint16_t code){
  for (int i = 0; i < Plugin_112_Repeat; i++)
  {      
      for (int bits = 0; bits < 14 ; ++bits )
      {
         if (code & (1U >> bits) )
         {
           Serial.print(F("1")); addLog(LOG_LEVEL_INFO, F("cancello: 1"));
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(short_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(long_delay);          
         }
         else
         {
           Serial.print(F("0")); addLog(LOG_LEVEL_INFO, F("cancello: 0"));
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(long_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(short_delay);        
         }
      }
      delay(extended_delay);
    }
}

String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}
#endif

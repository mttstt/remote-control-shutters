#ifdef USES_P112

//#######################################################################################################
//########################### Plugin 112: Output 433 MHZ - RF                 ###########################
//#######################################################################################################
/*
   Version: 2.0
   Description: use this script to send RF with a cheap FS1000A alike sender.
                This plugin differs from the original one as it allows you to send custom messages
   Example of usage:

      http://<ESP IP address>/control?cmd=RFSEND,canc
      http://<ESP IP address>/control?cmd=RFSEND,up7
      
      ????????????
      http://xxx.dlinkddns.com:7000/?shutter={{TextField}}&where=st
      ????????????

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
#define PLUGIN_xxx_DEBUG  false             //set to true for extra log info in the debug


int Plugin_112_Repeat;
int txPin_112;


//==============================================================================

//# ------- gate ---------
const int short_delay =    760; //μs
const int long_delay =    1520; //μs
const int extended_delay = 0.5;
String canc = "01111010010000";

//# -----Blinds------------
const int pulse = 360; //μs

const unsigned long upH = 3423141888;
const unsigned long stH = 3422945280;
const unsigned long doH = 3422683136;

const unsigned long 0L =  424780288;
const unsigned long 6L = 3103891968;
const unsigned long 7L = 2038342144;



/*
String up0 = "110011000000100100000000000000000001100101010001101000100000000000";
String st0 = "110011000000011000000000000000000001100101010001101000100000000000";
String do0 = "110011000000001000000000000000000001100101010001101000100000000000";
String up1 = "?";
String st1 = "?";
String do1 = "?";
String up2 = "110011000000100100000000000000001001100100010001101000100000000000";
String st2 = "110011000000011000000000000000001001100100010001101000100000000000";
String do2 = "110011000000001000000000000000001001100100010001101000100000000000";
String up3 = "110011000000100100000000000000000101100101100001101000100000000000";
String st3 = "110011000000011000000000000000000101100101100001101000100000000000";
String do3 = "110011000000001000000000000000000101100101100001101000100000000000";
String up4 = "110011000000100100000000000000001101100100100001101000100000000000";
String st4 = "110011000000011000000000000000001101100100100001101000100000000000";
String do4 = "110011000000001000000000000000001101100100100001101000100000000000";
String up5 = "110011000000100100000000000000000011100101000001101000100000000000";
String st5 = "110011000000011000000000000000000011100101000001101000100000000000";
String do5 = "110011000000001000000000000000000011100101000001101000100000000000";
String up6 = "110011000000100100000000000000001011100100000001101000100000000000";
String st6 = "110011000000011000000000000000001011100100000001101000100000000000";
String do6 = "110011000000001000000000000000001011100100000001101000100000000000";
String up7 = "110011000000100100000000000000000111100101111110101000100000000000";
String st7 = "110011000000011000000000000000000111100101111110101000100000000000";
String do7 = "110011000000001000000000000000000111100101111110101000100000000000";
String up8 = "110011000000100100000000000000001111100100111110101000100000000000";
String st8 = "110011000000011000000000000000001111100100111110101000100000000000";
String do8 = "110011000000001000000000000000001111100100111110101000100000000000";
String up9 = "110011000000100100000000000000000000010101011110101000100000000000";
String st9 = "110011000000011000000000000000000000010101011110101000100000000000";
String do9 = "110011000000001000000000000000000000010101011110101000100000000000";
String up88 ="010101010000100100000000000000001111100100111110101000100000000000";
String st88 ="010101010000011000000000000000001111100100111110101000100000000000";
String do88 ="010101010000001000000000000000001111100100111110101000100000000000";
String up99 ="010101010000100100000000000000000000010101011110101000100000000000";
String st99 ="010101010000011000000000000000000000010101011110101000100000000000";
String do99 ="010101010000001000000000000000000000010101011110101000100000000000";
*/

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

           String TmpStr1;
           String rfType;
           
           char command[6] = parseString(string, 1);
           char shutter[1] = parseString(string, 2);
           char where[2] = parseString(string, 3);
           
           if ( GetArgv(string.c_str(), TmpStr1, 2) ) rfType = TmpStr1.c_str();
           

           if (command == F("rfsend")) {

        
             
              addLog(LOG_LEVEL_INFO, F("command: RFSEND "));
              addLog(LOG_LEVEL_INFO, rfType); Serial.println(rfType);

             if ( rfType.equalsIgnoreCase("canc") ) { transmit_gate_code(canc); success = true; }
              else {
                      transmit_code(ch); success = true;
                      rfType == "up0";                     
               }
            }

            if (success) {
                            String url = String(Settings.Name) + "/control?cmd=" + string;
                            addLog(LOG_LEVEL_INFO, "To send this command again, ");
                            addLog(LOG_LEVEL_INFO, "use this: <a href=\"http://" + url + "\">URL</a>");
                            if (printToWeb)
                              {
                                 printWebString += F("RCSwitch Code Sent!");
                                 printWebString += F("<BR>Repeats: ");
                                 printWebString += String(Plugin_112_Repeat);
                                 printWebString += F("<BR><BR>");
                                 printWebString += F("<BR>Use URL: <a href=\"http://");
                                 printWebString += url;
                                 printWebString += F("\">http://");
                                 printWebString += url;
                                 printWebString += F("</a>");
                               }
                          }
            break;
        }
     }
     return success;
}

//==========================================================================


void transmit_code(String code){
  addLog(LOG_LEVEL_INFO, F("trasmitting")); Serial.println(F("trasmitting"));
  int len = code.length();
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

      for (int j=0;j<len;++j)
      {
         char ch = code.charAt(j);
         if (ch == '1')
         {
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(pulse);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(pulse*2);
         }
         else
         {
           digitalWrite( txPin_112, HIGH );
           delayMicroseconds( pulse * 2 );
           digitalWrite( txPin_112, LOW );
           delayMicroseconds( pulse );
         }
      }
      digitalWrite(txPin_112, LOW);
      delayMicroseconds(5000); // added 2 millis
    // ---------------------End Segnal --------------------------
    }
}


void transmit_gate_code(String code){
  int len = code.length();
  for (int i = 0; i < Plugin_112_Repeat; i++)
  {
      //digitalWrite(txPin_112, LOW);
      for (int j=0;j<len;++j)
      {
         char ch = code.charAt(j);
         if (ch == '1')
         {
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(short_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(long_delay);
           addLog(LOG_LEVEL_INFO, F("cancello: 1"));
         }
         else
         {
           digitalWrite(txPin_112, HIGH);
           delayMicroseconds(long_delay);
           digitalWrite(txPin_112, LOW);
           delayMicroseconds(short_delay);
           addLog(LOG_LEVEL_INFO, F("cancello: 0"));
         }
      }
      //digitalWrite(txPin_112, LOW);
      //delayMicroseconds(extended_delay); // added 2 millis
      delay(extended_delay);
    //  digitalWrite(LED_BUILTIN, LOW);
    }
}

#endif

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
      http://xxx.dlinkddns.com:7000/?rftx={{TextField}},st
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




const uint64_t up0 = 0b0101010100001001000000000000000000011001010100011010001000000000; //+00
st0 = '010101010000011000000000000000000001100101010001101000100000000000'
do0 = '010101010000001000000000000000000001100101010001101000100000000000'
up1 = '?'
st1 = '?'
do1 = '?'
up2 = '010101010000100100000000000000001001100100010001101000100000000000'
st2 = '010101010000011000000000000000001001100100010001101000100000000000'
do2 = '010101010000001000000000000000001001100100010001101000100000000000'
up3 = '010101010000100100000000000000000101100101100001101000100000000000'
st3 = '010101010000011000000000000000000101100101100001101000100000000000'
do3 = '010101010000001000000000000000000101100101100001101000100000000000'
up4 = '010101010000100100000000000000001101100100100001101000100000000000'
st4 = '010101010000011000000000000000001101100100100001101000100000000000'
do4 = '010101010000001000000000000000001101100100100001101000100000000000'
up5 = '010101010000100100000000000000000011100101000001101000100000000000'
st5 = '010101010000011000000000000000000011100101000001101000100000000000'
do5 = '010101010000001000000000000000000011100101000001101000100000000000'
up6 = '010101010000100100000000000000001011100100000001101000100000000000'
st6 = '010101010000011000000000000000001011100100000001101000100000000000'
do6 = '010101010000001000000000000000001011100100000001101000100000000000'
up7 = '010101010000100100000000000000000111100101111110101000100000000000'
st7 = '010101010000011000000000000000000111100101111110101000100000000000'
do7 = '010101010000001000000000000000000111100101111110101000100000000000'
up8 = '010101010000100100000000000000001111100100111110101000100000000000'
st8 = '010101010000011000000000000000001111100100111110101000100000000000'
do8 = '010101010000001000000000000000001111100100111110101000100000000000'
up9 = '010101010000100100000000000000000000010101011110101000100000000000'
st9 = '010101010000011000000000000000000000010101011110101000100000000000'
do9 = '010101010000001000000000000000000000010101011110101000100000000000'


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
           char command[6] = parseString(string, 1);
           char shutter[1] = parseString(string, 2);
           char where[2] = parseString(string, 3);

           if (command == F("rftx")) {
              addLog(LOG_LEVEL_INFO, F("command: RFTX "));
              addLog(LOG_LEVEL_INFO, shutter); Serial.println(shutter);

              if ( shutter.equalsIgnoreCase("canc") ) { transmit_gate_code(canc); success = true; }
              else {

                if (where.equals(F("up0"))) sendRFCode(up0);
                if (where.equals(F("st0"))) sendRFCode(st0);
                if (where.equals(F("do0"))) sendRFCode(do0);

                if (shutter.equals(F("up0"))) sendRFCode(0L);
                if (shutter.equals(F("1"))) sendRFCode(1L);
                if (shutter.equals(F("2"))) sendRFCode(2L);
                if (shutter.equals(F("3"))) sendRFCode(3L);
                if (shutter.equals(F("4"))) sendRFCode(4L);
                if (shutter.equals(F("5"))) sendRFCode(5L);
                if (shutter.equals(F("6"))) sendRFCode(6L);
                if (shutter.equals(F("7"))) sendRFCode(7L);
                if (shutter.equals(F("8"))) sendRFCode(8L);
                if (shutter.equals(F("9"))) sendRFCode(9L);

                transmit_code(shutter, where); success = true;

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


void transmit_code(char shutter, char where){
  addLog(LOG_LEVEL_INFO, F("trasmitting")); Serial.println(F("trasmitting"));

  if (where.equals(F("up"))) sendRFCode(upH);
  if (where.equals(F("st"))) sendRFCode(stH);
  if (where.equals(F("do"))) sendRFCode(doL);

  if (shutter.equals(F("0"))) sendRFCode(0L);
  if (shutter.equals(F("1"))) sendRFCode(1L);
  if (shutter.equals(F("2"))) sendRFCode(2L);
  if (shutter.equals(F("3"))) sendRFCode(3L);
  if (shutter.equals(F("4"))) sendRFCode(4L);
  if (shutter.equals(F("5"))) sendRFCode(5L);
  if (shutter.equals(F("6"))) sendRFCode(6L);
  if (shutter.equals(F("7"))) sendRFCode(7L);
  if (shutter.equals(F("8"))) sendRFCode(8L);
  if (shutter.equals(F("9"))) sendRFCode(9L);



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


        if (c & (1 << bits)) {
            Serial.print("1");
            digitalWrite(txPin_112, HIGH);
            delayMicroseconds(pulse);
            digitalWrite(txPin_112, LOW);
            delayMicroseconds(pulse*2);
          }
        else {
            Serial.print("0");
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



//#######################################################################################################
//########################### Plugin 112: Output 433 MHZ - RF                 ###########################
//#######################################################################################################
/*
   Version: 2.0
   Description: use this script to send RF with a cheap FS1000A alike sender.
   This plugin extends _P112_RFTX.ino in order to send custom segnals (RFCUSTOM).
   
   Example of usage:

   Learn codes via _P111_RF.ino plugin!
   Needs: RCSwitch library
   Tested on GPIO:14
   Author: mttstt
   Copyright: (c) 2019-2020 (mttstt)
   License: MIT
   License URI: http://en.wikipedia.org/wiki/MIT_License
   Status : "Proof of concept"

   Usage:
   1=RFSEND or RFCUSTOM or RFSWITCH or RFTRISTATE Or RFSWITCHTYPEA/B/C/D
   2=commando (1=on 0=off for DIP switches, otherwise use code)
   3=repeat (if not set will use default settings)
   4=bits (if not set will use default settings)
   5=group (for DIP switches only)
   6=device (for DIP switches only)
   7=family (for DIP switches only)
   
   RFSEND
                                       1      2              3  4
   http://<ESP IP address>/control?cmd=RFSEND,blablacommando,10,24
   
   RFCUSTOM
                                       1        2   3
   http://<ESP IP address>/control?cmd=RFCUSTOM,up6,10

   DIP switches A+B+D:
                                       1             2 5     6     3
   http://<ESP IP address>/control?cmd=RFSWITCHTYPEA,1,11001,01000,10

   DIP switches C (intertechno):
                                       1             2 5 6 7 3
   http://<ESP IP address>/control?cmd=RFSWITCHTYPEC,1,1,2,a,10

   TriState switches:
                                       1         2              3
   http://<ESP IP address>/control?cmd=RFTRISTATE,blablacommando,10

   This program was developed independently and it is not supported in any way.
 */

#ifdef PLUGIN_BUILD_TESTING

#include <RCSwitch.h>
RCSwitch *rcswitchSender;

#define PLUGIN_112
#define PLUGIN_ID_112         112
#define PLUGIN_NAME_112       "RF Transmit - FS1000A alike sender"

unsigned int Plugin_112_iCode;
unsigned int Plugin_112_Repeat;
unsigned int Plugin_112_Bits;
unsigned int Plugin_112_Pulse;
unsigned int Plugin_112_nGroup;
unsigned int Plugin_112_nDevice;
unsigned int Plugin_112_nAddressCode;
unsigned int Plugin_112_nChannelCode;
char* Plugin_112_sCodeWord;
char* Plugin_112_sGroup;
char* Plugin_112_sDevice;
char Plugin_112_csGroup;
char Plugin_112_sFamily;



//==============================================================================
// #define pin D2             //GPIO4
// #define NUM_ATTEMPTS 3

//# ------- gate ---------
const int short_delay =    760; //μs
const int long_delay =    1520; //μs
const int extended_delay = 0.5;
String canc = "01111010010000";

//# -----Blinds------------
const int pulse = 360; //μs
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
//======================================================================================


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
                char tmpString[140];
                Plugin_112_Bits = ExtraTaskSettings.TaskDevicePluginConfigLong[0];
                Plugin_112_Pulse = ExtraTaskSettings.TaskDevicePluginConfigLong[1];
                Plugin_112_Repeat = ExtraTaskSettings.TaskDevicePluginConfigLong[2];
                addFormNumericBox(F("Bits (default=24)"), F("plugin_112_bits"), Plugin_112_Bits, 1, 100);
                addFormNumericBox(F("Pulselength (default=165)"), F("plugin_112_pulse"), Plugin_112_Pulse, 1, 1000);
                addFormNumericBox(F("Repeat (default=1)"), F("plugin_112_repeat"), Plugin_112_Repeat, 1, 20);
                success = true;
                break;
        }

        case PLUGIN_WEBFORM_SAVE:
        {
                Plugin_112_Bits   = getFormItemInt(F("plugin_112_bits"));
                Plugin_112_Pulse  = getFormItemInt(F("plugin_112_pulse"));
                Plugin_112_Repeat = getFormItemInt(F("plugin_112_repeat"));

                if (Plugin_112_Bits > 100) Plugin_112_Bits = 24;
                if (Plugin_112_Pulse > 1000) Plugin_112_Pulse = 165;
                if (Plugin_112_Repeat > 20) Plugin_112_Repeat = 1;
                ExtraTaskSettings.TaskDevicePluginConfigLong[0] = Plugin_112_Bits;
                ExtraTaskSettings.TaskDevicePluginConfigLong[1] = Plugin_112_Pulse;
                ExtraTaskSettings.TaskDevicePluginConfigLong[2] = Plugin_112_Repeat;
                SaveTaskSettings(event->TaskIndex);
                success = true;
                break;
        }

        case PLUGIN_WEBFORM_SHOW_CONFIG:
        {
                string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[0]);
                string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[1]);
                string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[2]);
                success = true;
                break;
        }

        case PLUGIN_INIT:
        {
                LoadTaskSettings(event->TaskIndex);
                Plugin_112_Bits = ExtraTaskSettings.TaskDevicePluginConfigLong[0];
                Plugin_112_Pulse = ExtraTaskSettings.TaskDevicePluginConfigLong[1];
                Plugin_112_Repeat = ExtraTaskSettings.TaskDevicePluginConfigLong[2];

                int txPin = Settings.TaskDevicePin1[event->TaskIndex];

                if (rcswitchSender == 0 && txPin != -1)
                {
                        addLog(LOG_LEVEL_INFO, "INIT: RF433 TX created!");
                        //rcswitchSender = new RCSwitch(txPin);

                        rcswitchSender = new RCSwitch();

                        rcswitchSender->enableTransmit(txPin);
                        rcswitchSender->setPulseLength(Plugin_112_Pulse);

                        // Set TX Repeat xx times
                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);
                        // RepeatTransmit * PulseLength = TX time total
                        // Ex. RepeatTransmit = 3 & PulseLength = 330ms
                        // 3 * 330ms = 990ms or 0,990 Sec. for TX time total
                }

                if (rcswitchSender != 0 && txPin == -1)
                {
                        addLog(LOG_LEVEL_INFO, "INIT: RF433 TX REMOVED!");
                        delete rcswitchSender;
                        rcswitchSender = 0;
                }
                success = true;
                break;
        }

        case PLUGIN_WRITE:
        {
                //reset
                unsigned int Plugin_112_iCode = 0;
                unsigned int Plugin_112_nGroup = 0;
                unsigned int Plugin_112_nDevice = 0;
                unsigned int Plugin_112_nAddressCode = 0;
                unsigned int Plugin_112_nChannelCode = 0;
                char * Plugin_112_sCodeWord;
                char* Plugin_112_sGroup;
                char* Plugin_112_sDevice;
                char Plugin_112_csGroup = '\0';
                char Plugin_112_sFamily = '\0';

                char command[80]; command[0] = 0;
                char TmpStr1[80]; TmpStr1[0] = 0;
                char TmpStr2[80]; TmpStr2[0] = 0;
                char TmpStr3[80]; TmpStr3[0] = 0;
                char TmpStr4[80]; TmpStr4[0] = 0;
                string.toCharArray(command, 80);

                String tmpString = string;
                int argIndex = tmpString.indexOf(',');
                if (argIndex) tmpString = tmpString.substring(0, argIndex);

                if (tmpString.equalsIgnoreCase("RFSWITCHTYPEA") && rcswitchSender != 0)
                {
                        /* For DIP switches type A */
                        Serial.println("RFSWITCHTYPEA");
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_iCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr2, 3)) Plugin_112_sGroup = TmpStr2;
                        if (GetArgv(command, TmpStr3, 4)) Plugin_112_sDevice = TmpStr3;
                        if (GetArgv(command, TmpStr4, 5)) Plugin_112_Repeat = str2int(TmpStr4);

                        /* checks */
                        if (Plugin_112_iCode == 0) break;
                        //if (Plugin_112_sGroup == '\0') break;
                        //if (Plugin_112_sDevice == '\0') break;
                        if (Plugin_112_Repeat > 20) { break; }

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        /**
                         * Switch a remote switch on (Type A with 10 pole DIP switches)
                         *
                         * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
                         * @param sDevice       Code of the switch device (refers to DIP switches 6..10 (A..E) where "1" = on and "0" = off, if all DIP switches are on it's "11111")
                         *
                         * void RCSwitch::switchOn(const char* sGroup, const char* sDevice) {
                         */
                        if (Plugin_112_iCode == 1) {
                                rcswitchSender->switchOn(Plugin_112_sGroup, Plugin_112_sDevice);
                        } else {
                                rcswitchSender->switchOff(Plugin_112_sGroup, Plugin_112_sDevice);
                        }
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFSWITCHTYPEB") && rcswitchSender != 0)
                {
                        /* For DIP switches type B */
                        Serial.println("RFSWITCHTYPEB");
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_iCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 3)) Plugin_112_nAddressCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 4)) Plugin_112_nChannelCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 5)) Plugin_112_Repeat = str2int(TmpStr1);

                        /* checks */
                        if (Plugin_112_iCode == 0) break;
                        if (Plugin_112_nAddressCode == 0) break;
                        if (Plugin_112_nChannelCode == 0) break;
                        if (Plugin_112_Repeat > 20) break;

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        /**
                         * Switch a remote switch on (Type B with two rotary/sliding switches)
                         *
                         * @param nAddressCode  Number of the switch group (1..4)
                         * @param nChannelCode  Number of the switch itself (1..4)
                         *
                         * void RCSwitch::switchOn(int nAddressCode, int nChannelCode) {
                         */
                        if (Plugin_112_iCode == 1) {
                                rcswitchSender->switchOn((int)Plugin_112_nAddressCode, (int)Plugin_112_nChannelCode);
                        } else {
                                rcswitchSender->switchOff((int)Plugin_112_nAddressCode, (int)Plugin_112_nChannelCode);
                        }
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFSWITCHTYPEC") && rcswitchSender != 0)
                {
                        /* For DIP switches type C */
                        Serial.println("RFSWITCHTYPEC");
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_iCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 3)) Plugin_112_nDevice = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 4)) Plugin_112_nGroup = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr2, 5)) Plugin_112_sFamily = TmpStr2[0]; //only first char from string
                        if (GetArgv(command, TmpStr1, 6)) Plugin_112_Repeat = str2int(TmpStr1);

                        /* checks */
                        if (Plugin_112_iCode == 0) break;
                        if (Plugin_112_nDevice == 0) break;
                        if (Plugin_112_nGroup == 0) break;
                        if (Plugin_112_sFamily == '\0') break;
                        if (Plugin_112_Repeat > 20) break;

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        /**
                         * Switch a remote switch on (Type C Intertechno)
                         *
                         * @param sFamily  Familycode (a..f)
                         * @param nGroup   Number of group (1..4)
                         * @param nDevice  Number of device (1..4)
                         *
                         * void RCSwitch::switchOn(char sFamily, int nGroup, int nDevice) {
                         */
                        if (Plugin_112_iCode == 1) {
                                rcswitchSender->switchOn(Plugin_112_sFamily, Plugin_112_nGroup, Plugin_112_nDevice);
                        } else {
                                rcswitchSender->switchOff(Plugin_112_sFamily, Plugin_112_nGroup, Plugin_112_nDevice);
                        }
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFSWITCHTYPED") && rcswitchSender != 0)
                {
                        /* For DIP switches type D */
                        Serial.println("RFSWITCHTYPED");

                        /**
                         * Switch a remote switch on (Type D REV)
                         *
                         * @param sGroup        Code of the switch group (A,B,C,D)
                         * @param nDevice       Number of the switch itself (1..3)
                         *
                         * void RCSwitch::switchOn(char sGroup, int nDevice) {
                         */
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_iCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 3)) Plugin_112_nDevice = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr2, 4)) Plugin_112_csGroup = TmpStr2[0];
                        if (GetArgv(command, TmpStr1, 5)) Plugin_112_Repeat = str2int(TmpStr1);

                        /* checks */
                        if (Plugin_112_iCode == 0) break;
                        if (Plugin_112_nDevice == 0) break;
                        //if (Plugin_112_csGroup == '\0') break;
                        if (Plugin_112_Repeat > 20) break;

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        if (Plugin_112_iCode == 1) {
                                rcswitchSender->switchOn(Plugin_112_csGroup, Plugin_112_nDevice);
                        } else {
                                rcswitchSender->switchOff(Plugin_112_csGroup, Plugin_112_nDevice);
                        }
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFTRISTATE") && rcswitchSender != 0)
                {
                        /* For RFTRISTATE commands */
                        Serial.println("RFTRISTATE");
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_sCodeWord = TmpStr1;
                        if (GetArgv(command, TmpStr2, 3)) Plugin_112_Repeat = str2int(TmpStr2);

                        /* checks */
                        //if (strcmp(Plugin_112_sCodeWord, "") == 0) break;
                        if (Plugin_112_Repeat > 20) break;

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        /**
                         * @param sCodeWord   a tristate code word consisting of the letter 0, 1, F
                         *
                         * void RCSwitch::sendTriState(const char* sCodeWord) {
                         */
                        rcswitchSender->sendTriState(Plugin_112_sCodeWord);
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFSEND") && rcswitchSender != 0 && Plugin_112_Bits != 0)
                {
                        /* For general commands */
                        Serial.println("RFSEND");
                        if (GetArgv(command, TmpStr1, 2)) Plugin_112_iCode = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 3)) Plugin_112_Repeat = str2int(TmpStr1);
                        if (GetArgv(command, TmpStr1, 4)) Plugin_112_Bits = str2int(TmpStr1);

                        /* checks */
                        if (Plugin_112_iCode == 0) break;
                        if (Plugin_112_Bits > 100) break;
                        if (Plugin_112_Repeat > 20) break;

                        rcswitchSender->setRepeatTransmit(Plugin_112_Repeat);

                        /**
                         * Transmit the first 'length' bits of the integer 'code'. The
                         * bits are sent from MSB to LSB, i.e., first the bit at position length-1,
                         * then the bit at position length-2, and so on, till finally the bit at position 0.
                         *
                         * void RCSwitch::send(unsigned long code, unsigned int length) {
                         */
                        rcswitchSender->send(Plugin_112_iCode, Plugin_112_Bits);
                        success = true;
                }
                else if (tmpString.equalsIgnoreCase("RFCUSTOM") && rcswitchSender != 0)
                   {
                        /* For general commands */
                        Serial.println("RFCUSTOM");                        
                        char* rfType;
                        if (GetArgv(command, TmpStr1, 2)) rfType = (TmpStr1);                        
                        Serial.println("command: ");
                        Serial.println(rfType);
                        if ( rfType == '\0') break;
                        if ( rfType.equalsIgnoreCase("canc") ) { transmit_code(canc); };
                        else {
                           String ch;      
                           if ( rfType == "up0" ) { ch = up0; }
                           else if ( rfType == "st0") { ch = st0; }
                           else if ( rfType == "do0" ) { ch = do0; }      
                           else if ( rfType == "up1" ) { ch = up1; }
                           else if ( rfType == "st1" ) { ch = st1; }
                           else if ( rfType == "do1" ) { ch = do1; }      
                           else if ( rfType == "up2" ) { ch = up2; }
                           else if ( rfType == "st2" ) { ch = st2; }
                           else if ( rfType == "do2" ) { ch = do2; }      
                           else if ( rfType == "up3" ) { ch = up3; }
                           else if ( rfType == "st3" ) { ch = st3; }
                           else if ( rfType == "do3" ) { ch = do3; }      
                           else if ( rfType == "up4" ) { ch = up4; }
                           else if ( rfType == "st4" ) { ch = st4; }
                           else if ( rfType == "do4" ) { ch = do4; }      
                           else if ( rfType == "up5" ) { ch = up5; }
                           else if ( rfType == "st5" ) { ch = st5; }
                           else if ( rfType == "do5" ) { ch = do5; }     
                           else if ( rfType == "up6" ) { ch = up6; }
                           else if ( rfType == "st6" ) { ch = st6; }
                           else if ( rfType == "do6" ) { ch = do6; }      
                           else if ( rfType == "up7" ) { ch = up7; }
                           else if ( rfType == "st7" ) { ch = st7; }
                           else if ( rfType == "do7" ) { ch = do7; }      
                           else if ( rfType == "up8" ) { ch = up8; }
                           else if ( rfType == "st8" ) { ch = st8; }
                           else if ( rfType == "do8" ) { ch = do8; }      
                           else if ( rfType == "up9" ) { ch = up9; }
                           else if ( rfType == "st9" ) { ch = st9; }
                           else if ( rfType == "do9" ) { ch = do9; }
                           /**
                           * Transmit the first 'length' bits of the integer 'code'. The
                           */
                         
                           transmit_gate_code(ch);
                           
                        }                        
                        success = true;
                }

                if (success)
                {
                        String url = String(Settings.Name) + "/control?cmd=" + string;
                        addLog(LOG_LEVEL_INFO, "RF Code Sent: " + String(Plugin_112_iCode) + Plugin_112_sCodeWord);
                        addLog(LOG_LEVEL_INFO, "To send this command again, ");
                        addLog(LOG_LEVEL_INFO, "use this: <a href=\"http://" + url + "\">URL</a>");
                        if (printToWeb)
                        {
                                printWebString += F("RCSwitch Code Sent!");
                                printWebString += F("<BR>Value: ");
                                printWebString += Plugin_112_iCode;
                                printWebString += F("<BR>Tristate: ");
                                printWebString += String(Plugin_112_sCodeWord);
                                printWebString += F("<BR>Family: ");
                                printWebString += String(Plugin_112_sFamily);
                                printWebString += F("<BR>sFamily: ");
                                printWebString += String(Plugin_112_sFamily);
                                printWebString += F("<BR>nGroup: ");
                                printWebString += String(Plugin_112_nGroup);
                                printWebString += F("<BR>sGroup: ");
                                printWebString += String(Plugin_112_sGroup);
                                printWebString += String(Plugin_112_csGroup);
                                printWebString += F("<BR>nDevice: ");
                                printWebString += String(Plugin_112_nDevice);
                                printWebString += F("<BR>sDevice: ");
                                printWebString += String(Plugin_112_sDevice);
                                printWebString += F("<BR>nAddressCode: ");
                                printWebString += String(Plugin_112_nAddressCode);
                                printWebString += F("<BR>nChannelCode: ");
                                printWebString += String(Plugin_112_nChannelCode);
                                printWebString += F("<BR>Repeats: ");
                                printWebString += String(Plugin_112_Repeat);
                                printWebString += F("<BR>Bits: ");
                                printWebString += String(Plugin_112_Bits);
                                printWebString += F("<BR>Pulselength: ");
                                printWebString += String(Plugin_112_Pulse);
                                printWebString += F("<BR><BR>");
                                printWebString += F("<BR>Use URL: <a href=\"http://");
                                printWebString += url;
                                printWebString += F("\">http://");
                                printWebString += url;
                                printWebString += F("</a>");

                        }
                }
        }
        break;

        }
        return success;
}


void transmit_code(String code){
  int len = code.length();
  for (int i = 0; i < NUM_ATTEMPTS; i++) 
  {        
      // ----------------------- Preamble ----------------------
      digitalWrite(LED_BUILTIN, LOW);
      for (int i = 0; i < 12; ++i) 
      {
        digitalWrite(pin, HIGH); 
        delayMicroseconds(pulse);
        digitalWrite(pin, LOW); 
        delayMicroseconds(pulse);  
      }
      // -------------------End Preamble--Segnal ---------------
      digitalWrite(pin, LOW);
      delayMicroseconds(3500); // added 3,5 millis
      for (int j=0;j<len;++j) 
      {
         char ch = code.charAt(j);        
         if (ch == '1')
         {
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
      delayMicroseconds(5000); // added 2 millis
      digitalWrite(LED_BUILTIN, LOW);         
    // ---------------------End Segnal --------------------------   
    }
}


void transmit_gate_code(String code){
  int len = code.length();
  for (int i = 0; i < NUM_ATTEMPTS; i++) 
  {              
      digitalWrite(pin, LOW);
      for (int j=0;j<len;++j) 
      {
         char ch = code.charAt(j);        
         if (ch == '1')
         {
           digitalWrite(pin, HIGH);         
           delayMicroseconds(short_delay);
           digitalWrite(pin, LOW);          
           delayMicroseconds(long_delay);
         } 
         else 
         {            
           digitalWrite(pin, HIGH); 
           delayMicroseconds(long_delay);
           digitalWrite(pin, LOW); 
           delayMicroseconds(short_delay);
         }
      }
      digitalWrite(pin, LOW);
      delayMicroseconds(extended_delay); // added 2 millis
      delay(extended_delay);
      digitalWrite(LED_BUILTIN, LOW);         
    } 
}



#endif

//#######################################################################################################
//########################### Plugin 112: Output 433 MHZ - RF                 ###########################
//#######################################################################################################
/*
   Version: 2.0
   Description: use this script to send RF with a cheap FS1000A alike sender
   Example of usage:

   Learn codes via _P111_RF.ino plugin!
   Needs: RCSwitch library
   Tested on GPIO:14
   Author: mtt
   Copyright: (c) 2019-2020 mttstt
   License: MIT
   License URI: http://en.wikipedia.org/wiki/MIT_License
   Status : "Proof of concept"  
 */

#ifdef PLUGIN_BUILD_TESTING

#define PLUGIN_112
#define PLUGIN_ID_112         112
#define PLUGIN_NAME_112       "RF Transmit - FS1000A alike sender"

//==============================================================================
const int pulse = 360; //μs
//# ------- gate ---------
const int short_delay = 760; //μs
const int long_delay = 1520;
const int extended_delay = 0.5;
String canc = "01111010010000";
//# ----------------------

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
#define pin D2        //GPIO4
#define NUM_ATTEMPTS 3
//=================================================================================


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

        case PLUGIN_INIT:
        {
                LoadTaskSettings(event->TaskIndex);

                // ????? int txPin = Settings.TaskDevicePin1[event->TaskIndex];
                int txPin = pin;
                
                if (txPin != -1)
                {
                        addLog(LOG_LEVEL_INFO, "INIT: RF433 TX created!");
                        pinMode(txPin,OUTPUT);
                }

                if (txPin == -1)
                {
                        addLog(LOG_LEVEL_INFO, "INIT: RF433 TX REMOVED!");
                        pinMode(txPin,INPUT);
                }
                success = true;
                break;
        }


        case PLUGIN_WRITE:
        {
            if (????) { transmit_code(ch); } 
            else 
            {
                String ch;      
                if ( server.arg (i) == "up0" ) { ch = up0; }
                 else if ( server.arg(i) == "st0") { ch = st0; }
                 else if ( server.arg(i) == "do0" ) { ch = do0; }      
                 else if ( server.arg(i) == "up1" ) { ch = up1; }
                 else if ( server.arg(i) == "st1" ) { ch = st1; }
                 else if ( server.arg(i) == "do1" ) { ch = do1; }      
                 else if ( server.arg(i) == "up2" ) { ch = up2; }
                 else if ( server.arg(i) == "st2" ) { ch = st2; }
                 else if ( server.arg(i) == "do2" ) { ch = do2; }      
                 else if ( server.arg(i) == "up3" ) { ch = up3; }
                 else if ( server.arg(i) == "st3" ) { ch = st3; }
                 else if ( server.arg(i) == "do3" ) { ch = do3; }      
                 else if ( server.arg(i) == "up4" ) { ch = up4; }
                 else if ( server.arg(i) == "st4" ) { ch = st4; }
                 else if ( server.arg(i) == "do4" ) { ch = do4; }      
                 else if ( server.arg(i) == "up5" ) { ch = up5; }
                 else if ( server.arg(i) == "st5" ) { ch = st5; }
                 else if ( server.arg(i) == "do5" ) { ch = do5; }     
                 else if ( server.arg(i) == "up6" ) { ch = up6; }
                 else if ( server.arg(i) == "st6" ) { ch = st6; }
                 else if ( server.arg(i) == "do6" ) { ch = do6; }      
                 else if ( server.arg(i) == "up7" ) { ch = up7; }
                 else if ( server.arg(i) == "st7" ) { ch = st7; }
                 else if ( server.arg(i) == "do7" ) { ch = do7; }      
                 else if ( server.arg(i) == "up8" ) { ch = up8; }
                 else if ( server.arg(i) == "st8" ) { ch = st8; }
                 else if ( server.arg(i) == "do8" ) { ch = do8; }      
                 else if ( server.arg(i) == "up9" ) { ch = up9; }
                 else if ( server.arg(i) == "st9" ) { ch = st9; }
                 else if ( server.arg(i) == "do9" ) { ch = do9; }
                 else {addLog(LOG_LEVEL_INFO,"Input error"); }      
                 transmit_gate_code(ch);
                 success = true;
               }
               if (success)
                {
                        String url = String(Settings.Name) + "/shutter?s=" + string;
                        addLog(LOG_LEVEL_INFO, "RF Code Sent: " + String(Plugin_112_iCode) + Plugin_112_sCodeWord);
                        addLog(LOG_LEVEL_INFO, "To send this command again, ");
                        addLog(LOG_LEVEL_INFO, "use this: <a href=\"http://" + url + "\">URL</a>");
                        if (printToWeb)
                        {
                                printWebString += F("RCSwitch Code Sent!");
                        }
                }
        }
        break;
        }
        return success;
}


static void transmit_code(const char* code);
static void transmit_gate_code(const char* code);


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

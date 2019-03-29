#ifdef USES_P111

//#######################################################################################################
//###########################         Plugin 111: Aurora Inverter               #########################
//#######################################################################################################
/*
   Version: 2.0
   Description: Arduino based Power Monitor for Power-One / ABB Phototovoltaic Inverters
   Example of usage:
      http://<ESP IP address>/control?cmd=aurora,ask
   Needs: EspEasy
   Author: mtt
   Copyright: (c) 2019-2020 mttstt
   License: MIT
   License URI: http://en.wikipedia.org/wiki/MIT_License
   Status : "Proof of concept"

   PIN Wemos D1 mini successuful tested: GPIO-5(D1)
                                         GPIO-4(D2)
                                         GPIO-2(D4)
   References:
   https://github.com/H4ndl3/pvimon/blob/master/pvimon.ino
   http://www.drhack.it/images/PDF/AuroraCommunicationProtocol_4_2.pdf

   http://www.gianlucaghettini.net/lettura-fotovoltaico-da-remoto-con-nodemcu/
   https://github.com/jrbenito/ABBAurora
   https://forum.arduino.cc/index.php?topic=154407.0
 */

//uncomment one of the following as needed
#include <Arduino.h>
#include <ESPeasySerial.h>
//#include <TimeLib.h>

#define PLUGIN_111
#define PLUGIN_ID_111         111
#define PLUGIN_NAME_111       "Inverter logger (Aurora)"
#define PLUGIN_VALUENAME1_111 "ReadCumulatedEnergy"     //variable output of the plugin. The label is in quotation marks
#define PLUGIN_VALUENAME2_111 "ReadLastFourAlarms"
#define PLUGIN_VALUENAME3_111 "ReadFirmwareRelease"
#define PLUGIN_VALUENAME4_111 "ReadState"
#define PLUGIN_111_DEBUG  false             //set to true for extra log info in the debug

// PIN/port configuration is stored in the following:
// CONFIG_PIN1 - The first GPIO pin selected within the task
// CONFIG_PIN2 - The second GPIO pin selected within the task
// CONFIG_PIN3 - The third GPIO pin selected within the task
// CONFIG_PORT - The port in case the device has multiple in/out pins

int8_t Plugin_111_txPin;  //5 => GPIO5 (D1)

//RS485 control
#define RS485Transmit HIGH
#define RS485Receive LOW
// TX1 at GPIO15 and RX1 at GPIO13 => Serial1.println

class clsAurora {
private:
  int MaxAttempt = 1;
  byte Address = 0;
  void clearData(byte *data, byte len) {
    for (int i = 0; i < len; i++) {
      data[i] = 0;
    }
  }
  int Crc16(byte *data, int offset, int count)
  {
    byte BccLo = 0xFF;
    byte BccHi = 0xFF;
    for (int i = offset; i < (offset + count); i++)
    {
      byte New = data[offset + i] ^ BccLo;
      byte Tmp = New << 4;
      New = Tmp ^ New;
      Tmp = New >> 5;
      BccLo = BccHi;
      BccHi = New ^ Tmp;
      Tmp = New << 3;
      BccLo = BccLo ^ Tmp;
      Tmp = New >> 4;
      BccLo = BccLo ^ Tmp;
    }
    return (int)word(~BccHi, ~BccLo);
  }

  bool Send(byte address, byte param0, byte param1, byte param2, byte param3, byte param4, byte param5, byte param6)
  {
    SendStatus = false;
    ReceiveStatus = false;
    byte SendData[10];
    SendData[0] = address;
    SendData[1] = param0;
    SendData[2] = param1;
    SendData[3] = param2;
    SendData[4] = param3;
    SendData[5] = param4;
    SendData[6] = param5;
    SendData[7] = param6;
    int crc = Crc16(SendData, 0, 8);
    SendData[8] = lowByte(crc);
    SendData[9] = highByte(crc);
    clearReceiveData();

    //=================================================
    String log = "PLUGIN INVERTER AURORA - Send Function:";
    log += SendData[0]; log +=',';
    log += SendData[1]; log +=',';
    log += SendData[2]; log +=',';
    log += SendData[3]; log +=',';
    log += SendData[4]; log +=',';
    log += SendData[5]; log +=',';
    log += SendData[6]; log +=',';
    log += SendData[7]; log +=',';
    log += SendData[8]; log +=',';
    log += SendData[9];
    addLog(LOG_LEVEL_INFO, log);
    Serial.println(log);
   //=================================================

    for (int i = 0; i < MaxAttempt; i++)
    {
      digitalWrite(Plugin_111_txPin, RS485Transmit);
      delay(50);
      if (Serial1.write(SendData, sizeof(SendData)) != 0) {
        Serial1.flush();
        SendStatus = true;
        digitalWrite(Plugin_111_txPin, RS485Receive);
        if (Serial1.readBytes(ReceiveData, sizeof(ReceiveData)) != 0) {
          if ((int)word(ReceiveData[7], ReceiveData[6]) == Crc16(ReceiveData, 0, 6)) {
            ReceiveStatus = true;
            break;
          }
        }
      }
      else { Serial.println("Error while sending data"); digitalWrite(Plugin_111_txPin, RS485Receive); return(false); }
    }
    return ReceiveStatus;
  }

  union {
    byte asBytes[4];
    float asFloat;
  } foo;

  union {
    byte asBytes[4];
    unsigned long asUlong;
  } ulo;

public:
  bool SendStatus = false;
  bool ReceiveStatus = false;
  byte ReceiveData[8];
  clsAurora(byte address) {
    Address = address;
    SendStatus = false;
    ReceiveStatus = false;
    clearReceiveData();
  }
  void clearReceiveData() {
    clearData(ReceiveData, 8);
  }
  String TransmissionState(byte id) {
    switch (id)
    {
    case 0:
      return F("Everything is OK."); break;
    case 51:
      return F("Command is not implemented"); break;
    case 52:
      return F("Variable does not exist"); break;
    case 53:
      return F("Variable value is out of range"); break;
    case 54:
      return F("EEprom not accessible"); break;
    case 55:
      return F("Not Toggled Service Mode"); break;
    case 56:
      return F("Can not send the command to internal micro"); break;
    case 57:
      return F("Command not Executed"); break;
    case 58:
      return F("The variable is not available, retry"); break;
    default:
      return F("Sconosciuto"); break;
    }
  }

  String GlobalState(byte id) {
    switch (id)
    {
    case 0:
      return F("Sending Parameters"); break;
    case 1:
      return F("Wait Sun / Grid"); break;
    case 2:
      return F("Checking Grid"); break;
    case 3:
      return F("Measuring Riso"); break;
    case 4:
      return F("DcDc Start"); break;
    case 5:
      return F("Inverter Start"); break;
    case 6:
      return F("Run"); break;
    case 7:
      return F("Recovery"); break;
    case 8:
      return F("Pausev"); break;
    case 9:
      return F("Ground Fault"); break;
    case 10:
      return F("OTH Fault"); break;
    case 11:
      return F("Address Setting"); break;
    case 12:
      return F("Self Test"); break;
    case 13:
      return F("Self Test Fail"); break;
    case 14:
      return F("Sensor Test + Meas.Riso"); break;
    case 15:
      return F("Leak Fault"); break;
    case 16:
      return F("Waiting for manual reset"); break;
    case 17:
      return F("Internal Error E026"); break;
    case 18:
      return F("Internal Error E027"); break;
    case 19:
      return F("Internal Error E028"); break;
    case 20:
      return F("Internal Error E029"); break;
    case 21:
      return F("Internal Error E030"); break;
    case 22:
      return F("Sending Wind Table"); break;
    case 23:
      return F("Failed Sending table"); break;
    case 24:
      return F("UTH Fault"); break;
    case 25:
      return F("Remote OFF"); break;
    case 26:
      return F("Interlock Fail"); break;
    case 27:
      return F("Executing Autotest"); break;
    case 30:
      return F("Waiting Sun"); break;
    case 31:
      return F("Temperature Fault"); break;
    case 32:
      return F("Fan Staucked"); break;
    case 33:
      return F("Int.Com.Fault"); break;
    case 34:
      return F("Slave Insertion"); break;
    case 35:
      return F("DC Switch Open"); break;
    case 36:
      return F("TRAS Switch Open"); break;
    case 37:
      return F("MASTER Exclusion"); break;
    case 38:
      return F("Auto Exclusion"); break;
    case 98:
      return F("Erasing Internal EEprom"); break;
    case 99:
      return F("Erasing External EEprom"); break;
    case 100:
      return F("Counting EEprom"); break;
    case 101:
      return F("Freeze");
    default:
      return F("Sconosciuto"); break;
    }
  }

  String DcDcState(byte id) {
    switch (id)
    {
    case 0:
      return F("DcDc OFF"); break;
    case 1:
      return F("Ramp Start"); break;
    case 2:
      return F("MPPT"); break;
    case 3:
      return F("Not Used"); break;
    case 4:
      return F("Input OC"); break;
    case 5:
      return F("Input UV"); break;
    case 6:
      return F("Input OV"); break;
    case 7:
      return F("Input Low"); break;
    case 8:
      return F("No Parameters"); break;
    case 9:
      return F("Bulk OV"); break;
    case 10:
      return F("Communication Error"); break;
    case 11:
      return F("Ramp Fail"); break;
    case 12:
      return F("Internal Error"); break;
    case 13:
      return F("Input mode Error"); break;
    case 14:
      return F("Ground Fault"); break;
    case 15:
      return F("Inverter Fail"); break;
    case 16:
      return F("DcDc IGBT Sat"); break;
    case 17:
      return F("DcDc ILEAK Fail"); break;
    case 18:
      return F("DcDc Grid Fail"); break;
    case 19:
      return F("DcDc Comm.Error"); break;
    default:
      return F("Sconosciuto"); break;
    }
  }

  String InverterState(byte id) {
    switch (id)
    {
    case 0:
      return F("Stand By"); break;
    case 1:
      return F("Checking Grid"); break;
    case 2:
      return F("Run"); break;
    case 3:
      return F("Bulk OV"); break;
    case 4:
      return F("Out OC"); break;
    case 5:
      return F("IGBT Sat"); break;
    case 6:
      return F("Bulk UV"); break;
    case 7:
      return F("Degauss Error"); break;
    case 8:
      return F("No Parameters"); break;
    case 9:
      return F("Bulk Low"); break;
    case 10:
      return F("Grid OV"); break;
    case 11:
      return F("Communication Error"); break;
    case 12:
      return F("Degaussing"); break;
    case 13:
      return F("Starting"); break;
    case 14:
      return F("Bulk Cap Fail"); break;
    case 15:
      return F("Leak Fail"); break;
    case 16:
      return F("DcDc Fail"); break;
    case 17:
      return F("Ileak Sensor Fail"); break;
    case 18:
      return F("SelfTest: relay inverter"); break;
    case 19:
      return F("SelfTest : wait for sensor test"); break;
    case 20:
      return F("SelfTest : test relay DcDc + sensor"); break;
    case 21:
      return F("SelfTest : relay inverter fail"); break;
    case 22:
      return F("SelfTest timeout fail"); break;
    case 23:
      return F("SelfTest : relay DcDc fail"); break;
    case 24:
      return F("Self Test 1"); break;
    case 25:
      return F("Waiting self test start"); break;
    case 26:
      return F("Dc Injection"); break;
    case 27:
      return F("Self Test 2"); break;
    case 28:
      return F("Self Test 3"); break;
    case 29:
      return F("Self Test 4"); break;
    case 30:
      return F("Internal Error"); break;
    case 31:
      return F("Internal Error"); break;
    case 40:
      return F("Forbidden State"); break;
    case 41:
      return F("Input UC"); break;
    case 42:
      return F("Zero Power"); break;
    case 43:
      return F("Grid Not Present"); break;
    case 44:
      return F("Waiting Start"); break;
    case 45:
      return F("MPPT"); break;
    case 46:
      return F("Grid Fail"); break;
    case 47:
      return F("Input OC"); break;
    default:
      return F("Sconosciuto"); break;
    }
  }

  String AlarmState(byte id) {
    switch (id)
    {
    case 0:
      return F("No Alarm"); break;
    case 1:
      return F("Sun Low"); break;
    case 2:
      return F("Input OC"); break;
    case 3:
      return F("Input UV"); break;
    case 4:
      return F("Input OV"); break;
    case 5:
      return F("Sun Low"); break;
    case 6:
      return F("No Parameters"); break;
    case 7:
      return F("Bulk OV"); break;
    case 8:
      return F("Comm.Error"); break;
    case 9:
      return F("Output OC"); break;
    case 10:
      return F("IGBT Sat"); break;
    case 11:
      return F("Bulk UV"); break;
    case 12:
      return F("Internal error"); break;
    case 13:
      return F("Grid Fail"); break;
    case 14:
      return F("Bulk Low"); break;
    case 15:
      return F("Ramp Fail"); break;
    case 16:
      return F("Dc / Dc Fail"); break;
    case 17:
      return F("Wrong Mode"); break;
    case 18:
      return F("Ground Fault"); break;
    case 19:
      return F("Over Temp."); break;
    case 20:
      return F("Bulk Cap Fail"); break;
    case 21:
      return F("Inverter Fail"); break;
    case 22:
      return F("Start Timeout"); break;
    case 23:
      return F("Ground Fault"); break;
    case 24:
      return F("Degauss error"); break;
    case 25:
      return F("Ileak sens.fail"); break;
    case 26:
      return F("DcDc Fail"); break;
    case 27:
      return F("Self Test Error 1"); break;
    case 28:
      return F("Self Test Error 2"); break;
    case 29:
      return F("Self Test Error 3"); break;
    case 30:
      return F("Self Test Error 4"); break;
    case 31:
      return F("DC inj error"); break;
    case 32:
      return F("Grid OV"); break;
    case 33:
      return F("Grid UV"); break;
    case 34:
      return F("Grid OF"); break;
    case 35:
      return F("Grid UF"); break;
    case 36:
      return F("Z grid Hi"); break;
    case 37:
      return F("Internal error"); break;
    case 38:
      return F("Riso Low"); break;
    case 39:
      return F("Vref Error"); break;
    case 40:
      return F("Error Meas V"); break;
    case 41:
      return F("Error Meas F"); break;
    case 42:
      return F("Error Meas Z"); break;
    case 43:
      return F("Error Meas Ileak"); break;
    case 44:
      return F("Error Read V"); break;
    case 45:
      return F("Error Read I"); break;
    case 46:
      return F("Table fail"); break;
    case 47:
      return F("Fan Fail"); break;
    case 48:
      return F("UTH"); break;
    case 49:
      return F("Interlock fail"); break;
    case 50:
      return F("Remote Off"); break;
    case 51:
      return F("Vout Avg errror"); break;
    case 52:
      return F("Battery low"); break;
    case 53:
      return F("Clk fail"); break;
    case 54:
      return F("Input UC"); break;
    case 55:
      return F("Zero Power"); break;
    case 56:
      return F("Fan Stucked"); break;
    case 57:
      return F("DC Switch Open"); break;
    case 58:
      return F("Tras Switch Open"); break;
    case 59:
      return F("AC Switch Open"); break;
    case 60:
      return F("Bulk UV"); break;
    case 61:
      return F("Autoexclusion"); break;
    case 62:
      return F("Grid df / dt"); break;
    case 63:
      return F("Den switch Open"); break;
    case 64:
      return F("Jbox fail"); break;
    default:
      return F("Sconosciuto"); break;
    }
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    byte InverterState;
    byte Channel1State;
    byte Channel2State;
    byte AlarmState;
    bool ReadState;
  } DataState;

  DataState State;

  bool ReadState() {
    State.ReadState = Send(Address, (byte)50, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (State.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
      ReceiveData[2] = 255;
      ReceiveData[3] = 255;
      ReceiveData[4] = 255;
      ReceiveData[5] = 255;
    }
    State.TransmissionState = ReceiveData[0];
    State.GlobalState = ReceiveData[1];
    State.InverterState = ReceiveData[2];
    State.Channel1State = ReceiveData[3];
    State.Channel2State = ReceiveData[4];
    State.AlarmState = ReceiveData[5];
    return State.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Par1;
    String Par2;
    String Par3;
    String Par4;
    bool ReadState;
  } DataVersion;

  DataVersion Version;

  bool ReadVersion() {
    Version.ReadState = Send(Address, (byte)58, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (Version.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    Version.TransmissionState = ReceiveData[0];
    Version.GlobalState = ReceiveData[1];
    /*
    switch ((char)ReceiveData[2])
    {
    case 'i':
      Version.Par1 = F("Aurora 2 kW indoor"); break;
    case 'o':
      Version.Par1 = F("Aurora 2 kW outdoor"); break;
    case 'I':
      Version.Par1 = F("Aurora 3.6 kW indoor"); break;
    case 'O':
      Version.Par1 = F("Aurora 3.0 - 3.6 kW outdoor"); break;
    case '5':
      Version.Par1 = F("Aurora 5.0 kW outdoor"); break;
    case '6':
      Version.Par1 = F("Aurora 6 kW outdoor"); break;
    case 'P':
      Version.Par1 = F("3 - phase interface (3G74)"); break;
    case 'C':
      Version.Par1 = F("Aurora 50kW module"); break;
    case '4':
      Version.Par1 = F("Aurora 4.2kW new"); break;
    case '3':
      Version.Par1 = F("Aurora 3.6kW new"); break;
    case '2':
      Version.Par1 = F("Aurora 3.3kW new"); break;
    case '1':
      Version.Par1 = F("Aurora 3.0kW new"); break;
    case 'D':
      Version.Par1 = F("Aurora 12.0kW"); break;
    case 'X':
      Version.Par1 = F("Aurora 10.0kW"); break;
    default:
      Version.Par1 = F("Sconosciuto"); break;
    }

    switch ((char)ReceiveData[3])
    {
    case 'A':
      Version.Par2 = F("UL1741"); break;
    case 'E':
      Version.Par2 = F("VDE0126"); break;
    case 'S':
      Version.Par2 = F("DR 1663 / 2000"); break;
    case 'I':
      Version.Par2 = F("ENEL DK 5950"); break;
    case 'U':
      Version.Par2 = F("UK G83"); break;
    case 'K':
      Version.Par2 = F("AS 4777"); break;
    default:
      Version.Par2 = F("Sconosciuto"); break;
    }
    switch ((char)ReceiveData[4])
    {
    case 'N':
      Version.Par3 = F("Transformerless Version"); break;
    case 'T':
      Version.Par3 = F("Transformer Version"); break;
    default:
      Version.Par3 = F("Sconosciuto"); break;
    }
    switch ((char)ReceiveData[5])
    {
    case 'W':
      Version.Par4 = F("Wind version"); break;
    case 'N':
      Version.Par4 = F("PV version"); break;
    default:
      Version.Par4 = F("Sconosciuto"); break;
    }
    */
    return Version.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    float Valore;
    bool ReadState;
  } DataDSP;

  DataDSP DSP;

   /* type =
      1 Grid Voltage* For three-phases systems is the mean
      2 Grid Current* For three-phases systems is the mean
      3 Grid Power* For three-phases systems is the mean
      4 Frequency For three-phases systems is the mean
      5 Vbulk For Inverter with more Bulk is the sum
      6 Ileak (Dc/Dc)
      7 Ileak (Inverter)
      8 Pin1* Pin for single channel module
      9 Pin2
      21 Inverter Temperature
      22 Booster Temperature
      23 Input 1 Voltage Input Voltage for single channel module
      24 - - -
      25 Input 1 Current* Input Current for single channel module
      26 Input 2 Voltage
      27 Input 2 Current
      28 Grid Voltage (Dc/Dc)
      29 Grid Frequency (Dc/Dc)
      30 Isolation Resistance (Riso)
      31 Vbulk (Dc/Dc)
      32 Average Grid Voltage (VgridAvg)
      33 VbulkMid
      34 Power Peak
      35 Power Peak Today
      36 Grid Voltage neutral
      37 Wind Generator Frequency
      38 Grid Voltage neutral-phase
      39 Grid Current phase r
      40 Grid Current phase s
      41 Grid Current phase t
      42 Frequency phase r
      43 Frequency phase s
      44 Frequency phase t
      45 Vbulk +
      46 Vbulk -
      47 Supervisor Temperature
      48 Alim. Temperature
      49 Heat Sink Temperature
      50 Temperature 1
      51 Temperature 2
      52 Temperature 3
      53 Fan 1 Speed
      54 Fan 2 Speed
      55 Fan 3 Speed
      56 Fan 4 Speed
      57 Fan 5 Speed
      58 Power Saturation limit (Der.)
      59 Riferimento Anello Bulk
      60 Vpanel micro
      61 Grid Voltage phase r
      62 Grid Voltage phase s
      63 Grid Voltage phase t

      Global=
      if 1 requires the Global Measurements (Only For a Master)
      if 0 requires the Module Measurements (Master and Slave)
  */

  bool ReadDSP(byte type, byte global) {
    if ((((int)type >= 1 && (int)type <= 9) || ((int)type >= 21 && (int)type <= 63)) && ((int)global >= 0 && (int)global <= 1)) {
      DSP.ReadState = Send(Address, (byte)59, type, global, (byte)0, (byte)0, (byte)0, (byte)0);
      if (DSP.ReadState == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
    }
    else {
      DSP.ReadState = false;
      clearReceiveData();
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    DSP.TransmissionState = ReceiveData[0];
    DSP.GlobalState = ReceiveData[1];
    foo.asBytes[0] = ReceiveData[5];
    foo.asBytes[1] = ReceiveData[4];
    foo.asBytes[2] = ReceiveData[3];
    foo.asBytes[3] = ReceiveData[2];
    DSP.Valore = foo.asFloat;
    return DSP.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    unsigned long Secondi;
    bool ReadState;
  } DataTimeDate;

  DataTimeDate TimeDate;

  bool ReadTimeDate() {
    TimeDate.ReadState = Send(Address, (byte)70, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (TimeDate.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    TimeDate.TransmissionState = ReceiveData[0];
    TimeDate.GlobalState = ReceiveData[1];
    TimeDate.Secondi = ((unsigned long)ReceiveData[2] << 24) + ((unsigned long)ReceiveData[3] << 16) + ((unsigned long)ReceiveData[4] << 8) + (unsigned long)ReceiveData[5];
    return TimeDate.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    byte Alarms1;
    byte Alarms2;
    byte Alarms3;
    byte Alarms4;
    bool ReadState;
  } DataLastFourAlarms;

  DataLastFourAlarms LastFourAlarms;

  bool ReadLastFourAlarms() {
    LastFourAlarms.ReadState = Send(Address, (byte)86, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (LastFourAlarms.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
      ReceiveData[2] = 255;
      ReceiveData[3] = 255;
      ReceiveData[4] = 255;
      ReceiveData[5] = 255;
    }
    LastFourAlarms.TransmissionState = ReceiveData[0];
    LastFourAlarms.GlobalState = ReceiveData[1];
    LastFourAlarms.Alarms1 = ReceiveData[2];
    LastFourAlarms.Alarms2 = ReceiveData[3];
    LastFourAlarms.Alarms3 = ReceiveData[4];
    LastFourAlarms.Alarms4 = ReceiveData[5];
    return LastFourAlarms.ReadState;
  }

  bool ReadJunctionBoxState(byte nj) {
    return Send(Address, (byte)200, nj, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
  }

  bool ReadJunctionBoxVal(byte nj, byte par) {
    return Send(Address, (byte)201, nj, par, (byte)0, (byte)0, (byte)0, (byte)0);
  }

  // Inverters
  typedef struct {
    String PN;
    bool ReadState;
  } DataSystemPN;

  DataSystemPN SystemPN;

  bool ReadSystemPN() {
    SystemPN.ReadState = Send(Address, (byte)52, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    SystemPN.PN = String(String((char)ReceiveData[0]) + String((char)ReceiveData[1]) + String((char)ReceiveData[2]) + String((char)ReceiveData[3]) + String((char)ReceiveData[4]) + String((char)ReceiveData[5]));
    return SystemPN.ReadState;
  }

  typedef struct {
    String SerialNumber;
    bool ReadState;
  } DataSystemSerialNumber;

  DataSystemSerialNumber SystemSerialNumber;

  bool ReadSystemSerialNumber() {
    SystemSerialNumber.ReadState = Send(Address, (byte)63, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    SystemSerialNumber.SerialNumber = String(String((char)ReceiveData[0]) + String((char)ReceiveData[1]) + String((char)ReceiveData[2]) + String((char)ReceiveData[3]) + String((char)ReceiveData[4]) + String((char)ReceiveData[5]));
    return SystemSerialNumber.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Week;
    String Year;
    bool ReadState;
  } DataManufacturingWeekYear;

  DataManufacturingWeekYear ManufacturingWeekYear;

  bool ReadManufacturingWeekYear() {
    ManufacturingWeekYear.ReadState = Send(Address, (byte)65, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (ManufacturingWeekYear.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    ManufacturingWeekYear.TransmissionState = ReceiveData[0];
    ManufacturingWeekYear.GlobalState = ReceiveData[1];
    ManufacturingWeekYear.Week = String(String((char)ReceiveData[2]) + String((char)ReceiveData[3]));
    ManufacturingWeekYear.Year = String(String((char)ReceiveData[4]) + String((char)ReceiveData[5]));
    return ManufacturingWeekYear.ReadState;
  }
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Release;
    bool ReadState;
  } DataFirmwareRelease;

  DataFirmwareRelease FirmwareRelease;

  bool ReadFirmwareRelease() {
    FirmwareRelease.ReadState = Send(Address, (byte)72, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (FirmwareRelease.ReadState == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    FirmwareRelease.TransmissionState = ReceiveData[0];
    FirmwareRelease.GlobalState = ReceiveData[1];
    FirmwareRelease.Release = String(String((char)ReceiveData[2]) + "." + String((char)ReceiveData[3]) + "." + String((char)ReceiveData[4]) + "." + String((char)ReceiveData[5]));
    return FirmwareRelease.ReadState;
  }

  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    unsigned long Energia;
    bool ReadState;
  } DataCumulatedEnergy;

  DataCumulatedEnergy CumulatedEnergy;

  /*  par=
      0) Daily Energy
      1) Weekly Energy
      2) Not used
      3) Month Energy (Energy from the first day of current calendar month)
      4) Year Energy (Energy from the first day of current calendar year)
      5) Total Energy (total lifetime)
      6) Partial Energy (cumulated since reset)
  */
  bool ReadCumulatedEnergy(byte par) {
    if ((int)par >= 0 && (int)par <= 6) {
      CumulatedEnergy.ReadState = Send(Address, (byte)78, par, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
      if (CumulatedEnergy.ReadState == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
    }
    else {
      CumulatedEnergy.ReadState = false;
      clearReceiveData();
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    CumulatedEnergy.TransmissionState = ReceiveData[0];
    CumulatedEnergy.GlobalState = ReceiveData[1];
    if (CumulatedEnergy.ReadState == true) {
            ulo.asBytes[0] = ReceiveData[5];
            ulo.asBytes[1] = ReceiveData[4];
            ulo.asBytes[2] = ReceiveData[3];
            ulo.asBytes[3] = ReceiveData[2];
            CumulatedEnergy.Energia = ulo.asUlong;
    }
    return CumulatedEnergy.ReadState;
  }

  bool WriteBaudRateSetting(byte baudcode) {
    if ((int)baudcode >= 0 && (int)baudcode <= 3) {
      return Send(Address, (byte)85, baudcode, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    }
    else {
      clearReceiveData();
      return false;
    }
  }

  // Central
  bool ReadFlagsSwitchCentral()  { return Send(Address, (byte)67, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadCumulatedEnergyCentral(byte var, byte ndays_h, byte ndays_l, byte global) { return Send(Address, (byte)68, var, ndays_h, ndays_l, global, (byte)0, (byte)0); }
  bool ReadFirmwareReleaseCentral(byte var) { return Send(Address, (byte)72, var, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadBaudRateSettingCentral(byte baudcode, byte serialline) { return Send(Address, (byte)85, baudcode, serialline, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadSystemInfoCentral(byte var) { return Send(Address, (byte)101, var, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadJunctionBoxMonitoringCentral(byte cf, byte rn, byte njt, byte jal, byte jah) {return Send(Address, (byte)103, cf, rn, njt, jal, jah, (byte)0); }
  bool ReadSystemPNCentral() { return Send(Address, (byte)105, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadSystemSerialNumberCentral() { return Send(Address, (byte)107, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
};

/*
String stampaDataTime(unsigned long scn)
{
  String rtn;
  if (scn > 0) {
    setTime(0, 0, 0, 1, 1, 2000);
    if (timeStatus() == timeSet) {
    adjustTime(scn);
      rtn = String(day());
      rtn += String(F("/"));
      rtn += String(month());
      rtn += String(F("/"));
      rtn += String(year());
      rtn += String(F(" "));
      rtn += String(hour());
      rtn += String(F(":"));
      rtn += String(minute());
      rtn += String(F(":"));
      rtn += String(second());
    }
  }
  return rtn;
}
*/

// ==============================================
// ID inverter da cambiare a mano !!!
 clsAurora Inverter = clsAurora(1); // 
// ============================================== 

boolean Plugin_111(byte function, struct EventStruct *event, String& string)
{
        boolean success = false;
        switch (function)
        {
        case PLUGIN_DEVICE_ADD:
        {
                Device[++deviceCount].Number = PLUGIN_ID_111;
                Device[deviceCount].Type = DEVICE_TYPE_SINGLE; //how the device is connected
                Device[deviceCount].SendDataOption = false;
                Device[deviceCount].Ports = 0;
                Device[deviceCount].VType = SENSOR_TYPE_SWITCH; //type of value the plugin will return, used only for Domoticz
                Device[deviceCount].PullUpOption = false;
                Device[deviceCount].InverseLogicOption = false;
                Device[deviceCount].FormulaOption = false;
                Device[deviceCount].ValueCount = 9;  //number of output variables. The value should match the number of keys PLUGIN_VALUENAME1_xxx
                Device[deviceCount].TimerOption = false;
                Device[deviceCount].TimerOptional = false;
                Device[deviceCount].GlobalSyncOption = true;
                Device[deviceCount].DecimalsOnly = true;
                break;
        }

        case PLUGIN_GET_DEVICENAME:
        {
           string = F(PLUGIN_NAME_111);
           break;
        }

        case PLUGIN_GET_DEVICEVALUENAMES:
        {
           //called when the user opens the module configuration page
           //it allows to add a new row for each output variable of the plugin
           
           strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_111));           
           strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_111));
           strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_111));
           strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[3], PSTR(PLUGIN_VALUENAME4_111));

           break;
        }

        case PLUGIN_WEBFORM_LOAD:
        {
           addFormNumericBox(F("PVI Address (AT THE MOMENT IS NOT USED"), F("plugin_111_pviaddr"), PCONFIG(0),1,255);

           //after the form has been loaded, set success and break
           success = true;
           break;
        }

        case PLUGIN_WEBFORM_SAVE:
        {
           //this case defines the code to be executed when the form is submitted
           //the plugin settings should be saved to PCONFIG(x)
           //ping configuration should be read from CONFIG_PIN1 and stored

           PCONFIG(0) = getFormItemInt(F("plugin_111_pviaddr"));

           //after the form has been saved successfuly, set success and break
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
           // this case defines code to be executed when the plugin is initialised
           //clsAurora Inverter = clsAurora( PCONFIG(0) );  //verificare l'id dell'Inverter !!!

           Plugin_111_txPin = CONFIG_PIN1;

           Serial.print("Plugin_111_txPin: "); Serial.println(Plugin_111_txPin);
           addLog(LOG_LEVEL_INFO, "Plugin_111_txPin: "); addLog(LOG_LEVEL_INFO, Plugin_111_txPin);

           if ( Plugin_111_txPin != -1)
           {
               addLog(LOG_LEVEL_INFO, "INIT: Aurora 123 Inverter created!");
               // Serial.begin(9600);
               Serial1.setTimeout(500);
               Serial1.begin(19200);  // initialize serial connection to the inverter
               pinMode( Plugin_111_txPin, OUTPUT);
               // pinMode(rxPin, INPUT);  // set pin modes
               // pinMode(txPin, OUTPUT);
               // pinMode(rtsPin, OUTPUT);
               // digitalWrite( Plugin_111_txPin, RS485Receive);  // Init Transceiver
           }

           if ( Plugin_111_txPin == -1)
           {
               addLog(LOG_LEVEL_INFO, "INIT: Aurora 123 Inverter removed!");
               pinMode(Plugin_111_txPin, INPUT);
           }
           //after the plugin has been initialised successfuly, set success and break
           success = true;
           break;
        }

        case PLUGIN_READ:
        {
           //code to be executed to read data
           //It is executed according to the delay configured on the device configuration page, only once
           //after the plugin has read data successfuly, set success and break

           UserVar[event->BaseVarIndex + 0] = Inverter.ReadCumulatedEnergy(0);
           UserVar[event->BaseVarIndex + 1] = Inverter.ReadTimeDate();          
           UserVar[event->BaseVarIndex + 3] = Inverter.ReadSystemPN();
           UserVar[event->BaseVarIndex + 4] = Inverter.ReadSystemSerialNumber();
           UserVar[event->BaseVarIndex + 5] = Inverter.ReadManufacturingWeekYear();
           UserVar[event->BaseVarIndex + 6] = Inverter.ReadFirmwareRelease();
           UserVar[event->BaseVarIndex + 7] = Inverter.ReadVersion();
           UserVar[event->BaseVarIndex + 8] = Inverter.ReadState();

           String log = F("INVERTER LOGGER: ");
           log += UserVar[event->BaseVarIndex + 0]; log +=',';
           log += UserVar[event->BaseVarIndex + 1]; log +=',';
           log += UserVar[event->BaseVarIndex + 2]; log +=',';
           log += UserVar[event->BaseVarIndex + 3]; log +=',';
           log += UserVar[event->BaseVarIndex + 4]; log +=',';
           log += UserVar[event->BaseVarIndex + 5]; log +=',';
           log += UserVar[event->BaseVarIndex + 6]; log +=',';
           log += UserVar[event->BaseVarIndex + 7]; log +=',';
           log += UserVar[event->BaseVarIndex + 8];
           addLog(LOG_LEVEL_INFO,log);
           Serial.println(log);
           
           success = true;
           break;
        }

        case PLUGIN_WRITE:
        {
           //this case defines code to be executed when the plugin executes an action (command).
           //Commands can be accessed via rules or via http.
           //As an example, http://192.168.1.12//control?cmd=dothis
           //implies that there exists the command "dothis"

           //parse string to extract the command
           String TmpStr1;
           String rfType;
           String command = parseString(string, 1);
           if (command == F("aurora")) {
             String taskName = parseString(string, 2);
             //int8_t taskIndex = getTaskIndexByName(taskName);
             if ( GetArgv(string.c_str(), TmpStr1, 2) ) rfType = TmpStr1.c_str();
             addLog(LOG_LEVEL_INFO, "HTTP command: aurora ");
             addLog(LOG_LEVEL_INFO, rfType); Serial.println(rfType);
             if ( rfType.equalsIgnoreCase("ask") ) {
                read_RS485();
                success = true;  //set to true only if plugin has executed a command successfully
             }
           }

           if (success){
                 //String url = String(Settings.Name) + "/control?aurora=" + string;
                 String url = String(WiFi.localIP().toString()) + "/control?cmd=aurora,ask";
                 addLog(LOG_LEVEL_INFO, "To send this command again, ");
                 addLog(LOG_LEVEL_INFO, "use this: <a href=\"http://" + url + "\">URL</a>");
                 if (printToWeb)
                    {
                      printWebString += F("Invert command Aurora Sent!");
                      printWebString += F("<BR>PVI Address: ");
                      printWebString += String( PCONFIG(0) );
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
//==========================================================================
//==========================================================================

void read_RS485(){
   
  String log = F("INVERTER LOGGER: ");
  log += Inverter.ReadCumulatedEnergy(0); log +=',';
  log += Inverter.ReadTimeDate(); log +=',';
  log += Inverter.ReadLastFourAlarms(); log +=',';
  log += Inverter.ReadSystemPN(); log +=',';
  log += Inverter.ReadSystemSerialNumber(); log +=',';
  log += Inverter.ReadManufacturingWeekYear(); log +=',';
  log += Inverter.ReadVersion(); log +=',';
  log += Inverter.ReadState(); log +=',';
  log += Inverter.ReadDSP(50,1); log +=',';
  addLog(LOG_LEVEL_INFO,log);
  Serial.println(log);

/*
  String log = F("INVERTER LOGGER: ");
  log += Inverter.CumulatedEnergy.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.CumulatedEnergy.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.CumulatedEnergy.GlobalState); log +=',';
  log += Inverter.CumulatedEnergy.Energia; log +=',';

  log += Inverter.ReadTimeDate.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.TimeDate.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.TimeDate.GlobalState); log +=',';
  log += Inverter.TimeDate.Secondi; log +=',';
  log += stampaDataTime(Inverter.TimeDate.Secondi);log +=',';

  log += Inverter.LastFourAlarms.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.LastFourAlarms.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.LastFourAlarms.GlobalState); log +=',';
  log += Inverter.AlarmState(Inverter.LastFourAlarms.Alarms1); log +=',';
  log += Inverter.AlarmState(Inverter.LastFourAlarms.Alarms2); log +=',';
  log += Inverter.AlarmState(Inverter.LastFourAlarms.Alarms3); log +=',';
  log += Inverter.AlarmState(Inverter.LastFourAlarms.Alarms4); log +=',';

  log += Inverter.ReadSystemPN.ReadState; log +=',';
  log += Inverter.ReadSystemPN.PN; log +=',';
  log += Inverter.ReadSystemSerialNumber.ReadState; log +=',';
  log += Inverter.ReadSystemSerialNumber.SerialNumber; log +=',';

  log += Inverter.ReadManufacturingWeekYear.ReadState; log +=',';
  log += Inverter.ReadManufacturingWeekYear.Week; log +=',';
  log += Inverter.ReadManufacturingWeekYear.Year; log +=',';

  log += Inverter.ReadFirmwareRelease.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.FirmwareRelease.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.FirmwareRelease.GlobalState); log +=',';
  log += Inverter.ReadFirmwareRelease.Release +=',';

  log += Inverter.Version.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.Version.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.Version.GlobalState); log +=',';
  log += Inverter.Version.Par1; log +=',';

  log += Inverter.State.ReadState; log +=',';
  log += Inverter.TransmissionState(Inverter.State.TransmissionState); log +=',';
  log += Inverter.GlobalState(Inverter.State.GlobalState); log +=',';
  log += Inverter.InverterState(Inverter.State.InverterState); log +=',';
  log += Inverter.DcDcState(Inverter.State.Channel1State); log +=',';
  log += Inverter.DcDcState(Inverter.State.Channel2State); log +=',';
  log += Inverter.AlarmState(Inverter.State.AlarmState); log +=',';

  log += Inverter.ReadDSP.ReadState;

  addLog(LOG_LEVEL_INFO,log);
  Serial.println(log);
*/
  delay(50);
}

#endif

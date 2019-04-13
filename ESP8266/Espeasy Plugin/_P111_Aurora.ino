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

   PIN successuful tested:

         RS485    Wemos D1 mini
         Vcc      3v3
         Gnd      G
         RO       TX (GPIO-1)
         RI       RX (GPIO-3)
         RE/DE    D0 (GPIO-16)

   References:
   https://github.com/H4ndl3/pvimon/blob/master/pvimon.ino
   http://www.drhack.it/images/PDF/AuroraCommunicationProtocol_4_2.pdf
   http://www.gianlucaghettini.net/lettura-fotovoltaico-da-remoto-con-nodemcu/
   https://github.com/jrbenito/ABBAurora
   https://forum.arduino.cc/index.php?topic=154407.0
 */

//uncomment one of the following as needed
//#include <TimeLib.h>
#include <ESPeasySerial.h>

#define PLUGIN_111
#define PLUGIN_ID_111         111
#define PLUGIN_NAME_111       "Inverter logger (Aurora)"
#define PLUGIN_VALUENAME1_111 "DailyEnergy"     //variable output of the plugin. The label is in quotation marks
#define PLUGIN_VALUENAME2_111 "MonthEnergy"
#define PLUGIN_VALUENAME3_111 "YearEnergy"
#define PLUGIN_111_DEBUG  false             //set to true for extra log info in the debug

// PIN/port configuration is stored in the following:
// CONFIG_PIN1 - The first GPIO pin selected within the task
// CONFIG_PIN2 - The second GPIO pin selected within the task
// CONFIG_PIN3 - The third GPIO pin selected within the task
// CONFIG_PORT - The port in case the device has multiple in/out pins


//RS485 control
#define RS485Transmit HIGH
#define RS485Receive LOW
#define TX 1 // GPIO-1 (TX)
#define RX 3 // GPIO-3 (RX)
#define baudrate 19200 //baudrate RS485
#define SSerialTxControl D0 //GPIO-16 (D0)


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
    String log = "AURORA - Send data:";
    log += SendData[0]; log +=',';
    log += SendData[1]; log +=',';
    log += SendData[2]; log +=',';
    log += SendData[3]; log +=',';
    log += SendData[4]; log +=',';
    log += SendData[5]; log +=',';
    log += SendData[6]; log +=',';
    log += SendData[7]; log +=',';
    log += SendData[8]; log +=',';
    log += SendData[9]; log +='-';
    //addLog(LOG_LEVEL_INFO,log);
   //=================================================
    for (int i = 0; i < MaxAttempt; i++)
    {
      digitalWrite(SSerialTxControl, RS485Transmit);
      delay(50);
      if (Serial.write(SendData, sizeof(SendData)) != 0) {
        Serial.flush();
        SendStatus = true;
        digitalWrite(SSerialTxControl, RS485Receive);
        int rec = Serial.readBytes(ReceiveData, sizeof(ReceiveData));
        String log = "AURORA - Received data: ";
        log += ReceiveData[0]; log +=',';
        log += ReceiveData[1]; log +=',';
        log += ReceiveData[2]; log +=',';
        log += ReceiveData[3]; log +=',';
        log += ReceiveData[4]; log +=',';
        log += ReceiveData[5]; log +=',';
        log += ReceiveData[6]; log +=',';
        log += ReceiveData[7]; log +='-';
        log += rec;
        //addLog(LOG_LEVEL_INFO,log);
        if (rec != 0) {
          if ((int)word(ReceiveData[7], ReceiveData[6]) == Crc16(ReceiveData, 0, 6)) {
            ReceiveStatus = true;
            break;
          }
        }
      }
      else {
            // addLog(LOG_LEVEL_INFO,"Error while sending data");
            digitalWrite(SSerialTxControl, RS485Receive); return(false);
           }
    }
    String log1 = "ReceiveStatus:"; log1 +=',';
    log1 += ReceiveStatus;
    //addLog(LOG_LEVEL_INFO,log1);
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


  typedef struct {
    String TransmissionState;
    String GlobalState;
    String InverterState;
    byte Channel1State;
    byte Channel2State;
    String AlarmState;
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
    //State.TransmissionState = ReceiveData[0];
    //State.GlobalState = ReceiveData[1];
    //State.InverterState = ReceiveData[2];
    State.Channel1State = ReceiveData[3];
    State.Channel2State = ReceiveData[4];
    //State.AlarmState = ReceiveData[5]

      switch (ReceiveData[0])
      {
      case 0:
        State.TransmissionState = "Everything is OK."; break;
      case 51:
        State.TransmissionState = "Command is not implemented"; break;
      case 52:
        State.TransmissionState = "Variable does not exist"; break;
      case 53:
        State.TransmissionState = "Variable value is out of range"; break;
      case 54:
        State.TransmissionState = "EEprom not accessible"; break;
      case 55:
        State.TransmissionState = "Not Toggled Service Mode"; break;
      case 56:
        State.TransmissionState = "Can not send the command to internal micro"; break;
      case 57:
        State.TransmissionState = "Command not Executed"; break;
      case 58:
        State.TransmissionState = "The variable is not available, retry"; break;
      default:
        State.TransmissionState = "Sconosciuto"; break;
      }


        switch (ReceiveData[1])
        {
        case 0:
          State.GlobalState = "Sending Parameters"; break;
        case 1:
          State.GlobalState = "Wait Sun / Grid"; break;
        case 2:
          State.GlobalState = "Checking Grid"; break;
        case 3:
          State.GlobalState = "Measuring Riso"; break;
        case 4:
          State.GlobalState = "DcDc Start"; break;
        case 5:
          State.GlobalState = "Inverter Start"; break;
        case 6:
          State.GlobalState = "Run"; break;
        case 7:
          State.GlobalState = "Recovery"; break;
        case 8:
          State.GlobalState = "Pausev"; break;
        case 9:
          State.GlobalState = "Ground Fault"; break;
        case 10:
          State.GlobalState = "OTH Fault"; break;
        case 11:
          State.GlobalState = "Address Setting"; break;
        case 12:
          State.GlobalState = "Self Test"; break;
        case 13:
          State.GlobalState = "Self Test Fail"; break;
        case 14:
          State.GlobalState = "Sensor Test + Meas.Riso"; break;
        case 15:
          State.GlobalState = "Leak Fault"; break;
        case 16:
          State.GlobalState = "Waiting for manual reset"; break;
        case 17:
          State.GlobalState = "Internal Error E026"; break;
        case 18:
          State.GlobalState = "Internal Error E027"; break;
        case 19:
          State.GlobalState = "Internal Error E028"; break;
        case 20:
          State.GlobalState = "Internal Error E029"; break;
        case 21:
          State.GlobalState = "Internal Error E030"; break;
        case 22:
          State.GlobalState = "Sending Wind Table"; break;
        case 23:
          State.GlobalState = "Failed Sending table"; break;
        case 24:
          State.GlobalState = "UTH Fault"; break;
        case 25:
          State.GlobalState = "Remote OFF"; break;
        case 26:
          State.GlobalState = "Interlock Fail"; break;
        case 27:
          State.GlobalState = "Executing Autotest"; break;
        case 30:
          State.GlobalState = "Waiting Sun"; break;
        case 31:
          State.GlobalState = "Temperature Fault"; break;
        case 32:
          State.GlobalState = "Fan Staucked"; break;
        case 33:
          State.GlobalState = "Int.Com.Fault"; break;
        case 34:
          State.GlobalState = "Slave Insertion"; break;
        case 35:
          State.GlobalState = "DC Switch Open"; break;
        case 36:
          State.GlobalState = "TRAS Switch Open"; break;
        case 37:
          State.GlobalState = "MASTER Exclusion"; break;
        case 38:
          State.GlobalState = "Auto Exclusion"; break;
        case 98:
          State.GlobalState = "Erasing Internal EEprom"; break;
        case 99:
          State.GlobalState = "Erasing External EEprom"; break;
        case 100:
          State.GlobalState = "Counting EEprom"; break;
        case 101:
          State.GlobalState = "Freeze"; break;
        default:
          State.GlobalState = "Sconosciuto"; break;
        }

        switch (ReceiveData[2])
          {
          case 0:
            State.InverterState = "Stand By"; break;
          case 1:
            State.InverterState = "Checking Grid"; break;
          case 2:
            State.InverterState = "Run"; break;
          case 3:
            State.InverterState = "Bulk OV "; break;
          case 4:
            State.InverterState = "Out OC"; break;
          case 5:
            State.InverterState = "IGBT Sat"; break;
          case 6:
            State.InverterState = "Bulk UV"; break;
          case 7:
            State.InverterState = "Degauss Error"; break;
          case 8:
            State.InverterState = "No Parameters"; break;
          case 9:
            State.InverterState = "Bulk Low"; break;
          case 10:
            State.InverterState = "Grid OV"; break;
          case 11:
            State.InverterState = "Communication Error"; break;
          case 12:
            State.InverterState = "Degaussing"; break;
          case 13:
            State.InverterState = "Starting"; break;
          case 14:
            State.InverterState = "Bulk Cap Fail"; break;
          case 15:
            State.InverterState = "Leak Fail"; break;
          case 16:
            State.InverterState = "DcDc Fail"; break;
          case 17:
            State.InverterState = "Ileak Sensor Fail"; break;
          case 18:
            State.InverterState = "SelfTest: relay inverter"; break;
          case 19:
            State.InverterState = "SelfTest : wait for sensor test"; break;
          case 20:
            State.InverterState = "SelfTest : test relay DcDc + sensor"; break;
          case 21:
            State.InverterState = "SelfTest : relay inverter fail"; break;
          case 22:
            State.InverterState = "SelfTest timeout fail"; break;
          case 23:
            State.InverterState = "SelfTest : relay DcDc fail"; break;
          case 24:
            State.InverterState = "Self Test 1"; break;
          case 25:
            State.InverterState = "Waiting self test start"; break;
          case 26:
            State.InverterState = "Dc Injection"; break;
          case 27:
            State.InverterState = "Self Test 2"; break;
          case 28:
            State.InverterState = "Self Test 3"; break;
          case 29:
            State.InverterState = "Self Test 4"; break;
          case 30:
            State.InverterState = "Internal Error"; break;
          case 31:
            State.InverterState = "Internal Error"; break;
          case 40:
            State.InverterState = "Forbidden State"; break;
          case 41:
            State.InverterState = "Input UC"; break;
          case 42:
            State.InverterState = "Zero Power"; break;
          case 43:
            State.InverterState = "Grid Not Present"; break;
          case 44:
            State.InverterState = "Waiting Start"; break;
          case 45:
            State.InverterState = "MPPT"; break;
          case 46:
            State.InverterState = "Grid Fail"; break;
          case 47:
            State.InverterState = "Input OC"; break;
          default:
            State.InverterState = "Sconosciuto"; break;
          }

          switch (ReceiveData[5])
            {
            case 0:
              State.AlarmState = "No Alarm"; break;
            case 1:
              State.AlarmState = "Sun Low"; break;
            case 2:
              State.AlarmState = "Input OC"; break;
            case 3:
              State.AlarmState = "Input UV"; break;
            case 4:
              State.AlarmState = "Input OV"; break;
            case 5:
              State.AlarmState = "Sun Low"; break;
            case 6:
              State.AlarmState = "No Parameters"; break;
            case 7:
              State.AlarmState = "Bulk OV"; break;
            case 8:
              State.AlarmState = "Comm.Error"; break;
            case 9:
              State.AlarmState = "Output OC"; break;
            case 10:
              State.AlarmState = "IGBT Sat"; break;
            case 11:
              State.AlarmState = "Bulk UV"; break;
            case 12:
              State.AlarmState = "Internal error"; break;
            case 13:
              State.AlarmState = "Grid Fail"; break;
            case 14:
              State.AlarmState = "Bulk Low"; break;
            case 15:
              State.AlarmState = "Ramp Fail"; break;
            case 16:
              State.AlarmState = "Dc / Dc Fail"; break;
            case 17:
              State.AlarmState = "Wrong Mode"; break;
            case 18:
              State.AlarmState = "Ground Fault"; break;
            case 19:
              State.AlarmState = "Over Temp."; break;
            case 20:
              State.AlarmState = "Bulk Cap Fail"; break;
            case 21:
              State.AlarmState = "Inverter Fail"; break;
            case 22:
              State.AlarmState = "Start Timeout"; break;
            case 23:
              State.AlarmState = "Ground Fault"; break;
            case 24:
              State.AlarmState = "Degauss error"; break;
            case 25:
              State.AlarmState = "Ileak sens.fail"; break;
            case 26:
              State.AlarmState = "DcDc Fail"; break;
            case 27:
              State.AlarmState = "Self Test Error 1"; break;
            case 28:
              State.AlarmState = "Self Test Error 2"; break;
            case 29:
              State.AlarmState = "Self Test Error 3"; break;
            case 30:
              State.AlarmState = "Self Test Error 4"; break;
            case 31:
              State.AlarmState = "DC inj error"; break;
            case 32:
              State.AlarmState = "Grid OV"; break;
            case 33:
              State.AlarmState = "Grid UV"; break;
            case 34:
              State.AlarmState = "Grid OF"; break;
            case 35:
              State.AlarmState = "Grid UF"; break;
            case 36:
              State.AlarmState = "Z grid Hi"; break;
            case 37:
              State.AlarmState = "Internal error"; break;
            case 38:
              State.AlarmState = "Riso Low"; break;
            case 39:
              State.AlarmState = "Vref Error"; break;
            case 40:
              State.AlarmState = "Error Meas V"; break;
            case 41:
              State.AlarmState = "Error Meas F"; break;
            case 42:
              State.AlarmState = "Error Meas Z"; break;
            case 43:
              State.AlarmState = "Error Meas Ileak"; break;
            case 44:
              State.AlarmState = "Error Read V"; break;
            case 45:
              State.AlarmState = "Error Read I"; break;
            case 46:
              State.AlarmState = "Table fail"; break;
            case 47:
              State.AlarmState = "Fan Fail"; break;
            case 48:
              State.AlarmState = "UTH"; break;
            case 49:
              State.AlarmState = "Interlock fail"; break;
            case 50:
              State.AlarmState = "Remote Off"; break;
            case 51:
              State.AlarmState = "Vout Avg errror"; break;
            case 52:
              State.AlarmState = "Battery low"; break;
            case 53:
              State.AlarmState = "Clk fail"; break;
            case 54:
              State.AlarmState = "Input UC"; break;
            case 55:
              State.AlarmState = "Zero Power"; break;
            case 56:
              State.AlarmState = "Fan Stucked"; break;
            case 57:
              State.AlarmState = "DC Switch Open"; break;
            case 58:
              State.AlarmState = "Tras Switch Open"; break;
            case 59:
              State.AlarmState = "AC Switch Open"; break;
            case 60:
              State.AlarmState = "Bulk UV"; break;
            case 61:
              State.AlarmState = "Autoexclusion"; break;
            case 62:
              State.AlarmState = "Grid df / dt"; break;
            case 63:
              State.AlarmState = "Den switch Open"; break;
            case 64:
              State.AlarmState = "Jbox fail"; break;
            default:
              State.AlarmState = "Sconosciuto"; break;
            }
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

         if (((char)ReceiveData[2]) == 'i')  { Version.Par1 = "Aurora 2 kW indoor"; }
    else if (((char)ReceiveData[2]) == 'o')  { Version.Par1 = "Aurora 2 kW outdoor"; }
    else if (((char)ReceiveData[2]) == 'I')  { Version.Par1 = "Aurora 3.6 kW indoor"; }
    else if (((char)ReceiveData[2]) == 'O')  { Version.Par1 = "Aurora 3.0 - 3.6 kW outdoor"; }
    else if (((char)ReceiveData[2]) == '5')  { Version.Par1 = "Aurora 5.0 kW outdoor"; }
    else if (((char)ReceiveData[2]) == '6')  { Version.Par1 = "Aurora 6 kW outdoor"; }
    else if (((char)ReceiveData[2]) == 'P')  { Version.Par1 = "3 - phase interface (3G74)"; }
    else if (((char)ReceiveData[2]) == 'C')  { Version.Par1 = "Aurora 50kW module"; }
    else if (((char)ReceiveData[2]) == '4')  { Version.Par1 = "Aurora 4.2kW new"; }
    else if (((char)ReceiveData[2]) == '3')  { Version.Par1 = "Aurora 3.6kW new"; }
    else if (((char)ReceiveData[2]) == '2')  { Version.Par1 = "Aurora 3.3kW new"; }
    else if (((char)ReceiveData[2]) == '1')  { Version.Par1 = "Aurora 3.0kW new"; }
    else if (((char)ReceiveData[2]) == 'D')  { Version.Par1 = "Aurora 12.0kW"; }
    else if (((char)ReceiveData[2]) == 'X')  { Version.Par1 = "Aurora 10.0kW"; }
                                        else { Version.Par1 = "Sconosciuto"; };

         if (((char)ReceiveData[3]) == 'A')  { Version.Par2 = "UL1741"; }
    else if (((char)ReceiveData[3]) == 'E')  { Version.Par2 = "VDE0126"; }
    else if (((char)ReceiveData[3]) == 'S')  { Version.Par2 = "DR 1663 / 2000"; }
    else if (((char)ReceiveData[3]) == 'I')  { Version.Par2 = "ENEL DK 5950"; }
    else if (((char)ReceiveData[3]) == 'U')  { Version.Par2 = "UK G83"; }
    else if (((char)ReceiveData[3]) == 'K')  { Version.Par2 = "AS 4777"; }
                                       else  { Version.Par2 = "Sconosciuto"; };

         if (((char)ReceiveData[4]) == 'N')  { Version.Par3 = "Transformerless Version"; }
    else if (((char)ReceiveData[4]) == 'K')  { Version.Par3 = "Transformer Version"; }
                                        else { Version.Par3 = "Sconosciuto"; };

         if (((char)ReceiveData[5]) == 'N')  { Version.Par4 = "Wind version"; }
    else if (((char)ReceiveData[5]) == 'K')  { Version.Par4 = "PV version"; }
                                        else { Version.Par4 = "Sconosciuto"; };
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


//=====================================
clsAurora*  Inverter = NULL;
//=====================================

boolean Plugin_111(byte function, struct EventStruct *event, String& string)
{
        boolean success = false;
        switch (function)
        {
        case PLUGIN_DEVICE_ADD:
        {
          Device[++deviceCount].Number = PLUGIN_ID_111;
        //  Device[deviceCount].Type = DEVICE_TYPE_SINGLE; //how the device is connected
          Device[deviceCount].SendDataOption = true;
          Device[deviceCount].Ports = 0;
        //  Device[deviceCount].VType = SENSOR_TYPE_SWITCH; //type of value the plugin will return, used only for Domoticz
          Device[deviceCount].PullUpOption = false;
          Device[deviceCount].InverseLogicOption = false;
          Device[deviceCount].FormulaOption = false;
          Device[deviceCount].ValueCount = 3;  //number of output variables. The value should match the number of keys PLUGIN_VALUENAME1_xxx
          Device[deviceCount].TimerOption = true;
          Device[deviceCount].TimerOptional = false;
          Device[deviceCount].GlobalSyncOption = true;
          Device[deviceCount].DecimalsOnly = false;
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
           break;
        }

        case PLUGIN_GET_DEVICEGPIONAMES:
        {
          //  serialHelper_getGpioNames(event);
            break;
        }

        case PLUGIN_WEBFORM_LOAD:
        {
           addFormNumericBox(F("PVI Address"), F("plugin_111_pviaddr"), PCONFIG(0),1,255);
          // serialHelper_webformLoad(event);
           //after the form has been loaded, set success and break
           success = true;
           break;
        }

        case PLUGIN_WEBFORM_SAVE:
        {
           //this case defines the code to be executed when the form is submitted
           //the plugin settings should be saved to PCONFIG(x)
           //ping configuration should be read from CONFIG_PIN1 and stored

           //serialHelper_webformSave(event);

           PCONFIG(0) = getFormItemInt(F("plugin_111_pviaddr"));

           //after the form has been saved successfuly, set success and break
           success = true;
           break;
        }

        case PLUGIN_WEBFORM_SHOW_CONFIG:
        {
           //string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[0]);
           success = true;
           break;
        }


        case PLUGIN_INIT:
        {
           // this case defines code to be executed when the plugin is initialised
           if (Inverter) { delete Inverter; Inverter = nullptr; }; Inverter = new clsAurora( PCONFIG(0) );
           pinMode(SSerialTxControl, OUTPUT);
           Serial.setTimeout(500);
           Serial.begin(baudrate);
           Serial.flush();
           //=============================================================================================
           String log = F("INIT: PVI Address "); log += PCONFIG(0);
           if ( Inverter != nullptr ) {
           addLog(LOG_LEVEL_INFO, "INIT: Aurora Inverter created!");
           addLog(LOG_LEVEL_INFO, log);
           }
           else {
             addLog(LOG_LEVEL_INFO, "INIT: Aurora Inverter ERROR!");
           }
           success = true;
           break;
        }

        case PLUGIN_READ:
        {
           //code to be executed to read data
           //It is executed according to the delay configured on the device configuration page, only once
           //after the plugin has read data successfuly, set success and break
           Inverter->ReadCumulatedEnergy(0);
           UserVar[event->BaseVarIndex + 0] = Inverter->CumulatedEnergy.Energia;
           Inverter->ReadCumulatedEnergy(3);
           UserVar[event->BaseVarIndex + 1] = Inverter->CumulatedEnergy.Energia;
           Inverter->ReadCumulatedEnergy(4);
           UserVar[event->BaseVarIndex + 2] = Inverter->CumulatedEnergy.Energia;

           success = true;
           break;
        }

        case PLUGIN_EXIT:
        {
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

             String log = F("HTTP command: aurora, ");
             log += rfType;
             // addLog(LOG_LEVEL_INFO, log);

             if ( rfType.equalsIgnoreCase("ask") ) {
                read_RS485();
                success = true;  //set to true only if plugin has executed a command successfully
             }
           }

           if (success){
                 //String url = String(Settings.Name) + "/control?aurora=" + string;
                 String url = String(WiFi.localIP().toString()) + "/control?cmd=aurora,ask";
                 String log = F("To send this command again, ");
                 log += "use this: <a href=\"http://" + url + "\">URL</a>"; log += rfType;
                 // addLog(LOG_LEVEL_INFO, log);

                 if (printToWeb)
                    { printWebString += F("<BR>PVI Address: ");
                      printWebString += String( PCONFIG(0) );
                      printWebString += F("<BR>");
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

void read_RS485(){

  String log = F("read_RS485: "); log +=F("<BR><BR>");

  Inverter->ReadCumulatedEnergy(0); log=F("");
  log += F("Daily Energy: ");
  log += Inverter->CumulatedEnergy.Energia; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadCumulatedEnergy(1); log=F("");
  log += F("Weekly Energy: ");
  log += Inverter->CumulatedEnergy.Energia; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadCumulatedEnergy(3); log=F("");
  log += F("Month Energy: ");
  log += Inverter->CumulatedEnergy.Energia; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadCumulatedEnergy(4); log=F("");
  log += F("Year Energy: ");
  log += Inverter->CumulatedEnergy.Energia; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadLastFourAlarms(); log=F("");
  log += F("LastFourAlarms: ");
  log += Inverter->LastFourAlarms.Alarms1; log +=F(",");
  log += Inverter->LastFourAlarms.Alarms2; log +=F(",");
  log += Inverter->LastFourAlarms.Alarms3; log +=F(",");
  log += Inverter->LastFourAlarms.Alarms4; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadSystemPN(); log=F("");
  log += F("SystemPN: ");
  log += Inverter->SystemPN.PN; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadSystemSerialNumber(); log=F("");
  log += F("SystemSerialNumber: ");
  log += Inverter->SystemSerialNumber.SerialNumber; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadManufacturingWeekYear(); log=F("");
  log += F("ManufacturingWeekYear: ");
  log += Inverter->ManufacturingWeekYear.Week; log +=F(",");
  log += Inverter->ManufacturingWeekYear.Year; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadVersion();
  log = F("Version: ");
  log += Inverter->Version.Par1; log +=F(",");
  log += Inverter->Version.Par2; log +=F(",");
  log += Inverter->Version.Par3; log +=F(",");
  log += Inverter->Version.Par4; log +=F("<BR>");
  printWebString += log; delay(10);

  Inverter->ReadState();
  log = F("TransmissionState: ");log += Inverter->State.TransmissionState ; log +=F("<BR>");printWebString += log; delay(10);
  log = F("GlobalState: ");log += Inverter->State.GlobalState ; log +=F("<BR>");printWebString += log; delay(10);
  log = F("InverterState: ");log += Inverter->State.InverterState ; log +=F("<BR>");printWebString += log; delay(10);
  log = F("Channel1State: ");log += Inverter->State.Channel1State ; log +=F("<BR>");printWebString += log; delay(10);
  log = F("Channel2State: ");log += Inverter->State.Channel2State ; log +=F("<BR>");printWebString += log; delay(10);
  log = F("AlarmState: ");log += Inverter->State.AlarmState ; log +=F("<BR>");printWebString += log; delay(10);

  Inverter->ReadDSP(21,0);log = F("Inverter Temperature (°C): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(22,0); log = F("Booster Temperature (°C): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(23,1); log = F("Input 1 Voltage (Volt): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(25,1); log = F("Input 1 Current (Ampere): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(26,1); log = F("Input 2 Voltage (Volt): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(27,1); log = F("Input 2 Current (Ampere): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(30,0); log = F("Riso : "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(34,0);log = F("Power Peak (Watt): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);
  Inverter->ReadDSP(35,0); log = F("Power Peak Today (Watt): "); log += Inverter->DSP.Valore; log +=F("<BR>"); printWebString += log; delay(10);

//  addLog(LOG_LEVEL_INFO,log);
}

#endif

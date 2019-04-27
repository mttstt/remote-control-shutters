Plugin for EspEasy compatible with Espeasy mega-20190202 version.

Tested on Wemos d1 mini

### how to
For a more stable version is better use a STABLE version (no Dev or Test)

1. Clone last version if github letscontrolit/ESPEasy
2. modify file define_plugin_sets.h in /src folder in this way:

   a. Add in STABLE section the two lines
   
          #define USES_P111   // Aurora Inverter
          #define USES_P112   // RFTX
      
   b. Comment in TESTING section the tho lines
   
          // #define USES_P111	// RF
	        // #define USES_P111	// SenseAir     (MERGED?)
	        // #define USES_P112	// Power
	        // #define USES_P112	// RFTX
    
3. copy in /src folder of cloneproject the two file: _P111_Aurora.ino & _P112_RFTX.ino
4. from Atom-PLATFORMIO make PIO Upload (normal_ESP8266_4M)


### Output


Aurora Inverter - PVI Address: 2

Use URL: http://192.168.1.120/control?cmd=aurora,askData time: 27/4/2019 15:5:3

Daily Energy: 17856
Weekly Energy: 110516
Month Energy: 410792
Year Energy: 1438385
LastFourAlarms: 0,0,0,0
SystemPN: -3G79-
SystemSerialNumber: 912139
ReadFirmwareRelease: C.0.3.4
ManufacturingWeekYear: 12,15
Version: Aurora 4.2kW new,Sconosciuto,Transformerless Version,Wind version
TransmissionState: Everything is OK.
GlobalState: Run
InverterState: Run
Channel1State: 2
Channel2State: 2
AlarmState: 0
Inverter Temperature (C): 61.99
Booster Temperature (C): 55.96
Input 1 Voltage (Volt): 241.99
Input 1 Current (Ampere): 6.71
Input 2 Voltage (Volt): 229.67
Input 2 Current (Ampere): 6.76
Riso : 11.93
Power Peak (Watt): 4623.24
Power Peak Today (Watt): 3882.54

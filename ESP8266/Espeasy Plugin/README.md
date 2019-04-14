Plugin for EspEasy compatible with Espeasy mega-20190202 version.

Tested on Wemos d1 mini

### how to
For a more stable version:

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

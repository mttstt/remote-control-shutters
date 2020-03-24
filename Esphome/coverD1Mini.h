#include "esphome.h"

const int txPin = 16; //GPIO D16 (d1 mini)
//const int txPin = 12; //GPIO ? (ESP32cam)  

int Plugin_Repeat = 10;

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

class MyCustomCover : public Component, public Cover {
 public:
  void setup() override {
    // This will be called by App.setup()
    pinMode(txPin, OUTPUT);
  }
  CoverTraits get_traits() override {
    auto traits = CoverTraits();
    traits.set_is_assumed_state(true);
    traits.set_supports_position(true);
    traits.set_supports_tilt(false);
    return traits;
  }
  void control(const CoverCall &call) override {
    // This will be called every time the user requests a state change.
	// ESP_LOGD("'%s' Command", this->get_name().c_str());
    if (call.get_position().has_value()) {
      float pos = *call.get_position();
	    if ((strcmp(this->get_name().c_str(), "Camera bimbe") == 0) && (pos ==1)) {sendRFCode(up1); };
	    if ((strcmp(this->get_name().c_str(), "Camera bimbe") == 0) && (pos ==0)) {sendRFCode(do1); };
	    if ((strcmp(this->get_name().c_str(), "Camera nonna") == 0) && (pos ==1)) {sendRFCode(up2); };
	    if ((strcmp(this->get_name().c_str(), "Camera nonna") == 0) && (pos ==0)) {sendRFCode(do2); };
	    if ((strcmp(this->get_name().c_str(), "Bagno grande") == 0) && (pos ==1)) {sendRFCode(up3); };
	    if ((strcmp(this->get_name().c_str(), "Bagno grande") == 0) && (pos ==0)) {sendRFCode(do3); };
	    if ((strcmp(this->get_name().c_str(), "Camera genitori") == 0) && (pos ==1)) {sendRFCode(up4); };
	    if ((strcmp(this->get_name().c_str(), "Camera genitori") == 0) && (pos ==0)) {sendRFCode(do4); };
	    if ((strcmp(this->get_name().c_str(), "Bagno piccolo") == 0) && (pos ==1)) {sendRFCode(up5); };
	    if ((strcmp(this->get_name().c_str(), "Bagno piccolo") == 0) && (pos ==0)) {sendRFCode(do5); };
	    if ((strcmp(this->get_name().c_str(), "Sala piccola") == 0) && (pos ==1)) {sendRFCode(up6); };
	    if ((strcmp(this->get_name().c_str(), "Sala piccola") == 0) && (pos ==0)) {sendRFCode(do6); };
	    if ((strcmp(this->get_name().c_str(), "Sala grande") == 0) && (pos ==1)) {sendRFCode(up7); };
	    if ((strcmp(this->get_name().c_str(), "Sala grande") == 0) && (pos ==0)) {sendRFCode(do7); };
	    if ((strcmp(this->get_name().c_str(), "Ripostiglio") == 0) && (pos ==1)) {sendRFCode(up8); };
	    if ((strcmp(this->get_name().c_str(), "Ripostiglio") == 0) && (pos ==0)) {sendRFCode(do8); };
	    if ((strcmp(this->get_name().c_str(), "Cucina") == 0) && (pos ==1)) {sendRFCode(up9); };
	    if ((strcmp(this->get_name().c_str(), "Cucina") == 0) && (pos ==0)) {sendRFCode(do9); };
	    if ((strcmp(this->get_name().c_str(), "All") == 0) && (pos ==1)) {sendRFCode(up0); };
	    if ((strcmp(this->get_name().c_str(), "All") == 0) && (pos ==0)) {sendRFCode(do0); };
	    if (strcmp(this->get_name().c_str(), "Cancello") == 0) {sendRFCode_canc(canc); };

      // Publish new state
      this->position = pos;
      this->publish_state();
    }
    if (call.get_stop()) {
      // User requested cover stop
	    if (strcmp(this->get_name().c_str(), "Camera bimbe") == 0) {sendRFCode(st1); };
	    if (strcmp(this->get_name().c_str(), "Camera nonna") == 0) {sendRFCode(st2); };
	    if (strcmp(this->get_name().c_str(), "Bagno grande") == 0) {sendRFCode(st3); };
	    if (strcmp(this->get_name().c_str(), "Camera genitori") == 0) {sendRFCode(st4); };
	    if (strcmp(this->get_name().c_str(), "Bagno piccolo") == 0) {sendRFCode(st5); };
	    if (strcmp(this->get_name().c_str(), "Sala piccola") == 0) {sendRFCode(st6); };
	    if (strcmp(this->get_name().c_str(), "Sala grande") == 0) {sendRFCode(st7); };
	    if (strcmp(this->get_name().c_str(), "Ripostiglio") == 0) {sendRFCode(st8); };
	    if (strcmp(this->get_name().c_str(), "Cucina") == 0) {sendRFCode(st9); };
	    if (strcmp(this->get_name().c_str(), "All") == 0) {sendRFCode(st0); };
    }
  }


 	void sendRFCode_canc(uint16_t code){
	  ESP_LOGD("custom", "SendRFCode!");
	  for (int i = 0; i < Plugin_Repeat; i++)
	  {
		  for (int bits = 15; bits > 1 ; bits-- )
		  {
			 if (code & (1U << bits) )
			 {
			   digitalWrite(txPin, HIGH);
			   delayMicroseconds(short_delay);
			   digitalWrite(txPin, LOW);
			   delayMicroseconds(long_delay);
			 }
			 else
			 {
			   digitalWrite(txPin, HIGH);
			   delayMicroseconds(long_delay);
			   digitalWrite(txPin, LOW);
			   delayMicroseconds(short_delay);
			 }
		  }
		  delayMicroseconds(3000);
		}
	}


	void sendRFCode(uint64_t code){
	  ESP_LOGD("custom", "SendRFCode!");
	  for (int i = 0; i < Plugin_Repeat; i++)
		{
		  // ----------------------- Preamble ----------------------
		  for (int y = 0; y < 12; ++y)
		  {
			digitalWrite(txPin, HIGH);
			delayMicroseconds(pulse);
			digitalWrite(txPin, LOW);
			delayMicroseconds(pulse);
		  }
		  // -------------------End Preamble--Segnal ---------------
		  digitalWrite(txPin, LOW);
		  delayMicroseconds(3500); // added 3,5 millis

		  for (int bits = 63; bits > -1 ; --bits )
		  {
			if (code & (1ULL << bits) ) {
				digitalWrite(txPin, HIGH);
				delayMicroseconds(pulse);
				digitalWrite(txPin, LOW);
				delayMicroseconds(pulse*2);
			  }
			else {
				digitalWrite( txPin, HIGH );
				delayMicroseconds( pulse * 2 );
				digitalWrite( txPin, LOW );
				delayMicroseconds( pulse );
			}
		  }
		  // -------------------Close--Segnal:00 ---------------
		  digitalWrite( txPin, HIGH );
		  delayMicroseconds( pulse * 2 );
		  digitalWrite( txPin, LOW );
		  delayMicroseconds( pulse );
		  //
		  digitalWrite( txPin, HIGH );
		  delayMicroseconds( pulse * 2 );
		  digitalWrite( txPin, LOW );
		  delayMicroseconds( pulse );
		 // -------------------Close--Segnal:00 ---------------
		  digitalWrite(txPin, LOW);
		  delayMicroseconds(5000); // added 2 millis
		// ---------------------End Segnal --------------------------
		}
	}

};

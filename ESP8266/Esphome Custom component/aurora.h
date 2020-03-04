#include "esphome.h"
#include "ABBAurora.h"

#define RX2 16
#define TX2 17
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 4

class MyCustomSensor : public PollingComponent {
 public:
  ABBAurora *inverter;

  Sensor *DspValue_sensor = new Sensor();
  Sensor *ReadCumulatedEnergy_sensor = new Sensor();

  MyCustomSensor() : PollingComponent(15000) { }

  void setup() override {
    ESP_LOGD("aurora", "Init setup()");
    Serial.begin(115200);
    ABBAurora::setup(Serial1, RX2, TX2, TX_CONTROL_GPIO);
		inverter = new ABBAurora(INVERTER_ADDRESS);
		ESP_LOGD("aurora", "Finish setup()");
  }
  void update() override {
    ESP_LOGD("aurora", "Init update aurora");
  	if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
      {
  		//String version = inverter->Version.Par1;
      ESP_LOGD("aurora", "DSP.Value sensor is: %f", inverter->DSP.Value);
  		DspValue_sensor->publish_state(inverter->DSP.Value);
      }

  	if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
      {
      ESP_LOGD("aurora", "CumulatedEnergy.Energy sensor is: %f", inverter->CumulatedEnergy.Energy);
  		ReadCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
      }

    }
};

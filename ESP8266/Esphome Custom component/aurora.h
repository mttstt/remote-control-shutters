#include "esphome.h"
#include "ABBAurora.h"

#define RX1 3			// GPIO-1
#define TX1 1			// GPIO-3
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 16 	// GPIO-16

class MyCustomSensor : public PollingComponent {
 public:
  ABBAurora *inverter;

  Sensor *DspValue_sensor = new Sensor();
  Sensor *ReadCumulatedEnergy_sensor = new Sensor();
  Sensor *DayCumulatedEnergy_sensor = new Sensor();
  Sensor *MonthCumulatedEnergy_sensor = new Sensor();
  Sensor *YearCumulatedEnergy_sensor = new Sensor();
  Sensor *TransmissionState_sensor = new Sensor();
  Sensor *GlobalState = new Sensor();

  MyCustomSensor() : PollingComponent(15000) { }

  void setup() override 
  {
    ESP_LOGD("Aurora", "Init setup()");
    Serial.begin(115200);
    ABBAurora::setup(Serial1, RX1, TX1, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    ESP_LOGD("aurora", "Finish setup()");
  }

	
  void update() override 
  {
    ESP_LOGD("Aurora", "Init aurora update data");
    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
      {
      	ESP_LOGD("Aurora", "DSP.Value sensor is: %f", inverter->DSP.Value);
  	DspValue_sensor->publish_state(inverter->DSP.Value);
      }
    if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
      {
      	ESP_LOGD("Aurora", "CURRENT DAY Energy sensor is: %f", inverter->CumulatedEnergy.Energy);
  	DayCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
      }	  
    if (inverter->ReadCumulatedEnergy(CURRENT_MONTH))
      {
      	ESP_LOGD("Aurora", "CURRENT MONTH Energy sensor is: %f", inverter->CumulatedEnergy.Energy);
  	MonthCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
      }
    if (inverter->ReadCumulatedEnergy(CURRENT_YEAR))
      {
      	ESP_LOGD("Aurora", "CURRENT YEAR Energy sensor is: %f", inverter->CumulatedEnergy.Energy);
  	YearCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);   
      	ESP_LOGD("Aurora", "TransmissionState sensor is: %f", inverter->CumulatedEnergy.TransmissionState);
  	TransmissionState_sensor->publish_state(inverter->CumulatedEnergy.TransmissionState);
        ESP_LOGD("Aurora", "GlobalState sensor is: %f", inverter->CumulatedEnergy.GlobalState);
  	GlobalState_sensor->publish_state(inverter->CumulatedEnergy.GlobalState);
      }  	 
  }
	
	

};

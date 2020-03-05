#include "esphome.h"
#include "ABBAurora.h"

#define RX1 3			// GPIO-1
#define TX1 1			// GPIO-3
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 16 	// GPIO-16

// class MyCustomSensor : public PollingComponent {
class MyCustomSensor : public PollingComponent, public TextSensor {
 public:
  ABBAurora *inverter;

/*
  Sensor *DspValue_sensor = new Sensor();
  Sensor *ReadCumulatedEnergy_sensor = new Sensor();
  Sensor *DayCumulatedEnergy_sensor = new Sensor();
  Sensor *MonthCumulatedEnergy_sensor = new Sensor();
  Sensor *YearCumulatedEnergy_sensor = new Sensor();
  Sensor *TransmissionState_sensor = new Sensor();
  Sensor *GlobalState_sensor = new Sensor();
*/

  TextSensor *PowerIn1_sensor = new TextSensor();
  TextSensor *PowerIn2_sensor = new TextSensor();
  TextSensor *GridPower_sensor = new TextSensor();
  TextSensor *DayCumulatedEnergy_sensor = new TextSensor();
  TextSensor *MonthCumulatedEnergy_sensor = new TextSensor();
  TextSensor *YearCumulatedEnergy_sensor = new TextSensor();
  TextSensor *TransmissionState_sensor = new TextSensor();
  TextSensor *GlobalState_sensor = new TextSensor();


  MyCustomSensor() : PollingComponent(15000) { }

  void setup() override
  {
    ESP_LOGD("Aurora", "Init setup()");
    Serial.begin(115200);
    ABBAurora::setup(Serial1, RX1, TX1, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    ESP_LOGD("Aurora", "Finish setup()");
  }


  void update() override
  {
    ESP_LOGD("Aurora", "Init aurora update data");
    char temp[7];
    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
      {
        char temp4[7];
        dtostrf(inverter->DSP.Value,6,2,temp4);
      	ESP_LOGD("Aurora", "POWER_IN_1 sensor is: %s", temp4);
  	    PowerIn1_sensor->publish_state(temp4);
      }

    if (inverter->ReadDSPValue(POWER_IN_2, MODULE_MESSUREMENT))
        {
          char temp5[7];
          dtostrf(inverter->DSP.Value,6,2,temp5);
        	ESP_LOGD("Aurora", "POWER_IN_2 sensor is: %s", temp);
    	    PowerIn2_sensor->publish_state(temp5);
        }

    if (inverter->ReadDSPValue(GRID_POWER, MODULE_MESSUREMENT))
          {
            char temp6[7];
            dtostrf(inverter->DSP.Value,6,2,temp6);
          	ESP_LOGD("Aurora", "GRID_POWER sensor is: %s", temp6);
      	    GridPower_sensor->publish_state(temp6);
          }

    if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
      {
        dtostrf(inverter->CumulatedEnergy.Energy,6,2,temp);
      	ESP_LOGD("Aurora", "CURRENT DAY Energy sensor is: %s", temp);
  	    DayCumulatedEnergy_sensor->publish_state(temp);
      }

    if (inverter->ReadCumulatedEnergy(CURRENT_MONTH))
      {
        dtostrf(inverter->CumulatedEnergy.Energy,6,2,temp);
        ESP_LOGD("Aurora", "CURRENT MONTH Energy sensor is: %s", temp);
  	    MonthCumulatedEnergy_sensor->publish_state(temp);
      }
    if (inverter->ReadCumulatedEnergy(CURRENT_YEAR))
      {
        char temp3[6];
        dtostrf(inverter->CumulatedEnergy.Energy,6,2,temp3);
        ESP_LOGD("Aurora", "CURRENT YEAR Energy sensor is: %s", temp3);
  	    YearCumulatedEnergy_sensor->publish_state(temp3);

        char temp1[43];
        dtostrf(inverter->CumulatedEnergy.TransmissionState,6,2,temp1);
      	ESP_LOGD("Aurora", "TransmissionState sensor is: %s", temp1);
  	    TransmissionState_sensor->publish_state(temp1);

        char temp2[24];
        dtostrf(inverter->CumulatedEnergy.GlobalState,6,2,temp2);
        ESP_LOGD("Aurora", "GlobalState sensor is: %s", temp2);
  	    GlobalState_sensor->publish_state(temp2);
      }
  }

};

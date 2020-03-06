#include "esphome.h"
#include "ABBAurora.h"

#define RX1 3			// GPIO-1
#define TX1 1			// GPIO-3
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 16 	// GPIO-16

class AuroraTextSensor : public PollingComponent, public TextSensor {
 public:
  ABBAurora *inverter;
  TextSensor *TransmissionState_sensor = new TextSensor();
  TextSensor *GlobalState_sensor = new TextSensor();
  InverterState_sensor *InverterState = new TextSensor();
  Channel1State_sensor *Channel1State = new TextSensor();
  Channel2State_sensor *Channel2State = new TextSensor();
  AlarmState_sensor *AlarmState = new TextSensor();
 
  AuroraTextSensor() : PollingComponent(60000) { }

  void setup() override
  {
    ESP_LOGD("Aurora", "Init AuroraTextSensor setup()");
    Serial.begin(115200);
    ABBAurora::setup(Serial1, RX1, TX1, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    ESP_LOGD("Aurora", "Finish AuroraTextSensor setup()");
  }

  void update() override
  {
    ESP_LOGD("Aurora", "Init AuroraTextSensor update data");
    if ( inverter->ReadState() )
      {
       ESP_LOGD("Aurora", "TransmissionState sensor is: %s", Inverte->State.TransmissionState);
  	    TransmissionState_sensor->publish_state(Inverte->State.TransmissionState);
     
      	ESP_LOGD("Aurora", "GlobalState sensor is: %s", Inverte->State.GlobalState);
  	    GlobalState_sensor->publish_state(nverte->State.GlobalState);
     
       ESP_LOGD("Aurora", "State InverterState sensor is: %s", Inverte->State.InverterState);
  	    InverterState_sensor->publish_state(nverte->State.InverterState);
     
       ESP_LOGD("Aurora", "State Channel1State sensor is: %s", Inverte-> State.Channel1State);
  	    Channel1State_sensor->publish_state(nverte-> State.Channel1State);
     
       ESP_LOGD("Aurora", "State Channel2State sensor is: %s", Inverte->State.Channel2State);
  	    Channel2State_sensor->publish_state(nverte->State.Channel2State);
      
       ESP_LOGD("Aurora", "State AlarmState sensor is: %s", Inverte->State.AlarmState);
  	    AlarmState_sensor->publish_state(nverte->State.AlarmState);     
      }
  }
}; 
 
 
 
 
 
class AuroraSensor : public PollingComponent {
 public:
  ABBAurora *inverter;
 
  Sensor *PowerIn1_sensor = new Sensor();
  Sensor *PowerIn2_sensor = new Sensor();
  Sensor *GridPower_sensor = new Sensor();
  Sensor *DayCumulatedEnergy_sensor = new Sensor();
  Sensor *MonthCumulatedEnergy_sensor = new Sensor();
  Sensor *YearCumulatedEnergy_sensor = new Sensor();

  AuroraSensor() : PollingComponent(15000) { }

  void setup() override
  {
    ESP_LOGD("Aurora", "Init AuroraSensor setup()");
    Serial.begin(115200);
    ABBAurora::setup(Serial1, RX1, TX1, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    ESP_LOGD("Aurora", "Finish AuroraSensor setup()");
  }

  void update() override
  {
    ESP_LOGD("Aurora", "Init AuroraSensor update data");
    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
      {
      	ESP_LOGD("Aurora", "POWER_IN_1 sensor is: %f", inverter->DSP.Value);
  	    PowerIn1_sensor->publish_state(inverter->DSP.Value);
      }

    if (inverter->ReadDSPValue(POWER_IN_2, MODULE_MESSUREMENT))
       {
        ESP_LOGD("Aurora", "POWER_IN_2 sensor is: %f", inverter->DSP.Value);
    	   PowerIn2_sensor->publish_state(inverter->DSP.Value);
       }

    if (inverter->ReadDSPValue(GRID_POWER, MODULE_MESSUREMENT))
       {
       	ESP_LOGD("Aurora", "GRID_POWER sensor is: %f", inverter->DSP.Value);
      	 GridPower_sensor->publish_state(inverter->DSP.Value);
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
      }
  }
};

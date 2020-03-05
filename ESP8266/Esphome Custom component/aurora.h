esphome:
  name: Aurora
  platform: ESP8266
  board: esp01
  #board: esp01_1m //D1 Mini
  includes:
    - aurora.h
  libraries:
    - "ABB Aurora"

wifi:
  ssid: "MTT_2.4"
  password: "???"

 # Optional manual IP
  manual_ip:
    static_ip: 192.168.1.98
    gateway: 192.168.1.1
    subnet: 255.255.255.0
    dns1: 192.168.1.1
    dns2: 8.8.8.8

# Enable captive portal
captive_portal:

web_server:
  port: 80

# Enable logging
logger:
  level: DEBUG
#  esp8266_store_log_strings_in_flash: False

# Enable Home Assistant API
api:

# Enable ota
ota:

time:
  - platform: homeassistant
    id: homeassistant_time

#sensor:
text_sensor:
- platform: custom
  lambda: |-
    auto inverter = new MyCustomSensor();
    App.register_component(inverter);
    return {inverter->PowerIn1_sensor, inverter->PowerIn2_sensor, inverter->GridPower_sensor, inverter->DayCumulatedEnergy_sensor, inverter->MonthCumulatedEnergy_sensor, inverter->YearCumulatedEnergy_sensor, inverter->TransmissionState_sensor, inverter->GlobalState_sensor };


  #sensors:
  text_sensors:
  - name: "PowerIn1_sensor"
  - name: "PowerIn1_sensor"
  - name: "GridPower_sensor"
  - name: "Cumulated energy (day)"
  - name: "Cumulated energy (Month)"
  - name: "Cumulated energy (Year)"
  - name: "TransmissionState"
  - name: "GlobalState"

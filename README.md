# EspHome
remote_transmitter:
  pin: D1
  carrier_duty_percent: 50% 
  repeat: 10
  wait_time: 0s

switch:
  - platform: remote_transmitter
    name: "Cancello"
    rc_switch_raw:
      code: '..............'
      protocol:
        pulse_length: 360
        sync: [12, 12]
        zero: [1, 2]
        one: [2, 1]
        inverted: False


# remote-control-shutters
Remote control shutter using Raspberry and pigpio library

Note: Use power supply more than 1.0 Ampere

# Instructions as a service

Start it with

    sudo systemctl start shutter

Enable it to run on boot with

    sudo systemctl enable shutter

See logs

    sudo journalctl -u shutter

https://stackoverflow.com/questions/4018154/how-do-i-run-a-node-js-app-as-a-background-service

### Command
http://xxx.ddns.net:7000/?shutter=up6


### ToDO
- [x] Implemnented a subdomain url with DDNS: used free dlinkddns thank tom modem dlink DVA 5292  
- [x] Implmented HTTPS domain using letscript
- [x] Alpha test Google actions in order to avoid beging test message 
- [x] Migrate blind programs from Raspberry to ESP8266
- [x] Migrate 8266 program to plugin EspEasy
- [x] Interrogate Inverter Aurora with ESp8266
- [X] Integrate Tenvis webcam in HA
- [X] Upgrade RFTX, and test it
- [X] Move rfgateway in order to make adeguate test
- [X] Upgrade RFGateway ESP
- [x] Upgrade Aurora ESP
- [x] find api to call from outside
- [x] password in file
- [x] Upgrade APPS IFTTT to point from RPI to RFGateway
- [X] bug extractor install release 0.93
- [X] Upgrade configuration.yaml HA in order to use the new RFgateway
- [ ] search RF 433 frequency"camera bimbe" blind
- [ ] tests from vocal order
- [x] test cancello
- [x] Close ports on modem-router
- [ ] Shutdown RPI
- [ ] Test esp_wifi_repeater outdoor

### Powerline wifi extender AV500 Tplink Problem (abbandoned solution)
There are problems:
- I upgraded the firmware
- MQTT use TCP: some types of traffic are not being bridged by Powerline wifi

### Esp-link
command in order to program a ESP01 1Mb

    python "../../esptool/esptool.py" --port COM6 --baud 115200 write_flash --flash_freq 40m --flash_mode qio --flash_size 1MB 0x0000 boot_v1.7.bin 0x1000 user1.bin 0xFC000 esp_init_data_default.bin 0xFE000 blank.bin

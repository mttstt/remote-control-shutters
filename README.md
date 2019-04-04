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
- [ ] Use HTTPS for shutters
- [x] Alpha test Google actions in order to avoid beging test message 
- [ ] Migrate blind programs from Raspberry to ESP8266
- [ ] Migrate 8266 program to plugin EspEasy
- [ ] Interrogate Inverter Aurora with ESp8266


RS485   Wemos
Vcc     3v3
Gnd     G
RO      D6 (GPIO-14)
RI      D5 (GPIO-12)
RE/DE   D3 (GPIO0)

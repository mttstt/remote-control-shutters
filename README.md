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
- [ ] Use only HTTPS for shutters
- [ ] Add ESP8266 sensors
- [x] Alpha test Google actions in order to avoid beging test message 


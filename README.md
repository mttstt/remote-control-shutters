# remote-control-shutters
Remote control shutter using Raspberry and pigpio library


# Instructions as a service

Start it with

    sudo systemctl start shutter

Enable it to run on boot with

    sudo systemctl enable shutter

See logs

    sudo journalctl -u shutter


https://stackoverflow.com/questions/4018154/how-do-i-run-a-node-js-app-as-a-background-service



### Command
http://192.168.1.110:8889/?shutter=up6

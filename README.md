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
http://192.168.1.110:8889/?shutter=up6

###  sudo nano /etc/systemd/system/shutter.service

[Unit]
Description=Shutter

[Service]
ExecStart=/home/pi/remote-control-shutters/shutter.js
Restart=always
User=pi
# Note RHEL/Fedora uses 'nobody', Debian/Ubuntu uses 'nogroup'
Group=pi
Environment=PATH=/usr/bin:/usr/local/bin
Environment=NODE_ENV=production
WorkingDirectory=/home/pi

[Install]
WantedBy=multi-user.target

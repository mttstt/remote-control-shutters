Sketch for ESP8226 (NodeMCU)

comando da linux da utilizzare in abbinamento al modulo ser2net di espeasy e monitorare i dati trasmessi/ricevuti:

    sudo socat -d -d -d -x pty,link=/home/pi/ttyS0 tcp:192.168.1.120:9999

    aurora -a 2 -c -Y 3 -T -b  /home/pi/ttyS0


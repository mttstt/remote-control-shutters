Sketch for ESP8226 (NodeMCU)

comando da linux da utilizzare in abbinamento al modulo ser2net di espeasy e monitorare i dati trasmessi/ricevuti:

    socat -d -d -x pty,link=/home/pi/ttyS0,b9600,raw TCP:192.168.1.120:9999,reuseaddr

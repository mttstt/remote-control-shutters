#!/usr/bin/env python

# send.py
# 2018-02-15
# Public Domain

import time
import pigpio

Pulse_len=360

1up =   ""
1stop = ""
1down = "
2up =   ""
2stop = ""
2down = "
3up =   ""
3stop = ""
3down = "
4up =   ""
4stop = ""
4down = "
5up =   ""
5stop = ""
5down = "
6up =   "110011000000100100000000000000001011100100000001101000100000000000"
6stop = ""
6down = "
7up =   ""
7stop = ""
7down = "
8up =   ""
8stop = ""
8down = "
9up =   ""
9stop = ""
9down = "

GPIO = 22 # selects gpio
GP=(1<<GPIO) 
NONE=0       # selects no gpios
f1=[]

def transmit_code(code):         
   # ------ Preamble ----------------------------------------
   f1.append(pigpio.pulse(NONE, NONE, 3000)) # added 3 millis
   for i in range(1,12):
      f1.append(pigpio.pulse(GP, NONE, Pulse_len*1))
      f1.append(pigpio.pulse(NONE, GP, Pulse_len*1))
   # ------ End Preamble ------------------------------------

   # -------Segnal ------------------------------------------
   f1.append(pigpio.pulse(NONE, NONE, 3500)) # added 3,5 millis
   for c in segnal:
      if c == '1':
         f1.append(pigpio.pulse(GP, NONE, Pulse_len*1))
         f1.append(pigpio.pulse(NONE, GP, Pulse_len*2))
      elif c == '0':
         f1.append(pigpio.pulse(GP, NONE, Pulse_len*2))
         f1.append(pigpio.pulse(NONE, GP, Pulse_len*1))
      else:
         continue
   f1.append(pigpio.pulse(NONE, NONE, 3000)) # added 3 millis
   # -------End Segnal ------------------------------------------
   pi.wave_clear()
   pi.wave_add_generic(f1)
   f = pi.wave_create() # create and save id
   for t in range(NUM_ATTEMPTS):
      pi.wave_send_once(f)
   #pi.wave_send_repeat(f)
   #time.sleep(0.3)

pi = pigpio.pi() # connect to Pi
if not pi.connected:
   exit()
   
pi.set_mode(GPIO, pigpio.OUTPUT)

if __name__ == '__main__':
    for argument in sys.argv[1:]:
        print("sending {}".format(argument))   
        exec('transmit_code(' + str(argument) + ')')
   
pi.wave_tx_stop()
pi.stop()

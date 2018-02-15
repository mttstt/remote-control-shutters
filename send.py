#!/usr/bin/env python

# send.py
# 2018-02-15
# Public Domain

import time
import pigpio

Pulse_len=360
segnal = "110011000000100100000000000000001011100100000001101000100000000000"
GP=(1<<22) # selects gpio 17
NONE=0       # selects no gpios

pi = pigpio.pi() # connect to Pi
if not pi.connected:
   exit()

pi.set_mode(22, pigpio.OUTPUT)
f1=[]

args = len(sys.argv)

if args > 1:
      tx=_433.tx(pi, gpio=TX)
      for i in range(args-1):
         print("sending {}".format(sys.argv[i+1]))
         tx.send(int(sys.argv[i+1]))
         time.sleep(1)

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
      if c == '0':
         f1.append(pigpio.pulse(GP, NONE, Pulse_len*2))
         f1.append(pigpio.pulse(NONE, GP, Pulse_len*1))
   f1.append(pigpio.pulse(NONE, NONE, 3000)) # added 3 millis
   # -------End Segnal ------------------------------------------

   pi.wave_clear()
   pi.wave_add_generic(f1)
   f = pi.wave_create() # create and save id
   for t in range(NUM_ATTEMPTS):
      pi.wave_send_once(f)
   #pi.wave_send_repeat(f)
   #time.sleep(0.3)
   
pi.wave_tx_stop()
pi.stop()

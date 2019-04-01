#!/usr/bin/env python
# send.py 2018-02-15
# Public Domain

import time
import pigpio
import sys

Pulse_len=360
GPIO = 22 # selects gpio
GP=(1<<22)
NONE=0 # selects no gpios
NUM_ATTEMPTS = 3


def transmit_code(code):

      print repr(code)

      f1=[]
      # ------ Preamble ----------------------------------------
      f1.append(pigpio.pulse(NONE, NONE, 3000)) # added 3 millis
      for i in range(0,12):
         f1.append(pigpio.pulse(GP, NONE, Pulse_len*1))
         f1.append(pigpio.pulse(NONE, GP, Pulse_len*1))
      # ------ End Preamble ------------------------------------

      # -------Segnal ------------------------------------------
      f1.append(pigpio.pulse(NONE, NONE, 3500)) # added 3,5 millis
      for c in code:
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
            # -------Segnal ------------------------------------------
      f1.append(pigpio.pulse(NONE, NONE, 3500)) # added 3,5 millis
      for c in code:
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
         print("sending {}".format(t))
         pi.wave_send_repeat(f)
         time.sleep(0.2)


pi = pigpio.pi() # connect to Pi
if not pi.connected:
   exit()

pi.set_mode(GPIO, pigpio.OUTPUT)
if __name__ == '__main__':
    for argument in sys.argv[1:]:
        print repr(argument)
        exec('transmit_code(' + argument + ')')

pi.wave_tx_stop()
pi.stop()


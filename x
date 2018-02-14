#!/usr/bin/env python

import time

import pigpio

GP17=(1<<22) # selects gpio 17
NONE=0       # selects no gpios

pi = pigpio.pi() # connect to Pi

if not pi.connected:
   exit()

pi.set_mode(22, pigpio.OUTPUT)

##num_array=('i', [1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0]) ##84358

Pulse_len=360
f1=[]

f1.append(pigpio.pulse(NONE, NONE, 3000)) # added 5 millis inter message gap

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(NONE, NONE, 3500)) # added 5 millis inter message gap

# ---------------------------------------------------------------

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*1))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*2))

f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))
f1.append(pigpio.pulse(GP17, NONE, Pulse_len*2))
f1.append(pigpio.pulse(NONE, GP17, Pulse_len*1))

f1.append(pigpio.pulse(NONE, NONE, 5000)) # added 5 millis inter $

pi.wave_clear()

pi.wave_add_generic(f1)

f = pi.wave_create() # create and save id

#pi.wave_send_once(f)

pi.wave_send_repeat(f)

time.sleep(0.3)

pi.wave_tx_stop()

pi.stop()

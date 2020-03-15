#!/usr/bin/env python

# send.py 2018-02-15
# Public Domain
# Copyright 2019, mtt stt, Inc.

# References:
# https://1technophile.blogspot.com/2016/08/low-cost-low-power-6ua-garden-433mhz.html
# https://github.com/esp8266/Arduino/blob/master/doc/reference.rst
# https://arduino.stackexchange.com/questions/44531/arduino-esp8266-direct-fast-control-of-the-digital-pins
# https://www.instructables.com/id/Using-an-ESP8266-to-Control-Mains-Sockets-Using-43/
# http://nerdralph.blogspot.com/2015/04/a-4mbps-shiftout-for-esp8266arduino.html

# Note(1): Watt OK. max pinout watt of Nodemcu 1.0 10mW, Cheap transmitter 433mhz 10 10mW
# Note(2): The segnal of blinks is not rcswitch compliant, so I have write a program without this library
# Note(3): digitalwrite() Esp8286 function runs to 160Khz (6,25 μs): it is enough for this program

import time
import pigpio
import sys

Pulse_len=360
GPIO = 22 # selects gpio
GP=(1<<22)
NONE=0 # selects no gpios
NUM_ATTEMPTS = 3

# ------- gate ---------
short_delay = 760
long_delay = 1520
extended_delay = 0.5
canc = '01111010010000'
# ----------------------


up0 = '110011000000100100000000000000000001100101010001101000100000000000'
st0 = '110011000000011000000000000000000001100101010001101000100000000000'
do0 = '110011000000001000000000000000000001100101010001101000100000000000'
up1 = '?'
st1 = '?'
do1 = '?'
up2 = '110011000000100100000000000000001001100100010001101000100000000000'
st2 = '110011000000011000000000000000001001100100010001101000100000000000'
do2 = '110011000000001000000000000000001001100100010001101000100000000000'
up3 = '110011000000100100000000000000000101100101100001101000100000000000'
st3 = '110011000000011000000000000000000101100101100001101000100000000000'
do3 = '110011000000001000000000000000000101100101100001101000100000000000'
up4 = '110011000000100100000000000000001101100100100001101000100000000000'
st4 = '110011000000011000000000000000001101100100100001101000100000000000'
do4 = '110011000000001000000000000000001101100100100001101000100000000000'
up5 = '110011000000100100000000000000000011100101000001101000100000000000'
st5 = '110011000000011000000000000000000011100101000001101000100000000000'
do5 = '110011000000001000000000000000000011100101000001101000100000000000'
up6 = '110011000000100100000000000000001011100100000001101000100000000000'
st6 = '110011000000011000000000000000001011100100000001101000100000000000'
do6 = '110011000000001000000000000000001011100100000001101000100000000000'
up7 = '110011000000100100000000000000000111100101111110101000100000000000'
st7 = '110011000000011000000000000000000111100101111110101000100000000000'
do7 = '110011000000001000000000000000000111100101111110101000100000000000'
up8 = '110011000000100100000000000000001111100100111110101000100000000000'
st8 = '110011000000011000000000000000001111100100111110101000100000000000'
do8 = '110011000000001000000000000000001111100100111110101000100000000000'
up9 = '110011000000100100000000000000000000010101011110101000100000000000'
st9 = '110011000000011000000000000000000000010101011110101000100000000000'
do9 = '110011000000001000000000000000000000010101011110101000100000000000'

up88 ='010101010000100100000000000000001111100100111110101000100000000000'
st88 ='010101010000011000000000000000001111100100111110101000100000000000'
do88 ='010101010000001000000000000000001111100100111110101000100000000000'
up99 ='010101010000100100000000000000000000010101011110101000100000000000'
st99 ='010101010000011000000000000000000000010101011110101000100000000000'
do99 ='010101010000001000000000000000000000010101011110101000100000000000'

def transmit_code(code):
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
      pi.wave_clear()
      pi.wave_add_generic(f1)
      f = pi.wave_create() # create and save id
      for t in range(NUM_ATTEMPTS):
         print("sending {}".format(t))
         pi.wave_send_repeat(f)
         time.sleep(0.2)

def transmit_code_canc(code):
      f1=[]
      # -------Segnal ------------------------------------------
      f1.append(pigpio.pulse(NONE, NONE, 3500)) # added 3,5 millis
      for c in code:
         if c == '1':
            f1.append(pigpio.pulse(GP, NONE, short_delay))
            f1.append(pigpio.pulse(NONE, GP, long_delay))
         elif c == '0':
            f1.append(pigpio.pulse(GP, NONE, long_delay))
            f1.append(pigpio.pulse(NONE, GP, short_delay))
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
         time.sleep(extended_delay)

pi = pigpio.pi() # connect to Pi
if not pi.connected:
   exit()

pi.set_mode(GPIO, pigpio.OUTPUT)
if __name__ == '__main__':
    for argument in sys.argv[1:]:
        if argument == 'canc':
                exec('transmit_code_canc(' + str(argument) + ')')
        else:
                exec('transmit_code(' + str(argument) + ')')

pi.wave_tx_stop()
pi.stop()

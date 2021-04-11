import serial
import time
import os

arduino = serial.Serial( port='/dev/ttyUSB0' , baudrate=9600 , timeout=0.1 )

lv = 0
lm = 0

while True:
    my_read = arduino.readline().decode("utf-8")
    if my_read:

        dc_arr = my_read.replace(" ", "").replace('\r\n', "").split(",")
        volume = dc_arr[0]
        brightness = dc_arr[1]
        mute = dc_arr[2]

        print( "mute: ", repr(mute) )
        print( "Volume: " , repr(volume) )
        print( "brightness: ", repr(brightness) )
        if lv != volume:
            lv = volume
            os.system('amixer -q -D pulse sset Master {}%'.format(volume))
        if lm != mute:
            lm = mute
            if int(mute) == 1:
                os.system('amixer -q -D pulse sset Master mute')
            elif int(mute) == 0:
                os.system('amixer -q -D pulse sset Master unmute')

# volume-control

Overview:
I am trying to use an encoder to change the volume on my linux desktop; I just like to have the physical wheel. There are many examples 
of this online. Originally, I wanted to have an arduino act as an HID device, however I only have an Adafruit Metro availabe, which is not HID 
compatible, nor can it use HoodLoader2. My idea is to continue to use the Metro as a serial device which communicates with a ptyhon service to change
the volume of the computer.

There are several part to this:
1) Using an encoder to change the volume. 
2) Muting the computer with the encoder PB
3) Linking to volume of the system to a strip of LEDs (this means accounting for the case when the user changes the volume without the encoder)
4) Physical design for 3D printing

Notes:
----------Python Service----------
Not Implemented

I'm not seeing an obvious library to use for python to change the system volume. However, python does have the ability to make terminal command calls.
This should work as long as the call does not require sudo status. So look for a terminal command to set the system volume as well as to get the system
volume. 

This should have a packet of information that it expects to get from the arduino
Additionally, there should be some information sent to the arduino from python

Expected Input Packet (output from arduino)
volume_val, mute_status, report_time

Possible Output Packet(s) (input to arduino)
sys_volume - alerts about changes made from inside the system without the wheel

r , g , b  - request to change the color of the wheel? ( this might be a waste, just pick a color )


----------Scheduler----------
Basic cooperative scheduler added with the TaskScheduler library

----------Encoder----------
SW = Push Button Pin
DT / CLK = Encoder wheel A/B signals
+ = 5V
GND = GND

There are only two interrupt pins on the arduino uno (pins 2 & 3), so we cannot use an interrupt for the pushbutton.

Encoder Debounced

Encoder PB Implemented

----------LEDs----------
The LED strip will be made from a 8 LED package of adafruit neopixels. This is just what I had on hand.

The brightness off the LEDs will correspond to the volume level of the device. 

The LEDs will turn off 


----------CAD----------
Not Implemented


----------Serial----------
Serial communication work basically by default on arduino. This section
pertains to: 
how reports should be built
when should reports be transmitted
when should the device be listening

for the first bit, this will only be focused on outputting reports. so the last req isn't needed yet

UPDATE:
For anyone looking at the network history and thinking 'what the heck happened here?': I
did not originally update my global usernames to match my github account, so the commits
were posting from my linux username. I went back with rebase to fix those previous commits.



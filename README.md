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
----------Encoder----------
SW = Push Button Pin
DT / CLK = Encoder wheel A/B signals
+ = 5V
GND = GND

DT as encA
CLK as encB

There are only two interrupt pins on the arduino uno (pins 2 & 3), so we cannot use an interrupt for the pushbutton.

TODO: correctly debounce the encoder.

----------LEDs----------
Not Implemented


----------CAD----------
Not Implemented


// The metro board does not have the ability to use the usb HID functionality, get the M0 from home for this.
//#include "HID-Project.h"

/*
 * 
 * This is the main controller function. The goal for this one is to control everything 
 * managed by the HID libraries. For this project, this will just be media buttons
 * 
 * This will utilize a cooperative scheduler, placing priorities on encoder inputs
 * 
 */
const int pin_pb = 13;
const int pin_encA = 3;
const int pin_encB = 2;

int encA, encB, ccwise, cwise, val, last_val = 0;

void setup() {
  // put your setup code here, to run once:
  // declare pin modes
  pinMode( LED_BUILTIN , OUTPUT );
  pinMode( pin_pb , INPUT );
  pinMode( pin_encA , INPUT );
  pinMode( pin_encB , INPUT );

  attachInterrupt( digitalPinToInterrupt( pin_encA ) , down , FALLING );
  //attachInterrupt( digitalPinToInterrupt( pin_encB ) , up , FALLING );
  
  // start consumer ( controller for media buttons )
  //Consumer.begin()

  // start serial port
  Serial.begin(9600);

  // setup encoder
  

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println( val );
  

}

void down() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  int temp = digitalRead( pin_encB );
  if ( interrupt_time - last_interrupt_time < 200 ) {
    if ( temp ) {
      val++;
      }
    else {
      val--;
    }
  }
  last_interrupt_time = interrupt_time;
}

void up() {
  if ( digitalRead( pin_encA ) ) {cwise++;}
}

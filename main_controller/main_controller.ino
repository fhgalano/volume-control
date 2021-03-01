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
const int debounce_ms = 50; // ms to debounce the encoder
const int momentum_ms = 150; // ms to consier momentum for the encoder

int encA, encB, val, last_val, delta = 0;

void setup() {
  // put your setup code here, to run once:
  // declare pin modes
  pinMode( LED_BUILTIN , OUTPUT );
  pinMode( pin_pb , INPUT );
  pinMode( pin_encA , INPUT );
  pinMode( pin_encB , INPUT );

  attachInterrupt( digitalPinToInterrupt( pin_encA ) , encoder_f , FALLING );
  //attachInterrupt( digitalPinToInterrupt( pin_encB ) , up , FALLING );
  
  // start consumer ( controller for media buttons )
  //Consumer.begin()

  // start serial port
  Serial.begin(9600);  

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println( val );
  

}

void encoder_f() {
  // declare locals
  static unsigned long last_interrupt_time = 0; // statics persist throughout function calls
  unsigned long interrupt_time = millis();
  int temp = digitalRead( pin_encB ); // I want the second encoder pin to be read as fast as possible from the interrupt
  int delta_t = interrupt_time - last_interrupt_time;
  
  // This if loop creates a debounce
  if ( delta_t > debounce_ms ) {
    last_val = val;
    if ( temp ) {
      val--;
      }
    else {
      val++;
    }

    last_interrupt_time = interrupt_time; // last interrupt is more like last successful interrupt. Where a success is going into the if statement. 
  }

  //last_interrupt_time = interrupt_time;
  if ( ( delta_t < momentum_ms ) && ( val - last_val != delta ) ) {
    val = last_val;
  } 
  else {
    delta = val - last_val;
  }
  
}

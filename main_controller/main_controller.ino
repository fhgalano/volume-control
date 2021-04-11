/*
 * 
 * This is the main controller function. The goal for this one is to control everything 
 * managed by the HID libraries. For this project, this will just be media buttons
 * 
 * This will utilize a cooperative scheduler, placing priorities on encoder inputs
 * 
 */
//====================INCLUDES====================
//#include "HID-Project.h"
#include "Adafruit_NeoPixel.h"
#include "TaskSchedulerDeclarations.h"
#include "TaskSchedulerSleepMethods.h"
#include "TaskScheduler.h"

//====================VARS====================
// pins
const int pin_pb = 2;
const int pin_encA = 3;
const int pin_encB = 4;
const int pin_neo = 11;
const int count_led = 8;

// constants
const int debounce_ms = 50; // ms to debounce the encoder
const int momentum_ms = 150; // ms to consider momentum for the encoder
const int pb_debounce_ms = 1000; // ms to debounce the push button on the encoder

// globals
int encA, encB, val, last_val, delta, mute = 0;
int brightness = 10;

//====================PRE-SETUP====================
// leds
Adafruit_NeoPixel strip( count_led , pin_neo , NEO_RGBW + NEO_KHZ800 );
uint32_t fav_color = strip.Color( 20 , 0 , 100 , brightness );

// scheduler
Scheduler sc;

//====================PROTOTYPES====================
void encoder_f();
void pushbutton_f();
void val_limit();
void update_brightness();
void cereal_out();
void serial_report();

//====================TASKS====================
Task tprintout ( 300 , TASK_FOREVER , &cereal_out , &sc , true );
Task tupdateb ( 100 , TASK_FOREVER , &update_brightness , &sc , true); 
Task tserialreport ( 50 , TASK_ONCE , &serial_report , &sc , false );

//====================SETUP====================
void setup() {
  // put your setup code here, to run once:
  // declare pin modes
  pinMode( LED_BUILTIN , OUTPUT );
  pinMode( pin_pb , INPUT );
  pinMode( pin_encA , INPUT );
  pinMode( pin_encB , INPUT );

  attachInterrupt( digitalPinToInterrupt( pin_encA ) , encoder_f , FALLING );
  attachInterrupt( digitalPinToInterrupt( pin_pb ) , pushbutton_f , RISING );
  
  // start consumer ( controller for media buttons )
  //Consumer.begin()

  // start serial port
  Serial.begin(9600);  

  // set the color and start strip
  strip.begin();
  strip.show();
  strip.fill( fav_color );
  //strip.show();

}

//====================LOOP====================
void loop() {
  // put your main code here, to run repeatedly:
  sc.execute();

}

//====================FUNCTIONS====================
/*
 * 
 * ISR for getting and translating the encoder value
 * includes debounce of the encoder
 * also implements an idea of momentum - meaning that in this use case the user will
 * not rapidly shift from pushing volume up to down. 
 * 
 * This is mostly due to how cheap of an encoder I got, and I could avoid it with an optical encoder. 
 * 
 */
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

/*
 * 
 * ISR for getting the MUTE toggle signal from the pushbutton
 * Includes debounce
 * 
 */
void pushbutton_f() {
  static unsigned long pb_last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if ( pb_last_interrupt_time - interrupt_time > pb_debounce_ms ) {
    // toggle mute
    mute = !mute;

    // update debounce time
    pb_last_interrupt_time = interrupt_time;
  }
  
}

/*
 * 
 * changes the brightness to match the value of val variable. This makes it so the
 * brightness increases and decreases with the volume
 * 
 * Also, it will turn off the LEDs in the case that the mute signal is active
 * 
 */
void update_brightness() {
  // Limit the val here so that they can't conflict
  val_limit();

  // Manage brightness
  uint16_t temp = brightness;
  if ( val > brightness ) {
    brightness++;
  }
  else if ( val < brightness ) {
    brightness--;
  }

  if (  mute == 0  ) {
    strip.fill( strip.Color( 0 , 0 , 0 , brightness ) );
    strip.show();
  }
  else if ( mute == 1 ) {
    strip.fill( strip.Color( 0 , 0 , 0 , 0 ) );
    strip.show();
  }
}

/*
 * 
 * Limits the value of the val variable so that it doesn't go above 100 or below 0
 * 
 */
void val_limit() {
  if (val > 100 ) {
    val = 100;
  }
  else if ( val < 0 ) {
    val = 0;
  }
}

/*
 * 
 * This is what I use to debug to the serial monitor without changing the report
 * 
 */
void cereal_out() {
  if ( Serial ) {
    Serial.print( val );
    Serial.print( ", " );
    Serial.print( brightness );
    Serial.print( ", " );
    Serial.println( mute );
  }
}

/*
 * 
 * Create a report for the arduino to send to the python script. It will
 * be in the format of:
 * Volume,MuteStatus,Time
 * 
 */
void serial_report() {
  if ( Serial ) {
    Serial.print( val );
    Serial.print( "," );
    Serial.print( mute );
    Serial.print( "," );
    Serial.print( millis() );
  }
}

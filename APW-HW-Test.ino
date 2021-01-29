//  Sketch to test the AudioPhonic Workbench Hardware. 
//   
//   Generate a test tone using the Mozzi HI-FI
//   PWM mode.
//   Read each analogue input in turn and display
//   the values via the serial terminal
//   
//   Read and display the switch states.
//
//   *************************************** IMPORTANT ***********************************************
//
//   This sketch requires Mozzi/mozzi_config.h to be
//   be changed from STANDARD mode to HIFI.
//
//   In Mozz/mozzi_config.h,
//   change
//       //#define AUDIO_MODE STANDARD
//       #define AUDIO_MODE STANDARD_PLUS
//       //#define AUDIO_MODE HIFI
//   to
//       //#define AUDIO_MODE STANDARD
//       //#define AUDIO_MODE STANDARD_PLUS
//       #define AUDIO_MODE HIFI
//
//
//  John Potter (technoWomble), 2021 CC by-nc-sa.


#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h>                      // Oscillator sine wave table


Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> testOsc(SIN2048_DATA);    

#define CONTROL_RATE 1    

const int vr1Pin = A0;
const int vr2Pin = A1;
const int vr3Pin = A2;
const int vr4Pin = A3;
const int vr5Pin = A4;
const int vr6Pin = A5;
const int vr7Pin = A6;
const int vr8Pin = A7;

const int sw1Pin = 2;
const int sw2Pin = 3;


void setup(){
  startMozzi(CONTROL_RATE); 
  testOsc.setFreq(440);                               // set the frequency
  Serial.begin(9600);
  pinMode(sw1Pin, INPUT_PULLUP); 
  pinMode(sw2Pin, INPUT_PULLUP);  
}


void updateControl(){
   int potVal1 = mozziAnalogRead(vr1Pin);
   Serial.print(potVal1);
   Serial.print(" - ");
   int potVal2 = mozziAnalogRead(vr2Pin);
   Serial.print(potVal2);
   Serial.print(" - ");
   int potVal3 = mozziAnalogRead(vr3Pin);
   Serial.print(potVal3);
   Serial.print(" - ");
   int potVal4 = mozziAnalogRead(vr4Pin);
   Serial.print(potVal4);
   Serial.print(" - ");
   int potVal5 = mozziAnalogRead(vr5Pin);
   Serial.print(potVal5);
   Serial.print(" - ");
   int potVal6 = mozziAnalogRead(vr6Pin);
   Serial.print(potVal6);
   Serial.print(" - ");
   int potVal7 = mozziAnalogRead(vr7Pin);
   Serial.print(potVal7);
   Serial.print(" - ");
   int potVal8 = mozziAnalogRead(vr8Pin);
   Serial.print(potVal8);
   Serial.print(" - ");

int swVal1 = digitalRead(sw1Pin);
   Serial.print(swVal1 );
   Serial.print(" - ");
int swVal2 = digitalRead(sw2Pin);
   Serial.print(swVal2 );
   Serial.println(" "); 
}


int updateAudio(){
  return testOsc.next() << 5;                           // scale the output for HIFI mode
}

void loop() {

audioHook();                                           // required here
}

//  AudioPhonic WorkBench Sketch. 
// 
//  Simulate three vintage style test oscillators
//  with amplitude and frequency cross modulation
//
//   *************************************** IMPORTANT ***********************************************
//
//   This sketch requires mozzi_config.h to be
//   be changed from STANDARD mode to HIFI.
//
//   In mozzi_config.h,
//   change
//       //#define AUDIO_MODE STANDARD
//       #define AUDIO_MODE STANDARD_PLUS
//       //#define AUDIO_MODE HIFI
//   to
//       //#define AUDIO_MODE STANDARD
//       //#define AUDIO_MODE STANDARD_PLUS
//       #define AUDIO_MODE HIFI
//
//  John Potter (technoWomble), 2021 CC by-nc-sa.
//

#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h>    

#define AUDIO_RATE 32768
#define CONTROL_RATE 128

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> osc1(SIN2048_DATA);  
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> osc2(SIN2048_DATA);  
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> osc3(SIN2048_DATA);

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

int osc1Freq;
int osc2Freq;
int osc3Freq;

int osc1Level;
int osc2Level;
int osc3Level;

int osc2FMVal;
int osc3FMVal;
 
int osc2AMVal;
int osc3AMVal; 

void setup(){
  pinMode(sw1Pin, INPUT_PULLUP); 
  pinMode(sw2Pin, INPUT_PULLUP); 
  startMozzi(CONTROL_RATE); 
  osc1.setPhase(random(2047));                            // don't want the oscillators locked in phase
  osc2.setPhase(random(2047));
  osc3.setPhase(random(2047)); 
}

void updateControl(){
  osc2FMVal = 0;
  osc3FMVal = 0;
  osc2AMVal = 0;
  osc3AMVal = 0;
  
  osc1Freq = ((mozziAnalogRead (vr1Pin)) << 3) + 128;      // Osc1 Pitch, 
  osc2Freq = ((mozziAnalogRead (vr2Pin)) << 3) + 128;      // Osc2 Pitch    
  osc3Freq = ((mozziAnalogRead (vr3Pin)) << 3) + 128;      // Osc3 Pitch    

  osc1Level = mozziAnalogRead(vr4Pin) >> 2;                // Osc1 Volume, 0 - 255        
  osc2Level = mozziAnalogRead(vr5Pin) >> 2;                // Osc2 Volume
  osc3Level = mozziAnalogRead(vr6Pin) >> 2;                // Osc3 Volume
  
  int pot7Val = mozziAnalogRead(vr7Pin);                   // OSC2 MODULATION    
  if (pot7Val <= 512) {osc2FMVal = (512 - pot7Val);} else  // Osc2 FM amount, 0 to 511 
                      {osc2AMVal = (pot7Val- 512) >> 2 ;}  // Osc2 AM amount, 0 to 127                                                                             

  int pot8Val = mozziAnalogRead(vr8Pin);                   // OSC3 MODULATION
  if (pot8Val <= 512) {osc3FMVal = (512 - pot8Val);} else  // Osc3 FM amount, 0 to 511  
                      {osc3AMVal = (pot8Val- 512) >> 2;}   // Osc3 AM amount, 0 to 127             
   
  if (digitalRead(sw1Pin)==0) {osc1Freq = osc1Freq >> 7;}  // Osc1 high/low rate switch  
  if (digitalRead(sw2Pin)==0) {osc2Freq = osc2Freq >> 7;}  // Osc2 high/low rate switch  

  osc1Freq = constrain(osc1Freq,1,8194);
  osc1.setFreq(osc1Freq); 
}

int updateAudio(){
  int osc1Val = osc1.next();

  int osc2ModulatedFreq = (osc2Freq + ((osc1Val ) * (osc2FMVal >> 4)));        // calculate Osc2 FM           
  osc2ModulatedFreq = constrain(osc2ModulatedFreq,1,8194);
  osc2.setFreq(osc2ModulatedFreq); 

  int osc2Val = osc2.next();
  
  int osc2Trem = (osc1Val + 128L);                                              // calculate Osc2 AM    
  osc2Trem = 255 - ((osc2Trem * osc2AMVal) >> 7) ;
  osc2Val = (osc2Val * osc2Trem) >> 8;
  
  int osc3ModulatedFreq = (osc3Freq + ((osc2Val ) * (osc3FMVal >> 4)));        // calculate Osc3 FM                        
  osc3ModulatedFreq = constrain(osc3ModulatedFreq,1,8194);        
  osc3.setFreq(osc3ModulatedFreq);  

  int osc3Val = osc3.next(); 
 
  int osc3Trem = (osc2Val + 128);                                              // calculate Osc2 AM  
  osc3Trem = 255 - ((osc3Trem * osc3AMVal) >> 7) ;
  osc3Val = (osc3Val * osc3Trem) >> 8;

  int osc1Out = (osc1Val * osc1Level) >> 3;
  int osc2Out = (osc2Val * osc2Level) >> 3;                                    // set each oscillator volume                        
  int osc3Out = (osc3Val * osc3Level) >> 3;
    
  return (osc1Out + osc2Out + osc3Out) >> 1;                                  // mix and output                 
}

void loop(){
  audioHook();                                                                 // required according to Mozzi docs
}

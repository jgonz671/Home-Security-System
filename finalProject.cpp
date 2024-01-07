#include "Timer.h"
#include <IRremote.h>


const int buzzerPin = 4;
const int trigPin = 3;
const int echoPin = 2;


IRrecv IR(10);


enum STATE { INIT, DETECT, BUZZ, BUZZ_OFF } gstate = INIT;
enum LIGHT { WAIT, BLINK_ON, BLINK_OFF } lstate = WAIT;


long readUltrasonicDistance(){
   digitalWrite(trigPin, LOW);
   TimerSet(10);
   TimerOn();


   digitalWrite(trigPin, HIGH);
   TimerSet(10);
   TimerOn();


   digitalWrite(trigPin, LOW);
   TimerSet(10);
   TimerOn();


   return pulseIn(echoPin, HIGH) / 29 / 2;
}




void setup(){
   IR.enableIRIn();


   pinMode(buzzerPin, OUTPUT);
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);


   TimerSet(100);
   TimerOn();
}


void tick(void){
 switch(gstate){
       case INIT:
           if(readUltrasonicDistance() < 5){
               gstate = DETECT;
           }
           break;


       case DETECT:
           if(readUltrasonicDistance() < 5){
               gstate = BUZZ;


           }
           else{
               gstate = INIT;
           }
           break;


       case BUZZ:
           digitalWrite(buzzerPin, HIGH);
           digitalWrite(13, HIGH);
           break;


       case BUZZ_OFF:
           digitalWrite(buzzerPin, LOW);
           digitalWrite(13, LOW);
           gstate = INIT;
           break;


   }
}


void blink(void){
 switch(lstate){
   case WAIT:
     if(readUltrasonicDistance() < 5){
       lstate = BLINK_ON;
     }
     break;


   case BLINK_ON:
     if(readUltrasonicDistance() < 5){
     digitalWrite(5, HIGH);
     digitalWrite(6, HIGH);
     }
     break;


   case BLINK_OFF:
     digitalWrite(5, LOW);
     digitalWrite(6, LOW);
     lstate = WAIT;
   break;
 }
}


void remote(){
 if(IR.decode()){
       if(IR.decodedIRData.decodedRawData == 0xBA45FF00) {
           gstate = BUZZ_OFF;
           lstate = BLINK_OFF;
       }
       IR.resume();
   }
}


void loop(){
   remote();
   tick();
   blink();


   if(TimerFlag){
       TimerFlag = 0;
   }
}

// 
// ARDUTEX TRAILING EDGE MOSFET I2C AC LED DIMMER FOR 50HZ AND 60HZ AC LINES
//
// FULLY COMPATIBLE WITH ARDUINO PLATFORM
//
// BASED ON ARDUINO UNO BOOTLOADER @ATMEGA328P 16.000MHz
//
// USB-COM DRIVER CH340G
//
// Dimmer output power levels from 0 to 255. 0 - off, 255 full output power.
//
// Available for sale here - https://www.tindie.com/stores/bugrovs2012/
//
// More information - krida.electronics@gmail.com
//


#include <Wire.h>
#include <TimerOne.h>

#define ADDRESS 0x10  // Slave I2C address. Can be changeable!
#define GATE 3        // PORD.3 | D3 
#define FREQ 10000    // for 50HZ - 10000, for 60HZ - 8330

unsigned char NewEvent, WORK;
unsigned long int inByte, timer; //inByte- expected to recieve 0-100 from I2C master
float lower_limit=30; //lower limit at which light turns of or starts to flicker 0-250
float upper_limit=180; //upper limit at which brightness is maximum 0-250

void setup() {
  
  pinMode(GATE, OUTPUT);
  Wire.begin(ADDRESS); 
  Wire.onReceive(receiveEvent);
  attachInterrupt(0, Zero_Cross_interrupt, FALLING);    // zero-cross external interupt on INT0 | D2
  Timer1.attachInterrupt( timerIsr );
  Serial.begin(9600);
}

void receiveEvent(int bytes) {
  inByte = Wire.read();         // read one incoming byte from the master I2C device

  if (inByte>0) {WORK=1;}
   else {WORK=0;}
  
  NewEvent=1;                   // set new event flag
}  

void Zero_Cross_interrupt() {

 if (WORK) {
   
  digitalWrite(GATE, HIGH);
  Timer1.initialize(timer);    
 }
  else {Timer1.stop();}
}

void timerIsr() {
  
  Timer1.stop();   
  digitalWrite(GATE, LOW);
}
void convert(){
  float aux;
  int aux2;
  aux =lower_limit + inByte*((upper_limit-lower_limit)/100);
  aux2=(int)(aux*10);
  if ((aux2 % 10)>=5){
    inByte= (int)aux+1;
  }
  else{
    inByte= (int)aux;
  }

}

void loop() {

  if (NewEvent){
   
    Serial.print("<> Slave received - ");
    Serial.print(inByte);
    convert();
    Serial.print(" convertido ");
    Serial.println(inByte);
    
    
    if (inByte>250) {inByte=250;}          // if LED bulb flick at full power, change this value to lower

    timer=(inByte*FREQ)/255;

    NewEvent=0;                            // reset event flag
  }
}

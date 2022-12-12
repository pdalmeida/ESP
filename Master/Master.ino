#include <Wire.h>


#define ADDRESS1 0x10
#define ADDRESS2 0x11
// 30-170

unsigned int i, i_new;
unsigned long int lower_limit=30; //lower limit at which light turns of or starts to flicker 0-250
unsigned long int upper_limit=180; //upper limit at which brightness is maximum 0-250

void setup() 
{ 
 Wire.begin();        
 Serial.begin(9600);  
 
} 

void loop() 
{ 

  for (i=0; i<100; i=i+1){
    Wire.beginTransmission(ADDRESS1);    // transmit to device 1
    Wire.write(i);                      // sends ONE bytes
    Wire.endTransmission();             // stop transmitting
    /*Wire.beginTransmission(ADDRESS2);    // transmit to device 2
    Wire.write(100-i);                      // sends ONE bytes
    Wire.endTransmission();*/
    Serial.print("i");
    Serial.println(i);
    delay(500); 
  }

  for (i=100; i>0; i=i-1){
  
    Wire.beginTransmission(ADDRESS1);    // transmit to device 1
    Wire.write(i);                      // sends ONE bytes
    Wire.endTransmission();             // stop transmitting
    /*Wire.beginTransmission(ADDRESS2);    // transmit to device 2
    Wire.write(100-i);                      // sends ONE bytes
    Wire.endTransmission();*/
    Serial.print("i");
    Serial.println(i);
    delay(500); 
  }

//  Serial.print("i ");
//    Serial.print(i);
//    Wire.beginTransmission(ADDRESS1);    // transmit to device 1
//    i_new= lower_limit + (i*((upper_limit-lower_limit)/100));
//    Wire.write(i);                      // sends ONE bytes
//    Wire.endTransmission();             // stop transmitting
//    /*Wire.beginTransmission(ADDRESS2);    // transmit to device 2
//    Wire.write(100-i);                      // sends ONE bytes
//    Wire.endTransmission();*/
//    Serial.print(" new ");
//    Serial.println(i_new);
//    delay(500); 
// 

} 

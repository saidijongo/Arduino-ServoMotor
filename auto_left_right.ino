#include <Servo.h>

Servo servo;  // create servo object to control a servo

void setup() {
  servo.attach(44);  
  servo.write(0);  
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) { 
    // in steps of 1 degree
    servo.write(pos); 
    delay(200);  
  }

  for (int pos = 180; pos >= 0; pos -= 1) {  
    servo.write(pos);                    
    delay(40);                              
  }
}


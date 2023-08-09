#include <Servo.h>

Servo jongo; //servo object

int position = 0;    

void setup() {
  jongo.attach(3); 
}

void loop() {
  for (position = 0; position <= 180; position += 1) {  
    jongo.write(position);                  
    delay(15);                           
  }
  
  for (position = 180; position >= 0; position -= 1) {  
    jongo.write(position);                  
    delay(10);                           
  }
}

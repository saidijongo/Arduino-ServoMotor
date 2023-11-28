//90,1000 
//-60,100
#include <Servo.h>

Servo myservo;  //servo object

void setup() {
  Serial.begin(115200);  
  myservo.attach(45); // Second Section of MPAINO PWN block
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    commandProcess(command);
    Serial.print(command);
  }
}

void commandProcess(String command) {
  // Parse the command for angle and time
  int angle = command.substring(0, command.indexOf(',')).toInt();
  int time = command.substring(command.indexOf(',') + 1).toInt();

  //angle = constrain(angle, 0, 180);

  // Run the servo motor with the parsed values
  runServo(angle, time);
}

void runServo(int angle, int time) {
  myservo.write(angle);

  delay(time);
}

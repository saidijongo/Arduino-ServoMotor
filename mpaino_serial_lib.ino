#include <Servo.h>

Servo myservo;  //servo object

void setup() {
  Serial.begin(9600);  
  myservo.attach(11);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    commandProcess(command);
  }
}

void commandProcess(String command) {
  // Parse the command for angle and time
  int angle = command.substring(0, command.indexOf(',')).toInt();
  int time = command.substring(command.indexOf(',') + 1).toInt();

  angle = constrain(angle, 0, 180);

  // Run the servo motor with the parsed values
  runServo(angle, time);
}

void runServo(int angle, int time) {
  myservo.write(angle);

  delay(time);
}

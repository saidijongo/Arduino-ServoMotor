#include <Arduino.h>
#include <Servo.h>

Servo myServo;

const int servoPin = 10; 
const int dirPin = 9; 
const int pulPin = 8; 

const int numStepsPerRevolution = 6400;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  myServo.attach(servoPin);
  Serial.begin(115200);
  Serial.println("Setup completed.");
}

///SERVO////
void runServo(int angle, int runSpeed) {
  int mappedSpeed = map(runSpeed, 0, 2000, 0, 180);
  
  if (angle >= 0) {
    for (int i = 0; i <= angle; ++i) {
      myServo.write(i);
      delay(mappedSpeed);
    }
  } else {
    for (int i = 0; i >= angle; --i) {
      myServo.write(i);
      delay(mappedSpeed);
    }
  }
}


///STEPPER/////
void runStepper(int angle, int runTime) {
  Serial.print("Running stepper motor, Angle: " + String(angle) + " Run Time: " + String(runTime) + "\r\n");
  //Serial.print(angle);
  //Serial.print(", Run time: ");
  //Serial.println(runTime);

  int direction = (angle >= 0) ? HIGH : LOW;
  angle = abs(angle);
  digitalWrite(dirPin, direction);

  // Calculate the number of steps based on the angle
  int steps = angle * numStepsPerRevolution / 360;

  // Run the stepper motor
  for (int i = 0; i < steps; i++) {
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(runTime);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(runTime);
  }

  // Run for the requested time
  //delay(runTime);
}
/*
void processCommand(String command) {
  char separator = ',';

  int firstBracketIndex = command.indexOf('(');
  int secondBracketIndex = command.indexOf(')', firstBracketIndex + 1);

  if (firstBracketIndex != -1 && secondBracketIndex != -1) {
    // Extract motor type and parameters from the command
    String motorTypeAndParams = command.substring(firstBracketIndex + 1, secondBracketIndex);
    int commaIndex = motorTypeAndParams.indexOf(separator);

    if (commaIndex != -1) {
      int param1 = motorTypeAndParams.substring(0, commaIndex).toInt();
      int param2 = motorTypeAndParams.substring(commaIndex + 1).toInt();

      Serial.print("Param1: ");
      Serial.print(param1);
      Serial.print(", Param2: ");
      Serial.println(param2);

      if (motorTypeAndParams.startsWith("STEPPERMOTOR_OPERATION")) {
        runStepper(param1, param2);
      } else {
        Serial.println("Unknown motor type");
      }
    } else {
      Serial.println("Invalid command format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}
*/


/*
void processCommand(String command) {
  char separator = ',';

  // Find the index of the first and last parentheses
  int firstBracketIndex = command.indexOf('(');
  int lastBracketIndex = command.lastIndexOf(')');

  if (firstBracketIndex != -1 && lastBracketIndex != -1 && firstBracketIndex < lastBracketIndex) {
    // Extract motor type and parameters from the command
    String motorTypeAndParams = command.substring(firstBracketIndex + 1, lastBracketIndex);
    int commaIndex = motorTypeAndParams.indexOf(separator);

    if (commaIndex != -1) {
      int param1 = motorTypeAndParams.substring(0, commaIndex).toInt();
      int param2 = motorTypeAndParams.substring(commaIndex + 1).toInt();

      Serial.print("Param1: ");
      Serial.print(param1);
      Serial.print(", Param2: ");
      Serial.println(param2);

      if (motorTypeAndParams.startsWith("STEPPERMOTOR_OPERATION")) {
        runStepper(param1, param2);
      } else {
        Serial.println("Unknown motor type");
      }
    } else {
      Serial.println("Invalid command format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}

////////////////////
void processCommand(String command) {
  char separator = ',';

  // Find the index of the first and last parentheses
  int firstBracketIndex = command.indexOf('(');
  int lastBracketIndex = command.lastIndexOf(')');

  if (firstBracketIndex != -1 && lastBracketIndex != -1 && firstBracketIndex < lastBracketIndex) {
    // Extract motor type and parameters from the command
    String motorTypeAndParams = command.substring(firstBracketIndex + 1, lastBracketIndex);
    motorTypeAndParams.trim();

    // Split the string into an array of parameters
    String params[2];
    int numParams = motorTypeAndParams.split(separator, params, 2);

    if (numParams == 2) {
      int param1 = params[0].toInt();
      int param2 = params[1].toInt();

      Serial.print("Param1: ");
      Serial.print(param1);
      Serial.print(", Param2: ");
      Serial.println(param2);

      if (motorTypeAndParams.startsWith("STEPPERMOTOR_OPERATION")) {
        runStepper(param1, param2);
      } else {
        Serial.println("Unknown motor type");
      }
    } else {
      Serial.println("Invalid command format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}


/////////////////////////////////

void processCommand(String command) {
  char separator = ',';

  // Find the index of the first and last parentheses
  int firstBracketIndex = command.indexOf('(');
  int lastBracketIndex = command.lastIndexOf(')');

  if (firstBracketIndex != -1 && lastBracketIndex != -1 && firstBracketIndex < lastBracketIndex) {
    // Extract motor type and parameters from the command
    String motorTypeAndParams = command.substring(firstBracketIndex + 1, lastBracketIndex);
    motorTypeAndParams.trim();

    // Find the index of the separator
    int separatorIndex = motorTypeAndParams.indexOf(separator);

    if (separatorIndex != -1) {
      // Extract parameter 1 and parameter 2 from the substring before and after the separator
      String param1Str = motorTypeAndParams.substring(0, separatorIndex);
      String param2Str = motorTypeAndParams.substring(separatorIndex + 1);

      // Convert the parameter strings to integers
      int param1 = param1Str.toInt();
      int param2 = param2Str.toInt();

      Serial.print("Param1: ");
      Serial.print(param1);
      Serial.print(", Param2: ");
      Serial.println(param2);

      if (motorTypeAndParams.startsWith("STEPPERMOTOR_OPERATION")) {
        runStepper(param1, param2);
      } else {
        Serial.println("Unknown motor type");
      }
    } else {
      Serial.println("Invalid command format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}
*/


void processCommand(String command) {
  char separator = ',';

  int firstBracketIndex = command.indexOf('(');
  int secondBracketIndex = command.indexOf(')', firstBracketIndex + 1);

  if (firstBracketIndex != -1 && secondBracketIndex != -1) {
    // Extract motor type and SID from the first set of brackets
    String motorTypeAndSID = command.substring(firstBracketIndex + 1, secondBracketIndex);
    int spaceIndex = motorTypeAndSID.indexOf(' ');

    if (spaceIndex != -1) {
      String motorType = motorTypeAndSID.substring(0, spaceIndex);
      String SID = motorTypeAndSID.substring(spaceIndex + 1);

      //Serial.print("Motor Type: ");
      //Serial.println(motorType);
      Serial.print("SID: ");
      Serial.println(SID);

      
      // Relay control logic based on motorType
      if (motorType == "PUMPMOTOR_OPERATION") {
        //digitalWrite(driverOut1, HIGH);
        //iswashing = true;
      } else if (motorType == "WASHING_OPERATION") {
        //digitalWrite(driverOut2, HIGH);
        //iswashing = true;
      }


      // Extract pump number/angle and run time from subsequent brackets
      int index = secondBracketIndex + 1;
      while (index < command.length()) {
        int nextBracketIndex = command.indexOf('(', index);
        int endIndex = command.indexOf(')', nextBracketIndex + 1);

        if (nextBracketIndex != -1 && endIndex != -1) {
          String inputData = command.substring(nextBracketIndex + 1, endIndex);

          int commaIndex = inputData.indexOf(separator);
          if (commaIndex != -1) {
            int param1 = inputData.substring(0, commaIndex).toInt();
            int param2 = inputData.substring(commaIndex + 1).toInt();

            Serial.print(motorType);
            Serial.print(": Param1: ");
            Serial.print(param1);
            Serial.print(", Param2: ");
            Serial.println(param2);

            if (motorType == "PUMPMOTOR_OPERATION"); //runPumps(param1, param2);
            else if (motorType == "SERVOMOTOR_OPERATION") runServo(param1, param2);
            else if (motorType == "STEPPERMOTOR_OPERATION") runStepper(param1, param2);
            else Serial.println("Unknown motor type");
          } else {
            Serial.println("Invalid pump data format");
          }

          index = endIndex + 1;
        } else {
          break;
        }
      }
    } else {
      Serial.println("Invalid motor type and SID format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}


void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    //Serial.print("Received command: ");

    //Serial.println(data);

    processCommand(data);
  }
}

// "(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(56,3000),(58,4250),(56,3000),(60,4000)"
//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(54,1000),(55,425--2000),(56,3000),(57,4000),(58,3000)"
//"(SERVOMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(90,3000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(360,2000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(-360,2000)"

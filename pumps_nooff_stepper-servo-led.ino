
#include <Arduino.h>

#include <FastLED.h>
#include <elapsedMillis.h>
#include <Servo.h>



#define LED_PIN 11
#define NUM_LEDS 15

CRGB leds[NUM_LEDS];

elapsedMillis elapsedTime; // Declare elapsedTime as a static variable


const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83};
const int dirPin = 45;
const int pullPin = 46;
const int servoPWM = 44;
#define IR_PIN 13 

const int driverOut1 = 84;
const int driverOut2 = 85;

const int numPumps = sizeof(motorPins) / sizeof(motorPins[0]);

bool isReverse = true;

// Find the maximum runTime for param2 values.
int maxRunTime = 0;

unsigned long pumpStartTimes[numPumps] = {0};
unsigned long lastPumpEndTime = 0; // Variable to store the end time of the last pump

String currentMotorType; 
Servo myServo;

enum MotorState
{
  IDLE,
  RUNNING_PUMPS,
  RUNNING_STEPPER
};

MotorState motorState = IDLE;

void setup(){
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  //Serial.print("New baby: " + maxRunTime);

  for (int i = 0; i < numPumps; i++){
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }

  pinMode(dirPin, OUTPUT);
  pinMode(pullPin, OUTPUT);
  pinMode(servoPWM, OUTPUT);

  Serial.begin(115200);
}


void roundStrip(int speed, int runTime) {
  bool stripRunning = true;
  static uint8_t startIndex = 0;
  static uint8_t hue = 0;
  unsigned long startTime = millis();

  while (stripRunning && (millis() - startTime < runTime)) {
    // Fill the entire LED strip with a rainbow gradient
    fill_rainbow(leds, NUM_LEDS, hue, 4);

    // Move the rainbow effect from left to right
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = leds[(i + startIndex) % NUM_LEDS];
    }

    FastLED.show();
    FastLED.delay(speed);

    // Increment the rainbow hue to change colors
    hue++;

    // Move the rainbow gradient index
    startIndex++;
    if (startIndex >= NUM_LEDS) {
      startIndex = 0;
    }
  }

  // Turn off LEDs after the runTime
  fill_solid(leds, NUM_LEDS, CRGB::Black); // Set all LEDs to black (off)
  FastLED.show();
  stripRunning = false; //stop iterations
}

void ledStrip(int speed, int runTime) {
  unsigned long startTime = millis();
  unsigned long currentTime;

  // Set the background color to blue
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  while (elapsedTime < runTime) {
    currentTime = millis(); // Update currentTime
    // Move the group of 5 green LEDs from left to right
    for (int i = 0; i <= NUM_LEDS - 5; ++i) {
      fill_solid(leds + i, 5, CRGB::Green); // Set a group of 5 LEDs to green
      FastLED.show();
      delay(speed);
      fill_solid(leds + i, 5, CRGB::Blue); // Set the same group back to blue
    }

    // Move the group of 5 green LEDs from right to left
    for (int i = NUM_LEDS - 5; i >= 0; --i) {
      fill_solid(leds + i, 5, CRGB::Green); // Set a group of 5 LEDs to green
      FastLED.show();
      delay(speed);
      fill_solid(leds + i, 5, CRGB::Blue); // Set the same group back to blue
    }

    // Check if runtime exceeded
    if (currentTime - startTime >= runTime) {
      break;
    }
  }

  // Turn off LEDs after the runTime
  fill_solid(leds, NUM_LEDS, CRGB::Black); // Set all LEDs to black (off)
  FastLED.show();
}

void blinkLEDs() {
  const int blinkDuration = 20000;  // 20 seconds
  const int blinkInterval = 500;   // 500 milliseconds (0.5 seconds)

  static elapsedMillis elapsedTime; // Track elapsed time

  while (elapsedTime < blinkDuration) {
    // Turn on all LEDs
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
    delay(blinkInterval);

    // Turn off all LEDs
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(blinkInterval);
  }

  // Turn off LEDs after the specified duration
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}



void runPumps(int pumpNumber, int runTime){
  digitalWrite(isReverse ? driverOut2 : driverOut1, HIGH);
  digitalWrite(motorPins[pumpNumber - 54], HIGH);
  pumpStartTimes[pumpNumber - 54] = millis();

  // Update lastPumpEndTime if the current pump has a later end time
  if (millis() + runTime > lastPumpEndTime){
    lastPumpEndTime = millis() + runTime;
  }

  Serial.print("Running pump: ");
  Serial.println(pumpNumber);
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


/*
void runStepper(int angle = 3000, int runTime = 1000){
  int direction = (angle >= 0) ? HIGH : LOW;
  angle = abs(angle);
  digitalWrite(dirPin, direction);

  // Calculate the number of steps based on the angle
  int steps = int(2 * (angle / 1.8));

  // Run the stepper motor
  for (int i = 0; i < steps; i++){
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(500);
  }

  // Run for the requested time
  delay(runTime);
  Serial.print("New baby: " + maxRunTime);
    {
      // Run the stepper motor after all pumps have finished
      motorState = RUNNING_STEPPER;
    }
}
*/

void runStepper(int angle, int speed) {
  const float STEP_ANGLE = 1.8; 
  int direction = (angle >= 0) ? HIGH : LOW;
  angle = abs(angle);

  digitalWrite(dirPin, direction);
  int step_target_position = int(2 * (angle / STEP_ANGLE));

  int i = 0;
  while (i < step_target_position && digitalRead(IR_PIN) == HIGH) {
    digitalWrite(pullPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(pullPin, LOW);
    delayMicroseconds(speed);

    i++;
  }

  digitalWrite(pullPin, LOW);
  blinkLEDs();

  if (digitalRead(IR_PIN) == LOW || i < step_target_position) {
    blinkLEDs();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }
}

void processCommand(String command){
  char separator = ',';

  int firstBracketIndex = command.indexOf('(');
  int secondBracketIndex = command.indexOf(')', firstBracketIndex + 1);

  if (firstBracketIndex != -1 && secondBracketIndex != -1){
    // Extract motor type and SID from the first set of brackets
    String motorTypeAndSID = command.substring(firstBracketIndex + 1, secondBracketIndex);
    int spaceIndex = motorTypeAndSID.indexOf(' ');

    if (spaceIndex != -1){
      String motorType = motorTypeAndSID.substring(0, spaceIndex);
      String SID = motorTypeAndSID.substring(spaceIndex + 1);

      currentMotorType = motorType; // Store the current motor type

      // Extract pump number/angle and run time from subsequent brackets
      int index = secondBracketIndex + 1;
      while (index < command.length()){
        int nextBracketIndex = command.indexOf('(', index);
        int endIndex = command.indexOf(')', nextBracketIndex + 1);

        if (nextBracketIndex != -1 && endIndex != -1){
          String inputData = command.substring(nextBracketIndex + 1, endIndex);

          int commaIndex = inputData.indexOf(separator);
          if (commaIndex != -1){
            int param1 = inputData.substring(0, commaIndex).toInt();
            int param2 = inputData.substring(commaIndex + 1).toInt();

            Serial.print(currentMotorType); // Use the stored motor type
            Serial.print(": Param1: ");
            Serial.print(param1);
            Serial.print(", Param2: ");
            Serial.println(param2);

            if (currentMotorType == "PUMPMOTOR_OPERATION"){
              runPumps(param1, param2);
              maxRunTime = max(maxRunTime, param2);
              motorState = RUNNING_PUMPS;
              Serial.println("maxRunTime: " + String(maxRunTime));
            }
            else if (currentMotorType == "REVERSE_PUMPMOTOR_OPERATION"){
              runPumps(param1, param2);
              //motorState = RUNNING_PUMPS;
            }
            else if (currentMotorType == "SERVOMOTOR_OPERATION"){
              runServo(param1, param2);
            }

            else if (currentMotorType == "STEPPERMOTOR_OPERATION")
            {
              runStepper(param1, param2);
            }
            else
            {
              Serial.println("Unknown motor type");
            }
          }
          else
          {
            Serial.println("Invalid pump data format");
          }

          index = endIndex + 1;
        }
        else
        {
          break;
        }
      }
    }
    else
    {
      Serial.println("Invalid motor type and SID format");
    }
  }
  else
  {
    Serial.println("Invalid command format");
  }

  // Check and deactivate pumps if the run time has elapsed
  bool allPumpsOff = true;
  for (int i = 0; i < numPumps; i++)
  {
    if (millis() < pumpStartTimes[i] || digitalRead(motorPins[i]) == HIGH)
    {
      allPumpsOff = false;
      break;
    }
  }

  if (allPumpsOff && motorState == RUNNING_PUMPS)
  {
    if (currentMotorType == "PUMPMOTOR_OPERATION")
    {
      digitalWrite(driverOut1, LOW);
    }
    else if (currentMotorType == "REVERSE_PUMPMOTOR_OPERATION")
    {
      digitalWrite(driverOut2, LOW);
    }

    motorState = IDLE;
  }
}

void loop()
{
  //Serial.print("New baby: " + maxRunTime);

  if (motorState == IDLE)
  {
    if (Serial.available() > 0)
    {
      String data = Serial.readStringUntil('\n');
      Serial.println(data);
      processCommand(data);
    }
  }
  else if (motorState == RUNNING_PUMPS)
  {
    // Check and deactivate pumps if the run time has elapsed
    for (int i = 0; i < numPumps; i++)
    {
      if (millis() >= pumpStartTimes[i] + lastPumpEndTime && digitalRead(motorPins[i]) == HIGH)
      //if (millis() >= pumpStartTimes[i] + lastPumpEndTime)

      {
        digitalWrite(motorPins[i], LOW);
      //delay(maxRunTime);
      //ledStrip(500,maxRunTime);
      }
    }

    if (millis() >= lastPumpEndTime)
    {
      // Run the stepper motor after all pumps have finished
      motorState = RUNNING_STEPPER;
    }
  }
  else if (motorState == RUNNING_STEPPER)
  {
    // Run the stepper motor with appropriate values
    runStepper(5000,2000);

    // Reset motor state to IDLE
    motorState = IDLE;

  }
}
//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(54,1000),(55,3500),(56,2600),(57,1000),(58,2500),(59,4000),(59,1000),(60,5500),(61,500),(62,3600),(64,1000),(65,2500),(66,4000),(67,1000),(68,5500),(69,5000),(70,3600),(71,2000),(75,2500),(80,4000),(83,1000),(78,5500)"
//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(56,3000),(58,4250),(56,3000),(60,4000)"
//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(54,1000),(55,2000),(56,3000),(57,4000),(58,3000)"
//"(SERVOMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(90,3000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(360,2000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(-360,2000)"

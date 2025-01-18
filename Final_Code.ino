#include <Servo.h>

// Define servo objects for 3R manipulator and gripper
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Pin assignments
const int basePin = 6;
const int shoulderPin = 9;
const int elbowPin = 10;
const int gripperPin = 11;

// Gripper open and close positions
const int gripperOpenPos = 45;    // Angle when gripper is open
const int gripperClosePos = 150;    // Angle when gripper is closed

void setup() {
  Serial.begin(9600);
  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  gripperServo.attach(gripperPin);

  // Initialize arm and gripper position
  baseServo.write(0);
  shoulderServo.write(90);
  elbowServo.write(90);
  gripperServo.write(gripperOpenPos);  // Start with gripper open
}

void loop() {
  if (Serial.available() > 0) {
    char colorCode = Serial.read();

    if (colorCode == '1') {
      moveArmToBin(1);  // Red bin
    } else if (colorCode == '2') {
      moveArmToBin(2);  // Green bin
    } else if (colorCode == '3') {
      moveArmToBin(3);  // Blue bin
    } else {
      moveArmToBin(4);  // Default bin
    }
  }
}

// Function to move the arm to a specific bin
void moveArmToBin(int bin) {
  // Step 1: Move to initial pickup position
  baseServo.write(0);           // Base center position for pickup
  //shoulderServo.write(60);       // Lower shoulder to pickup position
  //elbowServo.write(120);         // Position elbow for pickup
  delay(1500);

  // Step 2: Close the gripper to pick up the object
  gripperServo.write(gripperClosePos);
  delay(1500);                    // Hold to ensure the object is gripped

  // Step 3: Move to the target bin position
  switch (bin) {
    case 1: // Red bin
      baseServo.write(90);       // Rotate base toward red bin
      //shoulderServo.write(120);  // Adjust shoulder to reach bin
      //elbowServo.write(60);
      delay(1500);
      break;
    case 2: // Green bin
      baseServo.write(250);      // Rotate base toward green bin
      //shoulderServo.write(120);
      //elbowServo.write(60);
      delay(1500);
      break;
    case 3: // Blue bin
      baseServo.write(135);      // Rotate base toward blue bin
      //shoulderServo.write(120);
      //elbowServo.write(60);
      delay(1500);
      break;
    default: // Default bin
      baseServo.write(180);      // Rotate base toward default bin
      //shoulderServo.write(120);
      //elbowServo.write(60);
      delay(1500);
      break;
  }

  // Step 4: Open the gripper to release the object
  gripperServo.write(gripperOpenPos);
  delay(1500);                    // Delay to ensure the object is released

  // Step 5: Return arm to initial position
  //shoulderServo.write(90);       // Lift arm
  //elbowServo.write(90);          // Reset elbow
  baseServo.write(0);           // Center base
  gripperServo.write(gripperOpenPos);  // Keep gripper open for next pickup
}

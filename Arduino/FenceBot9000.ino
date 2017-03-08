
 
// Abstract pin numbers for easy hardware changes
int BLK = 12;
int GRN = 11;
int RED = 10;
int BLU =  9;
int testSpeed = 700;

// Logic constants for step numbers
bool steps[8][4] = { {1, 0, 0, 0},
                     {1, 0, 1, 0},
                     {0, 0, 1, 0},
                     {0, 1, 1, 0},
                     {0, 1, 0, 0},
                     {0, 1, 0, 1},
                     {0, 0, 0, 1},
                     {1, 0, 0, 1} };

// Global variable to track current motor step
int currentStep = 0;

void set_step(int step) {
  //  Sets which step the motor is on. Step should be between 0 and 7.
  digitalWrite(BLK, steps[step][0]);
  digitalWrite(GRN, steps[step][1]);
  digitalWrite(RED, steps[step][2]);
  digitalWrite(BLU, steps[step][3]);
}

void rotate(int angle, int speed) {
  /*
  Rotates motor continuously.
  
  Angle is measured in degrees and can be positive or negative of any magnitude.
  
  Speed is measured in degrees per second.
  */

  int numSteps = angle * 10 / 9;
  if (numSteps < 0) {numSteps = -numSteps;}
  int period = 900000 / speed;
    
  for (int x = 0; x < numSteps; x++) {
    set_step(currentStep);
    if (angle > 0) {
      currentStep++;
      if (currentStep == 8) {currentStep = 0;}
    }
    else if (angle < 0) {
      currentStep--;
      if (currentStep == -1) {currentStep = 7;}
    }
    
    delayMicroseconds(period);
  }
}

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(BLK, OUTPUT);
  
  // For debugging
  Serial.begin(9600);
}
 
void loop() {
  //rotate(-720, testSpeed);
  rotate(360, 450);
  delay(150);
  rotate(-360, 450);
  delay(600);
  //set_step(4);
}

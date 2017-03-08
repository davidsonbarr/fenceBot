#include <avr/io.h>
#include <util/delay.h>
#include "stepper.h"

#define motorPORT PORTC
#define motorDDR  DDRC
// Note: it is assumed later that the motor occupies the less significant nibble
// of a single port. That could be remidied, but isn't now.
#define motorBLK PC3
#define motorGRN PC2
#define motorRED PC1
#define motorBLU PC0


// Logic constants for step numbers
static int steps[8] = {0b1000,
                0b1010,
                0b0010,
                0b0110,
                0b0100,
                0b0101,
                0b0001,
                0b1001};

// Global variable to track current motor step
static int currentStep = 0;

void motor_init() {
  motorDDR  |= 0b00001111;
  motorPORT &= 0b11110000;
}

// Mimic Arduino function to sleep for a variable amount of time.
static void delayMicroseconds(int period) {
  for (uint16_t i = 0; i < period; i++) {
    _delay_us(1);
  };
}

void set_step(int step) {
  //  Sets which step the motor is on. Step should be between 0 and 7.
  
  // Set lower nibble of motorPORT to 0000
  motorPORT &= 0b11110000;
  
  // Set lower nibble of motorPORT to whatever the next step is.
  motorPORT |= steps[step];
}

void step_up() {
  // Moves the motor one step clockwise from wherever it is.
  currentStep++;
  if (currentStep == 8) {currentStep = 0;}
  set_step(currentStep);
}

void step_down() {
  // Moves the motor one step counter-clockwise from wherever it is.
  currentStep--;
  if (currentStep == -1) {currentStep = 7;}
  set_step(currentStep);
}


void rotate(int16_t angle, uint16_t speed) {
  /*
  Rotates motor continuously.
  
  Angle is measured in degrees and can be positive or negative of any magnitude.
  
  Speed is measured in degrees per second.
  */

  int numSteps = angle * 10 / 9;
  if (numSteps < 0) {numSteps = -numSteps;}
  int period = 900000 / speed;
    
  for (int x = 0; x < numSteps; x++) {
    
    if (angle > 0) {
      step_up();
    }
    else if (angle < 0) {
      step_down();
    }
    
    delayMicroseconds(period);
  }
}

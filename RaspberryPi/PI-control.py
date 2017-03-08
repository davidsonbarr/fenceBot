#!/usb/bin/python
import RPi.GPIO as GPIO
from time import sleep


# Abstract pin numbers for easy hardware changes
RED = 10
BLU = 9
GRN = 11
BLK = 8

# Logic constants for step numbers

steps = [ [True , False, False, False],\
          [True , False, True , False],\
          [False, False, True , False],\
          [False, True , True , False],\
          [False, True , False, False],\
          [False, True , False, True ],\
          [False, False, False, True ],\
          [True , False, False, True ] ]
          
# Switching the 0 and 1 rows made the operation smoother, but still not smooth
# while testing on 15 Jan. Not sure why yet.

'''
steps = [ [True , False, False, False],\
          [False, False, True , False],\
          [False, True , False, False],\
          [False, False, False, True ]]
'''

# Basic initialization
currentStep = 0
GPIO.setmode(GPIO.BCM)
GPIO.setup(BLK, GPIO.OUT)
GPIO.setup(GRN, GPIO.OUT)
GPIO.setup(RED, GPIO.OUT)
GPIO.setup(BLU, GPIO.OUT)



def set_step(step):
  '''
    Sets which step the motor is on.
    
    Step should be an integer between 0 and 7.
  '''

  if step in range(len(steps)):
    GPIO.output(BLK, steps[step][0])
    GPIO.output(GRN, steps[step][1])
    GPIO.output(RED, steps[step][2])
    GPIO.output(BLU, steps[step][3])
    print(step)
    print(steps[step])
  else:
   raise ValueError("Notch should be an integer between 0 and 7")

def rotate(angle, speed):
  '''
  Rotates motor continuously.
  
  Angle is measured in degrees and specified as an int or float.
  It can be positive or negative and of any magnitude.
  
  Speed is measured in degrees per second and specified as an in or float.
  It must be positive.
  '''
  global currentStep
  
  numSteps = int(angle / 0.9)
  period = 0.9 / speed
  
  for x in range(numSteps):
    set_step(currentStep)  
    if angle > 0:
      currentStep = (currentStep + 1) % len(steps)
    elif angle < 0:
      currentStep = (currentStep - 1) % len(steps)
    sleep(period)
    
    
if __name__ == "__main__":
  set_step(0)
  rotate(180,500)

  GPIO.cleanup()
    
    
    
  

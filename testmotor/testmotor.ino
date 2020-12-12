// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>
#define dirPin1 5
#define stepPin1 4
#define dirPin2 7
#define stepPin2 6
#define enablePin1 8
#define enablePin2 9

#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
//AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup()
{  
//   stepper.setMaxSpeed(100);
//   stepper.setSpeed(100);	
    pinMode(stepPin1,OUTPUT); 
    pinMode(dirPin1,OUTPUT);
    pinMode(stepPin2,OUTPUT); 
    pinMode(dirPin2,OUTPUT);
    pinMode(enablePin1,OUTPUT); 
    pinMode(enablePin2,OUTPUT);
    digitalWrite(dirPin1,HIGH);
    digitalWrite(dirPin2,HIGH);
    digitalWrite(enablePin1,HIGH);
    digitalWrite(enablePin2,HIGH);
}

void loop()
{  
  digitalWrite(enablePin1,LOW);
  for(int x= 0; x<200; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stepPin1,HIGH); //Trigger one step forward
    delay(10);
    digitalWrite(stepPin1,LOW); //Pull step pin low so it can be triggered again
    delay(10);
  }
  digitalWrite(enablePin1,HIGH);

  digitalWrite(enablePin2,LOW);
  for(int x= 0; x<200; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stepPin2,HIGH); //Trigger one step forward
    delay(5);
    digitalWrite(stepPin2,LOW); //Pull step pin low so it can be triggered again
    delay(5);
  }
  digitalWrite(enablePin2,HIGH);
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(300); 
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(300);

}

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "multiplexer.h"
#include "debugger.h"

// This isn't the greatest placement
// We should add a RobotArm class so we can clean this up
Multiplexer mux = Multiplexer();
Debugger debug = Debugger(&mux);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Enable or disable debugger
boolean runDebugger = true;

void setup() 
{ 
  Serial.begin(9600); // 115200 is max
  
  // Setup mux
  mux = Multiplexer();
  
  // Setup pwm
  pwm = Adafruit_PWMServoDriver();
  pwm.begin();

  // Set the frequency for the pwm driver
  pwm.setPWMFreq(60);

  // Setup debugger if it is enabled
  if(runDebugger)
  {
    debug = Debugger(&mux);
  }
}

void loop() 
{
  if(runDebugger)
  {
    debug.loop();
  }
}

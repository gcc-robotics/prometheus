#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "multiplexer.h"

Multiplexer mux;
Adafruit_PWMServoDriver pwm;

// Globals
boolean debug = true; // While true, serial will display diagnostic information, as well as enable debug commands.

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
}

void loop() 
{
  if(debug)
  {
    diagnosticLoop();
  }
}

// Analog signal to encoder angle
float analogSignalToAngle(float input)
{
  return input / 1023.0 * 360.0;
}

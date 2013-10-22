#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


#include "multiplexer.h"
#include "debugger.h"

// This isn't the greatest placement
// We should add a RobotArm class so we can clean this up
Multiplexer mux = Multiplexer();
MotorController motor = MotorController();
Debugger debug = Debugger(&mux, &motor);

// Enable or disable debugger
boolean runDebugger = true;

void setup() 
{ 
  Serial.begin(9600); // 115200 is max
  motor.setup();
}

void loop() 
{
  //Serial.println("test");
  //delay(500);
  
  if(runDebugger)
  {
    debug.loop();
  }
}

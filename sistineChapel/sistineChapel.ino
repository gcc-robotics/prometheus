
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Globals
int debug = true; // While true, serial will display diagnostic information, as well as enable debug commands.

void setup() 
{ 
  Serial.begin(9600); // 115200 is max
  
  pwm.begin();
  pwm.setPWMFreq(60); //sets the frequency for the pwm driver
}

void loop() 
{
  //Serial.println(Serial.readString());
  delay(1000);
  
  if(debug)
  {
    diagnosticLoop();
  }
}


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Globals
int debug = true; // While true, serial will display diagnostic information, as well as enable debug commands.
void setup() 
{ 
  Serial.begin(115200);
  
  pwm.begin();
  pwm.setPWMFreq(60); //sets the frequency for the pwm driver
  if(debug == true)
  {
    enableDiagnostics();
  }
}

void loop() 
{
  diagnosticLoop();
}

#include <Wire.h>

// Globals
int debug = true; // While true, serial will display diagnostic information, as well as enable debug commands.

void setup() 
{ 
  Serial.begin(9600); // 115200 is max

  muxSetup();
}

void loop() 
{
  //delay(1000);
  
  if(debug)
  {
    diagnosticLoop();
  }
}

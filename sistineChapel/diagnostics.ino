int debugInputValue;
int debugMotorNumber;
int debugDutyCycle;
int debugState;

/* debugState values
0 = select state
1 = Waiting for motor number
2 = Waiting for duty cycle
*/

// Initilize debug variables
void configureDebug()
{
  debugState = 0;
}

// Called in loop() when debug == true
void diagnosticLoop()
{ 
  switch(debugState)
  {
    case 0:
      debugInputValue = Serial.parseInt();
      
      if(debugInputValue == 1)
      {
        Serial.println("Running Pin Status: ");
        d_pinReadout();
      }
      else if(debugInputValue == 2)
      {
        debugState = 1;
      }
      else
      {
        Serial.println("Input 1 to show the status of the digital input, 2 to set Motor PWM.");
      }
      break;
    case 1:
      Serial.println("Input Motor Number between 1 and 5");
      debugInputValue = Serial.parseInt();
      
      if(debugInputValue != 0)
      {
        debugMotorNumber = debugInputValue;
        
        debugState = 2;
      }
      break;
    
    case 2:
      Serial.println("Input Duty Cycle between 1 and 100, -1 for 0%");
      debugInputValue = Serial.parseInt();
      
      if(debugInputValue != 0)
      {
        debugDutyCycle = debugInputValue;
        
        forward(debugMotorNumber);
        setPwm(debugMotorNumber, debugDutyCycle);
        
        Serial.print("Setting motor ");
        Serial.print(debugMotorNumber);
        Serial.print(" to Duty Cycle ");
        Serial.print(debugDutyCycle);
        Serial.println("%");
        
        debugState = 0;
      }
      break;
  }
}

// Print a readout on all current pin status
void d_pinReadout()
{
  for(int i = 0; i < 8; i++)
  {
    Serial.print("Status of Pin: ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(digitalRead(i));
  }
}

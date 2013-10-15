// Initialize diagnostic setup
void enableDiagnostics()
{
}
// Print a readout on all current pin status
void d_pinReadout()
{
  for(int i = 0; i < 8; i++)
  {
    Serial.print("Status of Pin: ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.print(digitalRead(i));
  }
}
void diagnosticLoop()
{
  if(debug == true)
  {
    int num = Serial.read();
    if(num == 1)
    {
      Serial.print("Running Pin Status: ");
      d_pinReadout();
    }
    else if(num == 2)
    {
      Serial.print("Input Duty Cycle");
      int duty = Serial.read();
      Serial.print("Input Motor Number or '10' for All");
      int motor = Serial.read();
      for(int j = 0; j < motor; j++)
      {
        forward(j);
        setPwm(j, duty);
      }
    }
  }
}
  
  

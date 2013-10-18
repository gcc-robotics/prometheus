// Digital pins that control the analog mux
int muxPin0 = 4;
int muxPin1 = 5;
int muxPin2 = 6;
int muxPin3 = 7;

// Analog pin that reads the signal from the mux
int muxSigPin = A0;

void muxSetup() 
{ 
  pwm.begin();

  // Set the frequency for the pwm driver
  pwm.setPWMFreq(60); 
  
  // Set the mux control pins as outputs
  pinMode(muxPin0, OUTPUT);
  pinMode(muxPin1, OUTPUT);
  pinMode(muxPin2, OUTPUT);
  pinMode(muxPin3, OUTPUT);

  // Default the outputs to low
  digitalWrite(muxPin0, LOW);
  digitalWrite(muxPin1, LOW);
  digitalWrite(muxPin2, LOW);
  digitalWrite(muxPin3, LOW);
  
  // Set the mux input pin as an INPUT
  pinMode(muxSigPin, INPUT);
}

// Returns the value of the specified channel from the mux
int readMux(int channel)
{
  int controlPin[] = {muxPin0, muxPin1, muxPin2, muxPin3};

  int muxChannel[16][4] = {
    {0,0,0,0}, // Channel 0
    {1,0,0,0}, // Channel 1
    {0,1,0,0}, // Channel 2
    {1,1,0,0}, // Channel 3
    {0,0,1,0}, // Channel 4
    {1,0,1,0}, // Channel 5
    {0,1,1,0}, // Channel 6
    {1,1,1,0}, // Channel 7
    {0,0,0,1}, // Channel 8
    {1,0,0,1}, // Channel 9
    {0,1,0,1}, // Channel 10
    {1,1,0,1}, // Channel 11
    {0,0,1,1}, // Channel 12
    {1,0,1,1}, // Channel 13
    {0,1,1,1}, // Channel 14
    {1,1,1,1}  // Channel 15
  };

  // Set the mux control pins
  for(int i = 0; i < 4; i ++)
  {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  // Read the value at the SIG pin
  int val = analogRead(muxSigPin);

  return val;
}

// Analog signal to encoder angle
/*float readMuxInput(int inputNum)
{
  int analogAngle = readMux(encNum);
  float currentAngle = analogAngle*(360.0/1024.0);
  return currentAngle;
}*/

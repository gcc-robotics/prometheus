
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


int LED1 = 11;
int LED2 = 12;
int emergencyState = false;

//Digital pins that control the analog mux
int s0=4;
int s1=5;
int s2=6;
int s3=7;
//analog pin that reads the signal from the mux
int SIG_pin= A0;

float targetAngle = 120;
float angle[]= {0,0,0,0,0};
float ITerm[]= {0,0,0,0,0};
float PID_Gain[5][3]={
                      {.25,.01,.02},
                      {.28,.1,.04},
                      {.16,.01,0.02},
                      {.28,.01,.01},
                      {.17,.01,.01}
                      };
float angleErrorLast[]= {0,0,0,0,0};
float timeLast[]={0,0,0,0,0};
float currentAngle;
float angleError;

float kp = 1.5, ki =0, kd = 0;
float timeNow, timeDifference;

float PTerm, DTerm;
int PIDTuningMotor = 0, PIDSelector;


void setup() 
{ 
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(60); //sets the frequency for the pwm driver
  
  //sets mux control pins as outputs
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  //defaults the outputs to low
  digitalWrite(s0,LOW);
  digitalWrite(s1,LOW);
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  pinMode(10, INPUT);
  
  attachInterrupt(1, ESR, CHANGE );

}
void loop() 
{
  while(emergencyState == true)
  {
    brake(1);
    brake(2);
    brake(3);
    brake(4);
    brake(5);
    if(digitalRead(10)==HIGH)
      emergencyState = false;
  }
  
  
  motorControl(1);
  motorControl(2);
  motorControl(3);
  motorControl(4);
  motorControl(5);
  

  if(digitalRead(10)==HIGH)
  {
    ITerm[0] = 0;
    ITerm[1] = 0;
    ITerm[2] = 0;
    ITerm[3] = 0;
    ITerm[4] = 0;
    
    if(angle[4] == 120)
    {
      angle[0] = 30;
      angle[1] = 30;
      angle[2] = 30;
      angle[3] = 30;
      angle[4] = 30;
    }
    else 
    {
      angle[0] = 120;
      angle[1] = 120;
      angle[2] = 120;
      angle[3] = 120;
      angle[4] = 120;
    }
  }
 
  
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
}

//Tells the motor driver to run the motors forward
void forward(int motorNum)
{
  int pin = 3*(motorNum-1); //selects the first pin on the pwm driver       
  pwm.setPWM(pin,4095,0); //Sets the INA pin of the motor to LOW
  pwm.setPWM(pin+1,0,4095);//Sets the INB pin of the motor to High
}

//Tells the motor driver to run the motors in reverse
void reverse(int motorNum)
{
  int pin = 3*(motorNum-1);//selects the first pin on the pwm driver
  pwm.setPWM(pin,0,4095);//Sets the INA pin of the motor to HIGH
  pwm.setPWM(pin+1,4095,0);//Sets the INB pin of the motor to LOW
}

void brake(int motorNum)
{
  int pin = 3*(motorNum-1); //selects the first pin on the pwm driver       
  pwm.setPWM(pin,0,4095); //Sets the INA pin of the motor to High
  pwm.setPWM(pin+1,0,4095);//Sets the INB pin of the motor to High
}
//Sets the PWM duty cycle for the desired motor
void setPwm(int motorNum, float dutyCycle)
{
  if(dutyCycle > 100)
    dutyCycle = 100;
  if(dutyCycle < 0)
    dutyCycle = 0;
  int pin = 3*(motorNum-1); //selects the first pin ont he pwm driver
  float rate = dutyCycle*40.95;//calculates the integer value for the desired duty cycle
  pwm.setPWM(pin+2,0,int(rate));
}

//returns the current angle on the specified encoder
float readEncoder(int encNum)
{
  int analogAngle = readMux(encNum);
  float currentAngle = analogAngle*(360.0/1024.0);
  return currentAngle;
}

//return the value of the FSR
int readFsr()
{
  int FSRvalue = readMux(0);
  return FSRvalue;
}

//returns the value of the specified channel fromt he mux
int readMux(int channel)
{
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pin);

  //return the value
  return val;
}

void ESR ()
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  emergencyState = true;
  
  
}

void chooseDirection(int motorNum)
{  
  if((angleError >= -359 && angleError < -180)||(angleError >= 0 && angleError <= 180))
    forward(motorNum);
  else
    reverse(motorNum);
}
void motorControl(int motorNum)
{
  Serial.print("Motor ");
  Serial.print(motorNum);
  
  //sets the target angle according to motor being actuated
  targetAngle = angle[motorNum-1];
  Serial.print("\tTarget Angle ");
  Serial.print(targetAngle);
    
  currentAngle = readEncoder(motorNum);
  Serial.print("\tCurrent Angle ");
  Serial.print(currentAngle);
  
  angleError = currentAngle - targetAngle;
  chooseDirection(motorNum);
  if (abs(angleError)< 2)
    angleError = 0;
  setPID(motorNum);
  
}
void setPID(int motorNum)
{
/*
  kp = (float(analogRead(A1)-91)/932)*1;
  Serial.print("\tkp ");
  Serial.print(kp);
  
  ki = (float(analogRead(A2)-89)/934)*0.5;
  Serial.print("\t\tki ");
  Serial.print(ki);
  
  kd = (float(analogRead(A3)-88)/935)*0.5;
  Serial.print("\tkd ");
  Serial.print(kd);
 */ 
  //if(angleError < 0)
  //  angleError += 360;
  //determine the P term
  PIDTuning();
    
  PTerm = abs(angleError)*(PID_Gain[motorNum-1][0]);
  Serial.print("\tkp ");
  Serial.print(PID_Gain[motorNum-1][0]);
  
  //determine the time interval
  timeNow = millis();
  timeDifference = (timeNow - timeLast[motorNum-1])/1000;
  //determine the I term
  ITerm[motorNum-1] += timeDifference*abs(angleError)*(PID_Gain[motorNum-1][1]);
  Serial.print("\tki ");
  Serial.print(PID_Gain[motorNum-1][1]);
  
  if(angleError ==0)
    ITerm[motorNum-1] = 0;
  
  //determine the D term
  DTerm = (abs(angleErrorLast[motorNum-1])-abs(angleError))*(PID_Gain[motorNum-1][2])/timeDifference;
  Serial.print("\tkd ");
  Serial.print(PID_Gain[motorNum-1][2]);
  
  
  Serial.print("\tPTerm ");
  Serial.print(PTerm);
  
  Serial.print("\tITerm ");
  Serial.print(ITerm[motorNum-1]);
  
  Serial.print("\tDTerm ");
  Serial.print(DTerm);
  
  Serial.print("\tDutyCycle");
  
  setPwm(motorNum,PTerm + ITerm[motorNum-1] + DTerm);
  Serial.println(PTerm + ITerm[motorNum-1] + DTerm);
  
  timeLast[motorNum-1] = timeNow;
  angleErrorLast[motorNum-1] = angleError;
}

void PIDTuning()
{ 
  if(readMux(7)<100)
  {
    
    PIDSelector = readMux(8);
    if(PIDSelector >= 674 && PIDSelector < 732)
      PIDTuningMotor = 0;
    else if(PIDSelector >= 732 && PIDSelector < 790)
      PIDTuningMotor = 1;
    else if(PIDSelector >= 790 && PIDSelector < 848)
      PIDTuningMotor = 2;
    else if(PIDSelector >= 848 && PIDSelector < 906)
      PIDTuningMotor = 3;
    else if(PIDSelector >= 906 && PIDSelector < 964)
      PIDTuningMotor = 4;
    else if(PIDSelector >= 732 && PIDSelector <= 1022)
      PIDTuningMotor = 5;
      
    PIDLight();
      
    /*
    while(readMux(9))
    {
      PID_Gain[PIDTuningMotor-1][0] = (float(analogRead(A1)-91)/932)*1;
      PID_Gain[PIDTuningMotor-1][1] = (float(analogRead(A2)-89)/934)*0.5;
      PID_Gain[PIDTuningMotor-1][2] = (float(analogRead(A3)-88)/935)*0.5;
    }
    
    Serial.print("\tkp ");
    Serial.print(PID_Gain[PIDTuningMotor-1][0]);
    Serial.print("\tPOT1 ");
    Serial.print(((analogRead(A1)-91)/932)*1);
    Serial.print("\tki ");
    Serial.print(PID_Gain[PIDTuningMotor-1][1]);
    Serial.print("\tPOT2 ");
    Serial.print(((analogRead(A2)-89)/934)*0.5);
    Serial.print("\tkd ");
    Serial.print(PID_Gain[PIDTuningMotor-1][2]);
    Serial.print("\tPOT3 ");
    Serial.print(((analogRead(A3)-88)/935)*0.5);
    */
  }
}
void PIDLight()
{
  if(PIDTuningMotor == 0)
  {
    digitalWrite(2,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
  }
  else if(PIDTuningMotor == 1)
  {
    digitalWrite(2,HIGH);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
  }
  else if(PIDTuningMotor == 2)
  {
    digitalWrite(2,LOW);
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
  }
  else if(PIDTuningMotor == 3)
  {
    digitalWrite(2,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
  }
  else if(PIDTuningMotor == 4)
  {
    digitalWrite(2,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
  }
  else if(PIDTuningMotor == 5)
  {
    digitalWrite(2,HIGH);
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
  }
}




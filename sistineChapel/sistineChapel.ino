
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() 
{ 
  Serial.begin(115200);
  
  pwm.begin();
  pwm.setPWMFreq(60); //sets the frequency for the pwm driver
}

void loop() 
{
  forward(1);
  forward(2);
  forward(3);
  forward(4);
  forward(5);
  setPwm(1);
  setPwm(2);
  setPwm(3);
  setPwm(4);
  setPwm(5);
}

//Tells the motor driver to run the motors forward
void forward(int motorNum)
{
  int pin = 3 * (motorNum - 1); //selects the first pin on the pwm driver
    
  pwm.setPWM(pin, 4095, 0); //Sets the INA pin of the motor to LOW
  pwm.setPWM(pin + 1, 0, 4095);//Sets the INB pin of the motor to High
}

//Tells the motor driver to run the motors in reverse
void reverse(int motorNum)
{
  int pin = 3 * (motorNum - 1);//selects the first pin on the pwm driver
  
  pwm.setPWM(pin, 0, 4095);//Sets the INA pin of the motor to HIGH
  pwm.setPWM(pin + 1, 4095, 0);//Sets the INB pin of the motor to LOW
}

void brake(int motorNum)
{
  int pin = 3 * (motorNum - 1); //selects the first pin on the pwm driver
  
  pwm.setPWM(pin, 0, 4095); //Sets the INA pin of the motor to High
  pwm.setPWM(pin + 1, 0, 4095);//Sets the INB pin of the motor to High
}

//Sets the PWM duty cycle for the desired motor
void setPwm(int motorNum, float dutyCycle)
{
  if(dutyCycle > 100)
    dutyCycle = 100;
  
  if(dutyCycle < 0)
    dutyCycle = 0;
  
  int pin = 3 * (motorNum-1); //selects the first pin on the pwm driver
  float rate = dutyCycle * 40.95; //calculates the integer value for the desired duty cycle
  
  pwm.setPWM(pin + 2, 0, int(rate));
}

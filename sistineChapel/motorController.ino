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

/*
int getPwm(int motorNum)
{
  Serial.
}
*/

//INPUT PARAMETERS : (integer) Motor Number Identifier
// On Call Motor is set to move Forward.
void forward(int motorNum)
{
  int pin = 3 * (motorNum - 1); //selects the first pin on the pwm driver
    
  pwm.setPWM(pin, 4095, 0); //Sets the INA pin of the motor to LOW
  pwm.setPWM(pin + 1, 0, 4095);//Sets the INB pin of the motor to High
}

//INPUT PARAMETERS : (integer) Motor Number Identifier
// On Call Motor is set to move Reverse
void reverse(int motorNum)
{
  int pin = 3 * (motorNum - 1);//selects the first pin on the pwm driver
  
  pwm.setPWM(pin, 0, 4095);//Sets the INA pin of the motor to HIGH
  pwm.setPWM(pin + 1, 4095, 0);//Sets the INB pin of the motor to LOW
}

//INPUT PARAMETERS : (integer) Motor Number Identifier
// On call motor is reset, all pins set to HIGH. Motor is Stopped.
void brake(int motorNum)
{
  int pin = 3 * (motorNum - 1); //selects the first pin on the pwm driver
  
  pwm.setPWM(pin, 0, 4095); //Sets the INA pin of the motor to High
  pwm.setPWM(pin + 1, 0, 4095);//Sets the INB pin of the motor to High
}

#include <Servo.h>
Servo myservo;  // create servo object to control a servo

void ServoInitial()
{
  pinMode(Pin_SERVO,OUTPUT);
  ServoState = Close;
  myservo.attach(Pin_SERVO);
  ServoCalc();
  }

void ServoCalc()
{
  if (ServoState == Open)
  {
    myservo.write(90); 
    }else
  {
    myservo.write(0); 
    }
  }


void BuzzerInitial()
{
    pinMode(Pin_Buzzer,OUTPUT);
}

//ブザーの実行
void BuzzerCalc()
{
    if(Timer10ms.T.BuzzerOn != 0)
    {
        digitalWrite(Pin_Buzzer,HIGH);
        //Serial.println("High");
    }else
    {
        digitalWrite(Pin_Buzzer,LOW);        
        //Serial.println("Low");
    }
    
}

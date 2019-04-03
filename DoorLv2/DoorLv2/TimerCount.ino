

void TimerCounter()
{
  /*
  if(SWcalcTime > 0) SWcalcTime--;
  if(OpeningTime > 0) OpeningTime--;
  if(BuzzerTimer > 0) BuzzerTimer--;
  if(InputTime > 0) InputTime--;*/
  for(int i = 0; i < 5; i++)
  {
    if(Timer10ms.TimerAry[i] > 0) Timer10ms.TimerAry[i]--;
  }
}


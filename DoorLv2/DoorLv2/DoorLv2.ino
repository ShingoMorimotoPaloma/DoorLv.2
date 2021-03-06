

#include <MsTimer2.h>
#include "define.c"

const unsigned long E_Timing_OpeningTime = 1000; //扉が開いている時間は10秒
const unsigned long E_Timing_OpeningBuzzer = 300; //開いている時間の最後3秒はブザー鳴動
const unsigned long E_Timing_InputTime = 1000; //入力待ち時間は10秒
TM Timer10ms;
int MainState = (int)Closed;

extern int LcdState;
extern int PasscodeCharCount;
int KeyCounter = 0;

bool FlgTest = false;

void setup() {

  TestCalc();

  // put your setup code here, to run once:
  MsTimer2::set(10, TimerCounter); 
  MsTimer2::start();
  ServoInitial();
  BuzzerInitial();
  LcdSetup();
  LcdCalc();
  //EepromInitial();
  //IntegrationTestEepromInitial();
  for(int i = 0; i < 4; i++)
  {
    AlKey[i] = '0';
  }
  
}

void loop() {
  
  ServoCalc(); //サーボモーターの処理
  BuzzerCalc();
  LcdCalc();
  EepromCalc();
  MainStateSwitch();  //メイン状態遷移処理

}

void MainStateSwitch()
{
  switch(MainState)
  {
    case Closed:
      MainState_01_Closed();
      break;
    case Opened:
      MainState_02_Opened();
      break;
    case InputReady:
      MainState_03_InputReady();
      break;
    case InvaildPasscord:
      MainState_04_InvailPasscord();
      break;
    case InputOldKey:
      MainState_05_InputOldKey();
      break;
    case InputNewKey1:
      MainState_06_InputNewKey1();
      break;
    case InputNewKey2:
      MainState_07_InputNewKey2();
      break;
    case Succsess:
      MainState_08_Succsess();
      break;
    case Failed:
      MainState_09_Failed();
      break;
    }
  }
void MainState_01_Closed()
{
  char customKey = GetKey();
  if (customKey == '*'){
    KeyCounter = 0;
    PasscodeCharCount = 0;
    MainState = InputReady;
    Timer10ms.T.Input = E_Timing_InputTime;
    Timer10ms.T.BuzzerOn = 5;
    BufClear();
  }
  if (customKey == '#') {
    KeyCounter = 0;
    PasscodeCharCount = 0;
    MainState = InputOldKey;
    Timer10ms.T.Input = E_Timing_InputTime;
    Timer10ms.T.BuzzerOn = 5;
    BufClear();   
  }
  
  LcdState = 0;
  ServoState = Close;
  }
void MainState_02_Opened()
{
  if (Timer10ms.T.Opening <= E_Timing_OpeningBuzzer)
  {
      Timer10ms.T.BuzzerOn = 10; //100ms以内にループしてくることを期待
  }
  if (Timer10ms.T.Opening == 0)//タイムアップ
  {
    MainState = Closed;
    Timer10ms.T.BuzzerOn = 0;  //ブザー強制停止
    }
  LcdState = 4;
  ServoState = Open;
  }
void MainState_03_InputReady()
{
  char customKey = GetKey();
  if (customKey){
    KeyCounter++;
    PasscodeCharCount++;
    Timer10ms.T.BuzzerOn = 5;
    BufShift(customKey,KeyBuf);
  }

  if (Timer10ms.T.Input == 0)
  {
    MainState = Closed;
    Timer10ms.T.BuzzerOn = 100;
  }
  if (KeyCounter >= 4)
  {
    if (KeyMach(KeyBuf,AlKey))
    {
      MainState = Opened;
      Timer10ms.T.Opening = E_Timing_OpeningTime;
      Timer10ms.T.BuzzerOn = 30; //ブザーを300ms鳴らす    
    }else
    {
      MainState = InvaildPasscord;
      Timer10ms.T.InvaildAlert = 300;
      Timer10ms.T.BuzzerOn = 30; //ブザーを300ms鳴らす  
    }
    
  }
  LcdState = 2;
  ServoState = Close;
}

void MainState_04_InvailPasscord()
{
  Timer10ms.T.BuzzerOn = 10;
  if (Timer10ms.T.InvaildAlert == 0)
  {
    MainState = Closed;
    Timer10ms.T.BuzzerOn = 0;
  }

  LcdState = 3;
  ServoState = Close;
}
void MainState_05_InputOldKey()
{
  char customKey = GetKey();
  if (customKey){
    KeyCounter++;
    PasscodeCharCount++;
    Timer10ms.T.BuzzerOn = 5;
    BufShift(customKey,OldKeyOne);
  }

  if (KeyCounter >= 4)
  {
      KeyCounter = 0;
      PasscodeCharCount = 0;
      MainState = InputNewKey1;
      Timer10ms.T.BuzzerOn = 15; //ブザーを150ms鳴らす    
  }
  LcdState = 5;
  ServoState = Close;
}
void MainState_06_InputNewKey1()
{
  char customKey = GetKey();
  if (customKey){
    KeyCounter++;
    PasscodeCharCount++;
    Timer10ms.T.BuzzerOn = 5;
    BufShift(customKey,NewKeyOne);
  }

  if (KeyCounter >= 4)
  {
      KeyCounter = 0;
      PasscodeCharCount = 0;
      MainState = InputNewKey2;
      Timer10ms.T.BuzzerOn = 15; //ブザーを150ms鳴らす    
  }
  LcdState = 6;
  ServoState = Close;
}
void MainState_07_InputNewKey2()
{
  char customKey = GetKey();
  if (customKey){
    KeyCounter++;
    PasscodeCharCount++;
    Timer10ms.T.BuzzerOn = 5;
    BufShift(customKey,NewKeyTwo);
  }

  if (KeyCounter >= 4)
  {
    Serial.begin(9600);
    Serial.println(AlKey);
    Serial.println(OldKeyOne);
    Serial.println(NewKeyOne);
    Serial.println(NewKeyTwo);
    Serial.end();
    if (KeyMach(AlKey,OldKeyOne) & KeyMach(NewKeyOne,NewKeyTwo)) {
      Timer10ms.T.BuzzerOn = 50;
      Timer10ms.T.Display = 200;
      MainState = Succsess;
      for(int i = 0; i < 4; i++)
      {
        AlKey[i] = NewKeyOne[i];
      }
      
    }
    else {
      Timer10ms.T.BuzzerOn = 100;
      Timer10ms.T.Display = 200;
      MainState = Failed;
    }
    
  }
  LcdState = 7;
  ServoState = Close;
}

void MainState_08_Succsess()
{
  if (!Timer10ms.T.Display) {
    MainState = Closed;
  }
  
  LcdState = 8;
  ServoState = Close;
}
void MainState_09_Failed()
{
  if (!Timer10ms.T.Display) {
    MainState = Closed;
  }
  
  LcdState = 9;
  ServoState = Close;
}

bool KeyMach(char a[4],char c[4])
{
  bool b = true;
  for (int i = 0;i < 4;i++)
  {
    if (a[i] != c[i]) b = false;
    }  
  return b;
}
  

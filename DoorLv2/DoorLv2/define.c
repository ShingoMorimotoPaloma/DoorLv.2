

const int Pin_SERVO = 3;
const int Pin_Buzzer = 2;
int LED_State;// = RED;
//入力キーバッファ
char KeyBuf[4];
//正解のキー
char AlKey[4];
//古いパスコードバッファ
char OldKeyOne[4];
//新しいパスコードバッファ1
char NewKeyOne[4];
//新しいパスコードバッファ2
char NewKeyTwo[4];
//EEPROMに書き込むバッファ
char EepromBackupKey[4];

//↓タイマー郡

union TM
{
  unsigned long TimerAry[4];
  struct Timer
  {
    unsigned long Opening;
    unsigned long InvaildAlert;
    unsigned long BuzzerOn;
    unsigned long Input;
  }T;
};


enum TypeOfState 
{
  Closed,
  Opened,
  InputReady,
  InvaildPasscord
  };

enum TypeOfServo
{
  Close,Open
  };
int ServoState;

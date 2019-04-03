#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {19, 18, 17, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {15, 14, 6, 5}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char GetKey()
{
  return customKeypad.getKey();
}

void BufShift(char key,char (&buf)[4])
{
  buf[0] = KeyBuf[1];
  buf[1] = KeyBuf[2];
  buf[2] = KeyBuf[3];
  buf[3] = key;
  }

void BufClear()
{
  for (int i = 0;i < 4;i++)
  {
    KeyBuf[i] = 0;
    }
  }

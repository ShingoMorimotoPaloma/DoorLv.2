#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int AAAAA;
String LcdString[2] = {"",""};
String BeforeLcdString[2] = {"",""};

int LcdState = 0;
int PasscodeCharCount = 0;
void LcdSetup(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}

void LcdCalc(){
    switch (LcdState)
    {
        case 0:
            LcdState00Wait();
            break;
        case 1:
            LcdState01InputPasscode();
            break;
        case 2:
            LcdState01InputPasscode();
            break;
        case 3:
            LcdState03InvalidPasscode();
            break;
        case 4:
            LcdState04OpeningDoor();
            break;
        case 5:
            LcdState05InputOldPasscode();
            break;
        case 6:
            LcdState06InputNewPasscode1();
            break;
        case 7:
            LcdState07InputNewPasscode2();
            break;
        case 8:
            LcdState08ChangePassSuccsess();
            break;
        case 9:
            LcdState09ChangePassFailed();
            break;
        default:
            break;
    }
  LcdFlush();
}

void LcdState00Wait()
{
    lcd.noDisplay();
    LcdString[0] = "";
    LcdString[1] = ""; 
}
void LcdState01InputPasscode()
{
    lcd.display();
    LcdString[0] = "Input Passcode";
    LcdString[1] = StarChar(); 
}
void LcdState03InvalidPasscode()
{
    lcd.display();
    LcdString[0] = "Invalid Passcode";
    LcdString[1] = ""; 
}
void LcdState04OpeningDoor()
{
    lcd.display();
    LcdString[0] = "Opening Door";
    LcdString[1] = GetTimerString(); 
}
void LcdState05InputOldPasscode()
{
    lcd.display();
    LcdString[0] = "Input Old Pass";
    LcdString[1] = StarChar(); 
}
void LcdState06InputNewPasscode1()
{
    lcd.display();
    LcdString[0] = "Input New Pass";
    LcdString[1] = StarChar(); 
}
void LcdState07InputNewPasscode2()
{
    lcd.display();
    LcdString[0] = "Retype New Pass";
    LcdString[1] = StarChar(); 
}
void LcdState08ChangePassSuccsess()
{
    lcd.display();
    LcdString[0] = "Changed Passcode";
    LcdString[1] = "";
}
void LcdState09ChangePassFailed()
{
    lcd.display();
    LcdString[0] = "Change Passcode";
    LcdString[1] = "Faild";
}

//Lcdの表示を書き換える
void LcdFlush(){
    for(int i = 0; i < 2; i++)
    {
        if (LcdString[i] != BeforeLcdString[i]) {
            ClearCol(i);
            lcd.setCursor(0, i);
            lcd.print(LcdString[i]);
        }
        BeforeLcdString[i] = LcdString[i];
    }
    
}
void ClearCol(int column){
    lcd.setCursor(0,column);
    lcd.print("                ");
}

String StarChar()
{
    switch (PasscodeCharCount)
    {
        case 0:
            return "";
            break;
        case 1:
            return "*";
            break;
        case 2:
            return "**";
            break;
        case 3:
            return "***";
            break;    
        default:

            break;
    }
    return "****";
}

String GetTimerString()
{
    int t;
    t = Timer10ms.T.Opening / 100;
    t = t + 1;
    String stringOne =  String(t, DEC);
    stringOne = String(stringOne + " seconds");
    return stringOne;
}

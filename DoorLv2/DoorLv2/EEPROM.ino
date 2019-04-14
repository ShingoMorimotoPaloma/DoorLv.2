#include <EEPROM.h>


int EAddressKeyA[] = {0,1,2,3};
int EAddressKeyB[] = {5,6,7,8};


char CheckSumA;
char CheckSumB;
int EAddressCheckSumA = 4;
int EAddressCheckSumB = 9;

//EEPROMのパリティエラー
bool FlgEepromError = false;
bool FlgEepromWrite = false;
int PointOfEepWrite = 0;


//EEPROMのイニシャライズ
//setup関数から呼ばれることを期待
//EEPROMエラー判定も実行
void EepromInitial()
{   Serial.begin(9600);
    for(int i = 0;i < 4; i++)
    {
        byte b = EEPROM.read(EAddressKeyA[i]);
        if (!CheckParity(b)) FlgEepromError = true;
        AlKey[i] = (char)(b);
    }
    byte b = EEPROM.read(EAddressCheckSumA);
    if (!CheckParity(b)) FlgEepromError = true;
    if(!SumCheck())
    {
        for(int i = 0;i < 4; i++)
        {
            byte b = EEPROM.read(EAddressKeyB[i]);
            if (!CheckParity(b)) FlgEepromError = true;
            AlKey[i] = (char)(b);
        }        
    }
        //IntegrationTestEepromInitial();
}


//チェックサムの確認
bool SumCheck()
{
    unsigned short A[5];
    for(int i = 0; i < 4; i++)
    {
        A[i] = (unsigned short)AlKey[i];
    }
    A[4] = (unsigned short)CheckSumA;
    unsigned short t = A[0] + A[1] + A[2] + A[3];
    t = t & 0b0000000001111111;
    if (t == A[4]) {
        return true;
    }else
    {
        return false;
    }       
}

void EepWrite(int Address,char value)
{
    byte t = AddParity(value);
    EEPROM.write(Address,t);
}

void EepromCalc()
{
    CheckSumCalc(); //とりあえずチェックサムは毎回更新
    if (!KeyMach(EepromBackupKey,AlKey)) 
    {
        FlgEepromWrite = true;
        PointOfEepWrite = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        EepromBackupKey[i] = AlKey[i];
    }
    if (!FlgEepromWrite) {
        return;
    }
    if (PointOfEepWrite < 4) {
        EepWrite(EAddressKeyA[PointOfEepWrite],AlKey[PointOfEepWrite]);
        PointOfEepWrite++;
        return;
    }
    if (PointOfEepWrite == 4) {
        EepWrite(EAddressCheckSumA,CheckSumA);
        PointOfEepWrite++;
        return;
    }   
    if (PointOfEepWrite < 9) {
        EepWrite(EAddressKeyB[PointOfEepWrite - 5],AlKey[PointOfEepWrite - 5]);
        PointOfEepWrite++;
        return;
    }
    EepWrite(EAddressCheckSumB,CheckSumB);
    FlgEepromWrite = false;
}


//パリティを付加する
byte AddParity(char c)
{
    byte b = (byte)c;

    if (ParityCalc(b)) {
        b = (b | 0b10000000);
    }else
    {
        b = (b & 0b01111111); 
    }
    return b;
    
}
//パリティの計算
bool ParityCalc(char a)
{
    bool p = false;
    bool v = false;
    byte w = (byte)a;
    byte r = 0b01000000;
    for(int i = 0; i < 7; i++)
    {
        
        if ((r & w) != 0) 
        {
            v = true;
        }
        else
        {
            v = false;
        }
        p = p ^ v;
        r =  r >> 1;
        
    }

    return p;
    
}
//パリティが正しいかを判定する
bool CheckParity(byte v)
{   char t = (char)v;
    byte y = AddParity(t);
    if (v == y) {
        return true;
    }
    return false;
        
}

void CheckSumCalc()
{
    unsigned short A[5];
    for(int i = 0; i < 4; i++)
    {
        A[i] = (unsigned short)AlKey[i];
    }
    A[4] = (unsigned short)CheckSumA;
    unsigned short t = A[0] + A[1] + A[2] + A[3];
    t = t & 0b0000000001111111;
    CheckSumA = (char)t;
    CheckSumB = (char)t;
}

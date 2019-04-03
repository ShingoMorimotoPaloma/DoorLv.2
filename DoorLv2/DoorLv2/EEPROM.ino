#include <EEPROM.h>

typedef struct 
{
    int Address;
    char *Value;
}EEPROMdata;

typedef struct PassCode
{
    EEPROMdata *Key[4];
    EEPROMdata Checksum;
};



int EAddressKeyA[] = {0,1,2,3,4};
int EAddressKeyB[] = {5,6,7,8,9};

int EndAddress = 0xFFFF;

char CheckSumA;
char CheckSumB;
//EEPROMのパリティエラー
bool FlgEepromError = false;
bool FlgEepromWrite = false;
int PointOfEepWrite = 0;

EEPROMdata EepDataA[] =  
{
    {EAddressKeyA[0],&AlKey[0]},
    {EAddressKeyA[1],&AlKey[1]},
    {EAddressKeyA[2],&AlKey[2]},
    {EAddressKeyA[3],&AlKey[3]},
    {EAddressKeyA[4],&CheckSumA},
    {EndAddress,0}    
    };
EEPROMdata EepDataB[] =  
{
    {EAddressKeyB[0],&AlKey[0]},
    {EAddressKeyB[1],&AlKey[1]},
    {EAddressKeyB[2],&AlKey[2]},
    {EAddressKeyB[3],&AlKey[3]},
    {EAddressKeyB[4],&CheckSumB},
    {EndAddress,0}    
    };

//EEPROMのイニシャライズ
//setup関数から呼ばれることを期待
//EEPROMエラー判定も実行
void EepromInitial()
{   Serial.begin(9600);
    for(int i = 0;i < 4; i++)
    {
        Serial.println(EepDataA[i].Address,HEX);
        if (EepDataA[i].Address == EndAddress) break;
        //EepDataA[i].Value = (char*)(EEPROM.read(EepDataA[i].Address));
        byte b = EEPROM.read(EepDataA[i].Address);
        if (!CheckParity(b)) FlgEepromError = true;
        EepDataA[i].Value = (char*)(b);
        //AlKey[i] = (char)(b);
    }
    IntegrationTestEepromInitial();
    while(true){
        /* code */
    }
    
}

void EepWrite(int Address,char value)
{
    byte t = AddParity(value);
    EEPROM.write(Address,t);
}

void EepromCalc()
{
    if (KeyMach(EepromBackupKey,AlKey)) 
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
    if (PointOfEepWrite < 5) {
        EepWrite(EepDataA[PointOfEepWrite].Address,EepDataA[PointOfEepWrite].Value);
        PointOfEepWrite++;
    }
    else if(PointOfEepWrite < 10) {
        EepWrite(EepDataB[PointOfEepWrite - 5].Address,EepDataB[PointOfEepWrite - 5].Value);
        PointOfEepWrite++;       
    }
    else {
        FlgEepromWrite = false;
    }
    
    
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

    if (p) {
        return true;
    }
    else {
        return false;
    }
    
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
/*
void CheckSumCalc(PassCode p)
{
    byte b;
    b = p.Key[0];
    b = p.Key[1];
    b = p.Key[2];
    b = p.Key[3];
    P.Checksum = b;
}
    */
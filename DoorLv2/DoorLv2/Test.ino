/*
    テストコード
*/



//メインテストループ
void TestCalc()
{
    if(!FlgTest) return;
    Serial.begin(9600);
    UnitTestParityCalc();
    UnitTestAddParity();
    UnitTestCheckParity();
    UnitTestEepWrite();
    while(true){
        
    } 
}


//テスト対象：EEPROM AddParity関数
void UnitTestParityCalc()
{
    
    Serial.println("UnitTest:[ParityCalc]:Start");
    for(char i = 0; i < 127; i++)
    {
        Serial.print(i,BIN);
        Serial.print(',');
        Serial.println(ParityCalc(i));
    }
    Serial.println("UnitTest:[ParityCalc]:End");
}

//テスト対象：EEPROM AddParity関数
void UnitTestAddParity()
{
    Serial.println("UnitTest:[AddParity]:Start");
    for(char i = 0; i < 127; i++)
    {
        Serial.print(i,BIN);
        Serial.print(',');
        Serial.println(AddParity(i),BIN);
    }
    Serial.println("UnitTest:[AddParity]:End");
}

//テスト対象：EEPROM CheckParity
void UnitTestCheckParity()
{
    Serial.println("UnitTest:[CheckParity]:Start");
    for(char i = 0; i < 127; i++)
    {
        byte r = AddParity(i);
        Serial.print(r,BIN);
        Serial.print(',');
        Serial.println(CheckParity(r),BIN);       
    }
    for(char i = 0; i < 127; i++)
    {
        byte r = AddParity(i);
        Serial.print(r,BIN);
        Serial.print(',');
        Serial.println(CheckParity(r ^ 0b10000000),BIN);       
    }
    Serial.println("UnitTest:[CheckParity]:End");
}

//テスト対象：EEPROM EepWrite
void UnitTestEepWrite()
{
    Serial.println("UnitTest:[EepWrite]:Start");
    for(char i = 0; i < 127; i++)
    {
        EepWrite((int)i,i); 
        Serial.println("Written");    
    }
    for(char i = 0; i < 127; i++)
    {
        Serial.println(EEPROM.read((int)i),HEX);     
    }
    for(char i = 0; i < 127; i++)
    {
        char p = 127;
        p = p - i;
        EepWrite((int)i,p);     
    }
    for(char i = 0; i < 127; i++)
    {
        Serial.println(EEPROM.read((int)i),HEX);     
    }
    for(int i = 0; i < 10; i++)
    {
        EepWrite(i,'1'); 
    }

    Serial.println("UnitTest:[EepWrite]:End");
}

void IntegrationTestEepromInitial()
{
    //Serial.begin(9600);
    char t = 'c';
    Serial.println(t,BIN);
    Serial.println(AlKey[0],BIN);
    Serial.println(AlKey[1],BIN);
    Serial.println(AlKey[2],BIN);
    Serial.println(AlKey[3],BIN);
    Serial.println("End");
}

int *AAA;
int BBB;


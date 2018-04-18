#include <Keyboard.h>
#include <avr/pgmspace.h>
#include <FlashAsEEPROM.h>

//#define DEBUG_KEYPRESS
//#define DEBUG_TIME
#define DEBUG_JOYSTICK
#define RIGHT
//#define LEFT
#define FLASH

#define nbOfColumns 7
#define nbOfLines 5
#define nbKeys 35

const int coloumns[nbOfColumns] = { 38, 22, 24, 0, 9, 18, 17 };
const int lines[nbOfLines]      = { 23, 2, 1, 3, 4 };

bool key1[nbKeys]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
                                  false,
                               false,false,
                                  false
};
bool key2[nbKeys]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
                                  false,
                               false,false,
                                  false
};
bool key3[nbKeys]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
                                  false,
                               false,false,
                                  false
};


uint8_t keyMod[nbKeys]
{
    KEY_ESC, '1', '2', '3', '4', '5', KEY_RETURN,
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT,' ',' ',
        
                                      KEY_UP_ARROW,
                                KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
                                      KEY_DOWN_ARROW
};

/*
const EPortType port[nbOfColumns] = 
{
  g_APinDescription[38].ulPort,
  g_APinDescription[22].ulPort,
  g_APinDescription[24].ulPort,
  g_APinDescription[0].ulPort,
  g_APinDescription[9].ulPort,
  g_APinDescription[18].ulPort,
  g_APinDescription[17].ulPort
};

const uint32_t pins[nbOfColumns] =
{
  g_APinDescription[38].ulPin,
  g_APinDescription[22].ulPin,
  g_APinDescription[24].ulPin,
  g_APinDescription[0].ulPin,
  g_APinDescription[9].ulPin,
  g_APinDescription[18].ulPin,
  g_APinDescription[17].ulPin
};

uint8_t pinMask[nbOfColumns] = 
{
  0,0,0,0,0,0,0
};
*/

int max_x = 512;
int max_y = 512;
int min_x = 512;
int min_y = 512;
int x     = 512;
int y     = 512;
int current_x = 512;
int current_y = 512;

#ifdef DEBUG_TIME
unsigned long tick1 = 0;
unsigned long tick2 = 0;
long diff = 0;
int checkSerial = 0;
#endif

bool triggerSerial1 = false;
bool triggerSerial2 = false;

void inline SerialCOM()
{    
    if(SerialUSB.available() > 0)
    {
         String received = "";
         while(SerialUSB.available() > 0) { received += (char)(SerialUSB.read()); }       
         if(received[0]=='p' && received[1]=='i' && received[2]=='n' && received[3]=='g') { SerialUSB.print("pong"); }
         else if(received=="ack") { SendLayout(); }
         else
         {    if(received.length()>10)
              {   WriteKeyMod(received);                  
                  StoreKeyMod();                  
                  SendLayout();
              }
         }
    }
}

String GetValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++)
  {   if(data.charAt(i)==separator || i==maxIndex)
      {   found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void WriteKeyMod(String received)
{
    String part = "0";
    for(unsigned int n = 0; n < sizeof(keyMod); n++)
    {   part = GetValue(received,'|',n);
        keyMod[n] = part.toInt();                    
    }
}

void StoreKeyMod()
{  
    unsigned int n = 0;
    for (n=0; n<sizeof(keyMod); n++)
    {   SerialUSB.print(keyMod[n]);SerialUSB.print('|');
        EEPROM.write(n,keyMod[n]);      
    }
    EEPROM.write(n,0);      
    EEPROM.commit();
    delayMicroseconds(1);    
}
void ReadKeyMod()
{
    int tmp = -1;
    unsigned int n=0;
    for (n=0; n < sizeof(keyMod); n++)
    {   tmp = EEPROM.read(n); 
        keyMod[n] = tmp;      
    }
}

String SendLayout()
{
    String layout = "";
    for (unsigned int n = 0; n < sizeof(keyMod); n++) 
    {   layout+=keyMod[n]; 
        layout+="|";
    }
    SerialUSB.println(layout);
    return layout;
}

void setup() 
{
  SerialUSB.begin(115200);
  Keyboard.begin();
  Keyboard.releaseAll();
  
  //PREPARING THE MASK
  //for(int n = 0; n < nbOfColumns; n++) { pinMask[n] == (1<<coloumns[n]); }
  
  //PIN PREPARATION
  for(int n = 0; n < nbOfColumns; n++) { pinMode(coloumns[n],OUTPUT);         digitalWrite(coloumns[n],LOW); }
  for(int n = 0; n < nbOfLines; n++)   { pinMode( lines[n],  INPUT_PULLDOWN); }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  
  x = analogRead(A10);
  y = analogRead(A11);
  
#ifdef FLASH
  if (!EEPROM.isValid()) { SerialUSB.println("!EEPROM.isValid()"); }
  if (EEPROM.isValid())  { ReadKeyMod(); }
  else 
  {   //FIRST TIME PLUGING THE KEYBOARD
      StoreKeyMod();
  }
#endif
}

void Joystick()
{
  current_x = analogRead(A5);
  current_y = analogRead(A1);
  
#ifdef DEBUG_JOYSTICK
  pinMode(25,INPUT_PULLUP);
  if(digitalRead(25)==HIGH)
  {
    SerialUSB.println("UP");
  }
  else
  {
    SerialUSB.println("DOWN");
  }
  SerialUSB.print("A10 : "); SerialUSB.println(analogRead(A6));
  SerialUSB.print("A11 : "); SerialUSB.println(analogRead(A11));
  /*SerialUSB.print("A2 : "); SerialUSB.println(analogRead(A2));
  SerialUSB.print("A3 : "); SerialUSB.println(analogRead(A3));
  SerialUSB.print("A4 : "); SerialUSB.println(analogRead(A4));
  SerialUSB.print("A5 : "); SerialUSB.println(analogRead(A5));
  SerialUSB.println();*/
  delay(50);
#endif
}

void Key_Press()
{  
  //ORDER 1-2-3-4-5-6-0 FOR PRIORITY
  
  //COL1
  fastDigitalWrite(coloumns[1],HIGH);
  //LINE0
  key1[1] = fastDigitalRead(lines[0]);
  if(key1[1]!=key2[1])
  {
    key2[1] = key1[1];
  }
  else
  {
    if(key1[1] && !key3[1])
    {
      key3[1] = key1[1];
      Keyboard.press(keyMod[1]);
    }
    else if(!key1[1] && key3[1])
    {
      key3[1] = key1[1];
      Keyboard.release(keyMod[1]);
    }
  }
  //LINE1
  key1[8] = fastDigitalRead(lines[1]);
  if(key1[8]!=key2[8])
  {
    key2[8] = key1[8];
  }
  else
  {
    if(key1[8] && !key3[8])
    {
      key3[8] = key1[8];
      Keyboard.press(keyMod[8]);
    }
    else if(!key1[8] && key3[8])
    {
      key3[8] = key1[8];
      Keyboard.release(keyMod[8]);
    }
  }
  //LINE2
  key1[15] = fastDigitalRead(lines[2]);
  if(key1[15]!=key2[15])
  {
    key2[15] = key1[15];
  }
  else
  {
    if(key1[15] && !key3[15])
    {
      key3[15] = key1[15];
      Keyboard.press(keyMod[15]);
    }
    else if(!key1[15] && key3[15])
    {
      key3[15] = key1[15];
      Keyboard.release(keyMod[15]);
    }
  }
  //LINE3
  key1[21] = fastDigitalRead(lines[3]);
  if(key1[21]!=key2[21])
  {
    key2[21] = key1[21];
  }
  else
  {
    if(key1[21] && !key3[21])
    {
      key3[21] = key1[21];
      Keyboard.press(keyMod[21]);
    }
    else if(!key1[21] && key3[21])
    {
      key3[21] = key1[21];
      Keyboard.release(keyMod[21]);
    }
  }
  //LINE4
  key1[26] = fastDigitalRead(lines[4]);
  if(key1[26]!=key2[26])
  {
    key2[26] = key1[26];
  }
  else
  {
    if(key1[26] && !key3[26])
    {
      key3[26] = key1[26];
      Keyboard.press(keyMod[26]);
    }
    else if(!key1[26] && key3[26])
    {
      key3[26] = key1[26];
      Keyboard.release(keyMod[26]);
    }
  }  
  fastDigitalWrite(coloumns[1],LOW);

  //COL2
  fastDigitalWrite(coloumns[2],HIGH);
  //LINE0
  key1[2] = fastDigitalRead(lines[0]);
  if(key1[2]!=key2[2])
  {
    key2[2] = key1[2];
  }
  else
  {
    if(key1[2] && !key3[2])
    {
      key3[2] = key1[2];
      Keyboard.press(keyMod[2]);
    }
    else if(!key1[2] && key3[2])
    {
      key3[2] = key1[2];
      Keyboard.release(keyMod[2]);
    }
  }
  //LINE1
  key1[9] = fastDigitalRead(lines[1]);
  if(key1[9]!=key2[9])
  {
    key2[9] = key1[9];
  }
  else
  {
    if(key1[9] && !key3[9])
    {
      key3[9] = key1[9];
      Keyboard.press(keyMod[9]);
    }
    else if(!key1[9] && key3[9])
    {
      key3[9] = key1[9];
      Keyboard.release(keyMod[9]);
    }
  }
  //LINE2
  key1[16] = fastDigitalRead(lines[2]);
  if(key1[16]!=key2[16])
  {
    key2[16] = key1[16];
  }
  else
  {
    if(key1[16] && !key3[16])
    {
      key3[16] = key1[16];
      Keyboard.press(keyMod[16]);
    }
    else if(!key1[16] && key3[16])
    {
      key3[16] = key1[16];
      Keyboard.release(keyMod[16]);
    }
  }
  //LINE3
  key1[22] = fastDigitalRead(lines[3]);
  if(key1[22]!=key2[22])
  {
    key2[22] = key1[22];
  }
  else
  {
    if(key1[22] && !key3[22])
    {
      key3[22] = key1[22];
      Keyboard.press(keyMod[22]);
    }
    else if(!key1[22] && key3[22])
    {
      key3[22] = key1[22];
      Keyboard.release(keyMod[22]);
    }
  }
  //LINE4
  key1[27] = fastDigitalRead(lines[4]);
  if(key1[27]!=key2[27])
  {
    key2[27] = key1[27];
  }
  else
  {
    if(key1[27] && !key3[27])
    {
      key3[27] = key1[27];
      Keyboard.press(keyMod[27]);
    }
    else if(!key1[27] && key3[27])
    {
      key3[27] = key1[27];
      Keyboard.release(keyMod[27]);
    }
  }
  fastDigitalWrite(coloumns[2],LOW);

  //COL3
  fastDigitalWrite(coloumns[3],HIGH);
  //LINE0
  key1[3] = fastDigitalRead(lines[0]);
  if(key1[3]!=key2[3])
  {
    key2[3] = key1[3];
  }
  else
  {
    if(key1[3] && !key3[3])
    {
      key3[3] = key1[3];
      Keyboard.press(keyMod[3]);
    }
    else if(!key1[3] && key3[3])
    {
      key3[3] = key1[3];
      Keyboard.release(keyMod[3]);
    }
  }
  //LINE1
  key1[10] = fastDigitalRead(lines[1]);
  if(key1[10]!=key2[10])
  {
    key2[10] = key1[10];
  }
  else
  {
    if(key1[10] && !key3[10])
    {
      key3[10] = key1[10];
      Keyboard.press(keyMod[10]);
    }
    else if(!key1[10] && key3[10])
    {
      key3[10] = key1[10];
      Keyboard.release(keyMod[10]);
    }
  }
  //LINE2
  key1[17] = fastDigitalRead(lines[2]);
  if(key1[17]!=key2[17])
  {
    key2[17] = key1[17];
  }
  else
  {
    if(key1[17] && !key3[17])
    {
      key3[17] = key1[17];
      Keyboard.press(keyMod[17]);
    }
    else if(!key1[17] && key3[17])
    {
      key3[17] = key1[17];
      Keyboard.release(keyMod[17]);
    }
  }
  //LINE3
  key1[23] = fastDigitalRead(lines[3]);
  if(key1[23]!=key2[23])
  {
    key2[23] = key1[23];
  }
  else
  {
    if(key1[23] && !key3[23])
    {
      key3[23] = key1[23];
      Keyboard.press(keyMod[23]);
    }
    else if(!key1[23] && key3[23])
    {
      key3[23] = key1[23];
      Keyboard.release(keyMod[23]);
    }
  }
  //LINE4
  key1[28] = fastDigitalRead(lines[4]);
  if(key1[28]!=key2[28])
  {
    key2[28] = key1[28];
  }
  else
  {
    if(key1[28] && !key3[28])
    {
      key3[28] = key1[28];
      Keyboard.press(keyMod[28]);
    }
    else if(!key1[28] && key3[28])
    {
      key3[28] = key1[28];
      Keyboard.release(keyMod[28]);
    }
  }  
  fastDigitalWrite(coloumns[3],LOW);

  //COL4
  fastDigitalWrite(coloumns[4],HIGH);
  //LINE0
  key1[4] = fastDigitalRead(lines[0]);
  if(key1[4]!=key2[4])
  {
    key2[4] = key1[4];
  }
  else
  {
    if(key1[4] && !key3[4])
    {
      key3[4] = key1[4];
      Keyboard.press(keyMod[4]);
    }
    else if(!key1[4] && key3[4])
    {
      key3[4] = key1[4];
      Keyboard.release(keyMod[4]);
    }
  }
  //LINE1
  key1[11] = fastDigitalRead(lines[1]);
  if(key1[11]!=key2[11])
  {
    key2[11] = key1[11];
  }
  else
  {
    if(key1[11] && !key3[11])
    {
      key3[11] = key1[11];
      Keyboard.press(keyMod[11]);
    }
    else if(!key1[11] && key3[11])
    {
      key3[11] = key1[11];
      Keyboard.release(keyMod[11]);
    }
  }
  //LINE2
  key1[18] = fastDigitalRead(lines[2]);
  if(key1[18]!=key2[18])
  {
    key2[18] = key1[18];
  }
  else
  {
    if(key1[18] && !key3[18])
    {
      key3[18] = key1[18];
      Keyboard.press(keyMod[18]);
    }
    else if(!key1[18] && key3[18])
    {
      key3[18] = key1[18];
      Keyboard.release(keyMod[18]);
    }
  }
  //LINE3
  key1[24] = fastDigitalRead(lines[3]);
  if(key1[24]!=key2[24])
  {
    key2[24] = key1[24];
  }
  else
  {
    if(key1[24] && !key3[24])
    {
      key3[24] = key1[24];
      Keyboard.press(keyMod[24]);
    }
    else if(!key1[24] && key3[24])
    {
      key3[24] = key1[24];
      Keyboard.release(keyMod[24]);
    }
  }
  //LINE4
  key1[29] = fastDigitalRead(lines[4]);
  if(key1[29]!=key2[29])
  {
    key2[29] = key1[29];
  }
  else
  {
    if(key1[29] && !key3[29])
    {
      key3[29] = key1[29];
      Keyboard.press(keyMod[29]);
    }
    else if(!key1[29] && key3[29])
    {
      key3[29] = key1[29];
      Keyboard.release(keyMod[29]);
    }
  }  
  fastDigitalWrite(coloumns[4],LOW);

  //COL5
  fastDigitalWrite(coloumns[5],HIGH);
  //LINE0
  key1[5] = fastDigitalRead(lines[0]);
  if(key1[5]!=key2[5])
  {
    key2[5] = key1[5];
  }
  else
  {
    if(key1[5] && !key3[5])
    {
      key3[5] = key1[5];
      Keyboard.press(keyMod[5]);
    }
    else if(!key1[5] && key3[5])
    {
      key3[5] = key1[5];
      Keyboard.release(keyMod[5]);
    }
  }
  //LINE1
  key1[12] = fastDigitalRead(lines[1]);
  if(key1[12]!=key2[12])
  {
    key2[12] = key1[12];
  }
  else
  {
    if(key1[12] && !key3[12])
    {
      key3[12] = key1[12];
      Keyboard.press(keyMod[12]);
    }
    else if(!key1[12] && key3[12])
    {
      key3[12] = key1[12];
      Keyboard.release(keyMod[12]);
    }
  }
  //LINE2
  key1[19] = fastDigitalRead(lines[2]);
  if(key1[19]!=key2[19])
  {
    key2[19] = key1[19];
  }
  else
  {
    if(key1[19] && !key3[19])
    {
      key3[19] = key1[19];
      Keyboard.press(keyMod[19]);
    }
    else if(!key1[19] && key3[19])
    {
      key3[19] = key1[19];
      Keyboard.release(keyMod[19]);
    }
  }
  //LINE3
  key1[25] = fastDigitalRead(lines[3]);
  if(key1[25]!=key2[25])
  {
    key2[25] = key1[25];
  }
  else
  {
    if(key1[25] && !key3[25])
    {
      key3[25] = key1[25];
      Keyboard.press(keyMod[25]);
    }
    else if(!key1[25] && key3[25])
    {
      key3[25] = key1[25];
      Keyboard.release(keyMod[25]);
    }
  }
  //LINE4
#ifdef LEFT
  key1[30] = fastDigitalRead(lines[4]);
  if(key1[30]!=key2[30])
  {
    key2[30] = key1[30];
  }
  else
  {
    if(key1[30] && !key3[30])
    {
      key3[30] = key1[30];
      Keyboard.press(keyMod[30]);
    }
    else if(!key1[30] && key3[30])
    {
      key3[30] = key1[30];
      Keyboard.release(keyMod[30]);
    }
  }
#endif
  fastDigitalWrite(coloumns[5],LOW);

  //COL6
  fastDigitalWrite(coloumns[6],HIGH);
  //LINE0
  key1[6] = fastDigitalRead(lines[0]);
  if(key1[6]!=key2[6])
  {
    key2[6] = key1[6];
  }
  else
  {
    if(key1[6] && !key3[6])
    {
      key3[6] = key1[6];
      Keyboard.press(keyMod[6]);
    }
    else if(!key1[6] && key3[6])
    {
      key3[6] = key1[6];
      Keyboard.release(keyMod[6]);
    }
  }
  //LINE1
  key1[13] = fastDigitalRead(lines[1]);
  if(key1[13]!=key2[13])
  {
    key2[13] = key1[13];
  }
  else
  {
    if(key1[13] && !key3[13])
    {
      key3[13] = key1[13];
      Keyboard.press(keyMod[13]);
    }
    else if(!key1[13] && key3[13])
    {
      key3[13] = key1[13];
      Keyboard.release(keyMod[13]);
    }
  }
  //LINE2
  key1[20] = fastDigitalRead(lines[2]);
  if(key1[20]!=key2[20])
  {
    key2[20] = key1[20];
  }
  else
  {
    if(key1[20] && !key3[20])
    {
      key3[20] = key1[20];
      Keyboard.press(keyMod[20]);
    }
    else if(!key1[20] && key3[20])
    {
      key3[20] = key1[20];
      Keyboard.release(keyMod[20]);
    }
  }
  fastDigitalWrite(coloumns[6],LOW);
  
  //COL0
  fastDigitalWrite(coloumns[0],HIGH);
  //LINE0
  key1[0] = fastDigitalRead(lines[0]);
  if(key1[0]!=key2[0])
  {
    key2[0] = key1[0];
  }
  else
  {
    if(key1[0] && !key3[0])
    {
      key3[0] = key1[0];
      Keyboard.press(keyMod[0]);
    }
    else if(!key1[0] && key3[0])
    {
      key3[0] = key1[0];
      Keyboard.release(keyMod[0]);
    }
  }
  //LINE1
  key1[7] = fastDigitalRead(lines[1]);
  if(key1[7]!=key2[7])
  {
    key2[7] = key1[7];
  }
  else
  {
    if(key1[7] && !key3[7])
    {
      key3[7] = key1[7];
      Keyboard.press(keyMod[0]);
    }
    else if(!key1[7] && key3[7])
    {
      key3[7] = key1[7];
      Keyboard.release(keyMod[7]);
    }
  }
  //LINE2
  key1[14] = fastDigitalRead(lines[2]);
  if(key1[14]!=key2[14])
  {
    key2[14] = key1[14];
  }
  else
  {
    if(key1[14] && !key3[14])
    {
      key3[14] = key1[14];
      Keyboard.press(keyMod[14]);
    }
    else if(!key1[14] && key3[14])
    {
      key3[14] = key1[14];
      Keyboard.release(keyMod[14]);
    }
  }
  fastDigitalWrite(coloumns[0],LOW);

  
}

int inline analogReadFast(byte ADCpin) // inline library functions must be in header
{ 
    ADC->CTRLA.bit.ENABLE = 0;                     // Disable ADC
    while( ADC->STATUS.bit.SYNCBUSY == 1 );        // Wait for synchronization
    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV64 |   // Divide Clock by 64
                     ADC_CTRLB_RESSEL_10BIT; 
    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 |   // 1 sample 
                       ADC_AVGCTRL_ADJRES(0x00ul); // Adjusting result by 0
    ADC->SAMPCTRL.reg = 0x00;                      // Sampling Time Length = 0
  
    ADC->CTRLA.bit.ENABLE = 1;                     // Enable ADC
    while( ADC->STATUS.bit.SYNCBUSY == 1 );        // Wait for synchronization
    return analogRead(ADCpin);
}

void loop() 
{
#ifdef DEBUG_TIME
  tick1 = micros();
#endif

  Key_Press();
  Joystick();

#ifdef FLASH
  //CHECK THE SWITCHES
  if(key3[4] && key3[5] && key3[6] && !triggerSerial1)
  {
    triggerSerial1 = true;
    triggerSerial2 = !triggerSerial2;    
  }
  else if(!key3[4] && !key3[5] && !key3[6] && triggerSerial1)
  {
    triggerSerial1 = false;
  }
  if(triggerSerial2)
  {
    if(SerialUSB) { SerialCOM(); }
  }
#endif
  
#ifdef DEBUG_TIME
  tick2 = micros();
  diff = tick2 - tick1;
  SerialUSB.println(diff);
  delay(5);
#endif
  
#ifdef DEBUG_KEYPRESS
  bool found = false;
  for(int n = 0; n < nbKeys; n++)
  {
    if(key3[n])
    {
      found = true;
      SerialUSB.print(n);
      SerialUSB.print('\t');
    }
  }
  if(!false)
  {
    SerialUSB.println();
  }  
#endif
}

#include <Keyboard.h>

#define nbOfColumns 7
#define nbOfLines 5

int coloumns[nbOfColumns] = { 38, 22, 24, 0, 9, 18, 17 };
int lines[nbOfLines]      = { 23, 2, 1, 3, 4 };

bool key1[31]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
};
bool key2[31]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
};
bool key3[31]
{
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
  false,false,false,false,false,false,false,
      false,false,false,false,false,
      false,false,false,false,false,
};


uint8_t keyMod[31]
{
    KEY_ESC, '1', '2', '3', '4', '5', KEY_RETURN,
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT,' ',' '
};

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

void setup() 
{
  SerialUSB.begin(115200);
  Keyboard.begin();
  for(int n = 0; n < nbOfColumns; n++)
  {
    pinMask[n] == (1<<coloumns[n]);
  }
  for(int n = 0; n < nbOfColumns; n++)
  {
    delay(100);
    pinMode(coloumns[n],OUTPUT);
    digitalWrite(coloumns[n],LOW);
  }
  delay(100);
  for(int n = 0; n < nbOfLines; n++)
  {
    delay(100);
    pinMode(lines[n],INPUT_PULLDOWN);    
    //digitalWrite(lines[n],LOW);
  }
  Keyboard.releaseAll();
}

unsigned long lo = 0;
unsigned long tick1 = 0;
unsigned long tick2 = 0;
long diff = 0;
int key = 0;

void loop() 
{
  tick1 = micros();

  //COL0
  digitalWrite(coloumns[0],HIGH);
  //LINE0
  key1[0] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 0");
    }
    else if(!key1[0] && key3[0])
    {
      key3[0] = key1[0];
      Keyboard.release(keyMod[0]);
      //SerialUSB.println("Released - 0");
    }
  }
  //LINE1
  key1[7] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 7");
    }
    else if(!key1[7] && key3[7])
    {
      key3[7] = key1[7];
      Keyboard.release(keyMod[7]);
      //SerialUSB.println("Released - 7");
    }
  }
  //LINE2
  key1[14] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 14");
    }
    else if(!key1[14] && key3[14])
    {
      key3[14] = key1[14];
      Keyboard.press(keyMod[14]);
      //SerialUSB.println("Released - 14");
    }
  }
  digitalWrite(coloumns[0],LOW);

  //COL1
  digitalWrite(coloumns[1],HIGH);
  //LINE0
  key1[1] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 1");
    }
    else if(!key1[1] && key3[1])
    {
      key3[1] = key1[1];
      Keyboard.press(keyMod[1]);
      //SerialUSB.println("Released - 1");
    }
  }
  //LINE1
  key1[8] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 8");
    }
    else if(!key1[8] && key3[8])
    {
      key3[8] = key1[8];
      Keyboard.press(keyMod[8]);
      //SerialUSB.println("Released - 8");
    }
  }
  //LINE2
  key1[15] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 15");
    }
    else if(!key1[15] && key3[15])
    {
      key3[15] = key1[15];
      Keyboard.press(keyMod[15]);
      //SerialUSB.println("Released - 15");
    }
  }
  //LINE3
  key1[21] = digitalRead(lines[3]);
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
      //SerialUSB.println("Pressed - 21");
    }
    else if(!key1[21] && key3[21])
    {
      key3[21] = key1[21];
      Keyboard.press(keyMod[21]);
      //SerialUSB.println("Released - 21");
    }
  }
  //LINE4
  key1[26] = digitalRead(lines[4]);
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
      //SerialUSB.println("Pressed - 26");
    }
    else if(!key1[26] && key3[26])
    {
      key3[26] = key1[26];
      Keyboard.press(keyMod[26]);
      //SerialUSB.println("Released - 26");
    }
  }  
  digitalWrite(coloumns[1],LOW);

  //COL2
  digitalWrite(coloumns[2],HIGH);
  //LINE0
  key1[2] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 2");
    }
    else if(!key1[2] && key3[2])
    {
      key3[2] = key1[2];
      Keyboard.press(keyMod[2]);
      //SerialUSB.println("Released - 2");
    }
  }
  //LINE1
  key1[9] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 9");
    }
    else if(!key1[9] && key3[9])
    {
      key3[9] = key1[9];
      Keyboard.press(keyMod[9]);
      //SerialUSB.println("Released - 9");
    }
  }
  //LINE2
  key1[16] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 16");
    }
    else if(!key1[16] && key3[16])
    {
      key3[16] = key1[16];
      Keyboard.press(keyMod[16]);
      //SerialUSB.println("Released - 16");
    }
  }
  //LINE3
  key1[22] = digitalRead(lines[3]);
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
      //SerialUSB.println("Pressed - 22");
    }
    else if(!key1[22] && key3[22])
    {
      key3[22] = key1[22];
      Keyboard.press(keyMod[22]);
      //SerialUSB.println("Released - 22");
    }
  }
  //LINE4
  key1[27] = digitalRead(lines[4]);
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
      //SerialUSB.println("Pressed - 27");
    }
    else if(!key1[27] && key3[27])
    {
      key3[27] = key1[27];
      Keyboard.press(keyMod[27]);
      //SerialUSB.println("Released - 27");
    }
  }
  digitalWrite(coloumns[2],LOW);

  //COL3
  digitalWrite(coloumns[3],HIGH);
  //LINE0
  key1[3] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 3");
    }
    else if(!key1[3] && key3[3])
    {
      key3[3] = key1[3];
      Keyboard.press(keyMod[3]);
      //SerialUSB.println("Released - 3");
    }
  }
  //LINE1
  key1[10] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 10");
    }
    else if(!key1[10] && key3[10])
    {
      key3[10] = key1[10];
      Keyboard.press(keyMod[10]);
      //SerialUSB.println("Released - 10");
    }
  }
  //LINE2
  key1[17] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 17");
    }
    else if(!key1[17] && key3[17])
    {
      key3[17] = key1[17];
      Keyboard.press(keyMod[17]);
      //SerialUSB.println("Released - 17");
    }
  }
  //LINE3
  key1[23] = digitalRead(lines[3]);
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
      //SerialUSB.println("Pressed - 23");
    }
    else if(!key1[23] && key3[23])
    {
      key3[23] = key1[23];
      Keyboard.press(keyMod[23]);
      //SerialUSB.println("Released - 23");
    }
  }
  //LINE4
  key1[28] = digitalRead(lines[4]);
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
      //SerialUSB.println("Pressed - 28");
    }
    else if(!key1[28] && key3[28])
    {
      key3[28] = key1[28];
      Keyboard.press(keyMod[28]);
      //SerialUSB.println("Released - 28");
    }
  }  
  digitalWrite(coloumns[3],LOW);

  //COL4
  digitalWrite(coloumns[4],HIGH);
  //LINE0
  key1[4] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 4");
    }
    else if(!key1[4] && key3[4])
    {
      key3[4] = key1[4];
      Keyboard.press(keyMod[4]);
      //SerialUSB.println("Released - 4");
    }
  }
  //LINE1
  key1[11] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 11");
    }
    else if(!key1[11] && key3[11])
    {
      key3[11] = key1[11];
      Keyboard.press(keyMod[11]);
      //SerialUSB.println("Released - 11");
    }
  }
  //LINE2
  key1[18] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 18");
    }
    else if(!key1[18] && key3[18])
    {
      key3[18] = key1[18];
      Keyboard.press(keyMod[18]);
      //SerialUSB.println("Released - 18");
    }
  }
  //LINE3
  key1[24] = digitalRead(lines[3]);
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
      //SerialUSB.println("Pressed - 24");
    }
    else if(!key1[24] && key3[24])
    {
      key3[24] = key1[24];
      Keyboard.press(keyMod[24]);
      //SerialUSB.println("Released - 24");
    }
  }
  //LINE4
  key1[29] = digitalRead(lines[4]);
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
      //SerialUSB.println("Pressed - 29");
    }
    else if(!key1[29] && key3[29])
    {
      key3[29] = key1[29];
      Keyboard.press(keyMod[29]);
      //SerialUSB.println("Released - 29");
    }
  }  
  digitalWrite(coloumns[4],LOW);

  //COL5
  digitalWrite(coloumns[5],HIGH);
  //LINE0
  key1[5] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 5");
    }
    else if(!key1[5] && key3[5])
    {
      key3[5] = key1[5];
      Keyboard.press(keyMod[5]);
      //SerialUSB.println("Released - 5");
    }
  }
  //LINE1
  key1[12] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 12");
    }
    else if(!key1[12] && key3[12])
    {
      key3[12] = key1[12];
      Keyboard.press(keyMod[12]);
      //SerialUSB.println("Released - 12");
    }
  }
  //LINE2
  key1[19] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 19");
    }
    else if(!key1[19] && key3[19])
    {
      key3[19] = key1[19];
      Keyboard.press(keyMod[19]);
      //SerialUSB.println("Released - 19");
    }
  }
  //LINE3
  key1[25] = digitalRead(lines[3]);
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
      //SerialUSB.println("Pressed - 25");
    }
    else if(!key1[25] && key3[25])
    {
      key3[25] = key1[25];
      Keyboard.press(keyMod[25]);
      //SerialUSB.println("Released - 25");
    }
  }
  //LINE4
  key1[30] = digitalRead(lines[4]);
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
      //SerialUSB.println("Pressed - 30");
    }
    else if(!key1[30] && key3[30])
    {
      key3[30] = key1[30];
      Keyboard.press(keyMod[30]);
      //SerialUSB.println("Released - 30");
    }
  }  
  digitalWrite(coloumns[5],LOW);

  //COL6
  digitalWrite(coloumns[6],HIGH);
  //LINE0
  key1[6] = digitalRead(lines[0]);
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
      //SerialUSB.println("Pressed - 6");
    }
    else if(!key1[6] && key3[6])
    {
      key3[6] = key1[6];
      Keyboard.press(keyMod[6]);
      //SerialUSB.println("Released - 6");
    }
  }
  //LINE1
  key1[13] = digitalRead(lines[1]);
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
      //SerialUSB.println("Pressed - 13");
    }
    else if(!key1[13] && key3[13])
    {
      key3[13] = key1[13];
      Keyboard.press(keyMod[13]);
      //SerialUSB.println("Released - 13");
    }
  }
  //LINE2
  key1[20] = digitalRead(lines[2]);
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
      //SerialUSB.println("Pressed - 20");
    }
    else if(!key1[20] && key3[20])
    {
      key3[20] = key1[20];
      Keyboard.press(keyMod[20]);
      //SerialUSB.println("Released - 20");
    }
  }
  digitalWrite(coloumns[6],LOW);

  tick2 = micros();
  diff = tick2 - tick1;
  SerialUSB.println(diff);
  //delay(50);
}

#include <Keyboard.h>
//#include <avr/pgmspace.h>
//#include <FlashAsEEPROM.h>

#define nbOfColumns 7
#define nbOfLines 5
#define nbKeys 35
//#define LINES2COLUMS

const int coloumns[nbOfColumns] = { 38, 22, 24, 0, 9, 18, 17 };
const int lines[nbOfLines]      = { 23, 2, 1, 3, 4 };

bool key1[nbKeys]{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool key2[nbKeys]{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool key3[nbKeys]{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

const uint8_t keyMod1[nbKeys] //LAYOUT
{
    KEY_ESC, '1', '2', '3', '4', '5', 0,
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, 0x20,0x20,
        
                                      KEY_UP_ARROW,
                              KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
                                      KEY_DOWN_ARROW
};
const uint8_t keyMod2[nbKeys] //LAYOUT
{
    KEY_ESC, '6', '7', '8', '9', '0', 0,
    KEY_TAB, 'u', 'i', 'o', 'p', '[', ']', 
    KEY_TAB, 'j', 'k', 'l', ';', '\'', '\\', 
    KEY_RIGHT_SHIFT,'n', 'm', ',', '.', 
        KEY_RIGHT_CTRL, KEY_RIGHT_GUI, KEY_RIGHT_ALT, 0x20,0x20,
        
                                      KEY_UP_ARROW,
                              KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
                                      KEY_DOWN_ARROW
};
uint8_t keyMod[nbKeys] //LAYOUT
{
    KEY_ESC, '1', '2', '3', '4', '5', '6',
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, 0x20,0x20,
        
                                      KEY_UP_ARROW,
                              KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
                                      KEY_DOWN_ARROW
};
bool isFirstLayer = true;

int max_x = 512;
int max_y = 512;
int min_x = 512;
int min_y = 512;
int x     = 512;
int y     = 512;
int current_x = 512;
int current_y = 512;

int fastAnalogRead(byte ADCpin);

//#define DEBUGTIME

//void inline SerialCOM()
//{    
//    if(SerialUSB.available() > 0)
//    {
//         String received = "";
//         while(SerialUSB.available() > 0) { received += (char)(SerialUSB.read()); }       
//         if(received[0]=='p' && received[1]=='i' && received[2]=='n' && received[3]=='g') { SerialUSB.print("pong"); }
//         else if(received=="ack") { SendLayout(); }
//         else
//         {    if(received.length()>10)
//              {   WriteKeyMod(received);                  
//                  StoreKeyMod();                  
//                  SendLayout();
//              }
//         }
//    }
//}

int inline fastAnalogRead(byte ADCpin) // inline library functions must be in header
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

//String GetValue(String data, char separator, int index)
//{
//  int found = 0;
//  int strIndex[] = {0, -1};
//  int maxIndex = data.length()-1;
//
//  for(int i=0; i<=maxIndex && found<=index; i++)
//  {   if(data.charAt(i)==separator || i==maxIndex)
//      {   found++;
//          strIndex[0] = strIndex[1]+1;
//          strIndex[1] = (i == maxIndex) ? i+1 : i;
//      }
//  }
//  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
//}
//void WriteKeyMod(String received)
//{
//    String part = "0";
//    for(unsigned int n = 0; n < sizeof(keyMod); n++)
//    {   part = GetValue(received,'|',n);
//        keyMod[n] = part.toInt();                    
//    }
//}
//void StoreKeyMod()
//{  
//    unsigned int n = 0;
//    for (n=0; n<sizeof(keyMod); n++)
//    {   SerialUSB.print(keyMod[n]);SerialUSB.print('|');
//        EEPROM.write(n,keyMod[n]);      
//    }
//    EEPROM.write(n,0);      
//    EEPROM.commit();
//    delayMicroseconds(1);    
//}
//void ReadKeyMod()
//{
//    int tmp = -1;
//    unsigned int n=0;
//    for (n=0; n < sizeof(keyMod); n++)
//    {   tmp = EEPROM.read(n); 
//        keyMod[n] = tmp;      
//    }
//}
//String SendLayout()
//{
//    String layout = "";
//    for (unsigned int n = 0; n < sizeof(keyMod); n++) 
//    {   layout+=keyMod[n]; 
//        layout+="|";
//    }
//    SerialUSB.println(layout);
//    return layout;
//}

void setup() 
{    
    SerialUSB.begin(115200);
      
    Keyboard.begin();
    Keyboard.releaseAll();
    
    //KEY PIN PREPARATION
    for(int n = 0; n < nbOfColumns; n++) { pinMode(coloumns[n],OUTPUT); digitalWrite(coloumns[n],LOW); }
    for(int n = 0; n < nbOfLines; n++)   { pinMode(lines[n], INPUT_PULLDOWN); }
    
    //JOYSTICK PIN PREPARATION
    pinMode(A5, INPUT);
    pinMode(25, INPUT);
    x = analogRead(A5);
    y = analogRead(25);    
}

unsigned long tick1 = 0;
unsigned long tick2 = 0;
long diff = 0;

void loop() 
{
//  tick1 = micros();

    Key_Press();
    Joystick();
    if(!isFirstLayer)
    {
        SerialCOM();
    }
//  SerialCOM();    
//
//  tick2 = micros();
//  diff = tick2 - tick1;
//  SerialUSB.println(diff);
//  delay(20);
}


const int threshold = 150;
const bool right = true;

bool x_or_y = true;

void Joystick()
{  
    //SPEED UP THE KEY_PRESS
    x_or_y = !x_or_y;
    
    if(x_or_y)
    {
        if(right) { current_x = x-fastAnalogRead(A5); }
        else { current_x = x-fastAnalogRead(25); }
        
        if(current_x> threshold)
        { 
            Keyboard.press(keyMod[nbKeys-2]);
            key3[nbKeys-2]=true;
        }
        else if(current_x<-threshold)
        {
            Keyboard.press(keyMod[nbKeys-3]);
            key3[nbKeys-3]=true;
        }
        else
        { 
            Keyboard.release(keyMod[nbKeys-2]);
            Keyboard.release(keyMod[nbKeys-3]);
            key3[nbKeys-2]=false;
            key3[nbKeys-3]=false;
        }
    }
    else
    {
        if(right) { current_y = y-fastAnalogRead(25); }
        else { current_y = y-fastAnalogRead(A5); }

        if(current_y>threshold)
        {
            Keyboard.press(keyMod[nbKeys-4]);
            key3[nbKeys-4]=true;
        }
        else if(current_y<-threshold)
        {
            Keyboard.press(keyMod[nbKeys-1]);
            key3[nbKeys-1]=true;
        }
        else
        {
            Keyboard.release(keyMod[nbKeys-4]);
            Keyboard.release(keyMod[nbKeys-1]);
            key3[nbKeys-4]=false;
            key3[nbKeys-1]=false;
        }
    }
}

int k;
void Key_Press()
{
    for(int c = 0; c < nbOfColumns; c++)
    {
        digitalWrite(coloumns[c],HIGH);
        for(int l = 0; l < nbOfLines; l++)
        {
            k = c+l*nbOfColumns;
            //V CORRECTION
            switch(k)
            {
              case 21: continue;                
              case 22: k = 21; break;
              case 23: k = 22; break;
              case 24: k = 23; break;                
              case 25: k = 24; break;
              case 26: k = 25; break;
              case 27: continue;
              case 28: continue;
              case 29: k = 26; break;
              case 30: k = 27; break;
              case 31: k = 28; break;
              case 32: k = 29; break;
              case 33: k = 30; break;
              case 34: k = 31; break;              
            }
               
            key1[k] = digitalRead(lines[l]);

            //ONE CYCLE LPF
            if(key1[k]!=key2[k]) { key2[k] = key1[k]; }
            else
            {
                //LATCHING MECHANISM
                if(key1[k] && !key3[k])
                {
                    //SerialUSB.print("Column : ");SerialUSB.print(c);SerialUSB.print("\tLine : ");SerialUSB.print(l); SerialUSB.print("\tKey : ");SerialUSB.print(k);SerialUSB.print(" \t Char : ");SerialUSB.print(keyMod[k]);SerialUSB.println(" \t PRESSED");
                    key3[k] = key1[k];
                    if(key3[k]==6)
                    {
                        isFirstLayer = false;
                    }
                    else
                    {
                        Keyboard.release(keyMod[k]);
                    }
                }
                else if(!key1[k] && key3[k])
                {
                    //SerialUSB.print("Column : ");SerialUSB.print(c);SerialUSB.print("\tLine : ");SerialUSB.print(l); SerialUSB.print("\tKey : ");SerialUSB.print(k);SerialUSB.print(" \t Char : ");SerialUSB.print(keyMod[k]);SerialUSB.println(" \t RELEASED");
                    key3[k] = key1[k];
                    if(key3[k]==6)
                    {
                        isFirstLayer = true;
                    }
                    else
                    {
                        Keyboard.release(keyMod[k]);
                    }
                }
            }
        }
        digitalWrite(coloumns[c],LOW);
    }
}

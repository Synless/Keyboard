#include <Keyboard.h>
#include <avr/pgmspace.h>
#include <FlashAsEEPROM.h>

#define numberOfKeys  29

const uint8_t pinMod[numberOfKeys] //HARDWARE DEPENDENT
{
  28, 41, 20, 19, 36,  37,  1,
  29, 45, 11, 10, 33,  34,  0,
  9,  47, 51, 21, 24,  32,  35,
      8,  40, 3,  30,  25,
          39, 42, 18
};

/*const*/uint8_t keyMod[]
{
    KEY_ESC, '1', '2', '3', '4', '5', KEY_BACKSPACE,
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_ALT, ' ',
                                    KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW,
};

bool key1[numberOfKeys] =
{
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
       true,  true,true,true,  true,
            true,  true,  true
};
bool key2[numberOfKeys] =
{
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
       true,  true,true,true,  true,
            true,  true,  true
};
bool keyLock[numberOfKeys] =
 {
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
   true, true,true,true,true,true,true,
       true,  true,true,true,  true,
            true,  true,  true
 };

int max_x = 512;
int max_y = 512;
int min_x = 512;
int min_y = 512;
int x     = 512;
int y     = 512;
int current_x = 512;
int current_y = 512;

const int threshold = 150;

void setup()
{
    SerialUSB.begin(115200);
    Keyboard.begin();
    Keyboard.releaseAll();

    //_joystick.begin();

    //INPUT FOR KEYS
    for (int n = 0; n < numberOfKeys; n++) { pinMode(pinMod[n], INPUT); }

    //LED
    pinMode(46, OUTPUT);
    digitalWrite(46, LOW);

    //ANALOG SETUP
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    x = analogRead(A0);
    y = 1023-analogRead(A1);

    if (!EEPROM.isValid()) 
    {
        SerialUSB.println("!EEPROM.isValid()");
    }
    
    if (EEPROM.isValid()) 
    {
        readKeyMod();
    }
    else
    {
        //FIRST TIME PLUGING THE KEYBOARD
        storeKeyMod();
    }
    
}

long tmp1 = 0;
long tmp2 = 0;
void loop()
{
    //tmp1 = micros();
    serial();
    //tmp2 = micros();
    //SerialUSB.print("dif serial\t: "); SerialUSB.print((tmp2-tmp1)); SerialUSB.println("us"); SerialUSB.println();
    //tmp1 = micros();
    keypress();
    //tmp2 = micros();
    //SerialUSB.print("dif keypress\t: "); SerialUSB.print((tmp2-tmp1)); SerialUSB.println("us"); SerialUSB.println();
    //tmp1 = micros();
    joystick();
    //tmp2 = micros();
    //SerialUSB.print("dif joystick\t: "); SerialUSB.print((tmp2-tmp1)); SerialUSB.println("us"); SerialUSB.println();
    
    //delay(200);     
}

void serial()
{
    if(SerialUSB.available() > 0)
    {
         String received = "";
         while(SerialUSB.available() > 0)
         {
            received += (char)(SerialUSB.read());
         }       
         if(received[0]=='p' && received[1]=='i' && received[2]=='n' && received[3]=='g')
         {
            SerialUSB.print("pong");
         }
         else if(received=="ack")
         {
            sendLayout();
         }
         else
         {
              if(received.length()>10)
              {
                  writeKeyMod(received);                  
                  storeKeyMod();                  
                  sendLayout();
              }
         }
    }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++)
  {
      if(data.charAt(i)==separator || i==maxIndex)
      {
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void writeKeyMod(String received)
{
    String part = "0";
    for(int n = 0; n < sizeof(keyMod); n++)
    {
        part = getValue(received,'|',n);
        keyMod[n] = part.toInt();                    
    }
}

void storeKeyMod()
{    
    //SerialUSB.println("storeKeyMod");    
    int n = 0;
    for (n=0; n<sizeof(keyMod); n++)
    {
        SerialUSB.print(keyMod[n]);SerialUSB.print('|');
        EEPROM.write(n,keyMod[n]);      
    }
    EEPROM.write(n,0);      
    EEPROM.commit();
    delay(10);    
}

String readKeyMod()
{
    //SerialUSB.println("readKeyMod");
    int tmp = -1;
    int n=0;
    for (n=0; n < sizeof(keyMod); n++)
    {
        tmp = EEPROM.read(n); 
        //SerialUSB.print(tmp);SerialUSB.print('|');
        keyMod[n] = tmp;      
    }
    //SerialUSB.println();
}

String sendLayout()
{   
    //SerialUSB.println("Sending layout");
    String layout = "";
    for (int n = 0; n < sizeof(keyMod); n++) 
    { 
        layout+=keyMod[n]; 
        layout+="|";
    }
    SerialUSB.println(layout);
    return layout;
}

void keypress()
{
    //KEYPRESS PART
    digitalWrite(46, LOW);
    for (int n = 0; n < numberOfKeys; n++)
    { 
        key2[n] = digitalRead(pinMod[n]);
        if(key1[n]!=key2[n]) //ONE LOOP LOW-PASS FILTER
        {
            key1[n]=key2[n];
        }
        else
        {
            if (key1[n] == LOW && keyLock[n]) 
            { 
                keyLock[n] = false; 
                Keyboard.press(keyMod[n]); 
                digitalWrite(46, HIGH);
            }
            else if (key1[n] == HIGH && !keyLock[n]) 
            {
                keyLock[n] = true; 
                Keyboard.release(keyMod[n]); 
            }
        }
    }
}

#if defined(__arm__) 
int inline analogReadFast(byte ADCpin, byte prescalerBits) // inline library functions must be in header
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
#else
int inline analogReadFast(byte ADCpin, byte prescalerBits) // inline library functions must be in header
{ 
    byte ADCSRAoriginal = ADCSRA; 
    ADCSRA = (ADCSRA & B11111000) | prescalerBits; 
    int adc = analogRead(ADCpin);  
    ADCSRA = ADCSRAoriginal;
    return adc;
}
#endif


bool releasedX = false;
bool releasedY = false;
void joystick()
{
    //JOYSTICK //KEY_UP_ARROW,KEY_RIGHT_ARROW, KEY_DOWN_ARROW,KEY_LEFT_ARROW

    current_x = analogReadFast(A0,0);
    current_y = 1023-analogReadFast(A1,0);      
    /*SerialUSB.print("A0 : "); SerialUSB.println(current_x);
    SerialUSB.print("A1 : "); SerialUSB.println(current_y);
    SerialUSB.print("releasedX : "); SerialUSB.println(releasedX);
    SerialUSB.print("releasedY : "); SerialUSB.println(releasedY);*/
    //Serial.println();
    //_joystick.setXAxis(current_x);
    //_joystick.setYAxis(current_y);
    //X
    if (x - current_x > threshold)
    {   
        releasedX = false;
        Keyboard.press(keyMod[29]);
        Keyboard.release(keyMod[31]);        
    }
    else if (current_x - x > threshold)
    {   
        releasedX = false;
        Keyboard.press(keyMod[31]);
        Keyboard.release(keyMod[29]);
    }    
    else
    {   
        if(!releasedX)
        {
            releasedX = true;
            Keyboard.release(keyMod[29]);
            Keyboard.release(keyMod[31]);
        }
    }
    //Y
    if (y - current_y > threshold)
    {   
        releasedY = false;
        Keyboard.press(keyMod[30]);
        Keyboard.release(keyMod[32]);
    }    
    else if (current_y - y > threshold)
    {   
        releasedY = false;
        Keyboard.press(keyMod[32]);
        Keyboard.release(keyMod[30]);
    }
    else
    {   
        if(!releasedY)
        {
            releasedY = true;
            Keyboard.release(keyMod[30]);
            Keyboard.release(keyMod[32]);
        }
    }
}

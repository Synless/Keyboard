#include <Keyboard.h>
#include <avr/pgmspace.h>
#include <FlashStorage.h>
#define numberOfKeys  29

bool key[numberOfKeys] =
{
  true, true,true,true,true,true,true,
  true, true,true,true,true,true,true,
  true, true,true,true,true,true,true,
      true,  true,true,true,  true,
           true,  true,  true
};

const uint8_t pinMod[numberOfKeys] //HARDWARE DEPENDENT
{
  28, 41, 20, 19, 36,  37,  1,
  29, 45, 11, 10, 33,  34,  0,
  9,  47, 51, 21, 24,  32,  35,
      8,  40, 3,  30,  25,
          39, 42, 18
};
//const
uint8_t keyMod[]
{
    KEY_ESC, '1', '2', '3', '4', '5', KEY_BACKSPACE, 
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 
    KEY_TAB, 'a', 's', 'd', 'f', 'g', 'h', 
    KEY_LEFT_SHIFT,'x', 'c', 'v', 'b', 
        KEY_LEFT_CTRL, KEY_LEFT_ALT, ' ',
                                    KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW,
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

    //setKeyMod(readKeyMod());
}

void loop()
{
    serial();
    keypress();
    joystick();
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
                  setKeyMod(received);                  
                  storeKeyMod(received);
                  
                  //SerialUSB.println(tmp3);
                  sendLayout();
              }
         }
    }
}
void setKeyMod(String received)
{
    String part = "0";
    for(int n = 0; n < sizeof(keyMod); n++)
    {
      part = getValue(received,'|',n);
      keyMod[n] = part.toInt();                    
    }
}
FlashStorage(my_flash_store, char*);

void storeKeyMod(String received)
{    
    char tmp[150];
    received.toCharArray(tmp,received.length());
    //my_flash_store.write(tmp);
    delay(200);
    
}
String readKeyMod()
{
    char tmp2[150];
    my_flash_store.read();
    String tmp3 = "";
    for(int n = 0; n < 150; n++)
    {
      tmp3+=tmp2[n];
    }
    SerialUSB.println(tmp3);
    return tmp3;
}

String sendLayout()
{   
    SerialUSB.println("Entering layout");
    String layout = "";
    for (int n = 0; n < sizeof(keyMod); n++) { layout+=keyMod[n]; layout+="|"; if(n%5==0){ SerialUSB.println(" ");} }
    SerialUSB.println(layout);
    return layout;
}

void keypress()
{
    //KEYPRESS PART
    digitalWrite(46, LOW);
    for (int n = 0; n < numberOfKeys; n++)
    { if (digitalRead(pinMod[n]) == LOW && key[n]) { key[n] = false; Keyboard.press(keyMod[n]); digitalWrite(46, HIGH); }
      else if (digitalRead(pinMod[n]) == HIGH && !key[n]) { key[n] = true; Keyboard.release(keyMod[n]); }
    }
}
void joystick()
{
    //JOYSTICK //KEY_UP_ARROW,KEY_RIGHT_ARROW, KEY_DOWN_ARROW,KEY_LEFT_ARROW
    current_x = analogRead(A0);
    current_y = 1023-analogRead(A1);      
    //SerialUSB.print("A0 : "); SerialUSB.println(current_x);
    //SerialUSB.print("A1 : "); SerialUSB.println(current_y);
    //X
    if (x - current_x > threshold)
    {   Keyboard.press(keyMod[29]);
        Keyboard.release(keyMod[31]);        
    }
    else if (current_x - x > threshold)
    {   Keyboard.press(keyMod[31]);
        Keyboard.release(keyMod[29]);
    }    
    else
    {   Keyboard.release(keyMod[29]);
        Keyboard.release(keyMod[31]);
    }
    //Y
    if (y - current_y > threshold)
    {   Keyboard.press(keyMod[30]);
        Keyboard.release(keyMod[32]);
    }    
    else if (current_y - y > threshold)
    {   Keyboard.press(keyMod[32]);
        Keyboard.release(keyMod[30]);
    }
    else
    {   Keyboard.release(keyMod[30]);
        Keyboard.release(keyMod[32]);
    }
}

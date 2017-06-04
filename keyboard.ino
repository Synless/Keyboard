#include <Keyboard.h>
#define numberOfKeys  29

const int threshold = 150;

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

const uint8_t keyMod[numberOfKeys]
{
  KEY_ESC,    '1',  '2',  '3',  '4',  '5',  KEY_BACKSPACE,
  KEY_TAB,    'q',  'w',  'e',  'r',  't',  'y',
  KEY_TAB,    'a',  's',  'd',  'f',  'g',  'h',
  KEY_LEFT_SHIFT,   'x',  'c',  'v',  'b',
          KEY_LEFT_CTRL, KEY_LEFT_ALT, ' ' 
};

int max_x = 512;
int max_y = 512;
int min_x = 512;
int min_y = 512;
int x     = 512;
int y     = 512;
int current_x = 512;
int current_y = 512;

void setup()
{
    SerialUSB.begin(115200);
    Keyboard.begin();
    Keyboard.releaseAll();
f
    //INPUT FOR KEYS
    for (int n = 0; n < numberOfKeys; n++)
    {
        pinMode(pinMod[n], INPUT);
    }

    //LED
    pinMode(46, OUTPUT);
    digitalWrite(46, LOW);

    //ANALOG SETUP
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    x = analogRead(A0);
    y = 1023-analogRead(A1);    
}

void loop()
{
    //KEYPRESS PART
    digitalWrite(46, LOW);
    for (int n = 0; n < numberOfKeys; n++)
    { 
      if (digitalRead(pinMod[n]))
      {
          SerialUSB.print(n); SerialUSB.println("pressed");          
          digitalWrite(46, HIGH);
      }
    }
    
    //JOYSTICK
    current_x = analogRead(A0);
    current_y = 1023-analogRead(A1);      
    SerialUSB.print("A0 : "); SerialUSB.println(current_x);
    SerialUSB.print("A1 : "); SerialUSB.println(current_y);
    
    //X
    if (x - current_x > threshold)
    {        
        Keyboard.press(KEY_UP_ARROW);
        Keyboard.release(KEY_DOWN_ARROW);        
    }
    else if (current_x - x > threshold)
    {  
        Keyboard.press(KEY_DOWN_ARROW);
        Keyboard.release(KEY_UP_ARROW);
    }    
    else
    {        
        Keyboard.release(KEY_DOWN_ARROW);
        Keyboard.release(KEY_UP_ARROW);
    }
    //Y
    if (y - current_y > threshold)
    {   
        Keyboard.press(KEY_RIGHT_ARROW);
        Keyboard.release(KEY_LEFT_ARROW);
    }    
    else if (current_y - y > threshold)
    {        
        Keyboard.press(KEY_LEFT_ARROW);
        Keyboard.release(KEY_RIGHT_ARROW);
    }
    else
    {
        Keyboard.release(KEY_RIGHT_ARROW);
        Keyboard.release(KEY_LEFT_ARROW);
    }
}

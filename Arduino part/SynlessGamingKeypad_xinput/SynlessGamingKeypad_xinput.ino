#include <XInput.h>

long x, y, x_start, y_start;

#define RUMBLEOUTPUT 14

void setup()
{
    pinMode(A1,INPUT);//X
    pinMode(A0,INPUT);//Y
    pinMode(A3,INPUT);//Chip select
    
    pinMode(RUMBLEOUTPUT,OUTPUT);
    digitalWrite(RUMBLEOUTPUT,HIGH);
  
    x_start = analogRead(A0);
    y_start = analogRead(A1);

    XInput.setReceiveCallback(rumbleCallback);
    XInput.setAutoSend(true);
    XInput.begin();
    
    //Serial.begin(115200);
}

const int JoyMax = 32767;
int innerBorder_x = 260;
int innerBorder_y = 260;
bool once = true;

void loop()
{
    //Serial.println(analogRead(A1));
    
    if(analogRead(A3) < 1000)
    {
        once = true;

        x = y_start - analogRead(A1);
        y = x_start - analogRead(A0);        
        
        if(abs(x) > innerBorder_x) { innerBorder_x = abs(x); }
        if(abs(y) > innerBorder_y) { innerBorder_y = abs(y); }   
             
        x = map(x,-innerBorder_x,innerBorder_x,-JoyMax,JoyMax)*1.08;
        y = map(y,-innerBorder_y,innerBorder_y,-JoyMax,JoyMax)*1.08;
        
        x = max(-JoyMax,min(JoyMax,x));
        y = max(-JoyMax,min(JoyMax,y));
        
        XInput.setJoystick(JOY_LEFT, (int)x,(int)y);
    }
    else if(once)
    {
        once = false;
        XInput.setJoystick(JOY_LEFT, 0,0);
    }
 }

void rumbleCallback(uint8_t packetType)
{
    if (packetType == (uint8_t) XInputReceiveType::Rumble)
    {
        uint8_t rumbleValue = XInput.getRumbleLeft() | XInput.getRumbleRight();
        analogWrite(RUMBLEOUTPUT, rumbleValue);
    }
}

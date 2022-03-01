#include <XInput.h>
#define RUMBLEOUTPUT 17

long x, y, x_start, y_start;
bool rightSideLayout = true;

void setup()
{
    pinMode(RUMBLEOUTPUT,OUTPUT);
    digitalWrite(RUMBLEOUTPUT,HIGH);

    pinMode(A1,INPUT); //X or Y
    pinMode(A0,INPUT); //X or Y
    pinMode(A3,INPUT); //Chip select - Active LOW
    pinMode(A4,INPUT); //Right of left layout - LOW = joystick on the right, HIGH = joystick on the left
            
    rightSideLayout = analogRead(A4) < 1000;
    if(rightSideLayout) { y_start = analogRead(A0); x_start = analogRead(A1); }
    else { y_start = analogRead(A1); x_start = analogRead(A0); }

    XInput.setReceiveCallback(rumbleCallback);
    XInput.setAutoSend(true);
    XInput.begin();
}

void rumbleCallback(uint8_t packetType)
{
    // If we have an LED packet (0x01), do nothing
    if (packetType == (uint8_t) XInputReceiveType::LEDs) { return; }
  
    // If we have a rumble packet (0x00), see our rumble data on the LED
    else if (packetType == (uint8_t) XInputReceiveType::Rumble)
    {
        uint8_t rumbleValue = XInput.getRumbleLeft() | XInput.getRumbleRight();        
        digitalWrite(RUMBLEOUTPUT,LOW);
        analogWrite(RUMBLEOUTPUT, rumbleValue);
    }
}

const int JoyMax = 32767;
const int deadZone = 1000;
int innerBorder_x = 260;
int innerBorder_y = 260;
bool once = true;

void loop()
{
    if(analogRead(A3) < 1000) //digitalRead does not work for some reason
    {
        if(!once) //if this is the first activated cycle ...
        {
            rightSideLayout = analogRead(A4) < 1000;
            
            if(rightSideLayout) { y_start = analogRead(A0); x_start = analogRead(A1); }
            else { y_start = analogRead(A1); x_start = analogRead(A0); }

            digitalWrite(RUMBLEOUTPUT,LOW);  delay(50); //rumble pattern for activation
            digitalWrite(RUMBLEOUTPUT,HIGH); delay(90);
            digitalWrite(RUMBLEOUTPUT,LOW);  delay(50);
            digitalWrite(RUMBLEOUTPUT,HIGH);
        }
        once = true;
        
        if(rightSideLayout) { y = y_start - analogRead(A0); x = x_start - analogRead(A1); } //getting analog values for x and y
        else                { y = y_start - analogRead(A1); x = analogRead(A0) - x_start; }
        
        if(abs(x) > innerBorder_x) { innerBorder_x = abs(x); } //following the maximum value
        if(abs(y) > innerBorder_y) { innerBorder_y = abs(y); }
             
        x = map(x,-innerBorder_x,innerBorder_x,-JoyMax,JoyMax) * 1.3;
        y = map(y,-innerBorder_y,innerBorder_y,-JoyMax,JoyMax) * 1.3;
        x = max(-JoyMax,min(JoyMax,x)); //cropping maximum value to prevent 16 bits integer overflow
        y = max(-JoyMax,min(JoyMax,y));        
        
        if(x > -deadZone && x < deadZone){x = 0;} //if the value is too small, replace by 0
        if(y > -deadZone && y < deadZone){y = 0;}
        
       XInput.setJoystick(JOY_LEFT, (int)x,(int)y);
    }
    else if(once)
    {
        digitalWrite(RUMBLEOUTPUT,LOW); delay(80);  //rumble pattern for deactivation
        digitalWrite(RUMBLEOUTPUT,HIGH);
        
        once = false;
        XInput.setJoystick(JOY_LEFT, 0,0);
    }
}

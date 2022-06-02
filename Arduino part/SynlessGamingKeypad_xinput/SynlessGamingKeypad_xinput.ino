#include <XInput.h> //SEE https://github.com/dmadison/ArduinoXInput 
#define RUMBLEOUTPUT 17 //USED TO CONTROL THE RUMBLE MOTOR

long x, y, x_start, y_start;
const bool rightSideLayout = true;

void setup()
{  
  pinMode(RUMBLEOUTPUT, OUTPUT);
  digitalWrite(RUMBLEOUTPUT, HIGH);

  pinMode(A1, INPUT); //X OR Y
  pinMode(A0, INPUT); //X OR Y
  pinMode(A3, INPUT); //CHIP SELECT - ACTIVE LOW
  pinMode(A4, INPUT); //RIGHT OF LEFT SIDE LAYOUT - LOW = JOYSTICK ON THE RIGHT, HIGH = JOYSTICK ON THE LEFT

  rightSideLayout = analogRead(A4) < 1000;

  if (rightSideLayout)
  {
    y_start = analogRead(A0);
    x_start = analogRead(A1);
  }
  else
  {
    y_start = analogRead(A1);
    x_start = analogRead(A0);
  }
  
  //THE RUMBLE PART DOES WORK VERY WELL WITH XINPUT CALLBACKS
  //XInput.setReceiveCallback(rumbleCallback);
  
  XInput.setAutoSend(true);
  XInput.begin();
}

void rumbleCallback(uint8_t packetType)
{  
  // If we have an LED packet (0x01), do nothing
  if (packetType == (uint8_t) XInputReceiveType::LEDs)
  {
    return;
  }

  // If we have a rumble packet (0x00), see our rumble data on the LED
  else if (packetType == (uint8_t) XInputReceiveType::Rumble)
  {
    uint8_t rumbleValue = XInput.getRumbleLeft() | XInput.getRumbleRight();
    digitalWrite(RUMBLEOUTPUT, LOW);
    analogWrite(RUMBLEOUTPUT, rumbleValue);
  }
}

const int JoyMax = 32767;
const int deadZone = 1000; //BELOW THIS VALUE, VALUE = 0
int innerBorder_x = 260; //THRESHOLD FOR THE X AXIS
int innerBorder_y = 260; //THRESHOLD FOR THE Y AXIS
bool once = true;

int step = 1; //USED TO NOT BLOCK THE MAIN TASK WHILE MANAGING THE RUMBLE MOTOR
long t1 = 0;
long t2 = 0;

void loop()
{
  t1 = millis();

  //JOYSTICK PART - START ----------------------------------------------------------------
  if (analogRead(A3) < 1000) //digitalRead DOES NOT WORK FOR SOME REASON
  {
    if (!once) //IF THIS IS THE FIRST ACTIVATED CYCLE
    {
      y_start = analogRead(A0);
      x_start = analogRead(A1);
      
      step = 1;
      t2 = millis();
    }

    once = true;

    y = y_start - analogRead(A0);
    x = x_start - analogRead(A1);
    

    //THE BORDERS FOLLOW THE MAXIMUM VALUES READ
    if (abs(x) > innerBorder_x)
      innerBorder_x = abs(x);

    if (abs(y) > innerBorder_y)
      innerBorder_y = abs(y);

    //HERE, THE VALUES ARE ENHANCED THEN CROPPED IT IS GOES BEYONG 16 BITS TO PREVENT AN OVERFLOW 
    x = map(x, -innerBorder_x, innerBorder_x, -JoyMax, JoyMax) * 1.25;
    y = map(y, -innerBorder_y, innerBorder_y, -JoyMax, JoyMax) * 1.25;
    x = max(-JoyMax, min(JoyMax, x));
    y = max(-JoyMax, min(JoyMax, y));

    //IF THE VALUES ARE TOO SMALL, REPLACE THEM BY 0
    if (x > -deadZone && x < deadZone)
      x = 0;
    
    if (y > -deadZone && y < deadZone)
      y = 0;

    XInput.setJoystick(JOY_LEFT, (int)x, (int)y);
  }
  else if (once)
  {
    once = false;
    step = -1;
    t2 = millis();
    XInput.setJoystick(JOY_LEFT, 0, 0);
  }
  //JOYSTICK PART - END ------------------------------------------------------------------


  //JOYSTICK RUMBLE - START --------------------------------------------------------------
  //ASYNC RUMBLE WHEN CHANGING MODE
  if (step == 1)
  {
    digitalWrite(RUMBLEOUTPUT, LOW);

    if ((t1 - t2) > 50)
    {
      t2 = t1;
      step = 2;
    }
  }
  else if (step == 2)
  {
    digitalWrite(RUMBLEOUTPUT, HIGH);

    if ((t1 - t2) > 90)
    {
      t2 = t1;
      step = 3;
    }
  }
  else if (step == 3)
  {
    digitalWrite(RUMBLEOUTPUT, LOW);

    if ((t1 - t2) > 50)
    {
      t2 = t1;
      step = 4;
    }
  }
  else if (step == 4)
  {
    digitalWrite(RUMBLEOUTPUT, HIGH);
    t2 = t1;
    step = 0;
  }
  else if (step == -1)
  {
    digitalWrite(RUMBLEOUTPUT, LOW);

    if ((t1 - t2) > 80)
    {
      t2 = t1;
      step = -2;
    }
  }
  else if (step == -2)
  {
    digitalWrite(RUMBLEOUTPUT, HIGH);
    t2 = t1;
    step = 0;
  }
  //JOYSTICK RUMBLE - END ----------------------------------------------------------------
}

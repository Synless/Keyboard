#include <XInput.h>
#define RUMBLEOUTPUT 17

#include <EEPROM.h>
#define EEPROM_ADR 0

long x, y, x_start, y_start;
bool rightSideLayout = true;

void(* resetFunc) (void) = 0;

void setup()
{  
  pinMode(RUMBLEOUTPUT, OUTPUT);
  digitalWrite(RUMBLEOUTPUT, HIGH);

  pinMode(A1, INPUT); //X or Y
  pinMode(A0, INPUT); //X or Y
  pinMode(A3, INPUT); //Chip select - Active LOW
  pinMode(A4, INPUT); //Right of left layout - LOW = joystick on the right, HIGH = joystick on the left

  rightSideLayout = analogRead(A4) < 1000;
  if (rightSideLayout) {
    y_start = analogRead(A0);
    x_start = analogRead(A1);
  }
  else {
    y_start = analogRead(A1);
    x_start = analogRead(A0);
  }

  int eeprom_value = EEPROM.read(EEPROM_ADR);
  if(eeprom_value==0)
  {
    EEPROM.write(EEPROM_ADR,1);
  }
  else
  {
    EEPROM.write(EEPROM_ADR,0);
    resetFunc();
  }

  XInput.setReceiveCallback(rumbleCallback);
  XInput.setAutoSend(true);
  XInput.begin();
}

void rumbleCallback(uint8_t packetType)
{
  //Joystick does not work so well for now
  return;

  
  // If we have an LED packet (0x01), do nothing
  if (packetType == (uint8_t) XInputReceiveType::LEDs) {
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
const int deadZone = 1000;
int innerBorder_x = 260;
int innerBorder_y = 260;
bool once = true;
int step = 1;
long t1 = 0;
long t2 = 0;

void loop()
{
  t1 = millis();

  if (analogRead(A3) < 1000) //digitalRead does not work for some reason
  {
    if (!once) //if this is the first activated cycle ...
    {
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

      step = 1;
      t2 = millis();
    }

    once = true;

    if (rightSideLayout)
    {
      y = y_start - analogRead(A0);
      x = x_start - analogRead(A1);
    }
    else
    {
      y = y_start - analogRead(A1);
      x = analogRead(A0) - x_start;
    }

    if (abs(x) > innerBorder_x) {
      innerBorder_x = abs(x);  //following the maximum value
    }
    if (abs(y) > innerBorder_y) {
      innerBorder_y = abs(y);
    }

    x = map(x, -innerBorder_x, innerBorder_x, -JoyMax, JoyMax) * 1.25;
    y = map(y, -innerBorder_y, innerBorder_y, -JoyMax, JoyMax) * 1.25;
    x = max(-JoyMax, min(JoyMax, x)); //cropping maximum value to prevent 16 bits integer overflow
    y = max(-JoyMax, min(JoyMax, y));

    if (x > -deadZone && x < deadZone) {
      x = 0; //if the value is too small, replace by 0
    }
    if (y > -deadZone && y < deadZone) {
      y = 0;
    }

    XInput.setJoystick(JOY_LEFT, (int)x, (int)y);
  }
  else if (once)
  {
    once = false;
    step = -1;
    t2 = millis();
    XInput.setJoystick(JOY_LEFT, 0, 0);
  }

  //async rumble when changing mode
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
}

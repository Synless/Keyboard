#include QMK_KEYBOARD_H
#include "keyboard.h"
#include "analog.h"

#include "print.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [0] = LAYOUT_default(
        KC_ESC , KC_1    , KC_2    , KC_3    , KC_4   , KC_5    , KC_6,
        KC_TAB , KC_Q    , KC_W    , KC_E    , KC_R   , KC_T    , KC_Y,
        KC_TAB , KC_A    , KC_S    , KC_D    , KC_F   , KC_G    , KC_H,
        _______, KC_LSFT , KC_Z    , KC_X    , KC_C   , KC_V    , _______,
        _______, KC_LCTL , KC_LWIN , KC_LOPT , KC_SPC , _______ , _______
    )
};

//------------------------ JOYSTICK 1 VARIABLES - START
bool useJoystick = true;

const int x_1_thrs = 100;
const int y_1_thrs = 100;

int16_t x_1_startAnalogValue = 0;
int16_t y_1_startAnalogValue = 0;
int16_t x_1_analogValue = 0;
int16_t y_1_analogValue = 0;
int16_t x_1_diff = 0;
int16_t y_1_diff = 0;

bool b_1_right = false;
bool b_1_left = false;
bool b_1_up = false;
bool b_1_down = false;

bool left_right_or_up_down_1 = false;
//------------------------ JOYSTICK 1 VARIABLES - END


//------------------------ JOYSTICK 2 VARIABLES - START
const int x_2_thrs = 100;
const int y_2_thrs = 100;

int16_t x_2_startAnalogValue = 0;
int16_t y_2_startAnalogValue = 0;
int16_t x_2_analogValue = 0;
int16_t y_2_analogValue = 0;
int16_t x_2_diff = 0;
int16_t y_2_diff = 0;

bool b_2_right = false;
bool b_2_left = false;
bool b_2_up = false;
bool b_2_down = false;

bool left_right_or_up_down_2 = false;
//------------------------ JOYSTICK 2 VARIABLES - END

long counter = 0;


void keyboard_post_init_user(void)
{
	debug_enable=true;
	//debug_keyboard=true;

    setPinInput(F7);
    setPinInput(F6);
    setPinInput(F1);
    setPinInput(F0);

    setPinOutput(F4);
    writePinHigh(F4);

    //analog.c ----------- JOYSTICK 1 INIT
    x_1_startAnalogValue = analogReadPin(F7);
    y_1_startAnalogValue = analogReadPin(F6);

    //analog.c ----------- JOYSTICK 2 INIT
    x_2_startAnalogValue = analogReadPin(F0);
    y_2_startAnalogValue = analogReadPin(F1);
}

void the_other_joystick(void)
{
    left_right_or_up_down_2 = !left_right_or_up_down_2; //ONLY CHECKING ONCE EVERY OTHER LOOP UP-DOWN OR LEFT-RIGHT

    if(left_right_or_up_down_2)
    {
        y_2_analogValue = analogReadPin(F1);
        y_2_diff = y_2_startAnalogValue - y_2_analogValue;

        if(y_2_diff>y_2_thrs && !b_2_right)
        {
            register_code(KC_L);
            b_2_right = true;
        }
        else if(y_2_diff<=y_2_thrs && b_2_right) 
        {
            unregister_code(KC_L);
            b_2_right = false;
        }

        if(y_2_diff<-y_2_thrs && !b_2_left)
        {
            register_code(KC_J);
            b_2_left = true; 
        }
        else if(y_2_diff>=-y_2_thrs && b_2_left)
        {
            unregister_code(KC_J);
            b_2_left = false;
        }
    }
    else
    {
        x_2_analogValue = analogReadPin(F0);
        x_2_diff = x_2_startAnalogValue - x_2_analogValue;
        
        if(x_2_diff>x_2_thrs && !b_2_up)
        {
            register_code(KC_I);
            b_2_up = true;
        }
        else if(x_2_diff<=x_2_thrs && b_2_up)
        {
            unregister_code(KC_I);
            b_2_up = false;
        }

        if(x_2_diff<-x_2_thrs && !b_2_down)
        {
            register_code(KC_K);
            b_2_down = true; 
        }
        else if(x_2_diff>=-x_2_thrs && b_2_down)
        {
            unregister_code(KC_K);
            b_2_down = false;
        }
    }
}

void housekeeping_task_user(void)
{
    the_other_joystick();
    
    if(useJoystick)
    {
        left_right_or_up_down_1 = !left_right_or_up_down_1; //ONLY CHECKING ONCE EVERY OTHER LOOP UP-DOWN OR LEFT-RIGHT
        
        writePinHigh(F4);

        if(left_right_or_up_down_1)
        {
            y_1_analogValue = analogReadPin(F6);
            y_1_diff = y_1_startAnalogValue - y_1_analogValue;            

            if(y_1_diff>y_1_thrs && !b_1_right)
            {
                register_code(KC_RIGHT);
                b_1_right = true;
            }
            else if(y_1_diff<=y_1_thrs && b_1_right) 
            {
                unregister_code(KC_RIGHT);
                b_1_right = false;
            }

            if(y_1_diff<-y_1_thrs && !b_1_left)
            {
                register_code(KC_LEFT);
                b_1_left = true; 
            }
            else if(y_1_diff>=-y_1_thrs && b_1_left)
            {
                unregister_code(KC_LEFT);
                b_1_left = false;
            }
        }
        else
        {
            x_1_analogValue = analogReadPin(F7);
            x_1_diff = x_1_startAnalogValue - x_1_analogValue;
            
            if(x_1_diff>x_1_thrs && !b_1_up)
            {
                register_code(KC_UP);
                b_1_up = true;
            }
            else if(x_1_diff<=x_1_thrs && b_1_up)
            {
                unregister_code(KC_UP);
                b_1_up = false;
            }

            if(x_1_diff<-x_1_thrs && !b_1_down)
            {
                register_code(KC_DOWN);
                b_1_down = true; 
            }
            else if(x_1_diff>=-x_1_thrs && b_1_down)
            {
                unregister_code(KC_DOWN);
                b_1_down = false;
            }
        }
    }
    else
    {
        writePinLow(F4);

        if(b_1_up)    { unregister_code(KC_UP); b_1_up = false; }
        if(b_1_down)  { unregister_code(KC_DOWN); b_1_down = false; }
        if(b_1_right) { unregister_code(KC_RIGHT); b_1_right= false; }
        if(b_1_left)  { unregister_code(KC_LEFT); b_1_left = false; }
    }
}

bool kc_6_pressed = false; //VARIABLE USED TO CHANGE FROM QMK TO XINPUT MODE
bool mod_change = false; //VARIABLE USED TO CHANGE FROM QMK TO XINPUT MODE

bool kc_alt_pressed = false; //VARIABLE USED TO PREVENT ALT+TAB
bool kc_tab_pressed = false; //VARIABLE USED TO PREVENT ALT+TAB

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);

    if(keycode==KC_6)
    {
        if(record->event.pressed)
        {
            kc_6_pressed = true;
        }
        else
        {
            kc_6_pressed = false;

            if(!mod_change)
                tap_code(KC_6);

            mod_change = false;
        }

        return false;
    }
    else if(keycode==KC_SPC)
    {
        if(record->event.pressed)
        {
            unregister_code(KC_4);

        	if(kc_6_pressed)
            {
                uprintf("useJoystick: %u\n",useJoystick);
                useJoystick = !useJoystick;
                mod_change = true;

                return false;
            }
        }
    }
    else if(keycode==KC_LOPT)
    {
        kc_alt_pressed = record->event.pressed;

        if(kc_tab_pressed)
            unregister_code(KC_TAB);
    }
    else if(keycode==KC_TAB)
    {
        kc_tab_pressed = record->event.pressed;
        
        if(kc_alt_pressed)
            return false;
    }

    return true;
}
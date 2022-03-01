#include QMK_KEYBOARD_H
#include "keyboard.h"
#include "analog.h"
#include "print.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_default(
		KC_ESC , KC_1 , KC_2 , KC_3 , KC_4, KC_5, KC_6,
		KC_TAB , KC_Q , KC_W , KC_E , KC_R, KC_T, KC_Y,
		KC_TAB , KC_A , KC_S , KC_D , KC_F, KC_G, KC_H,
		_______ , KC_LSFT , KC_Z , KC_X , KC_C, KC_V, KC_RSFT,
		_______ , KC_LCTL , KC_LWIN , KC_LOPT , KC_SPC, KC_RCTL, _______
    )
};


//------------------------ PHYSICAL LAYOUT VARAIBLEST
const bool useLockingKey = false;
const bool rightSideLayout = true;
//------------------------ PHYSICAL LAYOUT VARAIBLEST


//------------------------ JOYSTICK VARIABLES - START
bool useJoystick = true;

const int x_thrs = 100;
const int y_thrs = 100;

int16_t x_startAnalogValue = 0;
int16_t y_startAnalogValue = 0;
int16_t x_analogValue = 0;
int16_t y_analogValue = 0;
int16_t x_prev_analogValue = 0;
int16_t y_prev_analogValue = 0;
int16_t x_diff = 0;
int16_t y_diff = 0;

bool b_right = false;
bool b_left = false;
bool b_up = false;
bool b_down = false;

bool left_right_or_up_down = false;
bool kc_6_pressed = false;
//------------------------ JOYSTICK VARIABLES - END


void keyboard_post_init_user(void)
{
	#ifdef CONSOLE_ENABLE
	    uprintf("keyboard_post_init_user\n");
	#endif

    //analog.c -------------- JOYSTICK INIT
    setPinInput(F7);
    setPinInput(F6);
    x_startAnalogValue = analogReadPin(F7);
    y_startAnalogValue = analogReadPin(F6);
    setPinOutput(F1);
    setPinOutput(F4);

	// Customise these values to desired behaviour
	//debug_enable=true;
	//debug_matrix=true;
	//debug_keyboard=true;
	//debug_mouse=true;
}

void housekeeping_task_user(void)
{
	if(useJoystick)
    {
        left_right_or_up_down = !left_right_or_up_down;
        
        writePinHigh(F4);

        if(left_right_or_up_down)
        {
            if(rightSideLayout)
            {
                y_analogValue = analogReadPin(F6);
                y_diff = y_startAnalogValue - y_analogValue;
            }
            else
            {
                y_analogValue = analogReadPin(F7);
                y_diff = y_analogValue - y_startAnalogValue;
            }

            if(y_diff>y_thrs && !b_right)
            {
            	register_code(KC_RIGHT);
            	b_right = true;
            }
            else if(y_diff<=y_thrs && b_right) 
            {
            	unregister_code(KC_RIGHT);
            	b_right = false;
            }

            if(y_diff<-y_thrs && !b_left)
            {
            	register_code(KC_LEFT);
            	b_left = true; 
            }
            else if(y_diff>=-y_thrs && b_left)
            {
            	unregister_code(KC_LEFT);
            	b_left = false;
            }
        }
        else
        {
            if(rightSideLayout)
            {
                x_analogValue = analogReadPin(F7);
                x_diff = x_startAnalogValue - x_analogValue;
            }
            else
            {
                x_analogValue = analogReadPin(F6);
                x_diff = x_startAnalogValue - x_analogValue;
            }
            
            if(x_diff>x_thrs && !b_up)
            {
            	register_code(KC_UP);
            	b_up = true;
            }
            else if(x_diff<=x_thrs && b_up)
            {
            	unregister_code(KC_UP);
            	b_up = false;
            }

            if(x_diff<-x_thrs && !b_down)
            {
            	register_code(KC_DOWN);
            	b_down = true; 
            }
            else if(x_diff>=-x_thrs && b_down)
            {
            	unregister_code(KC_DOWN);
            	b_down = false;
            }
        }
    }
    else
    {
        writePinLow(F4);

        if(b_up) 	{ unregister_code(KC_UP); 	b_up = false; 	}
        if(b_down)  { unregister_code(KC_DOWN); b_down = false; }
        if(b_right) { unregister_code(KC_RIGHT);b_right = false;}
        if(b_left) 	{ unregister_code(KC_LEFT); b_left = false; }
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	#ifdef CONSOLE_ENABLE
	    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
	#endif

	if(keycode==KC_6)
	{
		if(useLockingKey)
		{
			//made to work with a Cherry MX lock on the top line1/column7			
			useJoystick = record->event.pressed;
			
			return false;
		}
		else
		{
			#ifdef CONSOLE_ENABLE
				uprintf("kc_6_pressed: %u\n", record->event.pressed);
			#endif

			kc_6_pressed = record->event.pressed;

			return true;
		}
	}
	else if(keycode==KC_SPC)
	{
		if(record->event.pressed)
		{
			if(kc_6_pressed)
			{
				#ifdef CONSOLE_ENABLE
					uprintf("useJoystick: %u\n", useJoystick);
				#endif

				useJoystick = !useJoystick;

				return false;
			}
		}
	}

	return true;
}
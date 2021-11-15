#include QMK_KEYBOARD_H
#include "keyboard.h"
#include "analog.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_default(
		KC_ESC , KC_1 , KC_2 , KC_3 , KC_4, KC_5, KC_6,
		KC_TAB , KC_Q , KC_W , KC_E , KC_R, KC_T, KC_Y,
		KC_TAB , KC_A , KC_S , KC_D , KC_F, KC_G, KC_H,
		_______ , KC_LSFT , KC_Z , KC_X , KC_C, KC_V, KC_RSFT,
		_______ , KC_LCTL , KC_LWIN , KC_LOPT , KC_SPC, KC_RCTL, _______
    )
};

//refer to keyboard.c
bool useLockingKey = true;
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	if(keycode==KC_6)
	{
		if(useLockingKey)
		{
			//made to work with a Cherry MX lock on the top line1/column7
			if(record->event.pressed)
			{
				useJoystick = false;
			}
			else if(!record->event.pressed)
			{
				useJoystick = true;
			}
		}
		else
		{
			//made to work with regular key on the top line1/column7, toggle style
			if(record->event.pressed)
			{
				useJoystick = !useJoystick;
			}
		}

		return false;
	}
	return true;
}

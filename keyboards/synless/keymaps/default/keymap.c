#include QMK_KEYBOARD_H
#include "keyboard.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_default(
		KC_ESC , KC_1 , KC_2 , KC_3 , KC_4, KC_5, KC_6,
		KC_TAB , KC_Q , KC_W , KC_E , KC_R, KC_T, KC_Y,
		KC_TAB , KC_A , KC_S , KC_D , KC_F, KC_G, KC_H,
		KC_RSFT , KC_LSFT , KC_Z , KC_X , KC_C, KC_V, KC_RSFT,
		_______ , KC_LCTL , KC_LWIN , KC_LOPT , KC_SPC, KC_RCTL, _______
    )
};

void arduino_sync(bool isPressed)
{
	setPinOutput(F4);
	if(isPressed)
	{
		writePinHigh(F4);
		useJoystick = false;
	}
	else
	{		
		writePinLow(F4);
		useJoystick = true;
	}
}
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	if(keycode==KC_Y)
	{
		if(record->event.pressed)
			arduino_sync(true);		
		else if(!record->event.pressed)
			arduino_sync(false);

		return false;
	}
	return true;
}
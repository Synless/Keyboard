#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x2019
#define DEVICE_VER		0x0001
#define MANUFACTURER    Synless
#define PRODUCT         Keypad
#define DESCRIPTION     Gaming keypad

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 7

/* key matrix pins */
/*
PC6/PB6/PB5/PD7/PD2/PD1/PD0
PB4/PC7/PD6/PD4/PD3
*/
#define MATRIX_COL_PINS { C6, B6, B5, D7, D2, D1, D0 }
#define MATRIX_ROW_PINS { B4, C7, D6, D4, D3 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

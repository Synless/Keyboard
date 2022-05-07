# Gaming keypad
Mechanical keypad using two Atmel Atmega32u4 (one for QMK, the other one for xinput), a USB HUB, Cherry MX switches and a PSVita joystick

For the QMK part :
- Add the files of the QMK part to a QMK [keyboard] directory
- Open a QMK MSYS terminal and navigate to the root or the QMK directory
- Type "make synless:default"
- Flash `synless_default.hex` using QMK Toolbox or any other programming software you prefer

For the Arduino part :
- Refer to https://github.com/dmadison/ArduinoXInput for this part
- Add the xinpout variant `Arduino Leonardo w/ Xinput`
- Select this variant in the board selector in the Arduino IDE
- Flash and program the code

![](https://image.noelshack.com/fichiers/2021/45/2/1636447509-tangerine-1.jpg)

![](https://image.noelshack.com/fichiers/2021/45/6/1636810429-3.png)

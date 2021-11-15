# Gaming keypad
Mechanical keypad using 2xATMEL32U4 (one for QMK, the other one for xinput), a USB HUB, Cherry MX switches and a PSVita joystick :

![](https://image.noelshack.com/fichiers/2021/45/2/1636447509-tangerine-1.jpg)

![](https://image.noelshack.com/fichiers/2021/45/6/1636810429-3.png)

For the QMK part :
- Add the files of the QMK part to a fresh QMK directory
- Open a linux terminal and navigate to the root or the QMK directory
- Type "make synless:default"
- Flash `synless_default.hex` using QMK Toolbox or any other programming software you prefer

For the Arduino part :
- Refer to https://github.com/dmadison/ArduinoXInput for this part
- Add a xinpout variant of your board
- Select this variant in the board selector on the Arduino IDE
- Flash and program the code to the `Arduino Leonardo w/ Xinput`

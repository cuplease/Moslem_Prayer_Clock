### Description

In this project i made digital clock which is equipped with moslem prayer reminder feature. Thanks to several libraries available, i could make this project come alive. I used NTP (Network Time Protocol) to fetch time through internet. The time and prayer reminder will be displayed using Max7219 8x8 LED Matrix display. <br>

### Tools and Equipment
  - Wemos D1 Mini
  - Max7219 8x8 LED Dot Matrix Display
  - Buzzer
  - Power Supply

### Wiring Diagram
Here is the wiring diagram between WemosD1 Mini and LED Dot Matrix.
| Wemos D1 Mini      | LED Dot Matrix |
| ----------- | ----------- |
| VCC         | VCC       |
| GND         | GND        |
|D7 (GPIO13)|DIN|
|D5 (GPIO14)|CLK|
|D4|CS|
Actually you can change CS connection to any pin available, but for DIN and CLK pins are fixed to D7 and D5, because they need MOSI and SCLK pin from WemosD1 Mini. <br>
<br>
Here is the wiring diagram between WemosD1 mini and buzzer
| Wemos D1 Mini      | Buzzer |
| ----------- | ----------- |
| VCC         | VCC       |
| GND         | GND        |
|D3 (GPIO0)|Data|
You can also connect buzzer to any pin you like other than D3, just make sure to change the code.

### Reference
- Prayer Times Library.<br>
  https://github.com/asmaklad/Arduino-Prayer-Times
- NTP Client. <br>
  https://github.com/arduino-libraries/NTPClient
- Max 7219 Library. <br>
  https://github.com/markruys/arduino-Max72xxPanel
- Adafruit GFX Library. <br>
  https://github.com/adafruit/Adafruit-GFX-Library
Calibrate the internal oscillator (find the OSCCAL value for your MCU and change the value in the code)!

This is a port to ATTiny84a of the project IR Remote Wand v2 - see http://www.technoblogy.com/show?25TN
originally by David Johnson-Davies - www.technoblogy.com - 13th May 2018 for ATTiny85.
As I needed more pins and the ability to control three different devices I ported David's source to
ATTiny84a.
The improvement is a button that switch the functionality of all other buttons for 3 different devices,
with feedback in the status LED - one blink for device 1, 2 blinks for device 2, 3 blinks for device 3.
The code is povided as is, no further updates, bugfixes or maintenance will be made.

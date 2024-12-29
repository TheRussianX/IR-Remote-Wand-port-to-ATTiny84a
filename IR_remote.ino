/* IR Remote Wand
﻿  Ported to ATTiny84a
﻿  Set MCU at 1MHz
  The IR LED is on PA6 - Pin 6 for Arduino
  The status LED is on PA0
﻿*/
﻿//#warning "This is the CLOCKWISE pin mapping - make sure you're using the pinout diagram with the pins in clockwise order"
﻿//               ATMEL ATTINY84 / ARDUINO
﻿//
﻿//                       +-\/-+
﻿//                VCC  1|      |14  GND
﻿//        (D 10)  PB0  2|      |13  PA0  (D  0)        AREF
﻿//        (D  9)  PB1  3|      |12  PA1  (D  1)
﻿//        (D 11)  PB3  4|      |11  PA2  (D  2)
﻿//  PWM   (D  8)  PB2  5|      |10  PA3  (D  3)
﻿//  PWM   (D  7)  PA7  6|      |9   PA4  (D  4)
﻿//  PWM   (D  6)  PA6  7|      |8   PA5  (D  5)        PWM
﻿//                       +----+
﻿#include <avr/sleep.h>
﻿#include <avr/io.h>
﻿
﻿volatile int count=0;
﻿volatile unsigned int measure = 0;
﻿
﻿// IR transmitter **********************************************
﻿
﻿// Buttons
﻿
﻿const int S1 = 10;  // Rec PB0
﻿const int S2 = 9;  // Zoom In  PB1
﻿const int S3 = 8;  // Zoom Out  PB2
﻿const int S4 = 7;
﻿const int S5 = 5;
﻿
﻿const int S6 = 4;
﻿const int S7 = 3;
﻿const int S8 = 2;
﻿const int S9 = 1;
﻿
﻿const int LED = 6;        // IR LED output PA6
﻿const int LED_state = 0;  //Blink when change cam
﻿
﻿// Pin change interrupt service routine
﻿
﻿ISR(PCINT1_vect) {  // for port B pins
﻿  int in = PINB;
﻿  if (((in & (1 << PINB0)) == 0) && (count == 0)) SendSony(15, 0x5C99);  //Rec on PB0 for Sony
﻿  else if (((in & (1 << PINB0)) == 0) && (count == 1)) SendCanonP(0xFC03, 0xE383);  //Rec on PB0 for Canon
﻿  else if (((in & (1 << PINB0)) == 0) && (count == 2)) SendPanasonic(0x94CC28, 0x702002);  //Rec on PB0 for Panasonic
﻿
﻿  else if (((in & (1 << PINB1)) == 0) && (count == 0)) {  //PB1
﻿    while (digitalRead(S2) == LOW) SendSony(15, 0x6C9A);  //ZoomIn Sony active while the button is pressed
﻿  } 
﻿  else if (((in & (1 << PINB2)) == 0) && (count == 0)) {  //PB2
﻿    while (digitalRead(S3) == LOW) SendSony(15, 0x6C9B);  //ZoomOut Sony active while the button is pressed
﻿  }
﻿  else if (((in & (1 << PINB1)) == 0) && (count == 1)) {  //PB1
﻿    while (digitalRead(S2) == LOW) SendCanonP(0xE31C, 0xE383);  //ZoomIn Canon active while the button is pressed
﻿  } 
﻿  else if (((in & (1 << PINB2)) == 0) && (count == 1)) {  //PB2
﻿    while (digitalRead(S3) == LOW) SendCanonP(0xE21D, 0xE383);  //ZoomOut Canon active while the button is pressed
﻿  }
﻿  else if (((in & (1 << PINB1)) == 0) && (count == 2)) {  //PB1
﻿    while (digitalRead(S2) == LOW) SendPanasonic(0x3A6228, 0x702002);  //ZoomIn Panasonic active while the button is pressed
﻿  } 
﻿  else if (((in & (1 << PINB2)) == 0) && (count == 2)) {  //PB2
﻿    while (digitalRead(S3) == LOW) SendPanasonic(0x3B6328, 0x702002);  //ZoomOut Panasonic active while the button is pressed
﻿  }
﻿
﻿}
﻿
﻿ISR(PCINT0_vect) {  // for Port A pins
﻿  int ia = PINA;
﻿  if (((ia & 1 << S4) == 0) && (count == 0)) SendSony(12, 0x3DA);       //Display Sony
﻿  else if (((ia & 1 << S4) == 0) && (count == 1)) SendCanonP(0xB14E, 0xE383);       //Display Canon
﻿  else if (((ia & 1 << S4) == 0) && (count == 2)) SendPanasonic(0xF5728, 0x702002);       //Display Panasonic
﻿
﻿  else if (((ia & 1 << S5) == 0) && (count == 0)) SendSony(12, 0x39A);  //Play Sony
﻿  else if (((ia & 1 << S5) == 0) && (count == 1)) SendCanonP(0xDB24, 0xE383);  //Enter Canon
﻿  else if (((ia & 1 << S5) == 0) && (count == 2)) SendPanasonic(0x5828, 0x702002);  //Enter Panasonic
﻿
﻿  else if (((ia & 1 << S6) == 0) && (count == 0)) SendSony(12, 0x398);  //Stop Sony
﻿  else if (((ia & 1 << S6) == 0) && (count == 1)) SendCanonP(0xE817, 0xE383);  //Stop Canon
﻿  else if (((ia & 1 << S6) == 0) && (count == 2)) SendPanasonic(0x580028, 0x702002);  //Stop Panasonic
﻿
﻿  else if (((ia & 1 << S7) == 0) && (count == 0)) SendSony(12, 0x39B);  //Rewind Sony
﻿  else if (((ia & 1 << S7) == 0) && (count == 1)) SendCanonP(0xDC23, 0xE383);  //Left Canon
﻿  else if (((ia & 1 << S7) == 0) && (count == 2)) SendPanasonic(0x8AD329, 0x702002);  //Left Panasonic
﻿
﻿  else if (((ia & 1 << S8) == 0) && (count == 0)) SendSony(12, 0x39C);  //Fast-Forward
﻿  else if (((ia & 1 << S8) == 0) && (count == 1)) SendCanonP(0xDD22, 0xE383);  //Right Canon
﻿  else if (((ia & 1 << S8) == 0) && (count == 2)) SendPanasonic(0x8BD229, 0x702002);  //Right Panasonic
﻿
﻿  else if ((ia & 1 << S9) == 0) {
﻿    count = count+1;
﻿    if (count>2) (count=0);
﻿    for (int i=0; i<(count+1); i++) {
﻿      BlinkLed(20000,20000); // blinks the status LED on for 1 sec off for 1 sec; once - Sony, twice Canon, 3 times Panasonic
﻿    }
﻿
﻿  }
﻿}
﻿
﻿const int top = 25;    // 1000000/26 = 38.5kHz
﻿const int match = 19;  // approx. 25% mark/space ratio
﻿
﻿// Set up Timer/Counter1 to output PCM on OC1A (PA6)
﻿void SetupPCM() {
﻿  // Inverted fast PWM output on OC1A (PA6) divide by 1 with TOP value ICR1
﻿  TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (1 << WGM11);
﻿  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
﻿  ICR1 = top;   // 38.5kHz
﻿  OCR1A = top;  // Keep output low
﻿}
﻿
﻿// Generate count cycles of carrier followed by gap cycles of gap
﻿void Pulse(int count, int gap) {
﻿  OCR1A = match;  // Generate pulses
﻿  for (int i = 0; i < 2; i++) {
﻿    for (int c = 0; c < count; c++) {
﻿      while ((TIFR1 & 1 << TOV1) == 0);
﻿      TIFR1 = (1 << TOV1);
﻿    }
﻿    count = gap;  // Generate gap
﻿    OCR1A = top;
﻿  }
﻿}
﻿
﻿void BlinkLed(int count, int gap) {
﻿  digitalWrite(LED_state, HIGH);  // blinks the status LED
﻿  for (int i = 0; i < 2; i++) {
﻿    for (int c = 0; c < count; c++) {
﻿      while ((TIFR1 & 1 << TOV1) == 0);
﻿      TIFR1 = (1 << TOV1);
﻿    }
﻿    count = gap;  // Generate gap
﻿  digitalWrite(LED_state, LOW);
﻿  }
﻿}
﻿
﻿void SendSony(int NumberBits, unsigned long raw_code) {  //send Sony codes, 40KHz
﻿  const int HeaderUp = 96;                               //count/gap = duration[microsec]/(top+1)
﻿  const int HeaderGap = 24;
﻿  const int Bit1Up = 48;
﻿  const int Bit1Gap = 24;
﻿  const int Bit0Up = 24;
﻿  const int Bit0Gap = 24;
﻿
﻿  for (int repeat = 0; repeat < 5; repeat++) {  //original remote send each code 5 times
﻿    Pulse(HeaderUp, HeaderGap);
﻿    for (int Bit = 0; Bit < NumberBits; Bit++) {
﻿      if (raw_code & (unsigned long)1 << Bit) {
﻿        //Serial.print('1');
﻿        Pulse(Bit1Up, Bit1Gap);
﻿      } else {
﻿        //Serial.print('0');
﻿        Pulse(Bit0Up, Bit0Gap);
﻿      }
﻿    }
﻿    Pulse(0, 778);  //gap between transmissions
﻿  }
﻿}
﻿
﻿void SendCanonP(unsigned long raw_code2, unsigned long raw_code1) { //where Hex 32bits is made by raw_code1||raw_code_2, 38KHz
﻿  const int HeaderUp = 349;  // Tiny can't send reliably 32 bits in one sequence, so I split it in 2 x 16 bits
﻿  const int HeaderGap = 174;  
﻿  const int Bit1Up = 21;  // count/gap = duration[microsec]/(top+1)
﻿  const int Bit1Gap = 62; 
﻿  const int Bit0Up = 21;
﻿  const int Bit0Gap = 21;
﻿  TCNT1 = 0;                            // Start counting from 0
﻿
﻿  Pulse(HeaderUp, HeaderGap);   //send header
﻿  for (int Bit = 0; Bit<16; Bit++) {
﻿    if (raw_code1 & (unsigned long) 1<<Bit) {
﻿     Pulse(Bit1Up, Bit1Gap);
﻿     } else {       
﻿     Pulse(Bit0Up, Bit0Gap);
﻿    }
﻿  }
﻿  for (int Bit = 0; Bit<16; Bit++) {
﻿    if (raw_code2 & (unsigned long) 1<<Bit) {
﻿      Pulse(Bit1Up, Bit1Gap);
﻿    } else {
﻿     Pulse(Bit0Up, Bit0Gap);
﻿    }
﻿  }
﻿  Pulse(Bit0Up, 735); //end of transmission 0 bit and gap between transmissions
﻿}
﻿
﻿void SendPanasonic(unsigned long raw_code2, unsigned long raw_code1) { //where Hex 48bits is made from raw_code1||raw_code_2, 37KHz
﻿  const int HeaderUp = 128; 
﻿  const int HeaderGap = 64;
﻿  const int Bit1Up = 16;
﻿  const int Bit1Gap = 48;
﻿  const int Bit0Up = 16;
﻿  const int Bit0Gap = 16;
﻿  TCNT1 = 0;                            // Start counting from 0
﻿
﻿  Pulse(HeaderUp, HeaderGap);   //send header
﻿  for (int Bit = 0; Bit<24; Bit++) {
﻿    if (raw_code1 & (unsigned long) 1<<Bit) {
﻿     Pulse(Bit1Up, Bit1Gap);
﻿     } else {       
﻿     Pulse(Bit0Up, Bit0Gap);
﻿    }
﻿  }
﻿  for (int Bit = 0; Bit<24; Bit++) {
﻿    if (raw_code2 & (unsigned long) 1<<Bit) {
﻿      Pulse(Bit1Up, Bit1Gap);
﻿    } else {
﻿     Pulse(Bit0Up, Bit0Gap);
﻿    }
﻿  }
﻿  Pulse(Bit0Up, 2725); //end of transmission 0 bit and gap between transmissions
﻿}
﻿
﻿// Setup demo **********************************************
﻿
﻿void setup() {
﻿  OSCCAL = 128;  //MCU dependent
﻿  pinMode(LED, OUTPUT);
﻿  pinMode(LED_state, OUTPUT);
﻿  pinMode(S1, INPUT_PULLUP);
﻿  pinMode(S2, INPUT_PULLUP);
﻿  pinMode(S3, INPUT_PULLUP);
﻿  pinMode(S4, INPUT_PULLUP);
﻿  pinMode(S5, INPUT_PULLUP);
﻿  pinMode(S6, INPUT_PULLUP);
﻿  pinMode(S7, INPUT_PULLUP);
﻿  pinMode(S8, INPUT_PULLUP);
﻿  pinMode(S9, INPUT_PULLUP);
﻿  digitalWrite(LED_state, LOW);
﻿
﻿  SetupPCM();
﻿  // Configure pin change interrupts to wake on button presses
﻿  PCMSK1 |= (1<< PCINT8) | (1<< PCINT9) | (1<< PCINT10); //Mask PB pins
﻿  PCMSK0 |= (1<< PCINT1) | (1<< PCINT2) | (1<< PCINT3) | (1<< PCINT4) | (1<< PCINT5) | (1<< PCINT7); //Mask PA pins
﻿  GIMSK |= (1 << PCIE1) | (1 << PCIE0);  // Enable interrupts
﻿  GIFR |= (1 << PCIF1) | (1 << PCIF0);   // Clear interrupt flag
﻿  // Disable what we don't need to save power
﻿  ADCSRA &= ~(1 << ADEN);         // Disable ADC
﻿  PRR = 1 << PRUSI | 1 << PRADC;  // Turn off clocks to unused peripherals
﻿  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
﻿}
﻿
﻿// Stay asleep and just respond to interrupts
﻿void loop() {
﻿  sleep_enable();
﻿  sleep_cpu();
﻿}

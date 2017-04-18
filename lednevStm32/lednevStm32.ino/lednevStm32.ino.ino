#include <EEPROM.h>
#include <flash_stm32.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  Modified by Roger Clark. www.rogerclark.net for Maple mini 25th April 2015 , where the LED is on PB1
  
 */


// the setup function runs once when you press reset or power the board
uint16 short_period = 100;
uint16 long_period = 1;
long very_long_period;
uint16 AddressWrite = 0x10;

void setup() {
  // initialize digital pin PB1 as an output.

  Serial.begin(57600);
  Serial3.begin(9600);
  pinMode(PC13, OUTPUT);
  pinMode(PB12, OUTPUT);
  
  EEPROM.PageBase0 = 0x801F000;
  EEPROM.PageBase1 = 0x801F800;
  EEPROM.PageSize = 0x400;
  EEPROM.read(AddressWrite, &long_period);
  EEPROM.read(AddressWrite + 1, &short_period);
  
  very_long_period = 1000000 / long_period;
  Timer1.setChannel1Mode(TIMER_OUTPUTCOMPARE);
  Timer1.setPeriod(very_long_period);
  Timer1.attachInterrupt(0, Timer_interrupt);
}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);              // wait for a second
//  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);// wait for a second

  
  if (Serial3.available() > 0) {
    int command = Serial3.parseInt();
    if ( command != 0 ) {
      byte freq = lowByte(command);
      byte duty = highByte(command);
      EEPROM.write(AddressWrite, freq);
      EEPROM.write(AddressWrite + 1, duty);
      short_period = duty;
      very_long_period = 1000000 / freq;
      Timer1.setPeriod(very_long_period);
    }
  }
}

void Timer_interrupt() {
  digitalWrite(PC13, !digitalRead(PC13));
  GPIOB->regs->BSRR = 1 << 12;
  delayMicroseconds(short_period);
  GPIOB->regs->BRR = 1 << 12;
}

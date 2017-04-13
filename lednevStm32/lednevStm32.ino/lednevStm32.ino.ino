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
byte short_period = 1;
byte long_period = 1;
long very_long_period;
void setup() {
  // initialize digital pin PB1 as an output.
  very_long_period = 500000 / long_period;
  Serial.begin(57600);
  Serial3.begin(9600);
  pinMode(PC13, OUTPUT);
  //pinMode(PB11, OUTPUT);
  
  Timer1.setPeriod(1);
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
      short_period = duty;
      very_long_period = 500000 / freq;
      Timer1.setPeriod(very_long_period);
      
    } else {
        
    }
  }
}

void Timer_interrupt() {
  digitalWrite(PC13, !digitalRead(PC13));   // turn the LED on (HIGH is the voltage level)

//  GPIOB->regs->BSRR = 1 << 11;
//  delayMicroseconds(short_period);
//  GPIOB->regs->BRR = 1 << 11;
  //digitalWrite(PB11, LOW);
}

//arduino bike speedometer w serial.print()
//by Amanda Ghassaei 2012
//https://www.instructables.com/id/Arduino-Bike-Speedometer/

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

*/
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
//calculations
//tire radius ~ 9.5 inches
//circumference = pi*2*r =~ 60inches
//max speed of 35mph =~ 60inches/second
//max rps =~7.25

#define reed 2//pin connected to read switch

//storage variables
int reedVal;
long timer;// time between one full rotation (in ms)
float mph;
float kph;
float radius = 9.5;// tire radius (in inches)
float circumference;
unsigned long time_counter;
float prev_speed = 0;
float distance_covered = 0;

int maxReedCounter = 40;//min time (in ms) of one rotation (for debouncing)
unsigned long debouncer ;
int reedCounter;
unsigned long int revs = 0;
int high_speed = 0;
unsigned long int rev_time = 0;


void setup() {

  reedCounter = maxReedCounter;
  circumference = 2 * 3.14 * radius;
  pinMode(reed, INPUT);

  // TIMER SETUP- the timer interrupt allows precise timed measurements of the reed switch
  //for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();//stop interrupts

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  //END TIMER SETUP

  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
}


ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal) { //if reed switch is closed
    time_counter = millis();
    if (reedCounter == 0) { //min time between pulses has passed

      mph = (56.8 * float(circumference)) / float(timer); //calculate miles per hour
      kph = (mph * 1.609);


      if (((int)kph != 132) ) {
        prev_speed = kph;
        //        Serial.print("int reading is");
        //        Serial.println((int)kph);
      if(kph > high_speed){
        high_speed = kph;}
      } else {
        kph = prev_speed;
      }



      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
    }
    else {
      if (reedCounter > 0) { //don't let reedCounter go negative
        reedCounter -= 1;//decrement reedCounter
      }
    }

    // distance measuring code
    if(millis()-debouncer >= 40){
    distance_covered += 0.0254*(float)circumference;
        debouncer = millis();
    }

    
  } else if ((millis() - time_counter) >= 1000) {
    mph = 0;
    kph = 0;
    timer = 0;
    reedCounter = maxReedCounter;
  } else { //if reed switch is open
    if (reedCounter > 0) { //don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 1000) {
    mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else {
    timer ++;//increment timer
  }

  
}

void displayMPH() {
  Serial.print(kph);
  Serial.print("\t");
  Serial.print(distance_covered);
  Serial.print("\t");
  Serial.print(high_speed);
  Serial.println();
  
  lcd.setCursor(0, 0);
  lcd.print(kph);
  lcd.setCursor(0,7);
  lcd.print(high_speed);
  lcd.setCursor(0, 1);
  lcd.print(distance_covered);
  lcd.setCursor(8, 1);
  lcd.print("M");
}

void loop() {
  //print mph once a second
  displayMPH();

  delay(50);
}

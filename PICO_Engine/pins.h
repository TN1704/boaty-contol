# ifndef PINS_H
# define PINS_H

//Pin Set-Up
const int servo_pin = 2;  //Throttle Servo pin
const int servo1_pin = 3;  //Clutch servo pin
const int fuel_pin = 5;  //fuel pump pin
const int fuel_level = 26; //fuel sender pin
const int reserve_level = 27; //reserve tank fuel sender.
const int low_fuel_pump = 7; // intertank low presure fuel pump
const int FRAM_CS = 10; // chip select pin for the FRAM!
const int uart2_rx = 5;
const int uart2_tx = 4;
const int starter_pin = 8; //pin for the ssr to activate the starter solenoid!

void pin_setup(){

  pinMode(fuel_pin, OUTPUT);
  pinMode(fuel_level, INPUT);
  pinMode(reserve_level, INPUT);
  pinMode(low_fuel_pump, OUTPUT);
  pinMode(starter_pin, OUTPUT);
}
#endif
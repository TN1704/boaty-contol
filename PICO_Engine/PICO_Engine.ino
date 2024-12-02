// Engine control pico address 3

# include <Servo.h>
# include "config.h"
# include "pins.h"
# include "variables.h"
# include "functions.h"


void setup() {
  // put your setup code here, to run once:

bool ready = false;
fuel_on = false;

// servo setup
Throttle.attach(servo_pin);
Throttle.write(0);
Clutch.attach(servo1_pin);
Clutch.write(clutch_max);

//pin set up
pinMode(coolant_pin, OUTPUT);
pinMode(fuel_pin, OUTPUT);
pinMode(coolant_valve_pin, OUTPUT);
pinMode(fuel_level, INPUT);
pinMode(reserve_level, INPUT);
pinMode(low_fuel_pump, OUTPUT);

//Serial Setup
Serial.begin(9600);
Serial1.begin(9600);

digitalWrite(low_fuel_pump, LOW);
digitalWrite(fuel_pin, LOW);
digitalWrite(coolant_pin, LOW);
digitalWrite(coolant_valve_pin, LOW);


comms(DASH_ADDRESS, 5, 1);

while (!ready) {
  if(Serial1.available() >=2){
    address = Serial1.read();
    val = Serial1.read();
  }
  if(address == 5 && val == 1){
    ready = true;
  }
}
}



void loop() {
  // put your main code here, to run repeatedly:
if (Serial1.available() >= 2){
  address = Serial1.read();
  val = Serial1.read();}

switch (address){
  case 6:  //throttle postion
  throttle(val);
  address = 0;
  break;

  case 3:  //clutch state (on or off)
    if(val == 1){
      clutch_movement(1);
      coolant_on = false;
      comms(DASH_ADDRESS, 3, 1);
    }
    else if(val == 2){
      clutch_movement(0);
      coolant_on = true;
      comms(DASH_ADDRESS, 3, 2);
  }
  address = 0;
  break;

  case 2:  // fuel switch state
  if(val == 1){
    fuel_on = true;
    fuel_conf = false;
    address = 0;
  }
  else if (val == 0) {
  fuel_on = false;
  fuel_conf = false;
  address = 0;
  }
  break;

  case 4:  //reserve tank
    if(val == 0){
      is_reserve = false;
    }
    else if(val == 1){
      is_reserve = true;
    }
    address = 0;
    break;


  default:
  break;
}

if (fuel_on && !fuel_conf){
  digitalWrite(fuel_pin,HIGH);
  fuel_conf = true;
  comms(DASH_ADDRESS,5,1);}
else if(!fuel_on && !fuel_conf) {
  digitalWrite(fuel_pin, LOW);
  fuel_conf = false;
  comms(DASH_ADDRESS,5,0);}

if(coolant_on){
  coolant(true);
}
else{
  coolant(false);
}

fuel_amount = analogRead(fuel_level);
if(fuel_amount != fuel_previous){
  comms(DASH_ADDRESS, 4, round(map(fuel_amount,0,4095,0,240)));
  fuel_previous = fuel_amount;}

if(is_reserve && map(analogRead(reserve_level),0,4095,0,100) >=5){
  digitalWrite(low_fuel_pump, HIGH);
  }
else{
  digitalWrite(low_fuel_pump, LOW);
}

}


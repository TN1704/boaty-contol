// Engine control pico address 3


# include <SPI.h>
# include <Adafruit_FRAM_SPI.h>
# include <Servo.h>
# include "pins.h"
# include "config.h"
# include "variables.h"
# include "functions.h"


void setup() {
  bool ready = false;
  fuel_on = false;

  pin_setup();
  initial_setup();


  if(!throttle_check()){
    comms(DASH_ADDRESS, 2, 5, 2);
    //error state code to be written!
  }


  comms(DASH_ADDRESS, 2, 5, 1);

  while (!ready) {
    if(Serial1.available() >=3){
      check_can();
    }
    if(code == 5 && data[0] == 1){
      ready = true;
    }
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() >= 3){
      check_can();}

  switch (code){
    case 6:  //throttle postion
    throttle(data[0]);
    code = 0;
    break;

    case 2:  // fuel switch state
    if(data[0] == 1){
      fuel_on = true;
      fuel_conf = false;
      code = 0;
    }
    else if (data[0] == 0) {
    fuel_on = false;
    fuel_conf = false;
    code = 0;
    }
    break;

    case 4:  //reserve tank
      if(data[0] == 0){
        is_reserve = false;
      }
      else if(data[0] == 1){
        is_reserve = true;
      }
      code = 0;
      break;
    
    case 9: //Engine start
      if(data[0] == 1){
        engine_start();
        code = 0;
      }
      code = 0;
      break;

    case 10: //request for rpm
      if(data[0 == 1]){
        engine_data();
        uint16_t rpm = buffer[15] <<8 | buffer[14];
        comms(DASH_ADDRESS, 3, 10, rpm);
      }
    break;

    default:
    break;
  }

  if (fuel_on && !fuel_conf){
    digitalWrite(fuel_pin,HIGH);
    fuel_conf = true;
    comms(DASH_ADDRESS, 2, 5, 1);}
  else if(!fuel_on && !fuel_conf) {
    digitalWrite(fuel_pin, LOW);
    fuel_conf = false;
    comms(DASH_ADDRESS, 2, 5, 0);}


  fuel_amount = analogRead(fuel_level);
  if(fuel_amount != fuel_previous){
    comms(DASH_ADDRESS, 3, 4, fuel_amount);
    fuel_previous = fuel_amount;}

  if(is_reserve && map(analogRead(reserve_level),0,4095,0,100) >=5){
    digitalWrite(low_fuel_pump, HIGH);
    }
  else{
    digitalWrite(low_fuel_pump, LOW);
    }

}


// CAN Address 1 for this module.

#include "ReadMe.h"
#include <U8g2lib.h>
#include <SPI.h>
#include <TinyGPSPlus.h>
#include "images.h"
#include "pins.h"
#include "variables.h"
#include "config.h"
#include "functions.h"


void setup() {
  bool cont1 = false;
  bool cont2 = false;
  //call the pin set up function
  pin_setup();
  initial_setup();
  //Saftey interrupt.
  attachInterrupt(digitalPinToInterrupt(kill_switch),kill_system, HIGH);
  
  //Screen Welcome
  print_image(1, welcome_wait);
  print_image(2, welcome_wait);


  //make sure all other controlers are initialized & all in sync!  
  while(cont1 && cont2 == false){
    if(Serial2.available() >= 2){
      code = Serial2.read();
      data = Serial2.read();

      switch(code){
        case 5:
          switch(data){
            case 1:
            cont1 = true;
            data = 0;
            break;

            case 2:
            cont2 = true;
            data = 0;
            break;

            default:
            data = 0;
            break;
          }
          code = 0;
          break;
        
        default:
        code = 0;
        break;
      }
    }
  }

  // remove please wait screen and update to welcome!
  print_image(1,welcome);
  print_image(2,welcome);
  comms(ENGINE_ADDRESS,5,1);
  comms(CONTROLL_ADDRESS,5,1);
}

void loop() {
  InputStates();

  // Check if ignition and kill switch are on
  while (isKill == LOW && isIgnition == HIGH) {
    InputStates();
    //Fuel Pump check
    if(isFuel != fuel_conf){
      comms(ENGINE_ADDRESS,3, isFuel ? 0 : 1);
      fuel_conf = isFuel;
    }

    //Clutch Check
    if(isClutch != clutch_conf){
      if(!isClutch){
      comms(ENGINE_ADDRESS, 2, 2);
      clutch_conf = isClutch;}
      else if(isClutch){
      comms(ENGINE_ADDRESS, 2, 1);
      clutch_conf = isClutch;}
    }
   
    //Drive Check
    if(isDrive != drive_conf && !isNeutral){
      if(!isDrive){
        comms(CONTROLL_ADDRESS, 1, 1);
        digitalWrite(drive_led,HIGH);
        drive_conf = isDrive;}
      else if(isDrive){
        comms(CONTROLL_ADDRESS, 1, 2);
        digitalWrite(drive_led, LOW);
        drive_conf = isDrive;}
    }
  
    //Reverse Check
    if(isReverse != reverse_conf && !isNeutral){
      if(!isReverse){
        comms(CONTROLL_ADDRESS, 1, 3);
        digitalWrite(reverse_led, HIGH);
        reverse_conf = isReverse;}
      else if(isReverse){
        comms(CONTROLL_ADDRESS, 1, 2);
        digitalWrite(reverse_led, LOW);
        reverse_conf = isReverse;}
    }

    // Neutral Check
    if(isNeutral != neutral_conf){
      if(!isNeutral){
        comms(CONTROLL_ADDRESS, 1, 2);
        digitalWrite(neutral_led, HIGH);
        neutral_conf = isNeutral;}
      else if(isNeutral){
        comms(CONTROLL_ADDRESS, 1, 2);
        digitalWrite(neutral_led, LOW);}
    }

    //Ignition Check
    if(isIgnition != ignition_conf){
      if(isIgnition){
        digitalWrite(ignition, HIGH);
        ignition_conf = isIgnition;}
      else if(!isIgnition){
        digitalWrite(ignition, LOW);
        ignition_conf = isIgnition;}
    }

    //Reserve Tank Check
    if(isReserve != reserve_conf){
      if(isReserve){
      comms(ENGINE_ADDRESS, 4, 1);
      reserve_conf = isReserve;
      print_image(2, fuel_image);
      }
      else if(!isReserve){
        comms(ENGINE_ADDRESS, 4, 2);
        reserve_conf = isReserve;
        screen2.setDrawColor(0);
        screen2.drawBox(98, 32, 59, 12);
      }
    }

    //Start Engine
    if(debounce()){
      print_image(1, engine);
      print_image(2, engine);
      isOn = false;
      isStart = false;
      while(debounce() && isClutch){
        if(!isOn){
          comms(CONTROLL_ADDRESS, 4, 1);
          isOn = !isOn;}
      }
    }
    else if(isOn && !isStart){
      comms(CONTROLL_ADDRESS, 4, 0);
      print_image(1, main_display);
      print_image(2,fuel_image);
      isStart = true;
      }

 
  //GPS Processing
    if (Serial1.available()>0){
      if(gps.encode(Serial1.read())){
        if(gps.location.isUpdated()){
          //Get New position & time
          lat2 = gps.location.lat();
          lon2 = gps.location.lng();
          t2 = millis();
          hours = gps.time.hour();
          mins = gps.time.minute();
          seconds = gps.time.second();
          //check if not first loop
          if(lat1 !=0 && lon1 !=0){
            double distance = TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2);

            //calc time difference in seconds
            double time = (t2 - t1)/1000.0;

            //calculate spped in m/s
            speed = distance/time;

            speed *= 1.94384;
          }
        }
        else{
          speed = 0.0; //if no speed avoid division errors!
        }
        lat1 = lat2;
        lon1 = lon2;
        t1 = t2;
      }
    }
  

 
 if (Serial2.available() >= 2){
  code = Serial2.read();
  data = Serial2.read();
 }

 switch(code){
  case 1:
    switch (data) {
      case 1:
        print_image(1, main_display_d);
        data = 0;
      break;
      case 2:
        print_image(1, main_display_n);
        data = 0;
       break;
       case 3:
        print_image(1, main_display_r);
        data = 0;
      break;
      default:
      data = 0;
      break;}
  code = 0;
  break;

  case 2:
    switch (data) {
    case 1:
      digitalWrite(fuel_led,HIGH);
      data = 0;
      break;
    
    case 2:
      digitalWrite(fuel_led,LOW);
      data = 0;
      break;
    }
  code = 0;
  break;

  case 3:
    switch (data) {
      case 1:
      digitalWrite(clutch_led, HIGH);
      data = 0;
      break;

      case 2:
      digitalWrite(clutch_led, LOW);
      data = 0;
      break;
    default:
    data = 0;
    break;
    }
  code = 0;
  break;

  case 4:
    fuel_current = round(data);
    code = 0;
  break;

  default:
  code = 0;
  break;
 }

 //throttle position read
  throttle = analogRead(throttle_pot);
  throttle_send = constrain(map(throttle, 0 ,4095, 0, 255), 0, 255);
  if(throttle_send != throttle_send_old){
    comms(ENGINE_ADDRESS, 6, throttle_send);
    throttle_send_old = throttle_send;}

  if(map(fuel_current, 0, 240, 0, 100) <= 20){
    digitalWrite(low_fuel,HIGH);
  }
  else{
    digitalWrite(low_fuel, LOW);
  }
  // Screen Update
  if(isStart){
     if(speed != lastSpeed || throttle != lastThrottle){
      lastSpeed = speed;
      lastThrottle = throttle;
      //set the colour and font
      screen1.setDrawColor(1);
      screen1.setFont(u8g2_font_t0_22b_te);

      //draw speed
      screen1.setCursor(16,60);
      screen1.print(speed,2);
      screen1.print("Knots");

      //draw throttle
      screen1.setCursor(30, 90);
      screen1.print(throttle,3);
      screen1.print("%");

      //draw time from GPS
      screen1.setCursor(45, 56);
      screen1.print(hours);
      screen1.print(":");
      screen1.print(mins);
      screen1.print(":");
      screen1.print(seconds);
      }
    if(fuel_current != fuel_previous){
      screen2.drawBox(fuel_current - 8, 6, 240 - fuel_current, 8);
      fuel_previous = fuel_current;
    }

  }
  Fuel_Level();
}

  // If ignition or kill switch is off, turn off the ignition
  digitalWrite(ignition, LOW);
  comms(ENGINE_ADDRESS, 3, 0);
  print_image(1, engine_off);
  print_image(2, engine_off);

}

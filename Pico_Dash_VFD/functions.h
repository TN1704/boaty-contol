#include "Arduino.h"
// function to keep all the finctions for the main ino file in one place

# ifndef FUNCTIONS_H
# define FUNCTIONS_H

//image printing function
void print_image(int display, const unsigned char* image){
  if(display == 1){
    screen1.clear();
    screen1.setDrawColor(1);
    screen1.drawXBMP(0, 0, 256, 50, image);
    screen1.sendBuffer();
  }
  else if (display == 2) {
  screen2.clear();
  screen2.setDrawColor(1);
  screen2.drawXBM(0, 0, 256, 50, image);
  screen2.sendBuffer();
  }
}

//function for comms
void comms(u_int16_t address, int code, int data){
  Serial2.write((address >> 8) & 0xff);
  Serial2.write(address & 0xff);
  Serial2.write(2); //Send message length (only1)
  Serial2.write(code); //Send Message data to WS-TTL-CAN
  Serial2.write(data);
}
//Kill system function
void kill_system(){
  digitalWrite(ignition,LOW);
  if(!isOn){
    screen1.clear();
    screen1.drawXBMP(0, 0, 256, 50, killswitch);
    screen1.sendBuffer();
    screen2.clear();
    screen2.drawXBMP(0, 0, 256, 50, killswitch);
    screen2.sendBuffer();  }
  isOn = true;
}

//Debouncing logic for starter button
bool debounce(){
  static unsigned long lastDebounceTime = 0;
  static bool lastButtonState = LOW;
  bool currentstate = digitalRead(start_button);

  if(currentstate != lastButtonState){
    lastDebounceTime = millis(); //reset the counter
    }
  if((millis() - lastDebounceTime) > 50){
    if(currentstate == HIGH){
      lastButtonState = currentstate;
      return true;
    }
  }
  lastButtonState = currentstate;
  return false;
}

//Input state check
void InputStates(){
    isKill = !digitalRead(kill_switch);
    isDrive = !digitalRead(drive_input);
    isReverse = !digitalRead(reverse_input);
    isClutch = !digitalRead(clutch_input);
    isFuel = !digitalRead(fuel_input);
    isIgnition = digitalRead(key_switch);
    isReserve = digitalRead(reserve);
    if (digitalRead(drive_input) && digitalRead(reverse_input)){
      isNeutral = true;
    }
    else{
      isNeutral = false;
    }
}

//fuel level check
void Fuel_Level(){
  if(map(fuel_current, 0, 256, 0, 100) <= low_fuel_level){
    digitalWrite(low_fuel, HIGH);
  }
  else{
    digitalWrite(low_fuel, LOW);
  }
}

void initial_setup(){

    // Serial1 for GPS
  // Serial2 for interpico coms
  Serial.begin(9600);
  Serial1.setRX(gps_rx);
  Serial1.setTX(gps_tx);
  Serial1.begin(9600);
  Serial2.setRX(can_rx);
  Serial2.setTX(can_tx);
  Serial2.begin(9600);

  digitalWrite(screen1_cs, HIGH);
  digitalWrite(screen2_cs, HIGH);

  screen1.begin();
  screen2.begin();

// set the bools for start up
  isDrive = false;
  isReverse = false;
  isNeutral = false;
  isOn = false;
  fuel_conf = false;
  fuel = false;
  isReserve = false;
  bool cont1 = false;
  bool cont2 = false;

  // variable set
  current_drive = -1;
  clutch_conf = false;

  //GPS init
  lat1 = lon1 = 0;
  speed = 0.0;
  Serial1.println("$PMTK220,200*2C\r\n");
  digitalWrite(drive_led, HIGH);
  digitalWrite(reverse_led, HIGH);
  digitalWrite(neutral_led,HIGH);
  digitalWrite(fuel_led, HIGH);
  digitalWrite(clutch_led, HIGH);
  digitalWrite(low_fuel, HIGH);
  delay(1000);
  digitalWrite(drive_led, LOW);
  digitalWrite(reverse_led, LOW);
  digitalWrite(neutral_led,LOW);
  digitalWrite(fuel_led, LOW);
  digitalWrite(clutch_led, LOW);
  digitalWrite(low_fuel, LOW);
}

bool check_can(){
  uint8_t byte1 = Serial2.read();
  uint8_t byte2 = Serial2.read();
  int dlc = Serial2.read();
  rec_address = (uint16_t(byte1) << 8) | uint16_t(byte2);
  delay(50);

  for(int i = 0; i < dlc; i++){
      if (i == 0) {
      code = Serial2.read();
      }
      else {
      data[i-1] = Serial2.read();
      }
  }
return true;
}

void error_state(){
  // run this to take control of the main control to investigate error. (infinate while loop until exit or system restart)

}

#endif
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

}

#endif
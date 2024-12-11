#include "SerialUART.h"
#include "api/Common.h"
# ifndef FUNCTIONS_H
# define FUNCTIONS_H

void comms(uint16_t address, int dlc, int code, uint16_t data){
  Serial1.write((address >> 8) & 0xff);
  Serial1.write(address & 0xff);
  Serial1.write(dlc);
  Serial1.write(code);
  if(dlc > 2){
    uint8_t lowByte = data & 0xFF;
    uint8_t highByte = (data >> 8) & 0xFF;
    Serial1.write(lowByte);
    Serial1.write(highByte); 
  }
  else{
  Serial1.write(data);
  }
}

void throttle(int value){
  // TODO: ensure that if the value is unchanged to to write to servo!
  throttle_current = map(value,0,255,0,throttle_max);
  if(throttle_current != throttle_previous){
    Throttle.write(throttle_current);
    throttle_previous = throttle_current;
  }

}

/*Funtion to moving the clutch
void clutch_movement(bool engaged){
  if(engaged){
    Clutch.write(0);
  }
  else if(!engaged){
     for(int i = Clutch.read(); i < clutch_max; i ++){
      Clutch.write(i);
      delay(1);
    }
  }
}*/

// Function for recording runtime

void run_time(){
  time_current = millis() - time_previous;
  uint16_t faddress = FRAM_SIZE - 4;
  uint8_t buffer[4];
  uint32_t memory;
  flash.read(faddress,buffer,4);
  memory = *(uint32_t *)buffer;

  time_current = time_current + memory;

  flash.write(faddress, (uint8_t *)& time_current, 4);

  time_previous = millis();

}

//check for CAN messages
bool check_can(){
  uint8_t byte1 = Serial1.read();
  uint8_t byte2 = Serial1.read();
  int dlc = Serial1.read();
  rec_address = (uint16_t(byte1) << 8) | uint16_t(byte2);
  delay(50);

  for(int i = 0; i < dlc; i++){
      if (i == 0) {
      code = Serial1.read();
      }
      else {
      data[i-1] = Serial1.read();
      }
  }
  return true;
}

//check if the throttle servo is functional.
bool throttle_check(){
  Throttle.write(throttle_max);
  delay(200);
  int pos = Throttle.read();
  if(pos != throttle_max){
    return false;
  }
  Throttle.write(0);
  delay(200);
  pos = Throttle.read();
  if(pos != 0){
    return false;
  }
  return true;
}

void initial_setup(){
    // servo setup
  Throttle.attach(servo_pin);  
  Throttle.write(0);

    //Serial Setup
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.setRX(uart2_rx);
  Serial2.setTX(uart2_tx);
  Serial2.begin(115200);
  flash.begin();

    //output states set up
  digitalWrite(low_fuel_pump, LOW);
  digitalWrite(fuel_pin, LOW);
}

void engine_data(){

  Serial2.print("A");
  delay(200);

  if(Serial2.read() == 65){

    for(int i = 0; i <= 74; i++){
      buffer[i] = Serial2.read();
    }
  }

}

//engine starter
void engine_start(){
  digitalWrite(starter_pin, HIGH);
  engine_data();
  int start = buffer[2];
  comms(DASH_ADDRESS, 2, 9, 2);
  while(start == 1){
    digitalWrite(starter_pin, HIGH);
    engine_data();
    delay(20);
    start = buffer[2];
    if(Serial1.available() >= 3){
      check_can();
      if (code == 9 && data[0] == 0) {
      digitalWrite(starter_pin, LOW);
      return;
      }
    }
  }
  digitalWrite(starter_pin, LOW);
  comms(DASH_ADDRESS, 2, 9, 3);
}

#endif
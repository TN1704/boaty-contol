# ifndef FUNCTIONS_H
# define FUNCTIONS_H

void comms(uint16_t address, int code, int data){
  Serial1.write((address >> 8) & 0xff);
  Serial1.write(address & 0xff);
  Serial1.write(2);
  Serial1.write(code);
  Serial1.write(data);
}

void coolant(bool state){
  digitalWrite(coolant_valve_pin, state);
  digitalWrite(coolant_pin, state);
}

void throttle(int value){
// TODO: ensure that if the value is unchanged to to write to servo!
throttle_current = map(value,0,255,0,throttle_max);
if(throttle_current != throttle_previous){
  Throttle.write(throttle_current);
  throttle_previous = throttle_current;
}

}

//Funtion to moving the clutch
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
}


#endif
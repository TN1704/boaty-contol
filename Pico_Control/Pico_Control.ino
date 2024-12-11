// Controller side of the Pico setup address 2

# include <Servo.h>
# include <LittleFS.h>
# include "pins.h"
# include "variables.h"
# include "functions.h"



void setup() {
  // Set up communications
  Serial.begin(9600); // USB Serial
  Serial1.setRX(1);   // Set UART receive pin
  Serial1.setTX(0);   // Set UART Transmit pin
  Serial1.begin(9600); // Baud rate for WS-TTL-CAN

  //set ready to false
  ready = false;

  // Initialize step count
  steps = 0;

  // Setup GPIO pins
  pinMode(forward_switch, INPUT_PULLDOWN);
  pinMode(reverse_switch, INPUT_PULLDOWN);
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(starter_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);

  //Enable stepper
  digitalWrite(enable_pin, LOW);

  // Calibrate the drive position
  forward_state = digitalRead(forward_switch);
  reverse_state = digitalRead(reverse_switch);

  init_drive();
  comms(DASH_ADDRESS, 5, 2);

  while (!ready) {
    if(Serial1.available() >=2){
      code = Serial1.read();
      data = Serial1.read();
    }
    if(code == 5 && data == 2){
      ready = true;
    }
  }
}

void loop() {
  // Main code loop

  forward_state = digitalRead(forward_switch);
  reverse_state = digitalRead(reverse_switch);

  if(Serial1.available() > 2){
    code = Serial1.read();
    data = Serial1.read();
    confirm = false;
  }

  switch(code){

    case 1: //Set forward position
      switch(data){
         case 1:
          while(!forward_state){
            stepMotor(HIGH);
            forward_state = digitalRead(forward_switch);
          }
          data = 0;
          neutral_state = false;
          break;

        case 3: //set reverse position
          while(!reverse_state){
            stepMotor(LOW);
            reverse_state = digitalRead(reverse_switch);
          }
          data = 0;
          neutral_state = false;
          break;

        case 2:{ //set neutral position
          forward_state = digitalRead(forward_switch);
          reverse_state = digitalRead(reverse_switch);
          int current = 0;
          while(forward_state && neutral_pos == current){
            stepMotor(LOW);
            current ++;
          }
          while(reverse_state && neutral_pos == current){
            stepMotor(HIGH);
            current ++;
          }
          if(!forward_state && !reverse_state){
            neutral_state = true;
          }
          data = 0;
          break;}
        
        default:
        break;}
    code = 0;
    break;

    case 5: //start case
      if (data == 1){
        digitalWrite(starter_pin, HIGH);
        start = false;
      }
      else if(data == 0){
        digitalWrite(starter_pin,LOW);
        start = true;
      }
      code = 0;
      break;

      default:
      break;
  }

  //Serial comms to confirm drive position.
  if (!confirm){
    if(forward_state){
      comms(1,1,1);
      confirm = true;
    }
    else if(reverse_state){
      comms(1,2,1);
      confirm = true;
    }
    else if(neutral_state){
      comms(1,3,1);
      confirm = true;
    }
    else if(start){
      comms(1,4,1);
      confirm = true;
    }
  }
}

# ifndef FUNCTIONS_H
# define FUNCTIONS_H

// Function to step the motor
void stepMotor(int dir) {
  digitalWrite(dir_pin, dir);
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(1); // Adjust delay as per your motor specs
  digitalWrite(step_pin, LOW);
  delayMicroseconds(1);
}

// Function for comms
void comms(uint16_t address, int code, int data){
  Serial1.write((address >> 8) & 0xff);
  Serial1.write(address & 0xff);
  Serial1.write(2);
  Serial1.write(code);
  Serial1.write(data);
}

// function for initial position
void init_drive(){
  // Check if neutral
  if (!forward_state && !reverse_state) {
    neutral_state = true;
  }

  while (neutral_state) {
    stepMotor(LOW);
    if (digitalRead(forward_switch) == HIGH) {
      neutral_state = false;
    }
  }

  while (forward_state && !reverse_state) {
    // If in forward state, move to reverse and count steps
    stepMotor(HIGH);
    steps++;
    forward_state = digitalRead(forward_switch);
    reverse_state = digitalRead(reverse_switch);
  }

  while (!forward_state && reverse_state) {
    // If in reverse state, move to forward and count steps
    stepMotor(LOW);
    steps++;
    forward_state = digitalRead(forward_switch);
    reverse_state = digitalRead(reverse_switch);
  }

  neutral_pos = steps / 2; // Find the neutral position
  Serial.print("Calibration complete. Neutral position: ");
  Serial.println(neutral_pos);
}

# endif
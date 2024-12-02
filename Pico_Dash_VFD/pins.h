// file for GPIO pin set up 

#ifndef PINS_H
#define PINS_H

// Outputs
const int drive_led = 10;    // Drive display LED GPIO
const int neutral_led = 11;  // Neutral display LED GPIO
const int reverse_led = 12;  // Reverse display LED GPIO
const int ignition = 13;     // Ignition relay GPIO
const int fuel_led = 14;  //Fuel LED
const int clutch_led = 15;  //clutch LED GPIO
const int low_fuel = 16;  //low fuel warning


//SPI Comm pins
const int spi_tx = 3;  //SPI TX pin GPIO
const int spi_clk = 2;  //SPI RX pin GPIO
const int screen1_cs = 6; // chip select for screen 1
const int screen1_reset = 7;  //reset for screen 1
const int screen2_cs = 8;  //chip seect for screen 2
const int screen2_reset = 9; // reset for screen 2

//UART Comm Pins
const int gps_rx = 1;  //GPS RX pin GPIO
const int gps_tx = 0;  //GPS TX pin GPIO
const int can_rx = 5;  //CAN RX pin GPIO
const int can_tx = 4;  //CAN TX pin GPIO

// Inputs
const int drive_input = 17;  // Drive switch GPIO
const int reverse_input = 18; // Reverse Switch GPIO
const int key_switch = 19;    // Ignition switch GPIO
const int kill_switch = 20;   // Kill switch GPIO
const int start_button = 21;  // Start button GPIO
const int throttle_pot = 26;  // throttle pot GPIO - needs to be ADC pin
const int fuel_input = 22;  //fuel switch GPIO
const int clutch_input = 23;  //clutch switch GPIO
const int reserve = 24;  //reserve tank GPIO

void pin_setup(){

  
  // Initialize output pins
  pinMode(drive_led, OUTPUT);
  pinMode(neutral_led, OUTPUT);
  pinMode(reverse_led, OUTPUT);
  pinMode(ignition, OUTPUT);
  pinMode(fuel_led, OUTPUT);
  pinMode(low_fuel, OUTPUT);

  // Initialize input pins
  pinMode(drive_input, INPUT_PULLUP);
  pinMode(reverse_input, INPUT_PULLUP);
  pinMode(key_switch, INPUT_PULLDOWN);
  pinMode(kill_switch, INPUT_PULLDOWN);
  pinMode(start_button, INPUT_PULLDOWN);
  pinMode(fuel_input, INPUT_PULLUP);
  pinMode(throttle_pot, INPUT);
  pinMode(clutch_input, INPUT_PULLUP);
  pinMode(reserve, INPUT_PULLDOWN);
  
  // Set initial states
  digitalWrite(drive_led, LOW);
  digitalWrite(neutral_led, HIGH); // Start in neutral
  digitalWrite(reverse_led, LOW);
  digitalWrite(ignition, LOW);
}



#endif
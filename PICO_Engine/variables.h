# ifndef VARIABLES_H
# define VARIABLES_H

//Servo Variables
uint16_t fuel_amount;
uint16_t fuel_previous;
const int clutch_max = 180;
const int throttle_max = 180;
int throttle_previous;
int throttle_current;

//Other Variables
bool fuel_on; //variable to check if fuel pump should be on.
bool fuel_conf;  //variable to check if fuel conf sent
bool coolant_on;  //Check if coolant pump is needed
bool is_reserve;  //see if running on reserve.
const int DASH_ADDRESS = 1; // address of the main controller
const int CONTROL_ADDRESS = 2; //address of the peripheral controller
uint32_t time_current;
int time_previous;
int rec_address;
int data[8];
int code;
uint8_t buffer[75];


const int FRAM_SIZE = 8192;  //size of the FRAM storage

# endif
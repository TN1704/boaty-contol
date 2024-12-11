//Contains all the variable to tidy up the main ino file

#ifndef VARIABLES_H
#define VARIABLES_H


// Boolean values
bool isDrive;    // Drive Condition
bool isReverse;  // Reverse Condition
bool isNeutral;  // Neutral Condition
bool isClutch;  //Clutch Condition
bool isFuel;  //Fuel Condition
bool isIgnition;  // Keyswitch Condition
bool isDriveStatus; //check if in drive status
bool isReverseStatus; //check if in reverse status
bool isNeutralStatus; //check if in neutral status
bool isReserve;  //reserve tank condition
bool isKill;  //kill switch condition
bool isOn;  //if start has been pressed and engine should be on.
bool isStart;  //start confirmation
bool fuel;  //fuel on or off
bool fuel_conf;  //fuel confirmation
bool clutch_conf;  //clutch confirmation
bool drive_conf;
bool neutral_conf;
bool reverse_conf;
bool ignition_conf;
bool reserve_conf;



// Variable & Constants
int current_drive;
double speed;
int data[8];
int code;
int throttle;
int throttle_send;
int throttle_send_old;
const int ENGINE_ADDRESS = 3;
const int CONTROLL_ADDRESS = 2;
int fuel_current;
int fuel_previous;
double lastSpeed;
double lastThrottle;
int low_fuel_level = 15;  //threshold for low fuel LED
uint16_t rec_address;

// GPS Values

double lat1, lat2, lon1, lon2;
unsigned long t1, t2;
int hours;
int mins;
int seconds;

#endif
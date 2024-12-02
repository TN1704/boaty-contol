/*
| CAN code | Description                          | Data
|     1    | * Confirmation of drive state        | 1 - Drive
|          |                                      | 2 - Neutral
|          |                                      | 3 - Reverse
|          |                                      |
|     2    | * Fuel pump                          | 1 - On
|          |                                      | 2 - Off
|          |                                      |
|     3    | * Clutch                             | 1 - On (engaged)
|          |                                      | 2 - Off (disengaged)
|          |                                      |
|     4    | * Fuel level                         | Mapped from 0 to 240 to help with the display
|          |                                      |
|     5    | * Start confirmation code            | 1 - sent when the MC has completed setup() Dash to control confirmation to all other MC to allow loop
|          |                                      |
|     6    | * Throttle Position                  | Mapped int from 0 to 255.
|          |                                      |
|     7    | * Fuel reserve                       | 1 - Not reserve
|          |                                      | 2 - Engage reserve system. 


CAN ADDRESSES:
1 - Dash (main controller)
2 - Drive position & other peripherals
3 - Engine controller 




Usage Instructions
Setup:

Connect all hardware components as per the pin mappings in pins.h.
Ensure the CAN Bus transceiver is properly wired to the Pico.
Load all code files onto the Raspberry Pi Pico using the Arduino IDE or another compatible environment.

Operation:

Power on the system.
Ensure the kill switch is deactivated and the ignition switch is ON.
The dashboard will initialize, displaying a welcome screen.
Drive mode and other functionalities will respond to input switches.


Troubleshooting:

If the screens do not display any data, check the SPI connections and screen initialization in config.h.
For communication issues, verify CAN Bus wiring and baud rates.


Future Enhancements
Dynamic Configuration: Adjust thresholds like low_fuel_level via a CAN command.
Additional Inputs: Add support for more vehicle sensors.
Error Handling: Implement advanced error diagnostics for CAN communication.
External EEPROM for Hours run & Logging


Credits
Libraries: U8g2, TinyGPSPlus.
Developed by: Me!!.
Date: On Going!


*/
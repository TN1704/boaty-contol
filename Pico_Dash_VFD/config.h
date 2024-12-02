// used to have the configuration of the system

# ifndef CONFIG_H
# define CONFIG_H

// GPS Config
TinyGPSPlus gps;

// Screen Config
U8G2_GP1287AI_256X50_F_4W_HW_SPI screen1(U8G2_R0, /* cs=*/screen1_cs, /* dc=*/U8X8_PIN_NONE, /* reset=*/screen1_reset);
U8G2_GP1287AI_256X50_F_4W_HW_SPI screen2(U8G2_R0, /* cs=*/screen2_cs, /* dc=*/U8X8_PIN_NONE, /* reset=*/screen2_reset);

#endif
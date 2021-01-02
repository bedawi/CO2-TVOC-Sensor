// Rename this file into config.h and add it in .gitignore to make sure your credentials are not 
// accidentally leaked to Github
#include <cstdint>      // Only needed when using PlatformIO on VSCode

/* ----------------- General config -------------------------------- */
/* MQTT */

/* SEN-CCS811 Sensor */
uint32_t    g_CCS811_coldstart_period   = 1800;          // Seconds to warm up PMS before reading
uint32_t    g_CCS811_warmup_period      = 30;           // Seconds to warm up PMS before reading
uint32_t    g_CCS811_report_period      = 180;          // Seconds between reports

// Needs to be set up only if not one of the preconfigured boards is used:
//uint32_t    g_CCS811_wake_pin           = 2;            

/* BME/BMP280 sensor */
uint8_t     g_bme_i2c_address           = 0x76;          // Address of sensor on I2C bus
uint32_t    g_bme_report_period         = 60;          // Seconds between reports

/* Serial */
#define     SERIAL_BAUD_RATE    115200               // Speed for USB serial console

/* ----------------- Hardware-specific config ---------------------- */

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "SEN-CCS811";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
// !! Security advise: Change this password!
const char wifiInitialApPassword[] =    "thickAir12";

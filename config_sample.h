// -- Configuration file
// !! Security Advise: Copy this file into config.h and add it in .gitignore to make sure your credentials are not accidentally leaked to Github
#include <cstdint>      // Only needed when using PlatformIO on VSCode

/* ----------------- General config -------------------------------- */
/* MQTT */
const char* status_topic                = "events";        // MQTT topic to report startup

/* SEN-CCS811 Sensor */
uint32_t    g_CCS811_coldstart_period   = 180;          // Seconds to warm up PMS before reading
uint32_t    g_CCS811_warmup_period      = 30;           // Seconds to warm up PMS before reading
uint32_t    g_CCS811_report_period      = 180;          // Seconds between reports
uint32_t    g_CCS811_i2cSDA_pin         = 4;            // Not used ...yet
uint32_t    g_CCS811_i2cSCL_pin         = 15;           // Not used ...yet
uint32_t    g_CCS811_wake_pin           = 2;            // Port to which WAKE pin is connected

/* BME/BMP280 sensor */
uint8_t     g_bme_i2c_address           = 0x76;          // Address of sensor on I2C bus

/* Serial */
#define     SERIAL_BAUD_RATE            115200               // Speed for USB serial console

/* ----------------- Hardware-specific config ---------------------- */

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] =                "SEN-CCS811";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
// !! Security advise: Change this password!
const char wifiInitialApPassword[] =    "thickAir12";

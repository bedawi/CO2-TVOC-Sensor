// -- Configuration file
// !! Security Advise: Copy this file into config.h and add it in .gitignore to make sure your credentials are not accidentally leaked to Github
#include <cstdint>      // Only needed when using PlatformIO on VSCode

/* ----------------- General config -------------------------------- */
/* MQTT */
const char* status_topic                = "events";        // MQTT topic to report startup

/* SEN-CCS811 Sensor */
uint32_t    g_CCS811_coldstart_period   = 180;             // Seconds to warm up PMS before reading
uint32_t    g_CCS811_report_period      = 120;             // Seconds between reports

/* Serial */
#define     SERIAL_BAUD_RATE            115200               // Speed for USB serial console

/* ----------------- Hardware-specific config ---------------------- */

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] =                "SEN-CCS811";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
// !! Security advise: Change this password!
const char wifiInitialApPassword[] =    "thickAir12";

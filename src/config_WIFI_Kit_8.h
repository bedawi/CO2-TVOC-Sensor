#include <cstdint>

/* PMS5003 Sensor */
#define     PMS_RX_PIN              10               // Rx from PMS (== PMS Tx)
#define     PMS_TX_PIN              12               // Tx to PMS (== PMS Rx)

// Look up the GPIO PINs for the board here: 
// https://resource.heltec.cn/download/WiFi_Kit_8/WIFI_Kit_8_Pinout_Diagram.pdf
uint32_t g_CCS811_wake_pin = 14;            // Port to which WAKE pin is connected

#include <cstdint>

/* PMS5003 Sensor */
#define     PMS_RX_PIN              22               // Rx from PMS (== PMS Tx)
#define     PMS_TX_PIN              23               // Tx to PMS (== PMS Rx)

uint32_t g_CCS811_wake_pin = 2;            // Port to which WAKE pin is connected 
// -------- GENERAL

#define VERSION "2.2"
#define VERSION_CONFIG 210
#define FORCE_DEFAULT_PROFILE 1
#define CFG_PROFILE_DEFAULT_ID 1
// This gets defined by PlatformIO environment (platformio.ini)
// #define CFG_PROFILE_DEFAULT_NAME "433MHz 4 nodes"

// -------- LORA DEFAULTS

#define LORA_BANDWIDTH 250000 // 250000
#define LORA_CODING_RATE 5 // 5
#define LORA_SPREADING_FACTOR 10 // 9
#define LORA_POWER 20 

// This gets defined by PlatformIO environment (platformio.ini)
//#define LORA_FREQUENCY 433E6 // 433E6, 868E6, 915E6 ------
#define LORA_NODES_MAX 4 // ------
#define LORA_SLOT_SPACING 250

#define LORA_TIMING_DELAY -160
#define LORA_MSP_AFTER_TX_DELAY 150

#define LORA_NAME_LENGTH 3
#define LORA_CYCLE_SCAN 3000 // 3000
#define LORA_PEER_TIMEOUT 6000 // 6s
#define LORA_PEER_TIMEOUT_LOST 120000  // 2 mins
#define LORA_DRIFT_THRESHOLD 8 // Min for action
#define LORA_DRIFT_CORRECTION 12 // Max to correct

// --------- IO AND DISPLAY

#define DISPLAY_CYCLE 250
#define IO_LEDBLINK_DURATION 300
#define IO_LED_PIN 2

#define SERIAL_PIN_TX 23
#define SERIAL_PIN_RX 17
#define SERIAL_SPEED 115200 // 115200 or 38400

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)

// -------- PHASES

#define MODE_START       0
#define MODE_MENU        1
#define MODE_HOST_SCAN   2
#define MODE_LORA_INIT   3
#define MODE_LORA_SYNC   4
#define MODE_LORA_RX     5
#define MODE_LORA_TX     6

// -------- HOST

#define HOST_MSP_TIMEOUT 9000
#define HOST_NONE 0
#define HOST_GCS 1
#define HOST_INAV 2

// -------- STRUCTURE

struct peer_t {
   uint8_t id;
   uint8_t host;
   uint8_t state;
   uint8_t lost;
   uint8_t broadcast;
   uint32_t updated;
   uint32_t lq_updated;
   uint8_t lq_tick;
   uint8_t lq;
   int rssi;
   float distance; // --------- uint16_t
   int16_t direction;
   int16_t relalt;
   msp_raw_gps_t gps;
   msp_raw_gps_t gps_rec;
   msp_raw_gps_t gps_pre;
   uint32_t gps_pre_updated;
   msp_raw_gps_t gps_comp;   
   msp_analog_t fcanalog;
   char name[LORA_NAME_LENGTH + 1];
   };

struct curr_t {
    uint8_t id;
    uint8_t state;
    uint8_t host;
    char name[16];
    uint8_t tick;
    msp_raw_gps_t gps;
    msp_fc_version_t fcversion;
    msp_analog_t fcanalog;
};

struct air_type0_t { // 80 bits
    unsigned int id : 3;
    signed int lat : 25; // -9 000 000 to +9 000 000 (5 decimals)
    signed int lon : 26; // -18 000 000 to +18 000 000 (5 decimals)
    unsigned int alt : 13; // 0 to +8192m
    unsigned int extra_type : 3;
    signed int extra_value : 10;
};

struct config_t {

    // General

    uint16_t version;
    uint8_t profile_id;
    char profile_name[15];

    // Radio

    uint32_t lora_frequency;
    uint32_t lora_bandwidth;
    uint8_t lora_coding_rate;
    uint8_t lora_spreading_factor;
    uint8_t lora_power;

    // Timings

    uint8_t lora_nodes_max;
    uint16_t lora_slot_spacing;
    int16_t lora_timing_delay;
    uint16_t msp_after_tx_delay;

    // IO & Display

    bool display_enable;

};

struct system_t {
    uint8_t phase;

    uint16_t lora_cycle;
    uint8_t lora_tick = 0;

    uint32_t now = 0;
    uint32_t now_sec = 0;
    uint8_t air_last_received_id = 0;
    int last_rssi;

    uint8_t pps = 0;
    uint8_t ppsc = 0;
    uint8_t num_peers = 0;
    uint8_t num_peers_active = 0;

    bool lora_no_tx = 0;
    uint8_t lora_slot = 0;
    uint32_t lora_last_tx = 0;
    uint32_t lora_last_rx = 0;
    uint32_t lora_next_tx = 0;
    int32_t lora_drift = 0;
    int drift_correction = 0;

    uint32_t msp_next_cycle = 0;

    uint8_t display_page = 0;
    bool display_enable = 1;
    uint32_t display_updated = 0;

    uint32_t io_button_released = 0;
    bool io_button_pressed = 0;

    uint16_t cycle_stats;
    uint32_t cycle_scan_begin;

    uint32_t menu_begin;
    uint16_t menu_timeout = 4000;
    uint8_t menu_line = 1;

    uint32_t io_led_changestate;
    uint8_t io_led_count;
    uint8_t io_led_blink;
    uint32_t stats_updated = 0;

    bool io_bt_enabled = 0;

    char message[20];
};

struct stats_t {
    uint32_t timer_begin;
    uint32_t timer_end;

    float packets_total;
    uint32_t packets_received;
    uint8_t percent_received;

    uint16_t last_tx_duration;
    uint16_t last_rx_duration;
    uint16_t last_msp_duration[LORA_NODES_MAX];
    uint16_t last_oled_duration;
};

extern config_t cfg;
extern system_t sys;
extern stats_t stats;
extern curr_t curr;
extern peer_t peers[LORA_NODES_MAX];

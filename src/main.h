//
// ESP32 Radar
// Github : https://github.com/OlivierC-FR/ESP32-INAV-Radar
// RCgroups : https://www.rcgroups.com/forums/showthread.php?3304673-iNav-Radar-ESP32-LoRa-modems
//
// -------------------------------------------------------------------------------------------


// -------- GENERAL

#define VERSION "3.0.2"
#define VERSION_CONFIG 302
#define FORCE_DEFAULT_CONFIG 0
#define CFG_AUTOSTART_BT 0
#define START_DELAY 2500
// #define CFG_TARGET_NAME < (platformio.ini)
// #define CFG_TARGET_FULLNAME < (platformio.ini)

// -------- LORA DEFAULTS

#define LORA_POWER 20
#define LORA_MODE 0
#define LORA_AUTOMODE 0
#define LORA_FORCE_GS 0
// #define LORA_BAND 433 < (platformio.ini)
// #define LORA_FREQUENCY 433375000 < (platformio.ini)
#define LORA_FREQUENCY_433 433375000 // Hz
#define LORA_FREQUENCY_868 868500000 // Hz
#define LORA_FREQUENCY_915 915000000 // Hz
#define LORA_NODES_MAX 6

// --- Mode 0 (Standard)

#define LORA_M0_BANDWIDTH 250000 // Hz
#define LORA_M0_CODING_RATE 5
#define LORA_M0_SPREADING_FACTOR 10
#define LORA_M0_NODES 4
#define LORA_M0_SLOT_SPACING 250 // ms
#define LORA_M0_TIMING_DELAY -150 // ms
#define LORA_M0_MSP_AFTER_TX_DELAY 150 // ms

// --- Mode 1 (Long range)

#define LORA_M1_BANDWIDTH 250000 // Hz
#define LORA_M1_CODING_RATE 5
#define LORA_M1_SPREADING_FACTOR 11
#define LORA_M1_NODES 4
#define LORA_M1_SLOT_SPACING 500 // ms
#define LORA_M1_TIMING_DELAY -300 // ms
#define LORA_M1_MSP_AFTER_TX_DELAY 300 // ms

// --- Mode 2 (Fast)

#define LORA_M2_BANDWIDTH 250000 // Hz
#define LORA_M2_CODING_RATE 5
#define LORA_M2_SPREADING_FACTOR 9
#define LORA_M2_NODES 3
#define LORA_M2_SLOT_SPACING 166 // ms
#define LORA_M2_TIMING_DELAY -75 // ms
#define LORA_M2_MSP_AFTER_TX_DELAY 75 // ms

// --- All modes common

#define LORA_NAME_LENGTH 3
#define LORA_CYCLE_SCAN 5000 // 5s
#define LORA_PEER_TIMEOUT 6000 // 6s
#define LORA_PEER_TIMEOUT_LOST 120000  // 2 mins
#define LORA_DRIFT_THRESHOLD 6 // Min for action
#define LORA_DRIFT_CORRECTION 12 // Max to correct

// --------- IO AND DISPLAY

#define DISPLAY_CYCLE 250
#define IO_LEDBLINK_DURATION 300
#define SERIAL_SPEED 115200

// -------- PHASES

#define MODE_START       0
#define MODE_HOST_SCAN   1
#define MODE_LORA_SCAN   2
#define MODE_LORA_SYNC   3
#define MODE_LORA_RX     4
#define MODE_LORA_TX     5

// -------- HOST

#define HOST_MSP_TIMEOUT 8500
#define HOST_NONE 0
#define HOST_GCS 1
#define HOST_INAV 2

// -------- BEACON

// #define BEACON_TRIGGER_DELAY
// #define BEACON_CYCLE_DELAY

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
   float distance;
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

    uint16_t version;
    uint8_t profile_id;
    char target_name[8];

    uint8_t lora_power;
    uint16_t lora_band;
    uint32_t lora_frequency;

    uint8_t lora_mode;
    bool lora_automode;
    bool force_gs;

    uint32_t lora_bandwidth;
    uint8_t lora_coding_rate;
    uint8_t lora_spreading_factor;
    uint8_t lora_nodes;
    uint16_t lora_slot_spacing;
    int16_t lora_timing_delay;
    int16_t msp_after_tx_delay;

    // IO & Display

    bool display_enable;
};

struct system_t {
    bool debug;
    bool forcereset;
    uint8_t phase;

    uint16_t lora_cycle;
    uint8_t lora_tick = 0;

    uint32_t now = 0;
    uint32_t now_sec = 0;
    uint8_t air_last_received_id = 0;
    int last_rssi;
    float last_snr;
    long last_freqerror;

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

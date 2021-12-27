//
// ESP32 Radar
// Github : https://github.com/OlivierC-FR/ESP32-INAV-Radar
// RCgroups : https://www.rcgroups.com/forums/showthread.php?3304673-iNav-Radar-ESP32-LoRa-modems
//
// -------------------------------------------------------------------------------------------


// -------- Lilygo TTGO v1.0 and 1.3

#ifdef TARGET_LILYGO10
    #define SERIAL_PIN_TX 23
    #define SERIAL_PIN_RX 17
    #define IO_LED_PIN 2
    #define LORA_SCK 5
    #define LORA_MISO 19
    #define LORA_MOSI 27
    #define LORA_CS 18
    #define LORA_RST 14
    #define LORA_IRQ 26
    #define OLED_ADDRESS 0x3c
    #define OLED_SDA 4
    #define OLED_SCL 15
#endif

// -------- Lilygo TTGO v1.4

#ifdef TARGET_LILYGO14
    #define SERIAL_PIN_TX 16
    #define SERIAL_PIN_RX 17
    #define IO_LED_PIN 2
    #define LORA_SCK 18
    #define LORA_MISO 19
    #define LORA_MOSI 23
    #define LORA_CS 5
    #define LORA_RST 4
    #define LORA_IRQ 26
    #define OLED_ADDRESS 0x3c
    #define OLED_SDA 21
    #define OLED_SCL 22
#endif

// -------- Lilygo TTGO v1.6

#ifdef TARGET_LILYGO16
    #define SERIAL_PIN_TX 1
    #define SERIAL_PIN_RX 3
    #define IO_LED_PIN 2
    #define LORA_SCK 5
    #define LORA_MISO 19
    #define LORA_MOSI 27
    #define LORA_CS 18
    #define LORA_RST 23
    #define LORA_IRQ 26
    #define OLED_ADDRESS 0x3c
    #define OLED_SDA 21
    #define OLED_SCL 22
#endif

// -------- Lilygo TTGO v2.0

#ifdef TARGET_LILYGO20
    #define SERIAL_PIN_TX 23
    #define SERIAL_PIN_RX 17
    #define IO_LED_PIN 2
    #define LORA_SCK 14
    #define LORA_MISO 2
    #define LORA_MOSI 27
    #define LORA_CS 13
    #define LORA_RST 12
    #define LORA_IRQ 26
    #define OLED_ADDRESS 0x3c
    #define OLED_SDA 4
    #define OLED_SCL 15
#endif

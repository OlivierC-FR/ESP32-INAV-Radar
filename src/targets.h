// -------- Lilygo TTGO v1.0 and 1.3

#ifdef TARGET_LILYGO10
    #define SERIAL_PIN_TX 23
    #define SERIAL_PIN_RX 17

    #define IO_LED_PIN 2

    #define LORA_SCK 5 // SX1278 SCK
    #define LORA_MISO 19 // SX1278 MISO
    #define LORA_MOSI 27 // SX1278 MOSI
    #define LORA_CS 18 // SX1278 CS
    #define LORA_RST 14 // SX1278 RESET
    #define LORA_IRQ 26 // SX1278 IRQ

    #define OLED_ADDRESS 0x3c // OLED Screen address
    #define OLED_SDA 4 // OLED Screen SDA pin     
    #define OLED_SCL 15 // OLED Screen SCL pin         
#endif

// -------- Lilygo TTGO v1.4

#ifdef TARGET_LILYGO14
    #define SERIAL_PIN_TX 16 // ------------------
    #define SERIAL_PIN_RX 17

    #define IO_LED_PIN 2

    #define LORA_SCK 18 // SX1278 SCK
    #define LORA_MISO 19 // SX1278 MISO
    #define LORA_MOSI 23 // SX1278 MOSI
    #define LORA_CS 5 // SX1278 CS
    #define LORA_RST 4 // SX1278 RESET
    #define LORA_IRQ 26 // SX1278 IRQ

    #define OLED_ADDRESS 0x3c // OLED Screen address
    #define OLED_SDA 21 // OLED Screen SDA pin     
    #define OLED_SCL 22 // OLED Screen SCL pin   
#endif

// -------- Lilygo TTGO v1.6

#ifdef TARGET_LILYGO16
    #define SERIAL_PIN_TX 1
    #define SERIAL_PIN_RX 3

    #define IO_LED_PIN 2

    #define LORA_SCK 5 // SX1278 SCK
    #define LORA_MISO 19 // SX1278 MISO
    #define LORA_MOSI 27 // SX1278 MOSI
    #define LORA_CS 18 // SX1278 CS
    #define LORA_RST 23 // SX1278 RESET
    #define LORA_IRQ 26 // SX1278 IRQ

    #define OLED_ADDRESS 0x3c // OLED Screen address
    #define OLED_SDA 21 // OLED Screen SDA pin     
    #define OLED_SCL 22 // OLED Screen SCL pin   
#endif

// -------- Lilygo TTGO v2.0

#ifdef TARGET_LILYGO20
    #define SERIAL_PIN_TX 23
    #define SERIAL_PIN_RX 17

    #define IO_LED_PIN 2

    #define LORA_SCK 14 // SX1278 SCK
    #define LORA_MISO 2 // SX1278 MISO
    #define LORA_MOSI 27 // SX1278 MOSI
    #define LORA_CS 13 // SX1278 CS
    #define LORA_RST 12 // SX1278 RESET
    #define LORA_IRQ 26 // SX1278 IRQ

    #define OLED_ADDRESS 0x3c // OLED Screen address
    #define OLED_SDA 4 // OLED Screen SDA pin     
    #define OLED_SCL 15 // OLED Screen SCL pin   
#endif

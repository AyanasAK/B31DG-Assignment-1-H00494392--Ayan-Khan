#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
// #include "esp_rom_sys.h"  // ✅ Correct header for esp_rom_delay_us()
#include "esp_log.h"

#define DATA_PIN 25
#define SYNC_PIN 26
#define ENABLE_BTN 18
#define SELECT_BTN 17

#define SYNC_TIME 50  // SYNC pulse duration in microseconds

static const char *TAG = "ESP32_SIGNAL";

int KHAN[] = {3, 8, 1, 6};
int a, b, c, d;

bool signalEnbl = false;
bool altMode = false;

void normWave();
void altWave();
void syncTrig();
void buttonTask(void *arg);
void waveformTask(void *arg);

void app_main() {
    // Initialize runtime variables
    a = KHAN[0] * 100;
    b = KHAN[1] * 100;
    c = KHAN[2] + 4;
    d = KHAN[3] * 500;

    // Configure GPIOs
    gpio_reset_pin(DATA_PIN);
    gpio_reset_pin(SYNC_PIN);
    gpio_reset_pin(ENABLE_BTN);
    gpio_reset_pin(SELECT_BTN);

    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SYNC_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ENABLE_BTN, GPIO_MODE_INPUT);
    gpio_set_direction(SELECT_BTN, GPIO_MODE_INPUT);

    gpio_set_pull_mode(ENABLE_BTN, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(SELECT_BTN, GPIO_PULLDOWN_ONLY);

    // Create tasks for button handling and waveform generation
    xTaskCreate(buttonTask, "Button Task", 2048, NULL, 1, NULL);
    xTaskCreate(waveformTask, "Waveform Task", 2048, NULL, 1, NULL);
}

void buttonTask(void *arg) {
    while (1) {
        if (gpio_get_level(ENABLE_BTN) == 1) {
            signalEnbl = !signalEnbl;
            ESP_LOGI(TAG, "Signal A State changed: %d", signalEnbl);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        if (gpio_get_level(SELECT_BTN) == 1) {
            altMode = !altMode;
            ESP_LOGI(TAG, "Signal B State changed: %d", altMode);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void waveformTask(void *arg) {
    while (1) {
        if (signalEnbl) {
            if (altMode) {
                altWave();
            } else {
                normWave();
            }
        } else {
            gpio_set_level(DATA_PIN, 0);
            gpio_set_level(SYNC_PIN, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void normWave() {
    syncTrig();
    gpio_set_level(DATA_PIN, 1);
    esp_rom_delay_us(a);
    gpio_set_level(DATA_PIN, 0);
    esp_rom_delay_us(b);

    for (int n = 2; n <= c; n++) {
        long Ton = a + ((n - 1) * 50);
        gpio_set_level(DATA_PIN, 1);
        esp_rom_delay_us(Ton);
        gpio_set_level(DATA_PIN, 0);
        esp_rom_delay_us(b);
    }

    esp_rom_delay_us(d);
}

void altWave() {
    syncTrig();
    esp_rom_delay_us(d);

    for (int n = c; n >= 1; n--) {
        long Ton = a + ((n - 1) * 50);
        gpio_set_level(DATA_PIN, 1);
        esp_rom_delay_us(Ton);
        gpio_set_level(DATA_PIN, 0);
        esp_rom_delay_us(b);
    }
}

void syncTrig() {
    gpio_set_level(SYNC_PIN, 1);
    esp_rom_delay_us(SYNC_TIME);
    gpio_set_level(SYNC_PIN, 0);
}

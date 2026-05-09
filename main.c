#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN   16
#define LED_FREQ     800000
#define TOUCH_PIN    26
#define DEBOUNCE_MS  400

// Capacitive touch via cycle counting.
// Returns number of loop iterations until pin discharges LOW.
// Untouched: ~5-20 counts. Finger adds capacitance → 5-50x more counts.
static uint32_t capacitive_read(void) {
    gpio_set_dir(TOUCH_PIN, GPIO_OUT);
    gpio_put(TOUCH_PIN, 1);
    sleep_us(10);

    gpio_set_dir(TOUCH_PIN, GPIO_IN);
    gpio_pull_down(TOUCH_PIN);

    uint32_t count = 0;
    while (gpio_get(TOUCH_PIN)) {
        if (++count > 50000) break;
    }
    return count;
}

static inline void put_pixel(uint32_t grb) {
    pio_sm_put_blocking(pio0, 0, grb << 8u);
}

static const uint32_t COLORS[] = {
    0x500000,   // Red
    0x005000,   // Green
    0x000050,   // Blue
    0x005050,   // Cyan
    0x500050,   // Magenta
    0x505000,   // Yellow
    0x303030,   // White
};
#define NUM_COLORS (sizeof(COLORS) / sizeof(COLORS[0]))

int main() {

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, LED_FREQ, false);

    gpio_init(TOUCH_PIN);
    sleep_ms(200);

    // Baseline: average of 32 untouched readings
    uint32_t baseline = 0;
    for (int i = 0; i < 32; i++) {
        baseline += capacitive_read();
        sleep_ms(5);
    }
    baseline /= 32;
    // Touch threshold = 3x baseline discharge time
    uint32_t threshold = baseline * 3;

    uint8_t color_idx = 0;
    bool was_touched = false;
    absolute_time_t last_touch = nil_time;

    put_pixel(COLORS[color_idx]);

    while (true) {
        uint32_t t = capacitive_read();
        bool touched = (t > threshold);

        if (touched && !was_touched &&
            absolute_time_diff_us(last_touch, get_absolute_time()) > DEBOUNCE_MS * 1000) {
            color_idx = (color_idx + 1) % NUM_COLORS;
            put_pixel(COLORS[color_idx]);
            last_touch = get_absolute_time();
        }

        was_touched = touched;
        sleep_ms(10);
    }
}

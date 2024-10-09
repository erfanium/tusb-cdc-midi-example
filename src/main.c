#include <stdio.h>

#include "bsp/board.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "tusb.h"

enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

int main() {
  board_init();

  // tud_init(0);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  // stdio_usb_init();

  while (true) {
    tud_task();
    // stdio_put_string("Hello, Serial!", -1, true, false);
    led_blinking_task();
  }
}

void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if (board_millis() - start_ms < blink_interval_ms) return;  // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;  // toggle
  stdio_put_string("Hello, Serial!", -1, true, false);
}
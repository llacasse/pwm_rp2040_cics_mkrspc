// Author:      Luke LaCasse
// Date:        April 18, 2024
// Title:       Implement PWM using RP2040 SDK and RP2040 Microcontroller in Arduino IDE
// Description: Implement PWM on RP2040 in the Arduino IDE using the provided functions in the RP2040 SDK. This sketch is intended
//              to be used with a discrete indicator LED (built-in or external).

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_PIN LED_BUILTIN  // Pin Mapping for LED
#define PWM_MAX 255          // Maximum value for PWM, value set by Arduino

uint slice_num;              // PWM Slice Information

uint8_t brightness;          // Current PWM output value, Sets brightness of LED
bool    count_up;            // Determines count direction


void setup() {
  gpio_set_function(LED_PIN, GPIO_FUNC_PWM);     // Enable PWM on the LED Pin

  slice_num = pwm_gpio_to_slice_num(LED_PIN);    // Determine which PWM hardware slice the desired GPIO pin is on
                                                 // Note: A "pwm slice" refers to the pwm control hardware block 
                                                 // connected to a particular GPIO pin. See RP2040 SDK for more info.

  pwm_set_wrap(slice_num, PWM_MAX);              // Set the maximum pwm value, determines resolution of pwm output
  pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);  // Set the PWM counter compare register with an initial value. Used to determine output duty cycle.
  pwm_set_enabled(slice_num, true);              // Enable PWM on the correct pwm slice

  // Set initial values for PWM Output, LED OFF (PWM = 0), Counting Up
  count_up = true;
  brightness = 0;
  pwm_set_gpio_level(LED_PIN, brightness);       // Set PWM Output Value
}

void loop() {
  // Determine count direction based on current PWM output value
  if(count_up){
    if(brightness < PWM_MAX){
      brightness++;
    }else{
      count_up = false;
    }
  }else{
    if(brightness){
      brightness--;
    }else{
      count_up = true;
    }
  }

  // Set PWM value on LED pin
  pwm_set_gpio_level(LED_PIN, brightness);
  delay(5);
}

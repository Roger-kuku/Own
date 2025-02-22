#include <iostream>
#include <gpiod.h>
#include <chrono>
#include <thread>

#define CHIP_NAME "gpiochip4"  // Change based on `gpiodetect`
#define OUTPUT_PIN 22          // GPIO pin generating square wave
#define INPUT_PIN  17          // GPIO pin measuring the waveform
#define FREQUENCY 1000         // Square wave frequency in Hz

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *out_line, *in_line;
    int period_us = 1000000 / FREQUENCY;  // Convert Hz to microseconds

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return 1;
    }

    // Get output GPIO line
    out_line = gpiod_chip_get_line(chip, OUTPUT_PIN);
    if (!out_line) {
        std::cerr << "Failed to get output GPIO line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Get input GPIO line
    in_line = gpiod_chip_get_line(chip, INPUT_PIN);
    if (!in_line) {
        std::cerr << "Failed to get input GPIO line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Set output pin as OUTPUT
    if (gpiod_line_request_output(out_line, "oscillator", 0) < 0) {
        std::cerr << "Failed to set output GPIO\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Set input pin as INPUT
    if (gpiod_line_request_input(in_line, "measurer") < 0) {
        std::cerr << "Failed to set input GPIO\n";
        gpiod_chip_close(chip);
        return 1;
    }

    std::cout << "Generating and measuring square wave at " << FREQUENCY << " Hz\n";

    // Generate square wave and measure it
    while (true) {
        // Generate HIGH
        gpiod_line_set_value(out_line, 1);
        auto start = std::chrono::high_resolution_clock::now();

        std::this_thread::sleep_for(std::chrono::microseconds(period_us / 2));

        // Generate LOW
        gpiod_line_set_value(out_line, 0);
        auto end = std::chrono::high_resolution_clock::now();

        std::this_thread::sleep_for(std::chrono::microseconds(period_us / 2));

        // Read GPIO input state
        int input_value = gpiod_line_get_value(in_line);
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "Input pin state: " << input_value 
                  << ", Pulse duration: " << duration << " us\n";
    }

    // Cleanup (unreachable in infinite loop)
    gpiod_line_release(out_line);
    gpiod_line_release(in_line);
    gpiod_chip_close(chip);
    return 0;
}

#include <iostream>
#include <gpiod.h>
#include <chrono>
#include <thread>

#define CHIP_NAME "gpiochip4"  // Change based on your GPIO chip (check with `gpiodetect`)
#define GPIO_PIN 17            // GPIO Pin number
#define FREQUENCY 1000         // Frequency in Hz (adjust as needed)

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int period_us = 1000000 / FREQUENCY;  // Convert Hz to microseconds

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return 1;
    }

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!line) {
        std::cerr << "Failed to get GPIO line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Set the line as output
    if (gpiod_line_request_output(line, "oscillator", 0) < 0) {
        std::cerr << "Failed to set GPIO as output\n";
        gpiod_chip_close(chip);
        return 1;
    }

    std::cout << "Generating square wave on GPIO " << GPIO_PIN << " at " << FREQUENCY << " Hz\n";

    // Generate the square wave
    while (true) {
        gpiod_line_set_value(line, 1);  // Set HIGH
        std::this_thread::sleep_for(std::chrono::microseconds(period_us / 2));

        gpiod_line_set_value(line, 0);  // Set LOW
        std::this_thread::sleep_for(std::chrono::microseconds(period_us / 2));
    }

    // Cleanup (unreachable in infinite loop)
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}


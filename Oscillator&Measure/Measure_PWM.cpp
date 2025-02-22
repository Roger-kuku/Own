#include <iostream>
#include <gpiod.h>
#include <chrono>
#include <thread>

#define CHIP_NAME "gpiochip4"  // Change based on `gpiodetect`
#define INPUT_PIN  17          // GPIO pin measuring the waveform
#define FREQUENCY 1000         // PWM frequency in Hz

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *in_line;

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return 1;
    }

    // Get input GPIO line
    in_line = gpiod_chip_get_line(chip, INPUT_PIN);
    if (!in_line) {
        std::cerr << "Failed to get input GPIO line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Set input pin as INPUT
    if (gpiod_line_request_input(in_line, "pwm_measurer") < 0) {
        std::cerr << "Failed to set input GPIO\n";
        gpiod_chip_close(chip);
        return 1;
    }

    std::cout << "Measuring PWM signal at " << FREQUENCY << " Hz\n";

    auto last_measurement_time = std::chrono::high_resolution_clock::now();

    // Measure PWM signal and calculate duty cycle
    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - last_measurement_time;

        // Faster measurement every 100 ms
        if (elapsed.count() >= 0.1) {
            auto start = std::chrono::high_resolution_clock::now();

            // Wait until the signal goes HIGH
            while (gpiod_line_get_value(in_line) == 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));  // Reduced sleep for faster response
            }
            auto high_start = std::chrono::high_resolution_clock::now();

            // Measure the high duration
            while (gpiod_line_get_value(in_line) == 1) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto high_end = std::chrono::high_resolution_clock::now();

            // Measure the low duration
            auto low_start = std::chrono::high_resolution_clock::now();
            while (gpiod_line_get_value(in_line) == 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto low_end = std::chrono::high_resolution_clock::now();

            // Calculate high and low times in microseconds
            auto high_duration = std::chrono::duration_cast<std::chrono::microseconds>(high_end - high_start).count();
            auto low_duration = std::chrono::duration_cast<std::chrono::microseconds>(low_end - low_start).count();
            auto total_duration = high_duration + low_duration;

            // Calculate duty cycle
            double duty_cycle = (double)high_duration / total_duration * 100.0;

            std::cout << "High duration: " << high_duration << " us, "
                      << "Low duration: " << low_duration << " us, "
                      << "PWM Duty Cycle: " << duty_cycle << "%\n";

            // Update the last measurement time
            last_measurement_time = current_time;
        }

        // Reduce CPU load with small sleep between measurements
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }

    // Cleanup (unreachable in infinite loop)
    gpiod_line_release(in_line);
    gpiod_chip_close(chip);
    return 0;
}


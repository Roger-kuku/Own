Gpiod library 

Install in Raspberry OS (first time only) 

>> sudo apt-get install libgpiod2 libgpiod-dev 

 

Compile and build the file from C++ 

>> g++ -o (File_Name) (File_Name).cpp -lgpiod 

 

Running a file 

>> ./(File_name) or sudo ./(File_name) 

 

This is the library for GPIO 

>> #include <gpiod.h> 

User interface to see the message and print words 

>> #include <iostream> 

>> using namespace std; 

Time Management (use for delay, timer, benchmarking) 

>> #include <chrono> 

Multithreading and Delays 

>> #include <thread> 

 

 

Define the chip and pin 

>> #define GPIO_CHIP "/dev/gpiochip0"  // Default GPIO chip on Raspberry Pi 

>> #define MOTOR_PIN 4                           // GPIO pin number (not BCM) 

 

// Function to generate software PWM 

>> void software_pwm(gpiod_line *line, int duty_cycle, int period_ms, int duration_ms) { 

    auto start_time = std::chrono::steady_clock::now(); 

     

    while (std::chrono::steady_clock::now() - start_time < std::chrono::milliseconds(duration_ms)) { 

        // Turn ON the motor (HIGH) 

        gpiod_line_set_value(line, 1); 

        std::this_thread::sleep_for(std::chrono::milliseconds(duty_cycle * period_ms / 100)); 

  

        // Turn OFF the motor (LOW) 

        gpiod_line_set_value(line, 0); 

        std::this_thread::sleep_for(std::chrono::milliseconds((100 - duty_cycle) * period_ms / 100)); 

    } 

} 

 

WiringPi library 

Error Command for Calling wiringPi or building 

>> g++ -Wall –o "(File_Name)" "(File_Name).cpp" -lwiringPi 

For library <gpiod.h> 

>> g++ -o (File_Name) (File_Name).cpp -lgpiod 

 

 

Running a file 

>> ./(File_name) or sudo ./(File_name) 

 

This is the library for GPIO 

>> #include <wiringPi.h> 

User interface to see the message and print words 

>> #include <iostream> 

>> using namespace std; 

 

Set the GPIO pin 

 >> const int (Device_Name) = (GPIO.No); 

 

Inside the looping 

>> int main(int argc, char  *argv[]) 

 { 

// Initializing wiringPi 

If (wiringPiSetup() == -1) 

{ 

cout << "WiringPi setup failed!" <<endl; 

return –1; 

} 

 

 

pinMode(Device_name, OUTPUT or INPUT); 

cout << "…" << endl; 

 

while (true) 

{ 

digitalWrite(Device_Name, HIGH or LOW); 

} 

 

return 0; 

} 

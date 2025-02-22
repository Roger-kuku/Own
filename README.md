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

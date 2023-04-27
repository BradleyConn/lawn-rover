# lawn-rover-program

![Mower](/images/mower1.jpg "Mower")

Part of the lawn rover project. See [www.bradleyconn.space](https://www.bradleyconn.space/) for more details.
This is the main controller to interface with the remote reciever, interpret the signals, and interface to the motor drivers to drive the wheels.

An esp32 was used as I already had one readily available. Most microcontrollers would work. The Arduino IDE was used for its ease of use and effectiveness.

Everything was kept in a single file for simplicity, however that file has grown quite large and separate files should probably be considered.

## ESP32 wiring diagram
Below is how the ESP32 was connected to the R6B Receiver and connectors that attached to the wheel motor driver boards.

![Wiring Diagram](/diagrams/rover_fritzing.png "Wiring Diagram")

## System block diagram

A block diagram of the entire system can be seen below. The microcontroller which this repo is concerned with is shown at the bottom.

![System Block Diagram](/diagrams/rover_block_diagram.png "System Block Diagram")

And the actual wiring inside the system can be seen below.


![Mower Inside](/images/mower4.jpg "Mower Inside")

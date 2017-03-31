# Sample Motor Control Library

Defines the general structure for our motor control and encoder libraries.

On the rover, `main.c` would be the file containing the ROS callbacks and control loops.

`bdc_motor.c` and `bdc_motor.h` would need to be shared between multiple `main` files.

Instead of defining the `motor1` pins in `main.c`, ideally we'd have a separate pin configuration header.

We should probably also put all the general Tivaware includes in their own file

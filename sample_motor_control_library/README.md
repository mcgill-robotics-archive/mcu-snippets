# Sample Motor Control Library

Defines the general structure for our motor control and encoder libraries.

On the rover, `main.c` would be the file containing the ROS callbacks and control loops.

## Notes:

- `bdc_motor.c` and `bdc_motor.h` would need to be shared between multiple `main` files for different Tivas.

- Instead of defining the `motor1` pins in `main.c`, ideally we'd have a separate pin configuration header.

- We should probably also put all the general Tivaware includes in their own file

## TODO:

- Is it safe to enable a peripheral multiple times? e.g. `SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);`

- Set PWM resolution once and for all, then define input range for `bdc_set_velocity()`. Make the range the same for brushless.

- Change ADC sample depth / configure it correctly for current sensing. The current ADC code is just a single snapshot

- Implement `bdc_set_veloity()`

- Implement `bdc_get_current()`

- Clean up standard Tiva includes

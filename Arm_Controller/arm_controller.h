#ifndef ARM_CONTROLLER
#define ARM_CONTROLLER

/*
 * Global includes,  definitions and functions.
 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_qei.h"
#include "inc/hw_ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/adc.h"


/*******************Macro Defines*******************/
/*
 * Proportional Gain for position loop
 */
#define POS_KP 1

/*
 * Integral gain for position loop
 */
#define POS_KI 1

/*
 * Proportional gain for velocity loop
 */
#define VEL_KP 1

/*
 * Integral gain for velocity loop
 */
#define VEL_KI 1

/*
 * Proportional gain for torque loop
 */
#define TORQUE_KP 1

/*
 * Integral Gain for torque loop
 */
#define TORQUE_KI 1

/*
 * Period of position loop
 */
#define POS_FREQ 5

/*
 * Period of velocity loop
 */
#define VEL_FREQ 10

/*
 * Period of torque loop
 */
#define TORQUE_FREQ 20



/*******************Global Variables*******************/

/*
 * Direction of motor rotation
 */
volatile int32_t ui32QEIDirection;

/*
 * Position given by encoder
 */
volatile uint32_t ui32QEIPosition;

/*
 * System clock frequency
 */
volatile uint32_t ui32SysClkFreq;

/*
 * velocity returned by encoder
 */
volatile uint32_t ui32QEIVelocity;

/*
 * Initial Position read by encoder
 */
volatile uint32_t ui32QEIPositionInitial;

/*
 * Output of Position loop, input of velocit loop
 */
volatile uint32_t vel_input;

/*
 * Output of Velocity loop, input of torque loop
 */
volatile uint32_t torque_input;

/*
 * Output of Torque loop, input for motor_run
 */
volatile uint32_t motor_input;

/*
 * Buffer for reading current sensor ouput
 */
uint32_t pui32ADC0Value[1];

/*
 *TODO: figure out what this is
 */
volatile uint32_t toFileADCRead;

/*
 * Pointer to FILE to record states
 */
FILE* fp;


/*******************Function Declarations*******************/
/*
 * Position PI loop
 */
void pos_PI(uint32_t theta, uint32_t set_theta);
/*
 * Velocity PI loop
 */
void vel_PI(uint32_t vel, uint32_t set_vel);
/*
 * Torque PI loop
 */
void torque_PI(uint32_t current, uint32_t set_torque);

/*
 * Position Loop Test
 */
void pos_loop_test();

/*
 * Velocity Loop Test
 */
void vel_loop_test();

/*
 * Torque Loop Test
 */
void torque_loop_test();

/*
 *Timer for Torque Loop Interrupt
 */
void TimerIntHandler_Torque(void);

/*
 * Timer for Velocity Loop Interrupt
 */
void TimerIntHandler_Velocity(void);

/*
 * Timer to Position Loop Interrupt
 */
void TimerIntHandler_Position(void);

/*
 * Position Loop Interrupt Configuration
 */
void configure_pos_timer(void);

/*
 * Velocity Loop Interrupt Configuration
 */
void configure_vel_timer(void);

/*
 * Torque Loop Interrupt Configuration
 */
void configure_torque_timer(void);


/*
 * Encoder Configuration
 */
void configure_qei(void);

/*
 * GPIO Configuration
 */
void configure_gpio(void);

/*
 * PWM and INA and INB Configuration for motor
 */
void configure_motor(void);

/*
 * uDMA Configuration
 */
void configure_uDMA(void);

/*
 * ADC Configuration
 */
void configure_ADC(void);
/*
 * PWM motor function
 */
void motor_run(int direction, double percentage);

/*
 * Returns current sensor reading
 */
uint32_t getCurrent(void);

/*
 * Returns encoder position reading
 */
uint32_t getPosition(void);

/*
 * Returns encoder velocity reading
 */
uint32_t getVelocity(void);

/*
 * Returns encoder direction reading
 */
uint32_t getDirection(void);

uint32_t getSetPosition();
/*
 *TODO: figure out what this does
 */
void position_change_to(double degree, int direction, double speedPercentage);

#endif

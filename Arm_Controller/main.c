/*
 * PIV Control
 */

/* HOW/WHAT/WHY:
 * Torque loop is the fastest,
 * Velocity loop is second fastest
 * Position loop is the slowest
 * Timer interrupt handler of torque loop
 * calls the motor run
 */

#include "arm_controller.h"

/*
 * Position PI loop
 */
void pos_PI(uint32_t theta, uint32_t set_theta){
	double sum = 0;
	double sum_sat = 200;
	double integral = 0;
	double error = 0;
	//integration
	error = theta - set_theta;
	integral = integral + error;
	vel_input = POS_KP*error + POS_KI*integral;
}
/*
 * Velocity PI loop
 */
void vel_PI(uint32_t vel, uint32_t set_vel){
	double sum = 0;
	double sum_sat = 200;
	double integral = 0;
	double error = 0;
	//integration
	error = vel - set_vel;
	integral = integral + error;
	torque_input = VEL_KP*error + VEL_KI*integral;
}
/*
 * Torque PI loop
 */
void torque_PI(uint32_t current, uint32_t set_torque){
	double sum = 0;
	double sum_sat = 200;
	double integral = 0;
	double error = 0;
	//integration
	error = current - set_torque;
	integral = integral + error;
	motor_input = TORQUE_KP*error + TORQUE_KI*integral;
}

/*
 *Timer for Torque Loop Interrupt
 */
void TimerIntHandler_Torque(void){
	//get current measurement from current sensor
	uint32_t current = getCurrent(); //fictional function, need to write
	torque_PI(current, torque_input);
}

/*
 * Timer for Velocity Loop Interrupt
 */
void TimerIntHandler_Velocity(void){
	//get velocity measurement
	uint32_t velocity = getVelocity();
	vel_PI(velocity, vel_input);
}

/*
 * Timer to Position Loop Interrupt
 */
void TimerIntHandler_Position(void){
	//get position measurement
	uint32_t pos = getPosition();
	//get set position from path generator
	uint32_t set_pos = getSetPosition();
	pos_PI(pos, set_pos);
}




/*
 * main.c
 */
	int main(void) {
	//Set the system clock to 40MHz. 16MHz Main -> 400MHz PLL -> divide by 10
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	configure_gpio();
	configure_motor();
	configure_qei();
	int i;
	motor_run(-1, 50.00);
	
	return 0;
}

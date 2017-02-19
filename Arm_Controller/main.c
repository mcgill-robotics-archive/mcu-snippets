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
uint8_t ui8LED = 2;
/*
 * Position PI loop
 */
void pos_PI(){
	pos_flag = 0;
	//get position measurement
	uint32_t theta = getPosition();
	//get set position from path generator
	uint32_t set_theta = getSetPosition();

	double sum_sat = 200;
	double integral = 0;
	double error = 0;

	//integration
	error = theta - set_theta;
	integral = (integral < sum_sat) ? integral + error : 0;
	vel_input = POS_KP*error + POS_KI*integral;
}
/*
 * Velocity PI loop
 */
void vel_PI(){
	vel_flag = 0;

	double sum_sat = 200;
	double integral = 0;
	double error = 0;

	//get velocity measurement
	uint32_t vel = getVelocity();
	uint32_t set_vel = vel_input;

	//integration
	error = vel - set_vel;
	integral = (integral < sum_sat) ? integral + error : 0;
	torque_input = VEL_KP*error + VEL_KI*integral;
}
/*
 * Torque PI loop
 */
void torque_PI(){

	torque_flag = 0;

	uint32_t current = getCurrent();
	uint32_t set_torque = torque_input;

	double sum_sat = 200;
	double integral = 0;
	double error = 0;

	//integration
	error = current - set_torque;
	integral = (integral < sum_sat) ? integral + error : 0;
	motor_input = TORQUE_KP*error + TORQUE_KI*integral;
}

/*
 *Timer for Torque Loop Interrupt
 */
void TimerIntHandler_Torque(void){
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	torque_flag = 1;

}

/*
 * Timer for Velocity Loop Interrupt
 */
void TimerIntHandler_Velocity(void){
	// Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	//set vel_loop flag
	vel_flag = 1;
}

/*
 * Timer to Position Loop Interrupt
 */
void TimerIntHandler_Position(void){
	// Clear the timer interrupt
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	pos_flag = 1;
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
}



/*
 * main.c
 */

int main(void) {

	//set PI loop flags to 0
	pos_flag = 0;
	vel_flag = 0;
	torque_flag = 0;

	//Set the system clock to 40MHz. 16MHz Main -> 400MHz PLL -> divide by 10
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	configure_gpio();
	configure_motor();
	configure_qei();
	configure_ADC();

//	uint32_t current = getCurrent();
//	uint32_t currentconstant= 1;
//	uint32_t velocity = getVelocity();
//	uint32_t velocityconstant = 20;
//	double speed= 0;
//	int i= 0;

//	/*
//	 * constant velocity test
//	 */
//	while(velocity != velocityconstant ){
//		motor_run(-1, speed);
//		speed++;
//	}
//	while (i<2000){
//		motor_run(-1, speed);
//		i++;
//	}
//	motor_stop();
//
//
//	/*
//	 * constant current test
//	 */
//
//
//		while(current != currentconstant ){
//			motor_run(-1, speed);
//			speed++;
//		}
//		while (i<2000){
//			motor_run(-1, speed);
//			i++;
//		}
//		motor_stop();

	/*
	 * Loop testing
	 */

	configure_torque_timer();
	configure_vel_timer();
	configure_pos_timer();


	while(true){
		motor_input = 50;
		int dir = (motor_input > 0) ? 1 : -1;
		motor_run(dir, motor_input);

		if (torque_flag == 1){
			torque_PI();
		}
		else if (vel_flag == 1){
			vel_PI();
		}
		else if (pos_flag == 1){
			pos_PI();
		}
	}

}

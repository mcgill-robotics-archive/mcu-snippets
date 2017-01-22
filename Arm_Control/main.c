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

volatile int32_t ui32QEIDirection;
volatile uint32_t ui32QEIPosition;
volatile uint32_t ui32SysClkFreq;
volatile uint32_t ui32QEIVelocity;
volatile uint32_t ui32QEIPositionInitial;
#define POS_KP 1
#define POS_KI 1
#define VEL_KP 1
#define VEL_KI 1
#define TORQUE_KP 1
#define TORQUE_KI 1
#define POS_FREQ 1
#define VEL_FREQ 1
#define TORQUE_FREQ 1

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

/*Global Variables*/
volatile uint32_t vel_input;
volatile uint32_t torque_input;
volatile uint32_t motor_input;
volatile uint32_t pui32ADC0Value[1];
volatile uint32_t toFileADCRead;

FILE* fp;

uint32_t increment = 0;

uint32_t getSetPosition(){
	return increment+=50;
}


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

void pos_loop_test(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	uint32_t pos_period = (SysCtlClockGet() * 2000000) * 4; // 20 Hz
	TimerLoadSet(TIMER2_BASE, TIMER_A, pos_period -1);
	IntEnable(INT_TIMER2A);
	TimerEnable(TIMER2_BASE, TIMER_A);

	uint32_t ref_pos = 20;
	uint32_t increment = 10;
	uint32_t current_pos = getPosition(); //fictional function from the QEI module
	fp = fopen("/test_files/pos_test.csv", "w+");
	pos_PI(current_pos, ref_pos);
	ref_pos += increment;
	current_pos = getPosition();
	fprintf(fp, "%d, %d\n", ref_pos, current_pos);
}

/*
 * main.c
 */
int main(void) {

//	/*    Set the clock to run at 120MHz.*/
//		ui32SysClkFreq= SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	//Set the system clock to 40MHz. 16MHz Main -> 400MHz PLL -> divide by 10
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	configure_pins();

	//Enables the processor to respond to interrupts
	IntMasterEnable();

	//Set the system clock to 40MHz. 16MHz Main -> 400MHz PLL -> divide by 10
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//
	//Enable GPIO clock
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//Enables peripherals to operate and respond to read/writes
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	//Configure Timer 0 to be a concatenated 32-bit periodic timer
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

	//compute the period of interrupts generated by Timer0 in terms of system clock cycles
	uint32_t torque_period = (SysCtlClockGet() * 2000000); // 20 Hz
	uint32_t vel_period = torque_period * 2;
	uint32_t pos_period = vel_period * 2;

	//Set the interrupt period-1 as the max count for the timer
	TimerLoadSet(TIMER0_BASE, TIMER_A, torque_period -1);
	TimerLoadSet(TIMER1_BASE, TIMER_A, vel_period -1);
	TimerLoadSet(TIMER2_BASE, TIMER_A, pos_period -1);

	//enables TIMER0 to generate interrupts in the NVIC
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER1A);
	IntEnable(INT_TIMER2A);

	//sets TIMER0 to generate an interrupt when it times out
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	//Enables the processor to respond to interrupts
	IntMasterEnable();

	//Start the timers
	TimerEnable(TIMER2_BASE, TIMER_A);
	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerEnable(TIMER0_BASE, TIMER_A);

	return 0;
}

//torque loop
void Timer0IntHandler(void){
	//get current measurement from current sensor
	uint32_t current = getCurrent(); //fictional function, need to write
	torque_PI(current, torque_input);
	//drive the motor with the output value
	//call funcion
}

//velocity loop
void Timer1IntHandler(void){
	//get velocity measurement
	uint32_t velocity = getVelocity();
	vel_PI(velocity, vel_input);
}

//position loop
void Timer2IntHandler(void){
	//get position measurement
	uint32_t pos = getPosition();
	//get set position from path generator
	uint32_t set_pos = getSetPosition();
	pos_PI(pos, set_pos);
}

/*
 * Initialize all required pins
 * TODO: Breakup to smaller function calls
 */
void configure_pins(void){
/*    Enable peripherals.*/
	// Enable the GPIO peripheral base D and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}

	/*	Unlock pins on peripheral D*/
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; //In Tiva include this is the same as "_DD" in older versions (0x4C4F434B)
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

	// Enable the GPIO peripheral base F and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}

	// The Quadrature Encoder Interface peripheral enabled for use and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0)){}

	// The ADC0 peripheral must be enabled for use and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}


	//
	// For this example ADC0 is used with AIN0 on port E7.
	// The actual port and pins used may be different on your part, consult
	// the data sheet for more information.  GPIO port E needs to be enabled
	// so these pins can be used.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

	// MOTOR CONTROLLER CONFIG

	/* Set PWM clock base frequency.
	 * PWM base frequency is the processor clock frequency divided by X, where
	 * the input to SysCtlPWMClockSet is SYSCTL_PWMDIV_X.
	 * If you're using the external oscillator (default), the processor clock
	 * will be 80MHz. In this case, the divisor is 8, so the frequency base is
	 * 10MHz. Note that this isn't the actual frequency of the PWM signal. That
	 * will be set later to be an integer multiple of this value.
	 */
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	// Enable the PWM peripheral and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}

	// Configure the internal multiplexer to connect the PWM peripheral to PF3
	GPIOPinConfigure(GPIO_PF3_M1PWM7);

	// Set up the PWM module on pin PF3
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);

	// Set up INA pin as Pin D0
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	// Set up INB pin as Pin D1
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);


	//CURRENT SENSE CONFIG

	//
	// Select the analog ADC function for these pins.
	// Consult the data sheet to see which functions are allocated per pin.
	// TODO: change this to select the port/pin you are using.
	//
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	//
	// Enable sample sequence 3 with a processor signal trigger.  Sequence 3
	// will do a single sample when the processor sends a signal to start the
	// conversion.  Each ADC module has 4 programmable sequences, sequence 0
	// to sequence 3.  This example is arbitrarily using sequence 3.
	//
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

	//
	// Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
	// single-ended mode (default) and configure the interrupt flag
	// (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
	// that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
	// 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
	// sequence 0 has 8 programmable steps.  Since we are only doing a single
	// conversion using sequence 3 we will only configure step 0.  For more
	// information on the ADC sequences and steps, reference the datasheet.
	//
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
							 ADC_CTL_END);

	//
	// Since sample sequence 3 is now configured, it must be enabled.
	//
	ADCSequenceEnable(ADC0_BASE, 3);


	//ENCODER CONFIG

	/*
	 * 	Configure GPIO pins D6 and D7, as Phase A and B
	 *	respectively of quadrature encoder
	 */
		GPIOPinConfigure(GPIO_PD6_PHA0);
		GPIOPinConfigure(GPIO_PD7_PHB0);
		GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	/*    Configure QEI.*/
	/*    Disable everything first.*/
		QEIDisable(QEI0_BASE);
		QEIVelocityDisable(QEI0_BASE);
		QEIIntDisable(QEI0_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));

	/*    Configure the QEI to capture on both A and B, not to reset when there is an index pulse, configure it as a quadrature encoder,
		and doesn't swap signals PHA0 and PHB0 and set the maxmimum position as 1999. (MAX_POS_CAPTURE_A_B = (ppr*4)-1.'*/
		QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 1999);
		QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_16, 40000);
		QEIPositionSet(QEI0_BASE, 999);

	/*    Re-enable what needs to be enabled.*/
		QEIEnable(QEI0_BASE);
		QEIVelocityEnable(QEI0_BASE);
}
// Run motor at set speed
void motor_run(int direction, double percentage){
	long PWMFrequencyHz= 250;
	double PWMDutyCycleDivisor=(100.00/percentage);
	int N= SysCtlPWMClockGet()/PWMFrequencyHz;

	if ((direction == 1 || direction==-1 ) && percentage>0 && percentage < 100 && PWMDutyCycleDivisor!=1.000) {

		PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, N);

		/* Set the PWM duty cycle to 25%. The duty cycle is a function of the period.
		 *
		 * Warning! The PWM generator often freaks out when you set 100% duty cycle.
		 * It's a known issue in the silicon:
		 * https://e2e.ti.com/support/microcontrollers/tiva_arm/f/908/t/448664
		 * To avoid this, cap the PWM output at (MaxDuty - 1). I personally wouldn't
		 * trust it at 0% either. If you strictly need 0% and 100%, you should
		 * switch the pin back to a GPIO. Though this may take some time (~5us).
		 */
		// As muxed from M1PWM7
		int pulseWidth= PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/PWMDutyCycleDivisor;
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, pulseWidth);

		// Enable the PWM output signal
		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
		// Enable the PWM peripheral
		PWMGenEnable(PWM1_BASE, PWM_GEN_3);
		//can also write as:
		//GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
		//GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);

		//  Set motor direction
		//TODO: transform to smaller function call
		if (direction==1){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
		}
		else if(direction==-1){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
		}
	}

	else{
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
	}
}
uint32_t getCurrent(void){

	// Clear the interrupt status flag.  This is done to make sure the
	// interrupt flag is cleared before we sample.
	ADCIntClear(ADC0_BASE, 3);

	// Sample AIN0 forever
	while(1) {
		// Trigger the ADC conversion and wait for conversion to be completed.
		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false)) {}

		// Clear the ADC interrupt flag.
		ADCIntClear(ADC0_BASE, 3);

		// Read ADC Value.
		ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);

		toFileADCRead = pui32ADC0Value[0];
		// Delay for certain amount of time.
		SysCtlDelay(30);
		return toFileADCRead;
	}
}

uint32_t getPosition(void){
	return ui32QEIPosition = QEIPositionGet(QEI0_BASE);
}

uint32_t getVelocity(void){
	ui32QEIVelocity = QEIVelocityGet(QEI0_BASE);
	return ui32QEIVelocity;
}

uint32_t getDirection(void){
	return ui32QEIDirection = QEIDirectionGet(QEI0_BASE);
}

void position_change_to(double degree, int direction, double speedPercentage){
	ui32QEIPositionInitial = QEIPositionGet(QEI0_BASE);

	while (ui32QEIPosition != ((2000*degree/360)+ui32QEIPositionInitial)){
		motor_run(direction, speedPercentage);
		ui32QEIPosition = QEIPositionGet(QEI0_BASE);
	}
}

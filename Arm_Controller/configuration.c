
#include "arm_controller.h"

/*
 * Position Loop Interrupt Configuration
 */
void configure_pos_timer(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	uint32_t pos_period = SysCtlClockGet()/POS_FREQ;
	TimerLoadSet(TIMER2_BASE, TIMER_A, pos_period -1);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	//Enables the processor to respond to interrupts
	IntMasterEnable();
	TimerEnable(TIMER2_BASE, TIMER_A);
}

/*
 * Velocity Loop Interrupt Configuration
 */
void configure_vel_timer(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	uint32_t pos_period = SysCtlClockGet()/VEL_FREQ;
	TimerLoadSet(TIMER1_BASE, TIMER_A, pos_period -1);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	//Enables the processor to respond to interrupts
	IntMasterEnable();
	TimerEnable(TIMER1_BASE, TIMER_A);
}

/*
 * Torque Loop Interrupt Configuration
 */
void configure_torque_timer(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	uint32_t pos_period = SysCtlClockGet()/TORQUE_FREQ;
	TimerLoadSet(TIMER0_BASE, TIMER_A, pos_period -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//Enables the processor to respond to interrupts
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);
}


void configure_gpio(void){
	// Enable the GPIO peripheral base D and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}

	/*	Unlock pins on peripheral D*/
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; //In Tiva include this is the same as "_DD" in older versions (0x4C4F434B)
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

	// Set up INA pin as Pin D0
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	// Set up INB pin as Pin D1
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);

	// Enable the GPIO peripheral base F (PWM) and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}

	// Enable the GPIO peripheral base E (ADC) and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

}
/*
 * Encoder Configuration
 */
void configure_qei(void){

	/*
	 * 	Configure GPIO pins D6 and D7, as Phase A and B
	 *	respectively of quadrature encoder
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
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

/*
 * PWM and INA and INB Configuration for motor
 */
void configure_motor(void){


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
}

/*
 * uDMA Configuration
 */
void configure_uDMA(void){

}


/*
 * ADC Configuration
 */
void configure_ADC(void){
	// The ADC0 peripheral must be enabled for use and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}




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
}


/*******************************************************************************
 *                             Include Statements                              *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include <math.h>

/*******************************************************************************
 *                                Main Method                                  *
 ******************************************************************************/
void main(void) {
	initialize_pins();
	motor_run(-1, 20.00);
}

/*******************************************************************************
 *                              Helper Functions                               *
 ******************************************************************************/

void initialize_pins() {

	// Set system clock to 80MHz using a PLL (200MHz / 2.5 = 80MHz)
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);

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

	// Enable the GPIO ports with base F and wait for it to be ready.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}

	//Enable GPIO ports with base D and wait for it to be ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}

	// Configure the internal multiplexer to connect the PWM peripheral to PF3
	// PF3 is also attached to the LED
	GPIOPinConfigure(GPIO_PF3_M1PWM7);

	// Set up the PWM module on pin PF3
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	// INA pin chosen as Tiva Pin D0
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	// INB pin chosen as Tiva Pin D1
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
	//Can also write as:
	//GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1);

	/* Configure PWM mode to count up/down without synchronization
	 * This is just a detail, you probably don't have to worry about it.
	 * See pages 1235 and 1237 in the data sheet if you want to learn more.
	 */
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
			PWM_GEN_MODE_NO_SYNC);
}

void motor_run(int direction, double percentage){
	long PWMFrequencyHz= 250;
	double PWMDutyCycleDivisor=(100.00/percentage);

	int periodConstant= SysCtlPWMClockGet()/PWMFrequencyHz;

	/*
	 * Warning! The PWM generator often freaks out when you set 100% duty cycle.
	 * It's a known issue in the silicon:
	 * https://e2e.ti.com/support/microcontrollers/tiva_arm/f/908/t/448664
	 * To avoid this, cap the PWM output at (MaxDuty - 1). I personally wouldn't
	 * trust it at 0% either. If you strictly need 0% and 100%, you should
	 * switch the pin back to a GPIO. Though this may take some time (~5us).
	 */

	if ((direction == 1 || direction==-1 ) && percentage>0 && percentage < 100 && PWMDutyCycleDivisor!=1.000) {

		PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, periodConstant);

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

		// set motor speed according to PWM value
		if (direction==1){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
		}
		else if(direction==-1){
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
		}
		while(1){}
	}

	else{

	}
}



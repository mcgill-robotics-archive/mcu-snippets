#include "arm_controller.h"

// Run motor at set speed
void motor_run(int direction, double percentage){
	long PWMFrequencyHz= 20000;
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



#include "arm_controller.h"

// Run motor at set speed
void motor_stop(void){
		// Disable the PWM output signal
		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
		// Set InA and InB to low
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
}

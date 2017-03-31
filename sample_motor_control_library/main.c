/*
 * Test file for bdc_motor.c, to be replaced with rosserial code
 */

#include "bdc_motor.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);

	// Initialize a motor
	BDC motor1;
	// IN1 - Speed Output
	motor1.SYSCTL_PERIPH_PWM_IN1 = NULL;
	motor1.SYSCTL_PERIPH_GPIO_IN1 = NULL;
	motor1.GPIO_PWM_IN1 = NULL;
	motor1.GPIO_PORT_BASE_IN1 = NULL;
	motor1.GPIO_PIN_IN1 = NULL;
	motor1.PWM_BASE_IN1 = NULL;
	motor1.PWM_GEN_IN1 = NULL;
	motor1.PWM_OUT_BIT_IN1 = NULL;
	// IN2 - Direction Output
	motor1.SYSCTL_PERIPH_GPIO_IN2 = NULL;
	motor1.GPIO_PORT_BASE_IN2 = NULL;
	motor1.GPIO_PIN_IN2 = NULL;
	// nFAULT - Fault Status Input
	motor1.SYSCTL_PERIPH_GPIO_nFAULT = NULL;
	motor1.GPIO_PORT_BASE_nFAULT = NULL;
	motor1.GPIO_PIN_nFAULT = NULL;
	// nRESET - Reset Output
	motor1.SYSCTL_PERIPH_GPIO_nRESET = NULL;
	motor1.GPIO_PORT_BASE_nRESET = NULL;
	motor1.GPIO_PIN_nRESET = NULL;
	// BRAKE - Brake Output
	motor1.SYSCTL_PERIPH_GPIO_BRAKE = NULL;
	motor1.GPIO_PORT_BASE_BRAKE = NULL;
	motor1.GPIO_PIN_BRAKE = NULL;
	// CS - Current Sense Input
	motor1.SYSCTL_PERIPH_ADC_CS = NULL;
	motor1.SYSCTL_PERIPH_GPIO_CS = NULL;
	motor1.GPIO_PORT_BASE_CS = NULL;
	motor1.GPIO_PIN_CS = NULL;
	motor1.ADC_BASE_CS = NULL;
	motor1.ADC_CTL_CH_CS = NULL;

	// Example usage...
	bdc_init(motor1);
	bdc_set_brake(motor1, 0);
	bdc_set_enabled(motor1, 1);
	return 0;
}

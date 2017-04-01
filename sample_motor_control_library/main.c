/*
 * Test file for bdc_motor.c, to be replaced with rosserial code
 */

#include "bdc_motor.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);

	// Initialize a motor
	BDC motor1;
	// IN1 - Speed Output PB6
	motor1.SYSCTL_PERIPH_PWM_IN1 = SYSCTL_PERIPH_PWM0;
	motor1.SYSCTL_PERIPH_GPIO_IN1 = SYSCTL_PERIPH_GPIOB;
	motor1.GPIO_PWM_IN1 = GPIO_PB6_M0PWM0;
	motor1.GPIO_PORT_BASE_IN1 = GPIO_PORTB_BASE;
	motor1.GPIO_PIN_IN1 = GPIO_PIN_6;
	motor1.PWM_BASE_IN1 = PWM0_BASE;
	motor1.PWM_GEN_IN1 = PWM_GEN_0;
	motor1.PWM_OUT_BIT_IN1 = PWM_OUT_0_BIT;
	motor1.PWM_OUT_IN1 = PWM_OUT_0;
	// IN2 - Direction Output PB7
	motor1.SYSCTL_PERIPH_GPIO_IN2 = SYSCTL_PERIPH_GPIOB;
	motor1.GPIO_PORT_BASE_IN2 = GPIO_PORTB_BASE;
	motor1.GPIO_PIN_IN2 = GPIO_PIN_7;
	// nFAULT - Fault Status Input PE3
	motor1.SYSCTL_PERIPH_GPIO_nFAULT = SYSCTL_PERIPH_GPIOE;
	motor1.GPIO_PORT_BASE_nFAULT = GPIO_PORTE_BASE;
	motor1.GPIO_PIN_nFAULT = GPIO_PIN_3;
	// nRESET - Reset Output PA1
	motor1.SYSCTL_PERIPH_GPIO_nRESET = SYSCTL_PERIPH_GPIOA;
	motor1.GPIO_PORT_BASE_nRESET = GPIO_PORTA_BASE;
	motor1.GPIO_PIN_nRESET = GPIO_PIN_1;
	// BRAKE - Brake Output PF3
	motor1.SYSCTL_PERIPH_GPIO_BRAKE = SYSCTL_PERIPH_GPIOF;
	motor1.GPIO_PORT_BASE_BRAKE = GPIO_PORTF_BASE;
	motor1.GPIO_PIN_BRAKE = GPIO_PIN_3;
	// CS - Current Sense Input PE1
	motor1.SYSCTL_PERIPH_ADC_CS = SYSCTL_PERIPH_ADC0;
	motor1.SYSCTL_PERIPH_GPIO_CS = SYSCTL_PERIPH_GPIOE;
	motor1.GPIO_PORT_BASE_CS = GPIO_PORTE_BASE;
	motor1.GPIO_PIN_CS = GPIO_PIN_1;
	motor1.ADC_BASE_CS = ADC0_BASE;
	motor1.ADC_CTL_CH_CS = ADC_CTL_CH2;

	// Example usage...
	bdc_init(motor1);
	bdc_set_velocity(motor1, 1000);

	while(1){}
	return 0;
}

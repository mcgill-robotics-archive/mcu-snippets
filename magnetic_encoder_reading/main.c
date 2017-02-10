#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "driverlib/pin_map.h"
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

void RisingEdgeInt(void){
	TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
	// Read the current state of the GPIO pin (blue LED) and
		// write back the opposite state
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
		}
}

void FallingEdgeInt(void){
	TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);
	// write back the opposite state
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
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	GPIOPinConfigure(GPIO_PB6_T0CCP0);

	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_6);

	//Enable GPIO clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//Configure GPIO pins as output
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
	{
	}

	//Configure Timer 0 to be a concatenated 32-bit periodic timer
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_CAP_TIME_UP);

	TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);

	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
	TimerIntEnable(TIMER0_BASE, TIMER_CAPB_EVENT);

	TimerIntRegister(TIMER0_BASE, TIMER_A, RisingEdgeInt);
	TimerIntRegister(TIMER0_BASE, TIMER_B, FallingEdgeInt);


	//Enables the processor to respond to interrupts
	IntMasterEnable();

	//Start the timer
	TimerEnable(TIMER0_BASE, TIMER_A);
	TimerEnable(TIMER0_BASE, TIMER_B);
	return 0;
}

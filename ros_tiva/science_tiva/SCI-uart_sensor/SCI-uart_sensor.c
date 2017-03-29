#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void UARTIntHandler(void){	// The UART interrupt handler.
	uint32_t ui32Status;
	ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
	ROM_UARTIntClear(UART0_BASE, ui32Status);
}

void ADCPrint_w(uint32_t x, uint32_t y, uint32_t z){
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, 'W');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ':');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (x + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, (y + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, '.');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (z + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, 'm');
	ROM_UARTCharPutNonBlocking(UART0_BASE, '/');
	ROM_UARTCharPutNonBlocking(UART0_BASE, 's');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');

	SysCtlDelay(SysCtlClockGet() / (1000 * 3));
}

void ADCPrint_t(uint32_t x, uint32_t y, uint32_t z){
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, 'T');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ':');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (x + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, (y + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, '.');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (z + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, 'C');
	ROM_UARTCharPutNonBlocking(UART0_BASE, '*');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');

	SysCtlDelay(SysCtlClockGet() / (1000 * 3));
}

void ADCPrint_h(uint32_t x, uint32_t y, uint32_t z){
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, 'H');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ':');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (x + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, (y + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, '.');
	ROM_UARTCharPutNonBlocking(UART0_BASE, (z + 48));
	ROM_UARTCharPutNonBlocking(UART0_BASE, '%');
	ROM_UARTCharPutNonBlocking(UART0_BASE, ' ');
	ROM_UARTCharPutNonBlocking(UART0_BASE, '\n');

	SysCtlDelay(SysCtlClockGet() / (1000 * 3));
}

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count){	// Send a string to the UART.
	while(ui32Count--){
		ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
	}
}




int main(void){
	uint32_t a, b, c, wind[1], temperature[1], humidity[1];	//Declaring variables

	ROM_FPUEnable();
	ROM_FPULazyStackingEnable();
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// Enable the GPIO port that is used for the on-board LED.
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);		// Enable the GPIO pins for the LED (PF2).

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	// Enable the peripherals used by this example.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_IntMasterEnable();	// Enable processor interrupts.
	GPIOPinConfigure(GPIO_PA0_U0RX);	// Set GPIO A0 and A1 as UART pins.
	GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	ROM_IntEnable(INT_UART0);	// Enable the UART interrupt.
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);	// Enable the ADC peripheral and wait for it to be ready.
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}

	ADCHardwareOversampleConfigure(ADC0_BASE, 64);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	// Enable GPIO port B and wait for it to be ready.
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);	// Configure PB5 as an ADC input
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);

	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);// Configure the ADC sample sequence.

	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH7 | ADC_CTL_IE |ADC_CTL_END);	// Configure the ADC sample sequence steps
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH6 | ADC_CTL_IE |ADC_CTL_END);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH5 | ADC_CTL_IE |ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 3);	// Since sample sequence 3 is now configured, it must be enabled.


	ADCIntClear(ADC0_BASE, 3);

	UARTSend((uint8_t *)"\033[2JPress Enter: ", 16);	// Prompt for text to be entered.

	while(1){	// Echoing data through the UART.

		//wind
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0,  ADC_CTL_CH7 | ADC_CTL_IE |ADC_CTL_END); //PD0
		ADCIntClear(ADC0_BASE, 3);
		ADCProcessorTrigger(ADC0_BASE, 3);	// Trigger the ADC conversion.
		while(!ADCIntStatus(ADC0_BASE, 3, false)){}	// Wait for conversion to be completed.
		ADCIntClear(ADC0_BASE, 3);	// Clear the ADC interrupt flag.
		ADCSequenceDataGet(ADC0_BASE, 3, wind); // Read ADC Value. ADC has 12-bit precision so the output ranges from 0 to 4095


		//temperature
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0,  ADC_CTL_CH6 | ADC_CTL_IE |ADC_CTL_END); //PD1
		ADCIntClear(ADC0_BASE, 3);
		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false)){}
		ADCIntClear(ADC0_BASE, 3);
		ADCSequenceDataGet(ADC0_BASE, 3, temperature);


		//humidity
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0,  ADC_CTL_CH5 | ADC_CTL_IE |ADC_CTL_END); //PD2
		ADCIntClear(ADC0_BASE, 3);
		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false)){}
		ADCIntClear(ADC0_BASE, 3);
		ADCSequenceDataGet(ADC0_BASE, 3, humidity);


		if(ROM_UARTCharsAvail(UART0_BASE)){
//			UARTSend((uint8_t *)"\033[2JSensor Values:  ", 16);
			if(wind[0]>(uint32_t)495 & wind[0]<(uint32_t)2483){// the voltage is above 0.4 and below 2v.
				a = (((wind[0]-496)*324)/198600);
				b = (((wind[0]-496)*324)/19860)-(10*a);
				c = (((wind[0]-496)*324)/1986)-(100*a)-(10*b);
				ADCPrint_w(a,b,c);
			}
			else
			{
				ADCPrint_w(0,0,0);
			}
			if(temperature[0]>(uint32_t)1551){
				a = (((temperature[0]-1552))/621);
				b = (((temperature[0]-1552))/62)-(10*a);
				c = (((temperature[0]-1552))/6)-(100*a)-(10*b);
				ADCPrint_t(a,b,c);
			}
			else
			{
				ADCPrint_t(0,0,0);
			}
			if(humidity[0]>0)
			{
				a = (humidity[0]/4095);
				b = ((humidity[0]/4095)*10)-(10*a);
				c = ((humidity[0]/4095)*100)-(100*a)-(10*b);
				ADCPrint_h(a,b,c);
			}
			else
			{
				ADCPrint_h(0,0,0);
			}

	        ROM_UARTCharGetNonBlocking(UART0_BASE);

	        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
	        SysCtlDelay(SysCtlClockGet() / (2000 * 3));
	        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		}
	}
}

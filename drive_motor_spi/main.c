#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include "driverlib/pwm.h"

#define NUM_SSI_DATA            2

void
InitConsole(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
	{
	}
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTStdioConfig(0, 115200, 16000000);
}

int
main(void) {

	uint32_t pui32DataTx[NUM_SSI_DATA];
	uint32_t pui32DataRx[NUM_SSI_DATA];
	uint32_t ui32Index;

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}

	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
			PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 40000);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,
			PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/2);
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

	PWMGenEnable(PWM1_BASE, PWM_GEN_3);


	InitConsole();

	UARTprintf("SSI ->\n");
	UARTprintf("  Mode: SPI Master\n");
	UARTprintf("  Data: 16-bit\n\n");

	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA4_SSI0RX);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);

	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
			GPIO_PIN_2);


	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, 20000, 16);

	SSIEnable(SSI0_BASE);

	while (1){
		pui32DataTx[0] = 0x1202;
		pui32DataTx[1] = 0xF209;

		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
		while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx[0]))
		{
		}

		UARTprintf("Sent:\n  ");
		UARTprintf("'%04x' ", pui32DataTx[0]);
		SSIDataPut(SSI0_BASE, pui32DataTx[0]);

		SSIDataGet(SSI0_BASE, &pui32DataRx[0]);

		pui32DataRx[0] &= 0xFFFF;

		UARTprintf("'%04x' ", pui32DataRx[0]);
		while(SSIBusy(SSI0_BASE))
		{
		}
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
		SysCtlDelay(1);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
		UARTprintf("'%04x' ", pui32DataTx[1]);
		SSIDataPut(SSI0_BASE, pui32DataTx[1]);
		SSIDataGet(SSI0_BASE, &pui32DataRx[1]);

		pui32DataRx[1] &= 0xFFFF;

		UARTprintf("'%04x' ", pui32DataRx[1]);
		while(SSIBusy(SSI0_BASE))
		{
		}
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
	}
}

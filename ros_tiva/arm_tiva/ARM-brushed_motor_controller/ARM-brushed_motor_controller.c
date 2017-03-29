/*******************************************************************************
 *                     DRV8842 DC Motor Driver                                  *
 ******************************************************************************/

//Include
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"

//Debug
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//The UART interrupt handler
void UARTIntHandler(void){
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, ui32Status);
}

//Send a string to the UART
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count){
	while(ui32Count--){
		UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
	}
}

 void main(void) {
	//Variables
	bool StartStop = false;
	bool RotationDir = false;
	bool RotationDir_flag = false;
	char input = 0x7E;
	int num = 10; // 0 to 99
	int counter = 0;
	char point[16] = {};

	//Clock
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Set system clock to 80MHz

	//PWM setup
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8); // Set PWM clock to 10MHz
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); // Enable the PWM peripheral
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable the GPIO peripheral
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}

	//GPIO setup
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Enable GPIO ports with base D
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
	
	//UART setup with interrupt
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // Enable the UART peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX); // Set GPIO A0 and A1 as UART pins
	GPIOPinConfigure(GPIO_PA1_U0TX);
	IntMasterEnable(); // Enable interrupts

	//UART pin assignments with interrupt
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // Set up RX pin as A0, and TX as pin A1
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | // UART serial setup
			UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntEnable(INT_UART0); // Enable the UART interrupt
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); // Enable UART RX and TX

	//Pin assignments
	GPIOPinConfigure(GPIO_PF3_M1PWM7); // Connect the PWM peripheral to PF3
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3); // Set up the PWM module on pin PF3
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1); // Set up the InA pin as D1

	//PWM duty-cycle
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC); // Configure the PWM generator for count
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 40000); // Period = 40000 clock ticks
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/100)*num); // Duty-cycle of the PWM set as half
	PWMGenEnable(PWM1_BASE, PWM_GEN_3); // Enables the counter for a PWM

	//Outputs
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false); // Enable the PWM output signal
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00); // Pin InA set to low
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF); // Pin nRst_A set to low

	//Print line
	UARTSend((uint8_t *)"Press Spacebar:", 15);	// Prompt for press space-bar
	UARTSend((uint8_t *)"\r\n", 2);

	//Loop
	while(1){
		while(UARTCharsAvail(UART0_BASE)){
			input = UARTCharGet(UART0_BASE);
		}
		if(input == 0x20 && StartStop == false){ // Start with spacebar
			StartStop = true;
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xFF);
			if(RotationDir == true){ // reverse:counter clockwise
				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
			}
			else { // forward cw
				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
			}
			PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
			UARTSend((uint8_t *)"\rStart\n\r", 8);
			RotationDir_flag = false;
			counter = 0;
			input = 0x7E;
		}
		if(input == 0x20 && StartStop == true){ // Stop with spacebar
			StartStop = false;
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
			PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
			UARTSend((uint8_t *)"\rStop\n\r", 7);
			RotationDir_flag = false;
			counter = 0;
			input = 0x7E;
		}
		if(input == 0x2D){ // Negative direction
			RotationDir_flag = true;
			counter = 0;
			input = 0x7E;
		}
		if(isdigit(input)){ // Number input
			point[counter]= input;
			counter++;
			if(counter>3){
				counter--;
				UARTCharPutNonBlocking(UART0_BASE, 0x7F);
			}
			input = 0x7E;
		}
		if(input == 0x0D){ // Enter
			point[counter] = '\0';
			num = atoi(point);
			if(num == 0 && RotationDir_flag == true){
				RotationDir = false;
			}
			else {
				RotationDir = RotationDir_flag;
			}
			if(RotationDir == true){ // Reverse motor direction
				if(StartStop == false){
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
				}
				else {
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
				}
				UARTSend((uint8_t *)"-", 1);
				if(num == 0){
					num = 99;
					UARTSend((uint8_t *)"0", 1);
				}
				else if(num >= 100){
					num = 0;
					UARTSend((uint8_t *)"100", 3);
				}
				else {
					num=abs(num-100);
					UARTSend((uint8_t *)point, counter);
				}
			}
			else { // Forward motor direction
				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
				if(num == 0){
					UARTSend((uint8_t *)"0", 1);
				}
				else if(num >= 100){
					num = 99;
					UARTSend((uint8_t *)"100", 3);
				}
				else {
					UARTSend((uint8_t *)point, counter);
				}
			}
			UARTSend((uint8_t *)"\r\n", 2);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/100)*num);
			RotationDir_flag = false;
			counter = 0;
			input = 0x7E;
		}
		if(input == 0x7F){ // Delete
			if(RotationDir_flag == true && counter == 0){
				RotationDir_flag = false;
			}
			else if(counter == 0){
				counter = 0;
			}
			else {
				counter--;
			}
			input = 0x7E;
		}
		if(input != 0x0D && input != 0x2D && input != 0x20 && input != isdigit(input) && input != 0x00 && input != 0x7F && input != 0x7E){
			UARTCharPutNonBlocking(UART0_BASE, 0x7F);
			input = 0x7E;
		}
	}
}

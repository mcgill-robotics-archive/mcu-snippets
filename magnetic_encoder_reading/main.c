#include <stdint.h> // Variable definitions for the C99 standard.
#include <stdio.h> // Input and output facilities for the C99 standard.
#include <stdbool.h> // Boolean definitions for the C99 standard.
#include "inc/tm4c123gh6pm.h" // Definitions for the interrupt and register assignments.
#include "inc/hw_memmap.h" // Memory map definitions of the Tiva C Series device.
#include "inc/hw_types.h" // Definitions of common types and macros.
#include "driverlib/sysctl.h" // Definitions and macros for System Control API of DriverLib.
#include "driverlib/interrupt.h" // Defines and macros for NVIC Controller API of DriverLib.
#include "driverlib/gpio.h" // Definitions and macros for GPIO API of DriverLib.
#include "driverlib/timer.h" // Defines and macros for Timer API of DriverLib.
#include "driverlib/pin_map.h" //Mapping of peripherals to pins for all parts.
#include "driverlib/uart.h" // Definitions and macros for UART API of DriverLib.
#include "driverlib/adc.h" // Definitions for ADC API of DriverLib.
#include "driverlib/fpu.h" // Prototypes for the FPU manipulation routines.
#include "utils/uartstdio.h" // Prototypes for the UART console functions.
#include "utils/uartstdio.c"                            // Needs to add "utils/uartstdio.c" through a relative link.
#include "driverlib/pwm.h"
#include <math.h>
//definitions
#define UART1_BAUDRATE      115200  // UART baudrate in bps

// function prototypes
void init_timer(void);
void duty_cycle(void);
void init_UART(void);
void init_pwm(void);

// global variables
uint32_t sys_clock;
int32_t  start = 0, end = 0, length = 0;
uint32_t angle;

int main(void)
{
    // Configure system clock at 80 MHz.
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    sys_clock = SysCtlClockGet();

    // Enable the processor to respond to interrupts.
    IntMasterEnable();

    init_UART();
    init_pwm();
    init_timer();

    TimerEnable(WTIMER0_BASE, TIMER_BOTH);

    while(1) {
    	duty_cycle();
    }
}

void init_pwm(void)
{
    /*This function configures the rate of the clock provided to the PWM module as a ratio of the
	processor clock. This clock is used by the PWM module to generate PWM signals; its rate
	forms the basis for all PWM signals*/
	//PWM clock rate is 80MHz/8 = 10 MHz
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}

	//PF3 is the PWM output
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);

	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
					PWM_GEN_MODE_NO_SYNC);

	//PWM Period = 10000 PWM clock cycles = 10000*1/10M = 0.001 seconds
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 10000);

	//PWM Pulse width is 500 clock cycles, 500/10000 = 0.05, 0.05*0.001 = 0.00005 seconds
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 500);
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

void init_timer(void)
{
    // Enable and configure Timer0 peripheral.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);

    // Initialize timer A and B to count up in edge time capture mode
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

    // Timer a records pos edge time and Timer b records neg edge time
    TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(WTIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);

    //TimerPrescaleSet(WTIMER0_BASE, TIMER_A , 0 );
    //TimerPrescaleSet(WTIMER0_BASE, TIMER_B , 0 );

    //set the value that the timers count to (0x9C3F = 39999)
    //CO2 sensor outputs 1khz pwm so with mcu at 40Mhz, timers should stay in sync with CO2 output
    TimerLoadSet(WTIMER0_BASE, TIMER_BOTH, 0x9C3F);

    //Configure the pin that the timer reads from (PB6)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinConfigure(GPIO_PC4_WT0CCP0);
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);


    // Registers a interrupt function to be called when timer b hits a neg edge event
    IntRegister(INT_WTIMER0B, duty_cycle);
    // Makes sure the interrupt is cleared
    TimerIntClear(WTIMER0_BASE, TIMER_CAPB_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(WTIMER0_BASE, TIMER_CAPB_EVENT);
    // The specified interrupt is enabled in the interrupt controller.
    IntEnable(INT_WTIMER0B);
}

//When negative edge is hit, record the values and find the difference, output to putty
void duty_cycle(void)
{
    TimerIntClear(WTIMER0_BASE, TIMER_CAPB_EVENT);
    start = TimerValueGet(WTIMER0_BASE, TIMER_A);
    end = TimerValueGet(WTIMER0_BASE, TIMER_B);
    length = end - start;
    if (length<0){
    	UARTprintf("\nLENGTH = %d\n", length);
    	length= 40000+length;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle = (360*length)/80000;
    UARTprintf("\nSTART = %d\n", start);
    UARTprintf("\nEND = %d\n", end);
    UARTprintf("\nLENGTH = %d\n", length);
    UARTprintf("\nANGLE = %d\n", angle);
}

void init_UART(void)
{
    // Enable and configure UART0 for debugging printouts.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
    UARTStdioConfig(0, UART1_BAUDRATE, sys_clock);
}


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
void init_UART(void);
void init_pwm(void);
void interrupt_h_encoder_0(void);
void interrupt_h_encoder_1(void);
void interrupt_h_encoder_2(void);
void duty_cycle_0(void);
void duty_cycle_1(void);
void duty_cycle_2(void);

// global variables
uint32_t sys_clock;
int32_t  start_0 = 0, end_0 = 0, length_0 = 0;
int32_t  start_1 = 0, end_1 = 0, length_1 = 0;
int32_t  start_2 = 0, end_2 = 0, length_2 = 0;

uint32_t angle0, angle1, angle2;
int encoder0_flag, encoder1_flag, encoder2_flag;

int main(void)
{
	encoder0_flag=0;
	encoder1_flag=0;
	encoder2_flag=0;

    // Configure system clock at 80 MHz.
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    sys_clock = SysCtlClockGet();
    IntMasterEnable();


    init_UART();
    init_pwm();
    init_timer();

    IntEnable(INT_WTIMER0B);
    IntPrioritySet(INT_WTIMER0B, 2);


    IntEnable(INT_WTIMER1B);
    IntPrioritySet(INT_WTIMER1B, 1);


    IntEnable(INT_WTIMER2B);
    IntPrioritySet(INT_WTIMER2B, 0);


    // The specified interrupt is enabled in the interrupt controller.

    //IntEnable(INT_WTIMER0B|INT_WTIMER1B|INT_WTIMER2B);

    // Enable the processor to respond to interrupts.

    uint32_t counter = 0;

    while(1) {

    	counter = (counter+1)%3;
    	switch (counter){
    	case 0:
    		TimerEnable(WTIMER0_BASE, TIMER_BOTH);
    	    TimerDisable(WTIMER1_BASE, TIMER_BOTH);
    	    TimerDisable(WTIMER2_BASE, TIMER_BOTH);
    	    break;

    	case 1:
    		TimerDisable(WTIMER0_BASE, TIMER_BOTH);
    	    TimerEnable(WTIMER1_BASE, TIMER_BOTH);
    	    TimerDisable(WTIMER2_BASE, TIMER_BOTH);
    	    break;

    	case 2:
    		TimerDisable(WTIMER0_BASE, TIMER_BOTH);
    	    TimerDisable(WTIMER1_BASE, TIMER_BOTH);
    	    TimerEnable(WTIMER2_BASE, TIMER_BOTH);
    	    break;
    	}

    	if (encoder0_flag==1){
    		duty_cycle_0();
    	}
    	if(encoder1_flag==1){
    		duty_cycle_1();
    	}
    	if(encoder2_flag== 1){
    		duty_cycle_2();
    	}
    }
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
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER0)){}

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER1)){}

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER2)){}

    // Initialize timer A and B to count up in edge time capture mode
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

    TimerConfigure(WTIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

    TimerConfigure(WTIMER2_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));

    // Timer a records pos edge time and Timer b records neg edge time
    TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(WTIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);

    TimerControlEvent(WTIMER1_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(WTIMER1_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);

    TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);

    TimerPrescaleSet(WTIMER0_BASE, TIMER_A , 0 );
    TimerPrescaleSet(WTIMER0_BASE, TIMER_B , 0 );

    //set the value that the timers count to (0x9C3F = 39999)
    //CO2 sensor outputs 1khz pwm so with mcu at 40Mhz, timers should stay in sync with CO2 output
    TimerLoadSet(WTIMER0_BASE, TIMER_BOTH, 0x9C3F);
    TimerLoadSet(WTIMER1_BASE, TIMER_BOTH, 0x9C3F);
    TimerLoadSet(WTIMER2_BASE, TIMER_BOTH, 0x9C3F);
    //Configure the pin that the timer reads from (PB6)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //Timer pins are PC4, PC5; PC6, PC7; PD0, PD1
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);

    GPIOPinConfigure(GPIO_PC6_WT1CCP0);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PC7_WT1CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_7);

    GPIOPinConfigure(GPIO_PD0_WT2CCP0);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD1_WT2CCP1);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);


    // Registers a interrupt function to be called when timer b hits a neg edge event
    IntRegister(INT_WTIMER0B, interrupt_h_encoder_0);
    // Makes sure the interrupt is cleared
    TimerIntClear(WTIMER0_BASE, TIMER_CAPB_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(WTIMER0_BASE, TIMER_CAPB_EVENT);

    // Registers a interrupt function to be called when timer b hits a neg edge event
    IntRegister(INT_WTIMER1B, interrupt_h_encoder_1);
    // Makes sure the interrupt is cleared
    TimerIntClear(WTIMER1_BASE, TIMER_CAPB_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(WTIMER1_BASE, TIMER_CAPB_EVENT);

    // Registers a interrupt function to be called when timer b hits a neg edge event
    IntRegister(INT_WTIMER2B, interrupt_h_encoder_2);
    // Makes sure the interrupt is cleared
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);

}


void
interrupt_h_encoder_0(void){
    TimerIntClear(WTIMER0_BASE, TIMER_CAPB_EVENT);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 1);
    start_0 = TimerValueGet(WTIMER0_BASE, TIMER_A);
    end_0 = TimerValueGet(WTIMER0_BASE, TIMER_B);
	encoder0_flag= 1;
	UARTprintf("in ISR 0 \n");

}

void
interrupt_h_encoder_1(void){
    TimerIntClear(WTIMER1_BASE, TIMER_CAPB_EVENT);
    start_1 = TimerValueGet(WTIMER1_BASE, TIMER_A);
    end_1 = TimerValueGet(WTIMER1_BASE, TIMER_B);
	encoder1_flag= 1;
	UARTprintf("in ISR 1 \n");
}

void
interrupt_h_encoder_2(void){
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
    start_2 = TimerValueGet(WTIMER2_BASE, TIMER_A);
    end_2 = TimerValueGet(WTIMER2_BASE, TIMER_B);
	encoder2_flag= 1;
	UARTprintf("in ISR 2 \n");
}

//When negative edge is hit, record the values and find the difference, output to putty
void duty_cycle_0(void)
{
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 2);
	encoder0_flag= 0;
    length_0 = end_0 - start_0;
    //UARTprintf("\nLENGTH0 = %d\n", length_0);
    if (length_0<0){

    	length_0 = 40000+length_0;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle0 = (360*length_0)/80000;


//    UARTprintf("\nSTART0 = %d\n", start);
//    UARTprintf("\nEND0 = %d\n", end);
    UARTprintf("\nLENGTH0 = %d\n", length_0);
    UARTprintf("\nANGLE0 = %d\n", angle0);
}

void duty_cycle_1(void)
{
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
	encoder1_flag=0;

    length_1 = end_1 - start_1;
    if (length_1<0){
//    	UARTprintf("\nLENGTH1 = %d\n", length_1);
    	length_1= 40000+length_1;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle1 = (360*length_1)/80000;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 1);
//    UARTprintf("\nSTART1 = %d\n", start);
//    UARTprintf("\nEND1 = %d\n", end);
    UARTprintf("\nLENGTH1 = %d\n", length_1);
    UARTprintf("\nANGLE1 = %d\n", angle1);
}

void duty_cycle_2(void)
{
	encoder2_flag=0;

    length_2 = end_2 - start_2;
    if (length_2<0){
//    	UARTprintf("\nLENGTH2 = %d\n", length_2);
    	length_2= 40000+length_2;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle2 = (360*length_2)/80000;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 2);
//    UARTprintf("\nSTART2 = %d\n", start);
//    UARTprintf("\nEND2 = %d\n", end);
    UARTprintf("\nLENGTH2 = %d\n", length_2);
    UARTprintf("\nANGLE2 = %d\n", angle2);
}


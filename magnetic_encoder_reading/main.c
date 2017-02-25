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
#include "utils/uartstdio.c" // Needs to add "utils/uartstdio.c" through a relative link.
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
int32_t  start0 = 0, end0 = 0, length0 = 0,start1 = 0, end1 = 0, length1 = 0,start2 = 0, end2 = 0, length2 = 0;
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

    // Enable the processor to respond to interrupts.
    IntMasterEnable();

    init_UART();
    init_pwm();
    init_timer();

    TimerEnable(WTIMER0_BASE, TIMER_BOTH);
    TimerEnable(WTIMER1_BASE, TIMER_BOTH);
    TimerEnable(WTIMER2_BASE, TIMER_BOTH);

    while(1) {
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);

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

    //TimerPrescaleSet(WTIMER0_BASE, TIMER_A , 0 );
    //TimerPrescaleSet(WTIMER0_BASE, TIMER_B , 0 );

    //set the value that the timers count to (0x9C3F = 39999)
    //CO2 sensor outputs 1khz pwm so with mcu at 40Mhz, timers should stay in sync with CO2 output
    TimerLoadSet(WTIMER0_BASE, TIMER_BOTH, 0x9C3F);
    TimerLoadSet(WTIMER1_BASE, TIMER_BOTH, 0x9C3F);
    TimerLoadSet(WTIMER2_BASE, TIMER_BOTH, 0x9C3F);
    //Configure the pin that the timer reads from (PB6)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

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
    IntRegister(INT_WTIMER1B, interrupt_h_encoder_2);
    // Makes sure the interrupt is cleared
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);

    // The specified interrupt is enabled in the interrupt controller.
    IntEnable(INT_WTIMER0B);
    IntEnable(INT_WTIMER1B);
    IntEnable(INT_WTIMER2B);
}

void
interrupt_h_encoder_0(void){
    TimerIntClear(WTIMER0_BASE, TIMER_CAPB_EVENT);
    start0 = TimerValueGet(WTIMER0_BASE, TIMER_A);
    end0 = TimerValueGet(WTIMER0_BASE, TIMER_B);
    encoder0_flag= 1;
}

void
interrupt_h_encoder_1(void){
    TimerIntClear(WTIMER1_BASE, TIMER_CAPB_EVENT);
    start1 = TimerValueGet(WTIMER1_BASE, TIMER_A);
    end1 = TimerValueGet(WTIMER1_BASE, TIMER_B);
    encoder1_flag= 1;
}

void
interrupt_h_encoder_2(void){
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
    start2 = TimerValueGet(WTIMER2_BASE, TIMER_A);
    end2 = TimerValueGet(WTIMER2_BASE, TIMER_B);
    encoder2_flag= 1;
}

//When negative edge is hit, record the values and find the difference, output to putty
void duty_cycle_0(void)
{
    encoder0_flag= 0;
    length0 = end0 - start0;
    if (length0<0){
        //UARTprintf("\nLENGTH0 = %d\n", length);
        length0= 4000+length0;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle0 = (360*length0)/80000;
    UARTprintf("\nSTART0 = %d\n", start0);
    UARTprintf("\nEND0 = %d\n", end0);
    UARTprintf("\nLENGTH0 = %d\n", length0);
    UARTprintf("\nANGLE0 = %d\n", angle0);
}

void duty_cycle_1(void)
{
    encoder1_flag=0;

    length1 = end1 - start1;
    if (length1<0){
        UARTprintf("\nLENGTH1 = %d\n", length1);
        length1= 4000+length1;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle


    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle1 = (360*length1)/80000;
    UARTprintf("\nSTART1 = %d\n", start1);
    UARTprintf("\nEND1 = %d\n", end1);
    UARTprintf("\nLENGTH1 = %d\n", length1);
    UARTprintf("\nANGLE1 = %d\n", angle1);
}

void duty_cycle_2(void)
{
    encoder2_flag=0;

    length2 = end2 - start2;
    if (length2<0){
        UARTprintf("\nLENGTH2 = %d\n", length2);
        length2= 4000+length2;
    }
    //notes: 10MHZ clock, duty cycle of 500 pwm clock cycles => length is always 0.00005 seconds
    //length = 4001 => 4001 sysclock ticks => 4000 * 1/80M = 0.5 E-4 , length read by timer matches PWM duty cycle

*
    //since PWM period is 10000 PWM clock cycles, and length is given in system clock cycles
    //PWM subdiv = 8
    angle2 = (360*length2)/80000;
    UARTprintf("\nSTART2 = %d\n", start2);
    UARTprintf("\nEND2 = %d\n", end2);
    UARTprintf("\nLENGTH2 = %d\n", length2);
    UARTprintf("\nANGLE2 = %d\n", angle2);
}

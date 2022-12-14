/*******************************************************************************
 *                             Include Statements                              *
 ******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_qei.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"

volatile int32_t ui32QEIDirection;
volatile int32_t ui32QEIVelocity;
volatile uint32_t ui32QEIPosition;

/*******************************************************************************
 *                                    Main                                     *
 ******************************************************************************/

int main(void)
{
    volatile uint32_t ui32SysClkFreq;

/*    Set the clock to run at 120MHz.*/
	ui32SysClkFreq= SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
/*    Enable peripherals.*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // Enable Port D module so we can work with it

        // Make pin direction of bits 6 and 7 INPUTS (this may be unnecessary?)
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_3| GPIO_PIN_7 | GPIO_PIN_6, GPIO_DIR_MODE_IN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; //In Tiva include this is the same as "_DD" in older versions (0x4C4F434B)
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

/*    Configure GPIO pins.*/
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);
    GPIOPinConfigure(GPIO_PD3_IDX0);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_3);


/*    Configure QEI.*/
/*    Disable everything first.*/
    QEIDisable(QEI0_BASE);
    QEIVelocityDisable(QEI0_BASE);
    QEIIntDisable(QEI0_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));

/*    Configure the QEI to capture on both A and B, not to reset when there is an index pulse, configure it as a quadrature encoder,
    and doesn't swap signals PHA0 and PHB0 and set the maxmimum position as 1999. (MAX_POS_CAPTURE_A_B = (ppr*4)-1.'*/
// QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 1999);
    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_RESET_IDX | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 1999);
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, 400000);
    QEIPositionSet(QEI0_BASE, 0);

/*    Enable what needs to be enabled.*/
    QEIEnable(QEI0_BASE);
    QEIVelocityEnable(QEI0_BASE);

/*    Loop forever.*/
    while(1)
    {
/*        Get direction (1 = forward, -1 = backward).*/
        ui32QEIDirection = QEIDirectionGet(QEI0_BASE);
/*        Get velocity.*/
        ui32QEIVelocity = QEIVelocityGet(QEI0_BASE);

        ui32QEIPosition = QEIPositionGet(QEI0_BASE);

/*        Delay for certain amount of time.*/
        SysCtlDelay(30);
    }



}




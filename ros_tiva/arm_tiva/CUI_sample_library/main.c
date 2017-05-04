#include "CUI_encoder.h"

int main(void) {
    uint32_t ui32SysClkFreq= SysCtlFreqSet((SYSCTL_XTAL_25MHZ|SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    CUI_inc_encoder cui1;

    cui1.PHA_GPIO_PIN= GPIO_PIN_6;
    cui1.PHB_GPIO_PIN= GPIO_PIN_7;
    cui1.IDX_GPIO_PIN= GPIO_PIN_3;
    
    cui1.QEI_SYSCTL_PERIPH_GPIO= SYSCTL_PERIPH_GPIOD;

    cui1.QEI_GPIO_P_PHA= GPIO_PD6_PHA0;
    cui1.QEI_GPIO_P_PHB= GPIO_PD7_PHB0;
    cui1.QEI_GPIO_P_IDX= GPIO_PD3_IDX0;

    cui1.QEI_GPIO_PORT_BASE= GPIO_PORTD_BASE;
    cui1.QEI_BASE= QEI0_BASE;
    cui1.QEI_VELDIV= QEI_VELDIV_1;
    
    CUI_encoder_init(cui1);
    while (1){
	getDirection(cui1);
	getVelocity(cui1);
	getPosition(cui1);
    }

}

#include "CUI_encoder.h"

void CUI_encoder_init (CUI_inc_encoder cui){

    GPIODirModeSet(cui.QEI_GPIO_PORT_BASE, cui.PHA_GPIO_PIN|cui.PHB_GPIO_PIN|IDX_GPIO_PIN, GPIO_DIR_MODE_IN);
    SysCtlPeripheralEnable(cui.QEI_BASE);
    while (!SysCtlPeripheralReady(cui.QEI_BASE)){}

    if(cui.QEI_GPIO_PORT_BASE== GPIO_PORTD_BASE){
        HWREG(GPIO_PORTD_BASE+ GPIO_O_LOCK)= GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK)= 0;
    }

    GPIOPinconfigure(cui.QEI_GPIO_P_PHA |cui.QEI_GPIO_P_PHB| cui.QEI_GPIO_P_IDX);
    GPIOPinTypeQEI(cui.QEI_GPIO_PORT_BASE, cui.PHA_GPIO_PIN| cui.PHB_GPIO_PIN| cui.IDX_GPIO_PIN);
    QEIDisable(cui.QEI_BASE);
    QEIVelocityDisable(cui.QEI_BASE);
    QEIIntDisable(cui.QEI_BASE, (QEI_INTERROR| QEI_INTDIR| QEI_INTTIMER|QEI_INTINDEX));
    
    QEIConfigure(cui.QEI_BASE, (QEI_CONFIG_CAPTURE_A_B| QEI_CONFIG_RESET_IDX| QEI_CONFIG_QUADRATURE|QEI_CONFIG_NO_SWAP), 3999);
    QEIVelocityConfigure(cui.QEI_BASE, cui.QEI_VELDIV_!, 400000);
    QEIPositionSet(cui.QEI_BASE, 0);

    QEIEnable(cui.QEI_BASE);
    QEIVelocityEnable(cui.QEI_BASE);
}

int32_t getDirection(CUI_inc_encoder cui){
    int32_t i32QEIDirection= QEIDirectionGet(cui.QEI_BASE);
    return i32QEIDirection;
}
uint32_t getVelocity(CUI_inc_encoder cui){
    uint32_t ui32QEIVelocity= QEIVelocityGet(cui.QEI_BASE);
    return ui32QEIVelocity;
}

uint32_t getPosition(CUI_inc_encoder cui){
    uint32_t ui32QEIPosition= QEIPositionGet(cui.QEI_BASE);
    return ui32QEIPosition;
}

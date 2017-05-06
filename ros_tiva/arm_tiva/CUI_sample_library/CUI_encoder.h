#ifndef __CUI_ENCODER_H_
#define __CUI_ENCODER_H_
#include "tiva_lib_includes.h"

typedef struct CUI_encoders {
    uint32_t PHA_GPIO_PIN;//ex: GPIO_PIN_3
    uint32_t PHB_GPIO_PIN;//ex: GPIO_PIN_3
    uint32_t IDX_GPIO_PIN;//ex: GPIO_PIN_3
    
    uint32_t QEI_SYSCTL_PERIPH_GPIO;//ex: SYSCTL_PERIPH_GPIOD

    uint32_t QEI_GPIO_P_PHA;//ex: GPIO_PD6_PHA0
    uint32_t QEI_GPIO_P_PHB;//externx: GPIO_PD7_PHB0
    uint32_t QEI_GPIO_P_IDX;//ex: GPIO_PD3_IDX0

    uint32_t QEI_SYSCTL_PERIPH_QEI; //ex :SYSCTL_PERIPH_QEI0
    uint32_t QEI_GPIO_PORT_BASE;//ex: GPIO_PORTF_BASE
    uint32_t QEI_BASE; //externxx: QEI0_BASE
    uint32_t QEI_VELDIV;//ex: QEI_VELDIV_1
} CUI_encoder;

// Initialize everything needed for the encoder to run
void CUI_encoder_init(CUI_encoder cui);
//Read encoder values
int32_t getDirection(CUI_encoder cui);
uint32_t getVelocity(CUI_encoder cui);
uint32_t getPosition(CUI_encoder cui);

#endif //__CUI_ENCODER_H_


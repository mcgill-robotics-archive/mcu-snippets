
#include "tiva_lib_includes.h"

#ifndef CUI_INC_ENCODER_H
#define CUI_INC_ENCODER_H

typedef struct {
    uint32_t PHA_GPIO_PIN//ex: GPIO_PIN_3
    uint32_t PHB_GPIO_PIN//ex: GPIO_PIN_3
    uint32_t IDX_GPIO_PIN//ex: GPIO_PIN_3
    
    uint32_t QEI_SYSCTL_PERIPH_GPIO//ex: SYSCTL_PERIPH_GPIOD

    uint32_t QEI_GPIO_P_PHA//ex: GPIO_PD6_PHA0
    uint32_t QEI_GPIO_P_PHB//externx: GPIO_PD7_PHB0
    uint32_t QEI_GPIO_P_IDX//ex: GPIO_PD3_IDX0

    uint32_t QEI_GPIO_PORT_BASE//ex: GPIO_PORTF_BASE
    uint32_t QEI_BASE //externxx: QEI0_BASE
    uint32_t QEI_VELDIV//ex: QEI_VELDIV_1
} CUI_inc_encoder;

// Initialize everything needed for the encoder to run
extern void CUI_encoder_init(CUI_inc_encoder cui);
//Read encoder values
extern int32_t getDirection(CUI_inc_encoder cui);
extern uint32_t getVelocity(CUI_inc_encoder cui);
extern uint32_t getPosition(CUI_inc_encoder cui);

#endif

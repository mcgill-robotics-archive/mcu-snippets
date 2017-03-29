/*******************************************************************************
 *                          CUI pin configurations   			               *
 ******************************************************************************/

#include "tiv_lib_includes.h"

#ifndef CUI_encoder_tiva_pins_configure_h
#define CUI_encoder_tiva_pins_configure_h

#define QEI_pin_PHA_config					//ex: PF3
#define QEI_pin_PHB_config					//ex: PF3
#define QEI_pin_IDX_config					//ex: PF3

#define QEI_SYSCTL_PERIPH_GPIO				//ex: SYSCTL_PERIPH_GPIOD
#define QEI_GPIO_PHA_pin 					//ex: GPIO_PIN_3
#define QEI_GPIO_PHB_pin 					//ex: GPIO_PIN_3
#define QEI_GPIO_IDX_pin 					//ex: GPIO_PIN_3

#define QEI_GPIO_P_PHA						//ex: GPIO_PD6_PHA0
#define QEI_GPIO_P_PHB						//ex: GPIO_PD7_PHB0
#define QEI_GPIO_P_IDX						//ex: GPIO_PD3_IDX0

#define QEI_GPIO_base						//ex: GPIO_PORTF_BASE
#define QEI_base 							//ex: QEI0_BASE
#define QEI_VELDIV							//ex: QEI_VELDIV_1

#endif
/*******************************************************************************
 *                          DRV8842 pin configurations  		               *
 ******************************************************************************/

#include "tiv_lib_includes.h"

#ifndef DRV8842_tiva_pins_configure_h
#define DRV8842_tiva_pins_configure_h
//sleep is set to high in hardware

//PWM
#define PWM_pin_config					//ex: PF3
#define PWM_SYSCTL_PERIPH_PWM			//ex: SYSCTL_PERIPH_PWM0
#define PWM_SYSCTL_PWMDIV			 	//ex: SYSCTL_PWMDIV_8
#define PWM_pin_config					//ex: GPIO_PF3_M1PWM7
#define PWM_GPIO_base					//ex: GPIO_PORTF_BASE
#define PWM_GPIO_pin 					//ex: GPIO_PIN_3
#define PWM_base 						//ex: PWM1_BASE
#define PWM_out							//ex: PWM_OUT_7
#define PWM_out_bit						//ex: PWM_OUT_7_BIT
#define PWM_generator					//ex: PWM_GEN_3

//INA for direction
#define INA_pin_config					//ex: PA0
#define INA_SYSCTL_PERIPH_GPIO			//ex: SYSCTL_PERIPH_GPIOA
#define INA_GPIO_base					//ex: GPIO_PORTA_BASE
#define INA_GPIO_pin					//ex: GPIO_PIN_0
#define INA_GPIO_value					//ex: 0xFF for high 0x00 for low

//Reset for the board
#define RESET_pin_config				//ex: PD1
#define RESET_SYSCTL_PERIPH_GPIO		//ex: SYSCTL_PERIPH_GPIOD
#define RESET_GPIO_base					//ex: GPIO_PORTD_BASE
#define RESET_GPIO_pin					//ex: GPIO_PIN_0
#define RESET_GPIO_value				//ex: 0xFF for high 0x00 for low

//Fault in the board
#define FAULT_pin_config				//ex: PA1
#define FAULT_SYSCTL_PERIPH_GPIO		//ex: SYSCTL_PERIPH_GPIOD
#define FAULT_GPIO_base					//ex: GPIO_PORTD_BASE
#define FAULT_GPIO_pin					//ex: GPIO_PIN_1
#define FAULT_GPIO_value				//ex: 0xFF for high 0x00 for low

//Current sense configuration
//TODO: check Current sense config
#define CURRENTSENSE_pin_config			//ex: PA1
#define CURRENTSENSE_SYSCTL_PERIPH_ADC	//ex: SYSCTL_PERIPH_GPIOD
#define CURRENTSENSE_GPIO_base			//ex: GPIO_PORTD_BASE
#define CURRENTSENSE_GPIO_pin			//ex: GPIO_PIN_0
#define CURRENTSENSE_ADC_base			//ex: ADC0_BASE
#define CURRENTSENSE_ADC_sequence		//ex: 0, 1, 2, or 3

//Brake for the brake mosfet
//TODO: check this
#define BRAKE_pin_config				//ex: PA1
#define BRAKE_SYSCTL_PERIPH_GPIO		//ex: SYSCTL_PERIPH_GPIOD
#define BRAKE_GPIO_base					//ex: GPIO_PORTD_BASE
#define BRAKE_GPIO_pin					//ex: GPIO_PIN_0
#define BRAKE_GPIO_value				//ex: 0xFF for high 0x00 for low

#endif
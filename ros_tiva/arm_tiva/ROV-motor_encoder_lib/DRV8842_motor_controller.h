/*******************************************************************************
 *                          		DRV8842 DC     			                   *
 ******************************************************************************/

#ifndef DRV8842_motor_controller_h
#define DRV8842_motor_controller_h

typedef struct {
	volatile int32_t RotationDirection; 
	volatile uint32_t RotationVelocity;
	volatile uint32_t motor_current_sense;
	extern boolean brake;
	extern boolean fault;
} DRV8842_Motor;

//FUNCTION PROTOTYPES
extern void Initialize_DRV8842_motor(void);
extern void DRV8842_motor_run(int32_t RotationDirection, uint32_t RotationVelocity);
extern uint32_t getCurrent(void);

#endif
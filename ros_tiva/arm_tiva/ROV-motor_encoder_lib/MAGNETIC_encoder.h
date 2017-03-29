
#ifndef MAGNETIC_encoder_h
#define MAGNETIC_encoder_h

typedef struct {
	volatile int32_t MAGNETIC_abs_direction;
	volatile uint32_t MAGNETIC_abs_velocity;
	volatile uint32_t MAGNETIC_abs_position;
	volatile uint32_t MAGNETIC_abs_InitialPosition;
} MAGNETIC_abs_encoder;

extern int32_t getABSDirection();
extern uint32_t getABSVelocity();
extern uint32_t getABSPosition();

#endif
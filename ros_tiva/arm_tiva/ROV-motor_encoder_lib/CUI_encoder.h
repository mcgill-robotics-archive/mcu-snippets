
#ifndef CUI_inc_encoder_h
#define CUI_inc_encoder_h

typedef struct {
	volatile int32_t CUI_inc_direction;
	volatile uint32_t CUI_inc_velocity;
	volatile uint32_t CUI_inc_position;
} CUI_inc_encoder;

extern int32_t getINCDirection();
extern uint32_t getINCVelocity();
extern uint32_t getINCPosition();

#endif
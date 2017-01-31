#include "arm_controller.h"

void position_change_to(double degree, int direction, double speedPercentage) {
	uint32_t ui32QEIPositionInitial = getPosition();
	uint32_t ui32QEIPosition = getPosition();
	uint32_t tick_to_move_to= ((4000*degree/360)+ui32QEIPositionInitial);
	while ( ui32QEIPosition !=tick_to_move_to){
		motor_run(direction, speedPercentage);
		ui32QEIPosition = getPosition();
	}
	motor_stop();
}

void pos_loop_test(){
	uint32_t ref_pos = 20;
	uint32_t increment = 10;
	uint32_t current_pos = getPosition(); //fictional function from the QEI module
	fp = fopen("/test_files/pos_test.csv", "w+");
	pos_PI(current_pos, ref_pos);
	ref_pos += increment;
	current_pos = getPosition();
	fprintf(fp, "%d, %d\n", ref_pos, current_pos);
}

uint32_t getSetPosition(){
	return 20;
}


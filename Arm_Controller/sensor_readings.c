#include "arm_controller.h"


uint32_t getPosition(void){
	return ui32QEIPosition = QEIPositionGet(QEI0_BASE);
}

uint32_t getVelocity(void){
	ui32QEIVelocity = QEIVelocityGet(QEI0_BASE);
	return ui32QEIVelocity;
}

uint32_t getDirection(void){
	return ui32QEIDirection = QEIDirectionGet(QEI0_BASE);
}

uint32_t getCurrent(void){

	// Clear the interrupt status flag.  This is done to make sure the
	// interrupt flag is cleared before we sample.
	ADCIntClear(ADC0_BASE, 3);

	// Sample AIN0 forever
	while(1) {
		// Trigger the ADC conversion and wait for conversion to be completed.
		ADCProcessorTrigger(ADC0_BASE, 3);
		while(!ADCIntStatus(ADC0_BASE, 3, false)) {}

		// Clear the ADC interrupt flag.
		ADCIntClear(ADC0_BASE, 3);

		// Read ADC Value.
		ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);

		toFileADCRead = pui32ADC0Value[0];
		// Delay for certain amount of time.
		SysCtlDelay(30);
		return toFileADCRead;
	}
}

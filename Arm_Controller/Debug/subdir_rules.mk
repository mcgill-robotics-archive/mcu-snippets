################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configuration.obj: ../configuration.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="configuration.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

loop_tests.obj: ../loop_tests.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="loop_tests.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

motor_run.obj: ../motor_run.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="motor_run.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

sensor_readings.obj: ../sensor_readings.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="sensor_readings.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/er/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --include_path="/home/er/ti/SW-TM4C-2.1.3.156" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


// Includes
#include <stdbool.h>
#include <stdint.h>

// TivaC specific includes
extern "C"
{
  #include <driverlib/sysctl.h>
  #include <driverlib/gpio.h>
  #include <driverlib/pwm.h>
  #include "inc/hw_memmap.h"
}

// ROS includes
#include <ros.h>
#include <std_msgs/Int32.h>

// ROS nodehandle
ros::NodeHandle nh;

void messageCb(const std_msgs::Int32& msg) {
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, msg.data);
}

ros::Subscriber<std_msgs::Int32> sub("motor_test_1", &messageCb);

void init_pwm(void)
{
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
	GPIOPinConfigure(GPIO_PF3_M1PWM7);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
					PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 10000);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,9999 );
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

int main(void) {
  MAP_FPUEnable();
  MAP_FPULazyStackingEnable();
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
  init_pwm();

  nh.initNode();
  nh.subscribe(sub);

  while (1)
  {
    nh.spinOnce();
    nh.getHardware()->delay(100);
  }
}

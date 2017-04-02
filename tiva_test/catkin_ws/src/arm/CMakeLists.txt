cmake_minimum_required(VERSION 2.8.3)
project(arm)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  rosserial_client
  rosserial_tivac
  std_msgs
)

rosserial_generate_ros_lib(
  PACKAGE rosserial_tivac
  SCRIPT make_libraries_tiva
)

# Chatter tutorial
rosserial_configure_client(
  DIRECTORY arm_main
  TOOLCHAIN_FILE ${ROSSERIAL_TIVAC_TOOLCHAIN}
)
rosserial_add_client_target(arm_main arm_main.axf ALL)
rosserial_add_client_target(arm_main flash)
rosserial_add_client_target(arm_main size)
rosserial_add_client_target(arm_main dump)

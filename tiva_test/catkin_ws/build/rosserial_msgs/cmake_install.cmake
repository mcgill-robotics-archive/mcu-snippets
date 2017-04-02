# Install script for directory: /home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/steven/Documents/Robotics/tiva_test/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs/msg" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/msg/Log.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/msg/TopicInfo.msg"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs/srv" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/srv/RequestParam.srv"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/srv/RequestMessageInfo.srv"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/srv/RequestServiceInfo.srv"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs/cmake" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_msgs/catkin_generated/installspace/rosserial_msgs-msg-paths.cmake")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/include/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/roseus/ros/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/common-lisp/ros/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/gennodejs/ros/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python" -m compileall "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/lib/python2.7/site-packages/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/lib/python2.7/site-packages/rosserial_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_msgs/catkin_generated/installspace/rosserial_msgs.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs/cmake" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_msgs/catkin_generated/installspace/rosserial_msgs-msg-extras.cmake")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs/cmake" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_msgs/catkin_generated/installspace/rosserial_msgsConfig.cmake"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_msgs/catkin_generated/installspace/rosserial_msgsConfig-version.cmake"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_msgs" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_msgs/package.xml")
endif()


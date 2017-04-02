# Install script for directory: /home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/visualization_msgs/msg" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/ImageMarker.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarker.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarkerControl.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarkerFeedback.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarkerInit.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarkerPose.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/InteractiveMarkerUpdate.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/MarkerArray.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/Marker.msg"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/msg/MenuEntry.msg"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/visualization_msgs/cmake" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/common_msgs/visualization_msgs/catkin_generated/installspace/visualization_msgs-msg-paths.cmake")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/include/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/roseus/ros/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/common-lisp/ros/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/share/gennodejs/ros/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python" -m compileall "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/lib/python2.7/site-packages/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/site-packages" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/devel/lib/python2.7/site-packages/visualization_msgs")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/common_msgs/visualization_msgs/catkin_generated/installspace/visualization_msgs.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/visualization_msgs/cmake" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/common_msgs/visualization_msgs/catkin_generated/installspace/visualization_msgs-msg-extras.cmake")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/visualization_msgs/cmake" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/common_msgs/visualization_msgs/catkin_generated/installspace/visualization_msgsConfig.cmake"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/common_msgs/visualization_msgs/catkin_generated/installspace/visualization_msgsConfig-version.cmake"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/visualization_msgs" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/common_msgs/visualization_msgs/package.xml")
endif()


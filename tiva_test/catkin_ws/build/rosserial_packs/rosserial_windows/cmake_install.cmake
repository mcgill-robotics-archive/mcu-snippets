# Install script for directory: /home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_windows

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_packs/rosserial_windows/catkin_generated/installspace/rosserial_windows.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_windows/cmake" TYPE FILE FILES
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_packs/rosserial_windows/catkin_generated/installspace/rosserial_windowsConfig.cmake"
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_packs/rosserial_windows/catkin_generated/installspace/rosserial_windowsConfig-version.cmake"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_windows" TYPE FILE FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_windows/package.xml")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_windows/src" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_windows/src/ros_lib")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_windows/src" TYPE DIRECTORY FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_windows/src/examples")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rosserial_windows" TYPE PROGRAM FILES "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_windows/src/rosserial_windows/make_libraries.py")
endif()


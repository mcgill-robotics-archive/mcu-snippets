#!/bin/bash

set -e


echo "Installing gcc-arm-embedded..."
sudo add-apt-repository -y ppa:team-gcc-arm-embedded/ppa
sudo apt-get update -qqq
sudo apt-get install -qqq  gcc-arm-embedded
echo "Done"

if [[ ! -e /usr/local/bin/lm4flash ]]; then
    echo "Cloning and install lm4flash tool..."
    if [[ -f lm4tools ]]; then
        rm -rf lm4tools
    fi
    
    git clone -qq https://github.com/utzig/lm4tools.git
    cd lm4tools/lm4ﬂash
    make
    sudo cp lm4flash /usr/local/bin
    rm -rf ../../lm4tools
    
    echo "Done"
fi
if [[ ! -e /etc/udev/rules.d/10-tiva.rules ]]; then
    echo "Adding rules for USB devices..."
    sudo ln -s $(dirname $(readlink -f $0))/10-tiva.rules \
        /etc/udev/rules.d/10-tiva.rules
    sudo udevadm control --reload
    sudo udevadm trigger
    echo "Done"
fi

if [[ ! -d ${ROBOTIC_PATH}/tivaware/ ]]; then
    echo "Installing TivaWare..."
    cd ${ROBOTIC_PATH}
    git clone https://github.com/mcgill-robotics/tivaware.git
fi

echo "Installing rosserial and rosserial_tivac from ROS repositories..."
sudo apt-get install ros-kinetic-rosserial ros-kinetic-rosserial-msgs ros-kinetic-rosserial-client ros-kinetic-rosserial-python ros-kinetic-rosserial-tivac
echo "Done"

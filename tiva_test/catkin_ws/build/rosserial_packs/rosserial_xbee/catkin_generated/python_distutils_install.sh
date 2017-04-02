#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_xbee"

# snsure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/steven/Documents/Robotics/tiva_test/catkin_ws/install/lib/python2.7/site-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/steven/Documents/Robotics/tiva_test/catkin_ws/install/lib/python2.7/site-packages:/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/lib/python2.7/site-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/steven/Documents/Robotics/tiva_test/catkin_ws/build" \
    "/usr/bin/python" \
    "/home/steven/Documents/Robotics/tiva_test/catkin_ws/src/rosserial_packs/rosserial_xbee/setup.py" \
    build --build-base "/home/steven/Documents/Robotics/tiva_test/catkin_ws/build/rosserial_packs/rosserial_xbee" \
    install \
    $DESTDIR_ARG \
     --prefix="/home/steven/Documents/Robotics/tiva_test/catkin_ws/install" --install-scripts="/home/steven/Documents/Robotics/tiva_test/catkin_ws/install/bin"

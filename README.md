# rosserial

[![Build Status](https://travis-ci.org/ros-drivers/rosserial.svg?branch=melodic-devel)](https://travis-ci.org/ros-drivers/rosserial)

Please see [rosserial on the ROS wiki](http://wiki.ros.org/rosserial) to get started.

## Using the code
### ROS-side
launch the roscore：

<code>
roscore
  
rosrun rosserial_python serial_node.py /dev/ttyUSB0
</code>

need a publisher to public "cmd_vel" topic

Alternatively, if you want the ability to programmatically reset your Arduino, run using:

<code>rosrun rosserial_arduino serial_node.py _port:=/dev/ttyUSB0</code>

This will automatically provide a service endpoint at ~reset_arduino that you can call which will have the same effect as pressing the Arduino's reset button.

### Arduino-side
flush the arduino_twist_subscriber into arduino MEGA 2560

We are using ESC to control brushless or brushed DC motor's speed, and no encoder attached.

Pin define

left wheel pin 6
right wheel pin 7


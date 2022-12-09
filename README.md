# ros_keyboard_interface

ROS generic interface for keyboard control. The interface takes input for the keyboard and it translates into analog command velocity and button presses.

The keys configured for the analog control are:
- 'w': forward (linear velocity along positive x-axis)
- 'd': backward (linear velocity along negative x-axis)
- 'a': left (linear velocity along positive y-axis)
- 'd': right (linear velocity along negative y-axis)
- 'q': turn left (angular velocity along positive z-axis)
- 'e': turn right (angular velocity along negative z-axis)
- ESC: quit the control

Additionaly, four discrete commands are set for the following keys:
- 'u': button 0
- 'i': button 1
- 'o': button 2
- 'p': button 3

## Nodes
### Published Topics:
`/cmdvel` ([geometry_msgs::Twist](http://docs.ros.org/en/noetic/api/geometry_msgs/html/msg/Twist.html))  
Analog velocity control [-1.0 1.0] m/s

`/buttons` ([std_msgs::UInt8MultiArray](http://docs.ros.org/en/api/std_msgs/html/msg/UInt8MultiArray.html))  
Discrete button pressed

### Parameters
`~increment` (double, default: 0.05)  
How much the corresponding velocity is incremented while keep pressing a key

`reset_input` (bool, default: false)  
If true, the velocity will be set to 0 as soon as the key is not pressed any longer

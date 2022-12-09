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
''
/cmdvel
''

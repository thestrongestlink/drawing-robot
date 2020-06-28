# drawing-robot
An example in controlling the Zortrax Robot Arm in a Cartesian Coordinate System.

![ultrasonic-jetbot](https://i0.wp.com/thestrongestlink.nl/wp-content/uploads/2020/05/robotarm_small.png?resize=1200%2C1500&ssl=1?raw=true)

The 3-D printable Robot Arm from Zortrax offers a very nice start into the world of robotic experimentation, and offers plenty to learn on interfacing stepper motors, stepper motor drivers and controllers to make a working system. In an effort to step away from G-codes to control the individual stepper motors, I've written a short function that allows for controlled movement of the robot arm in a cartesian coordinate system.

This function then opens plenty of new opportunities to use the robot arm to play chess with you, move stuff around, help you with soldering and making pen drawings or paintings. I've included a short sample code that will draw a circle and a rectangle on a vertical plane, you can easily modify and expand from there to make your Zortrax robot arm do what you have in mind.

This repository contains:

* Sample demo source code in C for an Arduino microcontroller drawing a circle and a square in the cartesian space,
* A sample source code of the cartesian to spherical transformation function in C.

A complete write-up of the development of the drawing robot can be found on my [blog](https://thestrongestlink.nl/2020/05/23/to-step-or-not-to-step/).

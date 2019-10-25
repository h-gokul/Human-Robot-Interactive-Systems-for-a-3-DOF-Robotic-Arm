# Imitative-Control-Of-a-Robotic-Arm
Human-Computer interactive systems designed to control a 3 Degree of Freedom Robotic Arm
Contains a 
- Glove based system: IMU SensorGlove
- Attribute Based motion Tracking

Please Read Project Report for Mathematical Forumulation and Experimentation Details.

# Hardware Requirements


## Glove based system requires 
Two IMU sensors, they are :  
#### MPU 9250 
Contains accelerometer, gyrometer and magnetometer in all 3 axis 
Data from these inbuilt sensors are run through a sensor fusion algorithm to obtain Euler Angles, yaw, pitch and roll. The sensor glove uses yaw and pitch values to actuate the Servo of the robotic arm. Use of magnetometer in this device solves the yaw drift that occurs in IMU. Mounted in the wrist region of a glove.

#### MPU 6050 
Contains accelerometer and gyrometer  in all 3 axis 
Data from these inbuilt sensors are run through a sensor fusion algorithm to obtain Euler Angles, pitch and roll. The sensor glove uses only pitch values to actuate the Servo of the robotic arm. It is mounted in the finger region of the glove. 

## Attribute Based motion Tracking requires 
#### A camera.
Data from the images obtained for every frame is passed through the algorithm to detect and track the 3D dimensional motion of handheld stick 

#### Handheld stick with red-circular/ spherical notch. 

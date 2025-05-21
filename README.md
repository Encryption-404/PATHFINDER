This repository is designed for a drone project that employs SLAM (Simultaneous Localization and Mapping) algorithms to enable autonomous mapping and navigation in various environments. The system runs on ROS Noetic, the final release of ROS 1, known for its robustness and extensive community support. By utilizing SLAM, the drone can construct a real-time map of its surroundings while determining its location within the map, enabling precise and efficient autonomous operation.

![raspberry](https://github.com/user-attachments/assets/0f15ca7b-f831-47a2-9168-430ff81eeb69)

Now for the  LDROBOT D500 (LD19 or LD500) lidar is an affordable, lightweight 2D lidar suitable for applications like SLAM, navigation, and obstacle detection. It features a 360° scanning range, high-speed data acquisition (up to 12,000 points/sec), and good accuracy for mapping and localization. Its compact design makes it ideal for small robotics projects, and it is compatible with ROS, allowing integration with SLAM algorithms like Cartographer and Hector SLAM. Additionally, its USB communication makes it easy to set up with devices like the Raspberry Pi.

![lidar](https://github.com/user-attachments/assets/a0d82a6d-dfd1-4108-b8c8-f5dbef295172)


  The lidar to raspberry pi connection is via usb which makes it more versatile 
  
![rasp+lidar](https://github.com/user-attachments/assets/1fc4c997-416d-479d-8e99-bb57e97ec099).

The drone not only include the mapping part but also human presence detection using the wifi camera.
For the drone i used a toy brushless dorne and modified it to a frame.since the drone have a camera i used its ip address to get the video data.
the camera board looks like this
![WhatsApp Image 2025-05-21 at 03 39 18_1bde1aed](https://github.com/user-attachments/assets/5e0c90ab-bb52-41c2-96ae-66c2d7b51e19)
After that i faced the issue of weight so i used 2 drones and made a X8 copter.

![WhatsApp Image 2025-05-21 at 03 41 24_b97cd2c4](https://github.com/user-attachments/assets/e52281d3-c419-41c6-811b-d5a6e3454087)

To use this we have to install ros noetic distro and we will be using hector slam mapping the files are included above. install all this follow the step in the installation guide of ros and hector mapping.
after that modify the launch files to there corresponding locations for the human detection part we have to run the Detection.py file in the vs code to do so you have to install opencv,numpy etc for the voice or so


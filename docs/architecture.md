# Don Quichotte USV Architecture Overview

## Overview

---

The Don Quichotte Unmanned Surface Vehicle (USV) project aims to develop a fully autonomous USV capable of inspecting,
identifying, and maintaining offshore wind turbines.  
The project involves multiple ROS 2 packages that work together to
manage the USV's mission, perception, navigation, hardware control, and communication with an external platform.

## 1. **usv_mission_control package** (Mission Management and Orchestration)

#### **Description**:
This package remains responsible for mission lifecycle management. It handles transitions between different mission
phases (inspection, identification, maintenance, etc.), interacts with navigation and perception nodes, and manages
communication with the platform.

#### **Nodes**:
- **`mission_node`**: This node manages the mission, directs the navigation module to move the USV between waypoints
- (e.g., wind turbines), and receives updates from the perception system.

#### **Topics**:
- **Subscribed**:
  - `/platform/faulty_turbine_id` `[std_msgs/String]`: Receives faulty turbine ID from the platform.
  - `/turbine/detected` `[geometry_msgs/PoseStamped]`: Receives detected turbine location.
  - `/obstacle/detected` `[geometry_msgs/PoseStamped]`: Receives obstacle location from the perception system.
  - `/qrcode/data` `[std_msgs/String]`: Receives QR code data after scanning.

- **Published**:
  - `/mission/status` `[std_msgs/String]`: Publishes the current mission phase.
  - `/cmd_vel` `[geometry_msgs/Twist]`: Sends navigation commands for the guidance and control module (heading, speed).

#### **Services**:
- **`/mission/start`** `[std_srvs/Trigger]`: Starts the mission.
- **`/mission/stop`** `[std_srvs/Trigger]`: Stops the mission.
- **`/mission/pause`** `[std_srvs/Trigger]`: Pauses the mission.

#### **Actions**:
- **`move_to_waypoint`** (custom action): Directs the USV to navigate to specified GPS waypoints (e.g., wind turbines).

#### **Relation**:
The **`mission_node`** controls the flow of the mission, using the guidance and control systems for navigation and the
perception system to identify turbines and QR codes.

---

## 2. **usv_perception package** (Camera and Sensor Data Processing)

#### **Description**:
This package processes the camera's visual input to detect wind turbines and scan QR codes. It also integrates data
from the IMU to keep track of the vehicle’s orientation and from the GPS for global positioning.

#### **Nodes**:
- **`qr_code_scanner`**: Decodes QR codes using camera data to identify each wind turbine.
- **`turbine_detector`**: Uses camera images and orientation information (IMU) to detect and localize wind turbines.
- **`obstacle_detector`**: Detects obstacles in the USV's path using camera data.

#### **Topics**:
- **Subscribed**:
  - `/camera/image_raw` `[sensor_msgs/Image]`: Receives raw camera feed for QR code scanning and turbine detection.
  - `/imu/data` `[sensor_msgs/Imu]`: Receives IMU data for orientation tracking.
  - `/gps/fix` `[sensor_msgs/NavSatFix]`: Receives GPS data for global positioning.

- **Published**:
  - `/qrcode/data` `[std_msgs/String]`: Publishes decoded QR code information.
  - `/turbine/detected` `[geometry_msgs/PoseStamped]`: Publishes detected wind turbine positions based on visual data
  - and sensor fusion.

#### **Services**:
- **`/qrcode/scan`** `[std_srvs/Trigger]`: Initiates QR code scanning when the USV is in position near a wind turbine.

#### **Relation**:
- **`qr_code_scanner`** works closely with the **`turbine_detector`** to process visual data and match it with
positional data (GPS) and orientation (IMU).
- The **`turbine_detector`** helps localize wind turbines and supports the mission node in managing USV movements.

---

## 3. **usv_guidance_and_control package** (Navigation, Path Planning, and Control)

#### **Description**:
This package is responsible for ensuring accurate navigation using the available IMU, GPS, and camera data. It handles
the motor and rudder control to ensure precise heading and speed.

#### **Nodes**:
- **`path_planner`**: Plans the route from the current GPS location to the next waypoint. It avoids obstacles detected
by the camera and adjusts based on IMU data for accurate heading.
- **`controller_node`**: Translates high-level navigation commands (heading and speed) into motor and rudder control
commands for both left and right sides.

#### **Topics**:
- **Subscribed**:
  - `/turbine/detected` `[geometry_msgs/PoseStamped]`: Receives turbine positions for accurate path planning.
  - `/imu/data` `[sensor_msgs/Imu]`: Receives orientation data to keep the USV aligned on its path.
  - `/gps/fix` `[sensor_msgs/NavSatFix]`: Receives GPS data to track global position.

- **Published**:
  - `/cmd_vel` `[geometry_msgs/Twist]`: Sends velocity commands (linear and angular) to control both motors and rudders.
  - `/goal_reached` `[std_msgs/Bool]`: Publishes when the USV reaches a waypoint.

#### **Services**:
- **`/stabilize_at_position`** `[std_srvs/Trigger]`: Engages rudder and motor controls to stabilize the USV in front
of a turbine during the maintenance phase.

#### **Actions**:
- **`move_to_waypoint`** (custom action): Handles moving the USV to a specified GPS waypoint, adjusting heading and
speed based on sensor data.

#### **Relation**:
- **`path_planner`** generates navigation commands that are then executed by the **`controller_node`**, which adjusts
the motors and rudders accordingly.
- The **`controller_node`** constantly receives updates from the IMU and GPS to adjust the USV’s heading and position
in real-time.

---

## 4. **usv_hardware_interface package** (Motor and Rudder Control, Sensor Drivers)

#### **Description**:
This package interfaces directly with the USV’s actuators (motors, rudders) and sensors (IMU, GPS, camera). It collects
sensor data, converts control commands into motor and rudder actuation, and provides feedback to higher-level modules.

#### **Nodes**:
- **`motor_driver`**: Controls left and right motors based on velocity commands.
- **`rudder_driver`**: Controls left and right rudders to adjust heading.
- **`sensor_interface`**: Publishes data from the GPS and IMU sensors.

#### **Topics**:
- **Subscribed**:
  - `/cmd_vel` `[geometry_msgs/Twist]`: Receives velocity commands (linear and angular) to adjust the motors and
rudders.

- **Published**:
  - `/gps/fix` `[sensor_msgs/NavSatFix]`: Publishes the GPS position of the USV.
  - `/imu/data` `[sensor_msgs/Imu]`: Publishes IMU data (orientation, acceleration).

#### **Services**:
- **`/set_motor_speed`** `[std_srvs/SetBool]`: Sets motor speed or stops the motors.

#### **Relation**:
- **`motor_driver`** and **`rudder_driver`** receive high-level commands from the guidance system, converting them
into the necessary low-level control for the motors and rudders.
- **`sensor_interface`** ensures the latest sensor data is available for navigation and mission management.

---

## 5. **usv_communication package** (Platform Communication)

#### **Description**:
Handles communication between the USV and the external platform, including receiving mission-critical information
(faulty turbine ID) and sending mission status and QR code data.

#### **Nodes**:
- **`platform_comm`**: Manages all communication with the external platform.

#### **Topics**:
- **Subscribed**:
  - `/qrcode/data` `[std_msgs/String]`: Receives QR code data for transmission to the platform.

- **Published**:
  - `/platform/faulty_turbine_id` `[std_msgs/String]`: Publishes the ID of the faulty turbine sent by the platform.

#### **Services**:
- **`/send_qr_data`** `[std_srvs/Trigger]`: Sends QR code data to the platform after each successful scan.

---

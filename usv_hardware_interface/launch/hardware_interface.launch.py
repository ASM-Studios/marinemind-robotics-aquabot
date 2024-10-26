# Copyright 2024 MarineMindRobotics

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    ld = LaunchDescription()

    # Start the sensor_interface node
    sensor_interface_node = Node(
        package='usv_hardware_interface',
        executable='sensor_interface_node',
        name='sensor_interface_node',
        output='screen'
    )

    camera_control_server_node = Node(
        package='usv_hardware_interface',
        executable='camera_control_server_node',
        name='camera_control_server_node',
        output='screen'
    )

    ld.add_action(sensor_interface_node)
    ld.add_action(camera_control_server_node)

    return ld

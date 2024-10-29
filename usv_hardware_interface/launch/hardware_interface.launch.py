# This file is part of the MarineMindRobotics project.
#
# Copyright (C) 2024 MarineMindRobotics
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

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

    machinery_node = Node(
        package='usv_hardware_interface',
        executable='machinery_node',
        name='machinery_node',
        output='screen'
    )

    ld.add_action(sensor_interface_node)
    ld.add_action(camera_control_server_node)
    ld.add_action(machinery_node)

    return ld

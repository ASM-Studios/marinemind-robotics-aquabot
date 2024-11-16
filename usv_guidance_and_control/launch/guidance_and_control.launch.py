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

import os
from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration,ThisLaunchFileDir
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    # Start the sensor_interface node
    path_planner_node = Node(
        package='usv_guidance_and_control',
        executable='path_planner',
        name='path_planner_node',
        output='screen'
    )
    ld.add_action(path_planner_node)

    costmap_params_file = LaunchConfiguration('costmap_params_file', default=os.path.join('/config/vrx_ws/src/usv_guidance_and_control', 'config', 'costmap_params.yaml'))

    """costmap_2d_node = Node(
        package='nav2_costmap_2d',
        executable='nav2_costmap_2d',
        name='costmap_2d_node',
        output='screen',
        parameters=[costmap_params_file],
        remappings=[('/costmap', '/static_costmap')]
    )
    ld.add_action(costmap_2d_node)

    nav2_bringup_node = Node(
        package='nav2_bringup',
        executable='nav2_bringup',
        name='nav2_bringup_node',
        output='screen',
    )
    ld.add_action(nav2_bringup_node)"""

    return ld

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

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    ld = LaunchDescription()

    world_arg = DeclareLaunchArgument(
        'world',
        default_value='aquabot_regatta',
        description='World name'
    )
    headless_arg = DeclareLaunchArgument(
        'headless',
        default_value='true',
        description='Run simulation headless (no GUI)'
    )
    competition_mode_arg = DeclareLaunchArgument(
        'competition_mode',
        default_value='false',
        description='Disable debug topics'
    )

    ld.add_action(world_arg)
    ld.add_action(headless_arg)
    ld.add_action(competition_mode_arg)

    world_name = LaunchConfiguration('world')
    headless = LaunchConfiguration('headless')
    competition_mode = LaunchConfiguration('competition_mode')

    aquabot_competition_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('aquabot_gz'),
                         'launch/competition.launch.py')
        ),
        launch_arguments={
            'world': world_name,
            'headless': headless,
            'competition_mode': competition_mode
        }.items()
    )

    usv_hardware_interface_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('usv_hardware_interface'),
                         'launch/hardware_interface.launch.py')
        ),
        launch_arguments={
            'world': world_name,
            'headless': headless,
            'competition_mode': competition_mode
        }.items()
    )

    usv_guidance_and_control_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('usv_guidance_and_control'),
                         'launch/guidance_and_control.launch.py')
        ),
        launch_arguments={
            'world': world_name,
            'headless': headless,
            'competition_mode': competition_mode
        }.items()
    )

    ld.add_action(aquabot_competition_launch_file)
    ld.add_action(usv_hardware_interface_launch_file)
    ld.add_action(usv_guidance_and_control_launch_file)

    return ld

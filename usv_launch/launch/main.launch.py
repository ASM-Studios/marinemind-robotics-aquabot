# Copyright 2024 MarineMindRobotics

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

    ld.add_action(aquabot_competition_launch_file)
    ld.add_action(usv_hardware_interface_launch_file)

    return ld

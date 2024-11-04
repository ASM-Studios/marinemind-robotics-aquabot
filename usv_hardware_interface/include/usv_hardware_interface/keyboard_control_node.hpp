/*
 * This file is part of the MarineMindRobotics project.
 *
 * Copyright (C) 2024 MarineMindRobotics
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <termios.h>
#include <cmath>
#include <unordered_map>
#include <string>

#include "usv_hardware_interface/srv/get_machinery_state.hpp"

const double MAX_THRUST = 5000.0; // Maximum value for motor thrust
const double MAX_POSITION = M_PI / 4; // Maximum value for motor position
const double THRUST_STEP = 5.0;
const double POSITION_STEP = M_PI / 36;

struct MachineryTopic
{
  std::string side;
  double value;
};

class keyboard_control_node : public rclcpp::Node
{
  void publish_instruction(const std::string & topic, const std::string & side, double value);
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr thrust_publisher_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr position_publisher_;
  rclcpp::Client<usv_hardware_interface::srv::GetMachineryState>::SharedPtr client_;

public:
  keyboard_control_node();
  void run();
};

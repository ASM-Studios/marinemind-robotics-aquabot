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

#include <chrono>
#include <functional>
#include <memory>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/float64.hpp"
#include "usv_hardware_interface/srv/get_machinery_state.hpp"

namespace usv_hardware_interface
{
struct MachineryState
{
  double left_position;
  double right_position;
  double left_thrust;
  double right_thrust;
};

struct MachineryTopic
{
  std::string side;
  double value;
};

const double MAX_THRUST = 5000.0;   // Maximum value for motor thrust
const double MAX_POSITION = M_PI / 4; // Maximum value for motor position

class MachineryNode : public rclcpp::Node
{
  rclcpp::TimerBase::SharedPtr _m_timer;

  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr _m_thrust_sub;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr _m_position_sub;

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _left_pos_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _left_thrust_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _right_pos_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _right_thrust_pub_;

  rclcpp::Service<usv_hardware_interface::srv::GetMachineryState>::SharedPtr
    _get_machinery_state_service;

  MachineryState _state;

  void _thrustInstructionCallback(const std_msgs::msg::Float64::SharedPtr msg);
  void _positionInstructionCallback(const std_msgs::msg::Float64::SharedPtr msg);

  void _publishThrust();
  void _publishPosition();

  void init_service();
  void get_machinery_state_callback(
    const std::shared_ptr<usv_hardware_interface::srv::GetMachineryState::Request> request,
    std::shared_ptr<usv_hardware_interface::srv::GetMachineryState::Response> response);

public:
  MachineryNode();
  ~MachineryNode() = default;

  void publishState();
};
}

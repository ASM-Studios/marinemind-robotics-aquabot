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

#include "machinery_node.hpp"
#include "usv_hardware_interface/srv/get_machinery_state.hpp"

namespace usv_hardware_interface
{
MachineryNode::MachineryNode()
: Node("machinery_node"), _state({0.0, 0.0, 0.0, 0.0})
{
  _left_pos_pub_ = this->create_publisher<std_msgs::msg::Float64>(
    "/aquabot/thrusters/left/pos",
    10);
  _left_thrust_pub_ =
    this->create_publisher<std_msgs::msg::Float64>("/aquabot/thrusters/left/thrust", 10);
  _right_pos_pub_ = this->create_publisher<std_msgs::msg::Float64>(
    "/aquabot/thrusters/right/pos",
    10);
  _right_thrust_pub_ =
    this->create_publisher<std_msgs::msg::Float64>("/aquabot/thrusters/right/thrust", 10);

  _m_thrust_sub = this->create_subscription<std_msgs::msg::String>(
    "/usv/thrusters/machinery", 10,
    std::bind(&MachineryNode::_thrustInstructionCallback, this, std::placeholders::_1));
  _m_position_sub = this->create_subscription<std_msgs::msg::String>(
    "/usv/thrusters/pos", 10,
    std::bind(&MachineryNode::_positionInstructionCallback, this, std::placeholders::_1));

  _m_timer = this->create_wall_timer(
    std::chrono::milliseconds(100),
    std::bind(&MachineryNode::publishState, this));

  init_service();
}

void MachineryNode::_thrustInstructionCallback(const std_msgs::msg::String::SharedPtr msg)
{
  MachineryTopic topic;
  std::istringstream iss(msg->data);
  iss >> topic.side >> topic.value;
  topic.value = std::clamp(topic.value, -MAX_THRUST, MAX_THRUST);

  if (topic.side == "left") {
    _state.left_thrust = topic.value;
  }
  if (topic.side == "right") {
    _state.right_thrust = topic.value;
  }
  if (topic.side == "both") {
    _state.left_thrust = topic.value;
    _state.right_thrust = topic.value;
  }
}

void MachineryNode::_positionInstructionCallback(const std_msgs::msg::String::SharedPtr msg)
{
  MachineryTopic topic;
  std::istringstream iss(msg->data);
  iss >> topic.side >> topic.value;
  topic.value = std::clamp(topic.value, -MAX_POSITION, MAX_POSITION);

  if (topic.side == "left") {
    _state.left_position = topic.value;
  }
  if (topic.side == "right") {
    _state.right_position = topic.value;
  }
  if (topic.side == "both") {
    _state.left_position = topic.value;
    _state.right_position = topic.value;
  }
}

void MachineryNode::_publishThrust()
{
  auto left_msg = std_msgs::msg::Float64();
  left_msg.data = _state.left_thrust;
  _left_thrust_pub_->publish(left_msg);

  auto right_msg = std_msgs::msg::Float64();
  right_msg.data = _state.right_thrust;
  _right_thrust_pub_->publish(right_msg);
}

void MachineryNode::_publishPosition()
{
  auto left_msg = std_msgs::msg::Float64();
  left_msg.data = _state.left_position;
  _left_pos_pub_->publish(left_msg);

  auto right_msg = std_msgs::msg::Float64();
  right_msg.data = _state.right_position;
  _right_pos_pub_->publish(right_msg);
}

void MachineryNode::publishState()
{
  _publishThrust();
  _publishPosition();
}

void MachineryNode::init_service()
{
  _get_machinery_state_service = this->create_service<usv_hardware_interface::srv::GetMachineryState>(
    "/usv/hardware_interface/get_machinery_state",
    std::bind(
      &MachineryNode::get_machinery_state_callback, this, std::placeholders::_1,
      std::placeholders::_2));
}

void MachineryNode::get_machinery_state_callback(
  const std::shared_ptr<usv_hardware_interface::srv::GetMachineryState::Request> request
  [[maybe_unused]],
  std::shared_ptr<usv_hardware_interface::srv::GetMachineryState::Response> response)
{
  response->left_thrust = _state.left_thrust;
  response->right_thrust = _state.right_thrust;
  response->left_position = _state.left_position;
  response->right_position = _state.right_position;
}
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<usv_hardware_interface::MachineryNode>());
  rclcpp::shutdown();
  return 0;
}

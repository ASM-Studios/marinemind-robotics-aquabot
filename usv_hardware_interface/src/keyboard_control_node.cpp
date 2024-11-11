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

#include "keyboard_control_node.hpp"
#include "usv_hardware_interface/srv/get_machinery_state.hpp"

keyboard_control_node::keyboard_control_node()
: Node("keyboard_control_node")
{
  thrust_publisher_ = this->create_publisher<std_msgs::msg::String>("machinery_thurst_topic", 10);
  position_publisher_ = this->create_publisher<std_msgs::msg::String>(
    "machinery_position_topic",
    10);
  client_ =
    this->create_client<usv_hardware_interface::srv::GetMachineryState>("get_machinery_state");
  RCLCPP_INFO(this->get_logger(), "Keyboard Control Node has been started.");
  publish_instruction("thrust", "both", 0.0);
  publish_instruction("position", "both", 0.0);
}

void keyboard_control_node::publish_instruction(
  const std::string & topic, const std::string & side,
  double value)
{
  auto msg = std_msgs::msg::String();
  msg.data = side + " " + std::to_string(value);
  if (topic == "thrust") {
    thrust_publisher_->publish(msg);
  } else if (topic == "position") {
    position_publisher_->publish(msg);
  }
  RCLCPP_INFO(
    this->get_logger(), "Published instruction on %s: side=%s, value=%f", topic.c_str(),
    side.c_str(), value);
}

void keyboard_control_node::run()
{
  char key;
  struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  RCLCPP_INFO(this->get_logger(), "Use arrow keys to control the machinery. Press 'q' to quit.");

  while (rclcpp::ok()) {
    key = getchar();
    if (key == 'q') {
      rclcpp::shutdown();
      break;
    }

    std::string topic;
    std::string side;
    double value = 0.0;

    switch (key) {
      case 65: // Up arrow
        topic = "thrust";
        side = "both";
        value = THRUST_STEP;
        break;
      case 66: // Down arrow
        topic = "thrust";
        side = "both";
        value = -THRUST_STEP;
        break;
      case 67: // Right arrow
        topic = "position";
        side = "both";
        value = POSITION_STEP;
        break;
      case 68: // Left arrow
        topic = "position";
        side = "both";
        value = -POSITION_STEP;
        break;
      default:
        continue;
    }

    auto request = std::make_shared<usv_hardware_interface::srv::GetMachineryState::Request>();
    while (!client_->wait_for_service(std::chrono::seconds(1))) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
        return;
      }
      RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
    }

    auto result = client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(
        this->get_node_base_interface(),
        result) == rclcpp::FutureReturnCode::SUCCESS)
    {
      auto response = result.get();
      RCLCPP_INFO(
        this->get_logger(),
        "Current state - Left Thrust: %f, Right Thrust: %f, Left Position: %f, Right Position: %f",
        response->left_thrust, response->right_thrust, response->left_position,
        response->right_position);

      if (topic == "thrust") {
        response->left_thrust += value;
        response->right_thrust += value;
        publish_instruction(topic, "both", response->left_thrust);
        publish_instruction(topic, "both", response->right_thrust);
      } else if (topic == "position") {
        response->left_position += value;
        response->right_position += value;
        publish_instruction(topic, "both", response->left_position);
        publish_instruction(topic, "both", response->right_position);
      }
    } else {
      RCLCPP_ERROR(this->get_logger(), "Failed to call service get_machinery_state");
    }

    rclcpp::spin_some(this->get_node_base_interface());
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<keyboard_control_node>();
  node->run();
  rclcpp::shutdown();
  return 0;
}

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

#include "camera_control_server.hpp"

CameraControlServer::CameraControlServer()
: Node("camera_control_server_node"), previous_angle_(0.0)
{
  service_ = this->create_service<usv_hardware_interface::srv::SetCameraAngle>(
    "/usv/hardware_interface/set_camera_angle",
    std::bind(
      &CameraControlServer::handle_set_camera_angle, this, std::placeholders::_1,
      std::placeholders::_2)
  );
  publisher_ = this->create_publisher<std_msgs::msg::Float64>(
    "/aquabot/thrusters/main_camera_sensor/pos", 10);
}

void CameraControlServer::handle_set_camera_angle(
  const std::shared_ptr<usv_hardware_interface::srv::SetCameraAngle::Request> request,
  std::shared_ptr<usv_hardware_interface::srv::SetCameraAngle::Response> response)
{
  RCLCPP_INFO(this->get_logger(), "Setting camera angle to: %f degrees", request->angle);
  double desired_angle_radians = request->angle * M_PI / 180.0;
  double angle_to_publish = calculate_angle_to_publish(desired_angle_radians);
  auto message = std::make_shared<std_msgs::msg::Float64>();
  message->data = angle_to_publish;

  publisher_->publish(*message);
  RCLCPP_INFO(this->get_logger(), "Published camera angle: %f radians", angle_to_publish);

  previous_angle_ = desired_angle_radians;
  response->success = true;
}

double CameraControlServer::calculate_angle_to_publish(double desired_angle)
{
  double angle_difference = desired_angle - previous_angle_;
  if (angle_difference > M_PI) {
    angle_difference -= 2 * M_PI;
  } else if (angle_difference < -M_PI) {
    angle_difference += 2 * M_PI;
  }
  return previous_angle_ + angle_difference;
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraControlServer>());
  rclcpp::shutdown();
  return 0;
}

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

#include "../include/usv_guidance_and_control/path_planner.hpp"
#include "sensor_msgs/msg/detail/nav_sat_fix__struct.hpp"

void PathPlanner::_find_path(const usv_guidance_and_control::srv::PathPlanner::Request::SharedPtr request,
  usv_guidance_and_control::srv::PathPlanner::Response::SharedPtr response) {

}

void PathPlanner::_obstacle_detected_callback(const std_msgs::msg::Float64 msg) {

}

PathPlanner::PathPlanner() : Node("guide_module") {
  RCLCPP_INFO(this->get_logger(), "Path Planner Node started.");
  this->_service = this->create_service<usv_guidance_and_control::srv::PathPlanner>(
    "/usv/guidance_and_control/path_planner", std::bind(&PathPlanner::_find_path, this, std::placeholders::_1, std::placeholders::_2));

  this->_obstacle_detected = this->create_subscription<std_msgs::msg::Float64>(
    "/usv/perception/obstacle_detected", 10, std::bind(&PathPlanner::_obstacle_detected_callback, this, std::placeholders::_1));
}

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PathPlanner>());
  rclcpp::shutdown();
}

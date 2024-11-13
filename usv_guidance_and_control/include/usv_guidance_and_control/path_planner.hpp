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

#ifndef USV_GUIDANCE_AND_CONTROL__GUIDE_MODULE_HPP_
#define USV_GUIDANCE_AND_CONTROL__GUIDE_MODULE_HPP_

#include <vector>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <std_msgs/msg/float64.hpp>
#include "usv_guidance_and_control/srv/path_planner.hpp"

enum class ObstacleType {

};

struct Obstacle {
  sensor_msgs::msg::NavSatFix coordinate;
  ObstacleType type;
};

class PathPlanner : public rclcpp::Node {
  private:
    std::vector<Obstacle> _obstacles;
    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr _obstacle_detected;
    //rclcpp::Subscription<int>::SharedPtr _gps;

    rclcpp::Service<usv_guidance_and_control::srv::PathPlanner>::SharedPtr _service;

    void _find_path(const usv_guidance_and_control::srv::PathPlanner::Request::SharedPtr request,
      usv_guidance_and_control::srv::PathPlanner::Response::SharedPtr response);

    void _obstacle_detected_callback(const std_msgs::msg::Float64 msg);

  public:
    explicit PathPlanner();
    ~PathPlanner() = default;
};

#endif

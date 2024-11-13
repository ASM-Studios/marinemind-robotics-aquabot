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

#include <queue>
#include <tuple>

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/path.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "sensor_msgs/msg/imu.hpp"

struct ImuData {
  std_msgs::msg::Float64 roll;   // x
  std_msgs::msg::Float64 pitch;  // y
  std_msgs::msg::Float64 yaw;    // z
  std_msgs::msg::Float64 scalar; // w
};

class GuidanceNode: public rclcpp::Node {
    private:
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr _sub_imu;
        rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr _sub_gps;
        rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr _sub_waypoints;

        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _pub_deviation_detected;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _pub_cmd_vel;
        rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr _pub_goal_reached;

        std::queue<std::pair<std_msgs::msg::Float64, std_msgs::msg::Float64>> _waypoints;
        std::pair<std_msgs::msg::Float64, std_msgs::msg::Float64> _gps_position;
        ImuData _imu_data;

        void _imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg);
        void _gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
        void _waypointsCallback(const nav_msgs::msg::Path::SharedPtr msg);
    public:
        GuidanceNode();
        ~GuidanceNode();
};

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

#include "guidance_node.hpp"

GuidanceNode::GuidanceNode(): Node("guidance_node") {
    _sub_imu = this->create_subscription<sensor_msgs::msg::Imu>(
        "/usv/hardware_interface/imu_data",
        10,
        std::bind(&GuidanceNode::_imuCallback, this, std::placeholders::_1)
    );
    _sub_gps = this->create_subscription<sensor_msgs::msg::NavSatFix>(
        "/usv/hardware_interface/gps_fix",
        10,
        std::bind(&GuidanceNode::_gpsCallback, this, std::placeholders::_1)
    );
    _sub_waypoints = this->create_subscription<nav_msgs::msg::Path>(
        "/usv/guidance/waypoints",
        10,
        std::bind(&GuidanceNode::_waypointsCallback, this, std::placeholders::_1)
    );

    _pub_deviation_detected = this->create_publisher<std_msgs::msg::Float64>("/usv/guidance/deviation_detected", 10);
    _pub_cmd_vel = this->create_publisher<std_msgs::msg::Float64>("/usv/hardware_interface/cmd_vel", 10);
	_pub_goal_reached = this->create_publisher<std_msgs::msg::Bool>("/usv/guidance/goal_reached", 10);
};

GuidanceNode::~GuidanceNode() {};

void GuidanceNode::_imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    std_msgs::msg::Float64 roll, pitch, yaw, scalar;
    roll.data = msg->orientation.x;
    pitch.data = msg->orientation.y;
    yaw.data = msg->orientation.z;
    scalar.data = msg->orientation.w;
    _imu_data = {roll, pitch, yaw, scalar};
}

void GuidanceNode::_gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
    std_msgs::msg::Float64 latitude;
    std_msgs::msg::Float64 longitude;
    latitude.data = msg->latitude;
    longitude.data = msg->longitude;
    _gps_position = std::make_pair(latitude, longitude);
};

void GuidanceNode::_waypointsCallback(const nav_msgs::msg::Path::SharedPtr msg) {
    for (const auto& pose : msg->poses) {
        std_msgs::msg::Float64 x;
        std_msgs::msg::Float64 y;
        x.data = pose.pose.position.x;
        y.data = pose.pose.position.y;
        _waypoints.push(std::make_pair(x, y));
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GuidanceNode>());
    rclcpp::shutdown();
    return 0;
}

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

    this->create_wall_timer(
        std::chrono::milliseconds(1000),
        std::bind(&GuidanceNode::_handleGuidance, this)
    );
}

GuidanceNode::~GuidanceNode() {};

void GuidanceNode::_imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    Quaternion orientation;
    orientation.roll = msg->orientation.x;
    orientation.pitch = msg->orientation.y;
    orientation.yaw = msg->orientation.z;
    orientation.scalar = msg->orientation.w;

    Vector3D angular_velocity;
    angular_velocity.x = msg->angular_velocity.x;
    angular_velocity.y = msg->angular_velocity.y;
    angular_velocity.z = msg->angular_velocity.z;

    Vector3D linear_acceleration;
    linear_acceleration.x = msg->linear_acceleration.x;
    linear_acceleration.y = msg->linear_acceleration.y;
    linear_acceleration.z = msg->linear_acceleration.z;

    _imu_data = {orientation, angular_velocity, linear_acceleration};
}

void GuidanceNode::_gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
    _gps_position = std::make_pair(msg->latitude, msg->longitude);
}

void GuidanceNode::_waypointsCallback(const nav_msgs::msg::Path::SharedPtr msg) {
    for (const auto& pose : msg->poses) {
        _waypoints.push(std::make_pair(pose.pose.position.x, pose.pose.position.y));
    }
}

bool GuidanceNode::_checkWaypointReached() {
    const double tolerance = 0.0001;

    double delta_x = _current_waypoint.first - _gps_position.first;
    double delta_y = _current_waypoint.second - _gps_position.second;
    double distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);

    if (distance < tolerance) {
        RCLCPP_INFO(this->get_logger(), "Waypoint reached: (%f, %f)", _current_waypoint.first, _current_waypoint.second);
        return true;
    }
    return false;
}

void GuidanceNode::_handleGuidance() {
    if (_waypoints.empty()) {
        std_msgs::msg::Bool goal_reached_msg;
        goal_reached_msg.data = true;
        _pub_goal_reached->publish(goal_reached_msg);
        return;
    }

    if (_checkWaypointReached()) {
        _current_waypoint = _waypoints.front();
        _waypoints.pop();

        double delta_x = _current_waypoint.first - _gps_position.first;
        double delta_y = _current_waypoint.second - _gps_position.second;
        double required_orientation = std::atan2(delta_y, delta_x);

        RCLCPP_INFO(this->get_logger(), "New waypoint set: (%f, %f)", _current_waypoint.first, _current_waypoint.second);
        RCLCPP_INFO(this->get_logger(), "Required orientation: %f radians", required_orientation);

        // TODO: Orient the USV towards the required orientation, calculate velocity and publish to /usv/hardware_interface/cmd_vel
        return;
    }

    // TODO: Implement deviation detection and publish to /usv/guidance/deviation_detected
    return;
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GuidanceNode>());
    rclcpp::shutdown();
    return 0;
}

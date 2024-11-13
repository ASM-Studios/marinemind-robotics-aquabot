#pragma once

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"

class GuidanceNode: public rclcpp::Node {
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _sub_imu;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _sub_gps;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _sub_waypoints;

        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _pub_deviation_detected;

        void _imuCallback(const std_msgs::msg::String::SharedPtr msg);
        void _gpsCallback(const std_msgs::msg::String::SharedPtr msg);
        void _waypointsCallback(const std_msgs::msg::String::SharedPtr msg);
    public:
        GuidanceNode();
        ~GuidanceNode();
};

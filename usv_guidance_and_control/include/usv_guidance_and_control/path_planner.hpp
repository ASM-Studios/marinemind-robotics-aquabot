// Copyright 2024 MarineMindRobotics

#ifndef USV_GUIDANCE_AND_CONTROL__GUIDE_MODULE_HPP_
#define USV_GUIDANCE_AND_CONTROL__GUIDE_MODULE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/sensor_msgs/msg/nav_sat_fix.hpp>
#include <std_msgs/std_msgs/msg/float64.hpp>

enum class ObstacleType {

};

struct Obstacle {
  sensor_msgs::msg::NavSatFix coordinate;
  ObstacleType type;
};

class PathPlanner : public rclcpp::Node {
  private:
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr _turbine_detected;
    rclcpp::Subscription<Obstacle>::SharedPtr _obstacle_detected;
    rclcpp::Subscription<int>::SharedPtr _imu_data;
    rclcpp::Subscription<int>::SharedPtr _gps_fix;

    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _nav;

    rclcpp::Service<usv_guidance_and_control::srv::PathPlanner>::SharedPtr _service;

    void _turbine_detected_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);

  public:
    explicit PathPlanner();
    ~PathPlanner() = default;
};

#endif

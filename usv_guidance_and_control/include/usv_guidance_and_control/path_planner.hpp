// Copyright 2024 MarineMindRobotics

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

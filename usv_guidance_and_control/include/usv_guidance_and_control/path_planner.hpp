// Copyright 2024 MarineMindRobotics

#ifndef USV_GUIDANCE_AND_CONTROL__PATH_PLANNER_HPP_
#define USV_GUIDANCE_AND_CONTROL__PATH_PLANNER_HPP_

#include <vector>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/float64.hpp>
#include <nav_msgs/msg/path.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include "usv_guidance_and_control/srv/path_planner.hpp"

enum class ObstacleType {

};

struct Obstacle {
  geometry_msgs::msg::Point position;
  ObstacleType type;
};

class PathPlanner : public rclcpp::Node {
  private:
    nav_msgs::msg::OccupancyGrid _map;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr _obstacle_detected;
    //rclcpp::Subscription<int>::SharedPtr _gps;
    
    rclcpp::Service<usv_guidance_and_control::srv::PathPlanner>::SharedPtr _service;

    void _clean_coords(geometry_msgs::msg::Point &point);

    rclcpp::TimerBase::SharedPtr _map_timer;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr _map_publisher;
    void _publish_map();

    void _find_path(const usv_guidance_and_control::srv::PathPlanner::Request::SharedPtr request,
      usv_guidance_and_control::srv::PathPlanner::Response::SharedPtr response);

    void _obstacle_detected_callback(const geometry_msgs::msg::Point msg);

  public:
    explicit PathPlanner();
    ~PathPlanner() = default;
};

#endif

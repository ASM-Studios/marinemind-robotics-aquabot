#include "../include/usv_guidance_and_control/path_planner.hpp"
#include <sensor_msgs/msg/detail/nav_sat_fix__struct.hpp>

void PathPlanner::_clean_coords(geometry_msgs::msg::Point &point) {
    
}

void PathPlanner::_publish_map() {
  this->_map.header.stamp = this->now();
  this->_map_publisher->publish(this->_map);
  RCLCPP_INFO(this->get_logger(), "Map published.");
}

void PathPlanner::_find_path(const usv_guidance_and_control::srv::PathPlanner::Request::SharedPtr request,
  usv_guidance_and_control::srv::PathPlanner::Response::SharedPtr response) {
  RCLCPP_INFO(this->get_logger(), "Path Planner Service called.");
  auto x = request->point.x;
  auto y = request->point.y;
  RCLCPP_INFO(this->get_logger(), "Received x: %lf, y: %lf", x, y);
  response->tmp = 1;
}

void PathPlanner::_obstacle_detected_callback(const geometry_msgs::msg::Point msg) {
  RCLCPP_INFO(this->get_logger(), "Obstacle detected at x: %f, y: %f, z: %f", msg.x, msg.y, msg.z);
  //int x = msg.x * 100000;
  //int y = msg.y * 100000;
  //this->_map.data[x + y * this->_map.info.width] = 100;
}

PathPlanner::PathPlanner() : Node("pah_planner"), _map(nav_msgs::msg::OccupancyGrid()) {
  RCLCPP_INFO(this->get_logger(), "Path Planner Node started.");

  this->_map.header.frame_id = "map";
  this->_map.info.width = 500;
  this->_map.info.height = 500;
  this->_map.info.resolution = 0.01;
  this->_map.data.resize(this->_map.info.width * this->_map.info.height);
  for (int i = 0; i < this->_map.info.width * this->_map.info.height; i++) {
    this->_map.data[i] = 0;
  }

  this->_service = this->create_service<usv_guidance_and_control::srv::PathPlanner>(
    "/usv/guidance_and_control/path_planner", std::bind(&PathPlanner::_find_path, this, std::placeholders::_1, std::placeholders::_2));

  this->_map_publisher = this->create_publisher<nav_msgs::msg::OccupancyGrid>("/map", 10);
  this->_map_timer = this->create_wall_timer(std::chrono::seconds(1), std::bind(&PathPlanner::_publish_map, this));

  this->_obstacle_detected = this->create_subscription<geometry_msgs::msg::Point>(
    "/usv/perception/obstacle_detected", 10, std::bind(&PathPlanner::_obstacle_detected_callback, this, std::placeholders::_1));
}

int main(int ac, char **av) {
  rclcpp::init(ac, av);
  rclcpp::spin(std::make_shared<PathPlanner>());
  rclcpp::shutdown();
}

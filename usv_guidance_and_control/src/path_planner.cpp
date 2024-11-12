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

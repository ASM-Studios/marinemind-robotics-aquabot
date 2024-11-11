#include "../include/usv_guidance_and_control/path_planner.hpp"
#include "sensor_msgs/msg/detail/nav_sat_fix__struct.hpp"

void PathPlanner::_turbine_detected_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {

}

PathPlanner::PathPlanner() : Node("guide_module") {
  this->_turbine_detected = this->create_subscription<sensor_msgs::msg::NavSatFix>(
    "/turbine/detected", 10, std::bind(&PathPlanner::_turbine_detected_callback, this, std::placeholders::_1));
}

#include "guidance_node.hpp"

GuidanceNode::GuidanceNode(): Node("guidance_node") {
    _sub_imu = this->create_subscription<std_msgs::msg::String>("/usv/hardware_interface/imu_data", 10, std::bind(&GuidanceNode::_imuCallback, this, std::placeholders::_1));
    _sub_gps = this->create_subscription<std_msgs::msg::String>("/usv/hardware_interface/gps_fix", 10, std::bind(&GuidanceNode::_gpsCallback, this, std::placeholders::_1));
    _sub_waypoints = this->create_subscription<std_msgs::msg::String>("/usv/guidance/waypoints", 10, std::bind(&GuidanceNode::_waypointsCallback, this, std::placeholders::_1));

    _pub_deviation_detected = this->create_publisher<std_msgs::msg::Float64>("/usv/guidance/deviation_detected", 10);
};

GuidanceNode::~GuidanceNode() {};

void GuidanceNode::_imuCallback(const std_msgs::msg::String::SharedPtr msg) {
    // TODO: Parse the message and extract the data
};

void GuidanceNode::_gpsCallback(const std_msgs::msg::String::SharedPtr msg) {
    // TODO: Parse the message and extract the data
};

void GuidanceNode::_waypointsCallback(const std_msgs::msg::String::SharedPtr msg) {
    // TODO: Parse the message and extract the data
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GuidanceNode>());
  rclcpp::shutdown();
  return 0;
}

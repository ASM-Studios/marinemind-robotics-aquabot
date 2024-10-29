// Copyright 2024 MarineMindRobotics

#include "sensor_interface_node.hpp"
#include "rclcpp/logging.hpp"

SensorInterfaceNode::SensorInterfaceNode()
: Node("sensor_interface_node")
{
  RCLCPP_INFO(this->get_logger(), "Sensor Interface Node started.");

  image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
    "/aquabot/sensors/cameras/main_camera_sensor/image_raw",
    10,
    std::bind(&SensorInterfaceNode::image_callback, this, std::placeholders::_1)
  );
  camera_info_subscription_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
    "/aquabot/sensors/cameras/main_camera_sensor/camera_info",
    10,
    std::bind(&SensorInterfaceNode::camera_info_callback, this, std::placeholders::_1)
  );
  gps_subscription_ = this->create_subscription<sensor_msgs::msg::NavSatFix>(
    "/aquabot/sensors/gps/gps/fix",
    10,
    std::bind(&SensorInterfaceNode::gps_callback, this, std::placeholders::_1)
  );
  imu_subscription_ = this->create_subscription<sensor_msgs::msg::Imu>(
    "/aquabot/sensors/imu/imu/data",
    10,
    std::bind(&SensorInterfaceNode::imu_callback, this, std::placeholders::_1)
  );
  windturbine_subscription_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
    "/aquabot/ais_sensor/windturbines_positions",
    10,
    std::bind(&SensorInterfaceNode::windturbine_callback, this, std::placeholders::_1)
  );
  acoustics_subscription_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
    "/aquabot/sensors/acoustics/receiver/range_bearing",
    10,
    std::bind(&SensorInterfaceNode::acoustics_callback, this, std::placeholders::_1)
  );

  image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>(
    "/usv/hardware_interface/image_raw", 10);
  camera_info_publisher_ = this->create_publisher<sensor_msgs::msg::CameraInfo>(
    "/usv/hardware_interface/camera_info", 10);
  gps_publisher_ = this->create_publisher<sensor_msgs::msg::NavSatFix>(
    "/usv/hardware_interface/gps/fix", 10);
  imu_publisher_ = this->create_publisher<sensor_msgs::msg::Imu>(
    "/usv/hardware_interface/imu/data",
    10);
  windturbine_publisher_ = this->create_publisher<geometry_msgs::msg::PoseArray>(
    "/windturbines_positions", 10);
  acoustics_publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(
    "/acoustics_data",
    10);
}

void SensorInterfaceNode::image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
  RCLCPP_INFO(
    this->get_logger(), "Received an image: width=%d, height=%d", msg->width,
    msg->height);
}

void SensorInterfaceNode::camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
{
  RCLCPP_INFO(
    this->get_logger(), "Received camera info: width=%d, height=%d", msg->width, msg->height);
}

void SensorInterfaceNode::gps_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
  RCLCPP_INFO(
    this->get_logger(),
    "GPS Fix: Latitude=%f, Longitude=%f, Altitude=%f",
    msg->latitude,
    msg->longitude,
    msg->altitude
  );
  gps_publisher_->publish(*msg);
}

void SensorInterfaceNode::imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg)
{
  RCLCPP_INFO(
    this->get_logger(), "IMU Data: Orientation x=%f, y=%f, z=%f, w=%f",
    msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
  imu_publisher_->publish(*msg);
}

void SensorInterfaceNode::windturbine_callback(const geometry_msgs::msg::PoseArray::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(), "Received %zu wind turbine positions.", msg->poses.size());
}

void SensorInterfaceNode::acoustics_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
  if (msg->data.size() == 3) {
    double range = msg->data[0];
    double bearing = msg->data[1];
    double elevation = msg->data[2];
    RCLCPP_INFO(
      this->get_logger(),
      "Acoustics Data: Range=%f, Bearing=%f, Elevation=%f",
      range, bearing, elevation
    );
  } else {
    RCLCPP_WARN(this->get_logger(), "Received acoustics data with incorrect size.");
  }
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SensorInterfaceNode>());
  rclcpp::shutdown();
  return 0;
}

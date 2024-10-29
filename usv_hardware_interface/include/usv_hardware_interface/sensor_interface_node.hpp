// Copyright 2024 MarineMindRobotics

#ifndef USV_HARDWARE_INTERFACE__SENSOR_INTERFACE_NODE_HPP_
#define USV_HARDWARE_INTERFACE__SENSOR_INTERFACE_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

class SensorInterfaceNode : public rclcpp::Node
{
public:
  SensorInterfaceNode();

private:
  /**
   * \brief Callback function for image messages.
   * \param msg The received image message.
   */
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);

  /**
   * \brief Callback function for camera info messages.
   * \param msg The received camera info message.
   */
  void camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  /**
   * \brief Callback function for GPS fix messages.
   * \param msg The received GPS fix message.
   */
  void gps_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);

  /**
   * \brief Callback function for IMU data messages.
   * \param msg The received IMU data message.
   */
  void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg);

  /**
   * \brief Callback function for wind turbine position messages.
   * \param msg The received wind turbine position message.
   */
  void windturbine_callback(const geometry_msgs::msg::PoseArray::SharedPtr msg);

  /**
   * \brief Callback function for acoustics data messages.
   * \param msg The received acoustics data message.
   */
  void acoustics_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_subscription_;
  rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_subscription_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscription_;
  rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr windturbine_subscription_;
  rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr acoustics_subscription_;

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
  rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_publisher_;
  rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr gps_publisher_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
  rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr windturbine_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr acoustics_publisher_;
};

#endif  // USV_HARDWARE_INTERFACE__SENSOR_INTERFACE_NODE_HPP_

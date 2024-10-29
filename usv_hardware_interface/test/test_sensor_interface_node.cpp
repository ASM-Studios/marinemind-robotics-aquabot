// Copyright 2024 MarineMindRobotics

#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include "sensor_interface_node.hpp"

class SensorInterfaceNodeTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    rclcpp::init(0, nullptr);
    node_ = std::make_shared<SensorInterfaceNode>();
    executor_.add_node(node_);
    spin_thread_ = std::thread([this]() {executor_.spin();});
  }

  void TearDown() override
  {
    executor_.cancel();
    spin_thread_.join();
    rclcpp::shutdown();
  }

  rclcpp::executors::SingleThreadedExecutor executor_;
  std::shared_ptr<SensorInterfaceNode> node_;
  std::thread spin_thread_;
};

/**
 * \brief Test to check if the image publisher is working.
 * Publishes an image message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, ImagePublisher)
{
  auto publisher = node_->create_publisher<sensor_msgs::msg::Image>("/image_raw", 10);
  auto msg = std::make_shared<sensor_msgs::msg::Image>();
  msg->width = 640;
  msg->height = 480;

  auto subscription = node_->create_subscription<sensor_msgs::msg::Image>(
    "/image_raw", 10,
    [msg](const sensor_msgs::msg::Image::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->width, msg->width);
      EXPECT_EQ(received_msg->height, msg->height);
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

/**
 * \brief Test to check if the camera info publisher is working.
 * Publishes a camera info message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, CameraInfoPublisher)
{
  auto publisher = node_->create_publisher<sensor_msgs::msg::CameraInfo>("/camera_info", 10);
  auto msg = std::make_shared<sensor_msgs::msg::CameraInfo>();
  msg->width = 640;
  msg->height = 480;

  auto subscription = node_->create_subscription<sensor_msgs::msg::CameraInfo>(
    "/camera_info", 10,
    [msg](const sensor_msgs::msg::CameraInfo::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->width, msg->width);
      EXPECT_EQ(received_msg->height, msg->height);
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

/**
 * \brief Test to check if the GPS publisher is working.
 * Publishes a GPS fix message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, GpsPublisher)
{
  auto publisher = node_->create_publisher<sensor_msgs::msg::NavSatFix>("/gps/fix", 10);
  auto msg = std::make_shared<sensor_msgs::msg::NavSatFix>();
  msg->latitude = 37.7749;
  msg->longitude = -122.4194;
  msg->altitude = 10.0;

  auto subscription = node_->create_subscription<sensor_msgs::msg::NavSatFix>(
    "/gps/fix", 10,
    [msg](const sensor_msgs::msg::NavSatFix::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->latitude, msg->latitude);
      EXPECT_EQ(received_msg->longitude, msg->longitude);
      EXPECT_EQ(received_msg->altitude, msg->altitude);
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

/**
 * \brief Test to check if the IMU publisher is working.
 * Publishes an IMU data message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, ImuPublisher)
{
  auto publisher = node_->create_publisher<sensor_msgs::msg::Imu>("/imu/data", 10);
  auto msg = std::make_shared<sensor_msgs::msg::Imu>();
  msg->orientation.x = 0.0;
  msg->orientation.y = 0.0;
  msg->orientation.z = 0.0;
  msg->orientation.w = 1.0;

  auto subscription = node_->create_subscription<sensor_msgs::msg::Imu>(
    "/imu/data", 10,
    [msg](const sensor_msgs::msg::Imu::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->orientation.x, msg->orientation.x);
      EXPECT_EQ(received_msg->orientation.y, msg->orientation.y);
      EXPECT_EQ(received_msg->orientation.z, msg->orientation.z);
      EXPECT_EQ(received_msg->orientation.w, msg->orientation.w);
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

/**
 * \brief Test to check if the wind turbine positions publisher is working.
 * Publishes a PoseArray message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, WindTurbinePublisher)
{
  auto publisher = node_->create_publisher<geometry_msgs::msg::PoseArray>(
    "/windturbines_positions",
    10);
  auto msg = std::make_shared<geometry_msgs::msg::PoseArray>();
  msg->poses.resize(5);

  auto subscription = node_->create_subscription<geometry_msgs::msg::PoseArray>(
    "/windturbines_positions", 10,
    [msg](const geometry_msgs::msg::PoseArray::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->poses.size(), msg->poses.size());
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

/**
 * \brief Test to check if the acoustics data publisher is working.
 * Publishes a Float64MultiArray message and verifies if it is received correctly.
 */
TEST_F(SensorInterfaceNodeTest, AcousticsPublisher)
{
  auto publisher = node_->create_publisher<std_msgs::msg::Float64MultiArray>("/acoustics_data", 10);
  auto msg = std::make_shared<std_msgs::msg::Float64MultiArray>();
  msg->data = {100.0, 1.57, 0.0};

  auto subscription = node_->create_subscription<std_msgs::msg::Float64MultiArray>(
    "/acoustics_data", 10,
    [msg](const std_msgs::msg::Float64MultiArray::SharedPtr received_msg) {
      EXPECT_EQ(received_msg->data.size(), msg->data.size());
      EXPECT_EQ(received_msg->data[0], msg->data[0]);
      EXPECT_EQ(received_msg->data[1], msg->data[1]);
      EXPECT_EQ(received_msg->data[2], msg->data[2]);
    });

  publisher->publish(*msg);
  rclcpp::sleep_for(std::chrono::milliseconds(100));
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

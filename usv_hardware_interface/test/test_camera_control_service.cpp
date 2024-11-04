/*
 * This file is part of the MarineMindRobotics project.
 *
 * Copyright (C) 2024 MarineMindRobotics
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/executors/single_threaded_executor.hpp"
#include "usv_hardware_interface/srv/set_camera_angle.hpp"
#include "std_msgs/msg/float64.hpp"
#include "gtest/gtest.h"

class CameraControlServiceTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    node_ = std::make_shared<rclcpp::Node>("test_camera_control_service");
    client_ = node_->create_client<usv_hardware_interface::srv::SetCameraAngle>(
      "/usv/hardware_interface/set_camera_angle");
    publisher_ = node_->create_publisher<std_msgs::msg::Float64>(
      "/aquabot/thrusters/main_camera_sensor/pos", 10);
  }

  void TearDown() override
  {
    node_.reset();
  }

  rclcpp::Node::SharedPtr node_;
  rclcpp::Client<usv_hardware_interface::srv::SetCameraAngle>::SharedPtr client_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
};

TEST_F(CameraControlServiceTest, testSetCameraAngle) {
  auto request = std::make_shared<usv_hardware_interface::srv::SetCameraAngle::Request>();
  request->angle = 90.0;

  ASSERT_TRUE(client_->wait_for_service(std::chrono::seconds(5)));

  auto result = client_->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node_, result) == rclcpp::FutureReturnCode::SUCCESS) {
    EXPECT_TRUE(result.get()->success);
  } else {
    FAIL() << "Service call failed";
  }
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);
  int ret = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return ret;
}

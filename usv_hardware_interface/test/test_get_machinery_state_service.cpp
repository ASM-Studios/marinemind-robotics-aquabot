#include "rclcpp/rclcpp.hpp"
#include "rclcpp/executors/single_threaded_executor.hpp"
#include "usv_hardware_interface/srv/get_machinery_state.hpp"
#include "gtest/gtest.h"

class MachineryStateServiceTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    node_ = std::make_shared<rclcpp::Node>("test_machinery_state_service");
    client_ = node_->create_client<usv_hardware_interface::srv::GetMachineryState>(
      "/usv/hardware_interface/get_machinery_state");
  }

  void TearDown() override
  {
    node_.reset();
  }

  rclcpp::Node::SharedPtr node_;
  rclcpp::Client<usv_hardware_interface::srv::GetMachineryState>::SharedPtr client_;
};

TEST_F(MachineryStateServiceTest, testGetMachineryState) {
  auto request = std::make_shared<usv_hardware_interface::srv::GetMachineryState::Request>();

  ASSERT_TRUE(client_->wait_for_service(std::chrono::seconds(5)));

  auto result = client_->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node_, result) == rclcpp::FutureReturnCode::SUCCESS) {
    auto response = result.get();
    EXPECT_GE(response->left_thrust, 0.0);
    EXPECT_GE(response->right_thrust, 0.0);
    EXPECT_GE(response->left_position, 0.0);
    EXPECT_GE(response->right_position, 0.0);
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

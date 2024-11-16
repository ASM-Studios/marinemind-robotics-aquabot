#include "../include/usv_guidance_and_control/guidance.hpp"
#include <actionlib/client/simple_action_client.h>

Guidance::Guidance() : Node("guide") {
    /*this->_move_to_waypoint_action_server = rclcpp_action::create_server<usv_guidance_and_control::action::MoveToWaypoint>(
        "move_to_waypoint",
        std::bind(&Guidance::_handle_goal, this, std::placeholders::_1)
        std::bind(&Guidance::_handle_cancel, this, std::placeholders::_1),
        std::bind(&Guidance::_handle_accepted, this, std::placeholders::_1)
    );*/
}

int main(int ac, char **av) {
  rclcpp::init(ac, av);
  rclcpp::spin(std::make_shared<Guidance>());
  rclcpp::shutdown();
}

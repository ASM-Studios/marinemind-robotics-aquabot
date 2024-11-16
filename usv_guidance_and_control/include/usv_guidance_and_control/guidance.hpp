#ifndef USV_GUIDANCE_AND_CONTROL__GUIDANCE_HPP_
#define USV_GUIDANCE_AND_CONTROL__GUIDANCE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "usv_guidance_and_control/action/move_to_waypoint.hpp"

class Guidance : public rclcpp::Node {
    private:
       void _handle_goal(rclcpp_action::GoalUUID& uuid, usv_guidance_and_control::action::MoveToWaypoint::Goal::SharedPtr goal_handle);
        void _handleCancelResponse(usv_guidance_and_control::action::MoveToWaypoint::Goal::SharedPtr goal_handle);
        //void _handle_result()

        rclcpp_action::Server<usv_guidance_and_control::action::MoveToWaypoint>::SharedPtr _move_to_waypoint_action_server;

    public:
        Guidance();
};

#endif

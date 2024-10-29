// Copyright 2024 MarineMindRobotics

#ifndef USV_HARDWARE_INTERFACE__CAMERA_CONTROL_SERVER_HPP_
#define USV_HARDWARE_INTERFACE__CAMERA_CONTROL_SERVER_HPP_

#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "usv_hardware_interface/srv/set_camera_angle.hpp"
#include "std_msgs/msg/float64.hpp"

class CameraControlServer : public rclcpp::Node
{
public:
  CameraControlServer();

private:
  /**
   * \brief Handles the service request to set the camera angle.
   * \param request The request containing the desired camera angle in degrees.
   * \param response The response indicating the success of the operation.
   */
  void handle_set_camera_angle(
    const std::shared_ptr<usv_hardware_interface::srv::SetCameraAngle::Request> request,
    std::shared_ptr<usv_hardware_interface::srv::SetCameraAngle::Response> response);

  /**
   * \brief Calculates the angle to publish based on the desired angle.
   * \param desired_angle The desired camera angle in radians.
   * \return The angle to publish, adjusted to be within the range of -π to π.
   */
  double calculate_angle_to_publish(double desired_angle);

  rclcpp::Service<usv_hardware_interface::srv::SetCameraAngle>::SharedPtr service_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  double previous_angle_;
};

#endif  // USV_HARDWARE_INTERFACE__CAMERA_CONTROL_SERVER_HPP_

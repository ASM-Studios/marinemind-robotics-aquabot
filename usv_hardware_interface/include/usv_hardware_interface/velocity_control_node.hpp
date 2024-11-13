#pragma once

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/float64.hpp"
#include "sensor_msgs/msg/imu.hpp"

struct Quaternion {
    double roll;   // x
    double pitch;  // y
    double yaw;    // z
    double scalar; // w
};

struct Vector3D {
    double x;
    double y;
    double z;
};

struct ImuData {
    Quaternion orientation;
    Vector3D angular_velocity;
    Vector3D linear_acceleration;
};

const double MAX_THRUST = 5000.0;

class VelocityControlNode : public rclcpp::Node {
    private:
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr _sub_cmd_vel;
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr _sub_imu;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _pub_thrust;

        double _target_velocity;
        ImuData _imu_data;

        void _cmdVelCallback(const std_msgs::msg::Float64::SharedPtr msg);
        void _imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg);

    public:
        VelocityControlNode();
        ~VelocityControlNode();
};

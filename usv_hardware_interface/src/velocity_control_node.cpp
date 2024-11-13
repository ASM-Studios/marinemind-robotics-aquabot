#include "velocity_control_node.hpp"

VelocityControlNode::VelocityControlNode(): Node("velocity_control_node") {
    _sub_cmd_vel = this->create_subscription<std_msgs::msg::Float64>(
        "/usv/hardware_interface/cmd_vel",
        10,
        std::bind(&VelocityControlNode::_cmdVelCallback, this, std::placeholders::_1)
    );
    _sub_imu = this->create_subscription<sensor_msgs::msg::Imu>(
        "/usv/hardware_interface/imu_data",
        10,
        std::bind(&VelocityControlNode::_imuCallback, this, std::placeholders::_1)
    );

    _pub_thrust = this->create_publisher<std_msgs::msg::Float64>("/usv/thrusters/machinery", 10);
}

VelocityControlNode::~VelocityControlNode() {};

void VelocityControlNode::_cmdVelCallback(const std_msgs::msg::Float64::SharedPtr msg) {
    _target_velocity = msg->data;

    // Example control logic to adjust thrust based on target velocity and IMU data
    double current_velocity = _imu_data.linear_acceleration.x; // Assuming x-axis acceleration represents forward velocity
    double error = _target_velocity - current_velocity;
    double thrust = error * 1000; // Proportional control gain

    std::clamp(thrust, -MAX_THRUST, MAX_THRUST);

    auto thrust_msg = std_msgs::msg::Float64();
    thrust_msg.data = thrust;
    _pub_thrust->publish(thrust_msg);
}

void VelocityControlNode::_imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    Quaternion orientation;
    orientation.roll = msg->orientation.x;
    orientation.pitch = msg->orientation.y;
    orientation.yaw = msg->orientation.z;
    orientation.scalar = msg->orientation.w;

    Vector3D angular_velocity;
    angular_velocity.x = msg->angular_velocity.x;
    angular_velocity.y = msg->angular_velocity.y;
    angular_velocity.z = msg->angular_velocity.z;

    Vector3D linear_acceleration;
    linear_acceleration.x = msg->linear_acceleration.x;
    linear_acceleration.y = msg->linear_acceleration.y;
    linear_acceleration.z = msg->linear_acceleration.z;

    _imu_data = {orientation, angular_velocity, linear_acceleration};
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VelocityControlNode>());
    rclcpp::shutdown();
    return 0;
}

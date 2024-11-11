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

#ifndef USV_PERCEPTION__OBSTACLE_DETECTOR_NODE_HPP_
#define USV_PERCEPTION__OBSTACLE_DETECTOR_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

class ObstacleDetectorNode : public rclcpp::Node {
    public:
        ObstacleDetectorNode();

        void getImageFromCamera();

    private:
        void image_callback(const sensor_msgs::Image::SharedPtr msg);
        void orientation_callback(const sensor_msgs::Imu::SharedPtr msg);
        void camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
        rclcpp::Subscription<sensor_msgs::msg::Imu::SharedPtr> orientation_subscription_;
        rclcpp::Subscription<sensor_msgs::msg::CameraInfo::SharedPtr>  camera_info_subscription_;


};

#endif
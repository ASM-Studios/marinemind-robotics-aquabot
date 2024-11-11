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

#include "../include/obstacle_detector_node.hpp"

ObstacleDetectorNode::ObstacleDetectorNode() : Node("obstacle_detector_node")
{
    RCLCPP_INFO(this->get_logger(), "Obstacle Detector Node started.");

    image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/usv/hardware_interface/image_raw",
        10,
        std::bind(&ObstacleDetectorNode::image_callback,
        this,
        std::placeholders::_1));
}

void ObstacleDetectorNode::getImageFromCamera() {

}

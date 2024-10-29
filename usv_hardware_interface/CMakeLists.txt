cmake_minimum_required(VERSION 3.8)
project(usv_hardware_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Include directories
include_directories(include/usv_hardware_interface)

# Generate service files
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/SetCameraAngle.srv"
)
ament_export_dependencies(rosidl_default_runtime)

# Add executables
add_executable(sensor_interface_node src/sensor_interface_node.cpp)
ament_target_dependencies(sensor_interface_node rclcpp sensor_msgs geometry_msgs)

add_executable(camera_control_server_node src/camera_control_server.cpp)
ament_target_dependencies(camera_control_server_node rclcpp std_srvs std_msgs)

# Link libraries
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)

target_link_libraries(camera_control_server_node "${cpp_typesupport_target}")

# Install targets
install(TARGETS
  sensor_interface_node
  camera_control_server_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME}
)

# Testing
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  ament_add_gtest(test_camera_control_service test/test_camera_control_service.cpp)
  ament_target_dependencies(test_camera_control_service rclcpp std_msgs)
  target_link_libraries(test_camera_control_service "${cpp_typesupport_target}")
endif()

ament_package()
# Makefile

# Variables
DOCKER_COMPOSE := $(shell if command -v docker-compose > /dev/null 2>&1; then echo "docker-compose"; else echo "docker compose"; fi)
SUBMODULES = vrx aquabot_sirhena aquabot_competitor
DEV_PACKAGES = usv_hardware_interface
SKIP_PACKAGES = package_example aquabot_example opencv_example
NO_TESTS_PACKAGES = $(SKIP_PACKAGES) vrx_gazebo vrx_ros aquabot_python wamv_description aquabot_description wamv_gazebo vrx_ros vrx_gz aquabot_gz

# Rules
.PHONY: term set-perms dc-run dc-stop dc-restart dc-build dc-rmi dc-reset ros-build ros-clean ros-run--easy-headless-competition ros-run--easy-competition ros-cs-fix ros-rqt ros-console ros-test update-submodules pull-submodules init-submodules help

# Local rules
term:
	docker exec -it --user abc ubuntu_aquabot_webtop /bin/zsh

set-perms:
	chmod -R 755 .

# Docker rules
dc-run:
	$(DOCKER_COMPOSE) up -d

dc-stop:
	$(DOCKER_COMPOSE) down

dc-restart:
	$(DOCKER_COMPOSE) restart

dc-build: set-perms
	$(DOCKER_COMPOSE) up --build -d
	docker exec ubuntu_aquabot_webtop bash -c "cp -r /opt/ros_save/humble/include/* /opt/ros/humble/include/"

dc-rmi:
	$(DOCKER_COMPOSE) down --rmi local

dc-reset: dc-rmi dc-build

# ROS rules
ros-build:
	cd /config/vrx_ws; \
	colcon build --merge-install --packages-skip $(SKIP_PACKAGES)

ros-clean:
	rm -rf /config/vrx_ws/build /config/vrx_ws/install /config/vrx_ws/log

ros-run--easy-headless-competition:
	cd /config/vrx_ws; \
	ros2 launch usv_launch main.launch.py world:=aquabot_windturbines_easy headless:=true competition_mode:=true

ros-run--easy-competition:
	cd /config/vrx_ws; \
	ros2 launch usv_launch main.launch.py world:=aquabot_windturbines_easy headless:=false competition_mode:=true

ros-cs-fix:
	@for package in $(DEV_PACKAGES); do \
		sudo uncrustify -c uncrustify.cfg --replace --no-backup $$(find $$package -name "*.cpp" -o -name "*.hpp"); \
	done

ros-rqt:
	rqt

ros-console:
	ros2 run rqt_console rqt_console

ros-test:
	cd /config/vrx_ws; \
	colcon test --merge-install --packages-skip $(NO_TESTS_PACKAGES) $$package; \
	colcon test-result --verbose

# Git rules
update-submodules:
	git submodule update --remote --merge

pull-submodules:
	for submodule in $(SUBMODULES); do \
		cd $$submodule && git pull origin main && cd -; \
	done

init-submodules:
	git submodule update --init --recursive

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Local rules: to run only on host"
	@echo "  term            Open a terminal in the container"
	@echo "  set-perms       Setup rights for the project"
	@echo ""
	@echo "Docker rules:"
	@echo "  dc-run          Run the docker containers"
	@echo "  dc-stop         Stop the docker containers"
	@echo "  dc-restart      Restart the docker containers"
	@echo "  dc-build        Build the docker containers"
	@echo "  dc-reset        Reset the docker containers"
	@echo ""
	@echo "ROS rules: to run only in container"
	@echo "  ros-build       				   Build the ROS packages"
	@echo "  ros-clean       				   Clean the ROS packages"
	@echo "  ros-run-easy-headless-competition Run the easy competition in headless mode"
	@echo "  ros-run-easy-competition 		   Run the easy competition"
	@echo "  ros-rqt         				   Run rqt tool"
	@echo "  ros-console     				   Run the ROS console"
	@echo "  ros-cs-fix      				   Fix the coding style"
	@echo "  ros-test        				   Run the tests"
	@echo ""
	@echo "Help rules:"
	@echo "  help            Show this help message"

.DEFAULT_GOAL := help
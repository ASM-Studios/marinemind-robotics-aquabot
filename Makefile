# Makefile

# Variables
DOCKER_COMPOSE := $(shell command -v docker-compose > /dev/null 2>&1 && echo "docker-compose" || echo "docker compose")
SUBMODULES = vrx aquabot_sirhena aquabot_competitor
DEV_PACKAGES = usv_hardware_interface
SKIP_PACKAGES = package_example aquabot_example opencv_example
NO_TESTS_PACKAGES = $(SKIP_PACKAGES) vrx_gazebo vrx_ros aquabot_python wamv_description aquabot_description wamv_gazebo vrx_ros vrx_gz aquabot_gz
BASE_DIR := /config/vrx_ws
USER := abc

# Rules
.PHONY: term set-perms apply-license cs-fix dc-run dc-stop dc-restart dc-build dc-rmi dc-reset ros-build ros-clean ros-run--easy-headless-competition ros-run--easy-competition ros-rqt ros-console ros-test update-submodules pull-submodules init-submodules help

# Local rules
term: is-in-local ## Open a terminal in the container
	docker exec -it --user $(USER) ubuntu_aquabot_webtop /bin/zsh

set-perms: is-in-local ## Setup rights for the project
	chmod -R 755 .

apply-license: is-in-local ## Check and apply the license
	@sudo python3 apply_license.py

cs-fix: is-in-local apply-license ## Fix the coding style
	@for package in $(DEV_PACKAGES); do \
		sudo uncrustify -c uncrustify.cfg --replace --no-backup $$(find $$package -name "*.cpp" -o -name "*.hpp"); \
	done

# Docker rules
dc-run: is-in-local ## Run the docker containers
	$(DOCKER_COMPOSE) up -d

dc-stop: is-in-local ## Stop the docker containers
	$(DOCKER_COMPOSE) down

dc-restart: is-in-local ## Restart the docker containers
	$(DOCKER_COMPOSE) restart

dc-build: is-in-local set-perms ## Build the docker containers
	$(DOCKER_COMPOSE) up --build -d
	docker exec ubuntu_aquabot_webtop bash -c "cp -r /opt/ros_save/humble/include/* /opt/ros/humble/include/"

dc-rmi: is-in-local ## Remove docker images
	$(DOCKER_COMPOSE) down --rmi local

dc-reset: is-in-local dc-rmi dc-build ## Reset the docker containers

# Location validation
is-in-dc:
	@if [ "$(shell whoami)" != $(USER) ]; then \
		echo "Error: you must use 'make term' to run this command"; \
		exit 1; \
	fi

is-in-local:
	@if [ "$(shell whoami)" = $(USER) ]; then \
		echo "Error: you must be on the host to run this command"; \
		exit 1; \
	fi

# ROS rules
ros-build: is-in-dc ## Build the ROS packages
	cd $(BASE_DIR); colcon build --merge-install --packages-skip $(SKIP_PACKAGES)

ros-clean: is-in-dc ## Clean the ROS packages
	rm -rf $(BASE_DIR)/build $(BASE_DIR)/install $(BASE_DIR)/log

ros-run-easy-headless-competition: is-in-dc ## Run the easy competition in headless mode
	cd $(BASE_DIR); ros2 launch usv_launch main.launch.py world:=aquabot_windturbines_easy headless:=true competition_mode:=true

ros-run-easy-competition: is-in-dc ## Run the easy competition
	cd $(BASE_DIR); ros2 launch usv_launch main.launch.py world:=aquabot_windturbines_easy headless:=false competition_mode:=true

ros-create-node: is-in-dc ## Create a new ROS package
	@if [ -z "$(node)" ] || [ -z "$(package)" ]; then \
		echo "Error: node and package variables must be set"; \
		echo "Usage: make ros-create-node node=<node_name> package=<package_name>"; \
		exit 1; \
	fi
	cd $(BASE_DIR)/src; ros2 pkg create --build-type ament_cmake --node-name $(node) --license GPL-3.0-only --maintainer-email maintainer@marinemindrobotics.com $(package)

ros-rqt: is-in-dc ## Run rqt tool
	rqt

ros-console: is-in-dc ## Run the ROS console
	ros2 run rqt_console rqt_console

ros-test: is-in-dc ## Run the tests
	cd $(BASE_DIR); colcon test --merge-install --packages-skip $(NO_TESTS_PACKAGES) $$package; colcon test-result --verbose

# Git rules
update-submodules: is-in-local ## Update git submodules
	git submodule update --remote --merge

pull-submodules: is-in-local ## Pull git submodules
	@for submodule in $(SUBMODULES); do \
		cd $$submodule && git pull origin main && cd -; \
	done

init-submodules: is-in-local ## Initialize git submodules
	git submodule update --init --recursive

help: ## Show this help message
	@echo "Usage: make [target]"
	@echo ""
	@echo "Local rules: to run only on host"
	@echo "  term                          Open a terminal in the container"
	@echo "  set-perms                     Setup rights for the project"
	@echo "  apply-license                 Check and apply the license"
	@echo "  cs-fix                        Fix the coding style"
	@echo ""
	@echo "Docker rules:"
	@echo "  dc-run                              Run the docker containers"
	@echo "  dc-stop                             Stop the docker containers"
	@echo "  dc-restart                          Restart the docker containers"
	@echo "  dc-build                            Build the docker containers"
	@echo "  dc-rmi                              Remove docker images"
	@echo "  dc-reset                            Reset the docker containers"
	@echo ""
	@echo "ROS rules: to run only in container"
	@echo "  ros-build                           Build the ROS packages"
	@echo "  ros-clean                           Clean the ROS packages"
	@echo "  ros-run-easy-headless-competition   Run the easy competition in headless mode"
	@echo "  ros-run-easy-competition            Run the easy competition"
	@echo "  ros-create-node                     Create a new ROS package"
	@echo "  ros-rqt                             Run rqt tool"
	@echo "  ros-console                         Run the ROS console"
	@echo "  ros-test                            Run the tests"
	@echo ""
	@echo "Git rules:"
	@echo "  update-submodules                   Update git submodules"
	@echo "  pull-submodules                     Pull git submodules"
	@echo "  init-submodules                     Initialize git submodules"
	@echo ""
	@echo "Help rules:"
	@echo "  help                                Show this help message"

.DEFAULT_GOAL := help

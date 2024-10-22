# Makefile

# Variables
DOCKER_COMPOSE := $(shell if command -v docker-compose > /dev/null 2>&1; then echo "docker-compose"; else echo "docker compose"; fi)
SUBMODULES = vrx aquabot_sirhena aquabot_competitor

# Rules
.PHONY: start stop build clean reset term term-root update-submodules pull-submodules init-submodules help

run:
	$(DOCKER_COMPOSE) up -d

stop:
	$(DOCKER_COMPOSE) down

restart:
	$(DOCKER_COMPOSE) restart

build:
	$(DOCKER_COMPOSE) up --build -d
	docker exec ubuntu_aquabot_webtop bash -c "cp -r opt/ros_save/humble/include/* /opt/ros/humble/include/"

reset: stop build

term:
	docker exec -it --user abc ubuntu_aquabot_webtop /bin/bash

term-root:
	docker exec -it ubuntu_aquabot_webtop /bin/bash

update-submodules:
	git submodule update --remote --merge

pull-submodules:
	for submodule in $(SUBMODULES); do \
		cd $$submodule && git pull origin main && cd -; \
	done

init-submodules:
	git submodule update --init --recursive

help:
	@echo "Available commands:"
	@echo "  start  - Start the Docker containers in detached mode"
	@echo "  stop   - Stop and remove the Docker containers"
	@echo "  build  - Build and start the Docker containers in detached mode"
	@echo "  clean  - Stop and remove the Docker containers, then prune unused Docker objects"
	@echo "  reset  - Clean and rebuild the Docker containers"
	@echo "  term   - Open a terminal session in the webtop container as the abc user"
	@echo "  term-root - Open a terminal session in the webtop container as the root user"
	@echo "  update-submodules - Update the submodules to the latest commit on the main branch"
	@echo "  pull-submodules   - Pull the latest changes from the main branch of the submodules"
	@echo "  init-submodules   - Initialize the submodules"
	@echo "  help   - Display this help message"

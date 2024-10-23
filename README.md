# Aquabot project

## Overview
This project sets up a development environment using Docker for ROS 2 Humble and Gazebo Garden. It includes cloning and building the VRX and Aquabot repositories.

## Prerequisites
- Docker
- Docker Compose

## Setup

1. **Clone the repository:**
    ```sh
    git clone git@github.com:ASM-Studios/marinemind-robotics-aquabot.git
    cd marinemind-robotics-aquabot
    ```

[//]: # (2. **Initialize submodules:**)

[//]: # (    ```sh)

[//]: # (    make init-submodules)

[//]: # (    ```)

2. **Build and run the Docker containers:**
    ```sh
    make build
    make run
    ```

## Usage
- ROS 2 Humble and Gazebo Garden are pre-installed and sourced.
- Open Ubuntu Aquabot Webtop on [http://localhost:3000](http://localhost:3000) in your browser.

[//]: # (## Updating Submodules)

[//]: # (To update the submodules to the latest commit from the remote repository, run:)

[//]: # (```sh)

[//]: # (make update-submodules)

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Documentation

- [Project architecture](docs/architecture.md)
- [ROS good practices](docs/good-practices.md)
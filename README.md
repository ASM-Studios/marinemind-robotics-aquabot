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
    make dc-build
    ```

3. **Open a new terminal and build ROS packages:**
   ```sh
   make term
   ```
- Ignore the `/config/.zshrc:source:3: no such file or directory: /config/vrx_ws/install/setup.zsh` error and run:
   ```sh
   make ros-build
   ```
- Exit the terminal.

4. **Go to the Aquabot Webtop:**
    - Open [http://localhost:3000](http://localhost:3000) in your browser.
    - Open the terminal in the Webtop and run:
        ```sh
        make ros-run--easy-competition
        ```

## Development
- Run `make ros-cs-fix` to fix coding standards issues.
  - If you have a permission denied error, run `make set-perms` from your local host. 
- Run `make ros-test` to run tests.
- To avoid GUI consumption, use headless ros-run rules like `make ros-run--easy-headless-competition`.

## ROS tools
- Run `make ros-console` to open a GUI ROS console.
- Run `make ros-rqt` to open a GUI RQT.

## Troubleshooting
If you got this error:  
`Package 'usv_launch' not found: "package 'usv_launch' not found, searching: ['/opt/ros/humble']"`
- Be sure that you have already run `make ros-build`.
- Close the terminal and reopen it (sourcing is necessary).

[//]: # (## Updating Submodules)

[//]: # (To update the submodules to the latest commit from the remote repository, run:)

[//]: # (```sh)

[//]: # (make update-submodules)

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Documentation

- [Project architecture](docs/architecture.md)
- [ROS good practices](docs/good-practices.md)
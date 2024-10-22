FROM ghcr.io/linuxserver/baseimage-kasmvnc:ubuntujammy
# ubuntu 22.04 base image

ARG DEBIAN_FRONTEND="noninteractive"

# title
ENV TITLE="Ubuntu KDE"

# Prevent Ubuntu's firefox stub from being installed
COPY /root/etc/apt/preferences.d/firefox-no-snap /etc/apt/preferences.d/firefox-no-snap

# Copy repository content
COPY . /config/vrx_ws/src/

# Install base requirements packages
RUN \
  curl -o \
    /kclient/public/icon.png \
    https://raw.githubusercontent.com/linuxserver/docker-templates/master/linuxserver.io/img/webtop-logo.png && \
  add-apt-repository -y ppa:mozillateam/ppa && \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive \
  apt-get install --no-install-recommends -y \
    dolphin \
    firefox \
    gwenview \
    kde-config-gtk-style \
    kdialog \
    kfind \
    khotkeys \
    kio-extras \
    knewstuff-dialog \
    konsole \
    ksystemstats \
    kubuntu-settings-desktop \
    kubuntu-wallpapers \
    kubuntu-web-shortcuts \
    kwin-addons \
    kwin-x11 \
    kwrite \
    plasma-desktop \
    plasma-workspace \
    plymouth-theme-kubuntu-logo \
    qml-module-qt-labs-platform \
    systemsettings \
    locales \
    software-properties-common \
    curl && \
  sed -i \
    's/applications:org.kde.discover.desktop,/applications:org.kde.konsole.desktop,/g' \
    /usr/share/plasma/plasmoids/org.kde.plasma.taskmanager/contents/config/main.xml

# Set locale \
RUN \
  locale-gen en_US en_US.UTF-8 && \
  update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 && \
  export LANG=en_US.UTF-8

# Install ROS2 \
RUN \
  add-apt-repository universe && \
  apt-get update && \
  curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg && \
  sh -c 'echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" > /etc/apt/sources.list.d/ros2.list' && \
  apt-get update && \
  apt-get upgrade -y && \
  apt-get install -y ros-humble-desktop ros-dev-tools && \
  apt-get install -y lsb-release gnupg

# Install Gazebo \
RUN \
  curl https://packages.osrfoundation.org/gazebo.gpg --output /usr/share/keyrings/pkgs-osrf-archive-keyring.gpg && \
  sh -c 'echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/pkgs-osrf-archive-keyring.gpg] http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" > /etc/apt/sources.list.d/gazebo-stable.list' && \
  apt-get update && \
  apt-get install -y gz-garden

# Install VRX \
RUN \
  apt-get install -y \
    python3-sdformat13 \
    ros-humble-ros-gzgarden \
    ros-humble-xacro \
    python3-colcon-common-extensions && \
  cd /config/vrx_ws/src && \
  chown -R 1000:1000 /config/vrx_ws && \
  chmod -R 755 /config/vrx_ws && \
  bash -c "source /opt/ros/humble/setup.bash && cd /config/vrx_ws && colcon build --merge-install && . install/setup.bash" && \
  chown -R 1000:1000 /config/vrx_ws && \
  chmod -R 755 /config/vrx_ws

# cleanup \
RUN \
  apt-get autoclean && \
  rm -rf \
    /config/.cache \
    /config/.launchpadlib \
    /var/lib/apt/lists/* \
    /var/tmp/* \
    /tmp/*

# Set correct ownership and permissions for /config/.XDG
RUN mkdir -p /config/.XDG && \
    chown 1000:1000 /config/.XDG && \
    chmod 0700 /config/.XDG

# add local files
COPY /root /

# ports and volumes
EXPOSE 3000
VOLUME /config

RUN cp -r /opt/ros/ /opt/ros_save

# source ROS2 setup script
RUN echo "source /opt/ros/humble/setup.bash" >> /config/.bashrc
RUN echo "source /config/vrx_ws/install/setup.bash" >> /config/.bashrc

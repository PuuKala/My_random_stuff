# Exiting on any error
set -e

# The required Linux packages
sudo apt-get -y install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

# Optional packages
sudo apt-get -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

# My personal preferrence of CMake GUI, works on ssh terminal
sudo apt-get -y install cmake-curses-gui

# Clone version 4.4.0 shallow(due to memory restrictions)
git clone --depth=1 -b 4.4.0 https://github.com/opencv/opencv.git
cd opencv

# Make build dir and build
mkdir build
cd build
ccmake ..
make

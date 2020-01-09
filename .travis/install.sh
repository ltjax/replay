#!/bin/bash

set -e
set -x

uname -s
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 60 --slave /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION}
sudo update-alternatives --config gcc
wget https://dl.bintray.com/conan/installers/conan-ubuntu-64_1_20_5.deb
sudo dpkg -i ./conan-ubuntu-64_1_20_5.deb
conan --version
conan config install conan_config/

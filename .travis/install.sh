#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Linux' ]]; then
    sudo apt-get install g++-${GCC_VERSION}
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 60 --slave /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION}
    sudo update-alternatives --config gcc
    wget https://dl.bintray.com/conan/installers/conan-ubuntu-64_1_20_5.deb
    sudo dpkg -i ./conan-ubuntu-64_1_20_5.deb
    conan --version
    conan config install conan_config/
    conan profile new default --detect
    conan profile update settings.compiler.libcxx=libstdc++11 default
fi

if [[ "$(uname -s)" == 'Darwin' ]]; then
    brew update || brew update
    brew outdated pyenv || brew upgrade pyenv
    brew install pyenv-virtualenv
    brew install cmake || true

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi

    pyenv install 2.7.10
    pyenv virtualenv 2.7.10 conan
    pyenv rehash
    pyenv activate conan

    pip install conan --upgrade
    pip install conan_package_tools
    
    conan user
fi

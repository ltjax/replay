project(replay)

cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED Replay_USE_STBIMAGE)
  option(Replay_USE_STBIMAGE "Use stb_image.c to load image files" ON)
endif()

if(NOT DEFINED Replay_USE_LIBPNG)
  option(Replay_USE_LIBPNG "Use libpng to load png files" ON)
endif()

if(NOT DEFINED Replay_ENABLE_UNIT_TESTS)
  option(Replay_ENABLE_UNIT_TESTS "Build the unit tests" ON)
endif()

if(NOT DEFINED Replay_ENABLE_INSTALL)
  option(Replay_ENABLE_INSTALL "Create the install target" ON)
endif()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build)

if(Replay_USE_CONAN)
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(Replay_BOOST_TARGETS CONAN_PKG::boost_filesystem CONAN_PKG::boost_math)
else()
  # Replay is typically included in a host project that already has boost
  if(NOT Boost_FOUND AND NOT DEFINED Replay_BOOST_TARGETS)
    if(WIN32)
      find_package(Boost REQUIRED)
    else()
      set(Boost_USE_MULTITHREADED OFF)
        
      if(Replay_ENABLE_UNIT_TESTS)
          find_package(Boost 1.42 REQUIRED COMPONENTS unit_test_framework)
      else()
          find_package(Boost 1.42)
      endif()
    endif()
  endif()

  if(Replay_USE_LIBPNG)
    find_package(PNG REQUIRED)
  endif()
  
  if(Replay_USE_STBIMAGE AND NOT DEFINED Replay_STBIMAGE_PATH AND NOT Replay_USE_STBIMAGE_FROM_CONAN)
    find_path(Replay_STBIMAGE_PATH stb_image.h)
  endif()
endif()

add_subdirectory(source)

if(Replay_ENABLE_UNIT_TESTS)
  add_subdirectory(test)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.7.8)
  target_sources(replay INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/replay.natvis
  )
endif()

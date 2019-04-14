project(replay)

cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED REPLAY_USE_STBIMAGE)
  option(REPLAY_USE_STBIMAGE "Use stb_image.c to load image files" ON)
endif()

if(NOT DEFINED REPLAY_USE_LIBPNG)
  option(REPLAY_USE_LIBPNG "Use libpng to load png files" ON)
endif()

if(NOT DEFINED REPLAY_ENABLE_UNIT_TESTS)
  option(REPLAY_ENABLE_UNIT_TESTS "Build the unit tests" ON)
endif()

if(NOT DEFINED REPLAY_ENABLE_INSTALL)
  option(REPLAY_ENABLE_INSTALL "Create the install target" ON)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build)

# Replay is typically included in a host project that already has boost
if(NOT Boost_FOUND AND NOT DEFINED REPLAY_INJECTED_BOOST_TARGETS)
  if(WIN32)
    find_package(Boost REQUIRED)
  else()
    set(Boost_USE_MULTITHREADED OFF)
      
    if(REPLAY_ENABLE_UNIT_TESTS)
        find_package(Boost 1.42 REQUIRED COMPONENTS unit_test_framework)
    else()
        find_package(Boost 1.42)
    endif()
  endif()
endif()

link_directories(${Boost_LIBRARY_DIRS})

if(REPLAY_USE_LIBPNG)
	find_package(PNG REQUIRED)
endif()

if(REPLAY_USE_STBIMAGE AND NOT DEFINED REPLAY_STBIMAGE_PATH)
	find_path(REPLAY_STBIMAGE_PATH stb_image.h)
endif()

add_subdirectory(source)

if(REPLAY_ENABLE_UNIT_TESTS)
	add_subdirectory(test)
endif()

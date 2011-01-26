project( replay )

cmake_minimum_required( VERSION 2.8 )

set( Boost_ADDITIONAL_VERSIONS "1.42.0" "1.42" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${replay_BINARY_DIR}/build )

set( Boost_USE_STATIC_LIBS OFF )
if( WIN32 )
find_package( Boost 1.42 REQUIRED )
else( WIN32 )
set( Boost_USE_MULTITHREADED OFF )
find_package( Boost 1.42 REQUIRED COMPONENTS unit_test_framework )
endif( WIN32 )

find_package( PNG QUIET )
if( PNG_FOUND )
message("libPNG found, using it.")
endif( PNG_FOUND )

add_subdirectory( source )
add_subdirectory( test )

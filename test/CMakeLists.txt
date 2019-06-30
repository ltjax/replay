
set(TARGET_NAME replay_test)

add_executable(${TARGET_NAME}
  test_main.cpp
  math_test.cpp 
  index_map_test.cpp 
  minibox_test.cpp
  planar_direction_test.cpp
)

target_link_libraries(${TARGET_NAME}
  PUBLIC replay
)

if (Replay_USE_CONAN)
  target_link_libraries(${TARGET_NAME}
	PUBLIC CONAN_PKG::Catch2
  )
endif()

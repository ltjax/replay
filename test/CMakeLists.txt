
set(TARGET_NAME replay_test)

add_executable(${TARGET_NAME}
  test_main.cpp
  math.t.cpp 
  index_map.t.cpp 
  minibox.t.cpp
  planar_direction.t.cpp
  rle_vector.t.cpp
  table.t.cpp
  vector2.t.cpp
  vector3.t.cpp
  pixbuf.t.cpp
  byte_rgba.t.cpp
  vector_math.t.cpp
)

target_link_libraries(${TARGET_NAME}
  PUBLIC replay
)

if (Replay_USE_CONAN)
  target_link_libraries(${TARGET_NAME}
	PUBLIC CONAN_PKG::catch2
  )
endif()

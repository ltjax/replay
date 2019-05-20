set(TARGET_NAME replay)

set(HEADER_FILES
  ${replay_SOURCE_DIR}/include/replay/aabb.hpp
  ${replay_SOURCE_DIR}/include/replay/affinity.hpp
  ${replay_SOURCE_DIR}/include/replay/bounding_rectangle.hpp
  ${replay_SOURCE_DIR}/include/replay/box.hpp
  ${replay_SOURCE_DIR}/include/replay/box_packer.hpp
  ${replay_SOURCE_DIR}/include/replay/bstream.hpp
  ${replay_SOURCE_DIR}/include/replay/buffer.hpp
  ${replay_SOURCE_DIR}/include/replay/byte_color.hpp
  ${replay_SOURCE_DIR}/include/replay/common.hpp
  ${replay_SOURCE_DIR}/include/replay/couple.hpp
  ${replay_SOURCE_DIR}/include/replay/lines3.hpp
  ${replay_SOURCE_DIR}/include/replay/math.hpp
  ${replay_SOURCE_DIR}/include/replay/matrix2.hpp
  ${replay_SOURCE_DIR}/include/replay/matrix3.hpp
  ${replay_SOURCE_DIR}/include/replay/matrix4.hpp
  ${replay_SOURCE_DIR}/include/replay/minimal_sphere.hpp
  ${replay_SOURCE_DIR}/include/replay/pixbuf.hpp
  ${replay_SOURCE_DIR}/include/replay/pixbuf_io.hpp
  ${replay_SOURCE_DIR}/include/replay/plane3.hpp
  ${replay_SOURCE_DIR}/include/replay/quaternion.hpp
  ${replay_SOURCE_DIR}/include/replay/table.hpp
  ${replay_SOURCE_DIR}/include/replay/transformation.hpp
  ${replay_SOURCE_DIR}/include/replay/vector_math.hpp
  ${replay_SOURCE_DIR}/include/replay/vector2.hpp
  ${replay_SOURCE_DIR}/include/replay/vector2.inl
  ${replay_SOURCE_DIR}/include/replay/vector3.hpp
  ${replay_SOURCE_DIR}/include/replay/vector3.inl
  ${replay_SOURCE_DIR}/include/replay/vector4.hpp
  ${replay_SOURCE_DIR}/include/replay/vector4.inl
)
  
set(SOURCE_FILES
  aabb.cpp
  box_packer.cpp
  byte_color.cpp
  math.cpp
  matrix2.cpp
  matrix3.cpp
  matrix4.cpp
  pixbuf.cpp
  pixbuf_io.cpp
  plane3.cpp
  quaternion.cpp
  vector_math.cpp
)

add_library(replay STATIC ${HEADER_FILES} ${SOURCE_FILES})

# Default dependencies
target_include_directories(${TARGET_NAME}
  PUBLIC ${replay_SOURCE_DIR}/include)

if (DEFINED Replay_BOOST_TARGETS)
  target_link_libraries(${TARGET_NAME}
    PUBLIC ${Replay_BOOST_TARGETS})
	
  # Disable warning for deprecated header inclusion integer_log2.hpp - this can be removed for boost >= 1.70.0
  target_compile_definitions(${TARGET_NAME}
    PUBLIC -DBOOST_ALLOW_DEPRECATED_HEADERS)
else()
  target_include_directories(${TARGET_NAME} SYSTEM
    PUBLIC ${Boost_INCLUDE_DIR})
  
  target_link_libraries(${TARGET_NAME}
    ${Boost_LIBRARIES})
endif()

# Conditionally use stbimage
if(Replay_USE_STBIMAGE)
  target_compile_definitions(${TARGET_NAME}
    PUBLIC -DReplay_USE_STBIMAGE
    PRIVATE -DSTBI_NO_STDIO)
  target_include_directories(${TARGET_NAME}
    PRIVATE ${Replay_STBIMAGE_PATH})
endif()

# Conditionally use stbimage_write
if(Replay_USE_STBIMAGE_WRITE)
  target_compile_definitions(${TARGET_NAME}
    PUBLIC -DReplay_USE_STBIMAGE_WRITE
    PRIVATE -DSTBI_WRITE_NO_STDIO)
  target_include_directories(${TARGET_NAME}
    PRIVATE ${Replay_STBIMAGE_WRITE_PATH})
endif()

if(UNIX)
  set_target_properties(replay PROPERTIES COMPILE_FLAGS -fPIC)
endif()

if(Replay_ENABLE_INSTALL)
  install(FILES ${replay_HEADERS} DESTINATION include/replay)
  install(TARGETS replay ARCHIVE DESTINATION lib)
endif()

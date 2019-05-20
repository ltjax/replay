
set(TARGET_NAME replay_test)

add_executable(${TARGET_NAME} 
  math_test.cpp 
  minibox_test.cpp
)

target_link_libraries(${TARGET_NAME}
  PUBLIC replay
)

if (Replay_USE_CONAN)
  target_link_libraries(${TARGET_NAME}
    PUBLIC CONAN_PKG::boost_random
	PUBLIC CONAN_PKG::boost_test
	PUBLIC CONAN_PKG::boost_assign
  )
endif()

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${TARGET_NAME}
  ARGS --report_level=short --result_code=no
)

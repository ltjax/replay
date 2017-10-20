
set(TARGET_NAME replay-test)

add_executable(${TARGET_NAME} 
  math_test.cpp 
  minibox_test.cpp
)

target_link_libraries(${TARGET_NAME}
  replay
)

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${TARGET_NAME}
  ARGS --report_level=short --result_code=no)

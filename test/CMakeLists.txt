include_directories( ${replay_SOURCE_DIR}/include 
             ${Boost_INCLUDE_DIR} )

set( TestSources
    math_test.cpp 
    minibox_test.cpp )

if( CMAKE_COMPILER_IS_GNUCXX )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_TEST_DYN_LINK" )
endif( CMAKE_COMPILER_IS_GNUCXX )

link_directories(
  ${Boost_LIBRARY_DIRS}
)

add_executable( replay-test ${TestSources} )

target_link_libraries( replay-test
            replay
            ${Boost_LIBRARIES} )

add_custom_command( TARGET replay-test
                    POST_BUILD
            COMMAND replay-test ARGS --report_level=short )

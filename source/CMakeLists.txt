if( PNG_FOUND )
include_directories( ${replay_SOURCE_DIR}/include 
                     ${PNG_INCLUDE_DIR} )
else( PNG_FOUND )
include_directories( ${replay_SOURCE_DIR}/include )
endif( PNG_FOUND )

file( GLOB replay_HEADERS ${replay_SOURCE_DIR}/include/replay/*.hpp )

set( replaySources
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
	vector_math.cpp )
	 
add_library( replay STATIC ${replaySources} )

if( PNG_FOUND )
target_link_libraries( replay
                       ${PNG_LIBRARIES} )
endif( PNG_FOUND )

install( FILES ${replay_HEADERS} DESTINATION include/replay )
install( TARGETS replay ARCHIVE DESTINATION lib )
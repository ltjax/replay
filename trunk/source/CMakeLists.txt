if( PNG_FOUND )
include_directories( ${replay_SOURCE_DIR}/include 
					 ${Boost_INCLUDE_DIR}
					 ${PNG_INCLUDE_DIR} )
set( replay_HEADERS
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
	${replay_SOURCE_DIR}/include/replay/vector4.inl )
	
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
else( PNG_FOUND )
include_directories( ${replay_SOURCE_DIR}/include
					 ${Boost_INCLUDE_DIR} )
set( replay_HEADERS
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
	${replay_SOURCE_DIR}/include/replay/vector4.inl )
	
set( replaySources
	aabb.cpp
	box_packer.cpp
	byte_color.cpp
	math.cpp
	matrix2.cpp
	matrix3.cpp
	matrix4.cpp
	plane3.cpp
	quaternion.cpp
	vector_math.cpp )
endif( PNG_FOUND )


	 
add_library( replay STATIC ${replay_HEADERS} ${replaySources} )
if( UNIX )
set_target_properties( replay PROPERTIES COMPILE_FLAGS -fPIC )
endif( UNIX )

if( PNG_FOUND )
target_link_libraries( replay
					   ${PNG_LIBRARIES} )
endif( PNG_FOUND )

install( FILES ${replay_HEADERS} DESTINATION include/replay )
install( TARGETS replay ARCHIVE DESTINATION lib )
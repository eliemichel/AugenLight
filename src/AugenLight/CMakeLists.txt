
add_executable (
	AugenLight

	utils/debug.h
	utils/debug.cpp
	utils/window.h
	utils/window.cpp
	utils/shader.h
	utils/shader.cpp

	main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(AugenLight LINK_PRIVATE glad)
target_link_libraries(AugenLight LINK_PRIVATE glfw)

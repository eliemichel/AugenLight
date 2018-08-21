# Glad (v0.1.27), providing headers for OpenGL procedures
# Here it is configured to link to OpenGL 4.5 Core profile, but you can change
# the two following lines to get another one. If you do so, you must also
# change the window creation flags.
set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
set(GLAD_API "gl=4.5,gles2=" CACHE STRING "" FORCE)
add_subdirectory(glad)
set_property(TARGET glad PROPERTY FOLDER "External")

# GLFW (3.2.1), for openning the window and getting mouse/keyboard events
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_property(TARGET glfw PROPERTY FOLDER "External")

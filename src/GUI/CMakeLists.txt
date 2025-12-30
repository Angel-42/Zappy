cmake_minimum_required(VERSION 3.16)
project(ZappyGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeFiles/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeCXXCompilerId.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/Ile")
list(FILTER SOURCES EXCLUDE REGEX ".*/Renderer/2D/main.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/Renderer/2D/")

file(GLOB CLIENT_API_SOURCES 
    "../ClientAPI/*.cpp"
    "../ClientAPI/Model/*.cpp"
    "../ClientAPI/Network/*.cpp"
)
list(FILTER CLIENT_API_SOURCES EXCLUDE REGEX ".*test_main.cpp$")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Event
)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(SFML REQUIRED sfml-all)
find_package(GLEW REQUIRED)

# Executable
add_executable(zappy_gui
    ${SOURCES}
    ${CLIENT_API_SOURCES}
)

# Link libraries
target_link_libraries(zappy_gui
	${OPENGL_LIBRARIES}
	${SFML_LIBRARIES}
	${GLEW_LIBRARIES}
	GL
	sfml-graphics
	sfml-window
	sfml-system
)

# For glm (header-only)
target_include_directories(zappy_gui PRIVATE /usr/include/glm)

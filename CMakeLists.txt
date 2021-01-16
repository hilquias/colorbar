cmake_minimum_required(VERSION 3.10)

project(colorbar)

FILE(GLOB SOURCE_FILES src/*.c src/*/*.c)

add_executable(colorbar ${SOURCE_FILES})

add_definitions(-std=c11 -pedantic -Wall -O3)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
# find_package(SOIL REQUIRED)
find_package(cglm REQUIRED)

target_link_libraries(colorbar GL GLEW glfw SOIL cglm m)

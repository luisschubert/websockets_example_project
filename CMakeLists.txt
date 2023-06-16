cmake_minimum_required(VERSION 3.10)

project(websocket_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost 1.67.0 REQUIRED COMPONENTS system thread)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(websocket_example_server websocket_example_server.cpp)
add_executable(websocket_example_client websocket_example_client.cpp)

# Link libraries
target_link_libraries(websocket_example_server ${Boost_LIBRARIES})
target_link_libraries(websocket_example_client ${Boost_LIBRARIES})

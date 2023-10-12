cmake_minimum_required(VERSION 3.24)
project(tutorial)

set(CMAKE_CXX_STANDARD 17)
set(LUA_DIRECTORY /opt/homebrew/Cellar/lua/5.4.6)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_DIRECTORY}/include)
target_link_libraries(${PROJECT_NAME} ${LUA_DIRECTORY}/lib/liblua.a)

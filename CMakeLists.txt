cmake_minimum_required(VERSION 3.12)
project(GrooveFirmware)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(GrooveFirmware ${SOURCE_FILES})
target_link_libraries(GrooveFirmware portaudio)
target_link_libraries(GrooveFirmware pthread)
target_link_libraries(GrooveFirmware ncurses)
target_link_libraries(GrooveFirmware m)
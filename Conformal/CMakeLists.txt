cmake_minimum_required(VERSION 3.0)
project(conformal)

aux_source_directory(${CMAKE_SOURCE_DIR} SRC_LIST) 
include_directories(${CMAKE_SOURCE_DIR})
add_library(circle SHARED ${SRC_LIST})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
install(FILES f.hh DESTINATION /home/lihao/root/include)
install(TARGETS circle LIBRARY DESTINATION /home/lihao/root/lib)



cmake_minimum_required(VERSION 2.8)
project( kondo_b3m_driver )

ADD_DEFINITIONS(
-g
-O0
-Wall
-std=c++11
)

add_executable(sample 
  ./src/sample.cpp
) 
target_link_libraries(sample)

add_executable(reset 
  ./src/reset.cpp
) 
target_link_libraries(reset)

add_executable(read 
  ./src/read.cpp
) 
target_link_libraries(read)

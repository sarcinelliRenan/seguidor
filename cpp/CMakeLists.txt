##################################### 
cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)
set (CMAKE_CXX_STANDARD 11)
add_subdirectory (libs)
add_executable (simpletest_raspicam main.cpp) 
target_link_libraries (simpletest_raspicam raspicam m)
#####################################

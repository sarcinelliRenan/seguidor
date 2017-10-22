##################################### 
cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)
set (CMAKE_CXX_STANDARD 11)
set(raspicam_DIR "/usr/local/lib/cmake")
find_package(raspicam REQUIRED) 
add_executable (simpletest_raspicam main.cpp) 
target_link_libraries (simpletest_raspicam ${raspicam_LIBS})
#####################################

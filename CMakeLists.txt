# CMakeList.txt : CMake project for AobaTest
cmake_minimum_required(VERSION 3.12)
project(AobaTest)
add_executable(${PROJECT_NAME})
add_subdirectory(src)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#AobaAPI
add_subdirectory(libs/AobaAPI)
target_link_libraries(${PROJECT_NAME} "AobaAPI" "${AOBAAPI_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/AobaAPI/include")

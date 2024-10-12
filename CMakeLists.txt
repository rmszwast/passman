cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(passman VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add CLI11 to the project
add_subdirectory(lib/CLI11)


# set CLI11 to compile to a static library
set(CLI11_PRECOMPILED True)

# add Botan to the project
add_subdirectory(lib/botan)

# add the executable
add_executable(
    ${CMAKE_PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}.cpp
) 

# link libraries to the executable
target_link_libraries(${CMAKE_PROJECT_NAME} CLI11::CLI11 botan)

# expand search path for include files
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/include )
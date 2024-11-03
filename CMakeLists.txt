cmake_minimum_required(VERSION 3.20)

# set the project name and version
project(passman VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# verify Git is installed
find_package(Git REQUIRED)
if(NOT Git_FOUND)
    message(FATAL_ERROR "fatal: Git not found")
endif(NOT Git_FOUND)

# verify Python is installed
find_package(Python3 REQUIRED)
if(NOT Python3_FOUND)
    message(FATAL_ERROR "fatal: Python not found")
endif(NOT Python3_FOUND)

# get libraries
execute_process(
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ${GIT_EXECUTABLE} submodule update --init
    RESULT_VARIABLE EXIT_CODE
)
if(NOT EXIT_CODE EQUAL "0")
    message(FATAL_ERROR "fatal: failed to run ${GIT_EXECUTABLE} submodule update --init")
endif(NOT EXIT_CODE EQUAL "0")

# add libraries to the project
add_subdirectory(lib/CLI11)
add_subdirectory(lib/yaml-cpp)
add_subdirectory(lib/Catch2)

# set CLI11 to compile to a static library
set(CLI11_PRECOMPILED True)

# find Botan library
set(Botan_DIR ${PROJECT_SOURCE_DIR}/lib/botan/build/lib/cmake/Botan-3.6.0) 
find_package(Botan QUIET)
if(NOT Botan_FOUND)
    # configure make targets
    execute_process(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/botan/
        COMMAND ${Python3_EXECUTABLE} configure.py
            --disable-shared
            --prefix=${PROJECT_SOURCE_DIR}/lib/botan/build
            --disable-deprecated-features
        RESULT_VARIABLE EXIT_CODE
    )
    if(NOT EXIT_CODE EQUAL "0")
    message(FATAL_ERROR "fatal: failed to run ${Python3_EXECUTABLE} /workspaces/passman/lib/botan/configure.py")
    endif(NOT EXIT_CODE EQUAL "0")
    # build make targets
    execute_process(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/botan/
        COMMAND make install
    )
    set(Botan_DIR ${PROJECT_SOURCE_DIR}/lib/botan/build/lib/cmake/Botan-3.6.0) 
    find_package(Botan)
endif(NOT Botan_FOUND)

# add the executable
add_executable(
    ${CMAKE_PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}.cpp
) 

# link libraries to the executable
target_link_libraries(${CMAKE_PROJECT_NAME}
    CLI11::CLI11
    yaml-cpp::yaml-cpp
    Catch2::Catch2WithMain
    Botan::Botan-static
)

# expand search path for include files
target_include_directories(
    ${CMAKE_PROJECT_NAME} 
    PUBLIC ${PROJECT_SOURCE_DIR}/src/include
)
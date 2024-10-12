#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug -S ${PROJECT_SOURCE_DIR} -B ${PROJECT_SOURCE_DIR}/debug
cmake --build ${PROJECT_SOURCE_DIR}/debug
#cmake -DCMAKE_BUILD_TYPE=Release -S ${PROJECT_SOURCE_DIR} -B ${PROJECT_SOURCE_DIR}/release
#cmake --build ${PROJECT_SOURCE_DIR}/release

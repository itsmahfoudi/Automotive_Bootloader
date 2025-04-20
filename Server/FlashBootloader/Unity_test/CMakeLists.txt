cmake_minimum_required(VERSION 3.0)
project(MyUnityProject)

# Set source directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unity)

# Add Unity as a library
add_library(Unity STATIC ${UNITY_DIR}/src/unity.c)

# Add your source files
add_executable(my_tests ${TEST_DIR}/test_main.c ${SOURCE_DIR}/main.c)

# Include directories
target_include_directories(my_tests PRIVATE ${UNITY_DIR}/src ${SOURCE_DIR})

# Link Unity to your test executable
target_link_libraries(my_tests Unity)

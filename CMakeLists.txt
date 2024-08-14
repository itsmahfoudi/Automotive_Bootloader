# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.13)
# ##############################################################################
# Set project name and source code folder location

project(${PROJECT_NAME} C)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)
set(CM4_dir ${CMAKE_CURRENT_SOURCE_DIR}/CM4)
set(CM7_dir ${CMAKE_CURRENT_SOURCE_DIR}/CM7)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")
option(DUMP_ASM "Create full assembly of final executable" OFF)

# Set microcontroller information
set(MCU_FAMILY STM32H7xx)

set(CM4_MCU_MODEL STM32H755xx CORE_CM4)
set(CM4_CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

set(CM7_MCU_MODEL STM32H755xx CORE_CM7)
set(CM7_CPU_PARAMETERS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=hard)

set(CM4_STARTUP_SCRIPT ${CM4_dir}/Core/Startup/startup_stm32h755zitx.s)
set(CM4_LINKER_SCRIPT ${CM4_dir}/STM32H755ZITX_FLASH.ld)
set(CM7_STARTUP_SCRIPT ${CM7_dir}/Core/Startup/startup_stm32h755zitx.s)
set(CM7_LINKER_SCRIPT ${CM7_dir}/STM32H755ZITX_FLASH.ld)

# ##############################################################################

set(CM4_EXE ${CMAKE_PROJECT_NAME}-cm4)
set(CM7_EXE ${CMAKE_PROJECT_NAME}-cm7)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    USE_HAL_DRIVER)

#

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wdouble-promotion
  -Wformat=2
  -Wformat-truncation
  # -Wundef
  -Wno-unused-parameter
  -fno-common
  "$<$<COMPILE_LANGUAGE:CXX>:-Wconversion;-Wno-volatile;-Wold-style-cast;-Wuseless-cast;-Wsuggest-override>"
  "$<$<CONFIG:Debug>:-Og;-g3;-ggdb>"
  "$<$<CONFIG:Release>:-Og;-g0>")

add_link_options(
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage)

# ##############################################################################

set(STM32CUBEMX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H7xx_Nucleo
    )

file(GLOB_RECURSE STM32CUBEMX_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/BSP/STM32H7xx_Nucleo/*.c
)

# ##############################################################################
# CM4
file(GLOB_RECURSE CM4_SOURCES CONFIGURE_DEPENDS
    ${CM4_dir}/Core/*.c)

add_executable(${CM4_EXE}
    ${STM32CUBEMX_SOURCES}
    ${CM4_SOURCES}
    ${CM4_STARTUP_SCRIPT})

target_compile_definitions(${CM4_EXE} PRIVATE
    ${CM4_MCU_MODEL})

target_include_directories(${CM4_EXE} SYSTEM PRIVATE
    ${STM32CUBEMX_INCLUDE_DIRECTORIES})

target_include_directories(${CM4_EXE} PRIVATE
    ${CM4_dir}/Core/Inc)

target_compile_options(${CM4_EXE} PRIVATE
    ${CM4_CPU_PARAMETERS})

target_link_options(${CM4_EXE} PRIVATE
    -T${CM4_LINKER_SCRIPT}
    -Wl,-Map=${CM4_EXE}.map
    ${CM4_CPU_PARAMETERS})

# CM7
file(GLOB_RECURSE CM7_SOURCES CONFIGURE_DEPENDS
    ${CM7_dir}/Core/*.c)

add_executable(${CM7_EXE}
    ${STM32CUBEMX_SOURCES}
    ${CM7_SOURCES}
    ${CM7_STARTUP_SCRIPT})

target_compile_definitions(${CM7_EXE} PRIVATE
    ${CM7_MCU_MODEL})

target_include_directories(${CM7_EXE} SYSTEM PRIVATE
    ${STM32CUBEMX_INCLUDE_DIRECTORIES})

target_include_directories(${CM7_EXE} PRIVATE
    ${CM7_dir}/Core/Inc)

target_compile_options(${CM7_EXE} PRIVATE
    ${CM7_CPU_PARAMETERS})

target_link_options(${CM7_EXE} PRIVATE
    -T${CM7_LINKER_SCRIPT}
    -Wl,-Map=${CM7_EXE}.map
    ${CM7_CPU_PARAMETERS})

# ##############################################################################

function(make_output EXECUTABLE)
    add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin)

    if(${DUMP_ASM})
        add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -D -C $<TARGET_FILE:${EXECUTABLE}> > ${EXECUTABLE}.s)
    endif()
endfunction()

make_output(${CM4_EXE})
make_output(${CM7_EXE})

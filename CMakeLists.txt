cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME cold-storage-controller)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Chip type
add_compile_definitions(STM32G070xx)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Inc
set(FREERTOS_INC ${CMAKE_SOURCE_DIR}/FreeRTOS-Kernel-G0/include)
set(APP_INC ${CMAKE_SOURCE_DIR}/app/inc)
set(CORE_INC ${CMAKE_SOURCE_DIR}/Core/Inc)
#stm32g0xx.h
set(DEVICE_INC ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include) 
set(FREERTOS_ARM_INC ${CMAKE_SOURCE_DIR}/FreeRTOS-Kernel-G0/portable/GCC/ARM_CM0)
#core_cm0plus.h
set(CMSIS_INC ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include)
set(HAL_INC ${CMAKE_SOURCE_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(FreeRTOS-Kernel-G0)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    app
    lib
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 
            ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)

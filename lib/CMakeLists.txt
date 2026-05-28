add_library(lib 
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adc_filter.c
)

target_include_directories(lib
    PUBLIC
    ${APP_INC}
    ${CMSIS_INC}
    ${DEVICE_INC}
    ${CORE_INC}
    ${FREERTOS_INC}
    ${FREERTOS_ARM_INC}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
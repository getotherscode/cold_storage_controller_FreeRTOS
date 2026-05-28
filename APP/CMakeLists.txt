# source file dependencies and generate library target
## create target library target 
add_library(app STATIC
    src/adc_app.c
    src/alarm.c
    src/compressor.c
    src/log.c
    src/uart_app.c
)
## add target library target
target_sources(app PRIVATE

)

#sub module head files access interface
target_include_directories(app PUBLIC
    ${APP_INC}
    ${HAL_INC}
    ${CORE_INC}
    ${CMSIS_INC}
    ${DEVICE_INC}
    ${FREERTOS_INC}
    ${FREERTOS_ARM_INC}
    ${CMAKE_SOURCE_DIR}/lib/inc
)
#specific compile options
#sub-dependency
#link script and output target
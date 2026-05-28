# cold-storage-controller
Refrigeration control system firmware based on STM32 and FreeRTOS, featuring EEV control, VFD compressor management, condenser fan control and RS485 communication.

## build
--- CSCF
 |---app(write)
 |    |---CMakeLists
 |---cmake(auto generate by cubemx)
 |    |---CMakeLists(Core,Drivers)
 |    |---arm-eabi-none-gcc.cmake
 |---Core
 |---Drivers
 |---FreeRTOS(write)
 |    |---CMakeLists
 |---lib(write)
 |    |---CMakeLists
 |---test(write)
      |---CMakeLists
### software
1>CMake
2>Ninja
3>arm-none-ebai-gcc

### cmd
1>cd build
2>cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
3>ninja

## download and Debug

### software
xopenocd

### hardware
WCH DAP-Link, not specified tool, you can use whichever ones you like

### download cmd
openocd -f interface/cmsis-dap.cfg -f target/stm32g0x.cfg \
        -c "program D:/GitProject/cold-storage-controller/build/cold-storage-controller.hex verify reset exit"

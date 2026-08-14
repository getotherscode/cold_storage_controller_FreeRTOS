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

#### debug
1> cmake --preset Debug  
2> ninja -C build/Debug  

#### release
1> cmake --preset Release  
2> ninja -C build/Release  

## FLASH and RAM layout

### FLASH
1) bootloader 0x08000000-0x08004FFF size: 20K
2) main app   0x08005000-0x0801D000 size: 104K
3) crash log  0x0801E000-0x0801E7FF size: 2K
4) boot mark  0x0801E800-0x0801EFFF size: 2K

### RAM
1) common ram 0x20000000-0x20008BFF 35K
2) crash log  0x20000000-0x20009000 1K


## download and Debug

### software
xopenocd  

### hardware
WCH DAP-Link: not specified tool, you can use whichever ones you like  
drivers download link: https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html  

### download cmd
openocd -f interface/cmsis-dap.cfg -f target/stm32g0x.cfg\    
        -c "program ./build/Debug/cold-storage-controller.hex verify reset exit"

## test

### unity test
1> where: https://www.throwtheswitch.org/unity#unity-get-section  
2> clone: git clone https://github.com/ThrowTheSwitch/Unity.git  
3> needed files: 1)unity.c 2)unity.h 3) unity_internals.h  
4> where to put: test/unity  
5> verify test: in test folder, type "make", execute "test_main.exe"  

### mock test

## crash dump

### thirdparty utils
CMBackTrace

### locate abnormal code line
arm-none-eabi-addr2line -e ./build/Debug/cold-storage-controller.elf -f -C ${PC_value}/${LR_value}

### disassembly
arm-none-eabi-objdump -dS your_project.elf > disasm.txt

## database proxy design
any data read and write operation should be executed by database proxy safety functions  

## usart duties
-usart4: output debug info, sample data, runtime data
-usart2: data read, ota, data write

## references

### I2C 
UM10204 — I2C-bus specification and user manual

### EEP 
AT24C04RB  
https://item.szlcsc.com/82297.html?fromZone=s_s__%252224c04%2522&spm=sc.gbn.xh1.zy.n___sc.hm.hst.2&lcsc_vid=E1hXA1cEFlheBAACEVENVgcDRlkLU1UFRlYMUQBeQAUxVlNeRVVZVlVVT1leUjtW

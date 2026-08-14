#pragma once
#include <stdint.h>

/*
    cortex-M0+, ARMv6-M, Thumb
    R0 ~ R12    general-purpose register
    SP          Stack Pointer
    LR          Link Register
    PC          Program Counter
    xPSR        Program Status Register
    cfsr        configureable Fault Status Register
    hfsr        hard fault
    dfsr        debug fault
    afsr        auxiliary fault
    mmfar       memory management fault address
    bfar        bus fault address
*/

typedef struct
{
    uint32_t magic;

    uint32_t fault_type;
    uint32_t reset_reason;

    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;

    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t dfsr;
    uint32_t afsr;
    uint32_t mmfar;
    uint32_t bfar;

    uint32_t crc;

} Crash_Record_ST;

/*
    watch dog 
    reset cause
*/

/*
    software error
    active record
*/

void hardfault_hander_c(uint32_t *stack);

#include "crash_dump.h"

static Crash_Record_ST crash_record_st;

void hardfault_hander_c(uint32_t *stack)
{
    uint32_t r0   = stack[0];
    uint32_t r1   = stack[1];
    uint32_t r2   = stack[2];
    uint32_t r3   = stack[3];
    uint32_t r12  = stack[4];
    uint32_t lr   = stack[5];
    uint32_t pc   = stack[6];
    uint32_t xpsr = stack[7];

    crash_record_st.r0 = r0;
    crash_record_st.r1 = r1;
    crash_record_st.r2 = r2;
    crash_record_st.r3 = r3;
    crash_record_st.r12 = r12;
    crash_record_st.lr = lr;
    crash_record_st.pc = pc;
    crash_record_st.xpsr = xpsr;

    while (1)
    {
    }
}

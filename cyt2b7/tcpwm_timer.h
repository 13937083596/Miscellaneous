/**
 * CYT2B7 TCPWM timer — TCPWM0_GRP0_CNT0, 1 s tick @ TC interrupt.
 * Ref: Infineon AN220224 (TRAVEO T2G TCPWM).
 */
#ifndef CYT2B7_TCPWM_TIMER_H
#define CYT2B7_TCPWM_TIMER_H

#include "cy_project.h"

/* ponytail: board clock tree may differ — measure peri clock and retune */
#define CYT2B7_PERI_CLK_HZ   80000000u
#define CYT2B7_CNT_CLK_HZ    2000000u   /* after 16-bit peri divider */
#define CYT2B7_TICK_HZ       15625u     /* cnt_clk / prescaler_div128 */
#define CYT2B7_TICK_PERIOD    (CYT2B7_TICK_HZ - 1u)

#define CYT2B7_TIMER_CNT      TCPWM0_GRP0_CNT0
#define CYT2B7_TIMER_PCLK      PCLK_TCPWM0_CLOCKS0
#define CYT2B7_TIMER_DIV_NO    0u

extern volatile uint32_t cyt2b7_timer_ticks;

void cyt2b7_timer_init(void);
void cyt2b7_timer_isr(void);

#endif

#include "tcpwm_timer.h"

volatile uint32_t cyt2b7_timer_ticks;

static cy_stc_tcpwm_counter_config_t const timer_cfg = {
    .period             = CYT2B7_TICK_PERIOD,
    .clockPrescaler     = CY_TCPWM_COUNTER_PRESCALER_DIVBY_128,
    .runMode            = CY_TCPWM_PWM_CONTINUOUS,
    .countDirection     = CY_TCPWM_COUNTER_COUNT_UP,
    .debug_pause        = false,
    .CompareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE,
    .compare0           = 0u,
    .compare0_buff      = 0u,
    .compare1           = 0u,
    .compare1_buff      = 0u,
    .enableCompare0Swap = false,
    .enableCompare1Swap = false,
    .interruptSources   = 0u,
    .capture0InputMode  = 3u,
    .capture0Input      = 0u,
    .reloadInputMode    = 3u,
    .reloadInput        = 0u,
    .startInputMode     = 3u,
    .startInput         = 0u,
    .stopInputMode      = 3u,
    .stopInput          = 0u,
    .capture1InputMode  = 3u,
    .capture1Input      = 0u,
    .countInputMode     = 3u,
    .countInput         = 1u,
    .trigger1           = CY_TCPWM_COUNTER_OVERFLOW,
};

static cy_stc_sysint_irq_t const timer_irq = {
    .sysIntSrc = tcpwm_0_interrupts_0_IRQn,
    .intIdx    = CPUIntIdx3_IRQn,
    .isEnabled = true,
};

void cyt2b7_timer_init(void)
{
    uint32_t div = CYT2B7_PERI_CLK_HZ / CYT2B7_CNT_CLK_HZ;
    cy_stc_sysint_irq_t irq = timer_irq;

    __enable_irq();

    Cy_SysClk_PeriphAssignDivider(CYT2B7_TIMER_PCLK, CY_SYSCLK_DIV_16_BIT, CYT2B7_TIMER_DIV_NO);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, CYT2B7_TIMER_DIV_NO, div - 1u);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, CYT2B7_TIMER_DIV_NO);

    Cy_SysInt_InitIRQ(&irq);
    Cy_SysInt_SetSystemIrqVector(irq.sysIntSrc, cyt2b7_timer_isr);
    NVIC_SetPriority(irq.intIdx, 3u);
    NVIC_EnableIRQ(irq.intIdx);

    Cy_Tcpwm_Counter_Init(CYT2B7_TIMER_CNT, &timer_cfg);
    Cy_Tcpwm_Counter_SetTC_IntrMask(CYT2B7_TIMER_CNT);
    Cy_Tcpwm_Counter_Enable(CYT2B7_TIMER_CNT);
    Cy_Tcpwm_TriggerStart(CYT2B7_TIMER_CNT);
}

void cyt2b7_timer_isr(void)
{
    if (Cy_Tcpwm_Counter_GetTC_IntrMasked(CYT2B7_TIMER_CNT) == 1u) {
        Cy_Tcpwm_Counter_ClearTC_Intr(CYT2B7_TIMER_CNT);
        cyt2b7_timer_ticks++;
    }
}

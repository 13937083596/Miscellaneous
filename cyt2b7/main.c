/**
 * Minimal CYT2B7 entry — clocks come from startup/cybsp, timer from tcpwm_timer.
 * ponytail: link tcpwm_timer.c, include BSP/SDL paths for cy_project.h.
 */
#include "cy_project.h"
#include "tcpwm_timer.h"

int main(void)
{
#if defined(CY_USING_HAL) || defined(COMPONENT_CYBSP)
    if (cybsp_init() != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }
#endif

    cyt2b7_timer_init();

    for (;;) {
        /* cyt2b7_timer_ticks increments every 1 s — watch in debugger */
    }
}

/*
 * my_timer.c
 *
 *  Created on: Jun 24, 2016
 *      Author: Michael
 */

#include "my_timer.h"
#include "../inc/hw_timer.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../driverlib/debug.h"
#include "../driverlib/sysctl.h"

//*****************************************************************************
//! Initialize timers used for sound visualizer
//!
//! \return None.
//*****************************************************************************
void InitializeTimers()
{
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);				 //Configure Timer0: 32-bit periodic mode for ADC Timer Trigger
	TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet()/32000)-1);	 //16KHz interrupt
	TimerControlTrigger(TIMER0_BASE, TIMER_A, true); 			 	//Configure Timer0 to generate trigger event for ADC
	TimerEnable(TIMER0_BASE, TIMER_A);							 	//Enable Timer0
}

//*****************************************************************************
//! Enables the timer(s).
//!
//! \param ulBase is the base address of the timer module.
//! \param ulTimer specifies the timer(s) to enable; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//!
//! This function enables operation of the timer module.  The timer must be
//! configured before it is enabled.
//!
//! \return None.
//*****************************************************************************
void TimerEnable(unsigned long ulBase, unsigned long ulTimer)
{
    // Check the arguments.
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    // Enable the timer(s) module.
    HWREG(ulBase + TIMER_O_CTL) |= ulTimer & (TIMER_CTL_TAEN | TIMER_CTL_TBEN);
}

//*****************************************************************************
//! Configures the timer(s).
//!
//! \param ulBase is the base address of the timer module.
//! \param ulConfig is the configuration for the timer.
//!
//! This function configures the operating mode of the timer(s).  The timer
//! module is disabled before being configured and is left in the disabled
//! state.  There are two types of timers; a 16/32-bit variety and a 32/64-bit
//! variety.  The 16/32-bit variety is comprised of two 16-bit timers that can
//! operate independently or be concatenated to form a 32-bit timer.
//! Similarly, the 32/64-bit variety is comprised of two 32-bit timers that can
//! operate independently or be concatenated to form a 64-bit timer.
//!
//! The configuration is specified in \e ulConfig as one of the following
//! values:
//!
//! - \b TIMER_CFG_ONE_SHOT - Full-width one-shot timer
//! - \b TIMER_CFG_ONE_SHOT_UP - Full-width one-shot timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_PERIODIC - Full-width periodic timer
//! - \b TIMER_CFG_PERIODIC_UP - Full-width periodic timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_RTC - Full-width real time clock timer
//! - \b TIMER_CFG_SPLIT_PAIR - Two half-width timers
//!
//! When configured for a pair of half-width timers, each timer is separately
//! configured.  The first timer is configured by setting \e ulConfig to
//! the result of a logical OR operation between one of the following values
//! and \e ulConfig:
//!
//! - \b TIMER_CFG_A_ONE_SHOT - Half-width one-shot timer
//! - \b TIMER_CFG_A_ONE_SHOT_UP - Half-width one-shot timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_PERIODIC - Half-width periodic timer
//! - \b TIMER_CFG_A_PERIODIC_UP - Half-width periodic timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_CAP_COUNT - Half-width edge count capture
//! - \b TIMER_CFG_A_CAP_COUNT_UP - Half-width edge count capture that counts
//!   up instead of down (not available on all parts)
//! - \b TIMER_CFG_A_CAP_TIME - Half-width edge time capture
//! - \b TIMER_CFG_A_CAP_TIME_UP - Half-width edge time capture that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_PWM - Half-width PWM output
//!
//! Similarly, the second timer is configured by setting \e ulConfig to
//! the result of a logical OR operation between one of the corresponding
//! \b TIMER_CFG_B_* values and \e ulConfig.
//!
//! \return None.
//*****************************************************************************
void TimerConfigure(unsigned long ulBase, unsigned long ulConfig)
{
    // Check the arguments.
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulConfig == TIMER_CFG_ONE_SHOT) ||
           (ulConfig == TIMER_CFG_ONE_SHOT_UP) ||
           (ulConfig == TIMER_CFG_PERIODIC) ||
           (ulConfig == TIMER_CFG_PERIODIC_UP) ||
           (ulConfig == TIMER_CFG_RTC) ||
           ((ulConfig & 0xff000000) == TIMER_CFG_SPLIT_PAIR));
    ASSERT(((ulConfig & 0xff000000) != TIMER_CFG_SPLIT_PAIR) ||
           ((((ulConfig & 0x000000ff) == TIMER_CFG_A_ONE_SHOT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_ONE_SHOT_UP) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PERIODIC) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PERIODIC_UP) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_COUNT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_TIME) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PWM)) &&
            (((ulConfig & 0x0000ff00) == TIMER_CFG_B_ONE_SHOT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_ONE_SHOT_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PERIODIC) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PERIODIC_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_COUNT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_COUNT_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_TIME) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_TIME_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PWM))));

    // Disable the timers.
    HWREG(ulBase + TIMER_O_CTL) &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);

    // Set the global timer configuration.
    HWREG(ulBase + TIMER_O_CFG) = ulConfig >> 24;

    // Set the configuration of the A and B timers.  Note that the B timer
    // configuration is ignored by the hardware in 32-bit modes.
    HWREG(ulBase + TIMER_O_TAMR) = (ulConfig & 255) | TIMER_TAMR_TAPWMIE;
    HWREG(ulBase + TIMER_O_TBMR) =
        ((ulConfig >> 8) & 255) | TIMER_TBMR_TBPWMIE;
}

//*****************************************************************************
//! Enables or disables the ADC trigger output.
//!
//! \param ulBase is the base address of the timer module.
//! \param ulTimer specifies the timer to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//! \param bEnable specifies the desired ADC trigger state.
//!
//! This function controls the ADC trigger output for the specified timer.  If
//! the \e bEnable parameter is \b true, then the timer's ADC output trigger is
//! enabled; otherwise it is disabled.
//!
//! \return None.
//*****************************************************************************
void TimerControlTrigger(unsigned long ulBase, unsigned long ulTimer, int bEnable)
{
    // Check the arguments.
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    // Set the trigger output as requested.
    // Set the ADC trigger output as requested.
    ulTimer &= TIMER_CTL_TAOTE | TIMER_CTL_TBOTE;
    HWREG(ulBase + TIMER_O_CTL) = (bEnable ?
                                   (HWREG(ulBase + TIMER_O_CTL) | ulTimer) :
                                   (HWREG(ulBase + TIMER_O_CTL) & ~(ulTimer)));
}

//*****************************************************************************
//! Sets the timer load value.
//!
//! \param ulBase is the base address of the timer module.
//! \param ulTimer specifies the timer(s) to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.  Only \b TIMER_A should be used when the
//! timer is configured for full-width operation.
//! \param ulValue is the load value.
//!
//! This function configures the timer load value; if the timer is running then
//! the value is immediately loaded into the timer.
//!
//! \note This function can be used for both full- and half-width modes of
//! 16/32-bit timers and for half-width modes of 32/64-bit timers.  Use
//! TimerLoadSet64() for full-width modes of 32/64-bit timers.
//!
//! \return None.
//*****************************************************************************
void TimerLoadSet(unsigned long ulBase, unsigned long ulTimer,
             unsigned long ulValue)
{
    // Check the arguments.
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    // Set the timer A load value if requested.
    if(ulTimer & TIMER_A)
    {
        HWREG(ulBase + TIMER_O_TAILR) = ulValue;
    }

    // Set the timer B load value if requested.
    if(ulTimer & TIMER_B)
    {
        HWREG(ulBase + TIMER_O_TBILR) = ulValue;
    }
}

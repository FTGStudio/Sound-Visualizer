/*
 * my_timer.h
 *
 *  Created on: Jun 24, 2016
 *      Author: Michael
 */

#ifndef SRC_MY_DRIVERLIB_MY_TIMER_H_
#define SRC_MY_DRIVERLIB_MY_TIMER_H_

//*****************************************************************************
// Values that can be passed to TimerConfigure as the ulConfig parameter.
//*****************************************************************************
#define TIMER_CFG_ONE_SHOT       0x00000021  // Full-width one-shot timer
#define TIMER_CFG_ONE_SHOT_UP    0x00000031  // Full-width one-shot up-count
                                            // timer
#define TIMER_CFG_PERIODIC       0x00000022  // Full-width periodic timer
#define TIMER_CFG_PERIODIC_UP    0x00000032  // Full-width periodic up-count
                                            // timer
#define TIMER_CFG_RTC            0x01000000  // Full-width RTC timer
#define TIMER_CFG_SPLIT_PAIR     0x04000000  // Two half-width timers
#define TIMER_CFG_A_ONE_SHOT     0x00000021  // Timer A one-shot timer
#define TIMER_CFG_A_ONE_SHOT_UP  0x00000031  // Timer A one-shot up-count timer
#define TIMER_CFG_A_PERIODIC     0x00000022  // Timer A periodic timer
#define TIMER_CFG_A_PERIODIC_UP  0x00000032  // Timer A periodic up-count timer
#define TIMER_CFG_A_CAP_COUNT    0x00000003  // Timer A event counter
#define TIMER_CFG_A_CAP_COUNT_UP 0x00000013  // Timer A event up-counter
#define TIMER_CFG_A_CAP_TIME     0x00000007  // Timer A event timer
#define TIMER_CFG_A_CAP_TIME_UP  0x00000017  // Timer A event up-count timer
#define TIMER_CFG_A_PWM          0x0000000A  // Timer A PWM output
#define TIMER_CFG_B_ONE_SHOT     0x00002100  // Timer B one-shot timer
#define TIMER_CFG_B_ONE_SHOT_UP  0x00003100  // Timer B one-shot up-count timer
#define TIMER_CFG_B_PERIODIC     0x00002200  // Timer B periodic timer
#define TIMER_CFG_B_PERIODIC_UP  0x00003200  // Timer B periodic up-count timer
#define TIMER_CFG_B_CAP_COUNT    0x00000300  // Timer B event counter
#define TIMER_CFG_B_CAP_COUNT_UP 0x00001300  // Timer B event up-counter
#define TIMER_CFG_B_CAP_TIME     0x00000700  // Timer B event timer
#define TIMER_CFG_B_CAP_TIME_UP  0x00001700  // Timer B event up-count timer
#define TIMER_CFG_B_PWM          0x00000A00  // Timer B PWM output

//*****************************************************************************
// Values that can be passed to most of the timer APIs as the ulTimer
// parameter.
//*****************************************************************************
#define TIMER_A                 0x000000ff  // Timer A
#define TIMER_B                 0x0000ff00  // Timer B
#define TIMER_BOTH              0x0000ffff  // Timer Both

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void InitializeTimers();

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void TimerEnable(unsigned long ulBase, unsigned long ulTimer);
extern void TimerConfigure(unsigned long ulBase, unsigned long ulConfig);
extern void TimerControlTrigger(unsigned long ulBase, unsigned long ulTimer, int bEnable);
extern void TimerLoadSet(unsigned long ulBase, unsigned long ulTimer, unsigned long ulValue);

#endif /* SRC_MY_DRIVERLIB_MY_TIMER_H_ */

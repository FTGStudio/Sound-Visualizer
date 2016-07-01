/*
 * my_adc.c
 *
 *  Created on: Jun 24, 2016
 *      Author: Michael
 */

#include "my_adc.h"
#include "../inc/hw_adc.h"
#include "../inc/hw_ints.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../inc/hw_sysctl.h"
#include "../driverlib/debug.h"
#include "../driverlib/interrupt.h"

//*****************************************************************************
//
// These defines are used by the ADC driver to simplify access to the ADC
// sequencer's registers.
//
//*****************************************************************************
#define ADC_SEQ                 (ADC_O_SSMUX0)
#define ADC_SEQ_STEP            (ADC_O_SSMUX1 - ADC_O_SSMUX0)
#define ADC_SSMUX               (ADC_O_SSMUX0 - ADC_O_SSMUX0)
#define ADC_SSEMUX              (ADC_O_SSMUX0 - ADC_O_SSMUX0)
#define ADC_SSCTL               (ADC_O_SSCTL0 - ADC_O_SSMUX0)
#define ADC_SSFIFO              (ADC_O_SSFIFO0 - ADC_O_SSMUX0)
#define ADC_SSFSTAT             (ADC_O_SSFSTAT0 - ADC_O_SSMUX0)
#define ADC_SSOP                (ADC_O_SSOP0 - ADC_O_SSMUX0)
#define ADC_SSDC                (ADC_O_SSDC0 - ADC_O_SSMUX0)


void InitializeADC()
{
	ADCSequenceDisable(ADC0_BASE, 3);			//Disable Sequence 3 in order to safely configure ADC

	//Configure Sequence 3: processor trigger, priority=0
	ADCSequenceConfigure(ADC_BASE, 3, ADC_TRIGGER_TIMER, 0);

	//Configure Sequence 3, step 0, analog input 0 | interrupt | end of sequence
	ADCSequenceStepConfigure(ADC_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 3);	//Enable sequence 3
	ADCIntEnable(ADC0_BASE, 3);			//Enable interrupt for pin
	IntEnable(INT_ADC3);				//Enable interrupt in interrupt controller
}

//*****************************************************************************
//! Enables a sample sequence.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//!
//! Allows the specified sample sequence to be captured when its trigger is
//! detected.  A sample sequence must be configured before it is enabled.
//!
//! \return None.
//*****************************************************************************
void ADCSequenceEnable(unsigned long ulBase, unsigned long ulSequenceNum)
{
    // Check the arugments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);

    // Enable the specified sequence.
    HWREG(ulBase + ADC_O_ACTSS) |= 1 << ulSequenceNum;
}

//*****************************************************************************
//! Disables a sample sequence.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//!
//! Prevents the specified sample sequence from being captured when its trigger
//! is detected.  A sample sequence should be disabled before it is configured.
//!
//! \return None.
//*****************************************************************************
void ADCSequenceDisable(unsigned long ulBase, unsigned long ulSequenceNum)
{
    // Check the arugments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);

    // Disable the specified sequences.
    HWREG(ulBase + ADC_O_ACTSS) &= ~(1 << ulSequenceNum);
}

//*****************************************************************************
//! Configures the trigger source and priority of a sample sequence.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//! \param ulTrigger is the trigger source that initiates the sample sequence;
//! must be one of the \b ADC_TRIGGER_* values.
//! \param ulPriority is the relative priority of the sample sequence with
//! respect to the other sample sequences.
//!
//! This function configures the initiation criteria for a sample sequence.
//! Valid sample sequencers range from zero to three; sequencer zero captures
//! up to eight samples, sequencers one and two capture up to four samples,
//! and sequencer three captures a single sample.  The trigger condition and
//! priority (with respect to other sample sequencer execution) are set.
//!
//! The \e ulTrigger parameter can take on the following values:
//!
//! - \b ADC_TRIGGER_PROCESSOR - A trigger generated by the processor, via the
//!                              ADCProcessorTrigger() function.
//! - \b ADC_TRIGGER_COMP0 - A trigger generated by the first analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_COMP1 - A trigger generated by the second analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_COMP2 - A trigger generated by the third analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_EXTERNAL - A trigger generated by an input from the Port
//!                             B4 pin. Note that some microcontrollers can
//!                             select from any GPIO using the
//!                             GPIOADCTriggerEnable() function.
//! - \b ADC_TRIGGER_TIMER - A trigger generated by a timer; configured with
//!                          TimerControlTrigger().
//! - \b ADC_TRIGGER_PWM0 - A trigger generated by the first PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM1 - A trigger generated by the second PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM2 - A trigger generated by the third PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM3 - A trigger generated by the fourth PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_ALWAYS - A trigger that is always asserted, causing the
//!                           sample sequence to capture repeatedly (so long as
//!                           there is not a higher priority source active).
//!
//! Note that not all trigger sources are available on all Stellaris family
//! members; consult the data sheet for the device in question to determine the
//! availability of triggers.
//!
//! The \e ulPriority parameter is a value between 0 and 3, where 0 represents
//! the highest priority and 3 the lowest.  Note that when programming the
//! priority among a set of sample sequences, each must have unique priority;
//! it is up to the caller to guarantee the uniqueness of the priorities.
//!
//! \return None.
//*****************************************************************************
void ADCSequenceConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
                     unsigned long ulTrigger, unsigned long ulPriority)
{
    // Check the arugments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);
    ASSERT((ulTrigger == ADC_TRIGGER_PROCESSOR) ||
           (ulTrigger == ADC_TRIGGER_COMP0) ||
           (ulTrigger == ADC_TRIGGER_COMP1) ||
           (ulTrigger == ADC_TRIGGER_COMP2) ||
           (ulTrigger == ADC_TRIGGER_EXTERNAL) ||
           (ulTrigger == ADC_TRIGGER_TIMER) ||
           (ulTrigger == ADC_TRIGGER_PWM0) ||
           (ulTrigger == ADC_TRIGGER_PWM1) ||
           (ulTrigger == ADC_TRIGGER_PWM2) ||
           (ulTrigger == ADC_TRIGGER_PWM3) ||
           (ulTrigger == ADC_TRIGGER_ALWAYS));
    ASSERT(ulPriority < 4);

    // Compute the shift for the bits that control this sample sequence.
    ulSequenceNum *= 4;

    // Set the trigger event for this sample sequence.
    HWREG(ulBase + ADC_O_EMUX) = ((HWREG(ulBase + ADC_O_EMUX) &
                                   ~(0xf << ulSequenceNum)) |
                                  ((ulTrigger & 0xf) << ulSequenceNum));

    // Set the priority for this sample sequence.
    HWREG(ulBase + ADC_O_SSPRI) = ((HWREG(ulBase + ADC_O_SSPRI) &
                                    ~(0xf << ulSequenceNum)) |
                                   ((ulPriority & 0x3) << ulSequenceNum));
}



//*****************************************************************************
//! Configure a step of the sample sequencer.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//! \param ulStep is the step to be configured.
//! \param ulConfig is the configuration of this step; must be a logical OR of
//! \b ADC_CTL_TS, \b ADC_CTL_IE, \b ADC_CTL_END, \b ADC_CTL_D, one of the
//! input channel selects (\b ADC_CTL_CH0 through \b ADC_CTL_CH23), and one of
//! the digital comparator selects (\b ADC_CTL_CMP0 through \b ADC_CTL_CMP7).
//!
//! This function configures the ADC for one step of a sample sequence.  The
//! ADC can be configured for single-ended or differential operation
//! (the \b ADC_CTL_D bit selects differential operation when set), the
//! channel to be sampled can be chosen (the \b ADC_CTL_CH0 through
//! \b ADC_CTL_CH23 values), and the internal temperature sensor can be
//! selected (the \b ADC_CTL_TS bit).  Additionally, this step can be defined
//! as the last in the sequence (the \b ADC_CTL_END bit) and it can be
//! configured to cause an interrupt when the step is complete (the
//! \b ADC_CTL_IE bit).  If the digital comparators are present on the device,
//! this step may also be configured to send the ADC sample to the selected
//! comparator using \b ADC_CTL_CMP0 through \b ADC_CTL_CMP7. The configuration
//! is used by the ADC at the appropriate time when the trigger for
//! this sequence occurs.
//!
//! \note If the Digitial Comparator is present and enabled using the
//! \b ADC_CTL_CMP0 through \b ADC_CTL_CMP7 selects, the ADC sample is NOT
//! written into the ADC sequence data FIFO.
//!
//! The \e ulStep parameter determines the order in which the samples are
//! captured by the ADC when the trigger occurs.  It can range from zero to
//! seven for the first sample sequencer, from zero to three for the second and
//! third sample sequencer, and can only be zero for the fourth sample
//! sequencer.
//!
//! Differential mode only works with adjacent channel pairs (for example, 0
//! and 1).  The channel select must be the number of the channel pair to
//! sample (for example, \b ADC_CTL_CH0 for 0 and 1, or \b ADC_CTL_CH1 for 2
//! and 3) or undefined results are returned by the ADC.  Additionally, if
//! differential mode is selected when the temperature sensor is being sampled,
//! undefined results are returned by the ADC.
//!
//! It is the responsibility of the caller to ensure that a valid configuration
//! is specified; this function does not check the validity of the specified
//! configuration.
//!
//! \return None.
//*****************************************************************************
void ADCSequenceStepConfigure(unsigned long ulBase, unsigned long ulSequenceNum,
                         unsigned long ulStep, unsigned long ulConfig)
{
    unsigned long ulTemp;

    // Check the arugments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);
    ASSERT(((ulSequenceNum == 0) && (ulStep < 8)) ||
           ((ulSequenceNum == 1) && (ulStep < 4)) ||
           ((ulSequenceNum == 2) && (ulStep < 4)) ||
           ((ulSequenceNum == 3) && (ulStep < 1)));

    // Get the offset of the sequence to be configured.
    ulBase += ADC_SEQ + (ADC_SEQ_STEP * ulSequenceNum);

    // Compute the shift for the bits that control this step.
    ulStep *= 4;

    // Set the analog mux value for this step.
    HWREG(ulBase + ADC_SSMUX) = ((HWREG(ulBase + ADC_SSMUX) &
                                  ~(0x0000000f << ulStep)) |
                                 ((ulConfig & 0x0f) << ulStep));

    // Set the upper bits of the analog mux value for this step.
    HWREG(ulBase + ADC_SSEMUX) = ((HWREG(ulBase + ADC_SSEMUX) &
                                  ~(0x0000000f << ulStep)) |
                                  (((ulConfig & 0xf00) >> 8) << ulStep));

    // Set the control value for this step.
    HWREG(ulBase + ADC_SSCTL) = ((HWREG(ulBase + ADC_SSCTL) &
                                  ~(0x0000000f << ulStep)) |
                                 (((ulConfig & 0xf0) >> 4) << ulStep));

    // Enable digital comparator if specified in the ulConfig bit-fields.
    if(ulConfig & 0x000F0000)
    {
        // Program the comparator for the specified step.
        ulTemp = HWREG(ulBase + ADC_SSDC);
        ulTemp &= ~(0xF << ulStep);
        ulTemp |= (((ulConfig & 0x00070000) >> 16) << ulStep);
        HWREG(ulBase + ADC_SSDC) = ulTemp;

        // Enable the comparator.
        ulTemp = HWREG(ulBase + ADC_SSOP);
        ulTemp |= (1 << ulStep);
        HWREG(ulBase + ADC_SSOP) = ulTemp;
    }

    // Disable digital comparator if not specified.
    else
    {
        ulTemp = HWREG(ulBase + ADC_SSOP);
        ulTemp &= ~(1 << ulStep);
        HWREG(ulBase + ADC_SSOP) = ulTemp;
    }
}

//*****************************************************************************
//! Enables a sample sequence interrupt.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function enables the requested sample sequence interrupt.  Any
//! outstanding interrupts are cleared before enabling the sample sequence
//! interrupt.
//!
//! \return None.
//*****************************************************************************
void ADCIntEnable(unsigned long ulBase, unsigned long ulSequenceNum)
{
    // Check the arguments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);

    // Clear any outstanding interrupts on this sample sequence.
    HWREG(ulBase + ADC_O_ISC) = 1 << ulSequenceNum;

    // Enable this sample sequence interrupt.
    HWREG(ulBase + ADC_O_IM) |= 1 << ulSequenceNum;
}

//*****************************************************************************
//! Clears sample sequence interrupt source.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//!
//! The specified sample sequence interrupt is cleared, so that it no longer
//! asserts.  This function must be called in the interrupt handler to keep
//! the interrupt from being triggered again immediately upon exit.
//!
//! \note Because there is a write buffer in the Cortex-M processor, it may
//! take several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (because the interrupt controller still sees
//! the interrupt source asserted).
//!
//! \return None.
//*****************************************************************************
void ADCIntClear(unsigned long ulBase, unsigned long ulSequenceNum)
{
    // Check the arugments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);

    // Clear the interrupt.
    HWREG(ulBase + ADC_O_ISC) = 1 << ulSequenceNum;
}

//*****************************************************************************
//! Gets the captured data for a sample sequence.
//!
//! \param ulBase is the base address of the ADC module.
//! \param ulSequenceNum is the sample sequence number.
//! \param pulBuffer is the address where the data is stored.
//!
//! This function copies data from the specified sample sequencer output FIFO
//! to a memory resident buffer.  The number of samples available in the
//! hardware FIFO are copied into the buffer, which is assumed to be large
//! enough to hold that many samples.  This function only returns the samples
//! that are presently available, which may not be the entire sample sequence
//! if it is in the process of being executed.
//!
//! \return Returns the number of samples copied to the buffer.
//*****************************************************************************
long ADCSequenceDataGet(unsigned long ulBase, unsigned long ulSequenceNum,
                   unsigned long *pulBuffer)
{
    unsigned long ulCount;

    // Check the arguments.
    ASSERT((ulBase == ADC0_BASE) || (ulBase == ADC1_BASE));
    ASSERT(ulSequenceNum < 4);

    // Get the offset of the sequence to be read.
    ulBase += ADC_SEQ + (ADC_SEQ_STEP * ulSequenceNum);

    // Read samples from the FIFO until it is empty.
    ulCount = 0;
    while(!(HWREG(ulBase + ADC_SSFSTAT) & ADC_SSFSTAT0_EMPTY) && (ulCount < 8))
    {
        // Read the FIFO and copy it to the destination.
        *pulBuffer++ = HWREG(ulBase + ADC_SSFIFO);

        // Increment the count of samples read.
        ulCount++;
    }

    // Return the number of samples read.
    return(ulCount);
}

/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc.h"
#include "fsl_clock.h"
#include "fsl_power.h"

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_BASE ADC0
#define ADC_SAMPLE_CHANNEL_NUMBER 0U
#define ADC_CLOCK_DIVIDER 5U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
adc_result_info_t adcResultInfoStruct;
const uint32_t g_Adc_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void ADC_ClockPower_Configuration(void)
{
    /* SYSCON power. */
    POWER_DisablePD(kPDRUNCFG_PD_VDDA);    /* Power on VDDA. */
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);    /* Power on the ADC converter. */
    POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA); /* Power on the analog power supply. */
    POWER_DisablePD(kPDRUNCFG_PD_VREFP);   /* Power on the reference voltage source. */
    POWER_DisablePD(kPDRUNCFG_PD_TS);      /* Power on the temperature sensor. */

    CLOCK_EnableClock(kCLOCK_Adc0); /* SYSCON->AHBCLKCTRL[0] |= SYSCON_AHBCLKCTRL_ADC0_MASK; */
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL220M();
    BOARD_InitDebugConsole();

    /* Enable the power and clock for ADC. */
    ADC_ClockPower_Configuration();
    PRINTF("ADC basic example.\r\n");

#if !(defined(FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC) && FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC)
#if defined(FSL_FEATURE_ADC_HAS_CALIB_REG) && FSL_FEATURE_ADC_HAS_CALIB_REG
    /* Calibration after power up. */
    if (ADC_DoSelfCalibration(ADC_BASE))
#else
    uint32_t frequency;
#if defined(SYSCON_ADCCLKDIV_DIV_MASK)
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
#else
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE);
#endif /* SYSCON_ADCCLKDIV_DIV_MASK */
    /* Calibration after power up. */
    if (ADC_DoSelfCalibration(ADC_BASE, frequency))
#endif /* FSL_FEATURE_ADC_HAS_CALIB_REG */
    {
        PRINTF("ADC_DoSelfCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_DoSelfCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC */

    /* Configure the converter and work mode. */
    ADC_Configuration();
    PRINTF("Configuration Done.\r\n");

#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL */
    while (1)
    {
        /* Get the input from terminal and trigger the converter by software. */
        GETCHAR();
        ADC_DoSoftwareTriggerConvSeqA(ADC_BASE);

        /* Wait for the converter to be done. */
        while (!ADC_GetChannelConversionResult(ADC_BASE, ADC_SAMPLE_CHANNEL_NUMBER, &adcResultInfoStruct))
        {
        }
        PRINTF("adcResultInfoStruct.result        = %d\r\n", adcResultInfoStruct.result);
        PRINTF("adcResultInfoStruct.channelNumber = %d\r\n", adcResultInfoStruct.channelNumber);
        PRINTF("adcResultInfoStruct.overrunFlag   = %d\r\n", adcResultInfoStruct.overrunFlag ? 1U : 0U);
        PRINTF("\r\n");
    }
}

static void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;

/* Configure the converter. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) & FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode; /* Using sync clock source. */
#endif                                                     /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE */
    adcConfigStruct.clockDividerNumber = ADC_CLOCK_DIVIDER;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    adcConfigStruct.resolution = kADC_Resolution12bit;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL) & FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL
    adcConfigStruct.enableBypassCalibration = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_CTRL_TSAMP
    adcConfigStruct.sampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_TSAMP */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) & FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    adcConfigStruct.enableLowPowerMode = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE */
#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) & FSL_FEATURE_ADC_HAS_TRIM_REG
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;
#endif /* FSL_FEATURE_ADC_HAS_TRIM_REG */
    ADC_Init(ADC_BASE, &adcConfigStruct);

#if !(defined(FSL_FEATURE_ADC_HAS_NO_INSEL) && FSL_FEATURE_ADC_HAS_NO_INSEL)
    /* Use the temperature sensor input to channel 0. */
    ADC_EnableTemperatureSensor(ADC_BASE, true);
#endif /* FSL_FEATURE_ADC_HAS_NO_INSEL. */

    /* Enable channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER's conversion in Sequence A. */
    adcConvSeqConfigStruct.channelMask =
        (1U << ADC_SAMPLE_CHANNEL_NUMBER); /* Includes channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER. */
    adcConvSeqConfigStruct.triggerMask = 0U;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(ADC_BASE, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(ADC_BASE, true); /* Enable the conversion sequence A. */
    /* Clear the result register. */
    ADC_DoSoftwareTriggerConvSeqA(ADC_BASE);
    while (!ADC_GetChannelConversionResult(ADC_BASE, ADC_SAMPLE_CHANNEL_NUMBER, &adcResultInfoStruct))
    {
    }
    ADC_GetConvSeqAGlobalConversionResult(ADC_BASE, &adcResultInfoStruct);
}

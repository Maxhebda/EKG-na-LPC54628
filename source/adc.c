#include "adc.h"
#include "fsl_power.h"
#include "fsl_iocon.h"
#include "fsl_debug_console.h"

extern adc_result_info_t adcResultInfoStruct;

void ADC_ClockPower_Configuration(void)
{
	POWER_DisablePD(kPDRUNCFG_PD_VDDA);    /* Power on VDDA. */
	POWER_DisablePD(kPDRUNCFG_PD_ADC0);    /* Power on the ADC converter. */
	POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA); /* Power on the analog power supply. */
	POWER_DisablePD(kPDRUNCFG_PD_VREFP);   /* Power on the reference voltage source. */

	CLOCK_EnableClock(kCLOCK_Adc0); /* SYSCON->AHBCLKCTRL[0] |= SYSCON_AHBCLKCTRL_ADC0_MASK; */
}

void ADC_Configuration(void)
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
	    //ADC_EnableTemperatureSensor(ADC_BASE, true);
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

void ADC_Config(void)
{
	CLOCK_EnableClock(kCLOCK_Iocon);
	const uint32_t port2_pin0_config = (IOCON_PIO_MODE_INACT | IOCON_PIO_INV_DI | IOCON_PIO_INPFILT_OFF | IOCON_PIO_OPENDRAIN_DI);
	IOCON_PinMuxSet(IOCON, 2U, 0U, port2_pin0_config);

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
}

#ifndef ADC_H_
#define ADC_H_

#include "fsl_adc.h"

#define ADC_BASE ADC0
#define ADC_SAMPLE_CHANNEL_NUMBER 5U
#define ADC_CLOCK_DIVIDER 5U

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_INPFILT_OFF 0x0200u /*!<@brief Input filter disabled */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

void ADC_Config(void);

void ADC_ClockPower_Configuration(void);

void ADC_Configuration(void);

#endif /* ADC_H_ */

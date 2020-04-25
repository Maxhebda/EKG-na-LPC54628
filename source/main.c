#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "emwin_support.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "BUTTON.h"
#include "pin_mux.h"
#include "fsl_sctimer.h"
#include "fsl_adc.h"

#include "desktop.h"
#include "adc.h"

int main(void)
{
	/* Board initialization */
	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
	CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
	CLOCK_EnableClock(kCLOCK_Gpio2);
	CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);
	BOARD_InitPins();
	BOARD_BootClockPLL220M();
	BOARD_InitDebugConsole();
	BOARD_InitSDRAM();

	/* Buzzer */
	initBuzzerPin();

	/* ADC */
	ADC_Config();

	/* emWin */
	GUI_Init();
	WM_MULTIBUF_Enable(1);
	GUI_EnableAlpha(1);
	GUI_Clear();
	WM_SetSize(WM_HBKWIN, LCD_WIDTH, LCD_HEIGHT);
	WM_SetCallback(WM_HBKWIN, desktopCallback);

	WM_Exec();

	while (1) {
		if (BOARD_Touch_Poll()) {
			//WM_Exec();
		}
		GUI_Delay(1);
	}
}

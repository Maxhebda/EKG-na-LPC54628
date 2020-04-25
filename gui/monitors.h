#ifndef MONITORS_H_
#define MONITORS_H_

#include "GUI.h"
#include "plot.h"
#include "desktop.h"
#include "fsl_debug_console.h"

/* ------------------------------------------------------ */
/* ----------------------- MACROS ----------------------- */
/* ------------------------------------------------------ */

/* Dimensions */
#define MONIT_WIN_HEART_WIDTH	 	50
#define MONIT_WIN_HEART_HEIGHT		MONIT_WIN_HEART_WIDTH
#define MONIT_WIN_HEART_BG			DESKTOP_HEAD_COLOR

/* Monitors colors and dimensions */
#define MONIT_WIN_PULSE_WIDTH	 	80
#define MONIT_WIN_PULSE_HEIGHT		50
#define MONIT_WIN_PULSE_BG			DESKTOP_HEAD_COLOR
#define MONIT_WIN_PULSE_COLOR		GUI_WHITE
#define MONIT_WIN_PULSE_FONT		GUI_FONT_32B_ASCII
#define MONIT_WIN_PULSE_UNIT_FONT	GUI_FONT_8X8_ASCII

#define MONIT_HEART_X				(PLOT_WIDTH - MONIT_WIN_PULSE_WIDTH - MONIT_WIN_HEART_WIDTH)
#define MONIT_HEART_Y				0

#define MONIT_PULSE_X				(PLOT_WIDTH - MONIT_WIN_PULSE_WIDTH)
#define MONIT_PULSE_Y				0

/* GUI IDs */
#define MONIT_PULSE_TXT_ID			GUI_ID_TEXT0
#define MONIT_PULSE_UNIT_TXT_ID		GUI_ID_TEXT1

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

/**
 * @brief 		Heart monitor callback routine
 * @param pMsg	Callback message base address
 */
void heartMonitorCallback(WM_MESSAGE *pMsg);


/**
 * @brief 		Pulse monitor callback routine
 * @param pMsg	Callback message base address
 */
void pulseMonitorCallback(WM_MESSAGE *pMsg);


/**
 * @brief 		Set current heart level
 * @param level	Heart level
 */
void setHeartLevel(uint8_t level);

/**
 * @brief 		Set current pulse level
 * @param pulse Pulse level
 */
void setPulseLevel(uint8_t pulse);

#endif /* MONITORS_H_ */

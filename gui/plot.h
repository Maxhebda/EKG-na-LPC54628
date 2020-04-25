#ifndef PLOT_H_
#define PLOT_H_

#include "GUI.h"
#include "emwin_support.h"
#include "desktop.h"
#include "stdbool.h"

/* ------------------------------------------------------ */
/* ----------------------- MACROS ----------------------- */
/* ------------------------------------------------------ */

/* Colors */
#define PLOT_BG_COLOR_LIGHT		0xFFF1F1F1
#define PLOT_GRID_COLOR_LIGHT	0xFFC7C7C7
#define PLOT_COLOR_LIGHT 		0xFF3E89BD

#define PLOT_BG_COLOR_DARK		0xFF000000
#define PLOT_GRID_COLOR_DARK    0xFF212223
#define PLOT_COLOR_DARK_G		0xFF0EC919
#define PLOT_COLOR_DARK_R		GUI_BLUE

/* Dimensions */
#define PLOT_WIN_X				0
#define PLOT_WIN_Y				HEAD_HEIGHT
#define PLOT_X					0
#define PLOT_Y					0
#define PLOT_WIDTH				LCD_WIDTH
#define PLOT_HEIGHT				(LCD_HEIGHT - HEAD_HEIGHT)

/* EKG graph properties */
#define GRAPH_GRIDNUM_X			10
#define GRAPH_GRIDNUM_Y			10

/* GUI IDs */
#define EKG_GRAPH_ID			GUI_ID_GRAPH0

/* Timer related */
#define TIMER_ID				0
#define TIMER_INT				20

/* Graph data related */
#define DEMO_DATA_LEN			856
#define DATA_MAX_VAL			217
#define DATA_ADC_MAX_VAL		4095
#define DATA_HIST_LEN			50
#define THRESHOLD_VAL			40
#define DEFAULT_THRESHOLD		100

/* Demo text related */
#define DEMO_TXT_RECT_WIDTH		100
#define DEMO_TXT_RECT_HEIGHT    40
#define DEMO_TXT_RECT_X	 		(PLOT_WIDTH / 2 - DEMO_TXT_RECT_WIDTH / 2)
#define DEMO_TXT_RECT_Y			(PLOT_HEIGHT / 2 - DEMO_TXT_RECT_HEIGHT / 2)
#define DEMO_TXT_COLOR			GUI_GREEN
#define DEMO_TXT_FONT			GUI_FONT_20_ASCII

/* ------------------------------------------------------ */
/* ----------------------- TYPES ------------------------ */
/* ------------------------------------------------------ */

/**
 * @brief Graph update mode
 */
typedef enum {
	DEMO_MODE,	//!< Demo mode. Data is taken from local storage
	ACQ_MODE    //!< Acquisition mode. Data is taken from ADC
} DATA_MODE;

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

/**
 * @brief 		Plot window callback routine
 * @param pMsg	Callback message base address
 */
void plotCallback(WM_MESSAGE *pMsg);

/**
 * @brief 		Toggle graph update mode
 * @param acq	True for acquisition mode, false for demo mode
 */
void changeAcqMode(bool acq);

/**
 * @brief 		Toggle run mode
 * @param run 	True for turning on actions, false otherwise
 */
void toggleRun(bool run);

/**
 * @brief		Toggle sound/mute mode
 * @param sound True for enabling sound, false for muting
 */
void toggleSound(bool sound);

/**
 * @brief 		Create/delete heart monitor
 * @param state	True for creating heart monitor, false for deleting it
 */
void toggleHeartMonitorWin(bool state);

/**
 * @brief 		Create/delete pulse monitor
 * @param state	True for creating pulse monitor, false for deleting it
 */
void togglePulseMonitorWin(bool state);

#endif /* PLOT_H_ */

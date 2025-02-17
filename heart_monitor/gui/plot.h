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
#define PLOT_COLOR_DARK_R		GUI_RED

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
#define DEMO_DATA_LEN			336
#define DATA_MAX_VAL			217

/* Demo text */
#define DEMO_TXT_RECT_WIDTH		100
#define DEMO_TXT_RECT_HEIGHT    40
#define DEMO_TXT_RECT_X	 		(PLOT_WIDTH / 2 - DEMO_TXT_RECT_WIDTH / 2 - 1)
#define DEMO_TXT_RECT_Y			(PLOT_HEIGHT / 2 - DEMO_TXT_RECT_HEIGHT / 2 - 1)
#define DEMO_TXT_COLOR			GUI_BLUE
#define DEMO_TXT_FONT			GUI_FONT_20_ASCII

/* ------------------------------------------------------ */
/* ----------------------- TYPES ------------------------ */
/* ------------------------------------------------------ */

typedef enum {
	DEMO_MODE,
	ACQ_MODE
} DATA_MODE;

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

void plotCallback(WM_MESSAGE *pMsg);

void changeAcqMode(bool acq);

void toggleRun(bool run);

#endif /* PLOT_H_ */

#ifndef DESKTOP_H_
#define DESKTOP_H_

#include "emwin_support.h"
#include "WM.h"

/* ------------------------------------------------------ */
/* ----------------------- MACROS ----------------------- */
/* ------------------------------------------------------ */

#define DESKTOP_HEAD_COLOR	GUI_GRAY
#define DESKTOP_BG_COLOR	0xFF111214

#define HEAD_WIDTH			LCD_WIDTH
#define HEAD_HEIGHT			52

/* Buttons */
#define HEART_BTN_WIDTH 	32
#define HEART_BTN_HEIGHT 	HEART_BTN_WIDTH
#define HEART_BTN_X 		380
#define HEART_BTN_Y 		10

#define PULSE_BTN_WIDTH     32
#define PULSE_BTN_HEIGHT    PULSE_BTN_WIDTH
#define PULSE_BTN_X 		430
#define PULSE_BTN_Y 		10

#define RUN_BTN_WIDTH		32
#define RUN_BTN_HEIGHT		RUN_BTN_WIDTH
#define RUN_BTN_X			60
#define RUN_BTN_Y			10

#define ACQ_BTN_WIDTH		32
#define ACQ_BTN_HEIGHT		ACQ_BTN_WIDTH
#define ACQ_BTN_X			100
#define ACQ_BTN_Y			10

/* GUI IDs */
#define HEART_BTN_ID		GUI_ID_BUTTON0
#define PULSE_BTN_ID		GUI_ID_BUTTON1
#define RUN_BTN_ID			GUI_ID_BUTTON3
#define ACQ_BTN_ID			GUI_ID_BUTTON4

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

void desktopCallback(WM_MESSAGE *pMsg);

#endif /* DESKTOP_H_ */

#include "desktop.h"
#include "BUTTON.h"
#include "stdbool.h"
#include "plot.h"

/* ------------------------------------------------------ */
/* ----------------- PRIVATE VARIABLES ------------------ */
/* ------------------------------------------------------ */

/* Bitmaps */
extern GUI_CONST_STORAGE GUI_BITMAP bmHeartIcon, bmHeartIconDisabled, bmPulseIcon, bmPulseIconDisabled, bmPulseIconDisabled, bmPlay, bmStop, bmAcq, bmDemo, bmZI;

/* Buttons */
static BUTTON_Handle heartBtnHandle = 0, pulseBtnHandle = 0, runBtnHandle = 0, acqBtnHandle = 0;
static bool heartBtnState = false, pulseBtnState = false, runBtnState = false, acqBtnState = false;

/* Plot window */
static WM_HWIN plotWindow = 0;

/* ------------------------------------------------------ */
/* ----------------- PRIVATE FUNCTIONS ------------------ */
/* ------------------------------------------------------ */

static void _drawGUI(void)
{
	GUI_SetColor(DESKTOP_HEAD_COLOR);
	GUI_FillRect(0, 0, HEAD_WIDTH - 1, HEAD_HEIGHT - 1);
	GUI_DrawBitmap(&bmZI, 2, 2);
}

static void _createWidgets(void)
{
	/* Buttons */
	BUTTON_SetDefaultSkinClassic();
	BUTTON_SetDefaultBkColor(GUI_TRANSPARENT, BUTTON_CI_PRESSED);
	BUTTON_SetDefaultBkColor(GUI_TRANSPARENT, BUTTON_CI_UNPRESSED);

	heartBtnHandle = BUTTON_CreateEx(HEART_BTN_X, HEART_BTN_Y, HEART_BTN_WIDTH, HEART_BTN_HEIGHT, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0, HEART_BTN_ID);
	BUTTON_SetBitmap(heartBtnHandle, BUTTON_BI_UNPRESSED, &bmHeartIconDisabled);
	WIDGET_SetEffect(heartBtnHandle, &WIDGET_Effect_None);
	BUTTON_SetFocusable(heartBtnHandle, 0);

	pulseBtnHandle = BUTTON_CreateEx(PULSE_BTN_X, PULSE_BTN_Y, PULSE_BTN_WIDTH, PULSE_BTN_HEIGHT, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0, PULSE_BTN_ID);
	BUTTON_SetBitmap(pulseBtnHandle, BUTTON_BI_UNPRESSED, &bmPulseIconDisabled);
	WIDGET_SetEffect(pulseBtnHandle, &WIDGET_Effect_None);
	BUTTON_SetFocusable(pulseBtnHandle, 0);

	runBtnHandle = BUTTON_CreateEx(RUN_BTN_X, RUN_BTN_Y, RUN_BTN_WIDTH, RUN_BTN_HEIGHT, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0, RUN_BTN_ID);
	BUTTON_SetBitmap(runBtnHandle, BUTTON_BI_UNPRESSED, &bmStop);
	WIDGET_SetEffect(runBtnHandle, &WIDGET_Effect_None);
	BUTTON_SetFocusable(runBtnHandle, 0);

	acqBtnHandle = BUTTON_CreateEx(ACQ_BTN_X, ACQ_BTN_Y, ACQ_BTN_WIDTH, ACQ_BTN_HEIGHT, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0, ACQ_BTN_ID);
	BUTTON_SetBitmap(acqBtnHandle, BUTTON_BI_UNPRESSED, &bmDemo);
	WIDGET_SetEffect(acqBtnHandle, &WIDGET_Effect_None);
	BUTTON_SetFocusable(acqBtnHandle, 0);

	/* Plot window */
	plotWindow = WM_CreateWindowAsChild(PLOT_WIN_X, PLOT_WIN_Y, PLOT_WIDTH, PLOT_HEIGHT, WM_HBKWIN, WM_CF_SHOW, plotCallback, 0);
}

static void _heartBtnClicked(void)
{
	heartBtnState ? BUTTON_SetBitmap(heartBtnHandle, BUTTON_BI_UNPRESSED, &bmHeartIconDisabled) : BUTTON_SetBitmap(heartBtnHandle, BUTTON_BI_UNPRESSED, &bmHeartIcon);
	heartBtnState = !heartBtnState;
}

static void _pulseBtnClicked(void)
{
	pulseBtnState ? BUTTON_SetBitmap(pulseBtnHandle, BUTTON_BI_UNPRESSED, &bmPulseIconDisabled) : BUTTON_SetBitmap(pulseBtnHandle, BUTTON_BI_UNPRESSED, &bmPulseIcon);
	pulseBtnState = !pulseBtnState;
}

static void _runBtnClicked(void)
{
	runBtnState ? BUTTON_SetBitmap(runBtnHandle, BUTTON_BI_UNPRESSED, &bmStop) : BUTTON_SetBitmap(runBtnHandle, BUTTON_BI_UNPRESSED, &bmPlay);
	runBtnState = !runBtnState;

	toggleRun(runBtnState);
}

static void _acqBtnClicked(void)
{
	acqBtnState ? BUTTON_SetBitmap(acqBtnHandle, BUTTON_BI_UNPRESSED, &bmDemo) : BUTTON_SetBitmap(acqBtnHandle, BUTTON_BI_UNPRESSED, &bmAcq);
	acqBtnState = !acqBtnState;

	changeAcqMode(acqBtnState);
}

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

void desktopCallback(WM_MESSAGE *pMsg)
{
	int widgetId;

	switch (pMsg->MsgId) {


	case WM_NOTIFY_PARENT:
		widgetId = WM_GetId(pMsg->hWinSrc);

		if (pMsg->Data.v == WM_NOTIFICATION_CLICKED) {
			switch (widgetId) {
			case HEART_BTN_ID:
				_heartBtnClicked();
				break;
			case PULSE_BTN_ID:
				_pulseBtnClicked();
				break;
			case RUN_BTN_ID:
				_runBtnClicked();
				break;
			case ACQ_BTN_ID:
				_acqBtnClicked();
				break;
			}
		}
		break;

	/* WM_CREATE cannot be used because desktop window uses built-in HBKWIN */
	case WM_SET_CALLBACK:
		_createWidgets();
		break;

	/* Desktop repaint */
	case WM_PAINT:
		GUI_SetBkColor(DESKTOP_BG_COLOR);
		GUI_Clear();
		_drawGUI();
		break;
	default:
		WM_DefaultProc(pMsg);

	}
}

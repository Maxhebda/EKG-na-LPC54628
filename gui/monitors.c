#include "monitors.h"
#include "WM.h"
#include "TEXT.h"
#include "stdio.h"

/* ------------------------------------------------------ */
/* ----------------- PRIVATE VARIABLES ------------------ */
/* ------------------------------------------------------ */

/* Bitmaps */
extern GUI_CONST_STORAGE GUI_BITMAP bmHeart25, bmHeart50, bmHeart75, bmHeart100;

/* GUI handlers */
TEXT_Handle pulseTxtHandle = 0;
TEXT_Handle pulseUnitTxtHandle = 0;

/* Current data levels */
static uint8_t heartLevel;
static uint8_t pulseLevel;

/* Used for changing integer values to string */
char buff[10] = {'?', 0};

/* Text constants */
static const char *pulseUnitTxt = "bpm";

/* ------------------------------------------------------ */
/* ----------------- PRIVATE FUNCTIONS ------------------ */
/* ------------------------------------------------------ */

/**
 * @brief 			Create text widgets
 * @param parent	Parent widget
 */
static void _createWidgets(WM_HWIN parent)
{
	TEXT_SetDefaultTextColor(MONIT_WIN_PULSE_COLOR);

	/* Pulse text widget */
	pulseTxtHandle = TEXT_CreateEx(0, 0, MONIT_WIN_PULSE_WIDTH, MONIT_WIN_HEART_HEIGHT - 10, parent, WM_CF_SHOW, TEXT_CF_HCENTER | TEXT_CF_VCENTER, MONIT_PULSE_TXT_ID, buff);
	TEXT_SetFont(pulseTxtHandle, MONIT_WIN_PULSE_FONT);

	/* Pulse unit text widget */
	pulseUnitTxtHandle = TEXT_CreateEx(0, MONIT_WIN_HEART_HEIGHT - 10, MONIT_WIN_PULSE_WIDTH, 10, parent, WM_CF_SHOW, TEXT_CF_HCENTER | TEXT_CF_VCENTER, MONIT_PULSE_UNIT_TXT_ID, pulseUnitTxt);
	TEXT_SetFont(pulseUnitTxtHandle, MONIT_WIN_PULSE_UNIT_FONT);
}

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

void heartMonitorCallback(WM_MESSAGE *pMsg)
{
	switch (pMsg->MsgId) {
	case WM_PAINT:
		GUI_SetBkColor(MONIT_WIN_HEART_BG);
		GUI_Clear();

		/* Draw specific bitmap based on current heart level */
		switch (heartLevel) {
		case 5:
			GUI_DrawBitmap(&bmHeart25, 9, 9);
			break;
		case 10:
			GUI_DrawBitmap(&bmHeart50, 5, 5);
			break;
		case 15:
			GUI_DrawBitmap(&bmHeart75, 3, 3);
			break;
		default:
			GUI_DrawBitmap(&bmHeart100, 1, 1);
		}

		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void pulseMonitorCallback(WM_MESSAGE *pMsg)
{
	switch (pMsg->MsgId) {
	case WM_CREATE:
		_createWidgets(pMsg->hWin);
		break;
	case WM_PAINT:
		GUI_SetBkColor(MONIT_WIN_PULSE_BG);
		GUI_Clear();

		TEXT_SetText(pulseTxtHandle, buff);

		break;
	case WM_DELETE:
		WM_DeleteWindow(pulseTxtHandle);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void setHeartLevel(uint8_t level)
{
	heartLevel = level;
}

void setPulseLevel(uint8_t pulse)
{
	pulseLevel = pulse;
	sprintf(buff, "%d", pulse);
}

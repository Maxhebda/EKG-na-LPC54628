#include "plot.h"
#include "GRAPH.h"
#include "stdbool.h"
#include "fsl_adc.h"
#include "fsl_debug_console.h"
#include "adc.h"

/* ------------------------------------------------------ */
/* ----------------- PRIVATE VARIABLES ------------------ */
/* ------------------------------------------------------ */

/* ADC result structure */
adc_result_info_t adcResultInfoStruct;

/* GUI handlers */
static GRAPH_Handle graphHandle = 0;
static GRAPH_DATA_Handle graphDataHandle = 0;

/* Graph data related */
static DATA_MODE dataMode = DEMO_MODE;
static int16_t demoData[DEMO_DATA_LEN] = {84,86,84,84,83,84,56,196,163,86,78,80,81,91,97,105,100,93,83,82,84,80,82,84,87,89,87,85,84,84,85,71,197,105,78,77,83,86,92,97,100,99,92,83,83,85,84,84,84,84,89,89,85,83,84,87,87,99,92,83,83,85,84,84,84,84,89,89,85,83,84,87,63,196,139,84,80,84,84,86,92,99,101,96,82,80,81,84,87,87,85,85,84,86,86,85,87,87,59,196,180,82,80,82,81,84,92,97,101,99,85,80,80,86,87,88,85,84,87,89,89,86,86,87,90,80,174,196,59,81,84,84,87,93,101,101,97,86,79,78,84,84,84,85,82,81,84,82,88,83,84,83,87,71,196,197,75,79,80,84,85,93,102,102,95,81,78,83,84,83,83,83,84,85,87,89,84,83,87,87,71,197,79,80,80,84,85,89,98,101,102,89,80,78,84,84,83,84,84,86,87,86,83,85,86,89,63,197,186,83,80,84,84,89,99,103,105,93,83,81,84,86,85,86,85,87,86,91,84,84,88,89,65,196,194,88,80,84,85,89,93,101,102,98,85,80,84,86,86,86,84,85,84,88,85,83,85,86,82,142,197,64,78,80,82,86,96,101,103,100,84,80,84,84,88,88,85,85,87,85,90,91,86,89,89,84,128,197,75,83,84,85,87,96,101,99,95,82,80,83,84,83,82,80,81,84,82,83,80,82,85,80,63,197,34,74,76,80,85,91,97,99,97,84,80,76,76,80,84,84,81,83,88,85,83,83};
static uint16_t demoDataCtr = 0;
static int16_t prev;
bool running = false;

/* Demo text related */
static GUI_RECT demoTxtRect = {DEMO_TXT_RECT_X, DEMO_TXT_RECT_Y, DEMO_TXT_RECT_X + DEMO_TXT_RECT_WIDTH, DEMO_TXT_RECT_Y + DEMO_TXT_RECT_HEIGHT};
static const char *demoTxt = "DEMO";

/* ------------------------------------------------------ */
/* ----------------- PRIVATE FUNCTIONS ------------------ */
/* ------------------------------------------------------ */

static void _graphUserDraw(WM_HWIN hWin, int stage) {
	if (dataMode == DEMO_MODE) {
		switch (stage) {
		case GRAPH_DRAW_LAST:
			GUI_SetFont(DEMO_TXT_FONT);
			GUI_SetColor(DEMO_TXT_COLOR);
			GUI_DispStringInRect(demoTxt, &demoTxtRect, GUI_TA_VCENTER | GUI_TA_HCENTER);
			break;
		}
	}
}

static void _createWidgets(WM_HWIN parent)
{
	unsigned gridDistX = PLOT_WIDTH / GRAPH_GRIDNUM_X;
	unsigned gridDistY = PLOT_HEIGHT / GRAPH_GRIDNUM_Y;
	graphHandle = GRAPH_CreateEx(PLOT_X, PLOT_Y, PLOT_WIDTH, PLOT_HEIGHT, parent, WM_CF_SHOW, 0, EKG_GRAPH_ID);
	GRAPH_SetColor(graphHandle, PLOT_GRID_COLOR_DARK, GRAPH_CI_GRID);
	GRAPH_SetColor(graphHandle, PLOT_BG_COLOR_DARK, GRAPH_CI_BK);
	GRAPH_SetGridDistX(graphHandle, gridDistX);
	GRAPH_SetGridDistY(graphHandle, gridDistY);
	GRAPH_SetGridVis(graphHandle, true);
	GRAPH_SetUserDraw(graphHandle, _graphUserDraw);

	graphDataHandle = GRAPH_DATA_YT_Create(PLOT_COLOR_DARK_R, PLOT_WIDTH, NULL, 0);
	GRAPH_DATA_YT_SetAlign(graphDataHandle, GRAPH_ALIGN_LEFT);
	GRAPH_AttachData(graphHandle, graphDataHandle);
}

void _updateData(void)
{
	static int16_t acqData;

	switch (dataMode) {
	case DEMO_MODE:
		if (demoDataCtr >= DEMO_DATA_LEN) {
			demoDataCtr = 0;
		}
		GRAPH_DATA_YT_AddValue(graphDataHandle, prev);
		GRAPH_DATA_YT_AddValue(graphDataHandle, (prev + demoData[demoDataCtr]) / 2);
		GRAPH_DATA_YT_AddValue(graphDataHandle, demoData[demoDataCtr]);
		prev = demoData[demoDataCtr++];
		break;
	case ACQ_MODE:
		/* ADC readout */
        ADC_DoSoftwareTriggerConvSeqA(ADC_BASE);
        while (!ADC_GetChannelConversionResult(ADC_BASE, ADC_SAMPLE_CHANNEL_NUMBER, &adcResultInfoStruct)) {}
        acqData = adcResultInfoStruct.result * DATA_MAX_VAL / 4095;
        /* Acquisition data normalization */
		GRAPH_DATA_YT_AddValue(graphDataHandle, prev);
		GRAPH_DATA_YT_AddValue(graphDataHandle, (prev + acqData) / 2);
		GRAPH_DATA_YT_AddValue(graphDataHandle, acqData);
        prev = acqData;
		break;
	}
}

/* ------------------------------------------------------ */
/* ------------------------ API ------------------------- */
/* ------------------------------------------------------ */

void plotCallback(WM_MESSAGE *pMsg)
{
	switch (pMsg->MsgId) {
	case WM_CREATE:
		prev = *demoData; /* Set previous value to first array of demo elements */
		_createWidgets(pMsg->hWin);
		WM_CreateTimer(pMsg->hWin, TIMER_ID, TIMER_INT, 0);
		break;
	case WM_PAINT:
		GUI_SetBkColor(DESKTOP_BG_COLOR);
		GUI_Clear();
		break;
	case WM_TIMER:
		if (running) {
			_updateData();
		}
		WM_RestartTimer(pMsg->Data.v, TIMER_INT);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void changeAcqMode(bool acq)
{
	if (acq) {
		dataMode = ACQ_MODE;
	} else {
		dataMode = DEMO_MODE;
	}
	GRAPH_DATA_YT_Clear(graphDataHandle);
}

void toggleRun(bool run)
{
	running = run;
}

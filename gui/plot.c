#include "plot.h"
#include "GRAPH.h"
#include "stdbool.h"
#include "fsl_adc.h"
#include "fsl_debug_console.h"
#include "adc.h"
#include "monitors.h"
#include "stdlib.h"
#include "fsl_gpio.h"

/* ------------------------------------------------------ */
/* ----------------- PRIVATE VARIABLES ------------------ */
/* ------------------------------------------------------ */

/* Bitmaps */
extern GUI_CONST_STORAGE GUI_BITMAP bmEcg;

/* ADC result structure */
adc_result_info_t adcResultInfoStruct;

/* GUI handlers */
static GRAPH_Handle graphHandle = 0;
static GRAPH_DATA_Handle graphDataHandle = 0;
static WM_HWIN heartMonitorWindow = 0, pulseMonitorWindow = 0;

/* Plot window handle */
static WM_HWIN thisWin;

/* Graph data related */
static DATA_MODE dataMode = DEMO_MODE;
static int16_t demoData[DEMO_DATA_LEN] = {109,110,103,109,107,111,118,125,126,125,119,113,109,113,108,109,108,104,102,100,104,102,106,107,108,115,116,114,114,113,107,187,112,101,107,111,103,109,107,115,120,124,124,122,119,112,107,105,101,105,99,99,101,101,105,106,112,110,109,110,116,114,111,110,108,106,175,111,98,99,100,104,107,112,119,124,125,122,122,112,101,103,98,96,103,105,105,109,103,109,111,112,111,111,113,112,118,118,110,107,104,98,174,111,106,105,107,110,113,116,116,113,113,114,112,110,106,105,106,110,111,111,110,113,112,110,111,112,109,107,106,104,105,107,107,104,107,108,165,111,115,112,107,113,109,106,109,114,121,121,119,116,113,109,112,111,108,112,108,109,104,103,101,101,103,107,108,107,109,115,117,113,109,109,108,170,111,103,107,106,105,108,109,115,120,121,121,118,113,111,108,104,103,101,103,102,103,108,107,112,109,112,111,111,113,113,120,115,112,111,103,178,111,95,104,109,111,116,118,116,115,115,117,117,106,108,104,104,104,108,112,109,107,112,112,111,109,113,111,107,104,106,107,102,105,106,113,161,112,109,108,112,111,109,108,114,117,123,123,118,111,113,108,109,111,110,109,107,104,103,101,105,104,106,106,108,112,119,116,112,113,105,182,112,99,104,103,105,113,116,120,122,127,124,116,108,106,103,100,102,102,105,108,108,111,112,108,112,111,113,114,115,118,115,109,106,104,110,165,111,110,106,107,116,116,117,119,112,115,117,113,109,107,107,110,110,110,109,111,110,110,112,112,111,110,107,105,107,109,105,105,106,103,178,112,105,109,113,114,115,109,112,118,119,119,118,115,111,109,109,111,109,109,113,108,106,109,104,104,102,103,105,111,117,112,111,109,107,180,112,99,113,112,103,106,113,114,123,124,125,122,115,110,105,102,102,102,101,104,107,110,110,112,110,111,115,112,113,119,115,114,112,106,175,112,95,103,107,113,117,117,116,114,118,117,111,109,107,106,107,107,109,111,113,113,112,112,110,112,109,107,105,103,107,110,105,107,104,163,112,99,112,108,113,105,104,113,120,124,123,123,116,114,110,109,108,107,105,104,104,102,103,104,105,110,109,113,114,115,115,113,114,112,156,111,97,109,104,107,114,116,120,123,125,120,114,107,104,103,100,104,108,104,112,109,110,110,112,112,113,110,114,114,110,112,109,104,105,104,158,111,99,112,111,112,113,107,109,117,120,124,120,113,116,108,109,111,111,111,111,106,105,106,103,103,103,105,109,110,114,117,111,111,112,107,180,112,103,110,105,107,111,117,121,122,127,123,117,111,102,101,103,101,105,106,112,110,111,111,110,114,113,111,112,113,115,115,107,105,104,113,165,112,110,107,112,112,113,112,115,114,118,117,115,110,108,110,111,111,111,111,114,114,108,110,109,107,101,106,102,104,111,114,112,111,112,109,183,112,108,109,108,108,106,114,120,123,124,125,121,117,113,108,105,106,103,104,102,103,106,106,109,112,112,110,108,111,114,117,110,112,110,109,161,112,101,104,103,108,111,117,120,123,122,119,113,106,102,101,102,102,105,107,109,112,110,112,113,110,112,112,113,111,114,114,104,102,104,114,167,111,112,110,115,116,117,113,113,118,123,122,116,113,110,112,111,108,110,112,112,109,109,105,106,104,106,104,105,106,113,115,111,111,111,120,157,112,111,104,105,103,111,115,119,123,124,122,117,113,108,104,101,100,101,104,103,105,109,109,109,110,110,111,112,113,115,114,115,109,109,164,111,97,103,109,112,115,114,114,112,117,116,113,110,107,105,110,106,109,110,111,112,112,114,113,110,112,107,107,107,108,111,107,107,107,106,178,112,106,111,111,112,108,105,112,122,125,122,120,113,111,109,110,113,109,111,105,104,104,102,107,105,109,109,112,112,116,119,113,113,114,108,185,113,98,104};
static uint16_t demoDataCtr = 0;
static int16_t prev;

/* Data history */
int16_t dataHistory[DATA_HIST_LEN] = {0};
uint8_t dataHistoryCtr = 0;
uint8_t threshold = DEFAULT_THRESHOLD;

/* Time measurements */
extern volatile GUI_TIMER_TIME OS_TimeMS;
static GUI_TIMER_TIME timeOld;

/* Pulse related */
uint8_t currentBPM = 0;
bool belowThreshold = true;
uint8_t idleTime = 0;

/* Demo text related */
static GUI_RECT demoTxtRect = {DEMO_TXT_RECT_X, DEMO_TXT_RECT_Y, DEMO_TXT_RECT_X + DEMO_TXT_RECT_WIDTH, DEMO_TXT_RECT_Y + DEMO_TXT_RECT_HEIGHT};
static const char *demoTxt = "DEMO";

/* Button states */
bool running = false;
bool soundEnabled = false;

/* ------------------------------------------------------ */
/* ----------------- PRIVATE FUNCTIONS ------------------ */
/* ------------------------------------------------------ */

/**
 * @brief	Use buzzer if enabled and needed
 * @param 	state True for turning on, false otherwise
 */
static inline void _beep(bool state)
{
	(state && soundEnabled) ? GPIO_PinWrite(GPIO, BUZZER_PORT,BUZZER_PIN, false) : GPIO_PinWrite(GPIO, BUZZER_PORT,BUZZER_PIN, true);
}

/**
 * @brief	Calculate average heart level based on data history
 */
static void _calcAvgLevel(void)
{
	int s = 0;
	for (int i = 0; i < DATA_HIST_LEN; i++) {
		s += dataHistory[i];
	}
	threshold = s / DATA_HIST_LEN;
}

/**
 * @brief		Graph widget draw function
 * @param hWin	Graph widget handler
 * @param stage	Draw stage. Demo text should be drawn last
 */
static void _graphUserDraw(WM_HWIN hWin, int stage) {
	/* If demo mode enabled "DEMO" text should appear */
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

/**
 * @brief 			Create graph and it's data widgets
 * @param parent	Parent widget handler
 */
static void _createWidgets(WM_HWIN parent)
{
	/* Calculate distance between X and Y grids */
	uint32_t gridDistX = PLOT_WIDTH / GRAPH_GRIDNUM_X;
	uint32_t gridDistY = PLOT_HEIGHT / GRAPH_GRIDNUM_Y;

	/* Create graph widget */
	graphHandle = GRAPH_CreateEx(PLOT_X, PLOT_Y, PLOT_WIDTH, PLOT_HEIGHT, parent, WM_CF_SHOW, 0, EKG_GRAPH_ID);
	GRAPH_SetColor(graphHandle, PLOT_GRID_COLOR_DARK, GRAPH_CI_GRID);
	GRAPH_SetColor(graphHandle, PLOT_BG_COLOR_DARK, GRAPH_CI_BK);
	GRAPH_SetGridDistX(graphHandle, gridDistX);
	GRAPH_SetGridDistY(graphHandle, gridDistY);
	GRAPH_SetGridVis(graphHandle, true);
	GRAPH_SetUserDraw(graphHandle, _graphUserDraw);

	/* Create graph data widget */
	graphDataHandle = GRAPH_DATA_YT_Create(PLOT_COLOR_DARK_R, PLOT_WIDTH, NULL, 0);
	GRAPH_DATA_YT_SetAlign(graphDataHandle, GRAPH_ALIGN_LEFT);
	GRAPH_AttachData(graphHandle, graphDataHandle);
}

/**
 * @brief Update graph, store acquisition data history and send data to the monitors
 */
void _updateData(void)
{
	static int16_t acqData;

	switch (dataMode) {
	case DEMO_MODE:
		/* Demo data normalization */
		GRAPH_DATA_YT_AddValue(graphDataHandle, prev);
		GRAPH_DATA_YT_AddValue(graphDataHandle, (prev + demoData[demoDataCtr]) / 2);
		GRAPH_DATA_YT_AddValue(graphDataHandle, demoData[demoDataCtr]);
		prev = demoData[demoDataCtr];
		demoDataCtr = (demoDataCtr + 1) % DEMO_DATA_LEN;
		break;
	case ACQ_MODE:
		/* ADC readout */
		ADC_DoSoftwareTriggerConvSeqA(ADC_BASE);
		while (!ADC_GetChannelConversionResult(ADC_BASE, ADC_SAMPLE_CHANNEL_NUMBER, &adcResultInfoStruct)) {}
		acqData = adcResultInfoStruct.result * DATA_MAX_VAL / DATA_ADC_MAX_VAL;
		/* Acquisition data normalization */
		GRAPH_DATA_YT_AddValue(graphDataHandle, prev);
		GRAPH_DATA_YT_AddValue(graphDataHandle, (prev + acqData) / 2);
		GRAPH_DATA_YT_AddValue(graphDataHandle, acqData);
		prev = acqData;
		break;
	}

	/* Store current readout in history */
	dataHistory[dataHistoryCtr] = prev;
	dataHistoryCtr = (dataHistoryCtr + 1) % DATA_HIST_LEN;

	/* Calculate average heart level */
	_calcAvgLevel();

	if (dataHistory[dataHistoryCtr] > threshold + THRESHOLD_VAL && belowThreshold == true) {
		/* Current heart level exceeds average level */

		currentBPM = 60000 / abs(OS_TimeMS - timeOld); /* Calculate BPM and send it to the pulse monitor */
		setPulseLevel(currentBPM);

		setHeartLevel(20); /* Send heart level to the heart monitor */

		_beep(true); /* Turn on buzzer */

		/* Reset all needed variables */
		timeOld = OS_TimeMS;
		idleTime = 0;
		belowThreshold = false;

	}
	else if(demoData[demoDataCtr] < threshold) {
		/* Current heart level is below average */

		/* Set new heart level */
		if (idleTime < 3) {
			_beep(false); /* Turn off buzzer */
		} else if (idleTime < 5) {
			setHeartLevel(15);
		} else if (idleTime < 8) {
			setHeartLevel(10);
		} else {
			setHeartLevel(5);
		}

		/* Reset all needed variables */
		belowThreshold = true;
		idleTime++;
	}

	/* Refresh monitor windows if active */
	if (pulseMonitorWindow) {
		WM_InvalidateWindow(pulseMonitorWindow);
	}

	if (heartMonitorWindow) {
		WM_InvalidateWindow(heartMonitorWindow);
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
		thisWin = pMsg->hWin;
		WM_CreateTimer(pMsg->hWin, TIMER_ID, TIMER_INT, 0); /* Acquisition timer */
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
		demoDataCtr = 0;
	}

	/* Clear history and threshold */
	for (int i = 0; i < DATA_HIST_LEN; i++) {
		dataHistory[i] = 0;
	}
	threshold = DEFAULT_THRESHOLD;

	GRAPH_DATA_YT_Clear(graphDataHandle);
}

void toggleRun(bool run)
{
	running = run;
}

void toggleSound(bool sound)
{
	soundEnabled = sound;
	/* Turn off the buzzer */
	GPIO_PinWrite(GPIO, BUZZER_PORT,BUZZER_PIN, true);
}

void toggleHeartMonitorWin(bool state)
{
	if (state) {
		/* Heart monitor should be created */
		heartMonitorWindow = WM_CreateWindowAsChild(MONIT_HEART_X, MONIT_HEART_Y, MONIT_WIN_HEART_WIDTH, MONIT_WIN_HEART_HEIGHT, thisWin, WM_CF_SHOW, heartMonitorCallback, 0);
	} else {
		/* Heart monitor should be deleted */
		if (heartMonitorWindow) {
			WM_DeleteWindow(heartMonitorWindow);
			heartMonitorWindow = 0;
		}
	}
}

void togglePulseMonitorWin(bool state)
{
	if (state) {
		/* Pulse monitor should be created */
		pulseMonitorWindow = WM_CreateWindowAsChild(MONIT_PULSE_X, MONIT_PULSE_Y, MONIT_WIN_PULSE_WIDTH, MONIT_WIN_PULSE_HEIGHT, thisWin, WM_CF_SHOW, pulseMonitorCallback, 0);
	} else {
		/* Pulse monitor should be deleted */
		if (pulseMonitorWindow) {
			WM_DeleteWindow(pulseMonitorWindow);
			pulseMonitorWindow = 0;
		}
	}
}

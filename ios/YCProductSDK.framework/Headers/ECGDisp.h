#pragma once
#include "PublicFunc.h"
#include <stdbool.h>

#define WAIT_SAMPLE_COUNT   100
#define MAX_DIFF_DATA       25000

#define Abs(x) ((x)<0 ? (-(x)):(x))

// Module reset
extern void ECGDispInit();

// Main function for ECG display processing
extern void ECGDisp(int *ecgdata);

void ECGLP21IIR(int *ecgdata, bool init);
void ECGLP40IIR(int *ecgdata, bool init);
void ECGLP40FIR(int *ecgdata, bool init);

void ECGHP067IIR(int *ecgdata, bool init);

void ECGNotch50IIR(int *ecgdata, bool init);
void ECGNotch60IIR(int *ecgdata, bool init);
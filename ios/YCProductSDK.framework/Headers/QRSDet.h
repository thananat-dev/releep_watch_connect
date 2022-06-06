#pragma once
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "PublicFunc.h"
#include "ECGCodes.h"

#define Abs(x) ((x)<0 ? (-(x)):(x))

extern int QRS_FILT;

// Define rr interval types.
#define QQ	0	// Unknown-Unknown interval.
#define NN	1	// Normal-Normal interval.
#define NV	2	// Normal-PVC interval.
#define VN	3	// PVC-Normal interval.
#define VV	4	// PVC-PVC interval.

#define LEARNING	0
#define READY	    1

// Module reset
extern void QRSInit();

// Main function for QRS detection
extern bool QRSDet(int ecgdata);
extern bool ArrDet(int ecgdata);

void QRSStudy(int ecgdata, int integdata);

int BSLCheck(int indata[], int datapos, int* maxder);
int PeakCalc(int indata, int* peakdelay, bool init);
int UpdateQRSThresh(int qmean, int nmean);
int QRSThreshMean(int indata[], int datalen);
void UpdateRR();

void ARRFilt(int *ecgdata, int *filtdata, bool init);
int QRSLP20Filt(int *ecgdata, bool init);
int QRSHP05Filt(int *ecgdata, bool init);
int ARRNotch5060Filt(int *ecgdata, bool init);

void QRSFilt(int *ecgdata, int *filtdata, int *integdata, bool init);
int QRSLP13Filt(int *ecgdata, bool init);
int QRSHP5Filt(int *ecgdata, bool init);
int QRSNotch5060Filt(int *ecgdata, bool init);
int QRSDeriv1Filt(int *ecgdata, bool init);
int QRSDeriv2Filt(int *ecgdata, bool init);
int QRSMovAve(int *ecgdata, bool init);
int KalmanFilter(int data, bool init);
float ECGKurCalc(int* data, int n);

// Output
extern void getQRSRes(int *qrsdelay, int *qrstype);
extern void getHR(short *hrval);
extern void getRR(short *rrval);
extern void getHRV(short *hrvval);
extern void getAF(bool *afflag);

extern void getECGFinalResult(short *hr, int *qrstype, bool *afflag);

void resetARR();
void resetBDAC();

int  BeatClassify(int newbeat[], int rr, int noiselevel, int* fidadj, bool init);
int  LFNoiseCheck(int indata, int delay, int rr, int beatbegin, int beatend);
void DownsampleBeat(int *beatout, int *beatin);
bool NoiseEval(int indata[], int len);

int RRMatch(int rr0, int rr1);
int RRShort(int rr0, int rr1);
int RRShort2(int *rrintervals, int *rrtypes);
int RRMatch2(int rr0, int rr1);

int RhythmCheck(int rr);
int BigeCheck(void);
void ResetRhythmCheck();

void ResetMatch();
void UpdateBeatTemplate(int *avebeat, int *newbeat, int shift);
void BeatCopy(int srcbeat, int destbeat);
void SetBeatClass(int type, int beatclass);
void BestMorphMatch(int *newbeat, int *matchtype, float *matchindex, float *diffindex, int *shiftadj);
void UpdateBeatType(int matchtype, int *newbeat, float diffindex, int shiftadj);
void ClearLastNewType();
int SetNewBeatType(int *newbeat);

int GetTypesCount();
int GetBeatTypeCount(int type);
int GetBeatWidth(int type);
int GetBeatCenter(int type);
int GetBeatClass(int type);
int GetBeatBegin(int type);
int GetBeatEnd(int type);
int GetBeatAmp(int type);
int GetDominantType();

float DomCompare2(int *newbeat, int domtype);
float DomCompare(int newtype, int domtype);
float CompareBeats(int *beat1, int *beat2, int *shiftadj);
float CompareBeats2(int *beat1, int *beat2, int *shiftadj);
int _minBeatVariation(int type);
int _wideBeatVariation(int type);

void ResetPostClassify();
void PostClassify(int *recenttypes, int domtype, int *recentRRs, int width, float diffindex, int rhythmclass);
int CheckPostClass(int type);
int CheckPCRhythm(int type);

int ISOCheck(int *data, int isolength);
void AnalyzeBeat(int *beat, int *onset, int *offset, int *isolevel, int *beatbegin, int *beatend, int *amp);
void AdjustDomData(int oldtype, int newtype);
void CombineDomData(int oldtype, int newtype);

int HFNoiseCheck(int *beat);
int GetTempClass(int rhythmclass, int morphtype, int beatWidth, int domWidth, int domtype, int hfNoise, int noiselevel, int blShift, float domIndex);
int DomMonitor(int morphtype, int rhythmclass, int beatWidth, int rr, bool reset);
int GetDomRhythm();
int GetRunCount();

bool AFDetect();
float ShanEntropy();
float ReEntropy(float meanval);
float ScatAnaly();

void ECGBubbleSort(int a[], int n);

int MeanCalc(int indata[], int datalen);

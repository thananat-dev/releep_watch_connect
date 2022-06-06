#ifndef NORCH_FILTER_H_
#define NORCH_FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#define ECG_HRV 1
#define LORENZ 1
#define DEBUG_ECG_DIAG 0
    
#define  ECG_BPM_FILTER_NOT_INIT		-1
#define	 ECG_BPM_FILTER_INIT_OK			0


	typedef enum 
	{
		LORENZ_TYPE			= 1,		//洛伦茨数组
		ECG_DGN_TYPE		= 2,		//心电诊断
		ECG_RR_INTER_TYPE	= 3,		//RR间期
		ECG_HRV_TYPE		= 4,		//ECG HRV
	}NTF_TYPE;

	typedef void(*Ecg_Evt_Handle)(NTF_TYPE evt_type, void* params); // 所有事件都在该回调函数中处理
#if defined(DEBUG_ECG_DIAG)
	typedef enum
	{
		HRM_RATE_RAPID = 1,
		HRM_RATE_SLOW = 2,
		INTER_SUSPENSION = 3,
	}ECG_DIAGNOSE;
#endif //DEBUG_ECG_DIAG

typedef enum bpm_sample_freq
{
	ECG_BPM_FREQ_125_HZ = 125,
	ECG_BPM_FREQ_200_HZ = 200,
	ECG_BPM_FREQ_250_HZ = 250,
}ECG_BPM_SAMPLE_FREQ_ENUM;
/*
@Fun: int  ecg_data_proc_init(ECG_BPM_SAMPLE_FREQ_ENUM ecgFs);
@Desc: initial data process with sample frequence
@ ret : 0 success,-1 failure
*/
#if defined(DEBUG_ECG_DIAG) || defined(LORENZ) || defined(ECG_HRV)
//int  ecg_data_proc_init(ECG_BPM_SAMPLE_FREQ_ENUM ecgFs, Ecg_diagnose_evt_handle evt_handle);
int ecg_data_proc_init(ECG_BPM_SAMPLE_FREQ_ENUM ecgFs, Ecg_Evt_Handle evt_handle, int is_encrypt);
#else
int  ecg_data_proc_init(ECG_BPM_SAMPLE_FREQ_ENUM ecgFs);
#endif //DEBUG_ECG_DIAG

/*
@fun :int  ecg_data_proc(float ecgdata)
@desc: sampled ecg data send to this function,then user can process the data with the ret value;
@
*/
float ecg_data_proc(int ecgdata);

char* getVersion();

/*
@fun: int  get_ecg_data_buf(float** buf, int len);
@desc : if the return value of function ecg_data_proc(float ecgdata)  is not 0,user need call this fun;
@example: int len = ecg_data_proc(YData);
			if (0!= len)
			{
				float * ecgData = (float*)malloc(ret * sizeof(float));
				memset(ecgData, 0, sizeof(len * sizeof(float)));
				get_ecg_data_buf(&ecgData, ret);

				for (int  i = 0; i< len; i++)
				{
					printf("%.2f\n", ecgData[i]);
				}
				free(ecgData);
			}
*/
//int  get_ecg_data_buf(float** buf, int len);

#ifdef __cplusplus
}
#endif

#endif


/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 // #include "board_init.h"
#include <stdio.h>
#include "demo_config.h"
#include "demo_info.h"
#include "fsl_debug_console.h"
#include "model.h"
#include "output_postproc.h"
#include "timer.h"

enum
{
	AD01_INT8,
	KWS_INT8,
	RESNET_INT8,
	VWW_INT8,

    NEUTRON_AD01_INT8,
	NEUTRON_KWS,
	NEUTRON_RESNET,
	NEUTRON_VWW,

    MODELS_NUM
};
extern uint8_t neutron_ad01_int8[];
extern uint8_t neutron_kws_ref_model[];
extern uint8_t neutron_pretrainedResnet_int8[];
extern uint8_t neutron_vww_96_int8[];

extern uint8_t ad01_int8[];
extern uint8_t kws_ref_model[];
extern uint8_t pretrainedResnet_int8[];
extern uint8_t vww_96_int8[];


#define CFG_EZH_STRIPE_HEIGHT   12
const uint8_t *mlperf_models[MODELS_NUM] = {
	ad01_int8,
	kws_ref_model,
	pretrainedResnet_int8,
	vww_96_int8,

    neutron_ad01_int8,
	neutron_kws_ref_model,
	neutron_pretrainedResnet_int8,
	neutron_vww_96_int8,
};

const char *mlperf_model_name[MODELS_NUM] = {
	    "AD01_Int8_TFLite Micro",
	    "KWS_Int8_TFLite Micro",
	    "ResNet_Int8_TFLite Micro",
	    "VWW_Int8_TFLite Micro",
	    "AD01_Int8_NEUTRON",
	    "KWS_Int8_NEUTRON",
	    "ResNet_Int8_NEUTRON",
	    "VWW_Int8_NEUTRON",
};

void ShowProfiling(void);

volatile uint8_t g_isImgBufReady = 1;

void* GetModelInputPointer(void) {
    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t* inputData = MODEL_GetInputTensorData(&inputDims, &inputType);
  return inputData;
}

void ResetProfiling(void);
volatile uint32_t g_tsNdx;
volatile uint32_t g_usTotal;
volatile uint32_t g_ts0;
volatile uint32_t g_customOpCnt;

/* Add an array for demo show */
volatile uint32_t g_msModelCalTime[8] = {0};

int run_model(uint8_t id, bool show_profile)
{
    const uint8_t *model_data;
    const int model_len = 0;
    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t* inputData;

    tensor_dims_t outputDims;
    tensor_type_t outputType;
    uint8_t* outputData;    
    
    if(id >= MODELS_NUM)
    {
        PRINTF("invild model id");
        return 0;
    }
    ResetProfiling();
    model_data = mlperf_models[id];
    
    PRINTF("---\r\nRun Model: %s\r\n",mlperf_model_name[id]);
    uint32_t startTime = TIMER_GetTimeInUS();
    if (MODEL_Invoke(model_data) != kStatus_Success)
    {
        PRINTF("Failed initializing model" EOL);
        for (;;) {}
    }
    uint32_t endTime = TIMER_GetTimeInUS();
    PRINTF("Inference during %d.%03d ms\r\n",(endTime-startTime) / 1000, (endTime-startTime) % 1000);
    
    g_usTotal = endTime - startTime;
    /* round up the ms */    
    g_msModelCalTime[id] = (g_usTotal % 1000) >= 500? 1:0;
    g_msModelCalTime[id] += g_usTotal / 1000;
    
    if (show_profile == 1)
    {
        ShowProfiling();
        
    }
    
    
    return 0;
}


int benchmark_run_all(bool show_profile)
{
    MODEL_Init();
    for(int i=0;i<MODELS_NUM;i++)
    {
        if(i == NEUTRON_AD01_INT8)
            PRINTF("\r\n\r\n*********Inference on Neutron\r\n");
        run_model(i,show_profile);
    }
    return 0;
}

//----------------------------------------------------------

typedef struct HookOp_struct {
	uint32_t opCode;
	const char* pszName;
}HookOp_t;
#define MAX_CUSTOM_OP_KINDS 4
#define CFG_TS_CNT    512
#define CFG_OPC_CNT    48
typedef struct tagOpProfile {
	uint32_t usTime;
	uint32_t x10000Rate;
	uint16_t ndx;
	uint16_t opCode;
	char szOpName[32];
}OpProfile_t;

OpProfile_t g_opPfs[CFG_TS_CNT], g_opcPfs[CFG_OPC_CNT];


const char* g_customOpNames[MAX_CUSTOM_OP_KINDS];

void ResetProfiling(void)
{
	g_tsNdx = g_usTotal = g_ts0 = g_customOpCnt = 0;
}

uint32_t TFLm_HookBeforeInvoke(HookOp_t* pCtx) {
			int customOpNameNdx = -1;
	g_ts0 = TIMER_GetTimeInUS();
	if (g_tsNdx < CFG_TS_CNT) {
		OpProfile_t* p = g_opPfs + g_tsNdx;
		p->ndx = (uint16_t)g_tsNdx;

		if (strlen(pCtx->pszName) < 32) {
			strcpy(p->szOpName, pCtx->pszName);
		}
		else {
			memcpy(p->szOpName, pCtx->pszName, 31);
			p->szOpName[31] = 0;
		}
		p->opCode = (uint16_t)pCtx->opCode;
		// special proces for custom operator
		if (pCtx->opCode == 32 && g_customOpCnt < MAX_CUSTOM_OP_KINDS) {
			// custom operator, check whether it has already appeared
			customOpNameNdx = -1;
			int k;
			for (k=0; k<g_customOpCnt; k++) {
				if (strcmp(g_customOpNames[k], pCtx->pszName) == 0) {                        
					break;
				}
			}
			customOpNameNdx = k;
			if (k == g_customOpCnt) {
				// find a new custom operator
				g_customOpNames[g_customOpCnt++] = pCtx->pszName;
			}
			
			p->opCode = 144 + customOpNameNdx;
		}            
	}
	return 0;
}

uint32_t TFLm_HookAfterInvoke(HookOp_t* pCtx) {
	uint32_t dt = TIMER_GetTimeInUS() - g_ts0;
	if (g_tsNdx < CFG_TS_CNT) {
		OpProfile_t* p = g_opPfs + g_tsNdx;
		p->usTime = dt;
		g_tsNdx++;
	}
	g_usTotal += dt;
	return 0;
}


static void SortDescending(OpProfile_t* p, uint32_t cnt) {
    int i, j;
    OpProfile_t tmp;
    for (i = 0; i < cnt; i++) {
        for (j = i + 1; j < cnt; j++) {
            if (p[i].usTime < p[j].usTime) {
                tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
}

void ShowProfiling(void)
{
    OpProfile_t* p;
    printf("\r\n--------------------------------------------------");
    printf("\r\n-------------------Profiling----------------------");
    printf("\r\n--------------------------------------------------\r\n");
    printf("CPU %dMHz, Total inference time: %d.%03dms\r\n", SystemCoreClock / 1000000, g_usTotal / 1000, g_usTotal % 1000);

    for (int i = 0; i < g_tsNdx; i++) {
        g_opPfs[i].x10000Rate = (uint32_t)((uint64_t)g_opPfs[i].usTime * 10000 / g_usTotal);
    }

    // SortDescending(g_opPfs, g_tsNdx);
    uint32_t usAcc = 0, pcntAcc = 0;
    printf("odr, ndx, time ms,   unit%%, total%%\r\n");
    for (int i = 0; i < g_tsNdx; i++) {
        // g_opPfs[i].x10000Rate = (uint32_t)((uint64_t)g_opPfs[i].usTime * 10000 / g_usTotal);
        usAcc += g_opPfs[i].usTime;
        pcntAcc = (uint64_t)usAcc * 10000 / g_usTotal;
        printf("%03d, %03d, %03d.%03dms, %02d.%02d%%, %02d.%02d%%, %s\r\n",
            i + 1, g_opPfs[i].ndx,
            g_opPfs[i].usTime / 1000, g_opPfs[i].usTime % 1000,
            g_opPfs[i].x10000Rate / 100, g_opPfs[i].x10000Rate % 100,
            pcntAcc / 100, pcntAcc % 100, g_opPfs[i].szOpName);
    }
    // calculate by operator type 
    printf("\r\n--------------------------------------------------");
    printf("\r\n                  per operator                    ");
    printf("\r\n--------------------------------------------------\r\n");
    printf("CPU %dMHz, Total inference time: %d.%03dms\r\n", SystemCoreClock / 1000000, g_usTotal / 1000, g_usTotal % 1000);
    {
        int opCodeNdx, i, opCodeTypeCnt = 0;
        const char* pszName;
        uint32_t pcnt;
        usAcc = 0;
        pcntAcc = 0;
        for (opCodeNdx = 0; opCodeNdx < 256; opCodeNdx++) {
            uint32_t opcUs = 0;
            uint32_t opInstanceCnt = 0;
            for (i = 0; i < g_tsNdx; i++) {
                if (g_opPfs[i].opCode != opCodeNdx) {
                    continue;
                }
                pszName = g_opPfs[i].szOpName;
                opcUs += g_opPfs[i].usTime;
                opInstanceCnt++;
            }
            if (0 == opcUs)
                continue;
            if (opCodeTypeCnt >= CFG_OPC_CNT) {
                continue;
            }
            g_opcPfs[opCodeTypeCnt].ndx = opInstanceCnt;
            pcnt = (uint64_t)opcUs * 10000 / g_usTotal;
            g_opcPfs[opCodeTypeCnt].x10000Rate = pcnt;
            g_opcPfs[opCodeTypeCnt].usTime = opcUs;
            g_opcPfs[opCodeTypeCnt].opCode = opCodeNdx;
            if (strlen(pszName) < 32) {
                strcpy(g_opcPfs[opCodeTypeCnt].szOpName, pszName);
            }
            else {
                memcpy(g_opcPfs[opCodeTypeCnt].szOpName, pszName, 31);
                g_opcPfs[opCodeTypeCnt].szOpName[31] = 0;
            }
            opCodeTypeCnt++;
        }
        SortDescending(g_opcPfs, opCodeTypeCnt);
        printf("odr, opc, time ms,   unit%%, total%%,count,name\r\n");
        for (int i = 0; i < opCodeTypeCnt; i++) {
            usAcc += g_opcPfs[i].usTime;
            pcntAcc = (uint64_t)usAcc * 10000 / g_usTotal;
            printf("%02d, %03d, %03d.%03dms, %02d.%02d%%, %02d.%02d%%, %03d, %s\r\n",
                i + 1, g_opcPfs[i].opCode,
                g_opcPfs[i].usTime / 1000, g_opcPfs[i].usTime % 1000,
                g_opcPfs[i].x10000Rate / 100, g_opcPfs[i].x10000Rate % 100,
                pcntAcc / 100, pcntAcc % 100, g_opcPfs[i].ndx, g_opcPfs[i].szOpName);
        }
    }
}

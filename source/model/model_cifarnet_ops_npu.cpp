/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/kernels/neutron/neutron.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver()
{
	static tflite::MicroMutableOpResolver<15> s_microOpResolver;

    s_microOpResolver.AddAdd();
	s_microOpResolver.AddConv2D();
	s_microOpResolver.AddDepthwiseConv2D();
	s_microOpResolver.AddAveragePool2D();
	s_microOpResolver.AddFullyConnected();
	s_microOpResolver.AddReshape();
	s_microOpResolver.AddSlice();
	s_microOpResolver.AddSoftmax();
	s_microOpResolver.AddDequantize();
	s_microOpResolver.AddMaxPool2D();
	s_microOpResolver.AddPad();
	s_microOpResolver.AddDetectionPostprocess();
	s_microOpResolver.AddConcatenation();
	s_microOpResolver.AddQuantize();

    s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(),
        tflite::Register_NEUTRON_GRAPH());

    return s_microOpResolver;
}

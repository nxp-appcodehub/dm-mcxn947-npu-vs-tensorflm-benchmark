/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
	.section .rodata
	.align 8
	.global neutron_ad01_int8
	.global neutron_ad01_int8_end
	.global neutron_kws_ref_model
	.global neutron_kws_ref_model_end
	.global neutron_pretrainedResnet_int8
	.global neutron_pretrainedResnet_int8_end
	.global neutron_vww_96_int8
	.global neutron_vww_96_int8_end

	.global ad01_int8
	.global ad01_int8_end
	.global kws_ref_model
	.global kws_ref_model_end
	.global pretrainedResnet_int8
	.global pretrainedResnet_int8_end
	.global vww_96_int8
	.global vww_96_int8_end



	.global img_data
	.global img_data_end


ad01_int8:
	.incbin "../source/model/ad01_int8.tflite"
ad01_int8_end:

kws_ref_model:
	.incbin "../source/model/kws_ref_model.tflite"
kws_ref_model_end:

pretrainedResnet_int8:
	.incbin "../source/model/pretrainedResnet_quant.tflite"
pretrainedResnet_int8_end:

vww_96_int8:
	.incbin "../source/model/vww_96_int8.tflite"
vww_96_int8_end:


neutron_ad01_int8:
	.incbin "../source/model/ad01_int8_converted.tflite"
neutron_ad01_int8_end:

neutron_kws_ref_model:
	.incbin "../source/model/kws_ref_model_converted.tflite"
neutron_kws_ref_model_end:

neutron_pretrainedResnet_int8:
	.incbin "../source/model/pretrainedResnet_quant_converted.tflite"
neutron_pretrainedResnet_int8_end:

neutron_vww_96_int8:
	.incbin "../source/model/vww_96_int8_converted.tflite"
neutron_vww_96_int8_end:

img_data:

img_data_end:

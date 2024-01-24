# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## MCXN947 NPU vs Tensorflm Benchmark

This demo is based on [MLPerf™ Tiny Deep Learning Benchmarks for Embedded Devices](https://github.com/mlcommons/tiny), it shows the performance benchmark between NPU and TensorFLM. 
Real time results are displayed on TFT LCD, On-chip NPU accelerates AI/ML algorithms and improves the performance.
Please note that this result is not verified by MLCommons Association.


|       Use Case       |                   Description                   |       Cortex-M33 Inference time       |    NPU Inference time   |    CM33 / NPU ratio    |
|----------------------|-------------------------------------------------|---------------------------------------|-------------------------|------------------------|
|   Anomaly Detection  | Detecting anomalies in machine operating sounds |              9.452ms                  |       4.174ms           |          2.26          |
|   Keyword Spotting   |        Small vocabulary keyword spotting        |              67.794ms                 |       7.164ms           |          9.46          |
| Image Classification |            Small image classification           |              247.253ms                |       10.6ms            |         23.33          |
|   Visual Wake Words  |           Binary image classification           |              218.563ms                |       10.169ms          |         21.49          |

>**Note**: Unverified MLPerf® Tiny v1.0. Result not verified by MLCommons Association. The MLPerf name and logo are registered and unregistered trademarks of MLCommons Association in the United States and other countries. All rights reserved. Unauthorized use strictly prohibited. See www.mlcommons.org for more information. 

#### Boards: FRDM-MCXN947
#### Categories: AI/ML, Graphics
#### Peripherals: DISPLAY
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [FAQs](#step5) 
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software<a name="step1"></a>
* [MCUXpresso IDE V11.9.0 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).
* SDK_2_14_0_FRDM-MCXN947
* MCUXpresso for Visual Studio Code: This example supports MCUXpresso for Visual Studio Code, for more information about how to use Visual Studio Code please refer [here](https://www.nxp.com/design/training/getting-started-with-mcuxpresso-for-visual-studio-code:TIP-GETTING-STARTED-WITH-MCUXPRESSO-FOR-VS-CODE).

## 2. Hardware<a name="step2"></a>
* Board: FRDM-MCXN947
* LCD module: [Mikroe TFT PROTO 5" Capacitive](https://www.mikroe.com/tft-proto-5-capacitive-board)
* Type-C USB cable
* Personal Computer

## 3. Setup<a name="step3"></a>

### 3.1 Step 1
Connect the LCD to the FRDM-MCXN947, as shown below:  
![hardware](./image/hardware.png)

### 3.2 Step 2
* Import the project to MCUXpresso IDE.
1. Open MCUXpresso IDE, in the Quick Start Panel, choose **Import from Application Code Hub** 

	![](image/import_project_1.png)

2. Enter the demo name in the search bar.

	![](image/import_project_2.png) 

3. Click **Copy GitHub link**, MCUXpresso IDE will automatically retrieve project attributes, then click **Next>**.

	![](image/import_project_3.png)

4. Select **main** branch and then click **Next>**, Select the MCUXpresso project, click **Finish** button to complete import.

	![](image/import_project_4.png)

* Compile and download to the board. 
  * *For better performance, we can change the "build configuration" from "Debug" to "Release"*.
* Reset and run.

## 4. Results<a name="step4"></a>
The benchmark will be shown in the screen:  
![result](./image/result.png)  
>**Note**: Unverified MLPerf® Tiny v1.0. Result not verified by MLCommons Association. The MLPerf name and logo are registered and unregistered trademarks of MLCommons Association in the United States and other countries. All rights reserved. Unauthorized use strictly prohibited. See www.mlcommons.org for more information.   

It displays inference time, with the green bar indicating the time used by MCXN947 NPU and the blue bar indicating the time used for pure software(TensorFlow Lite Micro) calculations. Through the GUI, it can be visually seen that NPU has taken shorter time, resulting in better performance.

## 5. FAQs<a name="step5"></a>
No FAQs have been identified for this project.

## 6. Support<a name="step6"></a>


#### Project Metadata
<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXN947-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXN947+in%3Areadme&type=Repositories)

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-AI/ML-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+aiml+in%3Areadme&type=Repositories) [![Category badge](https://img.shields.io/badge/Category-GRAPHICS-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+graphics+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-DISPLAY-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+display+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 7. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        | January 30<sup>th</sup> 2024 |


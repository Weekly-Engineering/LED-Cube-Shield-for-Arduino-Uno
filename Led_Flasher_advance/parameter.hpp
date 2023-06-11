//=====================================================================================
//parameter.hpp V1.0
//Header file for parameter.
//See https://weeklyengineering.com/led-cube-sheeld-for-arduino-uno
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

#ifndef _PARAMETER_HPP_
#define _PARAMETER_HPP_

//ピン設定
const int LED_PIN_Z_AXIS[3] = {A0, 3, 4};  //LED Cube Z軸方向
const int LED_PIN_XY_AXIS[9] = {5, 6, 7, 8, 9, 10, 11, 12, 13};   //LED Cube XY平面
const int R_VARI_PIN = A1;  //可変抵抗
const int SW_INTERRRUPT_PIN = 2;  //スイッチ

#endif // _PARAMETER_HPP_
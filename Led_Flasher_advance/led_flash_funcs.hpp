//=====================================================================================
//led_flash_funcs.hpp V1.0
//Header file for led_flash_funcs.cpp
//See https://weeklyengineering.com/led-cube-sheeld-for-arduino-uno
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

#ifndef _LED_FLASH_FUNCS_HPP_
#define _LED_FLASH_FUNCS_HPP_

//LED点灯パターン平行移動方向制御用
enum class E_Shift_Direction {
  Minus,  //0
  Plus    //1
};

//LED点灯パターン保持用
enum class E_Flash_Hold {
  Do_Not_Hold,  //0
  Hold          //1
};

//LED照度調整用
enum class E_Adjust_Brightness {
  Do_Not_Adjust,  //0
  Turn_Down,      //1
  Turn_Up         //2
};

class Led{
  public:
  Led();
  Led(int brightness, E_Adjust_Brightness adjust_brightness);

  void led_flash_millis(unsigned long flash_time_ms);
  void led_off();
  void set_led_flash_pattern(int z, int xy, int brightness);
  int get_led_flash_pattern(int z, int xy);
  void adjust_brightness_of_leds();
  void set_adjust_brightness_param(int z, int xy, E_Adjust_Brightness adjust_brightness);
  E_Adjust_Brightness get_adjust_brightness_param(int z, int xy);
  void flash_pattern_shift_x(E_Shift_Direction direction, E_Flash_Hold flash_hold);
  void flash_pattern_shift_y(E_Shift_Direction direction, E_Flash_Hold flash_hold);
  void flash_pattern_shift_z(E_Shift_Direction direction, E_Flash_Hold flash_hold);
  
  private:
  static const int V_VAL_MAX = 515; //可変抵抗によるインプット電圧の最大値
  static const int V_VAL_MIN = 0;   //可変抵抗によるインプット電圧の最小値
  static const double MAGNIFICATION_MAX = 2.0;  //可変抵抗によって設定される点灯時間倍率の最大値
  static const double MAGNIFICATION_MIN = 0.5;  //可変抵抗によって設定される点灯時間倍率の最小値
  const double A = (MAGNIFICATION_MAX - MAGNIFICATION_MIN) / (V_VAL_MAX - V_VAL_MIN); //係数
  const double B = MAGNIFICATION_MAX - A * V_VAL_MAX; //定数項

  int led_flash_pattern[3][9];  //LED点灯パターン
  E_Adjust_Brightness adjust_brightness_param[3][9]; //LED照度調整パラメータ
};

class Timer{
  public:
  bool timer_micros(unsigned long time_micros);
  
  static const unsigned long DELAY_TIME_MICROS = 1; //ダイナミック点灯周期

  private:
  unsigned long calc_elapsed_time();
  
  unsigned long ref_time_micros = 0;  //時間計測開始時間
  bool timer_flg = false; //タイマー起動中フラグ
  
};

class Mode{
  public:
  static void raise_interrupt_flg();
  static void mode_increment();
  static int get_mode_num();
  static bool get_interrupt_flg();

  static void mode0();
  static void mode1();
  static void mode2();

  private:
  static const int MODE_MAX = 3;  //LED点灯モード数
  static int mode_num;  //LED点灯モード番号
  static bool interrupt_flg;  //割り込み有無フラグ
};

#endif // _LED_FLASH_FUNCS_HPP_
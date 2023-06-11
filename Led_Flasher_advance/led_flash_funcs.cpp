//=====================================================================================
//led_flash_funcs.cpp V1.0
//Functions for flashing LED-cube.
//See https://weeklyengineering.com/led-cube-sheeld-for-arduino-uno
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

#include <Arduino.h>
#include "led_flash_funcs.hpp"
#include "parameter.hpp"

//コンストラクタ、メンバ変数の初期化
Led::Led(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 9; j++){
      led_flash_pattern[i][j] = 0;
      adjust_brightness_param[i][j] = E_Adjust_Brightness::Do_Not_Adjust;
    }    
  }
}

//コンストラクタのオーバーロード、メンバ変数の初期化
Led::Led(int brightness, E_Adjust_Brightness adjust_brightness){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 9; j++){
      led_flash_pattern[i][j] = brightness;
      adjust_brightness_param[i][j] = adjust_brightness;
    }    
  }
}

//メンバ変数led_flash_patternに基づき、flash_time_ms[ms]で指定された基準時間 * 点灯時間倍率分、LEDを点灯させる。
//led_flash_pattern[i][j] = 0 :点灯無
//led_flash_pattern[i][j] = 1 :点灯有、少し明るい
//led_flash_pattern[i][j] = 2 :点灯有、明るい
//led_flash_pattern[i][j] = 3 :点灯有、とても明るい
//【引数】
//unsigned long flash_time_ms :LED点灯基準時間
void Led::led_flash_millis(unsigned long flash_time_ms){
  if(Mode::get_interrupt_flg() == false){
    Timer* flash = new Timer;
    Timer* one_micros = new Timer;
  
    flash_time_ms = (unsigned long)(flash_time_ms * (A * analogRead(R_VARI_PIN) + B));

    while(flash->timer_micros(flash_time_ms * 1000) == false){ 
      for(int i = 0; i < 3; i++){
        digitalWrite(LED_PIN_Z_AXIS[i], HIGH);
        for(int t = 0; t < 100; t++){
          for(int j = 0; j < 9; j++){
            if(led_flash_pattern[i][j] == 1){
              if(t < 1){
                digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
              } else {
                digitalWrite(LED_PIN_XY_AXIS[j], LOW);
              }
            } else if(led_flash_pattern[i][j] == 2){
              if(t < 10){
                digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
              } else {
                digitalWrite(LED_PIN_XY_AXIS[j], LOW);
              }
            } else if(led_flash_pattern[i][j] == 3){
              if(t < 100){
                digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
              } else {
                digitalWrite(LED_PIN_XY_AXIS[j], LOW);
              }
            }
          }
          while(one_micros->timer_micros(Timer::DELAY_TIME_MICROS) == false){ 
          }
        }
        Led::led_off();
      }
    }
    delete flash;
    delete one_micros;
  } else {
    //何もしない
  }
}

//全てのLEDを消灯させる
void Led::led_off(){
  for(int i = 0; i < 3; i++){
    digitalWrite(LED_PIN_Z_AXIS[i], LOW);
  }
  for(int i = 0; i < 9; i++){
    digitalWrite(LED_PIN_XY_AXIS[i], LOW);
  }
}

//任意の場所のLEDの照度をled_flash_patternに設定する。
//【引数】
//int z           :LEDの高さ、0：1段目、1：2段目、2：3段目
//int xy          :zで指定した段数におけるLEDのXY座標
//                 0　1　2
//                 3  4  5
//                 6  7  8
//int brightness  :照度
//                  0 ：点灯無
//　                1 ：点灯有、少し明るい
//　                2 ：点灯有、明るい
//　                3 ：点灯有、とても明るい
void Led::set_led_flash_pattern(int z, int xy, int brightness){
  led_flash_pattern[z][xy] = brightness;
}

//任意の場所のLEDの照度をled_flash_patternから読み込む。
//【引数】
//int z           :LEDの高さ、0：1段目、1：2段目、2：3段目
//int xy          :zで指定した段数におけるLEDのXY座標
//                 0　1　2
//                 3  4  5
//                 6  7  8
//【戻り値】
//int brightness  :照度
//                  0 ：点灯無
//　                1 ：点灯有、少し明るい
//　                2 ：点灯有、明るい
//　                3 ：点灯有、とても明るい
int Led::get_led_flash_pattern(int z, int xy){
  return led_flash_pattern[z][xy];
}

//任意の場所のLEDの照度調整用パラメータをadjust_brightness_paramに設定する。
//【引数】
//int z                                   :LEDの高さ、0：1段目、1：2段目、2：3段目
//int xy                                  :zで指定した段数におけるLEDのXY座標
//                                          0　1　2
//                                          3  4  5
//                                          6  7  8
//E_Adjust_Brightness adjust_brightness   :照度調整用パラメータ
//                                          Do_Not_Adjust :照度調整なし
//                                          Turn_Down     :照度を暗くする
//                                          Turn_Up       :照度を明るくする
void Led::set_adjust_brightness_param(int z, int xy, E_Adjust_Brightness adjust_brightness){
  adjust_brightness_param[z][xy] = adjust_brightness;
}

//任意の場所のLEDの照度調整用パラメータをadjust_brightness_paramから読み込む。
//【引数】
//int z                                        :LEDの高さ、0：1段目、1：2段目、2：3段目
//int xy                                       :zで指定した段数におけるLEDのXY座標
//                                               0　1　2
//                                               3  4  5
//                                               6  7  8
//【戻り値】
//E_Adjust_Brightness adjust_brightness_param   :照度調整用パラメータ
//                                                Do_Not_Adjust :照度調整なし
//                                                Turn_Down     :照度を暗くする
//                                                Turn_Up       :照度を明るくする
E_Adjust_Brightness Led::get_adjust_brightness_param(int z, int xy){
  return adjust_brightness_param[z][xy];
}

//メンバ変数adjust_flgに基づき、各LEDの照度を調整する。
//照度が最大の場合はそれ以上照度を明るくせず、消灯している場合はそれ以上暗くしない。
//adjust_brightness_param[i][j] = Turn_Up    ：1段階明るくする
//adjust_brightness_param[i][j] = Turn_Down  ：1段階暗くする
void Led::adjust_brightness_of_leds(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 9; j++){
      if(adjust_brightness_param[i][j] == E_Adjust_Brightness::Turn_Up){
        if(led_flash_pattern[i][j] < 3){
          led_flash_pattern[i][j] = led_flash_pattern[i][j] + 1;
        }
      } else {
        if(0 < led_flash_pattern[i][j]){
          led_flash_pattern[i][j] = led_flash_pattern[i][j] - 1;
        }
      }
    }
  }
}

//led_flash_patternをX軸方向にシフトさせる。
//【引数】
//E_Shift_Direction direction :シフトさせる方向
//E_Flash_Hold flash_hold     :X軸方向にシフトさせた際に座標外にシフトしたled_flash_pattern[i][j]を保持し、
//                             シフトさせた方向とは反対端のled_flash_pattern[i][j]への反映有無
void Led::flash_pattern_shift_x(E_Shift_Direction direction, E_Flash_Hold flash_hold){
  int temp;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(direction == E_Shift_Direction::Plus){
        temp = led_flash_pattern[i][3 * j + 2];
        led_flash_pattern[i][3 * j + 2] = led_flash_pattern[i][3 * j + 1];
        led_flash_pattern[i][3 * j + 1] = led_flash_pattern[i][3 * j + 0];
        if(flash_hold == E_Flash_Hold::Hold){
          led_flash_pattern[i][3 * j + 0] = temp;
        } else {
          led_flash_pattern[i][3 * j + 0] = 0;
        }
      } else {
        temp = led_flash_pattern[i][3 * j + 0];
        led_flash_pattern[i][3 * j + 0] = led_flash_pattern[i][3 * j + 1];
        led_flash_pattern[i][3 * j + 1] = led_flash_pattern[i][3 * j + 2];
        if(flash_hold == E_Flash_Hold::Hold){
          led_flash_pattern[i][3 * j + 2] = temp;
        } else {
          led_flash_pattern[i][3 * j + 2] = 0;
        }
      }
    }
  }    
}

//led_flash_patternをY軸方向にシフトさせる。
//【引数】
//E_Shift_Direction direction :シフトさせる方向
//E_Flash_Hold flash_hold     :Y軸方向にシフトさせた際に座標外にシフトしたled_flash_pattern[i][j]を保持し、
//                             シフトさせた方向とは反対端のled_flash_pattern[i][j]への反映有無
void Led::flash_pattern_shift_y(E_Shift_Direction direction, E_Flash_Hold flash_hold){
  int temp;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(direction == E_Shift_Direction::Plus){
        temp = led_flash_pattern[i][j + 6];
        led_flash_pattern[i][j + 6] = led_flash_pattern[i][j + 3];
        led_flash_pattern[i][j + 3] = led_flash_pattern[i][j + 0];
        if(flash_hold == E_Flash_Hold::Hold){
          led_flash_pattern[i][j + 0] = temp;
        } else {
          led_flash_pattern[i][j + 0] = 0;
        }
      } else {
        temp = led_flash_pattern[i][j + 0];
        led_flash_pattern[i][j + 0] = led_flash_pattern[i][j + 3];
        led_flash_pattern[i][j + 3] = led_flash_pattern[i][j + 6];
        if(flash_hold == E_Flash_Hold::Hold){
          led_flash_pattern[i][j + 6] = temp;
        } else {
          led_flash_pattern[i][j + 6] = 0;
        }
      }
    }
  }    
}

//led_flash_patternをZ軸方向にシフトさせる。
//【引数】
//E_Shift_Direction direction :シフトさせる方向
//E_Flash_Hold flash_hold     :Z軸方向にシフトさせた際に座標外にシフトしたled_flash_pattern[i][j]を保持し、
//                             シフトさせた方向とは反対端のled_flash_pattern[i][j]への反映有無
void Led::flash_pattern_shift_z(E_Shift_Direction direction, E_Flash_Hold flash_hold){
  int temp;
  for(int i = 0; i < 9; i++){
    if(direction == E_Shift_Direction::Plus){
      temp = led_flash_pattern[2][i];
      led_flash_pattern[2][i] = led_flash_pattern[1][i];
      led_flash_pattern[1][i] = led_flash_pattern[0][i];
      if(flash_hold == E_Flash_Hold::Hold){
        led_flash_pattern[0][i] = temp;
      } else {
        led_flash_pattern[0][i] = 0;
      }
    } else {
      temp = led_flash_pattern[0][i];
      led_flash_pattern[0][i] = led_flash_pattern[1][i];
      led_flash_pattern[1][i] = led_flash_pattern[2][i];
      if(flash_hold == E_Flash_Hold::Hold){
        led_flash_pattern[2][i] = temp;
      } else {
        led_flash_pattern[2][i] = 0;
      }
    }
  }    
}

//設定時間が経過するとtrueを返す。
//【引数】
//unsigned long time_micros :設定時間[us]
bool Timer::timer_micros(unsigned long time_micros){
  if(timer_flg == false){
      ref_time_micros = micros();
      timer_flg = true;
  }
  if(calc_elapsed_time() < time_micros){
      return false;
  } else {
      timer_flg = false;
      return true;
  }
}

//タイマー計測開始時刻からの経過時間を返す。
//【戻り値】
//unsigned long cur_time_micros - ref_time_micros :タイマー計測開始時刻からの経過時間
unsigned long Timer::calc_elapsed_time(){
  unsigned long cur_time_micros = micros();
  return cur_time_micros - ref_time_micros;
}

//割り込み有無フラグを有効化する(割り込み処理)。
void Mode::raise_interrupt_flg(){
  Mode::interrupt_flg = true;
}

//点灯モード番号をインクリメントすることで点灯モードの切り替えを実施し、割り込み有無フラグを無効化する。
void Mode::mode_increment(){
  if(mode_num < MODE_MAX - 1){
    mode_num++;
  } else {
    mode_num = 0;
  }
  delay(500);
  interrupt_flg = false;
}

//現在の点灯モード番号を返す。
//【戻り値】
//int mode_num  :現在の点灯モード番号
int Mode::get_mode_num(){
  return mode_num;
}

//現在の割り込み有無フラグを返す。
//【戻り値】
//bool interrupt_flg  :現在の割り込み有無フラグ
bool Mode::get_interrupt_flg(){
  return interrupt_flg;
}

//点灯モード0
void Mode::mode0(){
  Led* mode0 = new Led;
  while(Mode::get_interrupt_flg() == false){
    if(random(5) == 0){ //20%の確率で新しい雨が生成される
      mode0->set_led_flash_pattern(2, random(9), 3);
    }
    mode0->led_flash_millis(120);
    mode0->flash_pattern_shift_z(E_Shift_Direction::Minus , E_Flash_Hold::Do_Not_Hold);
  }
  delete mode0;
}

//点灯モード1
void Mode::mode1(){
  Led* mode1 = new Led(0, E_Adjust_Brightness::Turn_Up);
  while(Mode::get_interrupt_flg() == false){
    if(random(5) == 0){ //20%の確率
      int z = random(3);
      int xy = random(9);
      mode1->set_adjust_brightness_param(z, xy, E_Adjust_Brightness::Turn_Up);
    }
    
    mode1->led_flash_millis(120);
    mode1->adjust_brightness_of_leds();
    
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 9; j++){
        if(mode1->get_led_flash_pattern(i, j) == 3){
          mode1->set_adjust_brightness_param(i, j, E_Adjust_Brightness::Turn_Down); 
        }
      }
    }
  }
  delete mode1;
}

//点灯モード2
void Mode::mode2(){
  while(Mode::get_interrupt_flg() == false){
    Led* mode2 = new Led(3, E_Adjust_Brightness::Turn_Up);
    mode2->led_flash_millis(1000);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);

    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_z(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_z(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);

    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);

    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(400);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(400);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(300);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(300);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(200);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
    mode2->led_flash_millis(200);

    for(int i = 0; i < 5; i++){
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Do_Not_Hold);
      mode2->led_flash_millis(100);
    }
    for(int i = 0; i < 9; i++){
      mode2->set_led_flash_pattern(1, i, 1);
    }
    for(int i = 0; i < 5; i++){
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(50);
    }
    for(int i = 0; i < 9; i++){
      mode2->set_led_flash_pattern(1, i, 2);
      mode2->set_led_flash_pattern(2, i, 1);
    }
    for(int i = 0; i < 5; i++){
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(20);
    }
    for(int i = 0; i < 9; i++){
      mode2->set_led_flash_pattern(1, i, 3);
      mode2->set_led_flash_pattern(2, i, 2);
    }
    for(int i = 0; i < 5; i++){
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
      mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
      mode2->led_flash_millis(10);
    }
    for(int i = 0; i < 9; i++){
      mode2->set_led_flash_pattern(2, i, 3);
    }
    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(5);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(5);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(10);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(10);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(50);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Plus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(50);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(100);
    mode2->flash_pattern_shift_y(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(100);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(500);
    mode2->flash_pattern_shift_x(E_Shift_Direction::Minus, E_Flash_Hold::Hold);
    mode2->led_flash_millis(500);
    
    mode2->set_led_flash_pattern(0, 3, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 6, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 7, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 4, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 1, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 2, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 5, 3);
    mode2->led_flash_millis(500);
    mode2->set_led_flash_pattern(0, 8, 3);
    mode2->led_flash_millis(1000);
    delete mode2;
  }
}

int Mode::mode_num = 0;
bool Mode::interrupt_flg = false;
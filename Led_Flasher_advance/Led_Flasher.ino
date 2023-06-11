//=====================================================================================
//Led_Flasher.ino V1.0
//Control Flashing of 3*3*3 LED-Cube.
//See https://weeklyengineering.com/led-cube-sheeld-for-arduino-uno
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

#include "led_flash_funcs.hpp"
#include "parameter.hpp"

void setup() {
  for(int i = 0; i < 3; i++){
    pinMode(LED_PIN_Z_AXIS[i], OUTPUT);
  }
  for(int i = 0; i < 9; i++){
    pinMode(LED_PIN_XY_AXIS[i], OUTPUT);
  }
  pinMode(R_VARI_PIN, INPUT);
  pinMode(SW_INTERRRUPT_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(SW_INTERRRUPT_PIN), Mode::raise_interrupt_flg, RISING);
}

void loop(){
  if(Mode::get_interrupt_flg() == true){
    Mode::mode_increment();
  }
  if(Mode::get_mode_num() == 0){
    Mode::mode0();
  } else if(Mode::get_mode_num() == 1){
    Mode::mode1();
  } else if(Mode::get_mode_num() == 2){
    Mode::mode2();
  }
}


//=====================================================================================
//Led_Flasher_basic.ino V1.0
//Control Flashing of 3*3*3 LED-Cube simply.
//See https://weeklyengineering.com/led-cube-sheeld-for-arduino-uno
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

void led_off();

//ピン設定
const int LED_PIN_Z_AXIS[3] = {A0, 3, 4};  //LED Cube Z軸方向
const int LED_PIN_XY_AXIS[9] = {5, 6, 7, 8, 9, 10, 11, 12, 13};   //LED Cube XY平面

void setup() {
  for(int i = 0; i < 3; i++){
    pinMode(LED_PIN_Z_AXIS[i], OUTPUT);
  }
  for(int i = 0; i < 9; i++){
    pinMode(LED_PIN_XY_AXIS[i], OUTPUT);
  }
}

void loop(){
  //ダイナミック点灯で全LED点灯
  for(int t = 0; t < 1000; t++){
    for(int i = 0; i < 3; i++){
      digitalWrite(LED_PIN_Z_AXIS[i], HIGH);  //点灯させるLEDのZ座標(0~2)を指定
      for(int j = 0; j < 9; j++){
        digitalWrite(LED_PIN_XY_AXIS[j], HIGH);  //点灯させるLEDのXY座標(0~9)を指定
      }
      delay(1); //Z座標ごとに約1msのみ点灯
      led_off();  //LED消灯
    }
  }

  //LED消灯
  led_off();
  delay(1000);

  //全LEDを一つずつ点灯
  for(int i = 0; i < 3; i++){
    digitalWrite(LED_PIN_Z_AXIS[i], HIGH);  //点灯させるLEDのZ座標(0~2)を指定
    for(int j = 0; j < 9; j++){
      digitalWrite(LED_PIN_XY_AXIS[j], HIGH);  //点灯させるLEDのXY座標(0~9)を指定
      delay(100);
      digitalWrite(LED_PIN_XY_AXIS[j], LOW);  //点灯させたLEDを消灯
    }
    digitalWrite(LED_PIN_Z_AXIS[i], LOW);  //点灯させたLEDを消灯
  }

  //LED消灯
  led_off();
  delay(1000);


  //ダイナミック点灯でパターン点灯
  for(int t = 0; t < 100; t++){
    for(int i = 0; i < 3; i++){
      led_off();
      digitalWrite(LED_PIN_Z_AXIS[i], HIGH);  //点灯させるLEDのZ座標(0~2)を指定
      for(int j = 0; j < 9; j++){
        if((j + i) % 2 == 0){
          digitalWrite(LED_PIN_XY_AXIS[j], HIGH);  //点灯させるLEDのXY座標(0~9)を指定
        }
      }
      delay(1);
    }
  }

  for(int t = 0; t < 100; t++){
    for(int i = 0; i < 3; i++){
      led_off();
      digitalWrite(LED_PIN_Z_AXIS[i], HIGH);  //点灯させるLEDのZ座標(0~2)を指定
      for(int j = 0; j < 9; j++){
        if((j + i) % 2 == 1){
          digitalWrite(LED_PIN_XY_AXIS[j], HIGH);  //点灯させるLEDのXY座標(0~9)を指定
        }
      }
      delay(1);
    }
  }

  //LED消灯
  led_off();
  delay(1000);
}

//全LEDを消灯
void led_off(){
  for(int i = 0; i < 3; i++){
    digitalWrite(LED_PIN_Z_AXIS[i], LOW);
  }
  for(int i = 0; i < 9; i++){
    digitalWrite(LED_PIN_XY_AXIS[i], LOW);
  }
}
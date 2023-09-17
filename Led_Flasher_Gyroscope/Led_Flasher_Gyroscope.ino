//=====================================================================================
//Led_Flasher_Gyroscope.ino V1.0
//Control Flashing of 3*3*3 LED-Cube by LSM9DS1.
//See https://weeklyengineering.com/arduino-uno-r4-minima-ltika
//
//Copyright (c) 2023 Weekly Engineering
//
//Released under the MIT license.
//See https://opensource.org/licenses/MIT
//=====================================================================================

#include <Arduino.h>
#include <Wire.h>
#include <LSM9DS1.h>

//ピン設定
const int LED_PIN_Z_AXIS[3] = {A0, 3, 4};  //LED Cube Z軸方向
const int LED_PIN_XY_AXIS[9] = {5, 6, 7, 8, 9, 10, 11, 12, 13};   //LED Cube XY平面

int x_mdps = 0;
int y_mdps = 0;
int z_mdps = 0;

double x_drift_offset_mdps;
double y_drift_offset_mdps;
double z_drift_offset_mdps;

double x_deg = 0;
double y_deg = 0;
double z_deg = 0;

unsigned long new_time = 0;
unsigned long old_time = 0;

// the setup function runs once when you press reset or power the board
void setup() {
    for(int i = 0; i < 3; i++){
      pinMode(LED_PIN_Z_AXIS[i], OUTPUT);
    }
    for(int i = 0; i < 9; i++){
      pinMode(LED_PIN_XY_AXIS[i], OUTPUT);
    }
    Wire.begin();
    smeGyroscope.begin();
    calc_drift_correction();
    old_time = millis();
}

void calc_drift_correction(){
    for(int i = 0; i < 100; i++){
        x_drift_offset_mdps += smeGyroscope.readX();
        y_drift_offset_mdps += smeGyroscope.readY();
        z_drift_offset_mdps += smeGyroscope.readZ();
    }

    x_drift_offset_mdps = x_drift_offset_mdps / 100;
    y_drift_offset_mdps = y_drift_offset_mdps / 100;
    z_drift_offset_mdps = z_drift_offset_mdps / 100;
}

void led_off(){
  for(int i = 0; i < 3; i++){
    digitalWrite(LED_PIN_Z_AXIS[i], LOW);
  }
  for(int i = 0; i < 9; i++){
    digitalWrite(LED_PIN_XY_AXIS[i], LOW);
  }
}

// the loop function runs over and over again forever
void loop() {
    new_time = millis();

    x_deg += (double)(smeGyroscope.readX() - (int)x_drift_offset_mdps) * ((double)(new_time - old_time) / 1000) / 1000;
    y_deg += (double)(smeGyroscope.readY() - (int)y_drift_offset_mdps) * ((double)(new_time - old_time) / 1000) / 1000;
    z_deg += (double)(smeGyroscope.readZ() - (int)z_drift_offset_mdps) * ((double)(new_time - old_time) / 1000) / 1000;

    old_time = new_time;

    if(x_deg < -60 || 60 < x_deg){
        for(int t = 0; t < 50; t++){
            for(int i = 0; i < 3; i++){
                digitalWrite(LED_PIN_Z_AXIS[i], HIGH);
                for(int j = 3; j < 6; j++){
                    digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
                }
                delay(1);
                led_off();
            }
        }
    } else if(x_deg < -30){
        for(int t = 0; t < 50; t++){
            for(int i = 0; i < 3; i++){
                digitalWrite(LED_PIN_Z_AXIS[i], HIGH);
                for(int j = i * 3; j < i * 3 + 3; j++){
                    digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
                }
                delay(1);
                led_off();
            }
        }
    } else if(x_deg < 30){
        for(int t = 0; t < 50; t++){
            digitalWrite(LED_PIN_Z_AXIS[1], HIGH);
            for(int j = 0; j < 9; j++){
                digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
            }
            delay(3);
            led_off();
        }
    } else if(x_deg < 60){
        for(int t = 0; t < 50; t++){
            for(int i = 0; i < 3; i++){
                digitalWrite(LED_PIN_Z_AXIS[i], HIGH);
                for(int j = 8 - (i * 3); j > 8 - (i * 3) - 3; j--){
                    digitalWrite(LED_PIN_XY_AXIS[j], HIGH);
                }
                delay(1);
                led_off();
            }
        }
    }
}

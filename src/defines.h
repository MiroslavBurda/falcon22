#include "BluetoothSerial.h"
#include <Servo.h>

Servo servo0, servo1, servo2, servo3;
int position_servo0 = 85;
int position_servo1 = 165;
int position_servo2 = 180;
int position_servo3 = 160;
int alfa = 60;
double alfa_rad = 1.04719755119659;
int beta = 30;
double beta_rad = 0.52359877559829;
int gama = 90;
double gama_rad = 1.57079632679489;
float arm1 = 120.00;         //v mm
float arm2 = 80.00;          //v mm
float prepona_arm = 143.00;  //v mm
float delka_arm = 70.00;     //v mm
float vyska_arm = 120.00;    //v mm
float max_arm = arm1 + arm2;
float min_arm = 0.00;
float krok_arm = 3.00;
int krok_serva = 2;
int motor_power = 80;
bool L_G_light = false; // pro blikani zelene LED - indikuje, ze deska funguje
int otocka_kola = 13 * 2400 ; // převodovka (1:5) 1:8,  2400 tiků enkodéru na otáčku motoru
int speed_coef = 200000; // nasobeni hodnoty, co leze z joysticku

int axis[7] = {5,6,7,8,9,10,11};
byte btn[8] = {0,0,0,0,0,0,0,0};
byte btn_last[8] = {0,0,0,0,0,0,0,0};

BluetoothSerial SerialBT;
Stream* serial = nullptr;

/*
 * motorcontrol.c
 *
 *  Created on: Nov 22, 2025
 *      Author: hcwon
 */

#include "motorcontrol.h"
#include "string.h"
#include "math.h"

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;

// Global variables
float movetime = (1/PWMFREQUENCY)*1000;
//should be no less than (1/pwmfrequency)*1000 //one pwmcycle per move
Point cur = {0, 0, 0};
Point final = {0, 0, 0};
Point intermediate = {0, 0, 0};
Point storage1;
Point storage2;
Direction pastdirection;

  // 1d
  //x
  void movepx(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
  }

  void movenx(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
  }

  //y
  void movepy(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  void moveny(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  //z
  void movepz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  //2d
  //xy
  void movepxpy(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  void movepxny(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  void movenxpy(float time){
  	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  void movenxny(float time){
  	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
  }

  //xz
  void movepxpz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movepxnz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxpz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxnz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  //yz
  void movepypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movepynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  //3d
  void movepxpypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxpypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movepxnypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movepxpynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxnypz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxpynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movepxnynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void movenxnynz(float time){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
		HAL_Delay(time);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  }

  void moveindirection(Direction direction){
      switch(direction){
          case DIR_PX: movepx(movetime); break;
          case DIR_NX: movenx(movetime); break;
          case DIR_PY: movepy(movetime); break;
          case DIR_NY: moveny(movetime); break;
          case DIR_PZ: movepz(movetime); break;
          case DIR_NZ: movenz(movetime); break;

          case DIR_PXPY: movepxpy(movetime); break;
          case DIR_PXNY: movepxny(movetime); break;
          case DIR_NXPY: movenxpy(movetime); break;
          case DIR_NXNY: movenxny(movetime); break;

          case DIR_PXPZ: movepxpz(movetime); break;
          case DIR_PXNZ: movepxnz(movetime); break;
          case DIR_NXPZ: movenxpz(movetime); break;
          case DIR_NXNZ: movenxnz(movetime); break;

          case DIR_PYPZ: movepypz(movetime); break;
          case DIR_PYNZ: movepynz(movetime); break;
          case DIR_NYPZ: movenypz(movetime); break;
          case DIR_NYNZ: movenynz(movetime); break;

          case DIR_PXPYPZ: movepxpypz(movetime); break;
          case DIR_NXPYPZ: movenxpypz(movetime); break;
          case DIR_PXNYPZ: movepxnypz(movetime); break;
          case DIR_PXPYNZ: movepxpynz(movetime); break;
          case DIR_NXNYPZ: movenxnypz(movetime); break;
          case DIR_NXPYNZ: movenxpynz(movetime); break;
          case DIR_PXNYNZ: movepxnynz(movetime); break;
          case DIR_NXNYNZ: movenxnynz(movetime); break;
      }
  }

  Direction getoppositedirection(Direction dir) {
      switch (dir) {
          case DIR_PX: return DIR_NX;
          case DIR_NX: return DIR_PX;
          case DIR_PY: return DIR_NY;
          case DIR_NY: return DIR_PY;
          case DIR_PZ: return DIR_NZ;
          case DIR_NZ: return DIR_PZ;

          case DIR_PXPY: return DIR_NXNY;
          case DIR_PXNY: return DIR_NXPY;
          case DIR_NXPY: return DIR_PXNY;
          case DIR_NXNY: return DIR_PXPY;

          case DIR_PXPZ: return DIR_NXNZ;
          case DIR_PXNZ: return DIR_NXPZ;
          case DIR_NXPZ: return DIR_PXNZ;
          case DIR_NXNZ: return DIR_PXPZ;

          case DIR_PYPZ: return DIR_NYNZ;
          case DIR_PYNZ: return DIR_NYPZ;
          case DIR_NYPZ: return DIR_PYNZ;
          case DIR_NYNZ: return DIR_PYPZ;

          case DIR_PXPYPZ: return DIR_NXNYNZ;
          case DIR_NXPYPZ: return DIR_PXNYNZ;
          case DIR_PXNYPZ: return DIR_NXPYNZ;
          case DIR_PXPYNZ: return DIR_NXNYPZ;
          case DIR_NXNYPZ: return DIR_PXPYNZ;
          case DIR_NXPYNZ: return DIR_PXNYPZ;
          case DIR_PXNYNZ: return DIR_NXPYPZ;
          case DIR_NXNYNZ: return DIR_PXPYPZ;
      }
      return dir;
  }

  void updateposition(Direction dir) {
      switch(dir) {
          case DIR_PX:      cur.x += 1; break;
          case DIR_NX:      cur.x -= 1; break;
          case DIR_PY:      cur.y += 1; break;
          case DIR_NY:      cur.y -= 1; break;
          case DIR_PZ:      cur.z += 1; break;
          case DIR_NZ:      cur.z -= 1; break;

          case DIR_PXPY:    cur.x += 1; cur.y += 1; break;
          case DIR_PXNY:    cur.x += 1; cur.y -= 1; break;
          case DIR_NXPY:    cur.x -= 1; cur.y += 1; break;
          case DIR_NXNY:    cur.x -= 1; cur.y -= 1; break;

          case DIR_PXPZ:    cur.x += 1; cur.z += 1; break;
          case DIR_PXNZ:    cur.x += 1; cur.z -= 1; break;
          case DIR_NXPZ:    cur.x -= 1; cur.z += 1; break;
          case DIR_NXNZ:    cur.x -= 1; cur.z -= 1; break;

          case DIR_PYPZ:    cur.y += 1; cur.z += 1; break;
          case DIR_PYNZ:    cur.y += 1; cur.z -= 1; break;
          case DIR_NYPZ:    cur.y -= 1; cur.z += 1; break;
          case DIR_NYNZ:    cur.y -= 1; cur.z -= 1; break;

          case DIR_PXPYPZ:  cur.x += 1; cur.y += 1; cur.z += 1; break;
          case DIR_NXPYPZ:  cur.x -= 1; cur.y += 1; cur.z += 1; break;
          case DIR_PXNYPZ:  cur.x += 1; cur.y -= 1; cur.z += 1; break;
          case DIR_PXPYNZ:  cur.x += 1; cur.y += 1; cur.z -= 1; break;
          case DIR_NXNYPZ:  cur.x -= 1; cur.y -= 1; cur.z += 1; break;
          case DIR_NXPYNZ:  cur.x -= 1; cur.y += 1; cur.z -= 1; break;
          case DIR_PXNYNZ:  cur.x += 1; cur.y -= 1; cur.z -= 1; break;
          case DIR_NXNYNZ:  cur.x -= 1; cur.y -= 1; cur.z -= 1; break;
      }
  }

  float pointtolinedistance(Point P, Point start, Point end) {
      // Vector AB (line direction)
      Point AB = {end.x - start.x, end.y - start.y, end.z - start.z};

      // Vector AP (from start to point P)
      Point AP = {P.x - start.x, P.y - start.y, P.z - start.z};

      // Calculate dot products
      float AB_dot_AB = AB.x * AB.x + AB.y * AB.y + AB.z * AB.z;
      float AB_dot_AP = AB.x * AP.x + AB.y * AP.y + AB.z * AP.z;

      // If line segment has zero length, return distance to start point
      if (AB_dot_AB == 0) {
          return AP.x * AP.x + AP.y * AP.y + AP.z * AP.z;
      }

      // Calculate parameter t for projection (0 <= t <= 1 for line segment)
      float t = AB_dot_AP / AB_dot_AB;

      // Clamp t to [0,1] to ensure we stay on the line segment
      if (t < 0) t = 0;
      if (t > 1) t = 1;

      // Calculate the closest point on the line segment
      Point closest_point = {
          start.x + t * AB.x,
          start.y + t * AB.y,
          start.z + t * AB.z
      };

      storage1 = closest_point;

      // Calculate distance from P to closest point
      float dx = P.x - closest_point.x;
      float dy = P.y - closest_point.y;
      float dz = P.z - closest_point.z;

      return dx * dx + dy * dy + dz * dz;
  }

  Direction findbestdirection(Point cur, Point start, Point end, Direction pastdirection) {
      // Define all neighboring points
      Point px = cur; px.x++;    // +X
      Point nx = cur; nx.x--;    // -X
      Point py = cur; py.y++;    // +Y
      Point ny = cur; ny.y--;    // -Y
      Point pz = cur; pz.z++;    // +Z
      Point nz = cur; nz.z--;    // -Z

      Point pxpy = cur; pxpy.x++; pxpy.y++;    // +X+Y
      Point pxny = cur; pxny.x++; pxny.y--;    // +X-Y
      Point nxpy = cur; nxpy.x--; nxpy.y++;    // -X+Y
      Point nxny = cur; nxny.x--; nxny.y--;    // -X-Y

      Point pxpz = cur; pxpz.x++; pxpz.z++;    // +X+Z
      Point pxnz = cur; pxnz.x++; pxnz.z--;    // +X-Z
      Point nxpz = cur; nxpz.x--; nxpz.z++;    // -X+Z
      Point nxnz = cur; nxnz.x--; nxnz.z--;    // -X-Z

      Point pypz = cur; pypz.y++; pypz.z++;    // +Y+Z
      Point pynz = cur; pynz.y++; pynz.z--;    // +Y-Z
      Point nypz = cur; nypz.y--; nypz.z++;    // -Y+Z
      Point nynz = cur; nynz.y--; nynz.z--;    // -Y-Z

      Point pxpypz = cur; pxpypz.x++; pxpypz.y++; pxpypz.z++;    // +X+Y+Z
      Point nxpypz = cur; nxpypz.x--; nxpypz.y++; nxpypz.z++;    // -X+Y+Z
      Point pxnypz = cur; pxnypz.x++; pxnypz.y--; pxnypz.z++;    // +X-Y+Z
      Point pxpynz = cur; pxpynz.x++; pxpynz.y++; pxpynz.z--;    // +X+Y-Z
      Point nxnypz = cur; nxnypz.x--; nxnypz.y--; nxnypz.z++;    // -X-Y+Z
      Point nxpynz = cur; nxpynz.x--; nxpynz.y++; nxpynz.z--;    // -X+Y-Z
      Point pxnynz = cur; pxnynz.x++; pxnynz.y--; pxnynz.z--;    // +X-Y-Z
      Point nxnynz = cur; nxnynz.x--; nxnynz.y--; nxnynz.z--;    // -X-Y-Z

      Point neighboringpoint[] = {
          px,      // +X
          nx,      // -X
          py,      // +Y
          ny,      // -Y
          pz,      // +Z
          nz,      // -Z

          pxpy,    // +X+Y
          pxny,    // +X-Y
          nxpy,    // -X+Y
          nxny,    // -X-Y

          pxpz,    // +X+Z
          pxnz,    // +X-Z
          nxpz,    // -X+Z
          nxnz,    // -X-Z

          pypz,    // +Y+Z
          pynz,    // +Y-Z
          nypz,    // -Y+Z
          nynz,    // -Y-Z

          pxpypz,  // +X+Y+Z
          nxpypz,  // -X+Y+Z
          pxnypz,  // +X-Y+Z
          pxpynz,  // +X+Y-Z
          nxnypz,  // -X-Y+Z
          nxpynz,  // -X+Y-Z
          pxnynz,  // +X-Y-Z
          nxnynz   // -X-Y-Z
      };

      // Corresponding directions for each neighboring point (matching your enum)
      Direction directions[] = {
          DIR_PX,      // +X
          DIR_NX,      // -X
          DIR_PY,      // +Y
          DIR_NY,      // -Y
          DIR_PZ,      // +Z
          DIR_NZ,      // -Z

          DIR_PXPY,    // +X+Y
          DIR_PXNY,    // +X-Y
          DIR_NXPY,    // -X+Y
          DIR_NXNY,    // -X-Y

          DIR_PXPZ,    // +X+Z
          DIR_PXNZ,    // +X-Z
          DIR_NXPZ,    // -X+Z
          DIR_NXNZ,    // -X-Z

          DIR_PYPZ,    // +Y+Z
          DIR_PYNZ,    // +Y-Z
          DIR_NYPZ,    // -Y+Z
          DIR_NYNZ,    // -Y-Z

          DIR_PXPYPZ,  // +X+Y+Z
          DIR_NXPYPZ,  // -X+Y+Z
          DIR_PXNYPZ,  // +X-Y+Z
          DIR_PXPYNZ,  // +X+Y-Z
          DIR_NXNYPZ,  // -X-Y+Z
          DIR_NXPYNZ,  // -X+Y-Z
          DIR_PXNYNZ,  // +X-Y-Z
          DIR_NXNYNZ   // -X-Y-Z
      };

      // Initialize variables to track the best direction and minimal distance
      Direction bestdirection = DIR_PX; // default direction
      float mindistance = 9999999;

      // Loop through all neighboring points
      for (int i = 0; i < 26; i++) {
          float distance = pointtolinedistance(neighboringpoint[i], start, end);

          // If this point has a smaller distance to the line, update best values
          if (distance < mindistance && getoppositedirection(directions[i]) != pastdirection) {
              mindistance = distance;
              bestdirection = directions[i];
              storage2 = storage1;
          }
      }
      return bestdirection;
  }

  void movetopoint(int x, int y, int z){
  	final.x = x;
  	final.y = y;
  	final.z = z;

  	while(cur.x != final.x || cur.y != final.y || cur.z != final.z){
  		Direction direction = findbestdirection(cur, intermediate, final, pastdirection);
  		moveindirection(direction);
  		updateposition(direction);
  		pastdirection = direction;
  		intermediate = storage2;
  	}
  }

  void drawshape(Point points[], int numPoints) {
      for (int i = 0; i < numPoints; i++) {
          movetopoint(points[i].x, points[i].y, points[i].z);
      }
  }

  //Interesting functions
  void drawVerticalSpring(int centerX, int centerY, int startZ, int endZ,
                          float radius, int coils, int pointsPerCoil) {

      int totalPoints = coils * pointsPerCoil;
      int height = endZ - startZ;
      float pi = 3.1415926f;

      movetopoint(centerX + (int)radius, centerY, startZ);

      for (int i = 0; i <= totalPoints; i++) {
          float t = (float)i / totalPoints;
          float angle = t * coils * 2 * pi;

          int x = centerX + (int)(radius * cosf(angle));
          int y = centerY + (int)(radius * sinf(angle));
          int z = startZ + (int)(t * height);

          movetopoint(x, y, z);
      }

      movetopoint(centerX + (int)radius, centerY, endZ);
  }

  //writing
  void scaleLetterSize(Point points[], int numPoints, int startX, int startY, int baseZ, float size) {
      for (int i = 0; i < numPoints; i++) {
          points[i].x = startX + (int)(points[i].x * size);
          points[i].y = startY + (int)(points[i].y * size);
          points[i].z = baseZ;
      }
  }

  void writeLetter(char letter, int startX, int startY, int baseZ, float size) {
      // Normalized points for each letter (will be scaled)
      Point points[30];
      int pointCount = 0;

      switch (letter) {
          case 'A':
          case 'a': {
              Point baseA[] = {{0,0,0}, {15,40,0}, {30,0,0}, {5,20,0}, {25,20,0}};
              pointCount = 5;
              memcpy(points, baseA, sizeof(baseA));
              break;
          }

          case 'B':
          case 'b': {
              Point baseB[] = {{0,0,0}, {0,40,0}, {25,35,0}, {0,20,0}, {25,15,0}, {0,0,0}};
              pointCount = 6;
              memcpy(points, baseB, sizeof(baseB));
              break;
          }

          case 'C':
          case 'c': {
              Point baseC[] = {{25,5,0}, {5,5,0}, {0,20,0}, {5,35,0}, {25,35,0}};
              pointCount = 5;
              memcpy(points, baseC, sizeof(baseC));
              break;
          }

          case 'D':
          case 'd': {
              Point baseD[] = {{0,0,0}, {0,40,0}, {20,35,0}, {25,20,0}, {20,5,0}, {0,0,0}};
              pointCount = 6;
              memcpy(points, baseD, sizeof(baseD));
              break;
          }

          case 'E':
          case 'e': {
              Point baseE[] = {{25,0,0}, {0,0,0}, {0,40,0}, {25,40,0}, {0,20,0}, {15,20,0}};
              pointCount = 6;
              memcpy(points, baseE, sizeof(baseE));
              break;
          }

          case 'F':
          case 'f': {
              Point baseF[] = {{0,0,0}, {0,40,0}, {25,40,0}, {0,20,0}, {15,20,0}};
              pointCount = 5;
              memcpy(points, baseF, sizeof(baseF));
              break;
          }

          case 'G':
          case 'g': {
              Point baseG[] = {{25,5,0}, {5,5,0}, {0,20,0}, {5,35,0}, {25,35,0}, {25,20,0}, {15,20,0}};
              pointCount = 7;
              memcpy(points, baseG, sizeof(baseG));
              break;
          }

          case 'H':
          case 'h': {
              Point baseH[] = {{0,0,0}, {0,40,0}, {0,20,0}, {25,20,0}, {25,40,0}, {25,0,0}};
              pointCount = 6;
              memcpy(points, baseH, sizeof(baseH));
              break;
          }

          case 'I':
          case 'i': {
              Point baseI[] = {{0,0,0}, {25,0,0}, {12,0,0}, {12,40,0}, {0,40,0}, {25,40,0}};
              pointCount = 6;
              memcpy(points, baseI, sizeof(baseI));
              break;
          }

          case 'J':
          case 'j': {
              Point baseJ[] = {{20,0,0}, {20,35,0}, {15,40,0}, {5,40,0}, {0,35,0}, {0,25,0}};
              pointCount = 6;
              memcpy(points, baseJ, sizeof(baseJ));
              break;
          }

          case 'K':
          case 'k': {
              Point baseK[] = {{0,0,0}, {0,40,0}, {0,20,0}, {25,40,0}, {0,20,0}, {25,0,0}};
              pointCount = 6;
              memcpy(points, baseK, sizeof(baseK));
              break;
          }

          case 'L':
          case 'l': {
              Point baseL[] = {{0,40,0}, {0,0,0}, {25,0,0}};
              pointCount = 3;
              memcpy(points, baseL, sizeof(baseL));
              break;
          }

          case 'M':
          case 'm': {
              Point baseM[] = {{0,0,0}, {0,40,0}, {15,20,0}, {30,40,0}, {30,0,0}};
              pointCount = 5;
              memcpy(points, baseM, sizeof(baseM));
              break;
          }

          case 'N':
          case 'n': {
              Point baseN[] = {{0,0,0}, {0,40,0}, {25,0,0}, {25,40,0}};
              pointCount = 4;
              memcpy(points, baseN, sizeof(baseN));
              break;
          }

          case 'O':
          case 'o': {
              Point baseO[] = {{15,0,0}, {5,10,0}, {5,30,0}, {15,40,0}, {25,30,0}, {25,10,0}, {15,0,0}};
              pointCount = 7;
              memcpy(points, baseO, sizeof(baseO));
              break;
          }

          case 'P':
          case 'p': {
              Point baseP[] = {{0,0,0}, {0,40,0}, {25,40,0}, {25,25,0}, {0,20,0}};
              pointCount = 5;
              memcpy(points, baseP, sizeof(baseP));
              break;
          }

          case 'Q':
          case 'q': {
              Point baseQ[] = {{15,0,0}, {5,10,0}, {5,30,0}, {15,40,0}, {25,30,0}, {25,10,0}, {15,0,0}, {15,15,0}, {30,0,0}};
              pointCount = 9;
              memcpy(points, baseQ, sizeof(baseQ));
              break;
          }

          case 'R':
          case 'r': {
              Point baseR[] = {{0,0,0}, {0,40,0}, {25,40,0}, {25,25,0}, {0,20,0}, {15,20,0}, {25,0,0}};
              pointCount = 7;
              memcpy(points, baseR, sizeof(baseR));
              break;
          }

          case 'S':
          case 's': {
              Point baseS[] = {{25,5,0}, {5,5,0}, {0,15,0}, {5,20,0}, {20,25,0}, {25,35,0}, {5,35,0}};
              pointCount = 7;
              memcpy(points, baseS, sizeof(baseS));
              break;
          }

          case 'T':
          case 't': {
              Point baseT[] = {{0,40,0}, {25,40,0}, {12,40,0}, {12,0,0}};
              pointCount = 4;
              memcpy(points, baseT, sizeof(baseT));
              break;
          }

          case 'U':
          case 'u': {
              Point baseU[] = {{0,40,0}, {0,10,0}, {5,5,0}, {20,5,0}, {25,10,0}, {25,40,0}};
              pointCount = 6;
              memcpy(points, baseU, sizeof(baseU));
              break;
          }

          case 'V':
          case 'v': {
              Point baseV[] = {{0,40,0}, {12,0,0}, {25,40,0}};
              pointCount = 3;
              memcpy(points, baseV, sizeof(baseV));
              break;
          }

          case 'W':
          case 'w': {
              Point baseW[] = {{0,40,0}, {0,0,0}, {15,20,0}, {30,0,0}, {30,40,0}};
              pointCount = 5;
              memcpy(points, baseW, sizeof(baseW));
              break;
          }

          case 'X':
          case 'x': {
              Point baseX[] = {{0,0,0}, {25,40,0}, {12,20,0}, {0,40,0}, {25,0,0}};
              pointCount = 5;
              memcpy(points, baseX, sizeof(baseX));
              break;
          }

          case 'Y':
          case 'y': {
              Point baseY[] = {{0,40,0}, {12,20,0}, {25,40,0}, {12,20,0}, {12,0,0}};
              pointCount = 5;
              memcpy(points, baseY, sizeof(baseY));
              break;
          }

          case 'Z':
          case 'z': {
              Point baseZ[] = {{0,40,0}, {25,40,0}, {0,0,0}, {25,0,0}};
              pointCount = 4;
              memcpy(points, baseZ, sizeof(baseZ));
              break;
          }

          case ' ': {
              // Space - no drawing, just advance position
              Point space[] = {{30,0,0}};
              pointCount = 1;
              memcpy(points, space, sizeof(space));
              break;
          }

          case '.': {
              Point dot[] = {{12,0,0}, {12,5,0}, {12,0,0}};
              pointCount = 3;
              memcpy(points, dot, sizeof(dot));
              break;
          }

          case ',': {
              Point comma[] = {{12,0,0}, {12,5,0}, {8,10,0}};
              pointCount = 3;
              memcpy(points, comma, sizeof(comma));
              break;
          }

          case '!': {
              Point exclamation[] = {{12,40,0}, {12,10,0}, {12,5,0}, {12,0,0}};
              pointCount = 4;
              memcpy(points, exclamation, sizeof(exclamation));
              break;
          }

          case '?': {
              Point question[] = {{5,35,0}, {10,40,0}, {20,40,0}, {25,35,0}, {20,30,0}, {12,30,0}, {12,15,0}, {12,5,0}, {12,0,0}};
              pointCount = 9;
              memcpy(points, question, sizeof(question));
              break;
          }

          default:
              break;
      }

      // Scale and position the points
      scaleLetterSize(points, pointCount, startX, startY, baseZ, size);

      // Draw the letter
      drawshape(points, pointCount);
  }

  void writeWord(const char *word, int startX, int startY, int baseZ, float size, int letterSpacing) {
      int currentX = startX;
      int previousX = startX;
      int scaledSpacing = (int)(letterSpacing * size);

      for (int i = 0; word[i] != '\0'; i++) {
          writeLetter(word[i], currentX, startY, baseZ, size);
          currentX += scaledSpacing;
          if(i != 0){
        	  previousX += scaledSpacing;
          }
          if(word[i+1] != '\0'){
        	  movetopoint((currentX-previousX)/2+previousX, startY, baseZ+100);
          }
      }
  }

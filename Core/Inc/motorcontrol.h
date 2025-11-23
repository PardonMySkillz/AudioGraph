/*
 * motorcontrol.h
 *
 *  Created on: Nov 22, 2025
 *      Author: hcwon
 */

#ifndef INC_MOTORCONTROL_H_
#define INC_MOTORCONTROL_H_

#include "main.h"
#include <string.h>
#include <math.h>

// System configuration
#define SYSCLOCK 84000000
#define PWMFREQUENCY 10000
#define PERIOD ((SYSCLOCK / PWMFREQUENCY) - 1)
#define PULSE 1*168
//0.0000000595s per tick (1/168MHz) 168*0.0000000595 = 0.000001s = 1μs
//CHANGE INIT AFTER GENERATING CODE
// Type definitions
typedef struct {
    float x, y, z;
} Point;

typedef enum {
    DIR_PX, DIR_NX, DIR_PY, DIR_NY, DIR_PZ, DIR_NZ,
    DIR_PXPY, DIR_PXNY, DIR_NXPY, DIR_NXNY,
    DIR_PXPZ, DIR_PXNZ, DIR_NXPZ, DIR_NXNZ,
    DIR_PYPZ, DIR_PYNZ, DIR_NYPZ, DIR_NYNZ,
    DIR_PXPYPZ, DIR_NXPYPZ, DIR_PXNYPZ, DIR_PXPYNZ,
    DIR_NXNYPZ, DIR_NXPYNZ, DIR_PXNYNZ, DIR_NXNYNZ
} Direction;

// Global variables
extern float movetime;
extern Point cur;
extern Point final;
extern Point intermediate;
extern Direction pastdirection;

// 1D Movement functions
void movepx(float time);
void movenx(float time);
void movepy(float time);
void moveny(float time);
void movepz(float time);
void movenz(float time);

// 2D Movement functions
void movepxpy(float time);
void movepxny(float time);
void movenxpy(float time);
void movenxny(float time);
void movepxpz(float time);
void movepxnz(float time);
void movenxpz(float time);
void movenxnz(float time);
void movepypz(float time);
void movepynz(float time);
void movenypz(float time);
void movenynz(float time);

// 3D Movement functions
void movepxpypz(float time);
void movenxpypz(float time);
void movepxnypz(float time);
void movepxpynz(float time);
void movenxnypz(float time);
void movenxpynz(float time);
void movepxnynz(float time);
void movenxnynz(float time);

// Core movement control functions
void moveindirection(Direction direction);
Direction getoppositedirection(Direction dir);
void updateposition(Direction dir);
float pointtolinedistance(Point P, Point start, Point end);
Direction findbestdirection(Point cur, Point start, Point end, Direction pastdirection);
void movetopoint(int x, int y, int z);
void drawshape(Point points[], int numPoints);

// Advanced drawing functions
void drawVerticalSpring(int centerX, int centerY, int startZ, int endZ,
                       float radius, int coils, int pointsPerCoil);
void scaleLetterSize(Point points[], int numPoints, int startX, int startY, int baseZ, float size);
void writeLetter(char letter, int startX, int startY, int baseZ, float size);
void writeWord(const char *word, int startX, int startY, int baseZ, float size, int letterSpacing);

// Initialization function
void motor_control_init(void);



#endif /* INC_MOTORCONTROL_H_ */

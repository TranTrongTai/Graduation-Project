/* Library for point to point tracking robot 2 wheels
 * P2P_Algorithm.h
 * Created on: Jun 27, 2018
 * Reference https://www.youtube.com/watch?v=aE7RQNhwnPQ&t=12s
 * Reference https://www.youtube.com/watch?v=OfpB87pRoUk	
 */
 
#ifndef P2P_Algorithm_h
#define P2P_Algorithm_h

#include "stm32f4xx_hal.h"

float PID_L(float setvalue_l, int32_t pulse_l);		// PID-control for left-wheel
float PID_R(float setvalue_r, int32_t pulse_r);		// PID-control for right-wheel
float PID_theta(float _delta);										// PID-control for theta change
// Creating reference path
void Path_Reference_Update(float array_x[2^32], float array_y[2^32], uint32_t array_length, uint8_t _repeat);
// Position and planning movement update
void Position_Movement_Update(int32_t pulse_l, int32_t pulse_r, float* _omega_l, float* _omega_r);
// Position and planning movement update with external theta
void Position_Movement_Update_With_Extern_Theta(int32_t pulse_l, int32_t pulse_r, float _extern_theta, float* _omega_l, float* _omega_r);
// Trying to combine theta and yaw
void Mix_Theta_Yaw(int32_t pulse_l, int32_t pulse_r, float _extern_theta, float* _omega_l, float* _omega_r);

void Bang_Bang(int32_t pulse_l, int32_t pulse_r, float* _omega_l, float* _omega_r);

void Travelling_Distance(int32_t pulse_l, int32_t pulse_r);
void Calculating_Angular_Rate(void);
void Check_Distance(void);
void Reset_Position(void);
void Force_Stop(void);

#endif

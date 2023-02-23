#include "P2P_Algorithm.h"
#include <math.h>
#include <stdio.h>

//--------------------------------Position parameters---------------
const float PI = 3.14159265359f;
//const float max_pulse_l = 99392;
//const float max_pulse_r = 99605;
const float max_pulse_l = 119104;
const float max_pulse_r = 119453; 
//const float L = 0.44f;
const float L = 0.475f; // new version
const float _deg2rad = 3.14159265359f/180.0f;
float _n = 1.0f;	//  transfer rate motor -> wheel
float P_l = 0.556f;	//perimeter of left wheel
float P_r = 0.554f;	//perimeter of right wheel

// Current positions
float d_l,d_r,v_l,v_r;
float _v, _w, theta, theta_prev, _x, _y, _dis, delta_theta, x_ref, y_ref;
float w_r, w_l, dis_current;
float _path_ref_x[1000]={0};
float _path_ref_y[1000]={0};
uint32_t _cur=0,_point = 1;
uint8_t  x_positive,y_positive;
float currentvalue_r, currentvalue_l;	
uint8_t _Iteration = 0;
// set new velocity for 2 wheels, maximum no load right wheel = 60rpm --- left wheel = 56 rpm
const float _w2n = 60.0f/(2*PI);	
float v_des =  0.35f;		//2*PI*40*R/60; // desired velocity
float w_des; // desired Omega
// point to point offline road
float theta_set;
const float dis_lim = 0.05f;	//edit this parameter to prevent inertia

uint8_t _stop = 1;

//-------------------------------------------------------------------------------------
float PID_L(float setvalue_l, int32_t pulse_l)
{
	 static float Kp_L=2, Kd_L=0.0001, Ki_L=30, Kb_L=0, PWMperiod_L=400, alpha_L=1;
//	static float Kp_L=9, Kd_L=0.01, Ki_L=25, Kb_L=0, PWMperiod_L=400, alpha_L=1;
	 static float error_p_L=0;
	 static float i_p_L=0;
	 static float error_sat_L=0;
	 static float d_f_p_L=0;
	 static float error_L,error_windup_L;
	 static float p_L, d_L,d_f_L, i_L;
	 static float pid_L,pid_sat_L;
	static float dt = 0.01f;

	currentvalue_l = (float)(pulse_l*6000/max_pulse_l);	// currentvalue = (xung/max_pulse)*60*_n/t
	error_L = setvalue_l - currentvalue_l;
	p_L = Kp_L * error_L;
	d_L = Kd_L * (error_L-error_p_L)/dt;
	d_f_L = alpha_L * d_L + (1-alpha_L) * d_f_p_L;
	error_windup_L = Ki_L * error_L + Kb_L * error_sat_L;
	i_L = i_p_L + error_windup_L * dt;
	if (i_L > 400) i_L = 400;
	else if (i_L < 0) i_L = 0;
	i_p_L = i_L;
	error_p_L = error_L;
	d_f_p_L = d_f_L;
	
	pid_L = p_L + d_f_L + i_L;

	//saturation of PIDterm
	if (pid_L>PWMperiod_L)
		pid_sat_L = PWMperiod_L;	
	else if (pid_L<0)
		pid_sat_L = 0;
	else
		pid_sat_L = pid_L;
	error_sat_L = pid_sat_L-pid_L;

	return pid_sat_L;
}

float PID_R(float setvalue_r, int32_t pulse_r)
{
//	static float Kp_R=30, Kd_R=0.001, Ki_R=180, Kb_R=0, PWMperiod_R=400, alpha_R=1;
	//static float Kp_R=4, Kd_R=0.0001, Ki_R=70, Kb_R=0, PWMperiod_R=400, alpha_R=1;|
	static float Kp_R=2, Kd_R=0.0001, Ki_R=30, Kb_R=0, PWMperiod_R=400, alpha_R=1;
//	static float Kp_R=9, Kd_R=0.01, Ki_R=25, Kb_R=0, PWMperiod_R=400, alpha_R=1;
	static float error_p_R=0;
	static float i_p_R=0;
	static float error_sat_R=0;
	static float d_f_p_R=0;
	static float error_R,error_windup_R;
	static float p_R, d_R,d_f_R, i_R;
	static float pid_R,pid_sat_R;
	static float dt = 0.01f;
	
	currentvalue_r = (float)(pulse_r*6000/max_pulse_r);	// currentvalue = (xung/max_pulse)*60*_n/t
	error_R = setvalue_r - currentvalue_r;
	p_R = Kp_R * error_R;
	d_R = Kd_R * (error_R-error_p_R)/dt;
	d_f_R = alpha_R * d_R + (1-alpha_R) * d_f_p_R;
	error_windup_R = Ki_R * error_R + Kb_R * error_sat_R;
	i_R = i_p_R + error_windup_R * dt;
	if (i_R > 400) i_R = 400;
	else if (i_R < 0) i_R = 0;
	
	i_p_R = i_R;
	error_p_R = error_R;
	d_f_p_R = d_f_R;
	pid_R = p_R + d_f_R + i_R;

	//saturation of PID
	if (pid_R>PWMperiod_R)
		pid_sat_R = PWMperiod_R;	
	else if (pid_R<0)
		pid_sat_R = 0;
	else
		pid_sat_R = pid_R;
	error_sat_R = pid_sat_R-pid_R;

	return pid_sat_R;
}

float PID_theta(float _delta)
{
	static float Kp_O=3, Kd_O=0.0, Ki_O=0.0, max_omega=1.5f;
	static float error_p_O=0;
	static float error_O,error_windup_O=1.0f;
	static float p_O, d_O, i_O;
	static float pid_O;
	static float dt = 0.01f;
	
	error_O = _delta*_deg2rad;
	p_O = Kp_O * error_O;
	d_O = Kd_O * (error_O-error_p_O)/dt;
	i_O += Ki_O * error_O * dt;
	if (i_O > error_windup_O)	i_O = error_windup_O;
	else if (i_O < -error_windup_O)	i_O = error_windup_O;
	
	error_p_O = error_O;
	
	pid_O = p_O + d_O + i_O;

	//saturation of PID
	if (pid_O>max_omega)
		pid_O = max_omega;	
	else if (pid_O<-max_omega)
		pid_O = -max_omega;

	return pid_O;
}

void Path_Reference_Update(float array_x[], float array_y[], uint32_t array_length, uint8_t _repeat)
{
	if (array_length != 0)
	{
		for (uint32_t count=0;count<array_length;count++)
		{
			_path_ref_x[count] = array_x[count];
			_path_ref_y[count] = array_y[count];
		}
		_point = array_length;
		_cur = 0;
		_Iteration = _repeat;
		x_ref = _path_ref_x[_cur];
		y_ref = _path_ref_y[_cur];
		_stop = 0;
	}
}

void Position_Movement_Update(int32_t pulse_l, int32_t pulse_r, float* _omega_l, float* _omega_r)
{
	/* Position updates	*/
	Travelling_Distance(pulse_l,pulse_r);
	theta += _w/_deg2rad;
	if (theta > 360.0f)	theta -= 360.0f;
	else if (theta < 0) theta += 360.0f;
	_x += _dis*cos(theta*_deg2rad);
	_y += _dis*sin(theta*_deg2rad);

	/* Movement updates	*/
	//p2p set speed
	Check_Distance();
	Calculating_Angular_Rate();
	
	*_omega_l = w_l;
	*_omega_r = w_r;
}

void Position_Movement_Update_With_Extern_Theta(int32_t pulse_l, int32_t pulse_r, float _extern_theta, float* _omega_l, float* _omega_r)
{
	/* Position updates	*/
	Travelling_Distance(pulse_l,pulse_r);
	theta = _extern_theta;
	_x += _dis*cos(theta*_deg2rad);	//edit theta into yaw !!??
	_y += _dis*sin(theta*_deg2rad);

	/* Movement updates	*/
	//p2p set speed
	Check_Distance();
	Calculating_Angular_Rate();
	
	*_omega_l = w_l;
	*_omega_r = w_r;
}

void Mix_Theta_Yaw(int32_t pulse_l, int32_t pulse_r, float _extern_theta, float* _omega_l, float* _omega_r)
{
	/* Position updates	*/
	Travelling_Distance(pulse_l,pulse_r);
	theta += _w/_deg2rad;
	if (theta > 360.0f)	theta -= 360.0f;
	else if (theta < 0) theta += 360.0f;
	delta_theta = theta - theta_prev;
	if (delta_theta > 180)	delta_theta -= 360;
	else if (delta_theta < -180)	delta_theta += 360;
	if (fabs(delta_theta) > 0.1f)	theta = _extern_theta;
	_x += _dis*cos(theta*_deg2rad);	//edit theta into yaw !!??
	_y += _dis*sin(theta*_deg2rad);
	theta_prev = theta;

	/* Movement updates	*/
	//p2p set speed
	Check_Distance();
	Calculating_Angular_Rate();
	
	*_omega_l = w_l;
	*_omega_r = w_r;
}

void Bang_Bang(int32_t pulse_l, int32_t pulse_r, float* _omega_l, float* _omega_r)
{
	/* Position updates	*/
	Travelling_Distance(pulse_l,pulse_r);
	if (_w != 0)
	{
		theta += _w/_deg2rad;
		if (theta > 360.0f)	theta -= 360.0f;
		else if (theta < 0) theta += 360.0f;
		_x += (_dis/_w)*(sin(theta*_deg2rad)-sin(theta_prev*_deg2rad));	//edit theta into yaw !!??
		_y -= (_dis/_w)*(cos(theta*_deg2rad)-cos(theta_prev*_deg2rad));
		theta_prev = theta;
	}
	else
	{
		_x += _dis*cos(theta*_deg2rad);	//edit theta into yaw !!??
		_y += _dis*sin(theta*_deg2rad);
	}

	/* Movement updates	*/
	//p2p set speed
	Check_Distance();
	Calculating_Angular_Rate();
	
	*_omega_l = w_l;
	*_omega_r = w_r;
}

void Travelling_Distance(int32_t pulse_l, int32_t pulse_r)
{
	// All we need: Pc (current position), Pt (target position), Wc_max, Vc_max.
	d_l = _n * P_l * (pulse_l/max_pulse_l);	//Distance_left = 2pi*R*(Pulse_L/max_pulse)
	d_r = _n * P_r * (pulse_r/max_pulse_r);
	_dis = 0.5f * (d_l + d_r);						//https://www.youtube.com/watch?v=aE7RQNhwnPQ
	_w = (1/L) * (d_r - d_l);						// L = 2*b , distance between 2 wheels
}

void Calculating_Angular_Rate(void)
{
	// Last check before calculating angular rate 
	if (_stop == 1)	v_l = v_r = 0;
	else
	{
		if (dis_current < dis_lim)	v_l = v_r = 0;
		else
		{
			theta_set = atan2(_path_ref_y[_cur]-_y,_path_ref_x[_cur]-_x)/_deg2rad;			//atan2(y,x) (-PI,PI]
			//theta_set = 90.0f; // test PID Theta
			if (theta_set < 0)	theta_set += 360.0f;	//keep theta_set from 0 to 360
		
			//delta_theta = theta_set - theta;	
			delta_theta = theta_set - theta;				//test PID thet
			if (delta_theta < -180)	delta_theta += 360;		//check if rotate CW
			else if (delta_theta > 180)	delta_theta -= 360;		//check if perform rotate CCW
			w_des = PID_theta(delta_theta);

			v_r = v_des + (L/2) * w_des;
			v_l = v_des - (L/2) * w_des;
		}
	}
	// infer new set Omega from v_r and v_l
	w_r = v_r / ((P_r/(2*PI))*_n);
	w_l = v_l / ((P_l/(2*PI))*_n);
	w_r *= _w2n;
	w_l *= _w2n;
	if (w_r < 0) w_r = 0;
	else if (w_r >50) w_r = 50;
	if (w_l < 0) w_l = 0;
	else if (w_l >50) w_l = 50;
}

void Check_Distance(void)
{
	//distance between current point and target point
	dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
	if (dis_current < dis_lim)	//advance the next target
	{
		_cur++;
		if (_cur > (_point-1))
		{
			if ((_Iteration == 1) && (_point > 1))	_cur = 0;
			else 	{_stop = 1;	_cur--;}
		}
		x_ref = _path_ref_x[_cur];
		y_ref = _path_ref_y[_cur];
		dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
	}
	
	//skip point when target point is hard to approach
	if (_cur<(_point-1))
	{
		if (dis_current < 0.5f)
		{
				if (_path_ref_x[_cur+1] != _path_ref_x[_cur])
				{
					if (x_positive == 1)
					{
						if (_x > _path_ref_x[_cur])
						{
							if (_cur == _point - 1)
							{
								if (_Iteration == 0)	_stop = 1;
								else _cur = 0;
							}
							else	_cur++;
							x_ref = _path_ref_x[_cur];
							y_ref = _path_ref_y[_cur];
							dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
						}
					}
					else
					{
						if (_x < _path_ref_x[_cur])
						{
							if (_cur == _point - 1)
							{
								if (_Iteration == 0)	_stop = 1;
								else _cur = 0;
							}
							else	_cur++;
							x_ref = _path_ref_x[_cur];
							y_ref = _path_ref_y[_cur];
							dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
						}
					}
				}
				else 
				{
					if (y_positive == 1)
					{
						if (_y > _path_ref_y[_cur])
						{
							if (_cur == _point - 1)
							{
								if (_Iteration == 0)	_stop = 1;
								else _cur = 0;
							}
							else	_cur++;
							x_ref = _path_ref_x[_cur];
							y_ref = _path_ref_y[_cur];
							dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
						}
					}
					else
					{
						if (_y < _path_ref_y[_cur])
						{
							if (_cur == _point - 1)
							{
								if (_Iteration == 0)	_stop = 1;
								else _cur = 0;
							}
							else	_cur++;
							x_ref = _path_ref_x[_cur];
							y_ref = _path_ref_y[_cur];
							dis_current = sqrt((_path_ref_y[_cur]-_y)*(_path_ref_y[_cur]-_y) + (_path_ref_x[_cur]-_x)*(_path_ref_x[_cur]-_x));
						}
					}
				}
		}
	}

	if (_x <= _path_ref_x[_cur])	x_positive = 1;
	else x_positive = 0;
	if (_y <= _path_ref_y[_cur])	y_positive = 1;
	else y_positive = 0;
}

void Reset_Position(void)
{
	_x = _y = theta =0;
	_stop = 1;
}

void Force_Stop(void)
{
	_stop = 1;
}


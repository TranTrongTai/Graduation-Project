#include "getChar.h"
#include <string.h>
#include <stdio.h>
double h[20];
double h1[20];
//char a[11] = { '1','0','.','8','3','8','9','3','2' }; lat2 return form UAR 
//char b[11] = { '1','0','6','.','6','7','1','3','7','8' }; long2 return form UART

double _getLat2(char a[])
{	
	//double h[9];
	double _lat2=0.6f; // lat
	
	int len;
	int i;
	len = strlen(a);
	for (i = 0; i <= len; i++) {
		h[i] = (a[i] * 1 - 48);
	}
	//convert _lat2 ->double
	_lat2 = h[0] * 10 + h[1] + h[3] * (0.1) + h[4] * (0.01) + h[5] * (0.001) + h[6] * (0.0001) + h[7] * (0.00001) + h[8] * (0.000001);
	return _lat2;
}
//double _getLong2(char b[])
//{	
//	//double h1[10];
//	double _long2; // long
//	int len_;
//	int i_;
//	len_ = strlen(b);
//	printf("\r\nChuoi dai %d", len_);
//	for (i_ = 0; i_ <= len_; i_++) {
//		h1[i_] = (b[i_] * 1 - 48);
//	}
//	//convert _long2 ->double
//	_long2 = h1[0] * 100 + h1[1]*10 + h1[2] + h1[4] * (0.1) + h1[5] * (0.01) + h1[6] * (0.001) + h1[7] * (0.0001) + h1[8] * (0.00001) + h1[9] * (0.000001);
//	return _long2;
//}

// receive data_recive
double _getLong2(char a[])
{
	//double h1[10];
	double _long2; // long
	int len_;
	int i_;
	len_ = strlen(a);
	for (i_ = 0; i_ <= len_; i_++) {
		h1[i_] = (a[i_] * 1 - 48);
	}
	//convert _long2 ->double
	_long2 = h1[10] * 100 + h1[11] * 10 + h1[12] + h1[14] * (0.1) + h1[15] * (0.01) + h1[16] * (0.001) + h1[17] * (0.0001) + h1[18] * (0.00001) + h1[19] * (0.000001);
	return _long2;
}


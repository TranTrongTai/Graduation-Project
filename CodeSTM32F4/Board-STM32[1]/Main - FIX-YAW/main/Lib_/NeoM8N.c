#include "NeoM8N.h"	
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"


#define	_GPS_DEBUG					0
/* Earth radius */
#define GPS_EARTH_RADIUS		6371

/* Degrees to radians converter */
#define GPS_DEGREES2RADIANS(x)	((x) * (float)0.01745329251994)
/* Radians to degrees */
#define GPS_RADIANS2DEGREES(x)	((x) * (float)57.29577951308232)
//#define M_PI 3.14159
//#define radians(angleDegrees) (angleDegrees * M_PI / 180.0)
// 
//// Converts radians to degrees.
//#define degrees(angleRadians) (angleRadians * 180.0 / M_PI)

//#define TWO_PI 6.283185307179586476925286766559
//#define sq(x) ((x)*(x))
GPS_t GPS;
//move_to Move;
double convertDegMinToDecDeg (float degMin)
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}
//##################################################################################################################
void	GPS_Init(UART_HandleTypeDef* huart)
{	
	UART_HandleTypeDef* Handle = huart;
	GPS.rxIndex=0;
	HAL_UART_Receive_IT(Handle,&GPS.rxTmp,1);	
}
//##################################################################################################################
void	GPS_CallBack(UART_HandleTypeDef* huart)
{
	UART_HandleTypeDef* Handle = huart;
	GPS.LastTime=HAL_GetTick();
	if(GPS.rxIndex < sizeof(GPS.rxBuffer)-2)
	{
		GPS.rxBuffer[GPS.rxIndex] = GPS.rxTmp;
		GPS.rxIndex++;
	}	
	HAL_UART_Receive_IT(Handle,&GPS.rxTmp,1);
}
//##################################################################################################################
void	GPS_Process(UART_HandleTypeDef* huart)
{
	UART_HandleTypeDef* Handle = huart;
	if( (HAL_GetTick()-GPS.LastTime>50) && (GPS.rxIndex>0))
	{
		char	*str;
		#if (_GPS_DEBUG==1)
		printf("%s",GPS.rxBuffer);
		#endif
		str=strstr((char*)GPS.rxBuffer,"$GNGGA,");
		if(str!=NULL)
		{
			memset(&GPS.GNGGA,0,sizeof(GPS.GNGGA));
			sscanf(str,"$GNGGA,%2hhd%2hhd%2hhd.%3hd,%f,%c,%f,%c,%hhd,%hhd,%f,%f,%c,%hd,%s,*%2s\r\n",
			&GPS.GNGGA.UTC_Hour,&GPS.GNGGA.UTC_Min,&GPS.GNGGA.UTC_Sec,&GPS.GNGGA.UTC_MicroSec,&GPS.GNGGA.Latitude,&GPS.GNGGA.NS_Indicator,&GPS.GNGGA.Longitude,&GPS.GNGGA.EW_Indicator,&GPS.GNGGA.PositionFixIndicator,&GPS.GNGGA.SatellitesUsed,&GPS.GNGGA.HDOP,
			&GPS.GNGGA.MSL_Altitude,&GPS.GNGGA.MSL_Units,&GPS.GNGGA.AgeofDiffCorr,GPS.GNGGA.DiffRefStationID,GPS.GNGGA.CheckSum);
			
			if(GPS.GNGGA.NS_Indicator==0)
				GPS.GNGGA.NS_Indicator='-';
			if(GPS.GNGGA.EW_Indicator==0)
				GPS.GNGGA.EW_Indicator='-';
			if(GPS.GNGGA.Geoid_Units==0)
				GPS.GNGGA.Geoid_Units='-';
			if(GPS.GNGGA.MSL_Units==0)
				GPS.GNGGA.MSL_Units='-';
			GPS.GNGGA.LatitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Latitude);
			GPS.GNGGA.LongitudeDecimal=convertDegMinToDecDeg(GPS.GNGGA.Longitude);			
		}		
		memset(GPS.rxBuffer,0,sizeof(GPS.rxBuffer));
		GPS.rxIndex=0;
	}
	HAL_UART_Receive_IT(Handle,&GPS.rxTmp,1);
}

//##################################################################################################################
double bearing_Angle(double lat_1, double long_1, double lat_2, double long_2)
{
	static float f1_, f2_, l1_, l2_, df_, dfi_;
	static double bearing_rad;
	static double bearing_;
	//convert radian -> degree
	f1_ = GPS_DEGREES2RADIANS(lat_1);
	f2_ = GPS_DEGREES2RADIANS(lat_2);
	l1_ = GPS_DEGREES2RADIANS(long_1);
	l2_ = GPS_DEGREES2RADIANS(long_2);
	
	
	df_ = sin(l2_ - l1_) * cos(f2_);
	dfi_ = cos(f1_) * sin(f2_) - sin(f1_) * cos(f2_) * cos(l2_ - l1_);
	
	//caculate bearing radian
	bearing_rad = atan2(df_, dfi_);
	
	// degree
	//double bearing_;
	bearing_ = 	GPS_RADIANS2DEGREES(bearing_rad);
	
	/* Make bearing always positive from 0 - 360 degrees instead of -180 to 180 */
	if (bearing_ < 0) {
		bearing_ += 360.0 ;
	}
	
	return bearing_;
		
}
//##################################################################################################################
double distanceBetween(double lat1, double long1, double lat2, double long2) // chay dc 
{
	static float f1, f2, df, dfi, a;
	static double kc;
	static double c;
	f1 = GPS_DEGREES2RADIANS(lat1);
	f2 = GPS_DEGREES2RADIANS(lat2);
	df = GPS_DEGREES2RADIANS(lat2 - lat1);
	dfi = GPS_DEGREES2RADIANS(long2 - long1);
	a = sin(df * (float)0.5) * sin(df * (float)0.5) + cos(f1) * cos(f2) * sin(dfi * (float)0.5) * sin(dfi * (float)0.5);
	/* Get distance in meters */
	c = 2 * atan2(sqrt(a), sqrt(1 - a));
	kc = GPS_EARTH_RADIUS * c  * 1000;
	return kc;
}








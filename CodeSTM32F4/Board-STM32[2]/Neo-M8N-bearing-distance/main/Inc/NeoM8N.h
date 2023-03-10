#ifndef _NeoM8N_H_
#define _NeoM8N_H_
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "stdint.h"



typedef struct
{
	uint8_t			UTC_Hour;
	uint8_t			UTC_Min;
	uint8_t			UTC_Sec;
	uint16_t		UTC_MicroSec;
	
	float				Latitude;
	double			LatitudeDecimal;
	char				NS_Indicator;
	float				Longitude;
	double			LongitudeDecimal;
	char				EW_Indicator;
	
	uint8_t			PositionFixIndicator;
	uint8_t			SatellitesUsed;
	float				HDOP;
	float				MSL_Altitude;
	char				MSL_Units;
	float				Geoid_Separation;
	char				Geoid_Units;
	
	uint16_t		AgeofDiffCorr;
	char				DiffRefStationID[4];
	char				CheckSum[2];	
	
}GNGGA_t;

typedef struct 
{
	uint8_t		rxBuffer[512];
	uint16_t	rxIndex;
	uint8_t		rxTmp;	
	uint32_t	LastTime;	
	GNGGA_t		GNGGA;
	
}GPS_t;

extern GPS_t GPS;

//##################################################################################################################
void	GPS_Init(UART_HandleTypeDef* UartX);
void	GPS_CallBack(UART_HandleTypeDef* UartX);
void	GPS_Process(UART_HandleTypeDef* UartX);
double bearing_Angle(double lat_1, double long_1, double lat_2, double long_2);
double distanceBetween(double lat1, double long1, double lat2, double long2); // chay dc
double filter_low(float bearing);
//##############################

#endif



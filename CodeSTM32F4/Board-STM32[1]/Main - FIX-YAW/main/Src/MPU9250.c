#include "MPU9250.h"
#include "MadgwickAHRS.h"
#include <math.h>
#include <stdio.h>

int16_t temp;
uint8_t data[21];
uint8_t data_mag[8];
uint8_t d[2];
uint8_t reg = ACCEL_OUT;
uint8_t reg_mag = EXT_SENS_DATA_00;
int16_t gyro_x, gyro_y, gyro_z;
int16_t mag_x, mag_y, mag_z;
int16_t acc_x, acc_y, acc_z;
float ax, ay, az, gx, gy, gz, mx, my, mz;
float _magScaleX, _magScaleY, _magScaleZ;
float gyro_scale,	acc_scale;
float gyro_x_cal, gyro_y_cal, gyro_z_cal;
float acc_x_cal, acc_y_cal, acc_z_cal;
float _yaw, _pitch, _roll;
//--------------------------------Calib------------------
float magbias[3], magscale[3];
const float G = 9.807f;
const float _d2r = 3.14159265359f/180.0f;
 // magnetometer bias and scale factor estimation
    uint16_t _maxCounts = 10000;
    float _deltaThresh = 0.3f;
    uint8_t _coeff = 8;
    uint16_t _counter;
    float _framedelta, _delta;
    float _hxfilt, _hyfilt, _hzfilt;
    float _hxmax, _hymax, _hzmax;
    float _hxmin, _hymin, _hzmin;
    float _hxb, _hyb, _hzb;
    float _hxs = 1.0f;
    float _hys = 1.0f;
    float _hzs = 1.0f;
    float _avgs;
// acc
		float _axmax, _aymax, _azmax;
    float _axmin, _aymin, _azmin;
    float _axb, _ayb, _azb;
    float _axs = 1.0f;
    float _ays = 1.0f;
    float _azs = 1.0f;
//--------------------------------------------------------------------------------------
void MPU9250_Read_All(I2C_HandleTypeDef* I2Cx)
{
	I2C_HandleTypeDef* Handle = I2Cx;
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, &reg, 1, 1000);
	HAL_I2C_Master_Receive(Handle, (uint16_t)MPU6050_I2C_ADDR, data, 21, 30000);

	/* Format accelerometer data */
	acc_x = (int16_t)(data[0] << 8 | data[1]);
	acc_y = (int16_t)(data[2] << 8 | data[3]);
	acc_z = (int16_t)(data[4] << 8 | data[5]);
//	/* Format temperature */
//	temp = (data[6] << 8 | data[7]);
//	DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);
	/* Format gyroscope data */
	gyro_x = (int16_t)(data[8] << 8 | data[9]);
	gyro_y = (int16_t)(data[10] << 8 | data[11]);
	gyro_z = (int16_t)(data[12] << 8 | data[13]);
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
		/* Format magnetometer data*/
	mag_x = (int16_t)(data[15] << 8 | data[14]);
	mag_y = (int16_t)(data[17] << 8 | data[16]);
	mag_z = (int16_t)(data[19] << 8 | data[18]);
}
void MPU9250_Init_All(I2C_HandleTypeDef* I2Cx)
{
	I2C_HandleTypeDef* Handle = I2Cx;
	//MPU6050
	//reset IMU
	d[0]=PWR_MGMNT_1;
	d[1]=PWR_RESET;
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	HAL_Delay(100);
	// reset the AK8963
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL2,AK8963_RESET);	
	HAL_Delay(100);
	//start IMU, disable sleep mode
	d[0]=PWR_MGMNT_1;
	d[1]=SEN_ENABLE;
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	HAL_Delay(100);
	// select clock source to gyro
	d[0]=PWR_MGMNT_1;
	d[1]=CLOCK_SEL_PLL;
  HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, d, 2, 1000);	
	HAL_Delay(200);
	//set gyro +/- ???deg
	d[0]=GYRO_CONFIG;
	d[1]=GYRO_FS_SEL_2000DPS;	//0x00->250 0x08->500	0x10->1000 0x18->2000
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	gyro_scale = 2000.0f/32768.0f;
	//set accelerate +/-??g
	d[0]=ACCEL_CONFIG;
	d[1]=ACCEL_FS_SEL_16G;	//0x00->2 0x08->4	0x10->8 0x18->16
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	acc_scale = 16.0f/32768.0f;	
	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	d[0]=SMPDIV;
	d[1]=0x00;
  HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	

	// enable I2C master mode
	d[0]=USER_CTRL;
	d[1]=I2C_MST_EN;
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	// set the I2C bus speed to 400 kHz
	d[0]=I2C_MST_CTRL;
	d[1]=I2C_MST_CLK;
	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000);	
	
	//AK8963
	// reset the AK8963
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL2,AK8963_RESET);	
	HAL_Delay(100);
	//check who am I
//	uint8_t check[1];
//	HAL_I2C_Master_Transmit(&hi2c1,mag,&mag_who_am_I,1,1000);
//	HAL_I2C_Master_Receive(&hi2c1,mag,check, 1, 1000);
//	flag=check[0];	//expect 0x48 (72)
	//MPU9250_Read_Mag(0x00,1);
	// set AK8963 to Power Down
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_PWR_DOWN);	
	HAL_Delay(100);
	// set AK8963 to FUSE ROM access
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_FUSE_ROM);	
	HAL_Delay(100);
	// read the AK8963 ASA registers and compute magnetometer scale factors
	MPU9250_Read_Mag(I2Cx,AK8963_ASA,3);
		_magScaleX = ((((float)data_mag[0]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss)
		_magScaleY = ((((float)data_mag[1]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss)
		_magScaleZ = ((((float)data_mag[2]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss) 
//	if (_magScaleX > 0.1f)
//	{
//		_hxmin = -74.08055f;
//		_hxmax = -3.4270713f;
//		_hymin = -4.2218237f;
//		_hymax = 23.675205f;
//		_hzmin = -0.8420639f;
//		_hzmax = 27.295069;
//		// find the magnetometer bias
//  _hxb = (_hxmax + _hxmin) / 2.0f;
//  _hyb = (_hymax + _hymin) / 2.0f;
//  _hzb = (_hzmax + _hzmin) / 2.0f;

//  // find the magnetometer scale factor
//  _hxs = (_hxmax - _hxmin) / 2.0f;
//  _hys = (_hymax - _hymin) / 2.0f;
//  _hzs = (_hzmax - _hzmin) / 2.0f;
//  _avgs = (_hxs + _hys + _hzs) / 3.0f;
//  _hxs = _avgs/_hxs;
//  _hys = _avgs/_hys;
//  _hzs = _avgs/_hzs;
//	}
//	else
//	{
//	_hxb = -19.6775093f;
//  _hyb = 8.96677208f;
//  _hzb = 16.2785778f;
//  _hxs = 0.942259133f;
//  _hys = 1.04611719f;
//  _hzs = 1.01749563f;
//	}
//	_magScaleX = 0.08375496;
//	_hxb = -16.175049;
//  _hyb = 9.100713;
//  _hzb = 14.568213;
//  _hxs = 0.9223463;
//  _hys = 1.0754929;
//  _hzs = 1.0141965;
//	_hxb = -17.762774;
//  _hyb = 8.916008;
//  _hzb = 10.197252;
//  _hxs = 0.9352541;
//  _hys = 1.0372257;
//  _hzs = 1.0344883;
//	_hxb = -17.54894;
//  _hyb = 9.387283;
//  _hzb = 8.968702;
//  _hxs = 0.9437446;
//  _hys = 1.0392821;
//  _hzs = 1.0222976;
	// set AK8963 to Power Down
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_PWR_DOWN);	
	HAL_Delay(100);
	// set AK8963 to 16 bit resolution, 100 Hz update rate
	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_CNT_MEAS2|6);	
	HAL_Delay(100);
	// select clock source to gyro
	d[0]=PWR_MGMNT_1;
	d[1]=CLOCK_SEL_PLL;
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, d, 2, 1000) );	
	HAL_Delay(200);
	// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
	MPU9250_Read_Mag(I2Cx,AK8963_HXL,7);
	HAL_Delay(10);
//	HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, &reg, 1, 1000);
//	HAL_I2C_Master_Receive_DMA(Handle, (uint16_t)MPU6050_I2C_ADDR, data, 21);


//	//ver tai
//	I2C_HandleTypeDef* Handle = I2Cx;
//	// select clock source to gyro
//	d[0] = PWR_MGMNT_1;
//	d[1] = CLOCK_SEL_PLL  ;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR, (uint8_t *)d,2,1000);
//	// enable I2C master mode
//	d[0] = USER_CTRL;
//	d[1] = I2C_MST_EN;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR, (uint8_t *)d,2,1000) ;
//	// set the I2C bus speed to 400 kHz
//	d[0] = I2C_MST_CTRL;
//	d[1] = I2C_MST_CLK;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	// set AK8963 to Power Down
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_PWR_DOWN);
//	MPU9250_Write_Mag(I2Cx,PWR_MGMNT_1,PWR_RESET);
//	// reset the MPU9250
//	d[0] = PWR_MGMNT_1;
//	d[1] = PWR_RESET;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	// wait for MPU-9250 to come back up
//	HAL_Delay(100); 
//	// reset the AK8963
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL2,AK8963_RESET);
//	// select clock source to gyro
//	d[0] = PWR_MGMNT_1;
//	d[1] = CLOCK_SEL_PLL;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000) ;
//	// don't check WHO AM I byte
//	// enable accelerometer and gyro
//	d[0] = PWR_MGMNT_2;
//	d[1] = SEN_ENABLE;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	// setting accel range to 16G as default
//	d[0] = ACCEL_CONFIG;
//	d[1] = ACCEL_FS_SEL_16G; //0x00->2 0x08->4	0x10->8 0x18->16
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	acc_scale = 16.0f/32767.5f;
//	// setting the gyro range to 2000DPS as default
//	d[0] = GYRO_CONFIG;
//	d[1] = GYRO_FS_SEL_2000DPS; //0x00->250 0x08->500	0x10->1000 0x18->2000
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	gyro_scale = 2000.0f/32768.0f;
//	
//	 // enable I2C master mode
//	d[0] = USER_CTRL;
//	d[1] = I2C_MST_EN;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR, (uint8_t *)d,2,1000);
//	
//	// set the I2C bus speed to 400 kHz
//	d[0] = I2C_MST_CTRL;
//	d[1] = I2C_MST_CLK;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	//don't check whoAmI
//	//if( whoAmIAK8963() != 72 ){ } 0x48
//	/* get the magnetometer calibration */
//	// set AK8963 to Power Down
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_PWR_DOWN);
//	HAL_Delay(100);
//	// set AK8963 to FUSE ROM access
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_FUSE_ROM);
//	HAL_Delay(100);
//	 // read the AK8963 ASA registers and compute magnetometer scale factors
//	MPU9250_Read_Mag(I2Cx,AK8963_ASA,3);
//	_magScaleX = ((((float)data_mag[0]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss)
//	_magScaleY = ((((float)data_mag[1]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss)
//	_magScaleZ = ((((float)data_mag[2]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // uT (micro Gauss) 
//	// set AK8963 to Power Down
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_PWR_DOWN);
//	HAL_Delay(100);
//	// set AK8963 to 16 bit resolution, 100 Hz update rate
//	MPU9250_Write_Mag(I2Cx,AK8963_CNTL1,AK8963_CNT_MEAS2|6);
//	HAL_Delay(100);// long wait between AK8963 mode changes
//	// select clock source to gyro
//	d[0] = PWR_MGMNT_1;
//	d[1] = CLOCK_SEL_PLL;
//	HAL_I2C_Master_Transmit(Handle,(uint16_t) MPU6050_I2C_ADDR,(uint8_t *)d,2,1000);
//	// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
//	MPU9250_Read_Mag(I2Cx,AK8963_HXL,7);
//	HAL_Delay(10);
}

/* estimates the gyro biases */
void MPU9250_Calib_Gyro(I2C_HandleTypeDef* I2Cx)
{
	gyro_x_cal = 0;                                           
	gyro_y_cal = 0;                                            
	gyro_z_cal = 0;  
	uint16_t sample_total = 500;
	for(uint16_t count=0;count<sample_total;count++)
	{
		MPU9250_Read_All(I2Cx);
		gyro_x_cal += (float)gyro_x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
		gyro_y_cal += (float)gyro_y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
		gyro_z_cal += (float)gyro_z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
		HAL_Delay(4);                                                          //Delay 3ms to simulate the 250Hz program loop		
	}	
	gyro_x_cal /= (float)sample_total;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= (float)sample_total;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= (float)sample_total;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset 
}

/* finds bias and scale factor calibration for the accelerometer,
this should be run for each axis in each direction (6 total) to find
the min and max values along each */
void MPU9250_Calib_Acc(I2C_HandleTypeDef* I2Cx)
{
	acc_x_cal = 0;
	acc_y_cal = 0;
	acc_z_cal = 0;
	uint16_t sample_total = 500;
	for(uint16_t count=0;count<sample_total;count++)
	{
		MPU9250_Read_All(I2Cx);
		acc_x_cal += (float)acc_x;
		acc_y_cal += (float)acc_y;
		acc_z_cal += (float)acc_z;
		HAL_Delay(4);                                                          //Delay 3ms to simulate the 250Hz program loop		
	}	
	acc_x_cal *= G*acc_scale/(float)sample_total;
	acc_y_cal *= G*acc_scale/(float)sample_total;
	acc_z_cal *= G*acc_scale/(float)sample_total;

	if (acc_x_cal > 9.0f) {
    _axmax = (float)acc_x_cal;
  }
  if (acc_y_cal > 9.0f) {
    _aymax = (float)acc_y_cal;
  }
  if (acc_z_cal > 9.0f) {
    _azmax = (float)acc_z_cal;
  }
  if (acc_x_cal < -9.0f) {
    _axmin = (float)acc_x_cal;
  }
  if (acc_y_cal < -9.0f) {
    _aymin = (float)acc_y_cal;
  }
  if (acc_z_cal < -9.0f) {
    _azmin = (float)acc_z_cal;
  }
	// find bias and scale factor
  if ((fabs(_axmin) > 9.0f) && (fabs(_axmax) > 9.0f)) {
    _axb = (_axmin + _axmax) / 2.0f;
    _axs = G/((fabs(_axmin) + fabs(_axmax)) / 2.0f);
  }
  if ((fabs(_aymin) > 9.0f) && (fabs(_aymax) > 9.0f)) {
    _ayb = (_axmin + _axmax) / 2.0f;
    _ays = G/((fabs(_aymin) + fabs(_aymax)) / 2.0f);
  }
  if ((fabs(_azmin) > 9.0f) && (fabs(_azmax) > 9.0f)) {
    _azb = (_azmin + _azmax) / 2.0f;
    _azs = G/((fabs(_azmin) + fabs(_azmax)) / 2.0f);
  }
}

/* finds bias and scale factor calibration for the magnetometer,
the sensor should be rotated in a figure 8 motion until complete */
void MPU9250_Calib_Mag(I2C_HandleTypeDef* I2Cx)
{
	// get a starting set of data
  MPU9250_Read_All(I2Cx);
  _hxmax = (float)mag_x*_magScaleX;
  _hxmin = (float)mag_x*_magScaleX;
  _hymax = (float)mag_y*_magScaleY;
  _hymin = (float)mag_y*_magScaleY;
  _hzmax = (float)mag_z*_magScaleZ;
  _hzmin = (float)mag_z*_magScaleZ;

  // collect data to find max / min in each channel
  _counter = 0;
  while (_counter < _maxCounts) {
//	while (1) {
    _delta = 0.0f;
    _framedelta = 0.0f;
    MPU9250_Read_All(I2Cx);
    _hxfilt = (_hxfilt*((float)_coeff-1)+(float)mag_x*_magScaleX)/((float)_coeff);
    _hyfilt = (_hyfilt*((float)_coeff-1)+(float)mag_y*_magScaleY)/((float)_coeff);
    _hzfilt = (_hzfilt*((float)_coeff-1)+(float)mag_z*_magScaleZ)/((float)_coeff);
    if (_hxfilt > _hxmax) {
      _delta = _hxfilt - _hxmax;
      _hxmax = _hxfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_hyfilt > _hymax) {
      _delta = _hyfilt - _hymax;
      _hymax = _hyfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_hzfilt > _hzmax) {
      _delta = _hzfilt - _hzmax;
      _hzmax = _hzfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_hxfilt < _hxmin) {
      _delta = fabs(_hxfilt - _hxmin);
      _hxmin = _hxfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_hyfilt < _hymin) {
      _delta = fabs(_hyfilt - _hymin);
      _hymin = _hyfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_hzfilt < _hzmin) {
      _delta = fabs(_hzfilt - _hzmin);
      _hzmin = _hzfilt;
    }
    if (_delta > _framedelta) {
      _framedelta = _delta;
    }
    if (_framedelta > _deltaThresh) {
      _counter = 0;
    } else {
      _counter++;
    }
    HAL_Delay(4);
  }

  // find the magnetometer bias
  _hxb = (_hxmax + _hxmin) / 2.0f;
  _hyb = (_hymax + _hymin) / 2.0f;
  _hzb = (_hzmax + _hzmin) / 2.0f;

  // find the magnetometer scale factor
  _hxs = (_hxmax - _hxmin) / 2.0f;
  _hys = (_hymax - _hymin) / 2.0f;
  _hzs = (_hzmax - _hzmin) / 2.0f;
  _avgs = (_hxs + _hys + _hzs) / 3.0f;
  _hxs = _avgs/_hxs;
  _hys = _avgs/_hys;
  _hzs = _avgs/_hzs;
}
void MPU9250_Write_Mag(I2C_HandleTypeDef* I2Cx,uint8_t subaddress,uint8_t _data)
{
	I2C_HandleTypeDef* Handle = I2Cx;
	// set which slave to write
	d[0]=I2C_SLV0_ADDR;
	d[1]=AK8963_I2C_ADDR;
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// subaddress, where to begin
	d[0]=I2C_SLV0_REG;
	d[1]=subaddress;	//subaddress
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	//store the data for write
	d[0]=I2C_SLV0_DO;
	d[1]=_data;
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// enable I2C and send 1 byte
	d[0]=I2C_SLV0_CTRL;
	d[1]=I2C_SLV0_EN|1;	//data
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// read the register and confirm
	MPU9250_Read_Mag(I2Cx,subaddress,1);
}
void MPU9250_Read_Mag(I2C_HandleTypeDef* I2Cx,uint8_t subaddress,uint8_t Size)
{
	I2C_HandleTypeDef* Handle = I2Cx;
	// set which slave to read
	d[0]=I2C_SLV0_ADDR;
	d[1]=AK8963_I2C_ADDR|0x80;	//mag address and read mode
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// subaddress, where to begin
	d[0]=I2C_SLV0_REG;
	d[1]=subaddress;	//subaddress
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// enable I2C and request the bytes
	d[0]=I2C_SLV0_CTRL;
	d[1]=I2C_SLV0_EN|Size;	//data
  (HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, (uint8_t *)d, 2, 1000) );	
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
	(HAL_I2C_Master_Transmit(Handle, (uint16_t)MPU6050_I2C_ADDR, &reg_mag, 1, 1000) );	
	(HAL_I2C_Master_Receive(Handle, (uint16_t)MPU6050_I2C_ADDR, data_mag, Size, 1000) );
}
void MPU9250_Angles_Update(I2C_HandleTypeDef* I2Cx,MY_MPU9250* DataStruct)
{
	MPU9250_Read_All(I2Cx);
	// transform and convert to float values
	ax = ((float)acc_x*G*acc_scale - _axb)*_axs;
	ay = ((float)acc_y*G*acc_scale - _ayb)*_ays;
	az = ((float)acc_z*G*acc_scale - _azb)*_azs;
	/* Format gyroscope data */
	gx = (float)(gyro_x - gyro_x_cal)*gyro_scale;
	gy = (float)(gyro_y - gyro_y_cal)*gyro_scale;
	gz = (float)(gyro_z - gyro_z_cal)*gyro_scale;
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
		/* Format magnetometer data*/
	mx = ((float)mag_x*_magScaleX - _hxb)*_hxs;
	my = ((float)mag_y*_magScaleY - _hyb)*_hys;
	mz = ((float)mag_z*_magScaleZ - _hzb)*_hzs;
	MadgwickAHRSupdate(gx*_d2r,gy*_d2r,gz*_d2r,ax,ay,az,my,mx,-mz);
//	MadgwickAHRSupdateIMU(gx*_d2r,gy*_d2r,gz*_d2r,ax,ay,az);
	_yaw   = atan2f(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);   
	_pitch = -asinf(2.0f * (q1 * q3 - q0 * q2));
	_roll  = atan2f(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);
	//_yaw = (_yaw / _d2r ) - 9.183f;
	//DataStruct->Angle_Yaw = _yaw;
	DataStruct->Angle_Yaw = _yaw / _d2r;
	DataStruct->Angle_Pitch = _pitch/_d2r;
	DataStruct->Angle_Roll = _roll/_d2r;
}
void MPU9250_Get_Angles(I2C_HandleTypeDef* I2Cx, float* yaw, float* pitch, float* roll)
{
	MPU9250_Read_All(I2Cx);
	// transform and convert to float values
	ax = ((float)acc_x*G*acc_scale - _axb)*_axs;
	ay = ((float)acc_y*G*acc_scale - _ayb)*_ays;
	az = ((float)acc_z*G*acc_scale - _azb)*_azs;
	/* Format gyroscope data */
	gx = (float)(gyro_x - gyro_x_cal)*gyro_scale;
	gy = (float)(gyro_y - gyro_y_cal)*gyro_scale;
	gz = (float)(gyro_z - gyro_z_cal)*gyro_scale;
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
		/* Format magnetometer data*/
	mx = ((float)mag_x*_magScaleX - _hxb)*_hxs;
	my = ((float)mag_y*_magScaleY - _hyb)*_hys;
	mz = ((float)mag_z*_magScaleZ - _hzb)*_hzs;
	MadgwickAHRSupdate(gx*_d2r,gy*_d2r,gz*_d2r,ax,ay,az,my,mx,-mz);
	//MadgwickAHRSupdateIMU(gx*_d2r,gy*_d2r,gz*_d2r,ax,ay,az);
	_yaw   = atan2f(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);   
	_pitch = -asinf(2.0f * (q1 * q3 - q0 * q2));
	_roll  = atan2f(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);
	//_yaw = (_yaw / _d2r ) - 9.183f;
	*pitch = _pitch / _d2r;
	//*yaw = _yaw ;
	*yaw =_yaw / _d2r;
	*roll =_roll  / _d2r;
}

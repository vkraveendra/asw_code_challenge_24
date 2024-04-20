#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>

typedef enum {
    STATUS_OK,
    STATUS_ERROR
} status_t;

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

/* LIS3MDL register offset */
#define LIS3MDL_REG_CTRL1         0x20
#define LIS3MDL_REG_CTRL2         0x21
#define LIS3MDL_REG_OUT_X_L       0x28
#define LIS3MDL_REG_OUT_X_H       0x29
#define LIS3MDL_REG_OUT_Y_L       0x2a
#define LIS3MDL_REG_OUT_Y_H       0x2b
#define LIS3MDL_REG_OUT_Z_L       0x2c
#define LIS3MDL_REG_OUT_Z_H       0x2d
#define LIS3MDL_REG_INT_CFG       0x30


/* LIS3MDL addresses */
#define LIS3MDL_I2C_ADDRESS_1           0x3c  // SDO pin is high
#define LIS3MDL_I2C_ADDRESS_2           0x38  // SDO pin is low

/* Control register 1 bits */
typedef struct
{
  uint8_t st              : 1;
  uint8_t fast_odr        : 1;
  uint8_t od              : 3;
  uint8_t om              : 2;
  uint8_t temp_en         : 1;

} lis3mdl_ctrl_reg1_t;

/*Control register 2 bits */
typedef struct
{
  uint8_t not_used_01     : 2;
  uint8_t soft_rst        : 1;
  uint8_t reboot          : 1;
  uint8_t not_used_02     : 1;
  uint8_t fs              : 2;
  uint8_t not_used_03     : 1;
} lis3mdl_ctrl_reg2_t;

/* data rate config bits */
typedef enum
{
  LIS3MDL_fast          = 1,

  LIS3MDL_LP            = 0,
  LIS3MDL_MP            = 1,
  LIS3MDL_HP            = 2,
  LIS3MDL_UHP           = 3,

  LIS3MDL_OD_0H625      = 0,
  LIS3MDL_OD_1Hz25      = 1,
  LIS3MDL_OD_2Hz5       = 2,
  LIS3MDL_OD_5Hz        = 3,
  LIS3MDL_OD_10Hz       = 4,
  LIS3MDL_OD_20Hz       = 5,
  LIS3MDL_OD_40Hz       = 6,
  LIS3MDL_OD_80Hz       = 7,

} lis3mdl_date_rate_t;

/* Axis */
typedef enum
{
 LIS3MDL_X_AXIS         = 1,
 LIS3MDL_Y_AXIS         = 2,
 LIS3MDL_Z_AXIS         = 3,

} lis3mdl_axis_t;

/* interrupt enable/disable */
typedef enum
{
 LIS3MDL_INT_DISABLE     = 0,
 LIS3MDL_INT_ENABLE      = 1,
} lis3mdl_int_cfg_t;

/* Get the full-scale configuration */
uint8_t get_full_scale_config(void);
/* Get the current data rate */
float get_output_data_rate(void);
/* Set data rate */
void set_outut_data_rate(float set_rate);
/* Enable or disable the interrupt pin */
void enable_disable_interrupt( uint8_t int_flag);
/* Read the output data of a specified axis */
uint16_t output_data_axis(uint8_t axis);
#endif

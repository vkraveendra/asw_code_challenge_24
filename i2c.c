#include "i2c.h"

#include <stdint.h>
#include <stdio.h>


status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "read [%d] bytes from bus [%d] for register [%d]\n",
        length,
        bus_address,
        register_address);

    /* Setting the output to some arbitrary value */
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = 0xff;
    }

    return STATUS_OK;
}

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "write [%d] bytes to bus [%d] for register [%d]\n\t",
        length,
        bus_address,
        register_address);

    for (size_t i = 0; i < length; ++i) {
        printf("%p", buffer);
    }
    printf("\n");
    return STATUS_OK;
}
/* Get the full-scale configuration */
uint8_t get_full_scale_config(void)
{
	lis3mdl_ctrl_reg2_t reg_ctrl2;
	uint8_t full_scale_config,base_gauss;
	/* set base gauss */
	base_gauss = 4;
	/* Read control register 2 to get full scale config bits */
	if(i2c_read(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_CTRL2,1,(uint8_t*)&reg_ctrl2) == STATUS_OK){
		/*calcaulate full sale info based on config bits */
		full_scale_config = reg_ctrl2.fs;
		full_scale_config =  full_scale_config * base_gauss + base_gauss;
	}
	return full_scale_config;
}	

/* Get the current data rate */
float get_output_data_rate(void)
{
	lis3mdl_ctrl_reg1_t reg_ctrl1;
	float data_rate,basic_data_rate = 0.625;
	/* Read conrol resiter 1 to get the data rate */
	if(i2c_read(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_CTRL1,1,(uint8_t*)&reg_ctrl1) == STATUS_OK){
		/* check for fast data rates in accordance with mode of x and Y axis */
		if(reg_ctrl1.fast_odr == LIS3MDL_fast){
			switch(reg_ctrl1.om){
				case LIS3MDL_LP:
					data_rate = 1000;
					break;
				case LIS3MDL_MP:
					data_rate = 560;
					break;
				case LIS3MDL_HP:
					data_rate = 300;
					break;
				case LIS3MDL_UHP:
					data_rate = 155;
					break;
			}	
		}else{
			/* for non fast data rates ,caculate data rate basing on output data rate bits */
			data_rate = basic_data_rate * (1<<reg_ctrl1.od);
		}
	}
	return data_rate;
}
/* Set data rate */
void set_outut_data_rate(float set_rate)
{
	lis3mdl_ctrl_reg1_t reg_ctrl1;
	/* read control register 1 set the required bits */
	if(i2c_read(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_CTRL1,1,(uint8_t*)&reg_ctrl1) == STATUS_OK){
		if (set_rate == 1000){
			reg_ctrl1.fast_odr = LIS3MDL_fast;
			reg_ctrl1.om = LIS3MDL_LP;
		}
		else if(set_rate == 560){
			reg_ctrl1.fast_odr = LIS3MDL_fast;
			reg_ctrl1.om = LIS3MDL_MP;
		}
		else if(set_rate == 300){
			reg_ctrl1.fast_odr = LIS3MDL_fast;
			reg_ctrl1.om = LIS3MDL_HP;
		}
		else if(set_rate == 155){
			reg_ctrl1.fast_odr = LIS3MDL_fast;
			reg_ctrl1.om = LIS3MDL_UHP;
		}
		else if(set_rate == 0.625){
			reg_ctrl1.od = LIS3MDL_OD_0H625;
		}
		else if(set_rate == 1.25){
			reg_ctrl1.od = LIS3MDL_OD_1Hz25;
		}
		else if(set_rate == 2.5){
			reg_ctrl1.od = LIS3MDL_OD_2Hz5;
		}
		else if(set_rate == 5){
			reg_ctrl1.od = LIS3MDL_OD_5Hz;
		}
		else if(set_rate == 10){
			reg_ctrl1.od = LIS3MDL_OD_10Hz;
		}
		else if(set_rate == 20){
			reg_ctrl1.od = LIS3MDL_OD_20Hz;
		}
		else if(set_rate == 40){
			reg_ctrl1.od = LIS3MDL_OD_40Hz;
		}
		else if(set_rate == 80){
			reg_ctrl1.od = LIS3MDL_OD_80Hz;
		}
		/* write  the register with desired data rate */
		if(i2c_write(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_CTRL1,1,(uint8_t*)&reg_ctrl1) == STATUS_OK){
				printf("data rate set successfully\n");
		}
	}
}
	
/* Enable or disable the interrupt pin */
void enable_disable_interrupt( uint8_t int_flag){

	uint8_t int_cfg_reg;
	/* Read interrupt config */
	if(i2c_read(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_INT_CFG,1,&int_cfg_reg) == STATUS_OK)
	{
		if(int_flag == LIS3MDL_INT_ENABLE ){
			/* sets last bit of interrupt config and then writes updated value */
			int_cfg_reg = int_cfg_reg | 0x1;
			
			if (i2c_write(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_INT_CFG,1,&int_cfg_reg) == STATUS_OK){
				printf("interrupt pin is enabled\n");
			}
		} else if (int_flag == LIS3MDL_INT_DISABLE ){
			/* clears last bit of interrupt config  and then writes updated value */
			int_cfg_reg = int_cfg_reg & 0xfe;
		
			if(i2c_write(LIS3MDL_I2C_ADDRESS_1,LIS3MDL_REG_INT_CFG,1,&int_cfg_reg) == STATUS_OK){
				printf("interrupt pin is disabled\n");
			}
		}
	}	
}

/* Read the output data of a specified axis */
uint16_t output_data_axis(uint8_t axis){

    uint8_t axis_output_l,axis_output_h;
    uint16_t axis_output;
    uint8_t reg_out_l,reg_out_h;

	/* Checks axis and updates registers to read */
    if(axis == LIS3MDL_X_AXIS){
	    reg_out_l = LIS3MDL_REG_OUT_X_L;
	    reg_out_h = LIS3MDL_REG_OUT_X_H;
    }else if(axis == LIS3MDL_Y_AXIS){
	    reg_out_l = LIS3MDL_REG_OUT_Y_L;
	    reg_out_h = LIS3MDL_REG_OUT_Y_H;
    }else if(axis == LIS3MDL_Z_AXIS){
	    reg_out_l = LIS3MDL_REG_OUT_Z_L;
	    reg_out_h = LIS3MDL_REG_OUT_Z_H;
    }
	/* Read higher byte and lower byte of axis and then concate them to get output*/
    if(i2c_read(LIS3MDL_I2C_ADDRESS_1,reg_out_l,1,&axis_output_l) == STATUS_OK){
		if( i2c_read(LIS3MDL_I2C_ADDRESS_1,reg_out_h,1,&axis_output_h) == STATUS_OK){
    	    axis_output = (axis_output_h <<8) | axis_output_l;
		}
	}
    
    return axis_output;

}

int main(){
	uint8_t full_scale_config;
	float output_data_rate;
	uint16_t axis_output;
	
	full_scale_config = get_full_scale_config();

	output_data_rate =  get_output_data_rate();

	set_outut_data_rate(1.25);

	enable_disable_interrupt(LIS3MDL_INT_ENABLE);

	axis_output = output_data_axis(LIS3MDL_X_AXIS);

	return 0;
}	


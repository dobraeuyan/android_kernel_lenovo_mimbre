/*
 * platform_wm5102.c: wm51020 platform data initilization file
 *
 * (C) Copyright 2008 Intel Corporation
 * Author:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <asm/platform_sst_audio.h>
#include <linux/i2c.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/platform_device.h>
#include <linux/sfi.h>
#include <linux/spi/spi.h>
#include <asm/intel-mid.h>
#include "platform_wm5102.h"
#include <linux/mfd/arizona/pdata.h>
#include <linux/mfd/arizona/registers.h>

#define CODEC_GPIO_BASE			10

static const struct arizona_micd_range micd_ctp_ranges[] = {
	{ .max =  11, .key = KEY_MEDIA },
	{ .max =  220 , .key = KEY_VOLUMEUP },
	{ .max =  660 , .key = KEY_VOLUMEDOWN },
};

static struct arizona_micd_config wm5102_micd_config[]={
	{ 0        , 1 , 0 },
	{ ARIZONA_ACCDET_SRC	     ,  2  ,1},	
};

static struct arizona_pdata wm5102_pdata  = {
	.reset = 496,  
	.ldoena = 495,  
	.clk32k_src = ARIZONA_32KZ_MCLK2,
	.irq_gpio=505,
	.irq_flags = IRQF_TRIGGER_LOW  | IRQF_ONESHOT,
	.gpio_base = CODEC_GPIO_BASE,
	.micd_pol_gpio = CODEC_GPIO_BASE + 2,
        .jd_wake_time = 10,
	.init_mic_delay = 500,
	.micd_detect_debounce=500,
	.micd_bias_start_time = 7,
        .micd_rate = 6,
        .micd_configs= wm5102_micd_config,
        .num_micd_configs=ARRAY_SIZE(wm5102_micd_config),
        .micd_ranges=micd_ctp_ranges,
        .num_micd_ranges=ARRAY_SIZE(micd_ctp_ranges),
        .micd_force_micbias=1,
        .inmode = {
            [0]= ARIZONA_INMODE_DIFF, /*IN1L for Headset*/
            [1]= ARIZONA_INMODE_DIFF,
            [2]= ARIZONA_INMODE_DIFF,
         },
        .micbias={
           [0]={ /*MICBIAS1*/
                 .mV =2800 ,
                 .discharge =1 ,
                 .soft_start =0,             
                 .bypass =0,
           },
	   [1]={ /*MICBIAS2*/
                 .mV =2800 ,
                 .discharge =1 ,
                 .soft_start =0,          
                 .bypass =0,
           },
	   [2]={ /*MICBIAS3*/
                 .mV =2800 ,
                 .discharge =1 ,
                 .soft_start =0,                
                 .bypass =0,
           },
        },
};

static struct regulator_consumer_supply dc1v8_consumers[] = {
	REGULATOR_SUPPLY("AVDD", "1-001b"), /* wm5102 */
	REGULATOR_SUPPLY("DBVDD1", "1-001b"), /* wm5102 */
	REGULATOR_SUPPLY("LDOVDD", "1-001b"),
	REGULATOR_SUPPLY("DBVDD2", "wm5102-codec"),
	REGULATOR_SUPPLY("DBVDD3", "wm5102-codec"),
	REGULATOR_SUPPLY("CPVDD", "wm5102-codec"),
	REGULATOR_SUPPLY("SPKVDDL", "wm5102-codec"),
	REGULATOR_SUPPLY("SPKVDDR", "wm5102-codec"),
	REGULATOR_SUPPLY("CPVDD", "1-001b"),
};

static struct regulator_init_data dc1v8_data = {
	.constraints = {
		.always_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(dc1v8_consumers),
	.consumer_supplies = dc1v8_consumers,
};

static struct fixed_voltage_config dc1v8vdd_pdata = {
	.supply_name = "DC_1V8",
	.microvolts = 1800000,
	.init_data = &dc1v8_data,
	.gpio = -1,
};

static struct platform_device dc1v8_device = {
	.name		= "reg-fixed-voltage",
	.id		= 0,
	.dev = {
		.platform_data = &dc1v8vdd_pdata,
	},
};

static struct i2c_board_info __initdata wm5102_board_info = {
	I2C_BOARD_INFO("wm5102", 0x1b),
	.platform_data = &wm5102_pdata,
};

#define I2C_WM5102_DRIVER_NAME "wm5102"
static int __init byt_audio_platform_init(void)
{
	struct platform_device *pdev;
	int ret;
	int i2c_busnum = 1;
	struct i2c_board_info i2c_info;
        struct i2c_adapter *adapter;

	pr_info("in %s\n", __func__);

#if 0
       ret = add_sst_platform_device();

       if (ret < 0) {
               pr_err("%s failed to sst_platform device\n", __func__);
               return 0;
       }
#endif

	platform_device_register(&dc1v8_device);

#if 1 /*mask as new mode for Intel cherryChair I2C register*/
        memset(&i2c_info, 0, sizeof(i2c_info));
        strncpy(i2c_info.type,I2C_WM5102_DRIVER_NAME, strlen(I2C_WM5102_DRIVER_NAME));
	i2c_info.addr = 0x1b;
	i2c_info.platform_data = &wm5102_pdata;


	adapter = i2c_get_adapter(i2c_busnum);
	if(adapter){
		if(i2c_new_device(adapter,&i2c_info)){
			printk(KERN_ERR "add new i2c device %s , addr 0x%x\n", I2C_WM5102_DRIVER_NAME,i2c_info.addr);			
		}else{	
			printk(KERN_ERR "add new i2c device %s , addr 0x%x fail !!!\n", I2C_WM5102_DRIVER_NAME,i2c_info.addr);
		}
	}else{
		printk(KERN_ERR "[%s]get adapter %d fail\n",__func__,i2c_busnum);
		return -EINVAL;
	}
#else	
	i2c_register_board_info(2, &wm5102_board_info, 1);
#endif /*Mark end*/

#if 0 
	pdev = platform_device_alloc("byt_wm5102", -1);
	
	if (!pdev) {
		pr_err("failed to allocate byt_wm5102 platform device\n");
		return 0;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		pr_err("failed to add byt_wm5102 platform device\n");
		platform_device_put(pdev);
		return 0;
	}
#endif	
	return 0;
}

device_initcall(byt_audio_platform_init);



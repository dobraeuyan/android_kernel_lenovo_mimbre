#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/gpio.h>

#include <linux/input/smtc/misc/sx86xx.h> /* main struct, interrupt,init,pointers */
#include <linux/input/smtc/misc/sx9310_i2c_reg.h>
#include <linux/input/smtc/misc/sx9310_platform_data.h>  /* platform data */

#include "sx9310-specifics.h"
#define SAR_DYNAMIC_I2C
#define DRIVER_NAME "sx9310"

#ifdef CONFIG_INPUT_SX9310
static int __init sx9310_platform_init(void){
#ifdef SAR_DYNAMIC_I2C
	int i2c_busnum = 1;
#else
	int i2c_busnum = 2;
#endif

 	struct i2c_board_info i2c_info;
	struct i2c_adapter *adapter;
	printk(KERN_ERR "sx9310_platform_init \n");
	
	 memset(&i2c_info, 0, sizeof(i2c_info));
	 strncpy(i2c_info.type, DRIVER_NAME, strlen(DRIVER_NAME));

	 i2c_info.addr = 0x28;
	 i2c_info.irq = gpio_to_irq(GPIO_SX9310_NIRQ);;

	 printk(KERN_ERR "%s I2C bus = %d, name = %s, irq = 0x%2x, addr = 0x%x\n",__func__,
				 i2c_busnum,
				 i2c_info.type,
				 i2c_info.irq,
				 i2c_info.addr);

	 i2c_info.platform_data = &sx9310_config;
	 i2c_info.flags = I2C_CLIENT_WAKE;

#ifdef SAR_DYNAMIC_I2C
	adapter = i2c_get_adapter(i2c_busnum);
	if(adapter){
		if(i2c_new_device(adapter,&i2c_info)){
			printk(KERN_ERR "add new i2c device %s , addr 0x%x\n", DRIVER_NAME,i2c_info.addr);
			return 0;
		}else{
			printk(KERN_ERR "add new i2c device %s , addr 0x%x fail !!!\n", DRIVER_NAME,i2c_info.addr);
		}
	}else{
		printk(KERN_ERR "[%s]get adapter %d fail\n",__func__,i2c_busnum);
		return -EINVAL;
	}
#else
    return i2c_register_board_info(i2c_busnum, &i2c_info, 1);
#endif
}

#ifdef SAR_DYNAMIC_I2C
device_initcall(sx9310_platform_init);
#else
fs_initcall(sx9310_platform_init);
#endif
#endif

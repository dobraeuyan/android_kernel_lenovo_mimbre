# IPC Devices
#obj-y += platform_ipc.o
#obj-$(subst m,y,$(CONFIG_SND_SST_PLATFORM)) += platform_byt_audio.o
obj-$(subst m,y,$(CONFIG_MFD_INTEL_MSIC)) += platform_msic.o
obj-$(subst m,y,$(CONFIG_SND_MFLD_MACHINE)) += platform_msic_audio.o
obj-$(subst m,y,$(CONFIG_GPIO_MSIC)) += platform_msic_gpio.o
obj-$(subst m,y,$(CONFIG_MFD_INTEL_MSIC)) += platform_msic_ocd.o
obj-$(subst m,y,$(CONFIG_MFD_INTEL_MSIC)) += platform_msic_battery.o
obj-$(subst m,y,$(CONFIG_SND_CHT_MACHINE)) += platform_byt_audio.o
#obj-$(subst m,y,$(CONFIG_MFD_ARIZONA_I2C)) += platform_byt_audio_i2c.o
#obj-$(subst m,y,$(CONFIG_MFD_ARIZONA_SPI)) += platform_byt_audio_spi.o
obj-$(subst m,y,$(CONFIG_MFD_ARIZONA_I2C)) += platform_wm5102_i2c.o
obj-$(subst m,y,$(CONFIG_MFD_ARIZONA_SPI)) += platform_wm5102_spi.o
obj-$(subst m,y,$(CONFIG_INTEL_MID_POWER_BUTTON)) += platform_msic_power_btn.o
obj-$(subst m,y,$(CONFIG_GPIO_INTEL_PMIC)) += platform_pmic_gpio.o
obj-$(subst m,y,$(CONFIG_INTEL_MFLD_THERMAL)) += platform_msic_thermal.o
obj-$(subst m,y,$(CONFIG_VLV2_PLAT_CLK)) += platform_vlv2_plat_clk.o
# I2C Devices
obj-$(subst m,y,$(CONFIG_SENSORS_EMC1403)) += platform_emc1403.o
obj-$(subst m,y,$(CONFIG_BATTERY_BQ27x00)) += platform_bq27500.o
obj-$(subst m,y,$(CONFIG_BATTERY_BQ27x00_X13)) += platform_bq27500_x13.o
#obj-$(subst m,y,$(CONFIG_SENSORS_LIS3LV02D)) += platform_lis331.o
obj-$(subst m,y,$(CONFIG_GPIO_PCA953X)) += platform_max7315.o
obj-$(subst m,y,$(CONFIG_INPUT_MPU3050)) += platform_mpu3050.o
obj-$(subst m,y,$(CONFIG_INPUT_BMA150)) += platform_bma023.o
obj-$(subst m,y,$(CONFIG_GPIO_PCA953X)) += platform_tca6416.o
obj-$(subst m,y,$(CONFIG_DRM_MEDFIELD)) += platform_tc35876x.o
# SPI Devices
obj-$(subst m,y,$(CONFIG_SERIAL_MRST_MAX3110)) += platform_max3111.o
# MISC Devices
#obj-$(subst m,y,$(CONFIG_KEYBOARD_GPIO)) += platform_gpio_keys.o
# WIFI devices
obj-$(subst m,y,$(CONFIG_MMC_SDHCI_ACPI)) += platform_sdio_regulator.o
obj-y += platform_iwlwifi.o
#WC regulator support for camera
obj-$(subst m,y,$(CONFIG_REGULATOR_WHISKEY_COVE)) += platform_camera_regulator.o
obj-$(subst m,y,$(CONFIG_TOUCHSCREEN_SYNAPTICS_DSX)) += platform_s7813.o
obj-$(subst m,y,$(CONFIG_TOUCHSCREEN_HIDEEP)) += platform_hideep.o
obj-$(subst m,y,$(CONFIG_INPUT_SX9500)) += platform_sx9500.o
obj-$(subst m,y,$(CONFIG_INPUT_SX9310)) += platform_sx9310.o
obj-$(subst m,y,$(CONFIG_LEDS_LP5521)) += platform_lp5521.o


obj-$(CONFIG_X86_INTEL_MID) += intel-mid.o intel_mid_vrtc.o mfld.o mrfl.o
obj-$(CONFIG_EARLY_PRINTK_INTEL_MID) += early_printk_intel_mid.o

obj-$(CONFIG_GMIN_INTEL_MID) += intel_mid_pcihelpers.o
obj-$(CONFIG_GMIN_INTEL_MID) += atomisp_gmin_platform.o
obj-$(subst m,y,$(CONFIG_VIDEO_OV8865)) += platform_ov8865.o
obj-$(subst m,y,$(CONFIG_VIDEO_OV5693)) += platform_ov5693.o
obj-$(subst m,y,$(CONFIG_VIDEO_AR1335)) += platform_ar1335.o
obj-y += lenovo_mid_boards.o

obj-y += device_libs/

# SFI specific code
ifdef CONFIG_X86_INTEL_MID
obj-$(CONFIG_SFI) += sfi.o
endif

/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __EFFECT_OFFLOAD_H
#define __EFFECT_OFFLOAD_H
#include <linux/types.h>
#define SNDRV_EFFECT_VERSION SNDRV_PROTOCOL_VERSION(0, 1, 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct snd_effect {
 char uuid[16];
 __u32 device;
 __u32 pos;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 mode;
} __packed;
struct snd_effect_params {
 char uuid[16];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 device;
 __u32 size;
 __u64 buffer_ptr;
} __packed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct snd_effect_caps {
 __u32 size;
 __u64 buffer_ptr;
} __packed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_CTL_IOCTL_EFFECT_VERSION _IOR('E', 0x00, int)
#define SNDRV_CTL_IOCTL_EFFECT_CREATE _IOW('E', 0x01,  struct snd_effect)
#define SNDRV_CTL_IOCTL_EFFECT_DESTROY _IOW('E', 0x02,  struct snd_effect)
#define SNDRV_CTL_IOCTL_EFFECT_SET_PARAMS _IOW('E', 0x03,  struct snd_effect_params)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_CTL_IOCTL_EFFECT_GET_PARAMS _IOWR('E', 0x04,  struct snd_effect_params)
#define SNDRV_CTL_IOCTL_EFFECT_QUERY_NUM _IOR('E', 0x05, int)
#define SNDRV_CTL_IOCTL_EFFECT_QUERY_CAPS _IOWR('E', 0x06,  struct snd_effect_caps)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */

/*******************************************************************  
* (c) Copyright 2011-2012 Discretix Technologies Ltd.              *
* This software is protected by copyright, international           *
* treaties and patents.                                            *
* Use of this Software as part of or with the Discretix CryptoCell *
* or Packet Engine products is governed by the products'           *
* commercial end user license agreement ("EULA").                  *
* It is possible that copies of this Software might be distributed *
* under some type of GNU General Public License ("GPL").           *
* Notwithstanding any such distribution under the terms of GPL,    *
* GPL does not govern the use of this Software as part of or with  *
* the Discretix CryptoCell or Packet Engine products, for which a  *
* EULA is required.                                                *
* If the product's EULA allows any copy or reproduction of this    *
* Software, then such copy or reproduction must include this       *
* Copyright Notice as well as any other notices provided           *
* thereunder.                                                      *
********************************************************************/

#ifndef _DX_PAL_LOG_H_
#define _DX_PAL_LOG_H_

#include "dx_pal_log_plat.h"


/* PAL log levels (to be used in DX_PAL_logLevel) */
#define DX_PAL_LOG_LEVEL_ERR       0
#define DX_PAL_LOG_LEVEL_WARN      1
#define DX_PAL_LOG_LEVEL_INFO      2
#define DX_PAL_LOG_LEVEL_DEBUG     3
#define DX_PAL_LOG_LEVEL_TRACE     4
#define DX_PAL_LOG_LEVEL_DATA      5

/* This printf wrapper masks maps log level to MODULE_* levels and masks prints *
   from specific components at run time based on PAL_LOG_CUR_COMPONENT and    *
*  pal_log_component_mask.                                                    */
#define _DX_PAL_LOG(level, format, ...)  \
	if (DX_PAL_log_mask & DX_PAL_LOG_CUR_COMPONENT) \
		PRINTF("%s: " format, __func__, ##__VA_ARGS__)

/* Select compile time log level */
#if defined(DEBUG)
#include <stdio.h>
#define DX_PAL_MAX_LOG_LEVEL DX_PAL_LOG_LEVEL_DEBUG
#else /* only critical prints */
#define DX_PAL_MAX_LOG_LEVEL DX_PAL_LOG_LEVEL_ERR
#endif

void DX_PAL_LogInit();
void DX_PAL_LogLevelSet(unsigned int setLevel);

#ifdef _MAIN_C_
/* Declare global variable in main.c only */
unsigned int DX_PAL_logLevel = DX_PAL_MAX_LOG_LEVEL;
unsigned int DX_PAL_log_mask = 0xFFFFFFFF;
#else
extern unsigned int DX_PAL_logLevel;
extern unsigned int DX_PAL_log_mask;
#endif

#define DX_PAL_LOG_ERR(format, ...) \
	_DX_PAL_LOG(ERR, format, ##__VA_ARGS__)

#if (DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_WARN)
#define DX_PAL_LOG_WARN(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_WARN) \
		_DX_PAL_LOG(WARN, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_WARN(format, arg...) do {} while (0)
#endif

#if (DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_INFO)
#define DX_PAL_LOG_INFO(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_INFO) \
		_DX_PAL_LOG(INFO, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_INFO(format, arg...) do {} while (0)
#endif

#if (DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_DEBUG)
#define DX_PAL_LOG_DEBUG(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_DEBUG) \
		_DX_PAL_LOG(DEBUG, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_DEBUG(format, arg...) do {} while (0)
#endif

#if (DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_TRACE)
#define DX_PAL_LOG_TRACE(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_TRACE) \
		_DX_PAL_LOG(TRACE, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_TRACE(format, arg...) do {} while (0)
#endif

#if (DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_TRACE)
#define DX_PAL_LOG_DATA(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_TRACE) \
		_DX_PAL_LOG(DATA, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_DATA(format, arg...) do {} while (0)
#endif

#endif /*_DX_PAL_LOG_H_*/

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

#ifndef SEP_INIT_CC_ERROR_H
#define SEP_INIT_CC_ERROR_H
 
 
#include "sep_error.h"

	/*! \file lcs.h
    \brief This file containes lcs definitions
*/
#define DX_INIT_CC_OK					DX_SEP_OK
/* DX_INIT_CC_MODULE_ERROR_BASE - 0xE0004000 */
#define DX_CC_INIT_MSG_CS_ERROR				(DX_INIT_CC_MODULE_ERROR_BASE + 0x1)
#define DX_CC_INIT_MSG_WRONG_TOKEN_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x2)
#define DX_CC_INIT_MSG_WRONG_OP_CODE_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x3)
#define DX_CC_INIT_MSG_WRONG_RESIDENT_ADDR_ERROR	(DX_INIT_CC_MODULE_ERROR_BASE + 0x4)
#define DX_CC_INIT_MSG_WRONG_I_CACHE_ADDR_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x5)
#define DX_CC_INIT_MSG_WRONG_I_CACHE_DEST_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x6)
#define DX_CC_INIT_MSG_WRONG_D_CACHE_ADDR_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x7)
#define DX_CC_INIT_MSG_WRONG_D_CACHE_SIZE_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0x8)
#define DX_CC_INIT_MSG_WRONG_INIT_EXT_ADDR_ERROR	(DX_INIT_CC_MODULE_ERROR_BASE + 0x9)
#define DX_CC_INIT_MSG_WRONG_VRL_ADDR_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0xA)
#define DX_CC_INIT_MSG_WRONG_MAGIC_NUM_ERROR		(DX_INIT_CC_MODULE_ERROR_BASE + 0xB)
#define DX_CC_INIT_MSG_WRONG_OUTPUT_BUFF_ADDR_ERROR	(DX_INIT_CC_MODULE_ERROR_BASE + 0xC)
#define DX_CC_INIT_MSG_WRONG_OUTPUT_BUFF_SIZE_ERROR	(DX_INIT_CC_MODULE_ERROR_BASE + 0xD)
#define DX_RESERVED_0_ERROR				(DX_INIT_CC_MODULE_ERROR_BASE + 0xE)



/* DX_INIT_CC_EXT_MODULE_ERROR_BASE - 0xE0005000 */
#define DX_CC_INIT_EXT_FIRST_PARAM_ERRR			(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x1)
#define DX_CC_INIT_EXT_WRONG_LAST_PARAM_LENGTH_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x2)
#define DX_CC_INIT_EXT_WRONG_CHECKSUM_VALUE_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x3)
#define DX_CC_INIT_EXT_WRONG_DISABLE_MODULE_LENGTH_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x4)
#define DX_CC_INIT_EXT_WRONG_AXI_CONFIG_LENGTH_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x5)
#define DX_CC_INIT_EXT_WRONG_PARAM_ERRR			(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x6)
#define DX_CC_INIT_EXT_EXCEED_MAX_PARAM_PARAM_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x7)
#define DX_CC_INIT_EXT_WRONG_SEP_FREQ_LENGTH_ERRR	(DX_INIT_CC_EXT_MODULE_ERROR_BASE + 0x8)


#endif

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


/* This file contains the definitions of the OTP data that the SEP copies
into the SRAM during the first boot process */


#ifndef _SEP_SRAM_MAP_
#define _SEP_SRAM_MAP_

#define DX_FIRST_OEM_KEY_OFFSET_IN_SRAM         0x0
#define DX_SECOND_OEM_KEY_OFFSET_IN_SRAM        0x4
#define DX_THIRD_OEM_KEY_OFFSET_IN_SRAM         0x8
#define DX_LCS_OFFSET_IN_SRAM                   0xC
#define DX_MISC_OFFSET_IN_SRAM                  0xD
#define DX_CC_INIT_MSG_OFFSET_IN_SRAM		0x100
#define DX_PKA_MEMORY_OFFSET_IN_SRAM		0x200

#endif /*_GEN_SRAM_MAP_*/

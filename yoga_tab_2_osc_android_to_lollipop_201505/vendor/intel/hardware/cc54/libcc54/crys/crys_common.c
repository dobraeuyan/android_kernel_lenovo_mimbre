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
#define DX_PAL_LOG_CUR_COMPONENT DX_LOG_MASK_CRYS_API

/*****************************************************************************
  \file CRYS_common.c   						     *
  CRYS infrastructure functions common to all the modules (on host)          *
*****************************************************************************/
#include "driver_interface.h"
#include "crys_error.h"

/*!
 * Convert from DxDI return code to CRYS return code
 * 
 * \param diRc 
 * \param sepRetCode 
 */
CRYSError_t DX_DiErr2CrysErr(DxDI_RetCode_t diRc, DxUint32_t sepRetCode)
{
	switch (diRc) {
	case DXDI_RET_OK:
		return CRYS_OK;
	case DXDI_RET_ENODEV:   
	case DXDI_RET_EINTERNAL:
	case DXDI_RET_ENOTSUP:
	case DXDI_RET_ENOPERM:  
	case DXDI_RET_EINVAL:  
	case DXDI_RET_ENORSC:   
	case DXDI_RET_EHW:
		
		return diRc;
	case DXDI_RET_ESEP:
		/* Pass through SeP/CRYS error code */
		return sepRetCode;
	default:
		/* ??? */
		return diRc;
	}
}

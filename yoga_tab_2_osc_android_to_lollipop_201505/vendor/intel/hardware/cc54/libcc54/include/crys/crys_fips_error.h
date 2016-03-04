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

 
 
#ifndef CRYS_FIPS_ERROR_H
#define CRYS_FIPS_ERROR_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object % CRYS_FIPS_error.h    : %
   *  State           :  %state%
   *  Creation date   :  25 Sept.2008
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module contains the definitions of the CRYS AESCCM errors.
   *
   *  \version CRYS_AESCCM_error.h#1:incl:1
   *  \author R.Levin
   */

/************************ Defines ******************************/

/* The CRYS FIPS module errors. 
   CRYS_FIPS_MODULE_ERROR_BASE = 0x00F01700 */
#define CRYS_FIPS_INVALID_POINTER_ERROR     (CRYS_FIPS_MODULE_ERROR_BASE + 0x00UL)
#define CRYS_FIPS_INVALID_INPUT             (CRYS_FIPS_MODULE_ERROR_BASE + 0x01UL)

#define CRYS_FIPS_IS_NOT_SUPPORTED          (CRYS_FIPS_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif




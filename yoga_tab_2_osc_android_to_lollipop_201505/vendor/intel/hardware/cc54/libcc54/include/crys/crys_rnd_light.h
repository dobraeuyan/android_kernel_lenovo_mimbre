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



#ifndef CRYS_RND_LIGHT_H
#define CRYS_RND_LIGHT_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_error.h"
#include "crys_defs.h"
#include "crys_aes.h"
#include "llf_rnd.h"

#include "crys_pka_defs.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*
*  Object CRYS_RND.h
*  State           :  %state%
*  Creation date   :  Thu Dec 16 17:11:32 2004
*  Last modified   :  %modify_time% 18 Mar. 2010
*/
/** @file
*  \brief This file contains the CRYS APIs used for random number generation.
*
*  \version CRYS_RND.h#1:incl:15
*  \author adams, R.Levin
*  \remarks Copyright (C) 2005 by Discretix Technologies Ltd.
*           All Rights reserved
*/
/************************ Defines ******************************/

#define CRYS_RND_KEY_SIZE_IN_WORDS 4

/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/
/* -----------------------------------------------------------------------------
 * @brief The CRYS_RND_UnInstantiation cleans the unused RNG State for security goals.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */


CIMPORT_C CRYSError_t CRYS_RND_UnInstantiationLight(void);

/* -----------------------------------------------------------------------------
 * @brief The function performs instantiation of RNG and creates new 
 *        internal State (including Seed) of RNG. 
 *
 *       The function sets use user's seed as seed for RND operations.
 * 
 * @param[in] userSeed_ptr - A pointer to user's seed buffer used for RND.
 * 
 * @param[in] userSeedSizeInBytes
 * 
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */

CIMPORT_C CRYSError_t CRYS_RND_InstantiationLight( DxUint8_t  *userSeed_ptr,DxUint32_t  userSeedSizeInBytes);

/**********************************************************************************************************/
/* 
 * @brief The CRYS_RND_GenerateVectorInRange function generates a random vector Rand in range:
 *            1 < RandVect < MaxVect,   using the CRYS_RND_GenerateVector function:
 *        The function performs the following:
 *        1.  Check input parameters.
 *        2.  If maxVect != 0 (maxVect is provided), then calculate required size of random 
 *            equaled to actual bit size of MaxVector, else set it = rndSizeInBits.
 *        3.  Calls the CRYS_RND_GenerateVector() function for generating random vector 
 *            RndVect of required size.
 *        4.  If maxVect is provided, then:
 *             4.1. Sets all high bits of RndVect, greatest than MSBit of MaxVector, to 0.
 *             4.2. If size of random vector > 16 bytes, then: 
 *                     4.2.1. Compares high 16 bytes of randVect to maxVect and low limit 
 *                     4.2.2. If condition 1 < randVect16 < maxVect16 is not satisfied, 
 *                            then generate new high 16 bytes rndVect16 and go to step 4.2.1.
 *             4.3. Compares full value of RndVect with MaxVector and with 1 . If condition  
 *                 1 < RandVect < MaxVector is not satisfied, then go to step 3, else go to 6.
 *        5. If maxVect is not provided, then set MSBit of rndVect to 1.
 *        6. Output the result and Exit. 
 *
 *         Note: Random and Max vectors are given as sequence of bytes, where LSB is most left byte
 *               and MSB = most right one. 
 *
 * @rndSize[in]         - The maximal size of random vector (in bits).
 * @maxVect_ptr[in]     - The pointer to vector defines high limit of random vector.
 * @rndVect_ptr[in,out] - The output vector.
 * @return CRYSError_t  - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_GenerateVectorInRangeLight(DxUint32_t  rndSizeInBits,
							  DxUint8_t *maxVect_ptr, 
							  DxUint8_t *rndVect_ptr);
																			

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CRYS_RND_H */


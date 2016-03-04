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

#ifndef  _DX_MANAGEMENT_H
#define  _DX_MANAGEMENT_H


/******************************************************************************
*                        	DEFINITIONS
******************************************************************************/

/* ROM CRC address  */
#define DX_ROM_CRC_ADDR			0x0400FFEC
#ifndef DX_FAILURE
	#define DX_FAILURE		1
#endif

/******************************************************************************
*				TYPE DEFINITIONS
******************************************************************************/


/******************************************************************************
*				FUNCTION PROTOTYPES
******************************************************************************/

/*
 @Brief: This function returns the ROM CRC value
 @params:
	crcVal - returns the crc value	
 @return:
	DX_SUCCESS 
	
*/
DxUint32_t DX_MNG_GetRomCrc(DxUint32_t *crcVal);

/*
 @Brief: This function returns the ROM CRC value
 @params:
        lcsVal - returns the lcs value
 @return:
	DX_SUCCESS
	
*/
DxUint32_t DX_MNG_GetLcs(DxUint32_t *lcsVal);

/*
 @Brief: This function returns the fw and the rom versions
 @params:
        fwVer [out] - fw version
	romVer [out] - rom version
 @return:
	DX_SUCCESS 
	
*/
DxUint32_t DX_MNG_GetFwVersion(DxUint32_t *fwVer, DxUint32_t *romVer);

#endif /*_DX_MANAGEMENT_H*/

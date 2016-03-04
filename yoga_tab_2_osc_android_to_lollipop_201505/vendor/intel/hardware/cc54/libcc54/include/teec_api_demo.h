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

/* \file teec_api_demo.h
   Header file for definitions common to the host client application and the
   SeP Applet of the TEE Client API demo.
   The test is an SIMD calculator.
   There are 5 commands:
   1. Add. 2. Substract. 3. Multiply. 4. Divide. 5. Get result.
   Opening a session resets (to zero) the calculator state.
   Operations 1-4 take struct TeecTestData as 3 input data: a, b, c.
   Each of them is used independently in the SIMD style.
   Operation #5 returns in the same struct TeecTestData, the result for each.
*/
#ifndef __TEEC_API_DEMO_H__
#define __TEEC_API_DEMO_H__

/* Comment the following line to run the demo using memory reference to user data struct. */
/* After any change here both applet as well as host client application should be recompiled. */
//#define DEMO_BY_VALUE

#define DEMO_APP_UUID {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}

/* Demo. applet commands */
/* These values do not have to be aligned with TEE Client API return codes (but for 0 for success) */
#define DEMO_CMD_ADD  1
#define DEMO_CMD_SUB  2
#define DEMO_CMD_MULT 3
#define DEMO_CMD_DIV  4
#define DEMO_CMD_RESULT 5

/* Demo. error codes */
#define DEMO_ERR_ENORSC 1 /* Not enough resources (free sessions context) */
#define DEMO_ERR_EINVAL 2 /* Invalid parameters */
#define DEMO_ERR_GENERIC 9 /* Any other unexpected error */

/* Data structure for test function parameters */
struct TeecTestData {
	uint8_t a;
	uint16_t b;
	uint32_t c;
};

#endif /*__TEEC_API_DEMO_H__*/

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

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Wed Nov 17 17:25:55 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief A brief description of this module
   *
   *  \version CRYS_AES.c#1:csrc:6
   *  \author adams
   */



/************* Include Files ****************/

/* .............. CRYS level includes ................. */

#include "DX_VOS_Mem.h"
#include "DX_VOS_Memmap.h"
#include "DX_VOS_Sem.h"
#include "error.h"
#include "crys_aes_error.h"
#include "crys_aes.h"
#include "CRYS_COMMON.h"
#include "CRYS_EXT_IF_OTF.h"
#include "SEPDriver.h" 
#include "csi_host_op_code.h"
#include "gen.h"
/************************ Defines ******************************/



/************************ MACROS ******************************/



/************************ Global Data ******************************/


/************* Private function prototype ****************/

/****************************************************************************************************/
/** 
 * @brief This function is called by the SEP driver for CRYS callbacks 
 */
CRYSError_t  CRYS_CALLBACK_Handler()
{
  /* offset */
  DxUint32_t    sramOffset;
  
  /* The return error identifier */
  CRYSError_t   Error;
  
  /* message param */
  DxUint32_t    messageParam[2];
  
  /* CSI status */
  DxUint32_t    CSI_Status;
  
  /* data size */
  DxUint32_t    dataSize;
  
  /*----------------------
      CODE
  -----------------------*/
  
  /* new message arrived - parse it */
  
  /* start the message */
  Error = SEPDriver_StartIncomingMessage(&sramOffset);
  if(Error != DX_OK  && Error != DX_FAILED_CRC_IN_MESSAGE_ERR)
  {
    return;
  }
   
  /* read opcode */
  Error = SEPDriver_ReadParamater((DxUint32_t)messageParam,
                            sizeof(DxUint32_t),
                            sizeof(DxUint32_t),
                            &sramOffset ,
                            DX_FALSE);
  if(Error != DX_OK)
  {
    return;
  }
   
   
  switch(messageParam[0])
  {
    case DX_SEP_OP_CODE_CSI_START_OPERATION:
      
      /* read data size */
      SEPDriver_ReadParamater((DxUint32_t)&dataSize,
                               sizeof(DxUint32_t),
                               sizeof(DxUint32_t),
                               &sramOffset ,
                               DX_FALSE);
                               
      /* call CSI operation */
      CSI_StartOperation(dataSize);
      break;
      
    case DX_SEP_OP_CODE_CSI_START_OUTPUT_OPERATION:
    
      /* read data size */
      SEPDriver_ReadParamater((DxUint32_t)&dataSize,
                               sizeof(DxUint32_t),
                               sizeof(DxUint32_t),
                               &sramOffset ,
                               DX_FALSE);
                               
      /* call CSI operation */
      CSI_StartOutputOperation(dataSize);
      break;  
    
    case DX_SEP_OP_CODE_CSI_GET_STATUS:
      do
      {
        CSI_GetStatus( &CSI_Status );
      }while( CSI_Status != CSI_DONE);
      break;
        
    default:
      break;
  }
   
  /* write the response back */
  SEPDriver_StartMessage(&sramOffset);
    
  /************************************************************************/
  /*  Send message                                                        */
  /************************************************************************/
  
  SEPDriver_WriteParamater((DxUint32_t)messageParam,
                            sizeof(DxUint32_t),
                            sizeof(DxUint32_t),
                            &sramOffset ,
                            DX_FALSE);
    

  /************************************************************************/
  /* End message (signal the SEP)                                         */
  /************************************************************************/
  SEPDriver_EndReplyMessage(sramOffset);
}


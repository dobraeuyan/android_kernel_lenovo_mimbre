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

 
#ifndef CRYS_ECPKI_ELGAMAL_H
#define CRYS_ECPKI_ELGAMAL_H

/*
 *  Object name     :  CRYS_ECPKI_ELGAMAL.h
   *  State           :  %state%
   *  Creation date   :  02.02.2006
   *  Last modified   :  %modify_time%
   */
/** @file
 * \brief This module defines the API that supports EC Elgamal encryption and
 *        decryption operations 
 *
 * \version CRYS_ECPKI_ELGAMAL.h#1:cinc:1
 * \author R.Levin
 */
 
#include "crys_defs.h"
#include "crys_ecpki_types.h"
#include "crys_ecpki_error.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************************
 *		       _DX_ECPKI_ELGAMAL_Encrypt function	
 *****************************************************************************************/
/**
   @brief: 	This function performs encryption of the message (transmitted key or any other 
            plain text)  with public key of the receiver  using EC Elgamal algorithm. 
			
            For calling this function from CRYS use the macro definition: 
		    CRYS_ECPKI_ELGAMAL_Encrypt or CRYS_ECPKI_ELGAMAL_EncryptExact.
			
            Operation: The sender encrypts the message by the following sequence of steps:
		1. Checks validity of input pointers and parameters.
		2. Converts incoming message from big to little endian.
		3. Calls the LLF_ECPKI_ELGAMAL_Encrypt function, which performs encryption
		according to Elgamal algorithm and outputs Encrypted message as two EC points V and B:
		4. Converts the encrypted message (V,B) into big endian form.
		5. Outputs the encrypted message as bytes array in big endian.
		6. Exits.

		    Assumptions: 
		1. Public key W and EC domain parameters q, a, b, r, and G are valid and 
		   associated with each other; 
		2. Incoming receiver's public key must be in big endianness.  
		3. The size of incomming message f must be: 
			Case isExactMessageInSize = FALSE: great than 0 and equal or less, than:
				(size of EC modulus - 1)  - for case, that MSByte of modulus >= 100 :
				(size of EC modulus - 2)  - for case, that MSByte of modulus < 100.
			Case isExactMessageInSize = TRUE: equal to modulus size.
                4. The size of the message must be agreed with the receiver (i.e. in both encrypt and decrypt 
		   functions size of the plain message must be the same value).
                
   @param[in]  ReceiverUserPublKeyBE_ptr - A pointer to receiver public key (key in big endian).  
   @param[in]  MessageIn_ptr         - A pointer to message to be encrypted (message in big endian).
   @param[in]  MessageInSizeBytes    - A size of incoming message in bytes. 
				       If the flag isExactMessageInSize is set to TRUE, then the message size must be exactly 
					equal to modulus size and the message value must be suitable for Elgamal encrypt without change
					any bits. If these conditions are not accomplished, then the function returns error.
				       If the flag isExactMessageInSize is set to FALSE, then the function tries to change the
					high byte of the message buffer in order to set it on EC curve and encrypt. In this case 
					the maximal message size must be:
                                            MessageInSizeBytes <= ModulusSizeInBytes-1, but
					    if the value of MSByte of EC Domain modulus is less than 100,
					    then maximal size of the message must be decreased by 1 byte (now the
					    last condition is actual only for one of implemented domains - 
					    CRYS_ECPKI_DomainID_secp521r1).
   @param[in]  EncrMessageOut_ptr    - A pointer to buffer for encrypted message.
   @param[in,out] EncrMessOutSize_ptr- A pointer to size of uzer passed buffer for encrypted
                                       message (in) and actual size of encrypted message (out).
                                       Size of buffer must be not less, than 4*ModulusSizeInBytes. 
   @param[in]  IsEphemerKeyInternal  - A parameter defining whether ephemeral key internal or external (1,0).
   @param[in]  EphemerPrivKeyIn_ptr  - A pointer to ephemeral private key /user not uses this parameter/.
   @param[in]  EphemerPrivKeySizeBytes - A size (in bytes) of sender's ephemeral private key data, must be equal 
                                        or less than EC order size in bytes /user not uses this parameter/.
   @param[in]  TempData_ptr            - A pointer to structure, containing temporary buffers of specified
                                         type. 
   @psram[in]  isExactMessageInSize    - A flag, defining is the message given with exact size and suitable for 
					Elgamal encryption without changes any bits.
	    	     
   @return CRYSError_t - CRYS_OK, 
			CRYS_EC_ELGAMAL_ENCR_INVALID_RECEIVER_PUBL_KEY_PTR_ERROR
			CRYS_EC_ELGAMAL_RECEIVER_PUBL_KEY_VALIDATION_TAG_ERROR
			CRYS_EC_ELGAMAL_ENCR_INVALID_DOMAIN_ID_ERROR
			CRYS_EC_ELGAMAL_ENCR_INVALID_MESSAGE_DATA_IN_PTR_ERROR
			CRYS_EC_ELGAMAL_ENCR_INVALID_MESSAGE_DATA_IN_SIZE_ERROR						 
			CRYS_EC_ELGAMAL_ENCR_INVALID_ENCR_MESSAGE_OUT_PTR_ERROR
			CRYS_ECPKI_ELGAM_ENCR_INVALID_ENCR_MESSAGE_OUT_SIZE_PTR_ERROR
			CRYS_ECPKI_ELGAM_ENCR_INVALID_ENCR_MESSAGE_OUT_SIZE_ERROR
			CRYS_EC_ELGAMAL_ENCR_INVALID_TEMP_DATA_BUFFER_PTR_ERROR                         						 						 
			CRYS_EC_ELGAMAL_ENCR_INVALID_EPHEMERAL_KEY_DATA_ERROR
			CRYS_EC_ELGAMAL_ENCR_INVALID_EPHEMERAL_PRIV_KEY_SIZE_ERROR						 
*/  
CIMPORT_C CRYSError_t  _DX_ECPKI_ELGAMAL_Encrypt (
				CRYS_ECPKI_UserPublKey_t       *ReceiverUserPublKey_ptr, /*in*/
				DxUint8_t                      *MessageIn_ptr,           /*in*/
				DxUint32_t                      MessageInSizeBytes,      /*in*/
				DxUint8_t   		       *EncrMessageOut_ptr,      /*out*/
				DxUint32_t   		       *EncrMessOutSize_ptr,     /*in,out*/
				DxInt8_t                        IsEphemerKeyInternal,    /*in*/
				DxUint8_t                      *EphemerPrivKeyIn_ptr,    /*in*/ 
				DxUint32_t                      EphemerPrivKeySizeBytes, /*in*/ 
				CRYS_EC_ELGAMAL_TempData_t     *TempData_ptr,            /*in*/
				DxBool_t                        isExactMessageInSize );  /*in*/


/*****************************************************************************************
 *		       CRYS_ECPKI_ELGAMAL_Encrypt function	
 ****************************************************************************************/
/**
   @brief  The CRYS_ECDSA_Sign_Finish is the macro definition for calling the
           _DX_ECDSA_SignFinish function with internal generation of ephemeral keys.
           The function may change any high bits of the message buffer. 

	   The macro calls said function with following arguments as constant: 
	   IsEphemerKeyInternal = 1 , EphemerKeyData_ptr = DX_NULL, EphemerPrivKeySizeBytes = 0,
	   isExactMessageInSize = FALSE.
*/
#define CRYS_ECPKI_ELGAMAL_Encrypt(ReceiverUserPublKey_ptr,MessageIn_ptr,MessageInSizeBytes,EncrMessageOut_ptr,EncrMessageOutSize_ptr,TempData_ptr ) \
	_DX_ECPKI_ELGAMAL_Encrypt((ReceiverUserPublKey_ptr),(MessageIn_ptr),(MessageInSizeBytes),(EncrMessageOut_ptr),(EncrMessageOutSize_ptr),DX_TRUE,DX_NULL,0,(TempData_ptr), DX_FALSE)

/*****************************************************************************************
 *		       CRYS_ECPKI_ELGAMAL_EncryptExact function	
 ****************************************************************************************/
/**
   @brief  The CRYS_ECDSA_Sign_Finish is the macro definition for calling the
           _DX_ECDSA_SignFinish function with internal generation of ephemeral keys and
	   exact message size which is suitable for Elgamal encryption without changes of any bits.

	   The macro calls said function with following arguments as constant: 
	   IsEphemerKeyInternal = 1 , EphemerKeyData_ptr = DX_NULL, EphemerPrivKeySizeBytes = 0.
*/
#define CRYS_ECPKI_ELGAMAL_EncryptExact(ReceiverUserPublKey_ptr,MessageIn_ptr,MessageInSizeBytes,EncrMessageOut_ptr,EncrMessageOutSize_ptr,TempData_ptr) \
	_DX_ECPKI_ELGAMAL_Encrypt((ReceiverUserPublKey_ptr),(MessageIn_ptr),(MessageInSizeBytes),(EncrMessageOut_ptr),(EncrMessageOutSize_ptr),DX_TRUE,DX_NULL,0,(TempData_ptr), DX_TRUE)

/*************************************************************************************
 *		       _DX_ECPKI_ELGAMAL_Decrypt function	
 *************************************************************************************/
/**
  @brief:  This function performs decryption of the encrypted message (transmitted key or any other 
        text) with private key of the receiver using EC Elgamal algorithm. 
 
        The Decrypting  primitive uses the following data as input:
	       - The EC Domain parameters (received through DomainID included in ReceiverUserPrivKey).
	       - The  message, which includes two EC points (V, B). The message must be in big endianness
	         sequence.

  <b>Operation:</b> The receiver decrypts the message by the following sequence of steps:
	1.  Checks validity of input parameters (partially).
	2.  Converts incoming message (V, B) to little endian form.
	3.  Calls the LLF_ECPKI_ELGAMAL_Decrypt function, which performs decryption
	    according to Elgamal algorithm (used in WMDRM), which performs all decryption operations.
	4.  Converts decrypted message to big endian form.
	5.  Outputs the decrypted message as byte string of receiver passed length.   

	\Note: 1. The maximal size of decrypted message, which may be derived right from encrypted message (V, B), is: 
        	  If isExactMessageInSize = FALSE:
                       - (size of EC modulus - 1)  - for case, that MSByte of modulus >= 100 :
                       - (size of EC modulus - 2)  - for case, that MSByte of modulus < 100.
	                 The size of the decrypted message provided by receiver must be agreed with the sender 
			 (i.e. in both encrypt and decrypt functions the size of the plain message must be the same value).
		  If isExactMessageInSize = TRUE: the size is equal to modulus size.
			       
               2. EC domain parameters q, a, b, r, and G must be valid and associated with each other. 
 
   @param[in] ReceiverUserPrivKey_ptr    - A pointer to a receiver private key structure
   			                   (in affine coordinates).    				
   @param[in] EncrMessageIn_ptr          - The user passed pointer to the encrypted message 
                                           buffer.
   @param[in] EncrMessageSizeInBytes     - The size of input encrypted message. Must be equal to 
                                           4*(EC modulus size in bytes)   
   @param[out] DecrMessageOut_ptr        - The user passed pointer to buffer for output of  
   			                   decrypted message.
   @param[in/out] DecrMessageOutSize_ptr - A pointer to required size of decrypted message outpt:  
					   initial input value of the size must be not great, than 
					   described in the note 1 above.
   @param[in] TempData_ptr               - A pointer to structure, containing temporary buffers of specified 
                                           type.
   @psram[in] isExactMessageInSize       - A flag, defining is the message given with exact size and suitable 
					   for Elgamal encryption without changes any bits.

   @return CRYSError_t - CRYS_OK,
			CRYS_EC_ELGAMAL_DECR_INVALID_RECEIVER_PRIV_KEY_PTR_ERROR
			CRYS_EC_ELGAMAL_RECEIVER_PRIV_KEY_VALIDATION_TAG_ERROR
			CRYS_EC_ELGAMAL_DECR_INVALID_DOMAIN_ID_ERROR						   
			CRYS_EC_ELGAMAL_DECR_INVALID_ENCR_MESSAGE_IN_PTR_ERROR 
			CRYS_EC_ELGAMAL_DECR_INVALID_ENCR_MESSAGE_IN_SIZE_ERROR
			CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_PTR_ERROR
			CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_SIZE_PTR_ERROR
			CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_SIZE_ERROR
			CRYS_EC_ELGAMAL_DECR_INVALID_TEMP_DATA_BUFFER_PTR_ERROR
						 
*/
CIMPORT_C CRYSError_t  _DX_ECPKI_ELGAMAL_Decrypt (			      
				CRYS_ECPKI_UserPrivKey_t   *ReceiverUserPrivKey_ptr,  /*in*/	
				DxUint8_t   		   *EncrMessageIn_ptr,        /*in*/
				DxUint32_t                  EncrMessageSizeInBytes,   /*in*/
				DxUint8_t		   *DecrMessageOut_ptr,       /*out*/
				DxUint32_t                 *DecrMessageOutSize_ptr,   /*in/out*/
				CRYS_EC_ELGAMAL_TempData_t *TempData_ptr,             /*in*/ 
				DxBool_t                    isExactMessageInSize );   /*in*/
/**********************************************************************************************************/

/*************************************************************************************
 *		       CRYS_ECPKI_ELGAMAL_Decrypt macro	
 *************************************************************************************/
/**
  @brief:  This macro intended for calling DX_ECPKI_ELGAMAL_Decrypt function for decryption
	   encrypted message with changing the non significant high bytes. 
	   The macro sets parameter isExactMessageInSize = FALSE.
           
	  For parameters and returned values description see description of called function above
	  for case isExactMessageInSize = FALSE.		 
*/
#define CRYS_ECPKI_ELGAMAL_Decrypt(eceiverUserPrivKey_ptr,  EncrMessageIn_ptr, EncrMessageSizeInBytes, \
				   DecrMessageOut_ptr, DecrMessageOutSize_ptr, TempData_ptr) \
        _DX_ECPKI_ELGAMAL_Decrypt((eceiverUserPrivKey_ptr), (EncrMessageIn_ptr), (EncrMessageSizeInBytes), \
				   (DecrMessageOut_ptr), (DecrMessageOutSize_ptr), (TempData_ptr), DX_FALSE) 


/*************************************************************************************
 *		       CRYS_ECPKI_ELGAMAL_DecryptExact macro	
 *************************************************************************************/
/**
  @brief:  This macro intended for calling DX_ECPKI_ELGAMAL_Decrypt function for decryption
	   encrypted message without auxiliary non significant (changed) high bytes. 
	   The macro sets parameter isExactMessageInSize = TRUE.
           
	  For parameters and returned values description see description of called function above 
	  for case isExactMessageInSize = TRUE.		 
*/
#define CRYS_ECPKI_ELGAMAL_DecryptExact(eceiverUserPrivKey_ptr,  EncrMessageIn_ptr, EncrMessageSizeInBytes, \
				   DecrMessageOut_ptr, DecrMessageOutSize_ptr, TempData_ptr) \
        _DX_ECPKI_ELGAMAL_Decrypt((eceiverUserPrivKey_ptr), (EncrMessageIn_ptr), (EncrMessageSizeInBytes), \
				   (DecrMessageOut_ptr), (DecrMessageOutSize_ptr), (TempData_ptr), DX_TRUE) 

/**********************************************************************************************************/


#ifdef __cplusplus
}
#endif			

#endif

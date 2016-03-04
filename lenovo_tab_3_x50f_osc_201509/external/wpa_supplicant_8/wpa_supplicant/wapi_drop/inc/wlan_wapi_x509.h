/*
* Copyright (c) 2012 Qualcomm Atheros, Inc.
* All Rights Reserved.
* Qualcomm Atheros Confidential and Proprietary.
*/

#ifndef __WLAN_WAPI_X509_H_
#define __WLAN_WAPI_X509_H_

/*==============================================================
WLAN_WAPI_X509.H

DESCRIPTION

EXTERNALIZED FUNCTIONS


=======================================================================*/

/*=======================================================================

			EDIT HISTORY FOR FILE

$Header:  $
$Author:  $ $DateTime:  $

when        who     what, where, why
--------    ---     --------------------------------------------------

=======================================================================*/

#include "wlan_wapi_structure.h"

typedef struct asn1_st
{
	int length;
	unsigned char *data;
}WAPI_ASN1_INTEGER, WAPI_ASN1_BIT_STRING, WAPI_ASN1_TIME, WAPI_X509_NAME1;


typedef struct _st_x509_tmp2
{
	WAPI_ASN1_BIT_STRING *public_key;
}*st_x509_tmp2;

typedef struct _st_x509_tmp3
{
	WAPI_ASN1_TIME *notBefore;
	WAPI_ASN1_TIME *notAfter;
}*st_x509_tmp3;

typedef struct _st_x509_tmp1
{
	WAPI_ASN1_INTEGER *serialNumber;
	st_x509_tmp2 key;
	st_x509_tmp3 validity;
	WAPI_X509_NAME1 *issuer;
	WAPI_X509_NAME1 *subject;
}*st_x509_tmp1;

typedef struct wapi_x509_st
{
	st_x509_tmp1 cert_info;
}wapi_X509;


wapi_X509* d2i_wapi_X509(wapi_X509 **a, const unsigned char **in, long len);
int i2d_wapi_X509_NAME(WAPI_X509_NAME1 *a, unsigned char **out);
WAPI_X509_NAME1* X509_get_subject_name_wapi(wapi_X509 *a);
WAPI_X509_NAME1* X509_get_issuer_name_wapi(wapi_X509 *a);
WAPI_ASN1_INTEGER* X509_get_serialNumber_wapi(wapi_X509 *a);
int i2d_wapi_ASN1_INTEGER(WAPI_ASN1_INTEGER *a, unsigned char **out);


#endif		/*__WLAN_WAPI_X509_H_*/

/*
 *   Copyright(C) 2011-2018 Intel Corporation All Rights Reserved.
 *
 *   The source code, information  and  material ("Material") contained herein is
 *   owned  by Intel Corporation or its suppliers or licensors, and title to such
 *   Material remains  with Intel Corporation  or its suppliers or licensors. The
 *   Material  contains proprietary information  of  Intel or  its  suppliers and
 *   licensors. The  Material is protected by worldwide copyright laws and treaty
 *   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way  without Intel's  prior  express written  permission. No  license
 *   under  any patent, copyright  or  other intellectual property rights  in the
 *   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
 *   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
 *   intellectual  property  rights must  be express  and  approved  by  Intel in
 *   writing.
 *
 *   *Third Party trademarks are the property of their respective owners.
 *
 *   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
 *   this  notice or  any other notice embedded  in Materials by Intel or Intel's
 *   suppliers or licensors in any way.
 *
 */
#pragma once

#ifndef _SGX_ERROR_H_
#define _SGX_ERROR_H_

#define SGX_MK_ERROR(x)              (0x00000000|(x))

typedef enum _status_t
{
    SGX_SUCCESS                  = SGX_MK_ERROR(0x0000),

    SGX_ERROR_UNEXPECTED            = SGX_MK_ERROR(0x0001),      /* Unexpected error */
    SGX_ERROR_INVALID_PARAMETER     = SGX_MK_ERROR(0x0002),      /* The parameter is incorrect */
    SGX_ERROR_OUT_OF_MEMORY         = SGX_MK_ERROR(0x0003),      /* Not enough memory is available to complete this operation */
    SGX_ERROR_ENCLAVE_LOST          = SGX_MK_ERROR(0x0004),      /* Enclave lost after power transition or used in child process created by linux:fork() */
    SGX_ERROR_INVALID_STATE         = SGX_MK_ERROR(0x0005),      /* SGX API is invoked in incorrect order or state */
    SGX_ERROR_HYPERV_ENABLED        = SGX_MK_ERROR(0x0007),      /* Win10 platform with Hyper-V enabled */
    SGX_ERROR_FEATURE_NOT_SUPPORTED = SGX_MK_ERROR(0x0008),      /* Feature is not supported on this platform */

    SGX_ERROR_INVALID_FUNCTION   = SGX_MK_ERROR(0x1001),      /* The ecall/ocall index is invalid */
    SGX_ERROR_OUT_OF_TCS         = SGX_MK_ERROR(0x1003),      /* The enclave is out of TCS */
    SGX_ERROR_ENCLAVE_CRASHED    = SGX_MK_ERROR(0x1006),      /* The enclave is crashed */
    SGX_ERROR_ECALL_NOT_ALLOWED  = SGX_MK_ERROR(0x1007),      /* The ECALL is not allowed at this time, e.g. ecall is blocked by the dynamic entry table, or nested ecall is not allowed during initialization */
    SGX_ERROR_OCALL_NOT_ALLOWED  = SGX_MK_ERROR(0x1008),      /* The OCALL is not allowed at this time, e.g. ocall is not allowed during exception handling */

    SGX_ERROR_UNDEFINED_SYMBOL   = SGX_MK_ERROR(0x2000),      /* The enclave image has undefined symbol. */
    SGX_ERROR_INVALID_ENCLAVE    = SGX_MK_ERROR(0x2001),      /* The enclave image is not correct. */
    SGX_ERROR_INVALID_ENCLAVE_ID = SGX_MK_ERROR(0x2002),      /* The enclave id is invalid */
    SGX_ERROR_INVALID_SIGNATURE  = SGX_MK_ERROR(0x2003),      /* The signature is invalid */
    SGX_ERROR_NDEBUG_ENCLAVE     = SGX_MK_ERROR(0x2004),      /* The enclave is signed as product enclave, and can not be created as debuggable enclave. */
    SGX_ERROR_OUT_OF_EPC         = SGX_MK_ERROR(0x2005),      /* Not enough EPC is available to load the enclave */
    SGX_ERROR_NO_DEVICE          = SGX_MK_ERROR(0x2006),      /* Can't open SGX device */
    SGX_ERROR_MEMORY_MAP_CONFLICT= SGX_MK_ERROR(0x2007),      /* Page mapping failed in driver */
    SGX_ERROR_INVALID_METADATA   = SGX_MK_ERROR(0x2009),      /* The metadata is incorrect. */
    SGX_ERROR_DEVICE_BUSY        = SGX_MK_ERROR(0x200c),      /* Device is busy, mostly EINIT failed. */
    SGX_ERROR_INVALID_VERSION    = SGX_MK_ERROR(0x200d),      /* Metadata version is inconsistent between uRTS and sgx_sign or uRTS is incompatible with current platform. */
    SGX_ERROR_MODE_INCOMPATIBLE  = SGX_MK_ERROR(0x200e),      /* The target enclave 32/64 bit mode or sim/hw mode is incompatible with the mode of current uRTS. */
    SGX_ERROR_ENCLAVE_FILE_ACCESS = SGX_MK_ERROR(0x200f),     /* Can't open enclave file. */
    SGX_ERROR_INVALID_MISC        = SGX_MK_ERROR(0x2010),     /* The MiscSelct/MiscMask settings are not correct.*/
    SGX_ERROR_MEMORY_LOCKED       = SGX_MK_ERROR(0x2012),     /* Attempt to change system memory that should not be modified. */

    SGX_ERROR_MAC_MISMATCH       = SGX_MK_ERROR(0x3001),      /* Indicates verification error for reports, sealed datas, etc */
    SGX_ERROR_INVALID_ATTRIBUTE  = SGX_MK_ERROR(0x3002),      /* The enclave is not authorized */
    SGX_ERROR_INVALID_CPUSVN     = SGX_MK_ERROR(0x3003),      /* The cpu svn is beyond platform's cpu svn value */
    SGX_ERROR_INVALID_ISVSVN     = SGX_MK_ERROR(0x3004),      /* The SVN (ISVSVN or CONFIGSVN) is greater than the enclave's corresponding SVN */
    SGX_ERROR_INVALID_KEYNAME    = SGX_MK_ERROR(0x3005),      /* The key name is an unsupported value */

    SGX_ERROR_SERVICE_UNAVAILABLE       = SGX_MK_ERROR(0x4001),   /* Indicates aesm didn't respond or the requested service is not supported */
    SGX_ERROR_SERVICE_TIMEOUT           = SGX_MK_ERROR(0x4002),   /* The request to aesm timed out */
    SGX_ERROR_AE_INVALID_EPIDBLOB       = SGX_MK_ERROR(0x4003),   /* Indicates epid blob verification error */
    SGX_ERROR_SERVICE_INVALID_PRIVILEGE = SGX_MK_ERROR(0x4004),   /* Enclave has no privilege to get launch token */
    SGX_ERROR_EPID_MEMBER_REVOKED       = SGX_MK_ERROR(0x4005),   /* The EPID group membership is revoked. */
    SGX_ERROR_UPDATE_NEEDED             = SGX_MK_ERROR(0x4006),   /* SGX needs to be updated */
    SGX_ERROR_NETWORK_FAILURE           = SGX_MK_ERROR(0x4007),   /* Network connecting or proxy setting issue is encountered */
    SGX_ERROR_AE_SESSION_INVALID        = SGX_MK_ERROR(0x4008),   /* Session is invalid or ended by server */
    SGX_ERROR_BUSY                      = SGX_MK_ERROR(0x400a),   /* The requested service is temporarily not available */
    SGX_ERROR_MC_NOT_FOUND              = SGX_MK_ERROR(0x400c),   /* The Monotonic Counter doesn't exist or has been invalided */
    SGX_ERROR_MC_NO_ACCESS_RIGHT        = SGX_MK_ERROR(0x400d),   /* Caller doesn't have the access right to specified VMC */
    SGX_ERROR_MC_USED_UP                = SGX_MK_ERROR(0x400e),   /* Monotonic counters are used out */
    SGX_ERROR_MC_OVER_QUOTA             = SGX_MK_ERROR(0x400f),   /* Monotonic counters exceeds quota limitation */
    SGX_ERROR_KDF_MISMATCH              = SGX_MK_ERROR(0x4011),   /* Key derivation function doesn't match during key exchange */
    SGX_ERROR_UNRECOGNIZED_PLATFORM     = SGX_MK_ERROR(0x4012),   /* EPID Provisioning failed due to platform not recognized by backend server*/
    SGX_ERROR_SM_SERVICE_CLOSED            = SGX_MK_ERROR(0x4013),/* The secure message service instance was closed */
    SGX_ERROR_SM_SERVICE_UNAVAILABLE       = SGX_MK_ERROR(0x4014),/* The secure message service applet doesn't have existing session */
    SGX_ERROR_SM_SERVICE_UNCAUGHT_EXCEPTION= SGX_MK_ERROR(0x4015),/* The secure message service instance was terminated with an uncaught exception */
    SGX_ERROR_SM_SERVICE_RESPONSE_OVERFLOW = SGX_MK_ERROR(0x4016),/* The response data of the service applet is too much */
    SGX_ERROR_SM_SERVICE_INTERNAL_ERROR    = SGX_MK_ERROR(0x4017),/* The secure message service got an internal error */
    SGX_ERROR_UNSUPPORTED_CONFIG           = SGX_MK_ERROR(0x4018),/* The config for trigging EPID Provisiong or PSE Provisiong&LTP is invalid*/

    SGX_ERROR_NO_PRIVILEGE              = SGX_MK_ERROR(0x5002),   /* Not enough privilege to perform the operation */

    SGX_ERROR_PCL_ENCRYPTED                 = SGX_MK_ERROR(0x6001), /* trying to load an encrypted enclave without PCL feature requested */
	SGX_ERROR_PCL_NOT_ENCRYPTED             = SGX_MK_ERROR(0x6002), /* trying to load a plain enclave with PCL feature requested */

	/* SGX errors are only used in the file API when there is no appropriate EXXX (EINVAL, EIO etc.) error code */
    SGX_ERROR_FILE_BAD_STATUS               = SGX_MK_ERROR(0x7001),	/* The file is in bad status, run sgx_clearerr to try and fix it */
    SGX_ERROR_FILE_NO_KEY_ID                = SGX_MK_ERROR(0x7002),	/* The Key ID field is all zeros, can't re-generate the encryption key */
    SGX_ERROR_FILE_NAME_MISMATCH            = SGX_MK_ERROR(0x7003),	/* The current file name is different then the original file name (not allowed, substitution attack) */
    SGX_ERROR_FILE_NOT_SGX_FILE             = SGX_MK_ERROR(0x7004), /* The file is not an SGX file */
    SGX_ERROR_FILE_CANT_OPEN_RECOVERY_FILE  = SGX_MK_ERROR(0x7005),	/* A recovery file can't be opened, so flush operation can't continue (only used when no EXXX is returned)  */
    SGX_ERROR_FILE_CANT_WRITE_RECOVERY_FILE = SGX_MK_ERROR(0x7006), /* A recovery file can't be written, so flush operation can't continue (only used when no EXXX is returned)  */
    SGX_ERROR_FILE_RECOVERY_NEEDED          = SGX_MK_ERROR(0x7007),	/* When openeing the file, recovery is needed, but the recovery process failed */
    SGX_ERROR_FILE_FLUSH_FAILED             = SGX_MK_ERROR(0x7008),	/* fflush operation (to disk) failed (only used when no EXXX is returned) */
    SGX_ERROR_FILE_CLOSE_FAILED             = SGX_MK_ERROR(0x7009),	/* fclose operation (to disk) failed (only used when no EXXX is returned) */


    SGX_ERROR_UNSUPPORTED_ATT_KEY_ID        = SGX_MK_ERROR(0x8001),    /* platform quoting infrastructure does not support the key.*/
    SGX_ERROR_ATT_KEY_CERTIFICATION_FAILURE = SGX_MK_ERROR(0x8002),    /* Failed to generate and certify the attestation key.*/
    SGX_ERROR_ATT_KEY_UNINITIALIZED         = SGX_MK_ERROR(0x8003),    /* The platform quoting infrastructure does not have the attestation key available to generate quote.*/
    SGX_ERROR_INVALID_ATT_KEY_CERT_DATA     = SGX_MK_ERROR(0x8004),    /* TThe data returned by the platform library's sgx_get_quote_config() is invalid.*/
    SGX_ERROR_PLATFORM_CERT_UNAVAILABLE     = SGX_MK_ERROR(0x8005),    /* The PCK Cert for the platform is not available.*/
} sgx_status_t;

#endif

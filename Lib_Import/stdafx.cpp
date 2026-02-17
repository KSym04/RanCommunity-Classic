// stdafx.cpp : source file that includes just the standard includes
// Lib_Import.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
// #import "EGameEncrypt.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#if defined ( PH_PARAM ) || defined ( VN_PARAM )
IEGameEncryptPtr    m_pMyRossoEncrypto; //! Terra Decrypt Module
#else
IMyRossoEncryptPtr    m_pMyRossoEncrypto; //! Terra Decrypt Module
#endif

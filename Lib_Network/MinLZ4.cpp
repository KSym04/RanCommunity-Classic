/**
 * @file MinLZ4.cpp
 * @brief LZ4 compression library with LZO-compatible interface.
 * @note Optimized, bounds-safe implementation (drop-in replacement for CMinLzo)
 *
 * @author Eifelzocker
 */
#include "StdAfx.h"
#include "MinLZ4.h"

// LZ4 core header (path mirrors folder name 'Compression')
extern "C"
{
#include "../Compression/=ExternalLibraries/lz4/lz4.h"
}

// Link against prebuilt LZ4 static library
#pragma comment(lib, "../Compression/=ExternalLibraries/lz4/lz4.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMinLZ4::CMinLZ4()
    : m_bInit(false), m_totalCompressions(0), m_totalDecompressions(0), m_compressionErrors(0), m_decompressionErrors(0), m_totalBytesCompressed(0), m_totalBytesDecompressed(0), m_totalCompressionTime(0), m_totalDecompressionTime(0)
{
    ::InitializeCriticalSection(&m_CriticalSection);
}

CMinLZ4::~CMinLZ4()
{
    ::DeleteCriticalSection(&m_CriticalSection);
}

CMinLZ4 &CMinLZ4::GetInstance()
{
    static CMinLZ4 instance;
    if (!instance.m_bInit)
        instance.init();
    return instance;
}

int CMinLZ4::init()
{
    ::EnterCriticalSection(&m_CriticalSection);
    if (m_bInit)
    {
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_SUCCESS;
    }
    m_bInit = true;
    m_strError = "LZ4 compression system initialized";
    m_totalCompressions = m_totalDecompressions = 0;
    m_compressionErrors = m_decompressionErrors = 0;
    m_totalBytesCompressed = m_totalBytesDecompressed = 0;
    m_totalCompressionTime = m_totalDecompressionTime = 0;
    ::LeaveCriticalSection(&m_CriticalSection);
    return MINLZ4_SUCCESS;
}

bool CMinLZ4::validateParameters(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int nOutLength)
{
    if (!pInBuffer || !pOutBuffer)
    {
        m_strError = "NULL buffer pointer";
        return false;
    }
    if (nInLength <= 0 || nOutLength <= 0)
    {
        m_strError = "Invalid length";
        return false;
    }
    const int MAX_BUF = 16 * 1024 * 1024; // safety cap
    if (nInLength > MAX_BUF || nOutLength > MAX_BUF)
    {
        m_strError = "Buffer size limit";
        return false;
    }
    return true;
}

int CMinLZ4::lz4Compress(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int &nOutLength)
{
    if (!m_bInit)
    {
        m_strError = "LZ4 not initialized";
        return MINLZ4_ERROR;
    }
    if (!validateParameters(pInBuffer, nInLength, pOutBuffer, nOutLength))
        return MINLZ4_INPUT_DATA_ERROR;
    ::EnterCriticalSection(&m_CriticalSection);
    DWORD start = GetTickCount();
    m_totalCompressions++;
    int compressedSize = 0;
    try
    {
        compressedSize = LZ4_compress_default((const char *)pInBuffer, (char *)pOutBuffer, nInLength, nOutLength);
    }
    catch (...)
    {
        m_strError = "Compression exception";
        m_compressionErrors++;
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_INTERNAL_ERROR;
    }

    if (compressedSize <= 0)
    {
        m_strError = "Compression failed";
        m_compressionErrors++;
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_INTERNAL_ERROR;
    }
    if (compressedSize >= nInLength)
    {
        m_strError = "Data not compressible";
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_CAN_NOT_COMPRESS;
    }

    nOutLength = compressedSize;
    m_totalBytesCompressed += nInLength;
    m_totalCompressionTime += (GetTickCount() - start);
    ::LeaveCriticalSection(&m_CriticalSection);
    return MINLZ4_SUCCESS;
}

int CMinLZ4::lz4DeCompress(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int &nOutLength)
{
    if (!m_bInit)
    {
        m_strError = "LZ4 not initialized";
        return MINLZ4_ERROR;
    }
    if (!validateParameters(pInBuffer, nInLength, pOutBuffer, nOutLength))
        return MINLZ4_INPUT_DATA_ERROR;
    ::EnterCriticalSection(&m_CriticalSection);
    DWORD start = GetTickCount();
    m_totalDecompressions++;

    int decompressedSize = 0;
    try
    {
        decompressedSize = LZ4_decompress_safe((const char *)pInBuffer, (char *)pOutBuffer, nInLength, nOutLength);
    }
    catch (...)
    {
        m_strError = "Decompression exception";
        m_decompressionErrors++;
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_INTERNAL_ERROR;
    }

    if (decompressedSize < 0)
    {
        m_strError = "Decompression failed";
        m_decompressionErrors++;
        ::LeaveCriticalSection(&m_CriticalSection);
        return MINLZ4_ERROR;
    }

    nOutLength = decompressedSize; // actual size
    m_totalBytesDecompressed += nInLength;
    m_totalDecompressionTime += (GetTickCount() - start);
    ::LeaveCriticalSection(&m_CriticalSection);
    return MINLZ4_SUCCESS;
}

std::string &CMinLZ4::getErrorString() { return m_strError; }

void CMinLZ4::getStatistics(unsigned int &totalCompressions, unsigned int &totalDecompressions, unsigned int &compressionErrors, unsigned int &decompressionErrors)
{
    ::EnterCriticalSection(&m_CriticalSection);
    totalCompressions = m_totalCompressions;
    totalDecompressions = m_totalDecompressions;
    compressionErrors = m_compressionErrors;
    decompressionErrors = m_decompressionErrors;
    ::LeaveCriticalSection(&m_CriticalSection);
}

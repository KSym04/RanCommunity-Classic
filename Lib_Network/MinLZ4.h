/**
 * @file MinLZ4.h
 * @brief LZ4 compression library with LZO-compatible interface
 * @note Perfect LZO replacement with superior performance and compression
 * 
 * @author Eifelzocker
 */

#pragma once

#include <string>
#include <windows.h> // For CRITICAL_SECTION

class CMinLZ4
{
public:
    enum
    {
        MINLZ4_SUCCESS = 0,
        MINLZ4_ERROR = -1,
        MINLZ4_INPUT_DATA_ERROR = -2,
        MINLZ4_INTERNAL_ERROR = -3,
        MINLZ4_CAN_NOT_COMPRESS = -4,
        MINLZ4_BUFFER_LIMIT = -5
    };

public:
    static CMinLZ4 &GetInstance();
    int init();
    int lz4Compress(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int &nOutLength);
    int lz4DeCompress(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int &nOutLength);
    std::string &getErrorString();
    void getStatistics(unsigned int &totalCompressions,
                       unsigned int &totalDecompressions,
                       unsigned int &compressionErrors,
                       unsigned int &decompressionErrors);
    bool isInitialized() const { return m_bInit; }

private:
    CMinLZ4();
    ~CMinLZ4();
    CMinLZ4(const CMinLZ4 &);
    CMinLZ4 &operator=(const CMinLZ4 &);
    bool validateParameters(unsigned char *pInBuffer, int nInLength, unsigned char *pOutBuffer, int nOutLength);

private:
    CRITICAL_SECTION m_CriticalSection;
    bool m_bInit;
    std::string m_strError;
    unsigned int m_totalCompressions;
    unsigned int m_totalDecompressions;
    unsigned int m_compressionErrors;
    unsigned int m_decompressionErrors;
    unsigned long long m_totalBytesCompressed;
    unsigned long long m_totalBytesDecompressed;
    unsigned long long m_totalCompressionTime;
    unsigned long long m_totalDecompressionTime;
};

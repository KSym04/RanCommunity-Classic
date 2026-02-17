/**
 * @file MinLZ4.h
 * @brief LZ4 compression library with LZO-compatible interface
 * @note Perfect LZO replacement with superior performance and compression
 * @author LEGEND++ (RAN Community Architecture Team)
 * @date December 2024
 * @version 3.0 - PRODUCTION LZ4 IMPLEMENTATION
 * 
 * CRITICAL IMPLEMENTATION NOTES:
 * - Maintains EXACT LZO parameter order for seamless replacement
 * - Uses LZ4_compress_default() and LZ4_decompress_safe() internally
 * - Thread-safe design with critical sections
 * - Superior compression ratio and speed compared to LZO
 * - Drop-in replacement for CMinLzo - no protocol changes needed
 * 
 * PARAMETER ORDER COMPATIBILITY:
 * - Compression: (input, inputSize, output, outputSize)
 * - Decompression: (input, inputSize, output, outputSize) 
 * 
 * ERROR CODES (LZO-compatible):
 * - MINLZ4_SUCCESS (0): Operation successful
 * - MINLZ4_ERROR (-1): General error
 * - MINLZ4_INPUT_DATA_ERROR (-2): Invalid input parameters
 * - MINLZ4_CAN_NOT_COMPRESS (-4): Input not compressible (size increase)
 */

#pragma once

#include <string>
#include <windows.h>  // For CRITICAL_SECTION

/**
 * @class CMinLZ4
 * @brief High-performance LZ4 compression with LZO-compatible interface
 * @note Singleton pattern for memory efficiency and thread safety
 */
class CMinLZ4 {
public:
    // Error codes - EXACT LZO compatibility
    enum {
        MINLZ4_SUCCESS = 0,              // Operation successful
        MINLZ4_ERROR = -1,               // General error  
        MINLZ4_INPUT_DATA_ERROR = -2,    // Invalid input parameters
        MINLZ4_INTERNAL_ERROR = -3,      // Internal compression error
        MINLZ4_CAN_NOT_COMPRESS = -4,    // Data expands when compressed
        MINLZ4_BUFFER_LIMIT = -5         // Buffer size limit exceeded
    };

public:
    /**
     * @brief Get singleton instance (thread-safe)
     * @return Reference to singleton instance
     * @note Automatically initializes on first access
     */
    static CMinLZ4& GetInstance();

    /**
     * @brief Initialize LZ4 compression system
     * @return MINLZ4_SUCCESS on success, MINLZ4_ERROR on failure
     * @note Thread-safe initialization with critical section
     */
    int init();

    /**
     * @brief LZ4 compression with LZO-compatible interface
     * @param pInBuffer Input buffer containing data to compress
     * @param nInLength Input data length in bytes
     * @param pOutBuffer Output buffer for compressed data
     * @param nOutLength [IN/OUT] Max output buffer size / actual compressed size
     * @return MINLZ4_SUCCESS on success, error code on failure
     * @note Thread-safe operation with critical section protection
     * @note Returns MINLZ4_CAN_NOT_COMPRESS if compression increases size
     */
    int lz4Compress(unsigned char* pInBuffer,
                    int nInLength,
                    unsigned char* pOutBuffer,
                    int& nOutLength);

    /**
     * @brief LZ4 decompression with LZO-compatible parameter order
     * @param pInBuffer Input buffer containing compressed data
     * @param nInLength Compressed data length in bytes
     * @param pOutBuffer Output buffer for decompressed data  
     * @param nOutLength [IN/OUT] Expected output size / actual decompressed size
     * @return MINLZ4_SUCCESS on success, error code on failure
     * @note Thread-safe operation with critical section protection
     * @note Uses LZ4_decompress_safe for buffer overflow protection
     */
    int lz4DeCompress(unsigned char* pInBuffer,
                      int nInLength,
                      unsigned char* pOutBuffer,
                      int& nOutLength);

    /**
     * @brief Get detailed error message
     * @return Reference to error string with diagnostic information
     * @note Thread-safe access to error information
     */
    std::string& getErrorString();

    /**
     * @brief Get compression statistics
     * @param totalCompressions [OUT] Total compression operations
     * @param totalDecompressions [OUT] Total decompression operations
     * @param compressionErrors [OUT] Total compression errors
     * @param decompressionErrors [OUT] Total decompression errors
     * @note Thread-safe statistics access
     */
    void getStatistics(unsigned int& totalCompressions,
                       unsigned int& totalDecompressions,
                       unsigned int& compressionErrors,
                       unsigned int& decompressionErrors);

    /**
     * @brief Check if LZ4 system is initialized
     * @return true if initialized, false otherwise
     * @note Thread-safe state check
     */
    bool isInitialized() const { return m_bInit; }

private:
    // Private constructor/destructor for singleton pattern
    CMinLZ4();
    ~CMinLZ4();

    // Prevent copying and assignment
    CMinLZ4(const CMinLZ4&);
    CMinLZ4& operator=(const CMinLZ4&);

    /**
     * @brief Validate input parameters for compression/decompression
     * @param pInBuffer Input buffer pointer
     * @param nInLength Input data length
     * @param pOutBuffer Output buffer pointer
     * @param nOutLength Output buffer size
     * @return true if parameters are valid, false otherwise
     */
    bool validateParameters(unsigned char* pInBuffer,
                           int nInLength,
                           unsigned char* pOutBuffer,
                           int nOutLength);

private:
    // Thread safety
    CRITICAL_SECTION m_CriticalSection;  ///< Critical section for thread safety
    
    // State management
    bool m_bInit;                        ///< Initialization state flag
    std::string m_strError;              ///< Detailed error message string

    // Performance statistics (thread-safe access via critical section)
    unsigned int m_totalCompressions;    ///< Total compression operations performed
    unsigned int m_totalDecompressions;  ///< Total decompression operations performed
    unsigned int m_compressionErrors;    ///< Total compression error count
    unsigned int m_decompressionErrors;  ///< Total decompression error count
    
    // Performance tracking
    unsigned long long m_totalBytesCompressed;   ///< Total input bytes compressed
    unsigned long long m_totalBytesDecompressed; ///< Total input bytes decompressed
    unsigned long long m_totalCompressionTime;   ///< Total compression time (ms)
    unsigned long long m_totalDecompressionTime; ///< Total decompression time (ms)
}; 
/**
 * @file RcvMsgBuffer.h
 * @brief Secure message buffer header for server
 * @author Eifelzocker, MrNoName
 * @copyright Ran Community
 * @note Refactored based on _GSSource reference implementation
 */

#pragma once

#include "SendMsgBuffer.h"
#include <windows.h>  // For BYTE, DWORD, CRITICAL_SECTION

/**
 * @class CRcvMsgBuffer
 * @brief Secure receive message buffer implementation
 * 
 * Updated based on _GSSource reference implementation:
 * - Removed garbage data processing (security vulnerability)
 * - Simplified message handling
 * - Enhanced thread safety
 * - Improved error handling
 */
class CRcvMsgBuffer
{
public:
	/**
	 * @brief Constructor - Initialize secure message buffer
	 */
	CRcvMsgBuffer();
	~CRcvMsgBuffer(void);

	/**
	 * @brief Add received message to buffer with validation
	 * @param pMsg Message data pointer
	 * @param nSize Message size in bytes
	 * @return Buffer size on success, BUFFER_ERROR on failure
	 */
	int addRcvMsg(void* pMsg, int nSize);
	
	/**
	 * @brief Process and return one message from buffer
	 * @param szClientIP Client IP address for security logging (optional)
	 * @return Pointer to message on success, NULL on failure
	 */
	void* getMsg(const char* szClientIP = "CLIENT_GAME_EXE");

	/**
	 * @brief Reset message buffer position
	 */
	void resetPosition();

	/**
	 * @brief Get current receive buffer size
	 * @return Current buffer size in bytes
	 */
	int getRcvSize();

	/**
	 * @brief Reset all buffers
	 */
	void reset();

protected:
	/**
	 * @brief Extract one message from buffer
	 * @return Pointer to extracted message, NULL on failure
	 */
	void* getOneMsg();

protected:
	BYTE* m_pRcvBuffer; ///< Receive buffer for incoming network data
	BYTE* m_pOneMsg; ///< Buffer for single extracted message
	BYTE* m_pDecompressBuffer; ///< Temporary decompression buffer
	int m_nRcvSize; ///< Current size of received data
	DWORD m_dwSuccessfulPackets; ///< Counter for successful packet processing (auto-protection)
	CRITICAL_SECTION m_CriticalSection; ///< Thread synchronization object
};
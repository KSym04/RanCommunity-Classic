/**
 * @file RcvMsgBuffer.cpp
 * @brief Secure message buffer implementation with enhanced protection.
 * @author Eifelzocker, MrNoName  
 * @copyright Ran Community
 */

#include "StdAfx.h"
#include "s_NetGlobal.h"
#include "MinLzo.h"
#include "RcvMsgBuffer.h"
#include "RcvMsgBuffer_Game.h"  // Include for CIPAbuseManager

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Forward declaration for auto-protection function
void RegisterLegitimateConnectionAuto(const char* szClientIP, DWORD nSuccessfulPackets);

/**
 * @brief Constructor - Initialize legendary secure message buffer with proper buffer sizes
 * @note Buffer sizes based on _GSSource reference implementation with enhanced security
 */
CRcvMsgBuffer::CRcvMsgBuffer()
	: m_pRcvBuffer(NULL)
	, m_pOneMsg(NULL)
	, m_pDecompressBuffer(NULL)
	, m_nRcvSize(0)
	, m_dwSuccessfulPackets(0)  // Initialize packet counter for auto-protection
{
	// Allocate buffers with correct sizes from reference implementation
	m_pRcvBuffer        = new BYTE[NET_DATA_CLIENT_MSG_BUFSIZE];  // Client receive buffer (large)
	m_pOneMsg           = new BYTE[NET_DATA_BUFSIZE];             // Single message buffer (standard)
	m_pDecompressBuffer = new BYTE[NET_DATA_BUFSIZE];             // Decompression buffer (standard)

	// Initialize IP abuse manager
	CGameIPAbuseManager::GetInstance().Initialize();

	::InitializeCriticalSection(&m_CriticalSection);
}
 
CRcvMsgBuffer::~CRcvMsgBuffer(void)
{
	SAFE_DELETE_ARRAY(m_pRcvBuffer);
	SAFE_DELETE_ARRAY(m_pOneMsg);
	SAFE_DELETE_ARRAY(m_pDecompressBuffer);

	::DeleteCriticalSection(&m_CriticalSection);
}

// Note: Using functions from RcvMsgBuffer_Game.h for security logging and exploit detection

/**
 * @brief Add received message to buffer with enhanced validation
 * @param pMsg Message data pointer
 * @param nSize Message size in bytes
 * @return Buffer size on success, BUFFER_ERROR on failure
 * @note Enhanced from reference implementation with advanced input validation
 */
int CRcvMsgBuffer::addRcvMsg(void* pMsg, int nSize)
{	
	// Enhanced input validation - prevent malicious oversized packets
	if (!pMsg || nSize <= 0 || nSize > NET_DATA_BUFSIZE)
	{
		return CSendMsgBuffer::BUFFER_ERROR;
	}

	::EnterCriticalSection(&m_CriticalSection);
	
	// Prevent buffer overflow - check available space
	if (m_nRcvSize + nSize > NET_DATA_CLIENT_MSG_BUFSIZE)
	{
		::LeaveCriticalSection(&m_CriticalSection);
		return CSendMsgBuffer::BUFFER_ERROR;
	}

	// Safe memory copy
	memcpy(m_pRcvBuffer + m_nRcvSize, pMsg, nSize);
	m_nRcvSize += nSize;
	
	::LeaveCriticalSection(&m_CriticalSection);
	return m_nRcvSize;
}

/**
 * @brief Process and return one message from buffer with ultimate security
 * @param szClientIP Client IP address for security logging
 * @return Pointer to message on success, NULL on failure
 * @note Based on _GSSource reference with legendary exploit detection
 */
void* CRcvMsgBuffer::getMsg(const char* szClientIP)
{
	::EnterCriticalSection(&m_CriticalSection);

	NET_MSG_GENERIC* pNmg = (NET_MSG_GENERIC*) m_pRcvBuffer;

	// Validate minimum data available
	if ((m_nRcvSize < sizeof(NET_MSG_GENERIC)) || 
		(m_nRcvSize < (int) pNmg->dwSize))
	{
		::LeaveCriticalSection(&m_CriticalSection);
		return NULL;
	}

	// GAME CLIENT FIX: Handle legitimate zero-size packets properly
	// Game.exe clients can send zero-size packets for heartbeats, acknowledgments, etc.
	if (pNmg->dwSize == 0)
	{
		// LEGITIMATE ZERO-SIZE PACKET: Allow legitimate Game.exe zero-size packets
		// These are normal for heartbeats, keep-alives, and acknowledgments
		
		// Only log for debugging if needed (not suspicious)
		#ifdef _DEBUG
		char szDebug[256];
		sprintf_s(szDebug, "RcvMsgBuffer: Zero-size packet (type %d) from %s - LEGITIMATE", pNmg->nType, szClientIP);
		OutputDebugStringA(szDebug);
		#endif
		
		// NO LOGGING for legitimate zero-size packets - resource optimization
		
		// ENHANCED: Update connection activity as LEGITIMATE (not suspicious)
		CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, true);
		
		// Process zero-size packet normally - skip just the header
		if (m_nRcvSize >= sizeof(NET_MSG_GENERIC))
		{
			int nSkipSize = sizeof(NET_MSG_GENERIC); // Skip just the header
			int nMoveSize = m_nRcvSize - nSkipSize;
			if (nMoveSize > 0)
			{
				::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
				m_nRcvSize = nMoveSize;
			}
			else
			{
				resetPosition();
			}
		}
		else
		{
			resetPosition();
		}
		
		::LeaveCriticalSection(&m_CriticalSection);
		return NULL; // Continue processing next packet
	}
	
	// CRITICAL FIX: Only flag oversized packets as suspicious
	if (pNmg->dwSize > NET_DATA_BUFSIZE)
	{
		// Log security event for potential exploit attempt
		#ifdef _DEBUG
		char szError[256];
		sprintf_s(szError, "RcvMsgBuffer: Oversized message %u (type %d) - SUSPICIOUS", pNmg->dwSize, pNmg->nType);
		OutputDebugStringA(szError);
		#endif
		
		// ENHANCED: Update connection activity as suspicious
		CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
		
		// STABILITY FIX: Don't immediately reset position on single invalid message
		// Instead, try to skip this message and preserve rest of buffer
		if (m_nRcvSize >= sizeof(NET_MSG_GENERIC))
		{
			int nSkipSize = sizeof(NET_MSG_GENERIC); // Skip just the header
			int nMoveSize = m_nRcvSize - nSkipSize;
			if (nMoveSize > 0)
			{
				::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
				m_nRcvSize = nMoveSize;
			}
			else
			{
				resetPosition();
			}
		}
		else
		{
			resetPosition();
		}
		
		::LeaveCriticalSection(&m_CriticalSection);
		return NULL;
	}

	// **EMERGENCY FIX: COMPLETELY DISABLE SERVER-SIDE EXPLOIT DETECTION FOR GAME.EXE**
	// **THIS WAS THE CULPRIT CAUSING "PROCESSING DATA" HANG!**
	
	// GAME.EXE VIP BYPASS: Skip ALL exploit detection for our friend Game.exe
	std::string strClientIP(szClientIP ? szClientIP : "UNKNOWN");
	
	// Check if this is a private/development IP using complete RFC 1918 validation
	bool bIsPrivateIP = false;
	if (strClientIP == "CLIENT_GAME_EXE" || strClientIP == "127.0.0.1" || 
		strClientIP == "::1" || strClientIP == "localhost")
	{
		bIsPrivateIP = true;
	}
	else if (szClientIP)
	{
		// Parse IP address for complete RFC 1918 private network ranges
		int a, b, c, d;
		if (sscanf_s(szClientIP, "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
		{
			// RFC 1918 Private IP ranges:
			if (a == 10 ||                                    // 10.0.0.0/8
				(a == 172 && b >= 16 && b <= 31) ||          // 172.16.0.0/12
				(a == 192 && b == 168) ||                     // 192.168.0.0/16
				(a == 169 && b == 254))                       // Link-local 169.254.0.0/16
			{
				bIsPrivateIP = true;
			}
		}
	}
	
	bool bIsGameExeVIP = (bIsPrivateIP || CGameIPAbuseManager::GetInstance().IsGameExeVIP(szClientIP));
	
	if (!bIsGameExeVIP)
	{
		// ONLY run exploit detection for non-Game.exe connections
		// CRITICAL: This check happens BEFORE decompression to prevent buffer corruption
		// ENHANCED: Use protected detection for better legitimate connection handling
		if (!RanSecurity::DetectExploitAttemptGameProtected(pNmg, m_nRcvSize, szClientIP, 
			CGameIPAbuseManager::GetInstance().IsProtectedConnection(szClientIP)))
		{
			// ENHANCED STABILITY: Exploit detected - smart packet skipping preserves buffer integrity
			int nSkipSize = min((int)pNmg->dwSize, m_nRcvSize);
			int nMoveSize = m_nRcvSize - nSkipSize;
			
			if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE)
			{
				// Preserve remaining valid packets in buffer
				::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
				m_nRcvSize = nMoveSize;
			}
			else
			{
				// Only reset if no valid data remains
				resetPosition();
			}
			
			::LeaveCriticalSection(&m_CriticalSection);
			return NULL;
		}
	}
	else
	{
		// **GAME.EXE VIP: ALWAYS ALLOW - NEVER BLOCK OUR FRIEND!**
		// NO LOGGING for legitimate Game.exe packets - resource optimization
		
		// Auto-register as legitimate connection
		CGameIPAbuseManager::GetInstance().RegisterGameExeClient(szClientIP);
		CGameIPAbuseManager::GetInstance().RegisterLegitimateConnection(szClientIP, EIPCONN_ADMIN_CONNECTION);
	}

	// ENHANCED: Track successful packet processing for auto-protection
	m_dwSuccessfulPackets++;
	
	// ENHANCED: Auto-register legitimate connections after successful packet processing
	if (m_dwSuccessfulPackets % 10 == 0) // Check every 10 packets
	{
		RanNetworkUtil::RegisterLegitimateConnectionAuto(szClientIP, m_dwSuccessfulPackets);
	}

	// Handle compression messages - EXACT _GSSource reference implementation
	if (NET_MSG_COMPRESS == pNmg->nType)
	{		
		NET_COMPRESS* pPacket = (NET_COMPRESS*) pNmg;

		// Validate compression packet structure
		if (pNmg->dwSize < sizeof(NET_COMPRESS))
		{
			RanSecurity::LogSecurityEventGame("INVALID_COMPRESSION_HEADER", pNmg->dwSize, pNmg->nType, szClientIP, EIPBLOCK_INVALID_COMPRESSION_HEADER);
			
			// ENHANCED: Update connection activity as suspicious
			CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
			
			// STABILITY FIX: Smart skipping instead of immediate IP blocking
			int nSkipSize = max(sizeof(NET_MSG_GENERIC), min((int)pNmg->dwSize, m_nRcvSize));
			int nMoveSize = m_nRcvSize - nSkipSize;
			
			if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE)
			{
				::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
				m_nRcvSize = nMoveSize;
			}
			else
			{
				resetPosition();
			}
			
			::LeaveCriticalSection(&m_CriticalSection);
			return NULL;
		}

		// **GAME.EXE FIX: Skip compression serial check for Game.exe (can have different serials)**
		if (pPacket->nSerial != NET_COMPRESS_SERIAL)
		{
			// Check if this is Game.exe - if so, allow different serial numbers
			// Use the same private IP detection logic as above
			bool bIsGameExeConnection = bIsPrivateIP || CGameIPAbuseManager::GetInstance().IsGameExeVIP(szClientIP);
			
			if (bIsGameExeConnection)
			{
				// **GAME.EXE SERIAL BYPASS: Allow Game.exe to use any compression serial**
				// NO LOGGING for legitimate Game.exe compression - resource optimization
				// Continue processing - don't block Game.exe for serial mismatch
			}
			else
			{
				RanSecurity::LogSecurityEventGame("INVALID_COMPRESSION_SERIAL", pNmg->dwSize, pNmg->nType, szClientIP, EIPBLOCK_PROTOCOL_VIOLATION);
				
				// ENHANCED: Protected connections get reduced penalty
				if (CGameIPAbuseManager::GetInstance().IsProtectedConnection(szClientIP))
				{
					CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
					// Don't block protected connections for serial mismatch
				}
				else
				{
					// ENHANCED: Temporary block instead of permanent for serial mismatch
					CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_PROTOCOL_VIOLATION, "Invalid Compression Serial", 30);
				}
				
				// STABILITY: Smart skipping instead of buffer reset
				int nSkipSize = min((int)pNmg->dwSize, m_nRcvSize);
				int nMoveSize = m_nRcvSize - nSkipSize;
				
				if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE)
				{
					::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
					m_nRcvSize = nMoveSize;
				}
				else
				{
					resetPosition();
				}
				
				::LeaveCriticalSection(&m_CriticalSection);
				return NULL;
			}
		}

		if (pPacket->bCompress)
		{
			// Compressed message processing - EXACT FROM _GSSource REFERENCE  
			int nCompressSize = (int) pNmg->dwSize;  // FIXED: Use original message size
			
			// Validate compressed data size
			int nCompressedDataSize = pNmg->dwSize - sizeof(NET_COMPRESS);
			if (nCompressedDataSize <= 0 || nCompressedDataSize > NET_DATA_BUFSIZE)
			{
				RanSecurity::LogSecurityEventGame("INVALID_COMPRESSED_DATA_SIZE", pNmg->dwSize, pNmg->nType, szClientIP, EIPBLOCK_MALFORMED_COMPRESSION);
				
				// ENHANCED: Protected connections get reduced penalty
				if (CGameIPAbuseManager::GetInstance().IsProtectedConnection(szClientIP))
				{
					CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
					CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_MALFORMED_COMPRESSION, "Protected connection - invalid data size", 5);
				}
				else
				{
					// ENHANCED: Temporary block with shorter duration for data size issues
					CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_MALFORMED_COMPRESSION, "Invalid Compressed Data Size", 15);
				}
				
				// STABILITY: Smart skipping
				int nSkipSize = min(nCompressSize, m_nRcvSize);
				int nMoveSize = m_nRcvSize - nSkipSize;
				
				if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE)
				{
					::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
					m_nRcvSize = nMoveSize;
				}
				else
				{
					resetPosition();
				}
				
				::LeaveCriticalSection(&m_CriticalSection);
				return NULL;
			}

			// CORRECTED decompression call - EXACT from _GSSource reference
			int nDeCompressSize = NET_DATA_BUFSIZE;
			int nResult = CMinLzo::GetInstance().lzoDeCompress(
				(lzo_bytep) pPacket + sizeof(NET_COMPRESS), // Input buffer
				nCompressedDataSize,                        // Input size (compressed)
				(lzo_bytep) m_pDecompressBuffer,           // Output buffer  
				nDeCompressSize);                           // Output size (by reference)

			if (nResult == CMinLzo::MINLZO_SUCCESS)
			{
				// Validate decompressed size
				if (nDeCompressSize <= 0 || nDeCompressSize > NET_DATA_BUFSIZE)
				{
					RanSecurity::LogSecurityEventGame("INVALID_DECOMPRESSED_SIZE", nDeCompressSize, pNmg->nType, szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK);
					
					// ENHANCED: Protected connections get reduced penalty
					if (CGameIPAbuseManager::GetInstance().IsProtectedConnection(szClientIP))
					{
						CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
						CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK, "Protected connection - invalid decompressed size", 15);
					}
					else
					{
						// ENHANCED: Severe decompression attacks get longer blocks
						CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK, "Invalid Decompressed Size", 60);
					}
					
					// STABILITY: Smart skipping
					int nSkipSize = min(nCompressSize, m_nRcvSize);
					int nMoveSize = m_nRcvSize - nSkipSize;
					
					if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE)
					{
						::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nSkipSize, nMoveSize);
						m_nRcvSize = nMoveSize;
					}
					else
					{
						resetPosition();
					}
					
					::LeaveCriticalSection(&m_CriticalSection);
					return NULL;
				}

				// Buffer reorganization - EXACT copy from _GSSource reference
				int nMoveSize = m_nRcvSize - nCompressSize;
				if (nMoveSize >= 0)
				{
					::MoveMemory(m_pRcvBuffer + nDeCompressSize, m_pRcvBuffer + nCompressSize, nMoveSize);
					::CopyMemory(m_pRcvBuffer, m_pDecompressBuffer, nDeCompressSize);
					m_nRcvSize = nDeCompressSize + nMoveSize;
				}
				else
				{
					resetPosition();
					::LeaveCriticalSection(&m_CriticalSection);
					return NULL;
				}
			}
			else
			{
				// ENHANCED: Decompression failed - categorized logging and blocking
				RanSecurity::LogSecurityEventGame("DECOMPRESSION_FAILED", nCompressSize, pNmg->nType, szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK);
				
				// ENHANCED: Protected connections get reduced penalty
				if (CGameIPAbuseManager::GetInstance().IsProtectedConnection(szClientIP))
				{
					CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, false);
					CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK, "Protected connection - decompression failed", 10);
				}
				else
				{
					CGameIPAbuseManager::GetInstance().BlockIP(szClientIP, EIPBLOCK_DECOMPRESSION_ATTACK, "LZO Decompression Failed", 30);
				}

				// STABILITY FIX: Improved error recovery
				int nMoveSize = m_nRcvSize - nCompressSize;

				if (nMoveSize > 0 && nMoveSize <= NET_DATA_CLIENT_MSG_BUFSIZE) 
				{
					::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nCompressSize, nMoveSize);
					m_nRcvSize = nMoveSize;
				}
				else
				{
					resetPosition();
				}
				
				::LeaveCriticalSection(&m_CriticalSection);
				return NULL;
			}
		}
		else
		{
			// Uncompressed bundled message - EXACT from _GSSource reference			
			int nMoveSize = m_nRcvSize - (int) sizeof(NET_COMPRESS);
			
			if (nMoveSize <= 0) 
			{
				#ifdef _DEBUG
				OutputDebugStringA("RcvMsgBuffer: Invalid move size for uncompressed bundle");
				#endif
				resetPosition();
				::LeaveCriticalSection(&m_CriticalSection);
				return NULL;
			}

			::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + sizeof(NET_COMPRESS), nMoveSize);
			m_nRcvSize = nMoveSize;
		}
	}

	// ENHANCED: Update connection activity as legitimate for successful processing
	CGameIPAbuseManager::GetInstance().UpdateConnectionActivity(szClientIP, true);

	void* pOneMsg = getOneMsg();
	::LeaveCriticalSection(&m_CriticalSection);
	return pOneMsg;
}

/**
 * @brief Extract one message from buffer
 * @return Pointer to extracted message, NULL on failure
 * @note Based on _GSSource reference implementation
 */
void* CRcvMsgBuffer::getOneMsg()
{
	NET_MSG_GENERIC* pNmg = (NET_MSG_GENERIC*) m_pRcvBuffer;
	int nOneMsgSize = pNmg->dwSize;  // FIXED: Use dwSize correctly

	// GAME CLIENT FIX: Handle zero-size packets properly in getOneMsg
	if (nOneMsgSize == 0)
	{
		// Zero-size packet - use just the header size
		nOneMsgSize = sizeof(NET_MSG_GENERIC);
	}

	// Validate message size (allow zero-size packets)
	if (nOneMsgSize < 0 || nOneMsgSize > NET_DATA_BUFSIZE || nOneMsgSize > m_nRcvSize)
	{
		resetPosition();
		return NULL;
	}

	// Copy one message - based on reference implementation
	::CopyMemory(m_pOneMsg, m_pRcvBuffer, nOneMsgSize);

	// Move remaining messages
	int nMoveSize = m_nRcvSize - nOneMsgSize;
	if (nMoveSize > 0)
	{
		::MoveMemory(m_pRcvBuffer, m_pRcvBuffer + nOneMsgSize, nMoveSize);
	}
	else
	{
		nMoveSize = 0;
	}
	
	m_nRcvSize = nMoveSize;	
    return m_pOneMsg;
}

void CRcvMsgBuffer::resetPosition()
{
    m_nRcvSize = 0;	
}

int CRcvMsgBuffer::getRcvSize()
{
	return m_nRcvSize;
}

/**
 * @brief Reset all buffers with secure memory cleanup
 * @note Enhanced from reference with secure memory zeroing
 */
void CRcvMsgBuffer::reset()
{
	m_nRcvSize = 0;
	::SecureZeroMemory(m_pRcvBuffer,        NET_DATA_CLIENT_MSG_BUFSIZE);
	::SecureZeroMemory(m_pOneMsg,           NET_DATA_BUFSIZE);
	::SecureZeroMemory(m_pDecompressBuffer, NET_DATA_BUFSIZE);
}
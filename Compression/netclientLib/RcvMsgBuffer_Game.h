/**
 * @file RcvMsgBuffer_Game.h
 * @brief SIMPLIFIED VERSION: IP Abuse Management System for LZ4 migration preparation
 * @author Eifelzocker, MrNoName
 * @copyright Ran Community
 * @version LZ4_MIGRATION_SIMPLIFIED
 * @note SIMPLIFIED FOR LZ4 MIGRATION: All blocking conditions removed for clean integration
 * 
 * SIMPLIFIED FUNCTIONS BEHAVIOR:
 * - DetectExploitAttemptGame*(): Always allow packets (return true)
 * - CGameIPAbuseManager::BlockIP(): Only logs, never blocks
 * - CGameIPAbuseManager::IsIPBlocked(): Always returns false
 * - Authentication functions: Always return true (authenticated)
 * - Flood protection: Disabled (never blocks)
 * 
 * PURPOSE: Prepare clean foundation for LZ4 compression integration
 * 
 * @see MinLZ4.h for new LZ4 compression system interface
 * @see Document/Security/LZ4_MIGRATION_GUIDE.md for migration process
 */

#pragma once

#include "s_NetGlobal.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

/**
 * @enum EIP_BLOCK_REASON
 * @brief Enumeration for IP blocking reasons with severity levels
 */
enum EIP_BLOCK_REASON
{
	EIPBLOCK_NONE = 0,
	EIPBLOCK_LZO_DECOMPRESSION_HELL,     ///< Critical: LZO decompression attack (4096+ bytes)
	EIPBLOCK_BOUNDARY_RIDER,             ///< High: Boundary attack pattern (1024-2048 bytes)
	EIPBLOCK_INTEGER_OVERFLOW,           ///< Critical: Integer overflow exploit
	EIPBLOCK_MALFORMED_COMPRESSION,      ///< High: Malformed compression data
	EIPBLOCK_INVALID_COMPRESSION_HEADER, ///< Medium: Invalid compression packet structure
	EIPBLOCK_DECOMPRESSION_ATTACK,       ///< High: General decompression attack
	EIPBLOCK_MANUAL_ADMIN,               ///< Admin: Manual administrative block
	EIPBLOCK_PACKET_FLOODING,            ///< High: Packet flooding attack
	EIPBLOCK_PROTOCOL_VIOLATION,         ///< Medium: Network protocol violation
	EIPBLOCK_UNAUTHORIZED_APPLICATION,   ///< Critical: Third-party/unauthorized application detected
	EIPBLOCK_INVALID_CLIENT_SIGNATURE,   ///< Critical: Invalid client application signature
	EIPBLOCK_MISSING_AUTHENTICATION,     ///< High: Missing application authentication
	EIPBLOCK_TAMPERED_CLIENT,            ///< Critical: Client application has been tampered with
	EIPBLOCK_BOT_DETECTION,              ///< High: Automated bot behavior detected
	EIPBLOCK_MAX_REASON
};

/**
 * @enum EIP_CONNECTION_TYPE
 * @brief Connection type classification for protection levels
 */
enum EIP_CONNECTION_TYPE
{
	EIPCONN_UNKNOWN = 0,
	EIPCONN_LEGITIMATE_PLAYER,    ///< Verified legitimate player connection
	EIPCONN_ADMIN_CONNECTION,     ///< Administrative connection (highest protection)
	EIPCONN_SERVER_INTERNAL,      ///< Internal server-to-server communication
	EIPCONN_SUSPICIOUS,           ///< Suspicious but not yet blocked
	EIPCONN_CONFIRMED_ATTACKER    ///< Confirmed attacker (immediate block)
};

/**
 * @enum EAPP_TYPE
 * @brief Enumeration for authorized application types
 */
enum EAPP_TYPE
{
	EAPP_UNKNOWN = 0,
	EAPP_RAN_GAME_CLIENT,        ///< Official RAN Game Client
	EAPP_RAN_LAUNCHER,           ///< Official RAN Launcher
	EAPP_RAN_FIELD_SERVER,       ///< RAN Field Server
	EAPP_RAN_LOGIN_SERVER,       ///< RAN Login Server
	EAPP_RAN_SESSION_SERVER,     ///< RAN Session Server
	EAPP_RAN_AGENT_SERVER,       ///< RAN Agent Server
	EAPP_RAN_BOARD_SERVER,       ///< RAN Board Server
	EAPP_RAN_GAME_SERVER,        ///< RAN Game Server
	EAPP_RAN_TEST_SERVER,        ///< RAN Test Server
	EAPP_RAN_ADMIN_TOOL,         ///< Official RAN Admin Tools
	EAPP_RAN_PATCH_BUILDER,      ///< RAN Patch Builder Tool
	EAPP_RAN_VERSION_MANAGER,    ///< RAN Version Manager Tool
	EAPP_MAX_TYPE
};

/**
 * @struct SIP_BLOCK_INFO
 * @brief Information about a blocked IP address
 */
struct SIP_BLOCK_INFO
{
	std::string strReason;        ///< Human-readable blocking reason
	EIP_BLOCK_REASON eReason;     ///< Enumerated reason code
	SYSTEMTIME stBlockTime;       ///< Time when IP was blocked
	DWORD dwBlockDuration;        ///< Block duration in minutes (0 = permanent)
	DWORD dwExpireTime;           ///< Expiration time in GetTickCount() format (0 = permanent)
	DWORD dwAttemptCount;         ///< Number of exploit attempts from this IP
	DWORD dwFalsePositiveScore;   ///< Score for false positive detection (0-100)
	bool bVerifiedAttacker;       ///< True if confirmed as attacker through multiple patterns
	
	SIP_BLOCK_INFO() : eReason(EIPBLOCK_NONE), dwBlockDuration(0), dwExpireTime(0), 
					   dwAttemptCount(0), dwFalsePositiveScore(0), bVerifiedAttacker(false)
	{
		::ZeroMemory(&stBlockTime, sizeof(SYSTEMTIME));
	}
};

/**
 * @struct SIP_CONNECTION_INFO
 * @brief Information about legitimate connections for protection
 */
struct SIP_CONNECTION_INFO
{
	std::string strIP;
	EIP_CONNECTION_TYPE eType;
	SYSTEMTIME stFirstSeen;
	SYSTEMTIME stLastActivity;
	DWORD dwLegitimatePackets;
	DWORD dwSuspiciousPackets;
	bool bProtected;
	
	SIP_CONNECTION_INFO() : eType(EIPCONN_UNKNOWN), dwLegitimatePackets(0), 
							dwSuspiciousPackets(0), bProtected(false)
	{
		::ZeroMemory(&stFirstSeen, sizeof(SYSTEMTIME));
		::ZeroMemory(&stLastActivity, sizeof(SYSTEMTIME));
	}
};

/**
 * @struct SAPP_SIGNATURE
 * @brief Enhanced 5-signature application authentication system for Game.exe
 * @note Multiple signature layers prevent reverse engineering and tampering
 */
struct SAPP_SIGNATURE
{
	DWORD dwMagicNumber;         ///< Magic number for RAN Community applications
	DWORD dwAppType;             ///< Application type (EAPP_TYPE)
	DWORD dwVersionMajor;        ///< Major version number
	DWORD dwVersionMinor;        ///< Minor version number
	DWORD dwBuildNumber;         ///< Build number
	DWORD dwTimestamp;           ///< Compilation timestamp
	DWORD dwChecksum;            ///< Application checksum
	DWORD dwSecurityToken;       ///< Security token for verification
	
	// ENHANCED: 5-SIGNATURE PROTECTION SYSTEM
	DWORD dwGameSignature1;      ///< Game signature layer 1 (static)
	DWORD dwGameSignature2;      ///< Game signature layer 2 (version-based)
	DWORD dwGameSignature3;      ///< Game signature layer 3 (timestamp-based)
	DWORD dwGameSignature4;      ///< Game signature layer 4 (build-based)
	DWORD dwGameSignature5;      ///< Game signature layer 5 (dynamic hash)
	
	DWORD dwRotationHash;        ///< Signature rotation verification hash
	DWORD dwVersionHash;         ///< Version-specific hash
	DWORD dwBuildHash;           ///< Build-specific hash
	
	char szAppName[64];          ///< Application name string
	char szBuildSignature[128];  ///< Build signature hash
	char szVersionFingerprint[64]; ///< Version fingerprint for verification
	
	SAPP_SIGNATURE()
	{
		::ZeroMemory(this, sizeof(SAPP_SIGNATURE));
	}
};

/**
 * @struct SAPP_AUTH_INFO
 * @brief Application authentication information
 */
struct SAPP_AUTH_INFO
{
	std::string strIP;
	EAPP_TYPE eAppType;
	SYSTEMTIME stFirstAuth;
	SYSTEMTIME stLastAuth;
	DWORD dwAuthAttempts;
	DWORD dwFailedAttempts;
	bool bAuthenticated;
	bool bTrusted;
	SAPP_SIGNATURE signature;
	
	SAPP_AUTH_INFO() : eAppType(EAPP_UNKNOWN), dwAuthAttempts(0), 
					   dwFailedAttempts(0), bAuthenticated(false), bTrusted(false)
	{
		::ZeroMemory(&stFirstAuth, sizeof(SYSTEMTIME));
		::ZeroMemory(&stLastAuth, sizeof(SYSTEMTIME));
	}
};

// Application Authentication Constants
#define RAN_COMMUNITY_MAGIC_NUMBER    0x52414E43  // "RANC" in hex
#define RAN_SECURITY_TOKEN_BASE       0x12345678
#define RAN_MAX_AUTH_ATTEMPTS         3
#define RAN_AUTH_TIMEOUT_MINUTES      5

// ENHANCED: 5-SIGNATURE PROTECTION SYSTEM FOR GAME.EXE
// Multiple signature verification layers to prevent reverse engineering
#define RAN_GAME_SIGNATURE_1          0x47414D45  // "GAME" in hex
#define RAN_GAME_SIGNATURE_2          0x434C4E54  // "CLNT" in hex  
#define RAN_GAME_SIGNATURE_3          0x53454355  // "SECU" in hex
#define RAN_GAME_SIGNATURE_4          0x52495459  // "RITY" in hex
#define RAN_GAME_SIGNATURE_5          0x48415348  // "HASH" in hex

// Dynamic signature rotation keys (changes based on timestamp)
#define RAN_SIGNATURE_ROTATION_KEY_1  0x12AB34CD
#define RAN_SIGNATURE_ROTATION_KEY_2  0x56EF78AB  
#define RAN_SIGNATURE_ROTATION_KEY_3  0x9012CDEF
#define RAN_SIGNATURE_ROTATION_KEY_4  0x3456789A
#define RAN_SIGNATURE_ROTATION_KEY_5  0xBCDEF012

// Build signature salt (unique per build)
#define RAN_BUILD_SIGNATURE_SALT      0xDEADBEEF

// Version signature multipliers (change these for each version)
#define RAN_VERSION_SIG_MULTIPLIER_1  0x13579BDF
#define RAN_VERSION_SIG_MULTIPLIER_2  0x2468ACE0
#define RAN_VERSION_SIG_MULTIPLIER_3  0x369CF258
#define RAN_VERSION_SIG_MULTIPLIER_4  0x147AD036
#define RAN_VERSION_SIG_MULTIPLIER_5  0x258BE147

/**
 * @enum ESECURITY_MODE
 * @brief Security operation modes
 */
enum ESECURITY_MODE
{
	ESEC_MODE_PRODUCTION = 0,    ///< Production mode (full logging, strict blocking)
	ESEC_MODE_DEVELOPMENT = 1,   ///< Development mode (quiet mode for private IPs)
	ESEC_MODE_PENTEST = 2        ///< Pen-test mode (full analysis for specific IPs)
};

/**
 * @struct SSECURITY_CONFIG
 * @brief Dynamic security configuration
 */
struct SSECURITY_CONFIG
{
	ESECURITY_MODE eSecurityMode;
	std::unordered_set<std::string> setPentestIPs;
	std::unordered_set<std::string> setQuietIPs;
	std::unordered_set<std::string> setServerIPs;
	bool bAutoLoadServerIPs;
	std::string strServerConfigDir;
	bool bAutoAuthQuietIPs;
	bool bAutoAuthPentestIPs;
	bool bAutoAuthExternalIPs;
	int nFallbackAuthPacketCount;
	int nQuietIPLogLevel;
	int nPentestIPLogLevel;
	int nExternalIPLogLevel;
	int nMinorViolationBlock;
	int nMajorViolationBlock;
	int nCriticalViolationBlock;
	bool bEnableFalsePositiveDetection;
	int nFalsePositiveThreshold;
	int nMaxBlockedIPs;
	int nEmergencyCleanupThreshold;
	int nEmergencyCleanupInterval;
	int nMaxConnectionTracking;
	int nConnectionCleanupInterval;
	
	SSECURITY_CONFIG()
	{
		// Default values
		eSecurityMode = ESEC_MODE_DEVELOPMENT;
		bAutoLoadServerIPs = true;
		strServerConfigDir = "ServerConfigurations";
		bAutoAuthQuietIPs = true;
		bAutoAuthPentestIPs = false;
		bAutoAuthExternalIPs = false;
		nFallbackAuthPacketCount = 5;
		nQuietIPLogLevel = 1;
		nPentestIPLogLevel = 4;
		nExternalIPLogLevel = 3;
		nMinorViolationBlock = 15;
		nMajorViolationBlock = 60;
		nCriticalViolationBlock = 0;
		bEnableFalsePositiveDetection = true;
		nFalsePositiveThreshold = 30;
		nMaxBlockedIPs = 5000;
		nEmergencyCleanupThreshold = 2000;
		nEmergencyCleanupInterval = 30;
		nMaxConnectionTracking = 1500;
		nConnectionCleanupInterval = 120;
	}
};

// ENHANCED: Connection-level authentication constants
#define RAN_AUTH_HANDSHAKE_MAGIC      0x41555448  // "AUTH" in hex
#define RAN_AUTH_CHALLENGE_SIZE       32          // Challenge size in bytes
#define RAN_AUTH_RESPONSE_SIZE        64          // Response size in bytes
#define RAN_AUTH_TIMEOUT_SECONDS      10          // Connection auth timeout
#define RAN_SERVER_TOKEN_BASE         0x53525652  // "SRVR" server token base

/**
 * @enum EAUTH_HANDSHAKE_STATE
 * @brief Authentication handshake states
 */
enum EAUTH_HANDSHAKE_STATE
{
	EAUTH_STATE_NONE = 0,           ///< No authentication attempted
	EAUTH_STATE_CHALLENGE_SENT,     ///< Challenge sent to client
	EAUTH_STATE_RESPONSE_RECEIVED,  ///< Response received from client
	EAUTH_STATE_AUTHENTICATED,      ///< Successfully authenticated
	EAUTH_STATE_FAILED,             ///< Authentication failed
	EAUTH_STATE_SERVER_TRUSTED      ///< Server-to-server trusted connection
};

/**
 * @struct SAUTH_CHALLENGE
 * @brief Authentication challenge packet
 */
struct SAUTH_CHALLENGE
{
	DWORD dwMagic;                      ///< Magic number for validation
	DWORD dwChallenge[8];               ///< Random challenge data (32 bytes)
	DWORD dwTimestamp;                  ///< Challenge timestamp
	DWORD dwServerToken;                ///< Server authentication token
	
	SAUTH_CHALLENGE()
	{
		::ZeroMemory(this, sizeof(SAUTH_CHALLENGE));
		dwMagic = RAN_AUTH_HANDSHAKE_MAGIC;
		dwTimestamp = (DWORD)time(nullptr);
		dwServerToken = RAN_SERVER_TOKEN_BASE ^ dwTimestamp;
	}
};

/**
 * @struct SAUTH_RESPONSE
 * @brief Authentication response packet
 */
struct SAUTH_RESPONSE
{
	DWORD dwMagic;                      ///< Magic number for validation
	DWORD dwResponse[16];               ///< Computed response (64 bytes)
	SAPP_SIGNATURE signature;           ///< Application signature
	DWORD dwChecksum;                   ///< Response checksum
	
	SAUTH_RESPONSE()
	{
		::ZeroMemory(this, sizeof(SAUTH_RESPONSE));
		dwMagic = RAN_AUTH_HANDSHAKE_MAGIC;
	}
};

/**
 * @struct SAUTH_CONNECTION_INFO
 * @brief Per-connection authentication state
 */
struct SAUTH_CONNECTION_INFO
{
	std::string strIP;
	EAUTH_HANDSHAKE_STATE eState;
	SYSTEMTIME stConnectionStart;
	SYSTEMTIME stLastActivity;
	SAUTH_CHALLENGE challenge;
	EAPP_TYPE eAuthenticatedType;
	bool bServerToServer;
	DWORD dwFailedAttempts;
	
	SAUTH_CONNECTION_INFO() : eState(EAUTH_STATE_NONE), eAuthenticatedType(EAPP_UNKNOWN),
							  bServerToServer(false), dwFailedAttempts(0)
	{
		::ZeroMemory(&stConnectionStart, sizeof(SYSTEMTIME));
		::ZeroMemory(&stLastActivity, sizeof(SYSTEMTIME));
	}
};

/**
 * @class CGameIPAbuseManager
 * @brief Enhanced IP abuse detection and blocking system with self-healing capabilities for game clients
 * 
 * Advanced Features:
 * - Manages cfg/__abuse.cfg file for persistent IP blocking with atomic writes
 * - Automatically adds detected exploit attempts with categorization
 * - Provides real-time IP blocking functionality with temporary blocks
 * - Auto-creates cfg/ directory and config file if needed
 * - Supports timed blocks and permanent blocks
 * - Thread-safe operations with minimal locking overhead
 * - Detailed logging and statistics tracking
 * - Legitimate connection protection system
 * - Self-healing capabilities during attacks
 * - False positive detection and mitigation
 * 
 * @note Synchronized with server-side CIPAbuseManager for consistency
 * @warning Changes to this class affect both client and server packet processing
 */
class CGameIPAbuseManager
{
public:
	/**
	 * @brief Get singleton instance
	 * @return Reference to singleton instance
	 * @note Thread-safe singleton initialization
	 */
	static CGameIPAbuseManager& GetInstance();

	/**
	 * @brief Initialize abuse manager and load __abuse.cfg
	 * @return true on success, false on failure
	 * @note Must be called before any other operations
	 */
	bool Initialize();

	/**
	 * @brief Check if IP address is currently blocked
	 * @param szIP IP address to check
	 * @return true if IP is blocked, false otherwise
	 * @note Automatically removes expired temporary blocks
	 */
	bool IsIPBlocked(const char* szIP);

	/**
	 * @brief Add IP to abuse list with categorized blocking
	 * @param szIP IP address to block
	 * @param eReason Enumerated reason for blocking
	 * @param szCustomReason Custom reason string (optional)
	 * @param dwDurationMinutes Block duration in minutes (0 = permanent)
	 * @return true on success, false on failure
	 * @note Logs blocking action with timestamp and reason
	 */
	bool BlockIP(const char* szIP, EIP_BLOCK_REASON eReason, const char* szCustomReason = nullptr, DWORD dwDurationMinutes = 0);

	/**
	 * @brief Add IP to abuse list with string reason (legacy compatibility)
	 * @param szIP IP address to block
	 * @param szReason Reason for blocking
	 * @return true on success, false on failure
	 * @deprecated Use BlockIP with EIP_BLOCK_REASON instead
	 */
	bool BlockIP(const char* szIP, const char* szReason);

	/**
	 * @brief Remove IP from abuse list
	 * @param szIP IP address to unblock
	 * @return true on success, false on failure
	 */
	bool UnblockIP(const char* szIP);

	/**
	 * @brief Clear all blocked IPs (emergency use only)
	 * @return Number of IPs that were cleared
	 * @note Use with caution - removes all IP blocks
	 */
	int ClearAllBlocks();

	/**
	 * @brief Reload __abuse.cfg file with atomic operation
	 * @return true on success, false on failure
	 */
	bool ReloadConfig();

	/**
	 * @brief Get blocking information for an IP
	 * @param szIP IP address to query
	 * @param pBlockInfo Pointer to receive blocking information
	 * @return true if IP is blocked, false otherwise
	 */
	bool GetBlockInfo(const char* szIP, SIP_BLOCK_INFO* pBlockInfo);

	/**
	 * @brief Clean up expired temporary blocks
	 * @return Number of blocks removed
	 * @note Called automatically during IsIPBlocked() checks
	 */
	DWORD CleanupExpiredBlocks();

	/**
	 * @brief Get statistics about blocked IPs
	 * @param pTotalBlocked Pointer to receive total blocked count
	 * @param pPermanentBlocked Pointer to receive permanent blocks count
	 * @param pTemporaryBlocked Pointer to receive temporary blocks count
	 */
	void GetStatistics(DWORD* pTotalBlocked, DWORD* pPermanentBlocked, DWORD* pTemporaryBlocked);

	// ENHANCED SECURITY FEATURES

	/**
	 * @brief Register legitimate connection for protection
	 * @param szIP IP address to protect
	 * @param eType Connection type classification
	 * @return true on success, false on failure
	 * @note Protected IPs have higher thresholds for blocking
	 */
	bool RegisterLegitimateConnection(const char* szIP, EIP_CONNECTION_TYPE eType = EIPCONN_LEGITIMATE_PLAYER);

	/**
	 * @brief Check if IP is a protected legitimate connection
	 * @param szIP IP address to check
	 * @return true if protected, false otherwise
	 */
	bool IsProtectedConnection(const char* szIP);

	/**
	 * @brief Check if IP address is a server IP (for server-to-server connections)
	 * @param szIP IP address to check
	 * @return true if IP is a known server IP, false otherwise
	 * @note Used for automatic authentication of server-to-server connections
	 */
	bool IsServerIP(const char* szIP);

	/**
	 * @brief Update connection activity for legitimate tracking
	 * @param szIP IP address
	 * @param bLegitimatePacket true if packet was legitimate
	 * @return true on success, false on failure
	 */
	bool UpdateConnectionActivity(const char* szIP, bool bLegitimatePacket = true);

	/**
	 * @brief Trigger self-healing process during attack detection
	 * @param szAttackerIP Attacker IP address
	 * @return true if healing successful, false otherwise
	 * @note Preserves legitimate connections while isolating attackers
	 */
	bool TriggerSelfHealing(const char* szAttackerIP);

	/**
	 * @brief Validate IP for false positive detection
	 * @param szIP IP address to validate
	 * @param eReason Blocking reason
	 * @return true if should block, false if likely false positive
	 */
	bool ValidateBlockingDecision(const char* szIP, EIP_BLOCK_REASON eReason);

	/**
	 * @brief Save configuration with atomic write operation
	 * @return true on success, false on failure
	 * @note Uses temporary file and atomic rename for data integrity
	 */
	bool SaveConfigurationAtomic();

	// APPLICATION AUTHENTICATION SYSTEM

	/**
	 * @brief Authenticate application signature
	 * @param szIP IP address of connecting application
	 * @param pSignature Application signature to verify
	 * @return true if application is authorized, false otherwise
	 * @note Blocks unauthorized applications immediately
	 */
	bool AuthenticateApplication(const char* szIP, const SAPP_SIGNATURE* pSignature);

	// CONNECTION-LEVEL AUTHENTICATION SYSTEM (PROACTIVE SECURITY)

	/**
	 * @brief Initialize connection authentication for new connection
	 * @param szIP IP address of connecting client
	 * @param nClientID Client slot ID
	 * @return true if connection should be allowed to proceed, false to reject immediately
	 * @note This is called immediately upon connection acceptance
	 */
	bool InitializeConnectionAuth(const char* szIP, int nClientID);

	/**
	 * @brief Send authentication challenge to connecting client
	 * @param szIP IP address of client
	 * @param nClientID Client slot ID
	 * @return true if challenge sent successfully, false on error
	 */
	bool SendAuthenticationChallenge(const char* szIP, int nClientID);

	/**
	 * @brief Process authentication response from client
	 * @param szIP IP address of client
	 * @param pResponse Authentication response packet
	 * @return true if authentication successful, false if failed
	 */
	bool ProcessAuthenticationResponse(const char* szIP, const SAUTH_RESPONSE* pResponse);

	/**
	 * @brief Check if connection is authenticated and authorized for communication
	 * @param szIP IP address to check
	 * @param eRequiredType Required application type (optional)
	 * @return true if connection is authenticated, false otherwise
	 * @note This should be checked BEFORE processing any game packets
	 */
	bool IsConnectionAuthenticated(const char* szIP, EAPP_TYPE eRequiredType = EAPP_UNKNOWN);

	/**
	 * @brief Establish server-to-server trusted connection
	 * @param szIP IP address of server
	 * @param eServerType Type of server connecting
	 * @return true if server connection authorized, false otherwise
	 */
	bool EstablishServerConnection(const char* szIP, EAPP_TYPE eServerType);

	/**
	 * @brief Generate server authentication token for outgoing connections
	 * @param eServerType Type of this server
	 * @param pToken Buffer to receive token
	 * @return true on success, false on failure
	 */
	bool GenerateServerToken(EAPP_TYPE eServerType, DWORD* pToken);

	/**
	 * @brief Verify server authentication token from incoming server connection
	 * @param szIP IP address of connecting server
	 * @param dwToken Server token to verify
	 * @param eExpectedType Expected server type
	 * @return true if server token is valid, false otherwise
	 */
	bool VerifyServerToken(const char* szIP, DWORD dwToken, EAPP_TYPE eExpectedType);

	/**
	 * @brief Check if application is authenticated and authorized
	 * @param szIP IP address to check
	 * @param eRequiredAppType Required application type (optional)
	 * @return true if authenticated, false otherwise
	 */
	bool IsApplicationAuthenticated(const char* szIP, EAPP_TYPE eRequiredAppType = EAPP_UNKNOWN);

	/**
	 * @brief Generate application signature for current process
	 * @param eAppType Application type
	 * @param pSignature Pointer to receive generated signature
	 * @return true on success, false on failure
	 * @note Used by legitimate RAN applications to identify themselves
	 */
	bool GenerateApplicationSignature(EAPP_TYPE eAppType, SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Verify application signature integrity
	 * @param pSignature Signature to verify
	 * @return true if signature is valid, false otherwise
	 */
	bool VerifyApplicationSignature(const SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Register trusted application
	 * @param szIP IP address
	 * @param eAppType Application type
	 * @param pSignature Application signature
	 * @return true on success, false on failure
	 */
	bool RegisterTrustedApplication(const char* szIP, EAPP_TYPE eAppType, const SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Get application authentication information
	 * @param szIP IP address
	 * @param pAuthInfo Pointer to receive authentication info
	 * @return true if found, false otherwise
	 */
	bool GetApplicationAuthInfo(const char* szIP, SAPP_AUTH_INFO* pAuthInfo);

	/**
	 * @brief Clean up expired authentication entries
	 * @return Number of entries removed
	 */
	DWORD CleanupExpiredAuthentications();

	/**
	 * @brief Detect bot-like behavior patterns
	 * @param szIP IP address
	 * @param dwPacketInterval Interval between packets (ms)
	 * @param nPacketSize Packet size
	 * @return true if bot behavior detected, false otherwise
	 */
	bool DetectBotBehavior(const char* szIP, DWORD dwPacketInterval, int nPacketSize);

	/**
	 * @brief Check if connection is authenticated before processing any packets
	 * @param szClientIP IP address to check
	 * @param eRequiredType Required application type (optional)
	 * @return true if connection is authenticated, false to block
	 * @note Call this BEFORE processing ANY game packets
	 */
	bool IsConnectionProactivelyAuthenticated(const char* szClientIP, EAPP_TYPE eRequiredType = EAPP_UNKNOWN);

	/**
	 * @brief EMERGENCY: Fast authentication bypass to prevent deadlocks during flood attacks
	 * @param szClientIP IP address to check
	 * @return true if connection should be allowed, false to block
	 * @note This is a non-blocking version that prevents server deadlocks
	 */
	bool IsConnectionAuthenticatedFast(const char* szClientIP);

	// DYNAMIC CONFIGURATION SYSTEM

	/**
	 * @brief Load security configuration from cfg/__security_config.cfg
	 * @return true on success, false on failure
	 * @note Automatically loads server IPs from ServerConfigurations/*.cfg if enabled
	 */
	bool LoadSecurityConfiguration();

	/**
	 * @brief Reload security configuration (for runtime updates)
	 * @return true on success, false on failure
	 */
	bool ReloadSecurityConfiguration();

	/**
	 * @brief Get current security configuration
	 * @return Reference to current security configuration
	 */
	const SSECURITY_CONFIG& GetSecurityConfig() const { return m_securityConfig; }

	/**
	 * @brief Check if IP is a pen-test IP (should receive full logging)
	 * @param szIP IP address to check
	 * @return true if pen-test IP, false otherwise
	 */
	bool IsPentestIP(const char* szIP);

	/**
	 * @brief Check if IP is a quiet IP (should receive minimal logging)
	 * @param szIP IP address to check
	 * @return true if quiet IP, false otherwise
	 */
	bool IsQuietIP(const char* szIP);

	/**
	 * @brief Get appropriate log level for IP address
	 * @param szIP IP address
	 * @return Log level (0-4)
	 */
	int GetLogLevelForIP(const char* szIP);

	/**
	 * @brief Get appropriate block duration for violation type
	 * @param eReason Violation reason
	 * @return Block duration in minutes (0 = permanent)
	 */
	int GetBlockDurationForReason(EIP_BLOCK_REASON eReason);

	/**
	 * @brief Get string representation of block reason
	 * @param eReason Enumerated reason code  
	 * @return Human-readable reason string
	 */
	const char* GetReasonString(EIP_BLOCK_REASON eReason);

	/**
	 * @brief Register IP as confirmed Game.exe client with permanent VIP status
	 * @param szIP IP address to register as Game.exe
	 * @return true on success, false on failure
	 * @note Grants permanent VIP status - never banned, always trusted
	 */
	bool RegisterGameExeClient(const char* szIP);

	/**
	 * @brief Check if IP is a registered Game.exe VIP client
	 * @param szIP IP address to check
	 * @return true if confirmed Game.exe VIP, false otherwise
	 */
	bool IsGameExeVIP(const char* szIP);

	/**
	 * @brief EMERGENCY CLEAR ALL SECURITY STATES - Called on server startup
	 * @return true on success
	 * @note Clears all IP blocks, authentication states, and security restrictions
	 * @warning This ensures legitimate players can always login when server restarts
	 */
	bool EmergencyClearAllSecurityStates();

	/**
	 * @brief Clear all blocked IPs and reset security system
	 * @return Number of IPs cleared
	 * @note Called during server startup to allow all players to reconnect
	 */
	DWORD ClearAllIPBlocks();

	/**
	 * @brief Reset all authentication states for fresh server start
	 * @return Number of authentication states cleared
	 * @note Clears all connection authentication, application auth, etc.
	 */
	DWORD ResetAllAuthenticationStates();

	/**
	 * @brief Initialize fresh security state for server startup
	 * @return true on success
	 * @note Safe reset that preserves development IP whitelist but clears restrictions
	 */
	bool InitializeFreshSecurityForServerStart();

	/**
	 * @brief Load and validate security configuration from __security_config.cfg
	 * @return true if configuration loaded successfully
	 * @note Loads quiet mode settings, IP lists, and logging controls
	 */
	bool LoadAndValidateSecurityConfiguration();

	/**
	 * @brief Validate that __abuse.cfg is being respected for IP blocking
	 * @return true if abuse configuration is properly loaded and active
	 * @note Checks that blocked IPs from file are actually being enforced
	 */
	bool ValidateAbuseConfigurationActive();

	/**
	 * @brief Check if a specific log type should be written based on configuration
	 * @param logType Type of log (good_connection, auto_protection, etc.)
	 * @return true if this log type should be written
	 */
	bool ShouldWriteLogType(const char* logType);

	/**
	 * @brief Get current configuration validation status
	 * @return Detailed status of configuration loading and validation
	 */
	std::string GetConfigurationValidationStatus();

	/**
	 * @brief Initialize clean security state for fresh server session
	 * @return true on success
	 * @note Called automatically by ResetServerSecurityOnStartup()
	 */
	bool InitializeCleanSecurityForServerStart();

	/**
	 * @brief Test and validate configuration loading and abuse system
	 * @return Detailed validation report as string
	 * @note Use this to verify that __security_config.cfg and __abuse.cfg are working
	 * 
	 * Usage example:
	 * @code
	 * std::string report = ValidateSecurityConfiguration();
	 * std::cout << report << std::endl;
	 * @endcode
	 */
	std::string ValidateSecurityConfiguration();

	/**
	 * @brief Test IP blocking functionality with validation
	 * @param szTestIP IP to test blocking with (use a safe test IP)
	 * @return true if blocking system works correctly
	 * @note This will temporarily block and unblock the test IP to verify functionality
	 */
	bool TestIPBlockingFunctionality(const char* szTestIP = "192.168.200.200");

private:
	CGameIPAbuseManager() = default;
	~CGameIPAbuseManager() = default;
	
	CGameIPAbuseManager(const CGameIPAbuseManager&) = delete;
	CGameIPAbuseManager& operator=(const CGameIPAbuseManager&) = delete;

	/**
	 * @brief Load blocked IPs from __abuse.cfg with enhanced parsing
	 * @return true on success, false on failure
	 */
	bool LoadBlockedIPs();

	/**
	 * @brief Save blocked IPs to __abuse.cfg with metadata
	 * @return true on success, false on failure
	 */
	bool SaveBlockedIPs();

	/**
	 * @brief Check if a temporary block has expired
	 * @param blockInfo Block information to check
	 * @return true if expired, false if still valid
	 */
	bool IsBlockExpired(const SIP_BLOCK_INFO& blockInfo);

	/**
	 * @brief Check if IP is in private/local range (should not be blocked)
	 * @param szIP IP address to check
	 * @return true if private/local IP, false otherwise
	 */
	bool IsPrivateOrLocalIP(const char* szIP);

	/**
	 * @brief Calculate false positive score for blocking decision
	 * @param szIP IP address
	 * @param eReason Blocking reason
	 * @return Score from 0-100 (higher = more likely false positive)
	 */
	DWORD CalculateFalsePositiveScore(const char* szIP, EIP_BLOCK_REASON eReason);

	/**
	 * @brief Load server IPs from configuration files
	 * @return Number of server IPs loaded
	 */
	int LoadServerIPsFromConfigs();

	/**
	 * @brief Parse configuration line
	 * @param strLine Configuration line to parse
	 * @param strKey Key to extract
	 * @param strValue Value to extract
	 * @return true if line was parsed successfully
	 */
	bool ParseConfigLine(const std::string& strLine, std::string& strKey, std::string& strValue);

private:
	std::unordered_map<std::string, SIP_BLOCK_INFO> m_mapBlockedIPs; ///< Map of blocked IP addresses with metadata
	std::unordered_map<std::string, SIP_CONNECTION_INFO> m_mapLegitimateConnections; ///< Map of legitimate connections for protection
	std::unordered_map<std::string, SAPP_AUTH_INFO> m_mapAuthenticatedApps; ///< Map of authenticated applications
	std::unordered_map<std::string, SAUTH_CONNECTION_INFO> m_mapConnectionAuth; ///< Map of connection authentication states
	std::unordered_set<std::string> m_setTrustedServers; ///< Set of trusted server IPs
	std::unordered_set<std::string> m_setGameExeClients; ///< Set of confirmed Game.exe VIP clients
	CRITICAL_SECTION m_CriticalSection; ///< Thread synchronization
	bool m_bInitialized; ///< Initialization flag
	DWORD m_dwLastCleanupTime; ///< Last cleanup time for expired blocks
	DWORD m_dwSelfHealingTriggers; ///< Count of self-healing activations
	DWORD m_dwLastAuthCleanup; ///< Last authentication cleanup time
	DWORD m_dwLastConnectionCleanup; ///< Last connection authentication cleanup time
	SSECURITY_CONFIG m_securityConfig; ///< Dynamic security configuration
	DWORD m_dwLastCleanup; ///< Last cleanup timestamp

	// Configuration settings loaded from __security_config.cfg
	bool m_bGoodConnectionLog;      ///< Enable good connection logging
	bool m_bAutoProtectionLog;      ///< Enable auto-protection logging  
	bool m_bConfigLoadingLog;       ///< Enable configuration loading logging
	bool m_bGameExeVipLog;          ///< Enable Game.exe VIP logging
	bool m_bLegitimateConnectionsLog; ///< Enable legitimate connections logging
	bool m_bAttackLog;              ///< Enable attack logging (always true)
	bool m_bIpBlockingLog;          ///< Enable IP blocking logging (always true)
	bool m_bConfigValidation;       ///< Enable configuration validation
	bool m_bConfigReloadCheck;      ///< Enable periodic config reload checks
	DWORD m_dwConfigReloadInterval; ///< Config reload check interval (seconds)
	DWORD m_dwLastConfigCheck;      ///< Last configuration check timestamp
	std::string m_strConfigStatus;  ///< Current configuration status message
};

// Forward declarations
struct NET_MSG_GENERIC;

/**
 * @brief Log system events (non-security) to console and system log
 * @param szEvent Event name
 * @param szComponent Component name (System, Server, etc.)
 * @param szDetails Event details
 * @note This function logs system events to a separate log file, not security alerts
 */
void LogSystemEventToConsole(const char* szEvent, const char* szComponent, const char* szDetails);

/**
 * @namespace RanSecurity
 * @brief Core security functions for RAN Community game protection
 */
namespace RanSecurity
{
	/**
	 * @brief Log security events for game protection system
	 * @param szEvent Event description
	 * @param dwSize Data size involved in event
	 * @param nType Event type classification
	 * @param szClientIP Client IP address (default: "Server")
	 * @param eReason Block reason if applicable (default: EIPBLOCK_NONE)
	 */
	void LogSecurityEventGame(const char* szEvent, DWORD dwSize, int nType, const char* szClientIP = "Server", EIP_BLOCK_REASON eReason = EIPBLOCK_NONE);

	/**
	 * @brief Detect exploit attempts in game packets (SIMPLIFIED for LZ4 migration)
	 * @param pNmg Network message generic structure
	 * @param nAvailableSize Available buffer size
	 * @param szClientIP Client IP address (default: "Server")
	 * @return true (always allows packets in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool DetectExploitAttemptGame(NET_MSG_GENERIC* pNmg, int nAvailableSize, const char* szClientIP = "Server");

	/**
	 * @brief Detect exploit attempts with protected connection awareness (SIMPLIFIED)
	 * @param pNmg Network message generic structure
	 * @param nAvailableSize Available buffer size
	 * @param szClientIP Client IP address
	 * @param bProtectedConnection Whether connection is protected (default: false)
	 * @return true (always allows packets in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool DetectExploitAttemptGameProtected(NET_MSG_GENERIC* pNmg, int nAvailableSize, const char* szClientIP, bool bProtectedConnection = false);

	/**
	 * @brief Block unauthorized applications attempting to connect
	 * @param szClientIP Client IP address to block
	 * @param szReason Human-readable reason for blocking
	 * @return true if blocking was processed (logs only in simplified mode)
	 * @note SIMPLIFIED: Only logs, never actually blocks in LZ4 migration mode
	 */
	bool BlockUnauthorizedApplication(const char* szClientIP, const char* szReason);

	/**
	 * @brief Drop attacker connection with logging
	 * @param szClientIP Client IP address to drop
	 * @param szReason Human-readable reason for dropping
	 * @return true if drop was processed (logs only in simplified mode)
	 * @note SIMPLIFIED: Only logs, never actually drops in LZ4 migration mode
	 */
	bool DropAttackerConnection(const char* szClientIP, const char* szReason);

	/**
	 * @brief Detect automated bot behavior patterns
	 * @param szClientIP Client IP address
	 * @param dwPacketInterval Packet timing interval
	 * @param nPacketSize Packet size
	 * @param nPacketType Packet type identifier
	 * @return false (never detects bots in simplified mode)
	 * @note SIMPLIFIED: Always returns false for LZ4 migration preparation
	 */
	bool DetectAutomatedBotBehavior(const char* szClientIP, DWORD dwPacketInterval, int nPacketSize, int nPacketType);

	/**
	 * @brief Check if packet matches Game.exe patterns
	 * @param pNmg Network message generic structure
	 * @param nAvailableSize Available buffer size
	 * @return true (always matches in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool IsGameExePacketPattern(NET_MSG_GENERIC* pNmg, int nAvailableSize);
}

/**
 * @namespace RanAuthentication
 * @brief Application authentication and authorization functions
 */
namespace RanAuthentication
{
	/**
	 * @brief Authenticate connecting applications (SIMPLIFIED)
	 * @param szClientIP Client IP address
	 * @param pSignature Application signature structure
	 * @return true (always authenticates in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool AuthenticateConnectingApplication(const char* szClientIP, const SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Generate RAN application signature
	 * @param eAppType Application type to generate signature for
	 * @param pSignature Output signature structure
	 * @return true if signature generated successfully
	 */
	bool GenerateRanApplicationSignature(EAPP_TYPE eAppType, SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Verify RAN application signature (SIMPLIFIED)
	 * @param pSignature Signature structure to verify
	 * @return true (always verifies in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool VerifyRanApplicationSignature(const SAPP_SIGNATURE* pSignature);

	/**
	 * @brief Initialize proactive connection authentication (SIMPLIFIED)
	 * @param szClientIP Client IP address
	 * @param nClientID Client connection ID
	 * @return true (always initializes in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool InitializeProactiveConnectionAuth(const char* szClientIP, int nClientID);

	/**
	 * @brief Establish trusted server connection (SIMPLIFIED)
	 * @param szServerIP Server IP address
	 * @param eServerType Server type classification
	 * @return true (always establishes in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool EstablishTrustedServerConnection(const char* szServerIP, EAPP_TYPE eServerType);

	/**
	 * @brief Process client authentication response (SIMPLIFIED)
	 * @param szClientIP Client IP address
	 * @param pResponse Authentication response structure
	 * @return true (always processes in simplified mode)
	 * @note SIMPLIFIED: Always returns true for LZ4 migration preparation
	 */
	bool ProcessClientAuthenticationResponse(const char* szClientIP, const SAUTH_RESPONSE* pResponse);

	/**
	 * @brief Get human-readable application type name
	 * @param eAppType Application type enumeration
	 * @return String representation of application type
	 */
	const char* GetApplicationTypeName(EAPP_TYPE eAppType);
}

/**
 * @namespace RanNetworkUtil
 * @brief Network utility and validation functions
 */
namespace RanNetworkUtil
{
	/**
	 * @brief Validate IP address format and structure
	 * @param szIP IP address string to validate
	 * @return true if IP address is valid format
	 */
	bool ValidateIPAddress(const char* szIP);

	/**
	 * @brief Register legitimate connection automatically (SIMPLIFIED)
	 * @param szClientIP Client IP address
	 * @param nSuccessfulPackets Number of successful packets processed
	 * @note SIMPLIFIED: Only logs registration, no actual protection in LZ4 migration mode
	 */
	void RegisterLegitimateConnectionAuto(const char* szClientIP, DWORD nSuccessfulPackets);
}

/**
 * @namespace RanSystemUtil
 * @brief System utility functions for server operations
 */
namespace RanSystemUtil
{
	/**
	 * @brief Get formatted time string
	 * @param szBuffer Output buffer for formatted time
	 * @param nBufferSize Size of output buffer
	 * @param pSystemTime System time structure (default: current time)
	 * @return true if time was formatted successfully
	 */
	bool GetFormattedTime(char* szBuffer, int nBufferSize, const SYSTEMTIME* pSystemTime = nullptr);
}

/**
 * @namespace RanServerStartup
 * @brief Server startup initialization and reset functions
 */
namespace RanServerStartup
{
	/**
	 * @brief Reset server security systems on startup (SIMPLIFIED)
	 * @return true (always resets successfully in simplified mode)
	 * @note SIMPLIFIED: Only logs reset actions, minimal actual changes for LZ4 migration
	 * 
	 * @code
	 * // Usage in server startup:
	 * if (RanServerStartup::ResetServerSecurityOnStartup())
	 * {
	 *     // Security systems reset successfully
	 * }
	 * @endcode
	 */
	bool ResetServerSecurityOnStartup();

	/**
	 * @brief Clear all IP blocks on server start (SIMPLIFIED)
	 * @return 0 (no blocks cleared in simplified mode)
	 * @note SIMPLIFIED: Always returns 0 for LZ4 migration preparation
	 */
	DWORD ClearAllIPBlocksOnServerStart();

	/**
	 * @brief Reset authentication states on server start (SIMPLIFIED)
	 * @return 0 (no auths reset in simplified mode)
	 * @note SIMPLIFIED: Always returns 0 for LZ4 migration preparation
	 */
	DWORD ResetAuthenticationOnServerStart();

	/**
	 * @brief Initialize global security systems for server start (SIMPLIFIED)
	 * @return true (always initializes successfully in simplified mode)
	 * @note SIMPLIFIED: Only basic initialization for LZ4 migration preparation
	 */
	bool InitializeGlobalSecurityForServerStart();
}

/**
 * @class CFloodProtectionManager
 * @brief Thread-safe flood protection manager for packet rate limiting
 * @note LEGEND++ FIX: Solves critical thread safety vulnerabilities
 */
class CFloodProtectionManager
{
public:
	/**
	 * @brief Get singleton instance
	 * @return Reference to thread-safe singleton
	 */
	static CFloodProtectionManager& GetInstance();

	/**
	 * @brief Check if IP should be blocked for flooding
	 * @param strIP IP address to check
	 * @param dwCurrentTime Current timestamp
	 * @return true if should block, false if allowed
	 * @note Thread-safe packet counting and cleanup
	 */
	bool CheckFloodProtection(const std::string& strIP, DWORD dwCurrentTime);

	/**
	 * @brief Add IP to emergency blocked list during flood attacks
	 * @param strIP IP address to block
	 * @return true if added successfully
	 * @note Thread-safe emergency blocking for high contention scenarios
	 */
	bool AddEmergencyBlock(const std::string& strIP);

	/**
	 * @brief Check if IP is in emergency blocked list
	 * @param strIP IP address to check
	 * @return true if emergency blocked, false otherwise
	 */
	bool IsEmergencyBlocked(const std::string& strIP);

private:
	CFloodProtectionManager() { ::InitializeCriticalSection(&m_CriticalSection); }
	~CFloodProtectionManager() { ::DeleteCriticalSection(&m_CriticalSection); }
	
	CFloodProtectionManager(const CFloodProtectionManager&) = delete;
	CFloodProtectionManager& operator=(const CFloodProtectionManager&) = delete;

	/**
	 * @brief Cleanup old tracking entries
	 * @param dwCurrentTime Current timestamp
	 * @note Must be called within critical section
	 */
	void CleanupOldEntries(DWORD dwCurrentTime);

private:
	CRITICAL_SECTION m_CriticalSection; ///< Thread synchronization
	std::unordered_map<std::string, int> m_mapPacketCounts; ///< Packet count per IP
	std::unordered_map<std::string, DWORD> m_mapFirstPacketTime; ///< First packet time per IP
	std::unordered_set<std::string> m_setEmergencyBlockedIPs; ///< Emergency blocked IPs
	DWORD m_dwLastCleanup; ///< Last cleanup timestamp
};
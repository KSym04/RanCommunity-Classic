enum EMSTATUS
{
	EMSTATUS_FLOOD = 0,
	EMSTATUS_NOTFLOOD = 1,
	EMSTATUS_BAN = 2,
};

struct SCONNECT_IP
{
	DWORD	dwTime;
	DWORD	dwFlood;
	char	szIP[128];
	SCONNECT_IP()
	{
		dwTime = 0;
		dwFlood = 0;
		memset(szIP, 0, sizeof(char) * 128);
	}
};

struct SLIST_IP
{
	char	szIP[128];
	SLIST_IP()
	{
		memset(szIP, 0, sizeof(char) * 128);
	}
};

typedef std::map< DWORD, SCONNECT_IP >		SCONNECT_IP_MAP;
typedef SCONNECT_IP_MAP::iterator			SCONNECT_IP_MAP_ITER;

typedef std::vector<SCONNECT_IP>			SCONNECT_IP_VEC;
typedef SCONNECT_IP_VEC::iterator			SCONNECT_IP_VEC_ITER;

typedef std::vector<SLIST_IP>			SLIST_IP_VEC;
typedef SLIST_IP_VEC::iterator			SLIST_IP_VEC_ITER;

class CSensei_DLL
{
public:
	virtual DWORD GetClientCheckerTime() = 0;
	virtual void SetClientCheckerTimer(DWORD dwTime) = 0;

	virtual DWORD GetAntiFloodTime() = 0;
	virtual void SetAntiFloodTime(DWORD dwTime) = 0;

	virtual void SetClientCheck(bool bCheck) = 0;
	virtual void SetClientChecker(bool bChecker) = 0;

	virtual void SetAntiFlood(bool bFlood) = 0;
	virtual bool IsAntiFlood() = 0;

	virtual bool IsClientCheck() = 0;
	virtual bool IsClientChecker() = 0;

	virtual int IsFlooder(char* szIp, DWORD dwTime) = 0;

	virtual bool IsBlocked(char* szIP) = 0;
	virtual void BlockIP(char* szIP) = 0;
	virtual void DoBlocking() = 0;

	virtual HRESULT OneTimeInit() = 0;

	virtual int windows_system(const char *cmd) = 0;
	virtual int DoCount() = 0;

};
#pragma	once

struct	SITEMCUSTOM;
struct  SNATIVEID;

namespace	NS_ITEMINFO
{
	void	RESET ();

	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	void	LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
		const BOOL bNPCShop, const WORD wNPCShopType = 0, const BOOL bItemLink = FALSE, const BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE,
		BOOL bCurrencyShop = FALSE, DWORD dwCurrencyShopNpcID = 0, DWORD dwCurrencyShopItemID = 0, DWORD dwMarketChannel = 0 ); 

	void	LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom );
};

/* modern info tooltip, Juver, 2020/04/09 */
namespace	NS_ITEMINFO_EX
{
	void	RESET ();

	void	LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom_base );

	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	void	LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
		const BOOL bNPCShop, const WORD wNPCShopType = 0, const BOOL bItemLink = FALSE, const BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE,
		BOOL bCurrencyShop = FALSE, DWORD dwCurrencyShopNpcID = 0, DWORD dwCurrencyShopItemID = 0, DWORD dwMarketChannel = 0 ); 

};


struct	GLSKILL;
struct	SCHARSKILL;
struct	SNATIVEID;

namespace	NS_SKILLINFO
{
	void	RESET();
	void	LOAD(const SNATIVEID& sNativeID, const BOOL bNextLevel);

	/*multi skill Juver, 2018/09/05 */
	void	LOAD_SUB(const SNATIVEID& skill_main, const SNATIVEID& skill_additional, const BOOL next_level);

	/*extended buff info, EJCode, 2018/10/06 */
	void	LOAD_SIMPLE(const SNATIVEID& sNativeID, const WORD wLevel);
};

namespace	NS_SKILLINFO_EX
{
	void	RESET();
	void	LOAD(const SNATIVEID& sNativeID, const BOOL bNextLevel);

	/*multi skill Juver, 2018/09/05 */
	void	LOAD_SUB(const SNATIVEID& sNativeIDMain, const SNATIVEID& sNativeIDSub, const BOOL bNextLevel);

	/*extended buff info, EJCode, 2018/10/06 */
	void	LOAD_SIMPLE(const SNATIVEID& sNativeID, const WORD wLevel);
};
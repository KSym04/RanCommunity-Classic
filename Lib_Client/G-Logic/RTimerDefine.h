#ifndef RTIMERDEFINE_H__INCLUDED
#define RTIMERDEFINE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//timer default defines

#define RENDER_CHANGE_SETTING_DEFAULT		5.0f

#define PET_LAST_USE_TIME_DEFAULT			3.0f
#define PET_BLOCK_TIME_DEFAULT				3.0f

#define CURRENCY_SHOP_TIME_DEFAULT			1.0f

#define PERSONAL_LOCK_DELAY_TIME_DEFAULT	1.0f		/* personal lock system, Juver, 2019/12/06 */

/* party finder, Juver, 2020/01/03 */
#define PARTY_FINDER_SEARCH_DELAY_TIME_DEFAULT	30.0f		
#define PARTY_FINDER_JOIN_DELAY_TIME_DEFAULT	10.0f	

/* charinfoview, Juver, 2020/03/03 */
#define REQ_CHARINFO_DELAY_TIME_DEFAULT		5.0f

/* HWID watchlist, Juver, 2020/05/05 */
#define HWID_WATCHLIST_KICK_TIME			20.0f

#endif // RTIMERDEFINE_H__INCLUDED

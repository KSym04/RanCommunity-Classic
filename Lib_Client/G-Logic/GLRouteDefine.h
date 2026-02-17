/*!
 * \file GLRouteDefine.h
 *
 * \author Juver
 * \date 2020/10/24
 *
 * 
 */

#ifndef GLROUTEDEFINE_H_INCLUDED__
#define GLROUTEDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	EMMONEY_LOG				= 10000,			// 로그를 남길만한 금액변화.
	EMCLUBINCOMEMONEY_LOG	= 100000000,	// 클럽 수입 로그
};

// 아이템의 교환타입
enum EMITEM_ROUTE
{
	EMITEM_ROUTE_CHAR		= 0, // 캐릭터간 거래
	EMITEM_ROUTE_GROUND		= 1, // 바닥에 떨굼
	EMITEM_ROUTE_SHOP		= 2, // 상점에 판매,구입
	EMITEM_ROUTE_USERINVEN	= 3, // 사용자 락커에 넣기, 빼기
	EMITEM_ROUTE_DELETE		= 4, // 아이템 삭제
	EMITEM_ROUTE_CLUB		= 5, // 클럽 락커에 넣기, 빼기
	EMITEM_ROUTE_CLUBINCOME	= 6, // 선도클럽 세금 수입
	EMITEM_ROUTE_NPCREMAKE  = 7, // 돈을 지불하고 NPC 에게서 아이템 랜덤수치 개조
	EMITEM_ROUTE_PETCARD	= 8, // 팻카드의 사용여부
	EMITEM_ROUTE_ODDEVEN	= 9, // 홀짝게임
	EMITEM_ROUTE_VEHICLE	= 10, // 보드 생성 로그
	EMITEM_ROUTE_VNINVEN	= 11, // 베트남 탐닉 방지 인벤토리에서 돈을 꺼냈을 경우
	EMITEM_ROUTE_GARBAGE	= 12, // 휴지통으로 아이템 삭제시
	EMITEM_ROUTE_NPCCOME	= 13, // NPC 소환으로 생긴 수수료
	EMITEM_ROUTE_ITEMMIX	= 14, // 아이템 조합
	EMITEM_ROUTE_GATHERING	= 15, // 채집
	EMITEM_ROUTE_SYSTEM		= 16, // 시스템으로 일어난 로그

	EMITEM_ROUTE_SIZE		= 17,
};

enum EMVEHICLE_ACTION
{
	EMVEHICLE_ACTION_BATTERY_BEFORE	= 0, // 탈것 베터리 전
	EMVEHICLE_ACTION_BATTERY_AFTER	= 1, // 탈것 베터리 후
	EMVEHICLE_ACTION_BOOSTER_ENABLE			= 2,	/*vehicle booster system, Juver, 2017/08/10 */
	EMVEHICLE_ACTION_BIKE_CHANGE_COLOR		= 3,	/*bike color , Juver, 2017/11/12 */
	EMVEHICLE_ACTION_CAR_CART_CHANGE_COLOR	= 4,	/* car, cart color, Juver, 2018/02/14 */

};

// 펫 액션 타입
enum EMPET_ACTION
{
	EMPET_ACTION_FOOD_BEFORE	= 0, // 펫 먹이전
	EMPET_ACTION_FOOD_AFTER		= 1, // 펫 먹이후
	EMPET_ACTION_RENAME			= 2, // 펫 이름변경
	EMPET_ACTION_COLOR			= 3, // 펫 컬러변경
	EMPET_ACTION_STYLE			= 4, // 펫 스타일변경
	EMPET_ACTION_SKILL			= 5, // 펫 스킬
	EMPET_ACTION_REVIVE			= 6, // 펫 부활카드
	EMPET_ACTION_PETSKINPACK	= 7,  // 펫 스킨 팩

	EMPET_ACTION_PET_DUAL_SKILL	= 20, /*dual pet skill, Juver, 2017/12/29 */
};

#endif // GLROUTEDEFINE_H_INCLUDED__

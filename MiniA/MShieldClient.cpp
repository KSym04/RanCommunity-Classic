#include "stdafx.h"
#include "./MShieldClient.h"



#ifdef USE_MSHIELD

namespace MShield
{
  MTSETCALLBACK pMtSetCallback = 0;
  HANDSHAKE handShake = 0;
  HMODULE hMTD = 0;

  NOINLINE void __stdcall HandShake(PSHANDSHAKE param)
  {
	  UINT handShakeHash = (param->hashIN * 8);
	  param->result = 0;

	  if ((handShake(&handShakeHash, &param->result) == 0) || (handShakeHash != param->hashIN))
	  {
		  //Call Failed
		  param->result = 0;
	  }
  }

  /*
    Returns (IN_PARAM * 307592)
  */
  NOINLINE UINT MakeHandShakeHash(UINT handShakeIN)
  {
	  SHANDSHAKE param = { handShakeIN, 0 };

	  HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MShield::HandShake, &param, 0, 0);
	  DWORD waitResult = WaitForSingleObject(hThread, (HANDSHAKE_TIMEOUT_SECONDS * 1000));

	  if (waitResult == WAIT_TIMEOUT)
	  {
		  //MShield is suspended
		  TerminateThread(hThread, 0);
		  param.result = 0;
	  }

	  return param.result;
  }

  NOINLINE void Init()
  {
    hMTD = LoadLibraryA("mtd.dll");
    if (hMTD)
    {
	  ((MTU)GetProcAddress(hMTD, "_mtu@0"))();
      handShake = (HANDSHAKE)(((MTP)GetProcAddress(hMTD, "_mtp@0"))());
    }
  }
};

#endif

//###################################################################
/*
  Implement the code below inside your WindowProc Handler
*/
//LRESULT __stdcall WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//  LRESULT result = 0;
//
//  result = DefWindowProcA(hWnd, uMsg, wParam, lParam);
//
//  if (uMsg == WM_COPYDATA)
//  {
//    PCOPYDATASTRUCT cds = (PCOPYDATASTRUCT)lParam;
//
//    if ((cds->dwData == GyxMessages::SETACCOUNTCALLBACK) && (cds->lpData))
//    {
//      MoveMemory(&MShield::pMtSetCallback, cds->lpData, cds->cbData);
//    }
//  }
//
//  return result;
//}
//###################################################################


//###################################################################
/*
  Implement the code after a successful login
*/
//void onLoginSuccessful()
//{
//  MShield::pMtSetCallback(&ACTIVE_ACCOUNT_INFO::strActiveAccount);
//}
//###################################################################
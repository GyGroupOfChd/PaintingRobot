//Pcomm32dll.h
#if !defined(Pcomm32dll)
#define Pcomm32dll
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef int  (FAR WINAPI *DOWNLOADMSGPROC)(LPSTR str, BOOL newline);
typedef int  (FAR WINAPI *DOWNLOADGETPROC)(int nIndex, LPSTR lpszBuffer, int nMaxLength);
typedef void (FAR WINAPI *DOWNLOADPROGRESS)(int nPercent);


extern "C"
{
	
	__declspec(dllimport) BOOL __stdcall OpenPmacDevice(DWORD dwDevice);
	__declspec(dllimport) BOOL __stdcall ClosePmacDevice(DWORD dwDevice);
	__declspec(dllimport) long __stdcall PmacSelect( HWND hwnd );
	__declspec(dllimport) int  __stdcall PmacGetResponseA(DWORD dwDevice,LPTSTR s,UINT maxchar,LPCTSTR outstr);
	__declspec(dllimport) int  __stdcall PmacGetControlResponseA(DWORD dwDevice,LPTSTR s,UINT maxchar,CHAR outchar);
	__declspec(dllimport) void __stdcall PmacDownloadFile(DWORD dwDevice, char *fname);
	__declspec(dllimport) int  __stdcall PmacDownloadA(DWORD dwDevice, DOWNLOADMSGPROC msgp,DOWNLOADGETPROC getp, DOWNLOADPROGRESS ppgr,PCHAR filename, BOOL macro, BOOL map, BOOL log,BOOL dnld );
	__declspec(dllimport) int  __stdcall PmacSendLineA(DWORD dwDevice, LPCTSTR outstr);
	__declspec(dllimport) int  __stdcall PmacGetLineA(DWORD dwDevice, LPTSTR s,UINT maxchar);
	__declspec(dllimport) int  __stdcall PmacDownloadProgress(BOOL bDoChecksums,BOOL bAlwaysShow, HWND hWnd, DWORD dwDevice,PCHAR filename, BOOL macro, BOOL map, BOOL log,BOOL dnld );
	
}
#endif

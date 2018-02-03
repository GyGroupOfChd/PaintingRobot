// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "comacrsrvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IStatus properties

/////////////////////////////////////////////////////////////////////////////
// IStatus operations

CString IStatus::GetBstrVersion()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IStatus::GetNPort()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IStatus::SetNPort(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IStatus::GetNBPS()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IStatus::SetNBPS(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IStatus::GetBstrIP()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IStatus::SetBstrIP(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IStatus::GetNBus()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IStatus::SetNBus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IStatus::GetIsOffline()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IStatus::GetNStatusWaitRate()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IStatus::SetNStatusWaitRate(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IStatus::GetBOnConnectTest()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IStatus::SetBOnConnectTest(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IStatus::GetNCard()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IStatus::Disconnect()
{
	InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IStatus::Connect(long nTransport, long nIndex)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nTransport, nIndex);
}

VARIANT IStatus::GetACRGroup(LPCTSTR bstrRequest)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		bstrRequest);
	return result;
}

VARIANT IStatus::GetACRCustom(LPCTSTR bstrRequest)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		bstrRequest);
	return result;
}

VARIANT IStatus::GetACRMemory(long nType, long nAddress, long nCount)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		nType, nAddress, nCount);
	return result;
}

long IStatus::AddACRGroup(LPCTSTR bstrRequest)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bstrRequest);
	return result;
}

long IStatus::AddACRCustom(LPCTSTR bstrRequest)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bstrRequest);
	return result;
}

long IStatus::AddACRMemory(long nType, long nAddress, long nCount)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, nAddress, nCount);
	return result;
}

VARIANT IStatus::GetStatus(long nMsgid)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		nMsgid);
	return result;
}

void IStatus::DelStatus(long nMsgid)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMsgid);
}

long IStatus::GetLocalAddr(long nProg, long nType, long* nSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nProg, nType, nSize);
	return result;
}

long IStatus::GetLocalArrayAddr(long nProg, long nType, long nArray, long* nSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nProg, nType, nArray, nSize);
	return result;
}

BOOL IStatus::IsFlagSet(long nFlagGrp, long nFlagNdx)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nFlagGrp, nFlagNdx);
	return result;
}

BOOL IStatus::TestConnect()
{
	BOOL result;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IStatus::GetParmInfo(long nParameter, long* nType, long* nCode, long* nIndex, BSTR* bstrCatagory, BSTR* bstrDesc)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PBSTR VTS_PBSTR;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nParameter, nType, nCode, nIndex, bstrCatagory, bstrDesc);
	return result;
}

void IStatus::SetWatchdog(long nInterval, long nRetries)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nInterval, nRetries);
}

VARIANT IStatus::GetACRGroupRaw(long nType, long nCode, long nIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		nType, nCode, nIndex);
	return result;
}

long IStatus::AddACRGroupRaw(long nType, long nCode, long nIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, nCode, nIndex);
	return result;
}

long IStatus::GetParmType(long nParameter)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nParameter);
	return result;
}

long IStatus::GetParmAddr(long nParameter)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nParameter);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IControl properties

/////////////////////////////////////////////////////////////////////////////
// IControl operations

CString IControl::GetBstrVersion()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IControl::GetNPort()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNPort(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IControl::GetNBPS()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNBPS(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IControl::GetBstrIP()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IControl::SetBstrIP(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IControl::GetNBus()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNBus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IControl::GetIsOffline()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IControl::GetNMoveProfile()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNMoveProfile(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float IControl::GetFMoveVEL()
{
	float result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void IControl::SetFMoveVEL(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float IControl::GetFMoveFVEL()
{
	float result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void IControl::SetFMoveFVEL(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float IControl::GetFMoveACC()
{
	float result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void IControl::SetFMoveACC(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long IControl::GetNMoveMode()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNMoveMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IControl::GetBMoveAbsolute()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IControl::SetBMoveAbsolute(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IControl::GetNMoveCounter()
{
	long result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNMoveCounter(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IControl::GetNArcMode()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::SetNArcMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IControl::GetBArcAbsolute()
{
	BOOL result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IControl::SetBArcAbsolute(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IControl::GetBArcCCW()
{
	BOOL result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IControl::SetBArcCCW(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IControl::GetBOnConnectTest()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IControl::SetBOnConnectTest(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IControl::GetNCard()
{
	long result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IControl::Disconnect()
{
	InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IControl::Connect(long nTransport, long nIndex)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nTransport, nIndex);
}

void IControl::SetFlag(long nBit, BOOL bValue, BOOL bFast)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nBit, bValue, bFast);
}

void IControl::SetParmFloat(long nPparm, float fValue, BOOL bFast)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_BOOL;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nPparm, fValue, bFast);
}

void IControl::SetParmLong(long nPparm, long nValue, BOOL bFast)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nPparm, nValue, bFast);
}

void IControl::SetGlobal(long nCard, long nGlobal, double dValue, BOOL bFast)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R8 VTS_BOOL;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCard, nGlobal, dValue, bFast);
}

void IControl::Move(long nMask, const VARIANT& targets)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMask, &targets);
}

void IControl::Arc(long nMask, const VARIANT& targets)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMask, &targets);
}

void IControl::Stop(BOOL bDecel)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bDecel);
}

void IControl::SendRES(long nMask)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMask);
}

BOOL IControl::TestConnect()
{
	BOOL result;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IControl::SetWatchdog(long nInterval, long nRetries)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nInterval, nRetries);
}

void IControl::GetMoveCounter(long* nCounter, long* nIncrement)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCounter, nIncrement);
}

void IControl::SetMoveCounter(long nCounter, long nIncrement)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCounter, nIncrement);
}

void IControl::SetACRMemory(long nType, long nAddress, const VARIANT& values)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nType, nAddress, &values);
}

void IControl::SetACRMemoryMask(long nAddress, long nNAND, long nOR)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nAddress, nNAND, nOR);
}

void IControl::SetParmLongMask(long nPparm, long nNAND, long nOR)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nPparm, nNAND, nOR);
}

void IControl::SetFOV(long nMask, float fValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMask, fValue);
}

void IControl::SetROV(long nMask, float fValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMask, fValue);
}

void IControl::InitPerformance()
{
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT IControl::GetPerformance()
{
	VARIANT result;
	InvokeHelper(0x78, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void IControl::MoveBatch(long nType, const VARIANT& moves)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nType, &moves);
}

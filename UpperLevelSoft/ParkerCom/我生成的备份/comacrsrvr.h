// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IStatus wrapper class

// Import the BoxBridge Type Library to make COM stuff a bit easier to read
//#import "ComACRsrvr.tlb" no_namespace named_guids


class IStatus : public COleDispatchDriver
{
public:
	IStatus() {}		// Calls COleDispatchDriver default constructor
	IStatus(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IStatus(const IStatus& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetBstrVersion();
	long GetNPort();
	void SetNPort(long nNewValue);
	long GetNBPS();
	void SetNBPS(long nNewValue);
	CString GetBstrIP();
	void SetBstrIP(LPCTSTR lpszNewValue);
	long GetNBus();
	void SetNBus(long nNewValue);
	BOOL GetIsOffline();
	long GetNStatusWaitRate();
	void SetNStatusWaitRate(long nNewValue);
	BOOL GetBOnConnectTest();
	void SetBOnConnectTest(BOOL bNewValue);
	long GetNCard();
	void Disconnect();
	void Connect(long nTransport, long nIndex);
	VARIANT GetACRGroup(LPCTSTR bstrRequest);
	VARIANT GetACRCustom(LPCTSTR bstrRequest);
	VARIANT GetACRMemory(long nType, long nAddress, long nCount);
	long AddACRGroup(LPCTSTR bstrRequest);
	long AddACRCustom(LPCTSTR bstrRequest);
	long AddACRMemory(long nType, long nAddress, long nCount);
	VARIANT GetStatus(long nMsgid);
	void DelStatus(long nMsgid);
	long GetLocalAddr(long nProg, long nType, long* nSize);
	long GetLocalArrayAddr(long nProg, long nType, long nArray, long* nSize);
	BOOL IsFlagSet(long nFlagGrp, long nFlagNdx);
	BOOL TestConnect();
	BOOL GetParmInfo(long nParameter, long* nType, long* nCode, long* nIndex, BSTR* bstrCatagory, BSTR* bstrDesc);
	void SetWatchdog(long nInterval, long nRetries);
	VARIANT GetACRGroupRaw(long nType, long nCode, long nIndex);
	long AddACRGroupRaw(long nType, long nCode, long nIndex);
	long GetParmType(long nParameter);
	long GetParmAddr(long nParameter);
};
/////////////////////////////////////////////////////////////////////////////
// IControl wrapper class

class IControl : public COleDispatchDriver
{
public:
	IControl() {}		// Calls COleDispatchDriver default constructor
	IControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IControl(const IControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetBstrVersion();
	long GetNPort();
	void SetNPort(long nNewValue);
	long GetNBPS();
	void SetNBPS(long nNewValue);
	CString GetBstrIP();
	void SetBstrIP(LPCTSTR lpszNewValue);
	long GetNBus();
	void SetNBus(long nNewValue);
	BOOL GetIsOffline();
	long GetNMoveProfile();
	void SetNMoveProfile(long nNewValue);
	float GetFMoveVEL();
	void SetFMoveVEL(float newValue);
	float GetFMoveFVEL();
	void SetFMoveFVEL(float newValue);
	float GetFMoveACC();
	void SetFMoveACC(float newValue);
	long GetNMoveMode();
	void SetNMoveMode(long nNewValue);
	BOOL GetBMoveAbsolute();
	void SetBMoveAbsolute(BOOL bNewValue);
	long GetNMoveCounter();
	void SetNMoveCounter(long nNewValue);
	long GetNArcMode();
	void SetNArcMode(long nNewValue);
	BOOL GetBArcAbsolute();
	void SetBArcAbsolute(BOOL bNewValue);
	BOOL GetBArcCCW();
	void SetBArcCCW(BOOL bNewValue);
	BOOL GetBOnConnectTest();
	void SetBOnConnectTest(BOOL bNewValue);
	long GetNCard();
	void Disconnect();
	void Connect(long nTransport, long nIndex);
	void SetFlag(long nBit, BOOL bValue, BOOL bFast);
	void SetParmFloat(long nPparm, float fValue, BOOL bFast);
	void SetParmLong(long nPparm, long nValue, BOOL bFast);
	void SetGlobal(long nCard, long nGlobal, double dValue, BOOL bFast);
	void Move(long nMask, const VARIANT& targets);
	void Arc(long nMask, const VARIANT& targets);
	void Stop(BOOL bDecel);
	void SendRES(long nMask);
	BOOL TestConnect();
	void SetWatchdog(long nInterval, long nRetries);
	void GetMoveCounter(long* nCounter, long* nIncrement);
	void SetMoveCounter(long nCounter, long nIncrement);
	void SetACRMemory(long nType, long nAddress, const VARIANT& values);
	void SetACRMemoryMask(long nAddress, long nNAND, long nOR);
	void SetParmLongMask(long nPparm, long nNAND, long nOR);
	void SetFOV(long nMask, float fValue);
	void SetROV(long nMask, float fValue);
	void InitPerformance();
	VARIANT GetPerformance();
	void MoveBatch(long nType, const VARIANT& moves);
};

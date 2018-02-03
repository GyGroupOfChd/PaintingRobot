// ReadStatus.h: interface for the ReadStatus class.
//
//////////////////////////////////////////////////////////////////////
#include"atlcom.h"
#include"windows.h"
#if !defined(AFX_READSTATUS_H__47483B56_287D_4403_91E1_6DDC39231933__INCLUDED_)
#define AFX_READSTATUS_H__47483B56_287D_4403_91E1_6DDC39231933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// **ADDED**  Everything in this Header is added functionality.

// Import the Type Library to make COM stuff a bit easier to read.
// I put it here, since ReadStatus is the only class using its data.
// This import can be used instead of the MFC generated COM Wrapper (ComACRsrvr.h). 
// We will leave the MFC generated COM wrapper code in the main application to show
// the use of both.  When using both, let the #import use a namespace (default) or 
// else there will be name conflicts.
#import "ComACRServer.tlb" named_guids

// Global ATL structure needs to be declared here for the SINK_MAP, but cannot
// really be here because it will get created in too many .OBJ files.  Just
// use an extern place holder and really declare it in the .CPP filed.
extern _ATL_FUNC_INFO ReadAlertInfo;

//-----------------------------------------------------------------------------
// Connection Points are really callbacks, so we have to implement a COM method 
// that can be called from the automation server.  Using a class lets us inherit 
// from the ATL template, saving us the pain of implementing the generic interfaces.
//-----------------------------------------------------------------------------

class ReadStatus:
	public IDispEventSimpleImpl<1, ReadStatus, &DIID__IChannelEvents>
{
// The ReadStatus Class gets the status updates and passes them along
// to the window in the form of a message.  ReadStatus waits to be alerted 
// by Communications Server when data is available to read (then sends/posts
// a message with information needed to get the status.
//
public:
  // Constructor and Destructor
	ReadStatus(void *);
	virtual ~ReadStatus();

  // Catch alerts and pass on status data
  void __stdcall ReadAlert(long, long);

	// [
	// 	uuid(548F6282-AEE7-4AFA-B1F6-8C85389DD697),
	// 	helpstring("_IStatusEvents Interface")
	// ]
	// dispinterface _IStatusEvents
	// {
	// 	properties:
	// 	methods:
	// 	[id(1), helpstring("Status Waiting")] HRESULT StatusWaiting([in] long msgID, [in] long error);
	// };

BEGIN_SINK_MAP(ReadStatus)
	SINK_ENTRY_INFO(1, DIID__IChannelEvents, 1, ReadAlert, &ReadAlertInfo)
END_SINK_MAP()

private:
  //IStatusPtr m_stat;
  CComQIPtr<IStatus> m_stat;
  long m_Msgid;
  HWND m_hDialog;
};

#endif // !defined(AFX_READSTATUS_H__47483B56_287D_4403_91E1_6DDC39231933__INCLUDED_)

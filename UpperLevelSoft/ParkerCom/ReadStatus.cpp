// ReadStatus.cpp: implementation of the ReadStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ACR9000.h"
#include "ReadStatus.h"
//#include "ACR9000Dlg.h"
#include "..\LowMoniter.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ATL defined structure for defining the parameter data of the callback.  
// In this case there are two prameters.
_ATL_FUNC_INFO ReadAlertInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_I4}};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReadStatus::ReadStatus(void *winPtr)
{
  // We could have passed in just the specific data needed, to make this more
  // generic, but passing in the window pointer makes it look more MFC like.
  CLowMoniter *workIt = (CLowMoniter *) winPtr;
 	m_hDialog = workIt->m_hWnd;                       // window handle for messages

  //m_stat.CreateInstance(CLSID_Status);// start up COM Interface

   //CoInitializeEx(NULL, COINIT_MULTITHREADED); 
   CComPtr<IUnknown> spUnknown;
   spUnknown.CoCreateInstance(L_COMACRSERVER_PROGID);
  if (spUnknown)
  {
	  m_stat= spUnknown;
  }

  HRESULT hr = DispEventAdvise((IUnknown*)m_stat);
  m_stat->PutnStatusWaitRate(400);                    // Minimum time between status events
 // m_stat->
  //m_stat->PutnBPS(workIt->m_BPS);                   // Set BPS
  //m_stat->PutnPort(workIt->m_Port);                 // Set Port
  m_stat->PutbstrIP("192.168.10.40"); // Set IP Address
  //m_stat->PutnBus(workIt->m_BusType?1:0);           // Set Bus, checked(true) is ISA, else PCI
  m_stat->Connect(3,0);// Connect to Transport

  if(!m_stat->GetisOffline()){   
	  m_Msgid = m_stat->AddACRCustom(_T("P12295,P12551,P12807,P13063,P12290,P12546,P12802,P13058,P12291,P12547,P12803,P13059"));    // Start the status queue - p6916 is the ACR global clock
  }
}

ReadStatus::~ReadStatus()
{
  if(m_stat){
    m_stat->DelStatus(m_Msgid);
    DispEventUnadvise((IUnknown*)m_stat);
    //m_stat->Disconnect();
	m_stat.Release();
	//delete m_stat;
  }
}

void __stdcall ReadStatus::ReadAlert(long msg, long err)
{
  // Pass on the message id in a windows message
  // Either Send or Post message can be used.
  //::SendMessage(workIt->m_hDialog,WMU_UpdateStats,0,msg);
  ::PostMessage(m_hDialog,WMU_UpdateStats,0,msg);
} 

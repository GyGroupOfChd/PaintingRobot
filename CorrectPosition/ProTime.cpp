// ProTime.cpp : implementation file
//

#include "stdafx.h"
#include "CameraTime.h"
#include "ProTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProTime

CProTime::CProTime()
{
	QueryPerformanceFrequency((LARGE_INTEGER *) &m_CounterFrequency);
	Reset();
}

CProTime::~CProTime()
{
}


BEGIN_MESSAGE_MAP(CProTime, CWnd)
	//{{AFX_MSG_MAP(CProTime)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProTime message handlers
void CProTime::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER *) &m_ResetTime);
}

float CProTime::GetTime(bool reset)
{
	// Get current time
	LONGLONG currentTime;
	QueryPerformanceCounter((LARGE_INTEGER *) &currentTime);
	
	// Compute difference from last reset
	LONGLONG diffTime = currentTime - m_ResetTime;
	
	// Reset time
	if (reset)
		m_ResetTime = currentTime;
	
	// Return time in seconds
	return (float) diffTime / (float) m_CounterFrequency;
}
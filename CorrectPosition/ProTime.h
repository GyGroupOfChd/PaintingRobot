#if !defined(AFX_PROTIME_H__2B355C2C_CBEF_48C9_922E_1DD722EFABD6__INCLUDED_)
#define AFX_PROTIME_H__2B355C2C_CBEF_48C9_922E_1DD722EFABD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProTime.h : header file
//
#include "windows.h"
/////////////////////////////////////////////////////////////////////////////
// CProTime window

class CProTime : public CWnd
{
// Construction
public:
	CProTime();
	void Reset();
	float GetTime(bool reset);
// Attributes
public:
	LONGLONG m_CounterFrequency;
	LONGLONG m_ResetTime;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProTime)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProTime();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProTime)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTIME_H__2B355C2C_CBEF_48C9_922E_1DD722EFABD6__INCLUDED_)

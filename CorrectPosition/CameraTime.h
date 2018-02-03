// CameraTime.h : main header file for the CAMERATIME application
//

#if !defined(AFX_CAMERATIME_H__058B2DC4_32A9_4296_8507_D9B07C28C28C__INCLUDED_)
#define AFX_CAMERATIME_H__058B2DC4_32A9_4296_8507_D9B07C28C28C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCameraTimeApp:
// See CameraTime.cpp for the implementation of this class
//

class CCameraTimeApp : public CWinApp
{
public:
	CCameraTimeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraTimeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCameraTimeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERATIME_H__058B2DC4_32A9_4296_8507_D9B07C28C28C__INCLUDED_)

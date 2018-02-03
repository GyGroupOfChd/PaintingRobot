#pragma once
//#include "ParkerCom/ReadStatus.h"
//#include"Resource.h"
//#include <atlstr.h>
#include <string>
//class ReadStatus;        // **ADDED** Avoids #include-ing the class header here
#define USE_CONTENT_IN_DEPENDENT_CLASS //若定义本宏，函数的具体实现放在另外一个类中
// CLowMoniter 对话框

class CLowMoniter : public CDialog
{
	DECLARE_DYNAMIC(CLowMoniter)

public:
	CLowMoniter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLowMoniter();

// 对话框数据
	enum { IDD = IDD_LOW_MONITER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
public:
//	#ifndef USE_CONTENT_IN_DEPENDENT_CLASS
//	CComQIPtr<IChannel> m_Chan;
//	CComQIPtr<IContrl> m_Cntl;
//	CComQIPtr<IStatus> m_Stat;
//	CString m_pstr;
//	long m_pKey;
//#else
//	 CParkerMotionCardCommu m_cMotionCommu;					///<具体通讯的实现内容
//#endif	
};

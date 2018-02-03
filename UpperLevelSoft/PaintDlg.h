#pragma once
#include "afxwin.h"
//#include ".\pmac\runtime.h"
//#include ".\pmac\Include\dialog.h"
//#include ".\PMAC\Include\UtilDT.h"
// PaintDlg 对话框

struct AlignmentButton
{
	bool Button_AddX;
	bool Button_ReduceX;
	bool Button_AddY;
	bool Button_ReduceY;
};
class CPaintDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaintDlg)

public:
	CPaintDlg(CWnd* pParent = NULL);   // 标准构造函数
	CPaintDlg(CString LowCondition,bool Condition,CWnd* pParent = NULL);
	virtual ~CPaintDlg();
	bool bAGV_InitState;//AGV初始化标志位，成功为TRUE,失败为FALSE
	bool bInitState;
	DWORD mydwDevice;//AGV_Init函数的形参
	bool AGV_Init(DWORD* dwDevice);
// 对话框数据
	enum { IDD = IDD_PAINTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Pumppower;
	CButton m_Pumpoff;
	CButton m_Motorpower;
	CButton m_Motoroff;
	afx_msg void OnBnClickedComstatus();
	afx_msg void OnBnClickedPumppower();
	afx_msg void OnBnClickedBumpoff();
	afx_msg void OnBnClickedStartmotor();
	afx_msg void OnBnClickedMotoroff();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedGun1();
	afx_msg void OnBnClickedWash();
	//afx_msg void OnBnClickedGenarate();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedRun();
	void MadeAllButtonDown();

public:
	HWND m_ParentDialogDown;
	//DWORD mydwDevice;
	CString m_Connect;
	bool m_Connected;
	afx_msg void OnBnClickedStop();
	//afx_msg void OnBnClickedPlc();
	afx_msg void OnBnClickedStopall();
	afx_msg void OnBnClickedContinue();
	//CButton m_GenerateFile;
	CButton m_DownloadMotion;
	//CButton m_DownloadPLC;
	CButton m_Temporary;
	CButton m_BeginAll;
	CButton m_StopAll;
	CButton m_ContinueAll;
	bool m_gun1,m_gun2,m_gun3,m_gun4,m_allguns;
	AlignmentButton m_AlignmentButton;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAllguns();
	afx_msg void OnBnClickedGun2();
	afx_msg void OnBnClickedGun3();
	afx_msg void OnBnClickedGun4();
	afx_msg void OnBnClickedStopwash();
	afx_msg void OnBnClickedAgainchoise();
	afx_msg void OnBnClickedAddx();
	afx_msg void OnBnClickedReducex();
	afx_msg void OnBnClickedAddy();
	afx_msg void OnBnClickedReducey();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

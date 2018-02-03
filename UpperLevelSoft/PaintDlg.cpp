// PaintDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperLevelSoft.h"
#include "PaintDlg.h"
#include "afxdialogex.h"


// PaintDlg 对话框

IMPLEMENT_DYNAMIC(CPaintDlg, CDialog)

CPaintDlg::CPaintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaintDlg::IDD, pParent)
	, m_Connect(_T(""))
{
	m_ParentDialogDown=pParent->GetSafeHwnd();
	m_Connect="断";
	m_Connected=false;
	m_gun1=false;
	m_gun2=false;
	m_gun3=false;
	m_gun4=false;
	m_allguns=false;
	m_AlignmentButton.Button_AddX=false;
	m_AlignmentButton.Button_ReduceX=false;
	m_AlignmentButton.Button_AddY=false;
	m_AlignmentButton.Button_ReduceY=false;
	//m_GenerateFile.EnableWindow(TRUE);
}
CPaintDlg::CPaintDlg(CString LowCondition,bool Condition,CWnd* pParent): CDialog(CPaintDlg::IDD, pParent)
	,m_Connect(LowCondition)
	,m_Connected(Condition)
{
	
}
CPaintDlg::~CPaintDlg()
{
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Pumppower, m_Pumppower);
	DDX_Control(pDX, IDC_Bumpoff, m_Pumpoff);
	DDX_Control(pDX, IDC_StartMotor, m_Motorpower);
	DDX_Control(pDX, IDC_MOTOROFF, m_Motoroff);
	DDX_Text(pDX, IDC_CONNECT, m_Connect);
	//DDX_Control(pDX, IDC_Genarate, m_GenerateFile);
	DDX_Control(pDX, IDC_Download, m_DownloadMotion);
	//DDX_Control(pDX, IDC_PLC, m_DownloadPLC);
	DDX_Control(pDX, IDC_STOP,m_Temporary);
	DDX_Control(pDX, IDC_Run, m_BeginAll);
	DDX_Control(pDX, IDC_STOPALL, m_StopAll);
	DDX_Control(pDX, IDC_CONTINUE, m_ContinueAll);
	
}


BEGIN_MESSAGE_MAP(CPaintDlg, CDialog)
	ON_BN_CLICKED(IDC_COMStatus, &CPaintDlg::OnBnClickedComstatus)
	ON_BN_CLICKED(IDC_Pumppower, &CPaintDlg::OnBnClickedPumppower)
	ON_BN_CLICKED(IDC_Bumpoff, &CPaintDlg::OnBnClickedBumpoff)
	ON_BN_CLICKED(IDC_StartMotor, &CPaintDlg::OnBnClickedStartmotor)
	ON_BN_CLICKED(IDC_MOTOROFF, &CPaintDlg::OnBnClickedMotoroff)
	ON_BN_CLICKED(IDC_Return, &CPaintDlg::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_Quit, &CPaintDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_GUN1, &CPaintDlg::OnBnClickedGun1)
	ON_BN_CLICKED(IDC_WASH, &CPaintDlg::OnBnClickedWash)
	//ON_BN_CLICKED(IDC_Genarate, &CPaintDlg::OnBnClickedGenarate)
	ON_BN_CLICKED(IDC_Download, &CPaintDlg::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_Run, &CPaintDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_STOP, &CPaintDlg::OnBnClickedStop)
	//ON_BN_CLICKED(IDC_PLC, &CPaintDlg::OnBnClickedPlc)
	ON_BN_CLICKED(IDC_STOPALL, &CPaintDlg::OnBnClickedStopall)
	ON_BN_CLICKED(IDC_CONTINUE, &CPaintDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_ALLGUNS, &CPaintDlg::OnBnClickedAllguns)
	ON_BN_CLICKED(IDC_GUN2, &CPaintDlg::OnBnClickedGun2)
	ON_BN_CLICKED(IDC_GUN3, &CPaintDlg::OnBnClickedGun3)
	ON_BN_CLICKED(IDC_GUN4, &CPaintDlg::OnBnClickedGun4)
	ON_BN_CLICKED(IDC_STOPWASH, &CPaintDlg::OnBnClickedStopwash)
	ON_BN_CLICKED(IDC_AGAINCHOISE, &CPaintDlg::OnBnClickedAgainchoise)
	ON_BN_CLICKED(IDC_ADDX, &CPaintDlg::OnBnClickedAddx)
	ON_BN_CLICKED(IDC_REDUCEX, &CPaintDlg::OnBnClickedReducex)
	ON_BN_CLICKED(IDC_ADDY, &CPaintDlg::OnBnClickedAddy)
	ON_BN_CLICKED(IDC_REDUCEY, &CPaintDlg::OnBnClickedReducey)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// PaintDlg 消息处理程序


void CPaintDlg::OnBnClickedComstatus()
{
	//bInitState=AGV_Init(&mydwDevice);
	//if(!bInitState)
	//{
	//	 PostQuitMessage(0);
	//	 m_Connect="断";
	//	 m_Connected=false;
	//	 return ;
	//}
	//TCHAR strtmp[255];
	//TCHAR buftmp[255];
	//FillMemory(strtmp, 255, '\0');
	//FillMemory(buftmp, 255, '\0');
	//sprintf((char *)strtmp, "K");
	//PmacGetResponseA(mydwDevice, (PCHAR)buftmp, 255, (PCHAR)strtmp);// 通讯状态,如果弹出那个地址窗口，则视为成功
	//m_Connect="通";
	//m_Connected=true;
	//UpdateData(FALSE);
}
bool CPaintDlg::AGV_Init(DWORD* dwDevice)
{
	/*(*dwDevice)=PmacSelect(NULL);
	if ((*dwDevice)>=0&&(*dwDevice)<=7)
	{
		if(!OpenPmacDevice(*dwDevice))
		{
			bAGV_InitState=false;
			return false;
		}
	}
	else
	{
		bAGV_InitState=false;
		return false;
	}
	TCHAR strtmp[255];
	TCHAR buftmp[255];

	FillMemory(strtmp,255,'\0');	
	FillMemory(buftmp,255,'\0');

	sprintf((char *)strtmp,"#1o0 #2o0 #3o0 #4o0 m7917=1 m7918=1 m7937=1 m7938=0");
	PmacGetResponseA(*dwDevice,(PCHAR)buftmp,255,(PCHAR)strtmp);
	bAGV_InitState = true;
	mydwDevice = (*dwDevice);*/
	return true;
}


void CPaintDlg::OnBnClickedPumppower()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_PUMPPOWERON,0,0);
		m_Pumppower.ShowWindow(SW_HIDE);
		m_Pumpoff.ShowWindow(SW_SHOW);
	}
	
}


void CPaintDlg::OnBnClickedBumpoff()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_PUMPPOWEROFF,0,0);
		m_Pumpoff.ShowWindow(SW_HIDE);
		m_Pumppower.ShowWindow(SW_SHOW);
	}
}


void CPaintDlg::OnBnClickedStartmotor()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_MOTORON,0,0);
		m_Motoroff.ShowWindow(SW_SHOW);
		m_Motorpower.ShowWindow(SW_HIDE);
	}
}


void CPaintDlg::OnBnClickedMotoroff()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_MOTOROFF,0,0);
		m_Motoroff.ShowWindow(SW_HIDE);
		m_Motorpower.ShowWindow(SW_SHOW);
	}
}


void CPaintDlg::OnBnClickedReturn()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_RETURNZEROPOINT,0,0);
	}
}


void CPaintDlg::OnBnClickedQuit()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_STOPALLFUNCTION,0,0);
	}
}


void CPaintDlg::OnBnClickedGun1()
{
	m_gun1=true;
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(FALSE);
}


void CPaintDlg::OnBnClickedWash()
{
	CString Parm="";
	if(m_gun1==true)
		Parm="M4009=1";
	else
		Parm="M4009=0";
	if(m_gun2==true)
		Parm=Parm+"M4010=1";
	else
		Parm=Parm+"M4010=0";
	if(m_gun3==true)
		Parm=Parm+"M4011=1";
	else
		Parm=Parm+"M4011=0";
	if(m_gun4==true)
		Parm=Parm+"M4012=1";
	else
		Parm=Parm+"M4012=0";
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_BEGINWASHGUN,0,(LPARAM)Parm.AllocSysString());	
	}
	
	m_gun1=false;
	m_gun2=false;
	m_gun3=false;
	m_gun4=false;
}


//void CPaintDlg::OnBnClickedGenarate()
//{
//	//m_GenerateFile.EnableWindow(FALSE);
//	//m_DownloadMotion.EnableWindow(TRUE);
//	//m_DownloadPLC.EnableWindow(TRUE);
//	//m_ContinueAll.EnableWindow(TRUE);
//	//m_Temporary.EnableWindow(TRUE);
//	//m_StopAll.EnableWindow(TRUE);
//	//m_BeginAll.EnableWindow(TRUE);
//
//	//char tPath[1024];
//	//memset(tPath,0,sizeof(tPath));
//	//CFileDialog fileDlg(FALSE,_T(""),NULL,OFN_HIDEREADONLY,_T("All Files (*.*)|*.*||"), NULL); 
//	//if(fileDlg.DoModal() == IDOK)
//	//{
//	//	//创建文件夹
//	//	_stprintf(tPath,_T("%s"),fileDlg.GetPathName());
//	//	CreateDirectory(tPath,NULL);
//	//}
//	////::SendMessage(m_ParentDialogDown,WMU_SAVEPMCDIRECTORY,0,(LPARAM)tPath);
//	////this->PostMessageA(WMU_SAVEPMCDIRECTORY,0,0);
	//::SendMessageA(GetParent()->m_hWnd,WMU_SAVEPMCDIRECTORY,0,(LPARAM)tPath);
//	////::PostMessage(GetParent()->m_hWnd,WMU_SAVEPMCDIRECTORY,0,0);
//	////LPCSTR lpszFilter="PMAC files(*.pmc)|*.pmc|NC files(*.NC)|*.NC||";
//	////CFileDialog aDlg(false,_T("pmc"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFilter);
//	////CString path;
//	////CString pathname;
//	////if(aDlg.DoModal()==IDOK)
//	////{
//	////	HCURSOR hCursor=AfxGetApp()->LoadStandardCursor(IDC_WAIT);
//	////	::SetCursor(hCursor);
//	////	//Sleep(500);        
//	////	path=aDlg.GetPathName();
//	////	pathname=path.GetBuffer(0);
//	////	int DouHao=pathname.Find('.');
//	////	CString SaveDirectory=pathname.Left(DouHao);
//	////	//FILE  *fp;
//	////	//int m_nPointCount = ColPoint->GetItemCount();
//	////	/*fp = fopen(pathname, "wt");
//	////	if (NULL == fp)
//	////	{
//	////		AfxMessageBox("文件创建失败");
//	////	}*/
//	////	::PostMessage(GetParent()->m_hWnd,WMU_SAVEPMCDIRECTORY,0,(LPARAM)SaveDirectory.AllocSysString());
//	////}
//}


void CPaintDlg::OnBnClickedDownload()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_DOWNLOADFILE,0,0);
	}
}


void CPaintDlg::OnBnClickedRun()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_RUN,0,0);
	}
}


void CPaintDlg::OnBnClickedStop()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_TEMPORARYSTOP,0,0);
	}
}


//void CPaintDlg::OnBnClickedPlc()
//{
//	if(m_Connected==true)
//	{
//		LPCSTR lpszFilterl="PMAC files(*.pmc)|*.pmc|NC files(*.NC)|*.NC||";
//		CFileDialog aDlgl(true,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(LPCTSTR)lpszFilterl);
//		CString pathl;
//		char *pathnamel;
//		if(aDlgl.DoModal()==IDOK)
//		{
//			HCURSOR hCursor=AfxGetApp()->LoadStandardCursor(IDC_WAIT);
//			::SetCursor(hCursor);
//			Sleep(500);        // 暂停500毫秒
//			pathl=aDlgl.GetPathName();
//			pathnamel=(char *)pathl.GetBuffer(0);	
//			//DeviceDownload(0,NULL,NULL,NULL,pathnamel,1,1,1,1);
//			//PmacDownloadFile(mydwDevice,pathnamel);
//			PmacDownloadA(mydwDevice,NULL,NULL,NULL,pathnamel,1,1,1,1);
//		}
//	}
//	else
//		MessageBox(_T("请连接下位机！"));
//}


void CPaintDlg::OnBnClickedStopall()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd,WMU_STOPNOW,0,0);
	}
}


void CPaintDlg::OnBnClickedContinue()
{
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd, WMU_AGAINRUN,0,0);
	}
}


BOOL CPaintDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Pumpoff.EnableWindow(TRUE);
	m_Motoroff.EnableWindow(TRUE);
	m_Pumppower.ShowWindow(SW_HIDE);
	m_Motorpower.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMStatus)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STOPWASH)->EnableWindow(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPaintDlg::OnBnClickedAllguns()
{
	GetDlgItem(IDC_GUN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_GUN2)->EnableWindow(FALSE);
	GetDlgItem(IDC_GUN3)->EnableWindow(FALSE);
	GetDlgItem(IDC_GUN4)->EnableWindow(FALSE);
	m_gun1=true;
	m_gun2=true;
	m_gun3=true;
	m_gun4=true;
}


void CPaintDlg::OnBnClickedGun2()
{
	m_gun2=true;
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(FALSE);
}


void CPaintDlg::OnBnClickedGun3()
{
	m_gun3=true;
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(FALSE);
}


void CPaintDlg::OnBnClickedGun4()
{
	m_gun4=true;
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(FALSE);
}


void CPaintDlg::OnBnClickedStopwash()
{
	m_gun1=false;
	m_gun2=false;
	m_gun3=false;
	m_gun4=false;
	((CButton*)GetDlgItem(IDC_GUN1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN3))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN4))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_ALLGUNS))->SetCheck(FALSE);
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN4)->EnableWindow(TRUE);
	if (m_Connected==true)
	{
		::PostMessage(GetParent()->m_hWnd, WMU_STOPWASHGUN,0,0);
	}
}


void CPaintDlg::OnBnClickedAgainchoise()
{
	((CButton*)GetDlgItem(IDC_GUN1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN3))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_GUN4))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_ALLGUNS))->SetCheck(FALSE);
	GetDlgItem(IDC_ALLGUNS)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN1)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN2)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN3)->EnableWindow(TRUE);
	GetDlgItem(IDC_GUN4)->EnableWindow(TRUE);
	m_gun1=false;
	m_gun2=false;
	m_gun3=false;
	m_gun4=false;
}


void CPaintDlg::OnBnClickedAddx()
{
	m_AlignmentButton.Button_AddX=true;
}


void CPaintDlg::OnBnClickedReducex()
{
	m_AlignmentButton.Button_ReduceX=true;
}


void CPaintDlg::OnBnClickedAddy()
{
	m_AlignmentButton.Button_AddY=true;
}


void CPaintDlg::OnBnClickedReducey()
{
	m_AlignmentButton.Button_ReduceY=true;
}

//OnLButtonDown(UINT nFlags, CPoint point)函数的作用是当点击+X或+Y或-X或-Y按钮时，喷枪一直运动
void CPaintDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	void *m_ButtonAlignment=(void *)&m_AlignmentButton;
	::PostMessage(GetParent()->m_hWnd, WMU_ALIGNMENTCLICKED,(WPARAM)m_ButtonAlignment,0);
	CDialog::OnLButtonDown(nFlags, point);
}

//OnLButtonUp(UINT nFlags, CPoint point)函数的作用是当松开+X或+Y或-X或-Y按钮时，喷枪停止运动
void CPaintDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	::PostMessage(GetParent()->m_hWnd, WMU_ALIGNMENTLOOSEN,0,0);
	m_AlignmentButton.Button_AddX=false;
	m_AlignmentButton.Button_AddY=false;
	m_AlignmentButton.Button_ReduceX=false;
	m_AlignmentButton.Button_ReduceY=false;
	CDialog::OnLButtonUp(nFlags, point);
}

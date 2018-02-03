// LowMoniter.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperLevelSoft.h"
#include "LowMoniter.h"
#include "afxdialogex.h"


// CLowMoniter 对话框

IMPLEMENT_DYNAMIC(CLowMoniter, CDialog)

CLowMoniter::CLowMoniter(CWnd* pParent /*=NULL*/)
	: CDialog(CLowMoniter::IDD, pParent)
{

}

CLowMoniter::~CLowMoniter()
{
}

void CLowMoniter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLowMoniter, CDialog)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CLowMoniter::OnBnClickedBtnConnect)
END_MESSAGE_MAP()


// CLowMoniter 消息处理程序


void CLowMoniter::OnBnClickedBtnConnect()
{
//	try
//	{
//#ifndef USE_CONTENT_IN_DEPENDENT_CLASS
//		UpdateData();
//		CComBSTR ip("192.168.10.40");
//		m_Chan->put_bstrIP(ip);
//		m_Cntl->bstrIP="192.168.10.40"; //在应用程序中设置所连接的控制器的IP地址
//		m_Stat->bstrIP="192.168.10.40";
//		m_Cntl->Connect(3,0);               //与控制器通过以太网进行连接
//		m_Stat->Connect(3,0);  
//		m_Chan->Connect(3,0);
//		if(!m_Cntl->isOffline && !m_Stat->isOffline)   //判断控制器是否离线(连接成功)
//		{
//			//Sleep(100);
//			m_isOffline="控制器连接成功!";//+m_Stat->GetBstrVersion();  //如果连接成功，则对话框上显示
//			//“控制器连接成功”及版本号
//			m_Stat->DelStatus(-1);//删除所有队列请求
//			Sleep(100);
//			//如果连接成功，则对话框上显示“控制器连接成功”及版本号
//			//m_Stat->SetNStatusWaitRate(10); //设置控制器向应用程序发送事
//			//件消息的最小间隔为10m
//			m_pstr="P12289,P12545,P12801,P13057,P12290,P12546,P12802,P13056,P12291,P12547,P12803,P13059";
//			m_pKey=m_Stat->AddACRCustom(m_pstr.GetBuffer()); //调用AddACRCustom函数告知控制器应用程序要实时读取的P参数，并将该参数的返回值存储到m_pKey中。
//			Sleep(100);
//			QueryParmFromACR();
//
//		}
//		else
//		{
//			m_isOffline="未连接!";
//		}
//		UpdateData(false);                    //更新
//		m_workerEvent = new ReadStatus(this); //创建ReadStatus类对象
//#else
//		m_cMotionCommu.Connect(this);
//		m_isOffline=m_cMotionCommu.m_isOffline;
//		UpdateData(FALSE);
//		m_cMotionCommu.SetAutoResPonse(this);
//#endif
//		
//
//	}catch(_com_error &e) {
//		long m_errCode;
//#ifdef  USE_CONTENT_IN_DEPENDENT_CLASS
//		m_errCode=m_cMotionCommu.m_Chan->GetnLastErrorCode();
//		e;
//#else
//			m_errCode=m_Chan->GetnLastErrorCode();
//			e;
//#endif
//	
//		ShowControlErr(m_errCode);
//	}
//	catch(CException *e) { 
//		TCHAR szCause[1024];
//		e->GetErrorMessage(szCause, 1024);   
//		MessageBox(szCause, _T("non-COM Error"), MB_OK);
//	}

}

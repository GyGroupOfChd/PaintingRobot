//#include "StdAfx.h"
#include "stdafx.h"
#include "ParkerMotionCardCommu.h"
#include "LowMoniter.h"
#include "UpperLevelSoftDlg.h"

CParkerMotionCardCommu::CParkerMotionCardCommu(void): m_isOffline(_T(""))
, m_chooseAxis(0)
, M_axisXSecPo(0)
, M_axisY1SecPos(0)
, M_axisY2SecPo(0)
, M_axisZSecPos(0)
, M_axisXAcrPo(0)
, M_axisY1AcrPo(0)
, M_axisY2AcrPo(0)
, M_axisZAcrPo(0)
, M_axisXFowEr(0)
, M_axisY1FowEr(0)
, M_axisY2FowEr(0)
, M_axisZFowErr(0)
{
	//InitComWithParker();
}

CParkerMotionCardCommu::~CParkerMotionCardCommu(void)
{
}

bool CParkerMotionCardCommu::Connect(void* pTargetDlg)
{


	//UpdateData();
		CComBSTR ip("192.168.10.40");
		m_Chan->put_bstrIP(ip);

		m_Cntl->bstrIP="192.168.10.40";		//在应用程序中设置所连接的控制器的IP地址
		m_Stat->bstrIP="192.168.10.40";     //状态
		/*m_Cntl->isOffline=true;
		m_Cntl->*/
		m_Cntl->Connect(3,0);               //与控制器通过以太网进行连接
		m_Stat->Connect(3,0);  
		m_Chan->Connect(3,0);
		if(!m_Cntl->isOffline && !m_Stat->isOffline)   //判断控制器是否离线(连接成功)
		{
			//Sleep(100);
			m_isOffline="控制器连接成功!";//+m_Stat->GetBstrVersion();  //如果连接成功，则对话框上显示
			m_bisOffline=false;
			////“控制器连接成功”及版本号
			//m_Stat->DelStatus(-1);//删除所有队列请求
			//Sleep(100);
			////果连接成功，则对话框上显示“控制器连接成功”及版本号
			////m_Stat->SetNStatusWaitRate(10); //设置控制器向应用程序发送事
			////件消息的最小间隔为10m
			//m_pstr="P12289,P12545,P12801,P13057,P12290,P12546,P12802,P13056,P12291,P12547,P12803,P13059";
			//m_pKey=m_Stat->AddACRCustom(m_pstr.GetBuffer()); //调用AddACRCustom函数告知控制器应用程序要实时读取的P参数，并将该参数的返回值存储到m_pKey中。
			//Sleep(100);
			//QueryParmFromACR();
		}
		else
		{
			m_isOffline="未连接!";
			return false;
		}
		//m_workerEvent = new ReadStatus(pTargetDlg); //创建ReadStatus类对象
	return true;
	// TODO: 在此添加控件通知处理程序代码
}
bool CParkerMotionCardCommu::SetAutoResPonse(void * pTargetDlg)
{
	if(!m_Cntl->isOffline && !m_Stat->isOffline)				//判断控制器是否离线(连接成功)
	{
		//Sleep(100);
		m_Stat->DelStatus(-1);									//删除所有队列请求
		Sleep(100);
		//m_Stat->SetNStatusWaitRate(10);						//设置控制器向应用程序发送事
		//件消息的最小间隔为10m
		m_pstr="P12289,P12545,P12801,P13057,P12290,P12546,P12802,P13056,P12291,P12547,P12803,P13059";
		m_pKey=m_Stat->AddACRCustom(m_pstr.GetBuffer()); //调用AddACRCustom函数告知控制器应用程序要实时读取的P参数，并将该参数的返回值存储到m_pKey中。
		//m_Stat->
		Sleep(100);
		QueryParmFromACR();

	}
	else
	{
		m_isOffline="未连接!";
		return false;
	}
	m_workerEvent = new ReadStatus(pTargetDlg); //创建ReadStatus类对象
	return true;
}

//bool CParkerMotionCardCommu::Connect(CUpperLevelSoftDlg* pTargetDlg)
//{
//
//
//	//UpdateData();
//	CComBSTR ip("192.168.10.40");
//	m_Chan->put_bstrIP(ip);
//
//	m_Cntl->bstrIP="192.168.10.40"; //在应用程序中设置所连接的控制器的IP地址
//	m_Stat->bstrIP="192.168.10.40";
//	m_Cntl->Connect(3,0);               //与控制器通过以太网进行连接
//	m_Stat->Connect(3,0);  
//	m_Chan->Connect(3,0);
//	if(!m_Cntl->isOffline && !m_Stat->isOffline)   //判断控制器是否离线(连接成功)
//	{
//		//Sleep(100);
//		m_isOffline="控制器连接成功!";//+m_Stat->GetBstrVersion();  //如果连接成功，则对话框上显示
//		//“控制器连接成功”及版本号
//		m_Stat->DelStatus(-1);//删除所有队列请求
//		Sleep(100);
//		//果连接成功，则对话框上显示“控制器连接成功”及版本号
//		//m_Stat->SetNStatusWaitRate(10); //设置控制器向应用程序发送事
//		//件消息的最小间隔为10m
//		m_pstr="P12289,P12545,P12801,P13057,P12290,P12546,P12802,P13056,P12291,P12547,P12803,P13059";
//		m_pKey=m_Stat->AddACRCustom(m_pstr.GetBuffer()); //调用AddACRCustom函数告知控制器应用程序要实时读取的P参数，并将该参数的返回值存储到m_pKey中。
//		Sleep(100);
//		QueryParmFromACR();
//
//	}
//	else
//	{
//		m_isOffline="未连接!";
//		return false;
//	}
//	m_workerEvent = new ReadStatus(pTargetDlg); //创建ReadStatus类对象
//	return true;
//	// TODO: 在此添加控件通知处理程序代码
//}
bool CParkerMotionCardCommu::InitComWithParker(void)
{
	CComPtr<IUnknown> spUnknown;
	spUnknown.CoCreateInstance(L"ComACRServer.Channel.1");
	if (spUnknown)
	{
		spUnknown.QueryInterface(&m_Stat);
		spUnknown.QueryInterface(&m_Cntl);
		spUnknown.QueryInterface(&m_Chan);
	}
	m_workerEvent = NULL;
	m_bisOffline=true;
	return false;
}
void CParkerMotionCardCommu::DestroyComWithParker(void)
{

	if(m_workerEvent){
		delete m_workerEvent;
		m_workerEvent=NULL;		
	} 
	if(m_Cntl){
		m_Cntl->Disconnect();
		m_Cntl.Release();
		//delete m_Cntl;
	}
	if(m_Stat){
		m_Stat->Disconnect();
		m_Stat.Release();
	}
	if(m_Chan){
		m_Chan->Disconnect();
		m_Chan.Release();
	}
}
bool CParkerMotionCardCommu::UpdateStats(UINT nID, LONG xMsgid)
{
	CComVariant vaxis;
	CComVariant *buf;
	if(xMsgid==m_pKey)
	{
		if(m_Stat->isOffline)
		{	return false;  }
		vaxis = m_Stat->GetStatus(m_pKey);
		::SafeArrayAccessData(vaxis.parray,(void**)&buf);
		M_axisXSecPo=buf[0].lVal;
		M_axisY1SecPos=buf[1].lVal;
		M_axisY2SecPo=buf[2].lVal;
		M_axisZSecPos=buf[3].lVal;
		M_axisXAcrPo=buf[4].lVal;
		M_axisY1AcrPo=buf[5].lVal;
		M_axisY2AcrPo=buf[6].lVal;
		M_axisZAcrPo=buf[7].lVal;
		M_axisXFowEr=buf[8].lVal;
		M_axisY1FowEr=buf[9].lVal;
		M_axisY2FowEr=buf[10].lVal;
		M_axisZFowErr=buf[11].lVal;
		::SafeArrayUnaccessData(vaxis.parray);	
		return true;
	}
	return false;
}
void CParkerMotionCardCommu::Disconnect()
{
	bool bOffLine=false;
	if (m_Stat)
	{
		if(!m_Stat->isOffline)   //判断控制器是否离线(连接成功)
		{m_Stat->DelStatus(-1);}
		Sleep(100);		
		m_Stat->Disconnect();
		if(m_Stat->isOffline)
		{bOffLine=true;	}
	}
	else
	{bOffLine=true;	}

	if (m_Cntl){
		m_Cntl->Disconnect();
		if(m_Cntl->isOffline)
		{bOffLine=true;	}
		else{bOffLine=false;}
	}//断开连接
	else
	{bOffLine=true;	}
	if (bOffLine)
	{m_isOffline="未连接";	
		m_bisOffline=true;
	}
	if(m_workerEvent){
		delete m_workerEvent;
		m_workerEvent=NULL;		
	} 
	// TODO: 在此添加控件通知处理程序代码
}
void CParkerMotionCardCommu::SetVel(float Vel)
{
	if(m_Cntl->isOffline)
	{
		return;
	}
	//根据所选择轴的不同，设置JOG运动速度
	if(m_Cntl->isOffline)
	{
		return;
	}
	switch(m_chooseAxis)
	{
	case 0:
		{
			m_Cntl->SetParmFloat(12346, Vel, true);
			//调用Control类的SetParaFloat函数对ACR控制器的浮点型P参
			//数赋值
			break;
		}
	case 1:
		{
			m_Cntl->SetParmFloat(12602, Vel, true);
			break;
		}
	case 2:
		{
			m_Cntl->SetParmFloat(12858, Vel, true);
			break;
		}
	case 3:
		{
			m_Cntl->SetParmFloat(13114, Vel, true);
			break;
		}
	default:
		break;
	}
	// TODO: 在此添加控件通知处理程序代码
}
void CParkerMotionCardCommu::Motionfwd()
{
		//根据所选择轴的不同，控制各轴的JOG正向运动
		if(m_Cntl->isOffline)
		{
			return;
		}
		switch(m_chooseAxis)
		{
		case 0:
			{
				m_Cntl->SetFlag(797,false,true);
				//调用Control类的SetFlag函数对某一个BIT位置位或复位
				m_Cntl->SetFlag(796,true,true);
				break;
			}
		case 1:
			{
				m_Cntl->SetFlag(829,false,true);
				m_Cntl->SetFlag(828,true,true);
				break;
			}
		case 2:
			{
				m_Cntl->SetFlag(861,false,true);
				m_Cntl->SetFlag(860,true,true);
				break;
			}
		case 3:
			{
				m_Cntl->SetFlag(893,false,true);
				m_Cntl->SetFlag(892,true,true);
				break;
			}
		default:
			break;
		}
}
void CParkerMotionCardCommu::MotionStop()
{
	// TODO: Add your control notification handler code here
	if(m_Cntl->isOffline)
	{
		return;
	}
	switch(m_chooseAxis)
	{
	case 0:
		{
			m_Cntl->SetFlag(797,false,true);
			m_Cntl->SetFlag(796,false,true);
			break;
		}
	case 1:
		{
			m_Cntl->SetFlag(829,false,true);
			m_Cntl->SetFlag(828,false,true);
			break;
		}
	case 2:
		{
			m_Cntl->SetFlag(861,false,true);
			m_Cntl->SetFlag(860,false,true);
			break;
		}
	case 3:
		{
			m_Cntl->SetFlag(893,false,true);
			m_Cntl->SetFlag(892,false,true);
			break;
		}
	default:
		break;
	}
}
void CParkerMotionCardCommu::MotionReverse()
{
	// TODO: Add your control notification handler code here
	if(m_Cntl->isOffline)
	{
		return;
	}
	switch(m_chooseAxis)
	{
	case 0:
		{
			m_Cntl->SetFlag(796,false,true);
			m_Cntl->SetFlag(797,true,true);
			break;
		}
	case 1:
		{
			m_Cntl->SetFlag(828,false,true);
			m_Cntl->SetFlag(829,true,true);
			break;
		}
	case 2:
		{
			m_Cntl->SetFlag(860,false,true);
			m_Cntl->SetFlag(861,true,true);
			break;
		}
	case 3:
		{
			m_Cntl->SetFlag(892,false,true);
			m_Cntl->SetFlag(893,true,true);
			break;
		}
	default:
		break;
	}
}
void CParkerMotionCardCommu::QueryParmFromACR(void)
{
	CComVariant vaxis;   
	CComVariant *buf;
	vaxis = m_Stat->GetACRCustom(m_pstr.GetBuffer());
	::SafeArrayAccessData(vaxis.parray,(void**)&buf);          //用SafeArrayAccessData直接读写SafeArray缓冲区：
	M_axisXSecPo=buf[0].lVal;
	M_axisY1SecPos=buf[1].lVal;
	M_axisY2SecPo=buf[2].lVal;
	M_axisZSecPos=buf[3].lVal;
	M_axisXAcrPo=buf[4].lVal;
	M_axisY1AcrPo=buf[5].lVal;
	M_axisY2AcrPo=buf[6].lVal;
	M_axisZAcrPo=buf[7].lVal;
	M_axisXFowEr=buf[8].lVal;
	M_axisY1FowEr=buf[9].lVal;
	M_axisY2FowEr=buf[10].lVal;
	M_axisZFowErr=buf[11].lVal;
	::SafeArrayUnaccessData(vaxis.parray);
}
void CParkerMotionCardCommu::EnableDrive()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Cntl->isOffline)
	{
		return;
	}
	switch(m_chooseAxis)
	{
	case 0:
		{
			m_Cntl->SetFlag(8465,true,true);
			break;
		}
	case 1:
		{
			m_Cntl->SetFlag(8497,true,true);
			break;
		}
	case 2:
		{
			m_Cntl->SetFlag(8529,true,true);
			break;
		}
	case 3:
		{
			m_Cntl->SetFlag(8561,true,true);
			break;
		}
	default:
		break;
	}
}

void  CParkerMotionCardCommu::DisableDrive()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Cntl->isOffline)
	{
		return;
	}
	switch(m_chooseAxis)
	{
	case 0:
		{
			m_Cntl->SetFlag(8465,false,true);
			break;
		}
	case 1:
		{
			m_Cntl->SetFlag(8497,false,true);
			break;
		}
	case 2:
		{
			m_Cntl->SetFlag(8529,false,true);
			break;
		}
	case 3:
		{
			m_Cntl->SetFlag(8561,false,true);
			break;
		}
	default:
		break;
	}
}

void  CParkerMotionCardCommu::Destroy(bool DestroyOnly)
{
	if(m_Cntl){	
		if(!m_Cntl->isOffline)
		{
			if (!DestroyOnly)
			{DisableDrive();}		
			Disconnect();//先后不可搞错	
		}
	}
	DestroyComWithParker();
}

void  CParkerMotionCardCommu::SetPval(unsigned PIndex,float Pval)
{
	if(m_Cntl->isOffline)
	{
		return;
	}
	m_Cntl->SetParmFloat(PIndex,Pval, true);
	// TODO: 在此添加控件通知处理程序代码
}
bool CParkerMotionCardCommu::DownLoadParmFlow(MotionParm_It_TYPE ItBegin,MotionParm_It_TYPE ItEnd)
{
	unsigned PNum=P_NUM_BEGIN;
	for (ItBegin;ItBegin<ItEnd;ItBegin++)
	{
		if (PNum<P_NUM_END)
		{
			SetPval(PNum,*ItBegin);
			PNum++;
		}	
		else
		{
			return false;///<超出了许可设置的范围。出错。
		}
	}
	return true;
}


int CParkerMotionCardCommu::connect_Wraped(void)
{
	if (!m_isOffline)//防止重复连接
	{
		return 0;
	}
	try
	{
		Connect(this);
	}catch(_com_error &e) {
		long m_errCode;
		m_errCode=m_Chan->GetnLastErrorCode();
		e;
		ShowCacthedControlErr(m_errCode);
	}
	catch(CException *e) { 
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);   
		//MessageBox(szCause, "non-COM Error", MB_OK);
	}

	return 0;
}
void CParkerMotionCardCommu::ShowCacthedControlErr(long errCode)
{
	switch(errCode)
	{
	case 17056:
		{
			::MessageBox(NULL,_T("从控制器读取或向控制器中写入数据前，必须与控制器建立连接!"),_T("连接错误"),MB_OK);
			break;
		}
	case 17083:
		{
			MessageBox(NULL,_T("与控制器连接失败!"),_T("通讯错误"),MB_OK);
			break;
		}
	default:
		{
			MessageBox(NULL,_T("未知的错误!"),NULL,MB_OK);
			break;
		}
	}
}
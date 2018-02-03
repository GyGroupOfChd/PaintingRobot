#include "StdAfx.h"
#include "PlcCommu.h"
CMscomm* CPlcCommu::m_Mscomm;///<自存的串口通讯实例引用
CPlcCommu::CPlcCommu(void):m_cEnter(13),m_csCommandTail("*"),m_csDataWriteCommandHeader("@00WD")
{
	//char a= 13;
	m_csCommandTail=m_csCommandTail+m_cEnter;
	CPlcCommu::m_Mscomm=NULL;
	m_bWaitForResponse=false;
}

CPlcCommu::~CPlcCommu(void)
{
}
bool CPlcCommu::InitComm(unsigned PortNum,CMscomm* Mscomm)
{
	if (!Mscomm)
	{
		MessageBoxA(NULL,("串口输入错误，请检查串口通讯实例"),NULL,NULL);
		return false;
	}
	Mscomm->put_CommPort(PortNum);//com1
	if(Mscomm->get_PortOpen())
	{
		MessageBoxA(NULL,("串口已打开，请先关闭"),NULL,NULL);
		return false;
	}

	Mscomm->put_InBufferSize(1024);
	Mscomm->put_OutBufferSize(512);
	/*if(!Mscomm->get_PortOpen())
	{
		Mscomm->put_PortOpen(TRUE);
		MessageBox(NULL,("串口打开成功"),"成功",NULL);
	}
	else
		MessageBox(NULL,("无法打开串口端口"),NULL,NULL);*/
	//	m_mscomm1.SetSettings(chSet);//"4800,n,8,1"
	Mscomm->put_Settings(_T("9600,e,7,2"));
	//Mscomm->put_InputMode(Mscomm->comInputModeText);//二进制检取数据
	Mscomm->put_InputMode(Mscomm->comInputModeBinary);//二进制检取数据
	Mscomm->put_RThreshold(1);//触发事件字节数
//	Mscomm.put_SThreshold(0);//如果想使用串口时间中的自动发这里只要设一个大于0 的数值 它就会一直发送直至发送512字节 
	Mscomm->put_InputLen(0);//全部读完
	//Mscomm.put_Input();
	return true;
}
bool CPlcCommu::OnReceieveCom(CMscomm* Mscomm)
{
	int CommEvent=Mscomm->get_CommEvent();
	if(CommEvent == 2)
	{
		
		//clearData();//清空原来的数据
		COleVariant varInput;//VARIANT 
		COleSafeArray sfarrInput;
		static long len,k;		
		static CString strTemp;
		len = Mscomm->get_InBufferCount();
		varInput = Mscomm->get_Input();
		return false;//调试
		//sfarrInput.Attach(varInput);
	/*	varInput.ChangeType((VT_BSTR));
		CString sfarrInput = varInput.bstrVal;*/
		sfarrInput = varInput;
		len = sfarrInput.GetOneDimSize();
		if (len<=4)//多帧发送的中间帧,有回复就行，不检查
		{	m_bWaitForResponse=false;
			return true;
		}
		CString strTmp;	
		CString reced;	
		//double SingleDis;
//sfarrInput.ChangeType()
		for(k =0; k < len; k++)//不去掉报头，报尾
		{
			sfarrInput.GetElement( &k, m_rxdata+k);
		//	BYTE bt=*(char*)(m_rxdata+k);
			strTmp.Format(_T("%c"),*(m_rxdata+k));
			 reced+=strTmp;
		}

		m_rxdata[len]='\0';//增加字符串的尾部标示
		return true;
		//m_ReceviedDat=m_rxdata;
		std::string ReceivedStr(&m_rxdata[1],&m_rxdata[len-4]); //中间转化为string,并去掉报头，报尾
		std::string strFCSVal(&m_rxdata[len-4],&m_rxdata[len]);
		//std::string ReceivedStr(reced.GetBuffer()+1,&m_rxdata[len-4]); //中间转化为string,并去掉报头，报尾
		///////////////////////////////////////有问题，编译通不过///////////////////////////////////
		//CString CommandFCSVal=(strFCSVal.c_str());//有问题，编译通不过

		//std::string EndCode=ReceivedStr.substr(4,2);
		//m_ReceviedDat.Format(_T("%s"),ReceivedStr.c_str());//转化为CString
		//if (CheckFCS(m_ReceviedDat,CommandFCSVal))
		//{
		//	if (EndCode!="00")//命令执行有误
		//	{
		//		EndCode.insert(0,_T("错误代码为："));
		//		MessageBox(NULL,_T(EndCode.data()),_T("PLC执行命令错误"),NULL);
		//	}
		//	else
		//	{
		//		m_bWaitForResponse=false;//执行正确
		//	}			
		//}
		//else
		//{			
		//	return false;
		//}
		return true;
		//FCS(m_ReceviedDat);		
		//SendOnceCommand(Mscomm);//继续接受
	}
		//处理报尾
		//UpdateData(FALSE);
	return true;//不是校验错误都返回真
}

bool CPlcCommu::CloseComm(CMscomm* Mscomm)
{
	if(!Mscomm->get_PortOpen())
	{
		//	MessageBoxA(NULL,("com1 already close"),NULL,NULL);
		MessageBox(NULL,_T("com端口早已关闭"),_T("错误"),NULL);

		return false;
	}
	else
	{
		Mscomm->put_PortOpen(FALSE);
		//	MessageBox("com1 close sucessfully");
	}
	return true;
}
CString CPlcCommu::FCS(CString& strFrame)
{
	int i ,count ;
	unsigned int intMidData = 0;
	count =strFrame.GetLength() ;
	for (i = 0;i<count;i++)
		intMidData = intMidData^strFrame.GetAt(i) ;
	CString FrameFCS;
	 FrameFCS.Format (_T("%02X"),intMidData);//2表示宽度为2,实际位数多于它则按实际走，0表示第不足位补0,
	return FrameFCS;
}

void CPlcCommu::SendSingleFrameData(CString& strToSend, CString& Address,CMscomm* Mscomm)
{
	//CString m_strSendData;

//	strToSend="@00WD00050080";
	//strToSend.Insert(0,'@');
	m_uReSendCound=0;
	CString m_strSendData=FormatCommand(strToSend, Address);
	TRACE(m_strSendData);
	Mscomm->put_Output(COleVariant( m_strSendData));
	m_bWaitForResponse=true;
	//return 0;
}
void CPlcCommu::SendMultiFrameData(CString& strToSend, CString& Address,unsigned FramePos,CMscomm* Mscomm)
{
		CString strSendData;
		m_uReSendCound=0;
		switch (FramePos)
		{
		case 0://头帧
			strSendData=FormHeaderFrameOfMutiFrameCommand(strToSend, Address);
			break;
		case 2://尾帧
			strSendData=FormFinalFrameOfMutiFrameCommand(strToSend, Address);
			break;
		default:
			strSendData=FormMiddleFrameOfMutiFrameCommand(strToSend, Address);
			break;
		}

		TRACE(strSendData);
		Mscomm->put_Output(COleVariant(strSendData));
	m_bWaitForResponse=true;
}
bool CPlcCommu::CheckFCS(CString& strToCheck,CString RetrunFCSVal)
{
	
	CString FCSVal= FCS(strToCheck);
	return FCSVal==RetrunFCSVal;
}

bool CPlcCommu::openComm(CMscomm* Mscomm)
{
	if (!Mscomm)
	{
		MessageBoxA(NULL,("串口输入错误，请检查串口初始化"),NULL,NULL);
		return false;
	}
	if(!Mscomm->get_PortOpen())
	{
		Mscomm->put_PortOpen(TRUE);
		MessageBox(NULL,_T("串口打开成功"),_T("成功"),NULL);
		return true;
	}
	else
		MessageBox(NULL,_T("无法打开串口端口"),NULL,NULL);
	return false;
}

bool CPlcCommu::CheckCommandResponse(CMscomm* Mscomm)
{
	if (!m_bWaitForResponse)
	{
		return true;
	}
	else
	{
		if (m_uReSendCound<MAX_RESEND_TIMES)
		{ 
			ResendCommand(Mscomm);
			m_uReSendCound++;
			Sleep(30);//等待响应
			return CheckCommandResponse(Mscomm);//递归调用
		}
		else
		{
			MessageBox(NULL,_T("重复发送多次数据均未获得PLC正确响应"),_T("Error"),NULL);
		}
	}	
	return false;
}

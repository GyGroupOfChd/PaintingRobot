#include "SharpRed.h"
#include "afx.h"
FILE  *fp=fopen("E:\\txttxt.txt","w");
CString GetStringPaserll(CString gps_str,int num)  //截取逗号间的字符串
{
	int start=0;
	for(int i=0;i<num;i++)
	{
		int find_ok=gps_str.Find(",",start);
		if(find_ok>=0)
			start=find_ok+1;
		else break;
	}
	int end=gps_str.Find(",",start);
	if(end>start)
		return gps_str.Mid(start,end-start);
	else 
		return _T("");
}
SharpRedMeasure::SharpRedMeasure()
{
	m_SharpDistance=10;      //红外测距距离初始化
	m_SharpDistance2=10;
	m_SharpDistance1=10;
	m_OpenOrNot=0;
	ThresholdRankAndDistanceInit();
}
SharpRedMeasure::~SharpRedMeasure()
{
	//CloseDevice(CaiJiKa);
}
int SharpRedMeasure::VoltageTranslateDistance(float Voltage)
{
	int rank=VoltageRank(Voltage);       //得到距离等级
	return RankToDistance(rank,Voltage);
}
int SharpRedMeasure::VoltageRank(float Voltage)
{
	if (Voltage<ThresholdRank[13])
	{
		return 13;
	}
	for (int i=0;i<14;i++)
	{
		if ((Voltage-ThresholdRank[i])>=0)
		{
			return i;  
		}
	}
}
int SharpRedMeasure::RankToDistance(int Rank,float Voltage)
{
	float Distance=(ThresholdDistance[Rank+1]-ThresholdDistance[Rank])*(Voltage-ThresholdRank[Rank])/(ThresholdRank[Rank+1]-ThresholdRank[Rank])+ThresholdDistance[Rank];
	return (int)Distance;
}
void SharpRedMeasure::ThresholdRankAndDistanceInit()
{
	ThresholdRank[0]=ONERANG;
	ThresholdRank[1]=TWORANK;
	ThresholdRank[2]=THREERNK;
	ThresholdRank[3]=FOURRANK;
	ThresholdRank[4]=FIVERANK;
	ThresholdRank[5]=SIXRANK;
	ThresholdRank[6]=SEVENRANK;
	ThresholdRank[7]=EIGHTRANK;
	ThresholdRank[8]=NINERANK;
	ThresholdRank[9]=TENRANK;
	ThresholdRank[10]=ELEVENRANK;
	ThresholdRank[11]=TWENTHRANK;
	ThresholdRank[12]=THIRTEENRANK;
	ThresholdRank[13]=FOURTEENRANK;

	ThresholdDistance[0]=TWENTY;
	ThresholdDistance[1]=THIRTY;
	ThresholdDistance[2]=FORTY;
	ThresholdDistance[3]=FIFTY;
	ThresholdDistance[4]=SIXTY;
	ThresholdDistance[5]=SEVENTY;
	ThresholdDistance[6]=EIGHTY;
	ThresholdDistance[7]=NINETY;
	ThresholdDistance[8]=ONEHUNDRED;
	ThresholdDistance[9]=ONEHUNDREDANDTEN;
	ThresholdDistance[10]=ONEHUNDREDANDTWENTY;
	ThresholdDistance[11]=ONEHUNDREDANDTHIRTY;
	ThresholdDistance[12]=ONEHUNDREDANDFORTY;
	ThresholdDistance[13]=ONEHUNDREDANDFIFTY;
	
}
bool SharpRedMeasure::OpenSharpUsb(int portnum, int Baud)
{
	if(!m_SharpRed.Open(portnum,Baud));		//AfxMessageBox("open error");CnComm serial串口
	m_SharpRed.SetOption(m_SharpRed.GetOption() | CnComm::EN_RX_BUFFER);
	QueryPerformanceFrequency(&m_Freq);
	m_SharpPollingThread = CreateThread(NULL,0,GPSImprovement,this,0,&m_SharpPollingThreadID);//该函数在主线程的基础上创建一个新线程
	return true;
}
bool SharpRedMeasure::Stop()
{
	PostThreadMessage(m_SharpPollingThreadID,WM_USER,0,0);
	WaitForSingleObject(m_SharpPollingThread,500);
	CloseHandle(m_SharpPollingThread);
	m_SharpRed.Close();
	return true;
}
DWORD __stdcall GPSImprovement(void* pParam)
{
	SharpRedMeasure* pParentSharp = (SharpRedMeasure*)pParam;
	CString recv=_T("");
	int show_num=0;
	int start_paser=0;
	long Time_Started_mSecond=GetTickCount();   //时刻
	int dwBytesRead_total=0;
	double lat_f=0,lon_f=0;
	for(;;)
	{
		DWORD dwBytesRead = 0;
		BYTE szBuffer[512];
		DWORD selcomstr_num=512*2;
		{
			DWORD comstr_num=pParentSharp->m_SharpRed.Input().Size();
			dwBytesRead = pParentSharp->m_SharpRed.Read(szBuffer,selcomstr_num);
			if(dwBytesRead>0)
				dwBytesRead_total+=dwBytesRead;
			long detwtime=GetTickCount()-Time_Started_mSecond;
			if(detwtime>=1000)
			{
				CString rate=_T("");
				rate.Format(_T("%dbps"),dwBytesRead_total*8000/detwtime);
				dwBytesRead_total=0;
				Time_Started_mSecond+=detwtime;
			}
			for(int ij=0;ij<dwBytesRead;ij++)
			{
				
			}
		}
	}
	return 0;
}
bool SharpRedMeasure::OpenDeviceCaiJiKa()
{
	//CaiJiKa=FindDevice();
	//if (OpenDevice(CaiJiKa)==FALSE)
	{
		//MessageBox(_T("打开红外测距传感器失败!"),_T("这是标题"), MB_OKCANCEL );
		//MessageBox(_T("ww"));
	}
	return true;
}
void SharpRedMeasure::GetNumberStream()
{
	//m_ReadDeviceHID2BUF(CaiJiKa,ADBuf,1024,DAbuf,CNbuf,DIO);
}
void SharpRedMeasure::Init()
{
	hDLL=LoadLibrary("D:\\PaintRobot\\UpperLevelSoft\\UpperLevelSoft\\ADIO16.dll");							//具体要看dll的命名
	if(hDLL==NULL)
	{
		FreeLibrary(hDLL);
		return;
	}
	m_FindDeviceLL=(FindDeviceLL)GetProcAddress(hDLL,"FindDevice");
	if(m_FindDeviceLL==NULL)
	{
		FreeLibrary(hDLL);
		return;
	}
	uint32_t m_FindOrNot=m_FindDeviceLL();
	if (m_FindOrNot<=0)
	{
		return ;
	}
	m_OpenDeviceLL=(OpenDeviceLL)GetProcAddress(hDLL,"OpenDevice");
	if (m_OpenDeviceLL==NULL)
	{
		FreeLibrary(hDLL);
	}
	m_OpenOrNot=m_OpenDeviceLL(0);
	if (m_OpenOrNot==0)
	{
		return;
	}
	m_m=(m_ReadDeviceHID2BUFLL)GetProcAddress(hDLL,"m_ReadDeviceHID2BUF");
	if(m_m==NULL)
	{
		FreeLibrary(hDLL);
	}
	m_WriteDeviceHID=(WriteDeviceHIDEXLL)GetProcAddress(hDLL,"WriteDeviceHIDEX");
	if(m_WriteDeviceHID==NULL)
	{
		FreeLibrary(hDLL);
	}
	uint8_t Cmd=0xAA;
	uint8_t Doo=0x00;
	m_WriteDeviceHID(0,Cmd,Doo,0,0,0);
	uint8_t Cmdl=0xFA;
	uint8_t Dool=0x06;
	m_WriteDeviceHID(0,Cmdl,Dool,0,0,0);
	//CaiJiKa=FindDevice();
}
void SharpRedMeasure::GetCapyture()
{
	memset(ADBuf,0,sizeof(ADBuf));
	uint32_t tt=m_m(0,ADBuf,256,DAbuf,CNbuf,DIO);
	for (int i=0;i<64;i++)
	{
		if (ADBuf[16*i]!=0)
		{
			int Distance=ADBuf[16*i]&0x0fff;
			int Distance1=ADBuf[16*i+1]&0x0fff;
			int Distance2=ADBuf[16*i+2]&0x0fff;
			/*CString Distancel;
			Distancel.Format("%d",Distance);
			DWORD dwHex=strtoul(Distancel,NULL,16);
			CString FinallDistance;
			FinallDistance.Format("%ld",dwHex);*/
			float Voltage=Distance*VOLTAGERANGE/MODULUS;
			float Voltage1=Distance1*VOLTAGERANGE/MODULUS;
			float Voltage2=Distance2*VOLTAGERANGE/MODULUS;
			if (Voltage>ThresholdRank[13]&&Voltage<ThresholdRank[0]&&Voltage1>ThresholdRank[13]&&Voltage1<ThresholdRank[0]&&Voltage2>ThresholdRank[13]&&Voltage2<ThresholdRank[0])
			{
				m_SharpDistance=VoltageTranslateDistance(Voltage);
				m_SharpDistance1=VoltageTranslateDistance(Voltage1);
				m_SharpDistance2=VoltageTranslateDistance(Voltage2);
				CString tt;
				tt.Format("%d %d %d\n",m_SharpDistance,m_SharpDistance1,m_SharpDistance2);
				fwrite(tt,strlen(tt),1,fp);
			}
		}
	}
	Sleep(100);
}

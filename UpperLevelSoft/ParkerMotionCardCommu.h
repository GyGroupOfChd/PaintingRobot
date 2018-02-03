#pragma once
/** @file 
*@brief 本文件用于定义运动控制通讯相关内容
即与下位机运动控制卡通信类
* @author 
* @date 
* @version 1.0
*
* 
*/
#include "ParkerCom/ReadStatus.h"
//#include"Resource.h"
#include <atlstr.h>
#include <string>
#include "ResultAdapter.h"
#define L_COMACRSERVER_PROGID L"ComACRServer.Channel.1"

class ReadStatus;											// **ADDED** Avoids #include-ing the class header here
class CUpperLevelSoftDlg;									//交叉引用必须这么弄
class CLowMoniter;											//交叉引用必须这么弄
#define  P_NUM_BEGIN 48										//52开始是列数据，之前有4个是数据的头部分
#define  P_NUM_END 10000
class CParkerMotionCardCommu
{
public:
	CParkerMotionCardCommu(void);
	~CParkerMotionCardCommu(void);

	bool  UpdateStats(UINT,LONG);							///<自定义的运控卡通讯消息,CComQIPtr为活动模板库智能指针
	CComQIPtr<IChannel> m_Chan;								///<连接通道
	CComQIPtr<IContrl> m_Cntl;								///<控制量传输
	CComQIPtr<IStatus> m_Stat;								///<状态量传输
	ReadStatus * m_workerEvent;
/**
*@brief 显示错误信息
*
*，注意在connect函数中若出现错误可能会使得m_cnt->isoffline 失实
*@parm
*
*@return
*@todo
*/
	void ShowControlErr(long errCode);
	bool InitComWithParker(void);
	void DestroyComWithParker(void);
/**
*@brief 删除本类的实力
*
*
*@parm
*
*@return
*@todo
*/
	void Destroy(bool DestroyOnly=false);
	CString m_isOffline;///<显示连接状态的静态文本关联变量
	bool m_bisOffline;///<未连接成功则为真，connect函数中若出现错误可能会使得m_cnt->isoffline 失实，因此增加此变量
	void Disconnect();
/**
*@brief 查看控制控件是否连接，是则为真
*
*
*@parm
*
*@return
*@todo
*/
	bool QueryCntConectStat()
	{		
		//bool bIsOf=m_Cntl->get_isOffline();
		return !m_bisOffline;
	}
/**
*@brief 将参数流挨个下载
*
*
*@parm
*
*@return
*@todo
*/
	bool DownLoadParmFlow(MotionParm_It_TYPE ItBegin,MotionParm_It_TYPE ItEnd);
/**
*@brief 与卡实现连接，通讯前必须连接
*
*
*@parm DLG* pTargetDlg  接受反馈回来的参数对话框
*
*@return
*@todo
*/

	bool Connect(void* pTargetDlg);
/**
*@brief 显示错误提示
*
*
*@parm
*
*@return
*@todo
*/
	void ShowCacthedControlErr(long errCode);
	//bool Connect(CUpperLevelSoftDlg* pTargetDlg);
	unsigned m_chooseAxis;
	//float m_jogVel;
	void SelectAxis(unsigned Axis)
	{
		m_chooseAxis= Axis;
	}
	void SetVel(float Vel);
	//m_jogVel
	void Motionfwd();
	void MotionStop();
	void MotionReverse();
	long M_axisXSecPo;
	long M_axisY1SecPos;
	long M_axisY2SecPo;
	long M_axisZSecPos;
	long M_axisXAcrPo;
	long M_axisY1AcrPo;
	long M_axisY2AcrPo;
	long M_axisZAcrPo;
	long M_axisXFowEr;
	long M_axisY1FowEr;
	long M_axisY2FowEr;
	long M_axisZFowErr;
	CString m_pstr;
	long m_pKey;
	void QueryParmFromACR(void);
	//afx_msg void OnBnClickedButton7();
	//afx_msg void OnBnClickedButton8();
	void EnableDrive();
	void DisableDrive();
	void SetPval(unsigned PIndex,float Pval);
	
	bool SetAutoResPonse(void * pTargetDlg);
	int connect_Wraped(void);
};

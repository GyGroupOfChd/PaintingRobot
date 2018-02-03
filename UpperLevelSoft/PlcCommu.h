#pragma once
#include "mscomm.h"
#include <string>
#define MAX_RESEND_TIMES 5///<出现错误时，重复发送上一名令的最大次数。超过了则报错
class CPlcCommu
{
public:
	CPlcCommu(void);
	~CPlcCommu(void);
	void InsertMsCom(CMscomm* Mscomm)
	{	 m_Mscomm=Mscomm;	}
	static CMscomm* m_Mscomm;///<自存的串口通讯实例引用
	/**
	*@brief 给定CMscomm类，对其初始化
	*
	* 
	*@param CMscomm1& Mscomm 需要初始化的mscomm类 
	*@param PortNum 端口号
	*
	*@return 
	*@todo 
	*/
	bool InitComm(unsigned PortNum,CMscomm* Mscomm= m_Mscomm);
	/**
	*@brief 给定CMscomm类，对其关闭端口
	*
	* 
	*@param CMscomm1& Mscomm 需要关闭端口的mscomm类 
	*
	*@return 成功关闭返回真 
	*@todo 
	*/
	bool  CloseComm(CMscomm* Mscomm= m_Mscomm);
	/**
	*@brief 串口通讯接受，出发了接受事件，有东西收到了，进行解析
	*
	*@param CMscomm1& Mscomm 串口通讯类实例
	*@return 当解析到的内容经FCS校验，显示对的时候，返回真，否则返回假
	*@todo 这里只考虑了接受命令反馈的内容，其他事件还需进一步考虑
	*/
	bool OnReceieveCom(CMscomm* Mscomm= m_Mscomm);
	char m_rxdata[100];///<用来存放收到的回复
	CString m_ReceviedDat;///<用来存放收到的回复
	/**
	*@brief 根据命令帧生成FCS值及命令尾
	*
	*
	*@parm strToSend 命令内容，不包括头尾
	*
	*@return 返回增加后的命令帧
	*@todo
	*/
	CString FCS(CString& strFrame);
	/**
	*@brief 给命令帧加上FCS值及命令尾、头
	*
	*
	*@parm strToSend 命令内容，不包括头尾
	*
	*@return 返回增加后的命令帧
	*@todo
	*/
	CString FormatCommand(CString& strFrame,CString& Address)
	{		
		SendCommand.Empty();
		SendCommand=Address+strFrame;
		SendCommand.Insert(0,m_csDataWriteCommandHeader);//00是UnitNumber
		//SendCommand=SendCommand+FCS(SendCommand)+'*'+'\r';
		SendCommand=SendCommand+FCS(SendCommand)+m_csCommandTail;
		//CString FrameEnd;
		//FrameEnd.Format ("%02X",intMidData,'*','\n');//2表示宽度为2,实际位数多于它则按实际走，0表示第不足位补0,
		
		return SendCommand;
	}
/**
*@brief 形成多帧命令的首帧内容
*
*
*@parm
*
*@return
*@todo
*/
	CString FormHeaderFrameOfMutiFrameCommand(CString& strFrame,CString& Address)
	{
		SendCommand.Empty();
		SendCommand=Address+strFrame;
		SendCommand.Insert(0,m_csDataWriteCommandHeader);//00是UnitNumber
		//SendCommand=SendCommand+FCS(SendCommand)+'*'+'\r';
		SendCommand=SendCommand+FCS(SendCommand)+m_cEnter;
		return SendCommand;
	}
	CString FormMiddleFrameOfMutiFrameCommand(CString& strFrame,CString& Address)
	{
		SendCommand.Empty();
		//SendCommand=Address+strFrame;
		SendCommand=strFrame;		
		SendCommand=SendCommand+FCS(SendCommand)+m_cEnter;
		return SendCommand;
	}
	CString FormFinalFrameOfMutiFrameCommand(CString& strFrame,CString& Address)
	{
		SendCommand.Empty();
		SendCommand=strFrame;
		//SendCommand.Insert(0,m_csDataWriteCommandHeader);//00是UnitNumber
		//SendCommand=SendCommand+FCS(SendCommand)+'*'+'\r';
		SendCommand=SendCommand+FCS(SendCommand)+m_csCommandTail;
		return SendCommand;
	}
	/*CString FormHeaderOfCommand(CString& Address)
	{
		CString Header="@00WD";
		return Header;
	}*/
	//CString FormTailOfCommand()
	//{
	//	return CString Tail("*/r");
	//}

	/**
	*@brief 重新发送命令
	*
	*重新发送命令SendCommand中的内容，计数m_uReSendCound，+1
	*@parm 
	*
	*@return 
	*@todo
	*/
	void ResendCommand(CMscomm* Mscomm= m_Mscomm)
	{
		Mscomm->put_Output(COleVariant(SendCommand));
		m_uReSendCound++;
	}
/**
*@brief 对外接口函数，发送命令帧
*
*
*@parm Mscomm 串口通讯器
*@parm strToSend 命令内容，不包括头尾
*
*@return
*@todo
*/
	void SendSingleFrameData(CString& strToSend, CString& Address,CMscomm* Mscomm= m_Mscomm);
	/**
	*@brief 对外接口函数，发送包含多个命令帧的命令
	*
	*因单个命令长度限制，对超长的命令内容，分解为多帧，
	*@parm Mscomm 串口通讯器
	*@parm strToSend 命令内容，不包括头尾
	*@parm FramePos 当前命令帧在整个多挣命令中的位置，开头为0，结尾为2，中间为1（默认）
	*
	*@return
	*@todo
	*/

	void SendMultiFrameData(CString& strToSend, CString& Address,unsigned FramePos=1,CMscomm* Mscomm= m_Mscomm);
	/**
	*@brief 检查反馈的内容FCS是否校验正确。
	*
	*
	*@parm Mscomm 串口通讯器
	*@parm strToSend 命令内容，不包括头尾
	*
	*@return
	*@todo
	*/
	bool CheckFCS(CString& strToCheck,CString FCSVal);
protected:
	CString SendCommand;///<需要发送的命令保存在此
	const CString m_csDataWriteCommandHeader;///<写数据区的指令头
	const char m_cEnter;///<回车符
	CString m_csCommandTail;///<命令尾
	unsigned m_uReSendCound;///<因FCS失败重新发送命令的次数
	bool m_bWaitForResponse;///<发送了等待回复，若所有发送均有回复并正确，则为假，否则为真
public:
/**
*@brief 打开串口
*
*
*@parm
*
*@return 打开成功则返回真
*@todo
*/
	bool openComm(CMscomm* Mscomm= m_Mscomm);
/**
*@brief 检查PLC发送回来的响应，若错误则重复发送已有命令
*
* 检查PLC发送回来的响应，若响应情况显示错误则重复发送已有命令 MAX_RESEND_TIMES 次，
*直至达到最大次数或者响应正确
*@parm
*
*@return
*@todo
*/
	bool CheckCommandResponse(CMscomm* Mscomm= m_Mscomm);
};

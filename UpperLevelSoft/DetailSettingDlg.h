#pragma once
#include "Parm.h"
#include "afxwin.h"
#define PRE_MULTIED_FACTOR_FOR_REAL_PARM 1000//传送过去的实际参数为避免浮点到整形的精度损失所做的预乘系数
// DetailSettingDlg 对话框

class CDetailSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailSettingDlg)

public:
	CDetailSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	CDetailSettingDlg(float RealSize,float RelVel,unsigned GunNum,float GunDis,float RegWidth,float RegHeight,int PerGunColor,CWnd* pParent=NULL);   // 构造函数
	virtual ~CDetailSettingDlg();

// 对话框数据
	enum { IDD = IDD_FURTHER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fRealVel;
	float m_fPaintCellHeightSizeReal;
	float m_fWorkRegWidth;
	float m_fWorkRegHeight;
	float m_fGUINumOfGun;
	float m_fGUIDisOfGun;
	int m_iColorPerGun;
	
	afx_msg void OnBnClickedBtnSetParm();

private:
	HWND m_hParentDialog;
		GunParm GetGunParmFunc(void);
/*!
 *@brief 获得对话框中所有的延迟数据并以消息形式发送出去
 *
 *@return
 *@todo
 */
	void PostAllGunDelayParm(void);
	// 每把喷枪对应的颜色数，默认为1
		GunParm m_sCurGunParm;///<枪的参数结构体后面要传出去的
	/*int m_iColorPer;*/
	//int m_iGunModelSele;
	
public:
	BOOL m_bGunHorizentalMount;
	
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	int m_iGunMod;  //用于判断选择了那种喷枪模式
	// 第一把喷枪的开启延时

	// 第一把喷枪的开启延时
	float m_FirstGunOpenDelay;
	// 第一把喷枪的关闭延时
	float m_FirstGunCloseDelay;
	float m_SecondGunOpenDelay;
	float m_SecondGunCloseDelay;
	float m_ThirdGunOpenDelay;
	float m_ThirdGunCloseDelay;
	float m_FourthGunOpenDelay;
	float m_FourthGunCloseDelay;
	// 1喷枪逆向延迟开数值
	double m_dInverseOpenDelay1;
	// 1枪逆向延迟关闭
	double m_dInverseCloseDelay1;
	// 2枪逆向延迟开
	double m_dInverseOpenDelay2;
	double m_dInverseCloseDelay2;
	// 3枪逆向延迟开
	double m_dInverseOpenDelay3;
	// 3枪逆向延迟关
	double m_dInverseCloseDelay3;
	// 4枪逆向延迟开
	double m_dInverseOpenDelay4;
	// 4枪逆向延迟关
	double m_dInverseCloseDelay4;
	CEdit m_fa_sd;
	CEdit m_fa_sjk;
	CEdit m_fa_gzqkd;
	CEdit m_fa_gzqgd;
	CEdit m_fa_pqs;
	CEdit m_fa_pqjj;
	CEdit m_fa_f_o_pqys;
	CEdit m_fa_f_c_pqys;
	CEdit m_fa_s_o_pqys;
	CEdit m_fa_s_c_pqys;
	CEdit m_fa_t_o_pqys;
	CEdit m_fa_t_c_pqys;
	CEdit m_fa_fo_o_pqys;
	CEdit m_fa_fo_c_pqys;
	CEdit m_inverse_first_opendelay;
	CEdit m_inverse_first_closedelay;
	CEdit m_inverse_second_opendelay;
	CEdit m_inverse_second_closedelay;
	CEdit m_inverse_third_opendelay;
	CEdit m_inverse_third_closedelay;
	CEdit m_inverse_forth_opendelay;
	CEdit m_inverse_forth_closedelay;
	afx_msg void OnBnClickedOk();
};

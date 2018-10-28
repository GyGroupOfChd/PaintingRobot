#pragma once
#include "Parm.h"
#include "afxwin.h"
#define PRE_MULTIED_FACTOR_FOR_REAL_PARM 1000//���͹�ȥ��ʵ�ʲ���Ϊ���⸡�㵽���εľ�����ʧ������Ԥ��ϵ��
// DetailSettingDlg �Ի���

class CDetailSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailSettingDlg)

public:
	CDetailSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CDetailSettingDlg(float RealSize,float RelVel,unsigned GunNum,float GunDis,float RegWidth,float RegHeight,int PerGunColor,CWnd* pParent=NULL);   // ���캯��
	virtual ~CDetailSettingDlg();

// �Ի�������
	enum { IDD = IDD_FURTHER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
 *@brief ��öԻ��������е��ӳ����ݲ�����Ϣ��ʽ���ͳ�ȥ
 *
 *@return
 *@todo
 */
	void PostAllGunDelayParm(void);
	// ÿ����ǹ��Ӧ����ɫ����Ĭ��Ϊ1
		GunParm m_sCurGunParm;///<ǹ�Ĳ����ṹ�����Ҫ����ȥ��
	/*int m_iColorPer;*/
	//int m_iGunModelSele;
	
public:
	BOOL m_bGunHorizentalMount;
	
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	int m_iGunMod;  //�����ж�ѡ����������ǹģʽ
	// ��һ����ǹ�Ŀ�����ʱ

	// ��һ����ǹ�Ŀ�����ʱ
	float m_FirstGunOpenDelay;
	// ��һ����ǹ�Ĺر���ʱ
	float m_FirstGunCloseDelay;
	float m_SecondGunOpenDelay;
	float m_SecondGunCloseDelay;
	float m_ThirdGunOpenDelay;
	float m_ThirdGunCloseDelay;
	float m_FourthGunOpenDelay;
	float m_FourthGunCloseDelay;
	// 1��ǹ�����ӳٿ���ֵ
	double m_dInverseOpenDelay1;
	// 1ǹ�����ӳٹر�
	double m_dInverseCloseDelay1;
	// 2ǹ�����ӳٿ�
	double m_dInverseOpenDelay2;
	double m_dInverseCloseDelay2;
	// 3ǹ�����ӳٿ�
	double m_dInverseOpenDelay3;
	// 3ǹ�����ӳٹ�
	double m_dInverseCloseDelay3;
	// 4ǹ�����ӳٿ�
	double m_dInverseOpenDelay4;
	// 4ǹ�����ӳٹ�
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

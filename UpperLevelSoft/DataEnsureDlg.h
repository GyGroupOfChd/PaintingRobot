#pragma once
#include "afxwin.h"


// DataEnsureDlg 对话框

class DataEnsureDlg : public CDialog
{
	DECLARE_DYNAMIC(DataEnsureDlg)

public:
	DataEnsureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DataEnsureDlg();

// 对话框数据
	enum { IDD = IDD_DATAENSUREDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//void OnInitDialog();
	CStatic m_ch_sd;
	afx_msg void OnBnClickedOk();
	CStatic m_ch_sjk;
	CStatic m_ch_gzqkd;
	CStatic m_ch_gzqgd;
	CStatic m_ch_pqs;
	CStatic m_ch_pqjj;
	CStatic m_ch_f_o_pqys;
	CStatic m_ch_f_c_pqys;
	CStatic m_ch_s_o_pqys;
	CStatic m_ch_s_c_pqys;
	CStatic m_ch_t_o_pqys;
	CStatic m_ch_t_c_pqys;
	CStatic m_ch_fo_o_pqys;
	CStatic m_ch_fo_c_pqys;
	CStatic m_inverse_F_Odelay;
	CStatic m_inverse_S_Cdelay;
	CStatic m_inverse_S_Odelay;
	CStatic m_inverse_F_Cdelay;
	CStatic m_inverse_T_Odelay;
	CStatic m_inverse_T_Cdelay;
	CStatic m_inverse_FOR_Odelay;
	CStatic m_inverse_FOR_Cdelay;
};

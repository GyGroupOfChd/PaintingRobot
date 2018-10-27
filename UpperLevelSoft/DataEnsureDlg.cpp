// DataEnsureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperLevelSoft.h"
#include "DataEnsureDlg.h"
#include "afxdialogex.h"
#include "DetailSettingDlg.h"

// DataEnsureDlg 对话框

IMPLEMENT_DYNAMIC(DataEnsureDlg, CDialog)

DataEnsureDlg::DataEnsureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DataEnsureDlg::IDD, pParent)
{

}

DataEnsureDlg::~DataEnsureDlg()
{
}

void DataEnsureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_SD, m_ch_sd);
	DDX_Control(pDX, IDC_STATIC_SJKD, m_ch_sjk);
	DDX_Control(pDX, IDC_STATIC_GZQKD, m_ch_gzqkd);
	DDX_Control(pDX, IDC_STATIC_GZQGD, m_ch_gzqgd);
	DDX_Control(pDX, IDC_STATIC_PQS, m_ch_pqs);
	DDX_Control(pDX, IDC_STATIC_PQJJ, m_ch_pqjj);
	DDX_Control(pDX, IDC_STATIC_F_O_PQYS, m_ch_f_o_pqys);
	DDX_Control(pDX, IDC_STATIC_F_C_PQYS, m_ch_f_c_pqys);
	DDX_Control(pDX, IDC_STATIC_S_O_PQYS, m_ch_s_o_pqys);
	DDX_Control(pDX, IDC_STATIC_S_C_PQYS, m_ch_s_c_pqys);
	DDX_Control(pDX, IDC_STATIC_T_O_PQYS, m_ch_t_o_pqys);
	DDX_Control(pDX, IDC_STATIC_T_C_PQYS, m_ch_t_c_pqys);
	DDX_Control(pDX, IDC_STATIC_FO_O_PQYS, m_ch_fo_o_pqys);
	DDX_Control(pDX, IDC_STATIC_FO_C_PQYS, m_ch_fo_c_pqys);
	DDX_Control(pDX, IDC_STATIC_F_O_PQYS2, m_inverse_F_Odelay);
	DDX_Control(pDX, IDC_STATIC_FO_C_PQYS2, m_inverse_S_Cdelay);
	DDX_Control(pDX, IDC_STATIC_F_O_PQYS3, m_inverse_S_Odelay);
	DDX_Control(pDX, IDC_STATIC_FO_C_PQYS3, m_inverse_F_Cdelay);
	DDX_Control(pDX, IDC_STATIC_F_O_PQYS4, m_inverse_T_Odelay);
	DDX_Control(pDX, IDC_STATIC_FO_C_PQYS4, m_inverse_T_Cdelay);
	DDX_Control(pDX, IDC_STATIC_F_O_PQYS5, m_inverse_FOR_Odelay);
	DDX_Control(pDX, IDC_STATIC_FO_C_PQYS5, m_inverse_FOR_Cdelay);
}
//void DataEnsureDlg::OnInitDialog()
//{
//	m_edit.SetWindowText(this->m_content);
//}


BEGIN_MESSAGE_MAP(DataEnsureDlg, CDialog)
	ON_BN_CLICKED(IDOK, &DataEnsureDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// DataEnsureDlg 消息处理程序




void DataEnsureDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

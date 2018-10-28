// DetailSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpperLevelSoft.h"
#include "DetailSettingDlg.h"
#include "afxdialogex.h"
#include "DataEnsureDlg.h"

// DetailSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CDetailSettingDlg, CDialog)

CDetailSettingDlg::CDetailSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailSettingDlg::IDD, pParent)
	, m_fRealVel(0.4)
	, m_fPaintCellHeightSizeReal(10)
	, m_fWorkRegWidth(1000)
	, m_fWorkRegHeight(1000)
	, m_fGUINumOfGun(4)
	, m_fGUIDisOfGun(60)
	, m_bGunHorizentalMount(FALSE)
	, m_iGunMod(0)
	
	, m_dInverseOpenDelay1(0)
	, m_dInverseCloseDelay1(0)
	, m_dInverseOpenDelay2(0)
	, m_dInverseCloseDelay2(0)
	, m_dInverseOpenDelay3(0)
	, m_dInverseCloseDelay3(0)
	, m_dInverseOpenDelay4(0)
	, m_dInverseCloseDelay4(-20)
{
	m_iColorPerGun=1;
	m_hParentDialog=pParent->GetSafeHwnd();     //���
	m_iGunMod=1;
	m_iColorPerGun=1;
	m_FirstGunOpenDelay=20;
	m_FirstGunCloseDelay=20;
	m_SecondGunOpenDelay=20;
	m_SecondGunCloseDelay=0;
	m_ThirdGunOpenDelay=20;
	m_ThirdGunCloseDelay=20;
	m_FourthGunOpenDelay=20;
	m_FourthGunCloseDelay=20;
}
CDetailSettingDlg::CDetailSettingDlg(float RealSize,float RelVel,unsigned GunNum,float GunDis,float RegWidth,float RegHeight,int PerGunColor,CWnd* pParent): CDialog(CDetailSettingDlg::IDD, pParent)
,m_fRealVel(RelVel)
, m_fPaintCellHeightSizeReal(RealSize)
, m_fWorkRegWidth(RegWidth)
, m_fWorkRegHeight(RegHeight)
, m_fGUINumOfGun(GunNum)
, m_fGUIDisOfGun( GunDis)
, m_iColorPerGun(PerGunColor)
, m_bGunHorizentalMount(FALSE)
{
	m_hParentDialog=pParent->GetSafeHwnd();
	//m_iColorPerGun=1;
	m_iGunMod=1;
	m_FirstGunOpenDelay=0;
	m_FirstGunCloseDelay=0;
	m_SecondGunOpenDelay=0;
	m_SecondGunCloseDelay=0;
	m_ThirdGunOpenDelay=0;
	m_ThirdGunCloseDelay=0;
	m_FourthGunOpenDelay=0;
	m_FourthGunCloseDelay=0;
	//m_fWorkRegHeight+=(m_fGUINumOfGun-1)*m_fGUIDisOfGun;
}
CDetailSettingDlg::~CDetailSettingDlg()
{
}

void CDetailSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_CELL_SIZE, m_fPaintCellHeightSizeReal);
	DDV_MinMaxFloat(pDX, m_fPaintCellHeightSizeReal, 2, 2000);
	DDX_Text(pDX, IDC_ED_VEL, m_fRealVel);
	DDV_MinMaxFloat(pDX, m_fRealVel, 0.000001, 2);
	DDX_Text(pDX, IDC_ED_REG_WIDTH, m_fWorkRegWidth);
	DDV_MinMaxFloat(pDX, m_fWorkRegWidth, 50, 8000);
	DDX_Text(pDX, IDC_ED_REG_HEIGHT, m_fWorkRegHeight);
	DDV_MinMaxFloat(pDX, m_fWorkRegHeight, 50, 5500);
	DDX_Text(pDX, IDC_ED_NUM_OF_GUN, m_fGUINumOfGun);
	DDX_Text(pDX, IDC_ED_DIS_OF_GUN, m_fGUIDisOfGun);
	DDV_MinMaxFloat(pDX, m_fGUIDisOfGun, 0, 2000);
	DDX_Check(pDX, IDC_CHK_HORIZAN, m_bGunHorizentalMount);
	DDX_Text(pDX, IDC_ED_NUM_OF_COLOR_PER_GUN, m_iColorPerGun);
	/*DDX_Text(pDX, IDC_ED_NUM_OF_COLOR_PER_GUN, m_iColorPer);*/
	//DDX_Radio(pDX, IDC_RADIO1, m_iGunModelSele);
	/*DDV_MinMaxInt(pDX, m_iGunModelSele, 0, 4);*/

	DDV_MinMaxFloat(pDX, m_fGUINumOfGun, 1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_FirstGunOpenDelay);
	DDV_MinMaxFloat(pDX, m_FirstGunOpenDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT4, m_FirstGunCloseDelay);
	DDV_MinMaxDouble(pDX, m_FirstGunCloseDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT5, m_SecondGunOpenDelay);
	DDV_MinMaxFloat(pDX, m_SecondGunOpenDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT6, m_SecondGunCloseDelay);
	DDV_MinMaxFloat(pDX, m_SecondGunCloseDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT7, m_ThirdGunOpenDelay);
	DDV_MinMaxFloat(pDX, m_ThirdGunOpenDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT8, m_ThirdGunCloseDelay);
	DDV_MinMaxFloat(pDX, m_ThirdGunCloseDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT9, m_FourthGunOpenDelay);
	DDV_MinMaxFloat(pDX, m_FourthGunOpenDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT10, m_FourthGunCloseDelay);
	DDV_MinMaxFloat(pDX, m_FourthGunCloseDelay, -100, 100);
	DDX_Text(pDX, IDC_EDIT_REV_1, m_dInverseOpenDelay1);
	DDX_Text(pDX, IDC_EDIT_REV_2, m_dInverseCloseDelay1);
	DDX_Text(pDX, IDC_EDIT_REV_3, m_dInverseOpenDelay2);
	DDX_Text(pDX, IDC_EDIT_REV_4, m_dInverseCloseDelay2);
	DDX_Text(pDX, IDC_EDIT_REV_5, m_dInverseOpenDelay3);
	DDX_Text(pDX, IDC_EDIT_REV_6, m_dInverseCloseDelay3);
	DDX_Text(pDX, IDC_EDIT_REV_7, m_dInverseOpenDelay4);
	DDX_Text(pDX, IDC_EDIT_REV_8, m_dInverseCloseDelay4);
	DDX_Control(pDX, IDC_ED_VEL, m_fa_sd);
	DDX_Control(pDX, IDC_ED_CELL_SIZE, m_fa_sjk);
	DDX_Control(pDX, IDC_ED_REG_WIDTH, m_fa_gzqkd);
	DDX_Control(pDX, IDC_ED_REG_HEIGHT, m_fa_gzqgd);
	DDX_Control(pDX, IDC_ED_NUM_OF_GUN, m_fa_pqs);
	DDX_Control(pDX, IDC_ED_DIS_OF_GUN, m_fa_pqjj);
	DDX_Control(pDX, IDC_EDIT2, m_fa_f_o_pqys);
	DDX_Control(pDX, IDC_EDIT4, m_fa_f_c_pqys);
	DDX_Control(pDX, IDC_EDIT5, m_fa_s_o_pqys);
	DDX_Control(pDX, IDC_EDIT6, m_fa_s_c_pqys);
	DDX_Control(pDX, IDC_EDIT7, m_fa_t_o_pqys);
	DDX_Control(pDX, IDC_EDIT8, m_fa_t_c_pqys);
	DDX_Control(pDX, IDC_EDIT9, m_fa_fo_o_pqys);
	DDX_Control(pDX, IDC_EDIT10, m_fa_fo_c_pqys);
	DDX_Control(pDX, IDC_EDIT_REV_1, m_inverse_first_opendelay);
	DDX_Control(pDX, IDC_EDIT_REV_2, m_inverse_first_closedelay);
	DDX_Control(pDX, IDC_EDIT_REV_3, m_inverse_second_opendelay);
	DDX_Control(pDX, IDC_EDIT_REV_4, m_inverse_second_closedelay);
	DDX_Control(pDX, IDC_EDIT_REV_5, m_inverse_third_opendelay);
	DDX_Control(pDX, IDC_EDIT_REV_6, m_inverse_third_closedelay);
	DDX_Control(pDX, IDC_EDIT_REV_7, m_inverse_forth_opendelay);
	DDX_Control(pDX, IDC_EDIT_REV_8, m_inverse_forth_closedelay);
}


BEGIN_MESSAGE_MAP(CDetailSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SET_PARM, &CDetailSettingDlg::OnBnClickedBtnSetParm)
	
	ON_BN_CLICKED(IDC_RADIO2, &CDetailSettingDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDetailSettingDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDetailSettingDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDOK, &CDetailSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// DetailSettingDlg ��Ϣ�������

void CDetailSettingDlg::PostAllGunDelayParm(void)
{
	unsigned GunOpenDelay,GunCloseDelay;
	GunOpenDelay=20000+m_FirstGunOpenDelay*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_FirstGunCloseDelay*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN0_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_SecondGunOpenDelay*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_SecondGunCloseDelay*100;//С�������λ,�������������������20000-��

	::PostMessage(m_hParentDialog,WMU_GUN1_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_ThirdGunOpenDelay*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_ThirdGunCloseDelay*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN2_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_FourthGunOpenDelay*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_FourthGunCloseDelay*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN3_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	//::PostMessage(m_hParentDialog,WMU_GUN0_DELAY,(WPARAM)pGunParm,m_sCurGunParm.ColoPerGun);
	///////////////////////��������///////////////////////////////////////////////////

	GunOpenDelay=20000+m_dInverseOpenDelay1*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_dInverseCloseDelay1*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN0_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay2*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_dInverseCloseDelay2*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN1_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay3*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_dInverseCloseDelay3*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN2_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay4*100;//С�������λ,�������������������20000-��
	GunCloseDelay=20000+m_dInverseCloseDelay4*100;//С�������λ,�������������������20000-��
	::PostMessage(m_hParentDialog,WMU_GUN3_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	//////////////////////////////////////////////////////////////////////////
}
void CDetailSettingDlg::OnBnClickedBtnSetParm()
{
	UpdateData(TRUE);
	if(((((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())||(((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())||(((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck()))!=1)
	{
		MessageBox("��ѡ����ǹģʽ!");
	}
	long RSize=m_fPaintCellHeightSizeReal*10*PRE_MULTIED_FACTOR_FOR_REAL_PARM;//  mm*1000
	long Vel=m_fRealVel*1000*PRE_MULTIED_FACTOR_FOR_REAL_PARM;//  mm/s*1000
	::PostMessage(m_hParentDialog,WMU_SetRealToMapRatioAndVel,RSize,Vel);
	m_sCurGunParm=GetGunParmFunc();
	void* pGunParm=(void*)&m_sCurGunParm;
	::PostMessage(m_hParentDialog,WMU_SetGunParm,(WPARAM)pGunParm,m_sCurGunParm.ColoPerGun);
	//////////////////////////////////////////////////////////////////////////
	PostAllGunDelayParm();
	long WorkRegWidth=m_fWorkRegWidth*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
	long WorkRegHeight=m_fWorkRegHeight*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
	if (!m_bGunHorizentalMount)//����װ�Ļ���Ҫ���Ĺ������߶ȣ���ʹ�˶��ܹ���������ǹ���꣬���絽��
	{
		WorkRegHeight=m_fWorkRegHeight*PRE_MULTIED_FACTOR_FOR_REAL_PARM-(m_fGUINumOfGun-1)*m_sCurGunParm.GunDis;
		//WorkRegHeight=m_fWorkRegHeight*PRE_MULTIED_FACTOR_FOR_REAL_PARM;  //
	}
	else{//����װ�Ļ���Ҫ���Ĺ�������ȣ���ʹ�˶��ܹ���������ǹ���꣬���絽��
		WorkRegWidth=(m_fWorkRegWidth-(m_fGUINumOfGun-1)*m_sCurGunParm.GunDis)*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
	}	
	::PostMessage(m_hParentDialog,WMU_SetWorkRegParm,WorkRegWidth,WorkRegHeight);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
GunParm CDetailSettingDlg::GetGunParmFunc(void)
{
	UpdateData(TRUE);
	UINT nSex; 
	GunParm sCurGunParm;
	sCurGunParm.CurgunMode=GunParm::GunModel(m_iGunMod+1);//Ҫ+1
	//nSex= GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3); 
	switch(m_iGunMod+1)
	{
	case 1: 
		/*long GunNum=
		long GunDis=*/
		sCurGunParm.GunNum=m_fGUINumOfGun*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
		sCurGunParm.GunDis=m_fGUIDisOfGun*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
		sCurGunParm.ColoPerGun=1;
		break;
	case 2:
		sCurGunParm.GunNum=1*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
		sCurGunParm.GunDis=0;
		sCurGunParm.ColoPerGun=m_iColorPerGun;
		break;
	case 3:
		sCurGunParm.GunNum=m_fGUINumOfGun*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
		sCurGunParm.GunDis=m_fGUIDisOfGun*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
		sCurGunParm.ColoPerGun=m_iColorPerGun;
		break;
	}
	return sCurGunParm;
}


void CDetailSettingDlg::OnBnClickedRadio2()
{
	m_iGunMod=1;
	CWnd* pNumOfGun = GetDlgItem(IDC_ED_NUM_OF_GUN ); 
	pNumOfGun->EnableWindow(FALSE);
	CWnd* pDisOfGun = GetDlgItem(IDC_ED_DIS_OF_GUN); 
	pDisOfGun->EnableWindow(FALSE);
	CWnd* pNumOfColoPerGun = GetDlgItem(IDC_ED_NUM_OF_COLOR_PER_GUN); 
	pNumOfColoPerGun->EnableWindow(TRUE);
}
void CDetailSettingDlg::OnBnClickedRadio3()
{
	m_iGunMod=2;
	CWnd* pNumOfGun = GetDlgItem(IDC_ED_NUM_OF_GUN ); 
	pNumOfGun->EnableWindow(TRUE);
	CWnd* pDisOfGun = GetDlgItem(IDC_ED_DIS_OF_GUN); 
	pDisOfGun->EnableWindow(TRUE);
	CWnd* pNumOfColoPerGun = GetDlgItem(IDC_ED_NUM_OF_COLOR_PER_GUN); 
	pNumOfColoPerGun->EnableWindow(TRUE);
}


void CDetailSettingDlg::OnBnClickedRadio4()
{
	m_iGunMod=0;
	CWnd* pNumOfGun = GetDlgItem(IDC_ED_NUM_OF_GUN ); 
	pNumOfGun->EnableWindow(TRUE);
	CWnd* pDisOfGun = GetDlgItem(IDC_ED_DIS_OF_GUN); 
	pDisOfGun->EnableWindow(TRUE);
	CWnd* pNumOfColoPerGun = GetDlgItem(IDC_ED_NUM_OF_COLOR_PER_GUN); 
	pNumOfColoPerGun->EnableWindow(FALSE);
}


void CDetailSettingDlg::OnBnClickedOk()
{
	//DataEnsureDlg dlg;
	//dlg.DoModal();
	DataEnsureDlg* dlg;//ģ̬�Ի����У���ô���£�
	dlg = new DataEnsureDlg;
	dlg->Create(IDD_DATAENSUREDLG); 
	dlg->ShowWindow(SW_SHOW);
	CString sd,sjk,gzqkd,gzqgd,pqs,pqjj,F_O_pqys,F_C_pqys,S_O_pqys,S_C_pqys,T_O_pqys,T_C_pqys,Fo_O_pqys,Fo_C_pqys,
		IN_F_Odelay,IN_F_Cdelay,IN_S_Odelay,IN_S_Cdelay,IN_T_Odelay,IN_T_Cdelay,IN_FOR_Odelay,IN_FOR_Cdelay;
	//ע������GetWindowText��SetWindowText��GetDlgItemText��SetDlgItemText�÷���
	m_fa_sd.GetWindowText(sd);//�ٶ�
	dlg->m_ch_sd.SetWindowText(sd);
	m_fa_sjk.GetWindowText(sjk);//ʵ�ʿ�
	dlg->m_ch_sjk.SetWindowText(sjk);
	m_fa_gzqkd.GetWindowText(gzqkd);//���������
	dlg->m_ch_gzqkd.SetWindowText(gzqkd);
	m_fa_gzqgd.GetWindowText(gzqgd);//�������߶�
	dlg->m_ch_gzqgd.SetWindowText(gzqgd);
	m_fa_pqs.GetWindowText(pqs);//��ǹ��Ŀ
	dlg->m_ch_pqs.SetWindowText(pqs);
	m_fa_pqjj.GetWindowText(pqjj);//��ǹ���
	dlg->m_ch_pqjj.SetWindowText(pqjj);
	m_fa_f_o_pqys.GetWindowText(F_O_pqys);//��һ����ǹ������ʱ
	dlg->m_ch_f_o_pqys.SetWindowText(F_O_pqys);
	m_fa_f_c_pqys.GetWindowText(F_C_pqys);//��һ����ǹ�ر���ʱ
	dlg->m_ch_f_c_pqys.SetWindowText(F_C_pqys);
	m_fa_s_o_pqys.GetWindowText(S_O_pqys);//�ڶ�����ǹ�Ŀ�����ʱ
	dlg->m_ch_s_o_pqys.SetWindowText(S_O_pqys);
	m_fa_s_c_pqys.GetWindowText(S_C_pqys);//�ڶ�����ǹ�ر���ʱ
	dlg->m_ch_s_c_pqys.SetWindowText(S_C_pqys);
	m_fa_t_o_pqys.GetWindowText(T_O_pqys);//��������ǹ�Ŀ�����ʱ
	dlg->m_ch_t_o_pqys.SetWindowText(T_O_pqys);
	m_fa_t_c_pqys.GetWindowText(T_C_pqys);//��������ǹ�ر���ʱ
	dlg->m_ch_t_c_pqys.SetWindowText(T_C_pqys);
	m_fa_fo_o_pqys.GetWindowText(Fo_O_pqys);//���İ���ǹ�Ŀ�����ʱ
	dlg-> m_ch_fo_o_pqys.SetWindowText(Fo_O_pqys);
	m_fa_fo_c_pqys.GetWindowText(Fo_C_pqys);//���İ���ǹ�Ĺر���ʱ
	dlg->m_ch_fo_c_pqys.SetWindowText(Fo_C_pqys);

	m_inverse_first_opendelay.GetWindowText(IN_F_Odelay);//�����һ�ѵĿ����ӳ�
	dlg->m_inverse_F_Odelay.SetWindowText(IN_F_Odelay);
	m_inverse_first_closedelay.GetWindowText(IN_F_Cdelay);//�����һ�ѵĹر��ӳ�
	dlg->m_inverse_F_Cdelay.SetWindowText(IN_F_Cdelay);
	m_inverse_second_opendelay.GetWindowText(IN_S_Odelay);//����ڶ��ѵĿ����ӳ�
	dlg->m_inverse_S_Odelay.SetWindowText(IN_S_Odelay);
	m_inverse_second_closedelay.GetWindowText(IN_S_Cdelay);//����ڶ��ѵĹر��ӳ�
	dlg->m_inverse_S_Cdelay.SetWindowText(IN_S_Cdelay);
	m_inverse_third_opendelay.GetWindowText(IN_T_Odelay);//��������ѵĿ����ӳ�
	dlg->m_inverse_T_Odelay.SetWindowText(IN_T_Odelay);
	m_inverse_third_closedelay.GetWindowText(IN_T_Cdelay);//��������ѵĹر��ӳ�
	dlg->m_inverse_T_Cdelay.SetWindowText(IN_T_Cdelay);
	m_inverse_forth_opendelay.GetWindowText(IN_FOR_Odelay);//������İѵĿ����ӳ�
	dlg->m_inverse_FOR_Odelay.SetWindowText(IN_FOR_Odelay);
	m_inverse_forth_closedelay.GetWindowText(IN_FOR_Cdelay);//������İѵĹر��ӳ�
	dlg->m_inverse_FOR_Cdelay.SetWindowText(IN_FOR_Cdelay);

	CString DirectoryFileDetailData="";
	DirectoryFileDetailData="\\DetailData.pmc";
	FILE *m_p=fopen("E:\\��������˵��.txt","w");
	fprintf(m_p,"����ǰ���õ���ǹ�ٶ��ǣ���/�룩��\n");
	sd=sd+'\n';
	fwrite(sd,1,strlen(sd),m_p);
	fprintf(m_p,"����ǰ���õĵ�Ԫʵ�ʿ���ǣ����ף���\n");
	sjk=sjk+'\n';
	fwrite(sjk,1,strlen(sjk),m_p);
	fprintf(m_p,"����ǰ���õĹ���������ǣ����ף���\n");
	gzqkd=gzqkd+'\n';
	fwrite(gzqkd,1,strlen(gzqkd),m_p);
	fprintf(m_p,"����ǰ���õĹ������߶��ǣ����ף���\n");
	gzqgd=gzqgd+'\n';
	fwrite(gzqgd,1,strlen(gzqgd),m_p);
	fprintf(m_p,"����ǰ���õ���ǹ��Ŀ�ǣ��ѣ���\n");
	pqs=pqs+'\n';
	fwrite(pqs,1,strlen(pqs),m_p);
	fprintf(m_p,"����ǰ���õ���ǹ����ǣ����ף���\n");
	pqjj=pqjj+'\n';
	fwrite(pqjj,1,strlen(pqjj),m_p);
	fprintf(m_p,"����ǰ���õĵ�һ����ǹ��˳�����ӳ��ǣ����ף���\n");
	F_O_pqys=F_O_pqys+'\n';
	fwrite(F_O_pqys,1,strlen(F_O_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ�һ����ǹ��˳��ر��ӳ��ǣ����ף���\n");
	F_C_pqys=F_C_pqys+'\n';
	fwrite(F_C_pqys,1,strlen(F_C_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵڶ�����ǹ��˳�����ӳ��ǣ����ף���\n");
	S_O_pqys=S_O_pqys+'\n';
	fwrite(S_O_pqys,1,strlen(S_O_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵڶ�����ǹ��˳��ر��ӳ��ǣ����ף���\n");
	S_C_pqys=S_C_pqys+'\n';
	fwrite(S_C_pqys,1,strlen(S_C_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ�������ǹ��˳�����ӳ��ǣ����ף���\n");
	T_O_pqys=T_O_pqys+'\n';
	fwrite(T_O_pqys,1,strlen(T_O_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ�������ǹ��˳��ر��ӳ��ǣ����ף���\n");
	T_C_pqys=T_C_pqys+'\n';
	fwrite(T_C_pqys,1,strlen(T_C_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ��İ���ǹ��˳�����ӳ��ǣ����ף���\n");
	Fo_O_pqys=Fo_O_pqys+'\n';
	fwrite(Fo_O_pqys,1,strlen(Fo_O_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ��İ���ǹ��˳��ر��ӳ��ǣ����ף���\n");
	Fo_C_pqys=Fo_C_pqys+'\n';
	fwrite(Fo_C_pqys,1,strlen(Fo_C_pqys),m_p);
	fprintf(m_p,"����ǰ���õĵ�һ����ǹ���������ӳ��ǣ����ף���\n");
	IN_F_Odelay=IN_F_Odelay+'\n';
	fwrite(IN_F_Odelay,1,strlen(IN_F_Odelay),m_p);
	fprintf(m_p,"����ǰ���õĵ�һ����ǹ������ر��ӳ��ǣ����ף���\n");
	IN_F_Cdelay=IN_F_Cdelay+'\n';
	fwrite(IN_F_Cdelay,1,strlen(IN_F_Cdelay),m_p);
	fprintf(m_p,"����ǰ���õĵڶ�����ǹ���������ӳ��ǣ����ף���\n");
	IN_S_Odelay=IN_S_Odelay+'\n';
	fwrite(IN_S_Odelay,1,strlen(IN_S_Odelay),m_p);
	fprintf(m_p,"����ǰ���õĵڶ�����ǹ������ر��ӳ��ǣ����ף���\n");
	IN_S_Cdelay=IN_S_Cdelay+'\n';
	fwrite(IN_S_Cdelay,1,strlen(IN_S_Cdelay),m_p);
	fprintf(m_p,"����ǰ���õĵ�������ǹ���������ӳ��ǣ����ף���\n");
	IN_T_Odelay=IN_T_Odelay+'\n';
	fwrite(IN_T_Odelay,1,strlen(IN_T_Odelay),m_p);
	fprintf(m_p,"����ǰ���õĵ�������ǹ������ر��ӳ��ǣ����ף���\n");
	IN_T_Cdelay=IN_T_Cdelay+'\n';
	fwrite(IN_T_Cdelay,1,strlen(IN_T_Cdelay),m_p);
	fprintf(m_p,"����ǰ���õĵ��İ���ǹ���������ӳ��ǣ����ף���\n");
	IN_FOR_Odelay=IN_FOR_Odelay+'\n';
	fwrite(IN_FOR_Odelay,1,strlen(IN_FOR_Odelay),m_p);
	fprintf(m_p,"����ǰ���õĵ��İ���ǹ������ر��ӳ��ǣ����ף���\n");
	IN_FOR_Cdelay=IN_FOR_Cdelay+'\n';
	fwrite(IN_FOR_Cdelay,1,strlen(IN_FOR_Cdelay),m_p);


	fclose(m_p);
	FILE *m_p_read=fopen("E:\\DetailDataRead.txt","w");
	fwrite(sd,1,strlen(sd),m_p_read);
	fwrite(sjk,1,strlen(sjk),m_p_read);
	fwrite(gzqkd,1,strlen(gzqkd),m_p_read);
	fwrite(gzqgd,1,strlen(gzqgd),m_p_read);
	fwrite(pqs,1,strlen(pqs),m_p_read);
	fwrite(pqjj,1,strlen(pqjj),m_p_read);
	fwrite(F_O_pqys,1,strlen(F_O_pqys),m_p_read);
	fwrite(F_C_pqys,1,strlen(F_C_pqys),m_p_read);
	fwrite(S_O_pqys,1,strlen(S_O_pqys),m_p_read);
	fwrite(S_C_pqys,1,strlen(S_C_pqys),m_p_read);
	fwrite(T_O_pqys,1,strlen(T_O_pqys),m_p_read);
	fwrite(T_C_pqys,1,strlen(T_C_pqys),m_p_read);
	fwrite(Fo_O_pqys,1,strlen(Fo_O_pqys),m_p_read);
	fwrite(Fo_C_pqys,1,strlen(Fo_C_pqys),m_p_read);
	fwrite(IN_F_Odelay,1,strlen(IN_F_Odelay),m_p_read);
	fwrite(IN_F_Cdelay,1,strlen(IN_F_Cdelay),m_p_read);
	fwrite(IN_S_Odelay,1,strlen(IN_S_Odelay),m_p_read);
	fwrite(IN_S_Cdelay,1,strlen(IN_S_Cdelay),m_p_read);
	fwrite(IN_T_Odelay,1,strlen(IN_T_Odelay),m_p_read);
	fwrite(IN_T_Cdelay,1,strlen(IN_T_Cdelay),m_p_read);
	fwrite(IN_FOR_Odelay,1,strlen(IN_FOR_Odelay),m_p_read);
	fwrite(IN_FOR_Cdelay,1,strlen(IN_FOR_Cdelay),m_p_read);

	fclose(m_p_read);
	CDialog::OnOK();

}

// DetailSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperLevelSoft.h"
#include "DetailSettingDlg.h"
#include "afxdialogex.h"


// DetailSettingDlg 对话框

IMPLEMENT_DYNAMIC(CDetailSettingDlg, CDialog)

CDetailSettingDlg::CDetailSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailSettingDlg::IDD, pParent)
	, m_fRealVel(0.1)
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
	, m_dInverseCloseDelay4(0)
{
	m_iColorPerGun=1;
	m_hParentDialog=pParent->GetSafeHwnd();     //句柄
	m_iGunMod=1;
	m_iColorPerGun=1;
	m_FirstGunOpenDelay=12;
	m_FirstGunCloseDelay=0;
	m_SecondGunOpenDelay=-23.4;
	m_SecondGunCloseDelay=40;
	m_ThirdGunOpenDelay=30;
	m_ThirdGunCloseDelay=0;
	m_FourthGunOpenDelay=0;
	m_FourthGunCloseDelay=0;
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
}


BEGIN_MESSAGE_MAP(CDetailSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SET_PARM, &CDetailSettingDlg::OnBnClickedBtnSetParm)
	
	ON_BN_CLICKED(IDC_RADIO2, &CDetailSettingDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDetailSettingDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDetailSettingDlg::OnBnClickedRadio4)
END_MESSAGE_MAP()


// DetailSettingDlg 消息处理程序

void CDetailSettingDlg::PostAllGunDelayParm(void)
{
	unsigned GunOpenDelay,GunCloseDelay;
	GunOpenDelay=20000+m_FirstGunOpenDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_FirstGunCloseDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN0_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_SecondGunOpenDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_SecondGunCloseDelay*100;//小数点后两位,因数据有正负，因此用20000-它

	::PostMessage(m_hParentDialog,WMU_GUN1_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_ThirdGunOpenDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_ThirdGunCloseDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN2_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	GunOpenDelay=20000+m_FourthGunOpenDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_FourthGunCloseDelay*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN3_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	//::PostMessage(m_hParentDialog,WMU_GUN0_DELAY,(WPARAM)pGunParm,m_sCurGunParm.ColoPerGun);
	///////////////////////逆向数据///////////////////////////////////////////////////

	GunOpenDelay=20000+m_dInverseOpenDelay1*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_dInverseCloseDelay1*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN0_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay2*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_dInverseCloseDelay2*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN1_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay3*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_dInverseCloseDelay3*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN2_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);

	GunOpenDelay=20000+m_dInverseOpenDelay4*100;//小数点后两位,因数据有正负，因此用20000-它
	GunCloseDelay=20000+m_dInverseCloseDelay4*100;//小数点后两位,因数据有正负，因此用20000-它
	::PostMessage(m_hParentDialog,WMU_GUN3_REV_DELAY,(WPARAM) GunOpenDelay,(LPARAM) GunCloseDelay);
	//////////////////////////////////////////////////////////////////////////
}
void CDetailSettingDlg::OnBnClickedBtnSetParm()
{
	UpdateData(TRUE);
	if(((((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())||(((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())||(((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck()))!=1)
	{
		MessageBox("请选择喷枪模式!");
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
	if (!m_bGunHorizentalMount)//竖向安装的话需要更改工作区高度，以使运动能够让所有喷枪走完，都喷到。
	{
		WorkRegHeight=m_fWorkRegHeight*PRE_MULTIED_FACTOR_FOR_REAL_PARM-(m_fGUINumOfGun-1)*m_sCurGunParm.GunDis;
		//WorkRegHeight=m_fWorkRegHeight*PRE_MULTIED_FACTOR_FOR_REAL_PARM;  //
	}
	else{//横向安装的话需要更改工作区宽度，以使运动能够让所有喷枪走完，都喷到。
		WorkRegWidth=(m_fWorkRegWidth-(m_fGUINumOfGun-1)*m_sCurGunParm.GunDis)*PRE_MULTIED_FACTOR_FOR_REAL_PARM;
	}	
	::PostMessage(m_hParentDialog,WMU_SetWorkRegParm,WorkRegWidth,WorkRegHeight);

	// TODO: 在此添加控件通知处理程序代码
}
GunParm CDetailSettingDlg::GetGunParmFunc(void)
{
	UpdateData(TRUE);
	UINT nSex; 
	GunParm sCurGunParm;
	sCurGunParm.CurgunMode=GunParm::GunModel(m_iGunMod+1);//要+1
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


// UpperLevelSoftDlg.h : 头文件
//
#pragma once
#include "RecognizePicture.h"
#include "Parm.h"
#include "DetailSettingDlg.h"
#include "ResultAdapter.h"
#include "CButtonST文件/BtnST.h"
#include "./CButtonST文件/WinXPButtonST.h"
#include "./CButtonST文件/ShadeButtonST.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "mscomm.h"
#include "PlcCommu.h"
#include "afxwin.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "..\upperlevelsoft\cbuttonst文件\btnst.h"
#include "PaintDlg.h"
#include "DownParmValue.h"
#include "LowMoniter.h"
#include "SharpRed.h"
#include "math.h"
#include ".\pmac\runtime.h"
#include ".\pmac\Include\dialog.h"
#include ".\PMAC\Include\UtilDT.h"

//#include "ParkerMotionCardCommu.h"
UINT ServerRecvThread(LPVOID lpParm );
UINT ClientRecvThread(LPVOID lpParm );     //红外用的线程
struct JudgeIsOperationOrder
{
	BOOL ReadPicyureBtnDown;
	BOOL InvalidColorBtnDown;
	BOOL ReconfigureBtnDown;
	BOOL DrawAreaIdentificationResultsBtnDown;
	BOOL ConnectBtnDown;
	BOOL DesignatedWorkAreaBtnDown;
	BOOL IsWashGunBtnDown;
};
struct CameraPosition
{
	float FirstCameraX;
	float FirstCameraY;
	bool  Firstcameradignose;
	float SecondCameraX;
	float SecondCameraY;
	bool  Secondcameradignose;
};

// CUpperLevelSoftDlg 对话框
class CUpperLevelSoftDlg : public CDialogEx
{
// 构造
public:
	CUpperLevelSoftDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPPERLEVELSOFT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1ReadPic();
private:
	bool InitSystemParm(bool ReInit=false);
	bool ReadPicture2(bool FirstUSE,bool bIsRead);
	void InitPaintMachineParm(bool ReInit=false);
	bool ShowBMP2(bool bFirstShow=true,bool bIsRead=true);
	long OnSetVelAndRealRatio(WPARAM CellRealWidth,LPARAM Vel);
	long OnSetGunParm (WPARAM GunP,LPARAM GunMode);
	bool DistinctMap(CDC* pTargetDC);
	void EraseAll(CDC* pTargetDC,CRect& rect);
	bool ShowRecResOfGivenReg(CPoint& ChosedPoint);
	bool SelectRegDownLoadParm(CPoint& ChosedPoint);
	bool ShowRecResOnMap(CDC* pTargetDC,unsigned IndexOfReg);
	void StorePictureToShow();               //当PICTURE图片控件还没有加载图片时，用于显示动画图片，此函数用于加载图片，放在一个vector中
	int ShowRecResultDat(void);
	int ShowColorReced(void);
	void DownLoadPlcParm(unsigned RegIndex);
	bool DownLoadParmToMotionControlCard(void);
	void TranslateAllParmToNewStlye(void)
	{
		m_DownValue.GetDownValueMotion(m_itMotionParmVecBegin,m_itMotionParmVecEnd);           //17.12.19修改
		m_DownValue.GetDownValuePlc(m_Storage,m_cResAdapter.ReturnChangedColorPointBegin(),m_cResAdapter.ReturnChangedColorPointEnd(),m_uGunNum,m_fGunDis); //向下位机传PLC参数

	}
	CResultAdapter<CRecognizePicture,CPaintCell>::PLC_Parm_It_TYPE m_itPlcParmVecBegin;///<PLC参数序列头
	CResultAdapter<CRecognizePicture,CPaintCell>::PLC_Parm_It_TYPE m_itPlcParmVecEnd;///<PLC参数序列尾
	std::vector<int> m_Storage;///<PLC参数转存
	MotionParm_It_TYPE  m_itMotionParmVecBegin;//<运动参数序列头
	MotionParm_It_TYPE m_itMotionParmVecEnd;///<运动参数序列尾
	/**
*@brief 设置实际速度以及图片像素到实际的比例
*
*本函数中进行了参数的转化，Vel由mm/s转化为转/s,并将结果输入到CResultAdapter类中。注意避免浮点到整形导致的精度损失这里预乘了1000
*
*@parm WPARAM CellRealWidth 实际中单个单元的宽度 mm,为避免浮点到整形导致的精度损失这里预乘了1000。
*@parm WPARAM Vel 实际中运行速度mm/s,为避免浮点到整形导致的精度损失这里预乘了1000。
*
*@return
*@todo
*/
	
	/**
	*@brief 设置实际枪的参数
	*
	*本函数中进行了参数的转化，Vel由mm/s转化为转/s,并将结果输入到CResultAdapter类中。注意避免浮点到整形导致的精度损失这里预乘了1000
	*
	*@parm WPARAM GunNum 枪数,为避免浮点到整形导致的精度损失这里预乘了1000。
	*@parm WPARAM GunDis枪距,为避免浮点到整形导致的精度损失这里预乘了1000。
	*
	*@return
	*@todo
	*/
	//long OnSetGunParm (WPARAM GunNum,LPARAM GunDis);
	/*@brief 设置实际枪的参数
	*
	*与前一版本不同，这里传入的参数是一个结构体的指针，这样来通过消息传递更多参数。但是有一个潜在问题
	*本消息处理时要求指针有效，然而这点不能保证。
	*本函数中进行了参数的转化，Vel由mm/s转化为转/s,并将结果输入到CResultAdapter类中。注意避免浮点到整形导致的精度损失这里预乘了1000
	*
	*@parm WPARAM GunP GunParm结构体的指针，指向发消息方的一个GunParm结构变量，其中内容为所有的枪参数，其中枪数和枪距预先乘了1000。
	*@parm WPARAM GunMode喷枪选择的模式，1为多喷枪，2为单，3为多把多色喷枪。
	*
	*@return
	*@todo
	*/
	long SavePmcDiretory(WPARAM Value_Gun,LPARAM descirble);
	long OnSetWorkRegParm (WPARAM WorkRegWidth,LPARAM WorkRegHeight);
	
private:
	float m_fRealRatio;//
	float m_fYRealRatio;     //Y轴上像素与实际尺寸比例系数
	float m_fRealVel;
	CDC CPicDC;//原始图片
	CResultAdapter<CRecognizePicture,CPaintCell> m_cResAdapter;///<将识别结果按要求调整的类实例
	bool m_bLowerConnectStatu;///<与下位机的连接状态，PLC,运动控制卡都连上则为真，否则为假
	void InterfaceBeautification();          //界面美化
	DownValue m_DownValue;                            //下载参数类
	#ifdef USE_CONTENT_IN_DEPENDENT_CLASS 
	//CParkerMotionCardCommu m_cMotionCommu;///<具体通讯的实现内容
	#endif
public:
	JudgeIsOperationOrder m_JudgeIsOperationOrder;
	unsigned int m_uRegToBeDownLoad;///<需要下载参数的工作区
	/*int m_iGUIValOfCellWidth;
	int m_iGUIValOfCellHeight;
	int m_iGUINumOfRecedCell;
	int m_iGUIColorNum;*/
	
public:
	unsigned m_uGunNum;
	float m_fGunDis;
	int m_PerGunColor;
	unsigned m_fWorkRegWidth;///<工作区宽度大小，单位像素
	unsigned m_fWorkRegHeight;///<工作区高度大小，单位像素
	CRecognizePicture m_cRecgnizePic;
	/*CButtonST m_btnColor1;
	CButtonST m_btnColor2;
	CButtonST m_btnColor3;
	CButtonST m_btnColor4;
	CButtonST m_btnColor5;
	CButtonST m_btnColor6;*/
	CButtonST m_ReadPicture;
	CWinXPButtonST m_ReadPicture1;
	CShadeButtonST m_ReadPicture2;
	CWinXPButtonST m_HighDesign;
	/*CButtonST m_cNullColorBtn;*/
	#define CELL_COLOR RGB(0,10,0)
	CRect m_ClientRect;  
	std::vector<CString>::iterator m_StorePictureIterator;
	std::vector<CString> m_StorePicture;              //存放用于显示动画图片的地址
private:
	struct SRatioOfShow2Map
	{
		double xRatio;///<显示上的x除以图片中的X得到的比例
		double yRatio;///<显示上的y除以图片中的y得到的比例
	}m_sSHow2MapRatio;///<界面上显示图片时是经过缩放的，这个表示界面上显示的坐标与地图之间的比例关系。
	void TransforShowCoordIntoMapCoord(CPoint & PointOnShow)
	{
		PointOnShow.x=PointOnShow.x*m_sSHow2MapRatio.xRatio;
		PointOnShow.y=PointOnShow.y*m_sSHow2MapRatio.yRatio;
	}
	//double m_SHow2MapRatio;
	bool m_bSetNullColor;///<当前需要选取并设置无效颜色
	bool m_bSelectRegToDown;///<本标志位为真表示接下来需要选定工作区，以将其参数下载
	bool m_bUSE_CRossCursor;///<需要把鼠标设置为十字形
	bool m_bChoseRegToShow;///<要指定识别出的工作区以供显示
	bool m_bReadedPic;///<读入了图片，可以进行识别等操作了
	bool m_bRecedPic;///<进行了识别。本变量将不在InitSystemParm函数中被初始化，更改了。必须在外面初始化
	//bool m_bRecedResultValid;
	//bool m_bFirstShow;///<是否第一次显示图片？
	CBitmap m_cLoadedMap;///<装载的图片
	double X_Correct;
	double Y_Correct;
	double Angle_Correct;
public:
	afx_msg void OnBnClickedDetailSetting();
	afx_msg void OnBnClickedButton2SetNullCollor();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CButtonST m_cNullColorBtn;
	CButtonST m_cGUI_ConectBtn;
	CPlcCommu m_cPlcCommu;///<PLC通讯实例
	CMscomm m_cmscom;///<串口通讯实例
	unsigned int m_uCommPortNum_PLC;///<与PLC通讯所用的串口端口号
	int m_iGUISelectRegOrAll;///<选择下载一个区的参数(0)或全部工作区的参数，
public:
	void GetOwnSelfIp();
	void CreateServer();
	bool m_bServerCreated;
	bool m_bTerminateThread;
	SOCKET m_sockServer;
	// 服务器端口号
	unsigned short m_nPortNO;
	struct sockaddr_in m_clientAddr;
	CString CompileCameraParm(CString InputMessage,int num);
	CButton m_RecButton;
	afx_msg void OnBnClickedRecgnizeForMachine();
	CButtonST m_btnColor1;
	CButtonST m_btnColor2;
	CButtonST m_btnColor3;
	CButtonST m_btnColor4;
	CButtonST m_btnColor5;
	CButtonST m_btnColor6;
	afx_msg void OnBnClickedButton2SetRegionToShow();
	afx_msg void OnBnClickedRaRegToDown();
	afx_msg void OnBnClickedBtnConnectDown();
	afx_msg void OnBnClickedSelectReg();
	afx_msg void OnBnClickedDownParm();
	afx_msg void OnBnClickedAction();
	int m_iGUINumOfRecedCell;
	int m_iGUIColorNum;
	int m_iGUIValOfCellWidth;
	int m_iGUIValOfCellHeight;
	CString SaveDirectoryUpFile; 
	SharpRedMeasure m_SharpRedMeasure; //红外传感器类实例化
	afx_msg void OnBnClickedLowMoniter();
//#ifdef USE_CONTENT_IN_DEPENDENT_CLASS 
//	CParkerMotionCardCommu m_cMotionCommu;///<具体通讯的实现内容
//#endif

	afx_msg void OnBnClickedOpensharp();
	int c;
	void JudgeFloorIsFlat(int FirstDistance,int SecondDistance,int ThirdDistance);
	CString m_JudgeFloorIsFlatL;
	int m_SharpJudgeThreshold; //红外阈值
private:
	////////////////////////////////////
	//以下所有的函数和变量都是用作与下位机通信用的
	bool AGV_Init(DWORD* dwDevice);
	bool bAGV_InitState;//AGV初始化标志位，成功为TRUE,失败为FALSE
	long OnBnClickedPumppowerOn (WPARAM ValueOne,LPARAM ValueTwo);  //给泵机供电
	long OnBnClickedStartmotorOn(WPARAM ValueOne,LPARAM ValueTwo); //给电机供电 
	long OnBnClickedPumppowerOff(WPARAM ValueOne,LPARAM ValueTwo);  //给泵机供电
	long OnBnClickedStartmotorOff(WPARAM ValueOne,LPARAM ValueTwo); //给电机供电 
	long OnBnClickedReturn(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedQuit(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedDownload(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedRun(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedStop(WPARAM ValueOne,LPARAM ValueTwo);     //暂停响应函数
	long OnBnClickedStopall(WPARAM ValueOne,LPARAM ValueTwo);  //停止响应函数
	long OnBnClickedContinue(WPARAM ValueOne,LPARAM ValueTwo); //继续响应函数
	long OnBnClickedWash(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedStopwash(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnClickedAlignment(WPARAM ValueOne,LPARAM ValueTwo);
	long OnBnReleaseAlignment(WPARAM ValueOne,LPARAM ValueTwo);
	//JudgeUnitIsOutOfRange()函数用来判断在当前坐标系下，但喷涂单元是否超出喷涂极限,对于每一个待喷涂区域，在生成pmc文件之前都到计算是否超出限程
	CoordinateFrame JudgeUnitIsOutOfRange(float FirstCameraX,float FirstCameraY,float SecondCameraX,float SecondCameraY,int UintWidth,int Width,int Height,bool OneGun=true);
	CoordinateFrame m_CoordinateFrame;
	bool bInitState;
	DWORD mydwDevice;//AGV_Init函数的形参
	CString m_Connect;
	bool m_Connected;
	//////////////////////////////////
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedRaRegToDown1();
	int m_PointWorkSize;
	CameraPosition m_CameraPosition;
	float PerUnitWidth;
protected:
	double m_fGunDelay[4][2] ;//先开，后关,每行一把喷枪，正向运动时的单独延时
	double m_fRevGunDelay[4][2];//先开，后关,每行一把喷枪,逆向运动时的单独延时
	long OnSetGun0DelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun0Delay[2] ;//先开，后关
	long OnSetGun1DelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun1Delay[2] ;//先开，后关
	long OnSetGun2DelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun2Delay[2] ;//先开，后关
	long OnSetGun3DelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun3Delay[2] ;//先开，后关
	long OnSetGun0RevDelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun0Delay[2] ;//先开，后关
	long OnSetGun1RevDelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun1Delay[2] ;//先开，后关
	long OnSetGun2RevDelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
	//float m_fGun2Delay[2] ;//先开，后关
	long OnSetGun3RevDelayParm (WPARAM OpenDelay,LPARAM CloseDelay);
protected:
		bool UpdateAllTextOnColorRecBtn(void);
	/**
	*@brief 将数字，查找出该数字作为内容的字符串，返回指针
	*
	*
	*@parm
	*
	*@return
	*@todo
	*/
	char *strFromUns(unsigned u)   
	{   
		static char strDigits[] = "987654321";  
		/* 5个字符+’\0’ */   
		char *pch;
		/* u 超出范围了吗？使用UlongToStr … */  
		ASSERT(u<=65535);  
		/* 将每位数字自后向前存储在strDigits中 */  
		pch = &strDigits[10];  
		ASSERT(*pch == '\0');  
		do   
		*--pch = u%10 + '0';  
		while((u/=10)>0);  
		return(pch);  
	}
	bool ShowColorIndexInButonText(unsigned IndexOfRecedColor);
	afx_msg void OnBnClickedButton2ColorReced2();
	//int ShowColorReced(void);
	afx_msg void OnBnClickedButton2ColorReced();
	afx_msg void OnBnClickedButton2ColorReced3();
	afx_msg void OnBnClickedButton2ColorReced4();
	afx_msg void OnBnClickedButton2ColorReced5();
	afx_msg void OnBnClickedButton2ColorReced6();
};

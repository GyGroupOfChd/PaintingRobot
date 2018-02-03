
// CameraCorrectDlg.h : 头文件
//

#pragma once
#include  "windows.h"

//#include ".\CameraCorrect\MyDib.h"
//#include ".\CameraCorrect\256Dib.h"
#include "MyDib.h"
#include "256Dib.h"
#include "svPTGrab2.h"
#include "RoadLineDetect.h"
#include "RoadRecOnHalconWithPtgrey.h"
#include "ProTime.h"
#include "afxcmn.h"
UINT ClientRecvThread(LPVOID lpParm );
// CCameraCorrectDlg 对话框
class CCameraCorrectDlg : public CDialogEx
{
// 构造
public:
	CCameraCorrectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CameraCorrect_DIALOG };

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
	//float m_X_Correct;
	BOOL m_JudgeCameraFirst;
	BOOL m_JudgeCameraTwoFirst;
	CString m_Y_Correct;
	CString m_X_Correct;
	CString m_Angle_Correct;
	CString m_Y_CorrectTwo;
	CString m_X_CorrectTwo;
	ThresholdSlide m_Thresholderl;
	afx_msg void OnBnClickedOpenCamera();
	bool InitHalconDisp(void);
	bool InitHalconDispTwo(void);  //初始化第二个相机
	void ConnectCamera();          //连接（或打开）相机
	void ConnectCameraTwo();       //连接（或打开）第二个相机
	void ConnectServer();
	void SendValue(CString parm);
	static void WINAPI GrabDisplayCallback(void* pOwner,PTStatus status, unsigned char* pImgData, long lWidth, long lHeight, PTPixelFormat pixelFormat);
	static void WINAPI GrabDisplayCallbackTwo(void* pOwnerTwo,PTStatus statusTwo, unsigned char* pImgDataTwo, long lWidthTwo, long lHeightTwo, PTPixelFormat pixelFormatTwo);
	void grab();
	void grabTwo();
	void DrawImage(int nWidth,int nHeight);
	void DrawImage2(int nWidth,int nHeight);
	void DrawImageTwo(int nWidthTwo,int nHeightTwo);   //第二个相机显示原图
	void CreateMyDibFromMemory(PBYTE pImage, int nWidth, int nHeight);
	void CreateMyDibFromMemoryTwo(PBYTE pImageTwo, int nWidthTwo, int nHeightTwo);     //第二个相机显示原图
	bool RoadRec(unsigned char* pImgData, long lWidth, long lHeight);
	bool RoadRecTwo(unsigned char* pImgDataTwo, long lWidthTwo, long lHeightTwo);
	// 关闭其他含线程的实体，避免出现线程退出时间不同步导致的错误
	bool DestroyOtherThreadsObj(void);
public:
	bool m_bTerminateThread;
	char * IP;
	CString tempStr;
	// 客户端socket
	SOCKET m_sockClient;
	struct sockaddr_in m_serverAddr;
	unsigned short m_nServerPortNo;
	CString m_strSendData;  //发送数据存放数组
	char *m_strSendDatachar;
	int m_lWidth;
	int m_lHeight;
	int m_lWidthTwo;        //第二个相机像素宽度
	int m_lHeightTwo;		//第二个相机像素高度
	C256Dib m_dib1;
	CMyDib  *m_DibPict;
	scPTGrab2* m_Acq;
	CMyDib  *m_DibPictTwo;                //第二个相机实例化的类
	scPTGrab2 *m_AcpTwo;
	BOOL m_bIsGrab;	
	BOOL m_bIsGrabTwo;
	bool m_bDestroyNow;///<当采集、处理线程开始退出时本值为真，避免出现线程退出不同步导致的问题。
	bool m_bDestroyNowTwo; ///<当采集、处理线程开始退出时本值为真，避免出现线程退出不同步导致的问题。
	CMyDib*			m_pMyDib;								//DIB指针
	CMyDib*         m_pMyDibTwo;                            //DIB指针
	LocationCorrect m_CameraCorrectX_Y_Angle;
	LocationCorrect m_CameraCorrectX_Y_AngleTwo;
	CProTime  m_time;
	CProTime m_timeTwo;
	CRoadRecOnHalconWithPtgrey m_HalconRoadRec;//识别模块
	CRoadLineDetect  m_HalconRoadDet;
	bool m_bSoftTraggerOn;
	CSliderCtrl m_ThresholdMinGrey;
	CSliderCtrl m_ThresholdMaxGrey;
	ThresholdSlide m_ThresholdSliderl;
	ThresholdSlide m_ThresholdSliderlTwo;
	afx_msg void OnNMCustomdrawThresholdmingrey(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawThresholdmaxgrey(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	CString StreamBegin;
	CString StreamEnd;
	CWinThread *m_CaptureThread;
	afx_msg void OnNMCustomdrawThresholdmintwo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawThrehsoldmaxtwo(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_ThresholdMinGreyTwo;
	CSliderCtrl m_ThresholdMaxGreyTwo;
	double Angle;
};

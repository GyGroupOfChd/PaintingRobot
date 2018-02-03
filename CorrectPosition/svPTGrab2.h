// scPTGrab.h: interface for the scPTGrab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_)
#define AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\FlyCapture\include\FlyCapture2.h"
//#include "svImage.h"
#include <memory>
#include <map>
using namespace FlyCapture2;

#pragma pack(push,8)

// #ifdef SV_PTGRAB2_EXPORTS
// #define SV_PTGRAB2_API _declspec(dllexport)
// #else
// #define SV_PTGRAB2_API _declspec(dllimport)
// #endif

#define PTCLOCK_FREQ (1024000.0) // PT���ʱ��Ƶ�ʣ�DragonFly����

// ����ӿ�����
typedef enum tagPTCameraType 
{
	eInterfaceUnknow = 0,		    // δ֪�ӿ�
	eIEEE1394 = 1,                  // 1394�ӿ�
	eGigE,						    // GigE�ӿ�
	eUSB2,						    // USB2.0�ӿ�
	eUSB3						    // USB3.0�ӿ�
}PTCameraType;

// �����Ϣ
typedef struct tagPTCameraInfo
{
	unsigned int m_uiIndex;         // ���
	unsigned int m_uiSerialNum;     // ���к�
	PTCameraType m_CameraType;      // �������
	IPAddress	 m_ipAddress;	    // IP��ַ
	IPAddress    m_subnetMask;      // ��������
	IPAddress    m_defaultGateway;  // Ĭ������
	char		 m_cModelName[512];	// ����ͺ�
	char		 m_cResolution[512];// ����ֱ���
	tagPTCameraInfo()
	{
		m_uiIndex = -1;
		m_uiSerialNum = 0;
		memset(m_cModelName, 0, 512);
		memset(m_cResolution, 0, 512);
	}
}PTCameraInfo, *pPTCameraInfo ;

// Format7��ʽ
typedef struct tagPTFormat7Info
{
	Mode         m_mode;			// ģʽ
	unsigned int m_uiStartX;        // ��ʼX
	unsigned int m_uiStartY;		// ��ʼY
	unsigned int m_uiWidth;         // ����
	unsigned int m_uiHeight;		// �߶�
	float        m_fBusSpeed;       // ������
	PixelFormat  m_pixelFormat;     // ���ظ�ʽ
	
	tagPTFormat7Info()
	{
		m_mode			= MODE_0;		
		m_uiStartX		= 0;       
		m_uiStartY		= 0;		
		m_uiWidth		= 0;        
		m_uiHeight		= 0;		
		m_fBusSpeed		= 100.0;       
		m_pixelFormat	= PIXEL_FORMAT_MONO8;   
	}
}PTFormat7Info, *pPTFormat7Info ;

// Format7��ѯ��Ϣ
typedef struct tagPTFormat7QueryInfo
{
	Mode         m_mode;			// ģʽ
	unsigned int m_uiUnitStartX;    // ��ʼXUnit
	unsigned int m_uiUnitStartY;	// ��ʼYUnit
	unsigned int m_uiUnitWidth;     // ����Unit
	unsigned int m_uiUnitHeight;	// �߶�Unit
	unsigned int m_uiMaxWidth;		// ������
	unsigned int m_uiMaxHeight;		// ���߶�
	unsigned int m_uiPixelFormatSupported; // ֧�ֵ�PixelFormat
	
	tagPTFormat7QueryInfo()
	{
		m_mode						= MODE_0;		
		m_uiUnitStartX				= 0;       
		m_uiUnitStartY				= 0;		
		m_uiUnitWidth				= 0;        
		m_uiUnitHeight				= 0;		
		m_uiMaxWidth				= 0;
		m_uiMaxHeight				= 0;
		m_uiPixelFormatSupported	= 0;
	}
}PTFormat7QueryInfo, *pPTFormat7QueryInfo ;

// Fomrat��Ϣ����������
typedef enum tagPTFormat7InfoParaFilter
{
	eFmt7ParaAll = 0,
		eFmt7Mode,
		eFmt7StartX,
		eFmt7StartY,
		eFmt7Width,
		eFmt7Height,
		eFmt7BusSpeed,
		eFmt7PixelFormat,
}PTFormat7InfoParaFilter;

// �ɼ�ͼ��ģʽ
typedef enum tagGrabImageMode
{
	eWaitEvent = 0,                 // �ڲ��¼�����
	eGrabThread					    // �̷߳�ʽ	
}PTGrabImageMode;

// �������ģʽ
typedef enum tagPTTriggerMode
{
	ePTNotTrigger		= -1,		// �Ǵ���ģʽ
	ePTSoftwareTrigger	= 0,		// ��������ģʽ 
	ePTHardwareTrigger	= 1			// Ӳ������ģʽ	
} PTTriggerMode;	

// ���ظ�ʽ
typedef enum tagPTPixelFormat
{
	ePTUnknown	= -1,				// δ֪
	ePTGrey8	= 0,				// 8λ�Ҷ�ͼ��
	ePTRGB24	= 1,				// 24λ��ɫͼ��
	ePTRGBA32	= 2					// 32λ��ɫͼ��
} PTPixelFormat;

// ����ɼ��Ƿ�ɹ���״̬��־
typedef enum tagPTStatus
{
	ePTGrabOK		= 0,			// �ɼ��ɹ�
	ePTGrabFailed	= 1,			// �ɼ�ʧ��
	ePTGrabTimeout	= 2,			// �ɼ���ʱ
	ePTOffline		= 3				// �������
} PTStatus;

// ����ɼ��̹߳���ģʽ
typedef enum tagPTGrabThreadStatus
{
	ePTIdle = -1,					// ����
	ePTSnap = 0,					// ��֡
	ePTGrab							// ����
} PTGrabThreadStatus;
	
// GPIOģʽ
typedef enum tagPTGPIOMode
{
	ePTGMUnknown = -1,			// δ֪ģʽ
		ePTGMInput = 0,				// ����IOģʽ
		ePTGMOutput,				// ���IOģʽ
		ePTGMTrigger,				// PGR����ģʽ
		ePTGMStrobe,				// PGR Strobeģʽ
		ePTGMPWM,					// PGR PWMģʽ
		ePTGMDCAM=8					// DCAM���ģʽ(֧�����IO��������Strobeģʽ)
}PTGMMode;

// �������/���߻ص�����		
typedef void (WINAPI *CameraStatusCallback)(void* pParameter, unsigned int serialNumber);	


// ���߹�����Ϣ��
typedef struct tagPTBusMangerInfo
{
public:
	tagPTBusMangerInfo();
	~tagPTBusMangerInfo();
	void SetCameraArrivalCallback(void* pPara, CameraStatusCallback pCallback);				// ����������߻ص�
	void SetCameraRemovalCallback(void* pPara, CameraStatusCallback pCallback);				// ����������߻ص�

public:
	BusManager						   *m_pBusManger;					
	std::map<unsigned int, bool>		m_BusCamaraOnlineInfo;								// �������������Ϣ�б�
	CRITICAL_SECTION					m_csBusCallback;									// �ص��ٽ���
	CallbackHandle						m_pArrivalHandle;									// ����������߻ص����
	CallbackHandle						m_pRemovalHandle;									// ���������ص����
	CameraStatusCallback				m_pUserCameraArrivalCallback;						// ������߻ص�����
	CameraStatusCallback				m_pUserCameraRemovalCallback;						// ������߻ص�����
	void                               *m_pUserCameraArrivalCallbackPara;					// ������߻ص���������
	void                               *m_pUserCameraRemovalCallbackPara;					// ������߻ص���������	
}PTBusMangerInfo, *pPTBusMangerInfo;


// �ص���������������������ɼ���ɺ���øûص�������
// ����˵����
//		pOwner:		�����ö����ָ�룬��SetOwner���������pOwnerһ�£�
//		status:		�ɼ�״̬ö�٣��ɹ���������ʱ
//		pImage:		ͼ������ָ��
//		lWidth:		ͼ��Ŀ�
//		lHeight:	ͼ��ĸ�
//		pixelFormat:ͼ������ظ�ʽ
// ע��
//		����ɼ�ͼ��ɹ�����pImage��lWidth��lHeight��pixelFormatΪͼ����ص���Ч���ݣ�
//		����ɼ�ͼ���������pImage��lWidth��lHeight��pixelFormatΪ��Чֵ��һ��Ϊ0��
//		����ɼ�ͼ��ʱ����pImage��lWidth��lHeight��pixelFormatΪ��Чֵ��һ��Ϊ0��

typedef void (WINAPI *PTGrabbedCallback)(void* pOwner, PTStatus status, unsigned char* pImage 					
										 ,long lWidth, long lHeight, PTPixelFormat pixelFormat);


//
// FlyCapture2.0�ɼ���
//
class /*SV_PTGRAB2_API*/ scPTGrab2  
{
public:
	// ���캯���������òɼ�ģʽ�����grabImageModeΪeWaitEvent���ڲ����ô����̣߳�
	// ���grabImageModeΪeGrabThread���ڲ��ᴴ���߳����ɼ�ͼ��
	scPTGrab2(PTGrabImageMode grabImageMode = eWaitEvent);								
	virtual ~scPTGrab2();

public:	
	////////////////////////////////////���ûص�����////////////////////////////////
	void SetOwner(void* pOwner, PTGrabbedCallback funcCallback);							// ���õ����ߺͻص�����
	void SetSendCallbackEnable(bool bEnable);												// �����Ƿ���ûص�����
	bool GetSendCallbackEnable();															// ��ȡ�Ƿ���ûص�����
	
	////////////////////////////////////�����ʼ��/////////////////////////////////
	bool InitFromSerial(unsigned int uiSerialNum);											// ����������кų�ʼ�����
	bool InitFromIndex(unsigned int nCamIndex);												// ���������ų�ʼ�����
	bool InitFromGuid(PGRGuid *pGuid);														// �������Guid��ʼ�����
	bool IsOnline();																		// �Ƿ��������
	bool IsInit();																			// ��ʼ���Ƿ�ɹ���������ʼ���Ͽ������
	bool Destroy();																			// �Ͽ��������
	bool Restart();																			// ����������ʺ��������ʼ���ɹ����Ͽ������ӵ����

	////////////////////////////////////�ɼ�����///////////////////////////////////
	bool Grab();						
	bool Freeze();																			// ���������ɼ�����Ҫ�Ѿ������ɼ��߳�
	bool IsGrabbing();																		// ��ȡ�Ƿ��������ɼ�״̬
	bool SetGrabTimeout(unsigned long ulTimeout);											// ���òɼ���ʱ
	bool SetMonitorTimeout(unsigned long ulTimeout);										// ���ü�زɼ���ʱ���̲߳ɼ���ʽ
	// �ɼ���֡ͼ��ֻ֧��FreeRunģʽ����ע����Ҫ�Ѿ������ɼ��̣߳�
	// ���bWaitSnapped=true, �ȴ�ֱ���ɼ���ɲŷ��أ�
	// ���bWaitSnapped=false, ���ú󲻵ȴ��ɼ���ֱ�ӷ��أ�
	bool Snap(bool bWaitSnapped = false);
	bool IsAcquireSuccess();																// ͼ���Ƿ�ɼ��ɹ�
	bool Start();																			// ���������ʼ�ɼ�ͼ�񣬲���Ҫ�����ɼ��߳�
	bool Stop();																			// �������ֹͣ�ɼ�ͼ�񣬲���Ҫ�����ɼ��߳�
	//////////////////////////////////////////////////////////////////////////
	// �ɼ���֡ͼ��
	// �òɼ��������������������ɼ�����ͬʱ���ã�
	// �òɼ�����ֱ���ɼ���ɺ��˳���
	// �ú����������ڲɼ��̣߳�
	bool SnapImage();																		// ֱ�Ӳɼ�һ֡ͼ��

	/////////////////////////////////��������//////////////////////////////////////
	// ��������Ĵ���ģʽ(��������Ӳ���������ǷǴ���)��ע�����������Ϊ��������Ӳ����ʱ����Ҫ�Ѿ������ɼ��߳�
	bool SetTriggerMode(PTTriggerMode mode, unsigned long ulTimeout = 2000);                   
	PTTriggerMode  GetTriggerMode();														// ��ȡ��ǰ�Ĵ���ģʽ	
	bool CloseTriggerMode();																// �رմ���ģʽ
	bool IsTrigger();																		// �Ƿ��Դ�����ʽ����������Ӳ�������ɼ�ͼ��
	// �����������źŲɼ���֡ͼ�����������ɼ���ȡ��֡ͼ��
	// �������������źź󣬸ú�����������
	// �ɼ�һ֡ͼ��ɹ��󣬵��ûص�����
	bool SendSoftTrigger();																	// �����������ź�
	// �����������źŲɼ���֡ͼ�����������ɼ���ȡ��֡ͼ��
	// �������������źź󣬸ú�����������
	// �ɼ�һ֡ͼ��ɹ��󣬵��ûص�����
	bool TriggerSnap();																		// �����ɼ�һ֡ͼ��
	bool CheckTriggerReady();																// ��ѯ���ȴ���ֱ��������Խ��д���
	bool CheckSoftwareTriggerPresence(); 													// ��ѯ����Ƿ����������ɼ�

	///////////////////////////////���ͼ���������////////////////////////////////
	PTPixelFormat GetImagePixelFormat();													// ��ȡͼ������ظ�ʽ
	long GetImageWidth();																	// ��ȡͼ��Ŀ�
	long GetImageHeight();																	// ��ȡͼ��ĸ�
	long GetImageDataSize();																// ��ȡ�洢ͼ��������ڴ��С���ֽڣ�	
	bool GetImageData(unsigned char* pImage);												// ��ȡͼ�����ݣ�Ҫ���ⲿ�ѷ�����ڴ棬 �ڴ�Ĵ�С����GetImageDataSize()��ã�
//	bool GetImage(scImageGray& image, long alignModulus = 1);								// ��ȡ�Ҷ�ͼ��������ظ�ʽΪePTRGB24�򷵻�false
//	bool GetImage(scImageRGB& image, long alignModulus = 1);								// ��ȡRGBͼ��������ظ�ʽΪePTGrey8�򷵻�false
	
	/////////////////////////////////1394�����ͨ��Ƶģʽ����//////////////////////
	bool IsVideoModeSupported(VideoMode vmVideoMode);										// ��ѯ1394����Ƿ�֧�ָ�������Ƶģʽ
	bool IsFrameRateSupported(FrameRate frFrameRate);										// ��ѯ1394����ڵ�ǰ��Ƶģʽ�£��Ƿ�֧�ָ�����֡��
	bool SetCurVideoMode(VideoMode vmVideoMode);											// ����1394�����Ƶģʽ��ֻ֧��Y8��RGBģʽ��������Ҫ�ı䵱ǰ֡��
	bool GetCurVideoMode(VideoMode* pvmVideoMode);											// ��ȡ1394�����Ƶģʽ
	bool SetCurFrameRate(FrameRate frFrameRate);											// ����1394���֡�ʣ�������Ҫ�ı������С�ع�ʱ��
	bool GetCurFrameRate(FrameRate* pfrFrameRate);											// ��ȡ1394�����ǰ��֡��
	bool GetMaxFrameRate(FrameRate* pfrFrameRate);											// ��ȡ1394����ڵ�ǰ��Ƶģʽ�£�֧�ֵ����֡��
	
	///////////////////////////////Format7��Ƶģʽ����/////////////////////////////
	bool QueryFormat7Info(PTFormat7QueryInfo &format7QueryInfo);							// ��ѯFormat7ģʽ��Ϣ����Ҫ��֤��ǰΪFormat7��ʽ��
	bool GetFormat7Config(PTFormat7Info &format7Info);										// ��ȡFormat7ģʽ�µĲ�������Ҫ��֤��ǰΪFormat7��ʽ��					
	bool SetFormat7Config(PTFormat7Info format7Info);										// ����Format7ģʽ�µĲ���
	bool IsFormat7InfoSupported(PTFormat7Info &format7Info, PTFormat7InfoParaFilter filter=eFmt7ParaAll); // �ж�Formatģʽ��Ϣ�Ƿ��������Ҫ��֤��ǰΪFormat7��ʽ��	

	/////////////////////////////////����������//////////////////////////////////
	// ֡������
	bool SetCurFrameRate(float fFrameRate);													// ����֡�ʣ�������Ҫ�ı������С�ع�ʱ��
	bool GetCurFrameRate(float *pfFrameRate);												// ��ȡ��ǰ֡��
	bool GetFrameRateRange(float *fMinFrameRate, float *fMaxFrameRate);						// ��ȡ���֡�ʷ�Χ
	// �ع�ʱ������
	bool SetCurShutter(float fShutter);														// �����ع�ʱ��
	bool GetCurShutter(float* pfShutter);													// ��ȡ��ǰ���ع�ʱ��
	bool GetShutterRange(float *pfMinShutter, float *pfMaxShutter);							// ��ȡ�ع�ʱ�䷶Χ
	// ��������
	bool SetCurGain(float fGain);															// ��������
	bool GetCurGain(float* pfGain);															// ��ȡ��ǰ����
	bool GetGainRange(float *pfMinGain, float *pfMaxGain);									// ��ȡ���淶Χ
	// Sharpness����
	bool SetCurSharpness(unsigned int uiSharpness);											// ����Sharpness
	bool GetCurSharpness(unsigned int* piSharpness);										// ��ȡ��ǰSharpness
	bool GetSharpnessRange(unsigned int *piMinSharpness, unsigned int *piMaxSharpness);		// ��ȡSharpness��Χ
	// Gamma����
	bool SetCurGamma(float fGamma);															// ����Gamma
	bool GetCurGamma(float* pfGamma);														// ��ȡ��ǰGamma
	bool GetGammaRange(float *pfMinGamma, float *pfMaxGamma);								// ��ȡGamma��Χ
	// Brightness����
	bool SetCurBrightness(float fBrightness);												// ����Brightness
	bool GetCurBrightness(float* pfBrightness);												// ��ȡ��ǰBrightness
	bool GetBrightnessRange(float *pfMinBrightness, float *pfMaxBrightness);				// ��ȡBrightness��Χ
	
	///////////////////////////GigE��������///////////////////////////////
	// GigEIP���÷�ʽ����
	bool SetGigEIPConfigurationType(BOOL bPersistent=TRUE, BOOL bDHCP=FALSE);				// ����GigEIP���÷�ʽ
	bool GetGigEIPConfigurationType(BOOL &bPersistent, BOOL &bDHCP);						// ��ȡGigEIP���÷�ʽ
	// Persitant IP����
	bool SetGigEPersistentIPAddress(IPAddress ipAddress, IPAddress subnetMask, 
		IPAddress defaultGateway);															// ����GigE���Persistent IP��ַ
	bool GetGigEPersistentIPAddress(IPAddress &ipAddress, IPAddress &subnetMask, 
		IPAddress &defaultGateway);															// ��ȡGigE���Persistent IP��ַ
	// PacketSize����
	bool SetCurGigEPacketSize(unsigned int uiPacketSize);									// ����PacketSize
	bool GetCurGigEPacketSize(unsigned int* piPacketSize);									// ��ȡ��ǰPacketSize
	bool GetGigEPacketSizeRange(unsigned int *piMinPacketSize, unsigned int *piMaxPacketSize);// ��ȡPacketSize��Χ
	// PacketDelay����
	bool SetCurGigEPacketDelay(unsigned int uiPacketDelay);									// ����PacketDelay
	bool GetCurGigEPacketDelay(unsigned int* piPacketDelay);								// ��ȡ��ǰPacketDelay
	bool GetGigEPacketDelayRange(unsigned int *piMinPacketDelay, unsigned int *piMaxPacketDelay);// ��ȡPacketDelay��Χ
	// HeartBeatʹ������
	bool SetCurGigEHeartBeatEnable(unsigned int uiHeartBeatEnable);							// ����HeartBeatEnable
	bool GetCurGigEHeartBeatEnable(unsigned int* piHeartBeatEnable);						// ��ȡ��ǰHeartBeatEnable
	// HeartBeatTimeOut����
	bool SetCurGigEHeartBeatTimeOut(unsigned int uiHeartBeatTimeOut);						// ����HeartBeatTimeOut
	bool GetCurGigEHeartBeatTimeOut(unsigned int* piHeartBeatTimeOut);						// ��ȡ��ǰHeartBeatTimeOut
	bool GetGigEHeartBeatTimeOutRange(unsigned int *piMinHeartBeatTimeOut, 
										unsigned int *piMaxHeartBeatTimeOut);				// ��ȡHeartBeatTimeOut��Χ

public:	
	////////////////////////////////////��ȡ�����Ϣ///////////////////////////////
	bool GetCameraInfo(CameraInfo* pCamInfo);												// ��ȡ����������Ϣ
	bool GetCameraSerialNumber(unsigned int* pSerialNumber);								// ��ȡ��������к�
	bool GetCameraIndex(unsigned int* pCamIndex);											// ��ȡ�������ֵ

	///////////////////////////////////�������////////////////////////////////////
	bool SaveToCamera(long lChannel = 1);													// ���浽�����
	bool SetBusSpeed(BusSpeed  asyncBusSpeed, BusSpeed  isochBusSpeed);						// �����첽��ͬ�������ٶ�
	bool GetBusSpeed(BusSpeed* pAsyncBusSpeed, BusSpeed* pIsochBusSpeed);					// ��ȡ�첽��ͬ�������ٶ�
	bool WriteRegisiter(unsigned int uiAdd, unsigned int uiValue);							// д�Ĵ���
	bool ReadRegisiter(unsigned int uiAdd, unsigned int *uiValue);							// ���Ĵ���			
	PTCameraType  GetCameraType();															// ����������,1394��USB��GigE
	// ����brightness��exposure��shutter��gainΪ�ֶ�״̬��һ��Ϊ�ڲ�����
	bool SetStatusManual();                        

	//////////////////////////////////////��������//////////////////////////////////
	static bool RescanBus();																// ����ɨ�����ߣ���ǰ������ӻᱻ�ж�
	static bool SetUserBusCameraArrivalCallback(void* pParameter, CameraStatusCallback pCallback);	// ��������������߻ص�
	static bool SetUserBusCameraRemovalCallback(void* pParameter, CameraStatusCallback pCallback);	// ��������������߻ص�
	static bool GetBusCameraCount(unsigned int* pCamCount);									// ��ȡ���������������
	static bool EnumerateBusCameras(PGRGuid* pGuid, unsigned int* pSize);					// ö�������ϵĿ����������Guid
	static bool EnumerateBusCameras(unsigned int* pSerialInfo, unsigned int* pSize);		// ö�������ϵĿ�������������к�
	static bool EnumerateBusCameras(PTCameraInfo* pCameraInfo, unsigned int* pSize);		// ö�������ϵĿ������������Ϣ
	static bool GetCameraGuidFromIndex(int iIndex, PGRGuid *pGuid);							// ��������Guid(ͨ�����)
	static bool GetCameraGuidFromSerialNum(unsigned int pSerialNum, PGRGuid *pGuid);	    // ��������Guid(ͨ�����к�)
	static PTCameraType GetCameraTypeFromGuid(PGRGuid *pGuid);								// �ж��������(ͨ��Guid)
	static PTCameraType GetCameraTypeFromIndex(unsigned int uiIndex);						// �ж��������(ͨ�����)
	static PTCameraType GetCameraTypeFromSerialNum(unsigned int uiSerialNum);				// �ж��������(ͨ�����к�)
	static bool GetGigECameraIPAddressByGuid(PGRGuid *pGuid, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// ���GigE���IP��ַ(ͨ��Guid)
	static bool GetGigECameraIPAddressByIndex(unsigned int uiIndex, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// ���GigE���IP��ַ(ͨ�����)
	static bool GetGigECameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// ���GigE���IP��ַ(ͨ�����к�)
	static bool SetGigECameraIPAddressByGuid(PGRGuid *pGuid, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// ����GigE���IP��ַ(ͨ��Guid)
	static bool SetGigECameraIPAddressByIndex(unsigned int uiIndex, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// ����GigE���IP��ַ(ͨ�����)
	static bool SetGigECameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// ǿ����������GigE���IP��ַ
	static bool AutoForceAllGigECamerasIP();												// ��������������GigECamera���IP
	static BusManager* GetBusManager();														// ��ȡBusManager����
	static bool DeleteBusManager();															// ��ʽɾ��BusManager������DLL��ʹ�á�
	static void InternalBusCameraArrivalCallback(void* pParameter, unsigned int serialNumber);// ������߻ص�
	static void InternalBusCameraRemovalCallback(void* pParameter, unsigned int serialNumber);// ������߻ص�

	///////////////////////GPIOģʽ����/////////////////////////////////////
	bool SetGPIOMode(int iPin, PTGMMode mode);												// ����GPIOģʽ,iPin:0,1,2,3
	PTGMMode GetGPIOMode(int iPin);  														// ��ȡGPIOģʽ,iPin:0,1,2,3
	bool GetGPIOInput(int iPin, int &iValue);												// inputģʽ�£���ȡ����ֵ	
	bool SetGPIOOutput(int iPin, int iValue);												// outputģʽ�£��������ֵ(֧��ePTGMOutput��ePTGMDCAM)
	bool SetGPIOTriggerPolarity(int iPin, bool bPolarity); 									// Triggerģʽ�£����ô������ԣ�bPolarity:0,�½���;1:������(֧��ePTGMTrigger��ePTGMInput)	
	bool GetGPIOTriggerPolarity(int iPin, bool &bPolarity);									// Triggerģʽ�£���ȡ�������ԣ�bPolarity:0,�½���;1:������
	bool SetGPIOStrobeParam(int iPin, bool bPolarity, float fDelay, float fDuration); 		// Strobeģʽ�£�bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms(֧��ePTGMStrobe��ePTGMDCAM)
	bool GetGPIOStrobeParam(int iPin, bool &bPolarity, float &fDelay, float &fDuration); 	// Strobeģʽ�£���ȡ����bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms
	bool SetGPIOPWMParam(int iPin, bool bPolarity, int iPWMCnt, float fLowDuration, float fHighDuration, 	
		bool bEnableEnPin=false, int iEnablePinNum=0, bool bEnablePinPolarity=false);		// PWMģʽ�£����ü��ԣ��������������ʱ��ms(�ͺ͸�),bEnableEnPin:�Ƿ����EnPin;iEnablePinNum:EnPin���
	bool GetGPIOPWMParam(int iPin, bool &bPolarity, int &iPWMCnt, float &fLowDuration, float &fHighDuration, 
		bool &bEnableEnPin, int &iEnablePinNum, bool &bEnablePinPolarity);	// PWMģʽ�£���ȡ���������ԣ��������������ʱ��ms(�ͺ͸�),bEnableEnPin:�Ƿ����EnPin;iEnablePinNum:EnPin���(֧��ePTGMPWM)

private:
	///////////////////////////////�ڲ����ú���(�ɼ����)//////////////////////////
	bool TransferData();
public:
	bool CreateGrabThread();																// �����ɼ��̣߳�һ���ɼ�����ֻ����һ���ɼ��߳�	
private:
	bool TerminateGrabThread();																// �����ɼ��߳�
	PTStatus AcquireImage();																// ������ɼ�һ��ͼ�񣬲�������Ӧ��ת��
	void SendCallback(PTStatus	status);													// �ɼ���ɵ��ûص�����
    static void PTImageEventCallback(Image *pImage, const void *pCallbackData);				// �ڲ��ص�����  
	static unsigned int GrabThread(LPVOID lpParam);											// �ɼ��߳�
	static unsigned int MonitorAcquireImageThread(LPVOID lpParam);							// 	��زɼ��ص�����

	///////////////////////////////�ڲ����ú���(1394��Ƶģʽ���)//////////////////
	// ��ָ����ƵģʽתΪ������Ĵ�������Ӧ������ֵ
	bool ConvertVideoModeToULongs(VideoMode vmVideoMode, unsigned long* pulFormat 
		,unsigned long* pulMode);														
	// ��ָ��֡��ģʽתΪ������Ĵ�������Ӧ������ֵ   
	bool ConvertFrameRateToULong(FrameRate frFrameRate
								  ,unsigned long* pulFrameRate);	
	bool ResetCurFrameRateByVideoMode();													// ��������ĵ�ǰ��Ƶģʽ������ǰ֡��
	bool ResetCurShutterByFrameRate();														// ��������ĵ�ǰ֡�ʵ����ع�ʱ��



private:
//	CameraBase			*m_pCamera;															// ����ɼ�����
	
	PTCameraType         m_cameraType;														// ������࣬1394��GigE

	PTGrabImageMode      m_grabImageMode;													// �ɼ�ͼ��ʽ
public:
	//GigECamera			*m_pCamera;
	Camera			*m_pCamera;
	bool				 m_bOnline;															// ����Ƿ�����
	bool	             m_bIsGrabbing;														// �Ƿ����������ɼ�ͼ��
private:
	bool                 m_bIsSnap;															// �Ƿ����ڵ�֡�ɼ�ͼ��
	PTGrabThreadStatus	 m_threadStatus;													// �ɼ��߳������Ĺ���ģʽ
	HANDLE				 m_hEventSnapped;	
	bool                 m_bAcquireSuceess;                                     
	bool				 m_bFreezeStop;														// �Ƿ�ΪFreezeStop
	Image                m_ImageRaw;														// ����ɼ�ԭͼ��
	unsigned char*       m_pGrabImage;														// �ɼ�����ͼ������
public:
	long	             m_lWidth;															// ͼ��Ŀ�
	long	             m_lHeight;															// ͼ��ĸ�
	PTTriggerMode		 m_nTriggerMode;													// ����ģʽ
	PTPixelFormat		 m_pixelFormat;														// ͼ������ظ�ʽ	
private:
				
	void*	             m_pMainFrm;														// �ص�����������ָ��
	PTGrabbedCallback	 m_funcCallback;													// �ص�����	
	bool				 m_bSendCallbackEnable;												// �Ƿ������ص�	

	CRITICAL_SECTION	 m_criticalSection;													// �ٽ���������ͼ�����ݵĶ�д	
	unsigned int		*m_pSerialNum;														// ������к�
	///////////////////////////�̷߳�ʽ�ɼ�////////////////////////////////////////
	CWinThread*	         m_pWinThread;														// �����ɼ��߳�
	HANDLE				 m_hEventToRun;														// �ɼ��̴߳�����ͣʱ��������Ϣ��ʼ�ɼ��¼����
	HANDLE				 m_hEventExit;			  											// ���߳�֪ͨ���߳��ѽ����¼����
	bool				 m_bTerminate;														// �˳�ʵʱ��ʾ��־
	bool				 m_bHaveStopped;													// �Ƿ��Ѿ��˳�ʵʱ�ɼ���ʾ
	///////////////////////////����߳����////////////////////////////////////////
	CWinThread			*m_pMonitorThread;													// ����߳�
	long                 m_lMonitorTimeout;													// ��س�ʱ
	bool				 m_bMonitorExit;													// �˳�����߳�	
	HANDLE               m_hEventMonitorBegin;												// ��ؿ�ʼ�¼�
	HANDLE				 m_hEventMonitorExit;												// ����˳��¼�
	bool				 m_bMonitorEnd;
public:
	static std::auto_ptr<PTBusMangerInfo>	m_AutoBusManager;								// ���߹�����
};

#pragma pack(pop)

#endif // !defined(AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_)
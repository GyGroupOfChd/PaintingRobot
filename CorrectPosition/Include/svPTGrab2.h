// scPTGrab.h: interface for the scPTGrab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_)
#define AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlyCapture2.h"
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

#define PTCLOCK_FREQ (1024000.0) // PT相机时钟频率，DragonFly除外

// 相机接口类型
typedef enum tagPTCameraType 
{
	eInterfaceUnknow = 0,		    // 未知接口
	eIEEE1394 = 1,                  // 1394接口
	eGigE,						    // GigE接口
	eUSB2,						    // USB2.0接口
	eUSB3						    // USB3.0接口
}PTCameraType;

// 相机信息
typedef struct tagPTCameraInfo
{
	unsigned int m_uiIndex;         // 序号
	unsigned int m_uiSerialNum;     // 序列号
	PTCameraType m_CameraType;      // 相机类型
	IPAddress	 m_ipAddress;	    // IP地址
	IPAddress    m_subnetMask;      // 子网掩码
	IPAddress    m_defaultGateway;  // 默认网关
	char		 m_cModelName[512];	// 相机型号
	char		 m_cResolution[512];// 相机分辨率
	tagPTCameraInfo()
	{
		m_uiIndex = -1;
		m_uiSerialNum = 0;
		memset(m_cModelName, 0, 512);
		memset(m_cResolution, 0, 512);
	}
}PTCameraInfo, *pPTCameraInfo ;

// Format7格式
typedef struct tagPTFormat7Info
{
	Mode         m_mode;			// 模式
	unsigned int m_uiStartX;        // 起始X
	unsigned int m_uiStartY;		// 起始Y
	unsigned int m_uiWidth;         // 宽度
	unsigned int m_uiHeight;		// 高度
	float        m_fBusSpeed;       // 数据率
	PixelFormat  m_pixelFormat;     // 像素格式
	
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

// Format7查询信息
typedef struct tagPTFormat7QueryInfo
{
	Mode         m_mode;			// 模式
	unsigned int m_uiUnitStartX;    // 起始XUnit
	unsigned int m_uiUnitStartY;	// 起始YUnit
	unsigned int m_uiUnitWidth;     // 宽度Unit
	unsigned int m_uiUnitHeight;	// 高度Unit
	unsigned int m_uiMaxWidth;		// 最大宽度
	unsigned int m_uiMaxHeight;		// 最大高度
	unsigned int m_uiPixelFormatSupported; // 支持的PixelFormat
	
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

// Fomrat信息参数过滤器
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

// 采集图像模式
typedef enum tagGrabImageMode
{
	eWaitEvent = 0,                 // 内部事件触发
	eGrabThread					    // 线程方式	
}PTGrabImageMode;

// 相机触发模式
typedef enum tagPTTriggerMode
{
	ePTNotTrigger		= -1,		// 非触发模式
	ePTSoftwareTrigger	= 0,		// 软件触发模式 
	ePTHardwareTrigger	= 1			// 硬件触发模式	
} PTTriggerMode;	

// 像素格式
typedef enum tagPTPixelFormat
{
	ePTUnknown	= -1,				// 未知
	ePTGrey8	= 0,				// 8位灰度图像
	ePTRGB24	= 1,				// 24位彩色图像
	ePTRGBA32	= 2					// 32位彩色图像
} PTPixelFormat;

// 相机采集是否成功的状态标志
typedef enum tagPTStatus
{
	ePTGrabOK		= 0,			// 采集成功
	ePTGrabFailed	= 1,			// 采集失败
	ePTGrabTimeout	= 2,			// 采集超时
	ePTOffline		= 3				// 相机掉线
} PTStatus;

// 相机采集线程工作模式
typedef enum tagPTGrabThreadStatus
{
	ePTIdle = -1,					// 空闲
	ePTSnap = 0,					// 单帧
	ePTGrab							// 连续
} PTGrabThreadStatus;
	
// GPIO模式
typedef enum tagPTGPIOMode
{
	ePTGMUnknown = -1,			// 未知模式
		ePTGMInput = 0,				// 输入IO模式
		ePTGMOutput,				// 输出IO模式
		ePTGMTrigger,				// PGR触发模式
		ePTGMStrobe,				// PGR Strobe模式
		ePTGMPWM,					// PGR PWM模式
		ePTGMDCAM=8					// DCAM输出模式(支持输出IO、触发、Strobe模式)
}PTGMMode;

// 相机在线/离线回调函数		
typedef void (WINAPI *CameraStatusCallback)(void* pParameter, unsigned int serialNumber);	


// 总线管理信息类
typedef struct tagPTBusMangerInfo
{
public:
	tagPTBusMangerInfo();
	~tagPTBusMangerInfo();
	void SetCameraArrivalCallback(void* pPara, CameraStatusCallback pCallback);				// 设置相机在线回调
	void SetCameraRemovalCallback(void* pPara, CameraStatusCallback pCallback);				// 设置相机离线回调

public:
	BusManager						   *m_pBusManger;					
	std::map<unsigned int, bool>		m_BusCamaraOnlineInfo;								// 总线相机在线信息列表
	CRITICAL_SECTION					m_csBusCallback;									// 回调临界区
	CallbackHandle						m_pArrivalHandle;									// 总线相机在线回调句柄
	CallbackHandle						m_pRemovalHandle;									// 总线相机离回调句柄
	CameraStatusCallback				m_pUserCameraArrivalCallback;						// 相机在线回调函数
	CameraStatusCallback				m_pUserCameraRemovalCallback;						// 相机离线回调函数
	void                               *m_pUserCameraArrivalCallbackPara;					// 相机在线回调函数参数
	void                               *m_pUserCameraRemovalCallbackPara;					// 相机离线回调函数参数	
}PTBusMangerInfo, *pPTBusMangerInfo;


// 回调函数类型声明，当相机采集完成后调用该回调函数；
// 参数说明：
//		pOwner:		主调用对象的指针，与SetOwner的输入参数pOwner一致；
//		status:		采集状态枚举：成功、出错或超时
//		pImage:		图像数据指针
//		lWidth:		图像的宽
//		lHeight:	图像的高
//		pixelFormat:图像的像素格式
// 注：
//		如果采集图像成功，则pImage、lWidth、lHeight和pixelFormat为图像相关的有效数据；
//		如果采集图像出错，则pImage、lWidth、lHeight和pixelFormat为无效值，一般为0；
//		如果采集图像超时，则pImage、lWidth、lHeight和pixelFormat为无效值，一般为0；

typedef void (WINAPI *PTGrabbedCallback)(void* pOwner, PTStatus status, unsigned char* pImage 					
										 ,long lWidth, long lHeight, PTPixelFormat pixelFormat);


//
// FlyCapture2.0采集类
//
class /*SV_PTGRAB2_API*/ scPTGrab2  
{
public:
	// 构造函数，需设置采集模式，如果grabImageMode为eWaitEvent，内部不用创建线程；
	// 如果grabImageMode为eGrabThread，内部会创建线程来采集图像；
	scPTGrab2(PTGrabImageMode grabImageMode = eWaitEvent);								
	virtual ~scPTGrab2();

public:	
	////////////////////////////////////设置回调函数////////////////////////////////
	void SetOwner(void* pOwner, PTGrabbedCallback funcCallback);							// 设置调用者和回调函数
	void SetSendCallbackEnable(bool bEnable);												// 设置是否调用回调函数
	bool GetSendCallbackEnable();															// 获取是否调用回调函数
	
	////////////////////////////////////相机初始化/////////////////////////////////
	bool InitFromSerial(unsigned int uiSerialNum);											// 根据相机序列号初始化相机
	bool InitFromIndex(unsigned int nCamIndex);												// 根据索引号初始化相机
	bool InitFromGuid(PGRGuid *pGuid);														// 根据相机Guid初始化相机
	bool IsOnline();																		// 是否连接相机
	bool IsInit();																			// 初始化是否成功，不检测初始化断开的情况
	bool Destroy();																			// 断开相机连接
	bool Restart();																			// 相机重启，适合于相机初始化成功，断开又连接的情况

	////////////////////////////////////采集操作///////////////////////////////////
	bool Grab();						
	bool Freeze();																			// 结束连续采集，需要已经创建采集线程
	bool IsGrabbing();																		// 获取是否处于连续采集状态
	bool SetGrabTimeout(unsigned long ulTimeout);											// 设置采集延时
	bool SetMonitorTimeout(unsigned long ulTimeout);										// 设置监控采集延时，线程采集方式
	// 采集单帧图像（只支持FreeRun模式）。注：需要已经创建采集线程；
	// 如果bWaitSnapped=true, 等待直到采集完成才返回；
	// 如果bWaitSnapped=false, 调用后不等待采集完直接返回；
	bool Snap(bool bWaitSnapped = false);
	bool IsAcquireSuccess();																// 图像是否采集成功
	bool Start();																			// 控制相机开始采集图像，不需要创建采集线程
	bool Stop();																			// 控制相机停止采集图像，不需要创建采集线程
	//////////////////////////////////////////////////////////////////////////
	// 采集单帧图像；
	// 该采集函数不能与上述其他采集函数同时调用；
	// 该采集函数直到采集完成后退出；
	// 该函数不依赖于采集线程；
	bool SnapImage();																		// 直接采集一帧图像

	/////////////////////////////////触发设置//////////////////////////////////////
	// 设置相机的触发模式(软触发、硬触发、或是非触发)。注：当设置相机为软触发和硬触发时，需要已经创建采集线程
	bool SetTriggerMode(PTTriggerMode mode, unsigned long ulTimeout = 2000);                   
	PTTriggerMode  GetTriggerMode();														// 获取当前的触发模式	
	bool CloseTriggerMode();																// 关闭触发模式
	bool IsTrigger();																		// 是否以触发方式（软触发或硬触发）采集图像
	// 发送软触发信号采集单帧图像，利用连续采集获取单帧图像
	// 发送完软触发信号后，该函数立即返回
	// 采集一帧图像成功后，调用回调函数
	bool SendSoftTrigger();																	// 发送软触发信号
	// 发送软触发信号采集单帧图像，利用连续采集获取单帧图像
	// 发送完软触发信号后，该函数立即返回
	// 采集一帧图像成功后，调用回调函数
	bool TriggerSnap();																		// 触发采集一帧图像
	bool CheckTriggerReady();																// 查询并等待，直到相机可以进行触发
	bool CheckSoftwareTriggerPresence(); 													// 查询相机是否有软触发采集

	///////////////////////////////相机图像属性相关////////////////////////////////
	PTPixelFormat GetImagePixelFormat();													// 获取图像的像素格式
	long GetImageWidth();																	// 获取图像的宽
	long GetImageHeight();																	// 获取图像的高
	long GetImageDataSize();																// 获取存储图像所需的内存大小（字节）	
	bool GetImageData(unsigned char* pImage);												// 获取图像数据，要求外部已分配好内存， 内存的大小可由GetImageDataSize()获得；
//	bool GetImage(scImageGray& image, long alignModulus = 1);								// 获取灰度图像，如果像素格式为ePTRGB24则返回false
//	bool GetImage(scImageRGB& image, long alignModulus = 1);								// 获取RGB图像，如果像素格式为ePTGrey8则返回false
	
	/////////////////////////////////1394相机普通视频模式设置//////////////////////
	bool IsVideoModeSupported(VideoMode vmVideoMode);										// 查询1394相机是否支持给定的视频模式
	bool IsFrameRateSupported(FrameRate frFrameRate);										// 查询1394相机在当前视频模式下，是否支持给定的帧率
	bool SetCurVideoMode(VideoMode vmVideoMode);											// 设置1394相机视频模式：只支持Y8和RGB模式；可能需要改变当前帧率
	bool GetCurVideoMode(VideoMode* pvmVideoMode);											// 获取1394相机视频模式
	bool SetCurFrameRate(FrameRate frFrameRate);											// 设置1394相机帧率，可能需要改变最大最小曝光时间
	bool GetCurFrameRate(FrameRate* pfrFrameRate);											// 获取1394相机当前的帧率
	bool GetMaxFrameRate(FrameRate* pfrFrameRate);											// 获取1394相机在当前视频模式下，支持的最大帧率
	
	///////////////////////////////Format7视频模式设置/////////////////////////////
	bool QueryFormat7Info(PTFormat7QueryInfo &format7QueryInfo);							// 查询Format7模式信息，需要保证当前为Format7格式。
	bool GetFormat7Config(PTFormat7Info &format7Info);										// 获取Format7模式下的参数，需要保证当前为Format7格式。					
	bool SetFormat7Config(PTFormat7Info format7Info);										// 设置Format7模式下的参数
	bool IsFormat7InfoSupported(PTFormat7Info &format7Info, PTFormat7InfoParaFilter filter=eFmt7ParaAll); // 判断Format模式信息是否合理，需要保证当前为Format7格式。	

	/////////////////////////////////相机属性相关//////////////////////////////////
	// 帧率设置
	bool SetCurFrameRate(float fFrameRate);													// 设置帧率，可能需要改变最大最小曝光时间
	bool GetCurFrameRate(float *pfFrameRate);												// 获取当前帧率
	bool GetFrameRateRange(float *fMinFrameRate, float *fMaxFrameRate);						// 获取相机帧率范围
	// 曝光时间设置
	bool SetCurShutter(float fShutter);														// 设置曝光时间
	bool GetCurShutter(float* pfShutter);													// 获取当前的曝光时间
	bool GetShutterRange(float *pfMinShutter, float *pfMaxShutter);							// 获取曝光时间范围
	// 增益设置
	bool SetCurGain(float fGain);															// 设置增益
	bool GetCurGain(float* pfGain);															// 获取当前增益
	bool GetGainRange(float *pfMinGain, float *pfMaxGain);									// 获取增益范围
	// Sharpness设置
	bool SetCurSharpness(unsigned int uiSharpness);											// 设置Sharpness
	bool GetCurSharpness(unsigned int* piSharpness);										// 获取当前Sharpness
	bool GetSharpnessRange(unsigned int *piMinSharpness, unsigned int *piMaxSharpness);		// 获取Sharpness范围
	// Gamma设置
	bool SetCurGamma(float fGamma);															// 设置Gamma
	bool GetCurGamma(float* pfGamma);														// 获取当前Gamma
	bool GetGammaRange(float *pfMinGamma, float *pfMaxGamma);								// 获取Gamma范围
	// Brightness设置
	bool SetCurBrightness(float fBrightness);												// 设置Brightness
	bool GetCurBrightness(float* pfBrightness);												// 获取当前Brightness
	bool GetBrightnessRange(float *pfMinBrightness, float *pfMaxBrightness);				// 获取Brightness范围
	
	///////////////////////////GigE属性设置///////////////////////////////
	// GigEIP配置方式设置
	bool SetGigEIPConfigurationType(BOOL bPersistent=TRUE, BOOL bDHCP=FALSE);				// 设置GigEIP配置方式
	bool GetGigEIPConfigurationType(BOOL &bPersistent, BOOL &bDHCP);						// 获取GigEIP配置方式
	// Persitant IP设置
	bool SetGigEPersistentIPAddress(IPAddress ipAddress, IPAddress subnetMask, 
		IPAddress defaultGateway);															// 设置GigE相机Persistent IP地址
	bool GetGigEPersistentIPAddress(IPAddress &ipAddress, IPAddress &subnetMask, 
		IPAddress &defaultGateway);															// 获取GigE相机Persistent IP地址
	// PacketSize设置
	bool SetCurGigEPacketSize(unsigned int uiPacketSize);									// 设置PacketSize
	bool GetCurGigEPacketSize(unsigned int* piPacketSize);									// 获取当前PacketSize
	bool GetGigEPacketSizeRange(unsigned int *piMinPacketSize, unsigned int *piMaxPacketSize);// 获取PacketSize范围
	// PacketDelay设置
	bool SetCurGigEPacketDelay(unsigned int uiPacketDelay);									// 设置PacketDelay
	bool GetCurGigEPacketDelay(unsigned int* piPacketDelay);								// 获取当前PacketDelay
	bool GetGigEPacketDelayRange(unsigned int *piMinPacketDelay, unsigned int *piMaxPacketDelay);// 获取PacketDelay范围
	// HeartBeat使能设置
	bool SetCurGigEHeartBeatEnable(unsigned int uiHeartBeatEnable);							// 设置HeartBeatEnable
	bool GetCurGigEHeartBeatEnable(unsigned int* piHeartBeatEnable);						// 获取当前HeartBeatEnable
	// HeartBeatTimeOut设置
	bool SetCurGigEHeartBeatTimeOut(unsigned int uiHeartBeatTimeOut);						// 设置HeartBeatTimeOut
	bool GetCurGigEHeartBeatTimeOut(unsigned int* piHeartBeatTimeOut);						// 获取当前HeartBeatTimeOut
	bool GetGigEHeartBeatTimeOutRange(unsigned int *piMinHeartBeatTimeOut, 
										unsigned int *piMaxHeartBeatTimeOut);				// 获取HeartBeatTimeOut范围

public:	
	////////////////////////////////////获取相机信息///////////////////////////////
	bool GetCameraInfo(CameraInfo* pCamInfo);												// 获取相机的相关信息
	bool GetCameraSerialNumber(unsigned int* pSerialNumber);								// 获取相机的序列号
	bool GetCameraIndex(unsigned int* pCamIndex);											// 获取相机索引值

	///////////////////////////////////相机参数////////////////////////////////////
	bool SaveToCamera(long lChannel = 1);													// 保存到相机中
	bool SetBusSpeed(BusSpeed  asyncBusSpeed, BusSpeed  isochBusSpeed);						// 设置异步和同步总线速度
	bool GetBusSpeed(BusSpeed* pAsyncBusSpeed, BusSpeed* pIsochBusSpeed);					// 获取异步和同步总线速度
	bool WriteRegisiter(unsigned int uiAdd, unsigned int uiValue);							// 写寄存器
	bool ReadRegisiter(unsigned int uiAdd, unsigned int *uiValue);							// 读寄存器			
	PTCameraType  GetCameraType();															// 获得相机种类,1394、USB或GigE
	// 设置brightness、exposure、shutter、gain为手动状态，一般为内部调用
	bool SetStatusManual();                        

	//////////////////////////////////////公共函数//////////////////////////////////
	static bool RescanBus();																// 重新扫描总线，当前相机连接会被中断
	static bool SetUserBusCameraArrivalCallback(void* pParameter, CameraStatusCallback pCallback);	// 设置总线相机在线回调
	static bool SetUserBusCameraRemovalCallback(void* pParameter, CameraStatusCallback pCallback);	// 设置总线相机掉线回调
	static bool GetBusCameraCount(unsigned int* pCamCount);									// 获取总线上相机的数量
	static bool EnumerateBusCameras(PGRGuid* pGuid, unsigned int* pSize);					// 枚举总线上的可用相机及其Guid
	static bool EnumerateBusCameras(unsigned int* pSerialInfo, unsigned int* pSize);		// 枚举总线上的可用相机及其序列号
	static bool EnumerateBusCameras(PTCameraInfo* pCameraInfo, unsigned int* pSize);		// 枚举总线上的可用相机及其信息
	static bool GetCameraGuidFromIndex(int iIndex, PGRGuid *pGuid);							// 获得相机的Guid(通过序号)
	static bool GetCameraGuidFromSerialNum(unsigned int pSerialNum, PGRGuid *pGuid);	    // 获得相机的Guid(通过序列号)
	static PTCameraType GetCameraTypeFromGuid(PGRGuid *pGuid);								// 判断相机类型(通过Guid)
	static PTCameraType GetCameraTypeFromIndex(unsigned int uiIndex);						// 判断相机类型(通过序号)
	static PTCameraType GetCameraTypeFromSerialNum(unsigned int uiSerialNum);				// 判断相机类型(通过序列号)
	static bool GetGigECameraIPAddressByGuid(PGRGuid *pGuid, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// 获得GigE相机IP地址(通过Guid)
	static bool GetGigECameraIPAddressByIndex(unsigned int uiIndex, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// 获得GigE相机IP地址(通过序号)
	static bool GetGigECameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress &ipAddress, 
		IPAddress &subnetMask, IPAddress &defaultGateway);									// 获得GigE相机IP地址(通过序列号)
	static bool SetGigECameraIPAddressByGuid(PGRGuid *pGuid, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// 设置GigE相机IP地址(通过Guid)
	static bool SetGigECameraIPAddressByIndex(unsigned int uiIndex, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// 设置GigE相机IP地址(通过序号)
	static bool SetGigECameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress ipAddress, 
		IPAddress subnetMask, IPAddress defaultGateway);									// 强制设置所有GigE相机IP地址
	static bool AutoForceAllGigECamerasIP();												// 重新设置总线上GigECamera相机IP
	static BusManager* GetBusManager();														// 获取BusManager对象。
	static bool DeleteBusManager();															// 显式删除BusManager对象，在DLL中使用。
	static void InternalBusCameraArrivalCallback(void* pParameter, unsigned int serialNumber);// 相机在线回调
	static void InternalBusCameraRemovalCallback(void* pParameter, unsigned int serialNumber);// 相机掉线回调

	///////////////////////GPIO模式设置/////////////////////////////////////
	bool SetGPIOMode(int iPin, PTGMMode mode);												// 设置GPIO模式,iPin:0,1,2,3
	PTGMMode GetGPIOMode(int iPin);  														// 获取GPIO模式,iPin:0,1,2,3
	bool GetGPIOInput(int iPin, int &iValue);												// input模式下，获取输入值	
	bool SetGPIOOutput(int iPin, int iValue);												// output模式下，设置输出值(支持ePTGMOutput和ePTGMDCAM)
	bool SetGPIOTriggerPolarity(int iPin, bool bPolarity); 									// Trigger模式下，设置触发极性，bPolarity:0,下降沿;1:上升沿(支持ePTGMTrigger和ePTGMInput)	
	bool GetGPIOTriggerPolarity(int iPin, bool &bPolarity);									// Trigger模式下，获取触发极性，bPolarity:0,下降沿;1:上升沿
	bool SetGPIOStrobeParam(int iPin, bool bPolarity, float fDelay, float fDuration); 		// Strobe模式下，bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms(支持ePTGMStrobe和ePTGMDCAM)
	bool GetGPIOStrobeParam(int iPin, bool &bPolarity, float &fDelay, float &fDuration); 	// Strobe模式下，获取参数bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms
	bool SetGPIOPWMParam(int iPin, bool bPolarity, int iPWMCnt, float fLowDuration, float fHighDuration, 	
		bool bEnableEnPin=false, int iEnablePinNum=0, bool bEnablePinPolarity=false);		// PWM模式下，设置极性，脉冲个数，持续时间ms(低和高),bEnableEnPin:是否采用EnPin;iEnablePinNum:EnPin序号
	bool GetGPIOPWMParam(int iPin, bool &bPolarity, int &iPWMCnt, float &fLowDuration, float &fHighDuration, 
		bool &bEnableEnPin, int &iEnablePinNum, bool &bEnablePinPolarity);	// PWM模式下，获取参数，极性，脉冲个数，持续时间ms(低和高),bEnableEnPin:是否采用EnPin;iEnablePinNum:EnPin序号(支持ePTGMPWM)

private:
	///////////////////////////////内部调用函数(采集相关)//////////////////////////
	bool TransferData();
	bool CreateGrabThread();																// 创建采集线程，一个采集对象只能有一个采集线程	
	bool TerminateGrabThread();																// 结束采集线程
	PTStatus AcquireImage();																// 从相机采集一幅图像，并进行相应的转换
	void SendCallback(PTStatus	status);													// 采集完成调用回调函数
    static void PTImageEventCallback(Image *pImage, const void *pCallbackData);				// 内部回调函数  
	static unsigned int GrabThread(LPVOID lpParam);											// 采集线程
	static unsigned int MonitorAcquireImageThread(LPVOID lpParam);							// 	监控采集回调函数

	///////////////////////////////内部调用函数(1394视频模式相关)//////////////////
	// 将指定视频模式转为在相机寄存器中相应的索引值
	bool ConvertVideoModeToULongs(VideoMode vmVideoMode, unsigned long* pulFormat 
		,unsigned long* pulMode);														
	// 将指定帧率模式转为在相机寄存器中相应的索引值   
	bool ConvertFrameRateToULong(FrameRate frFrameRate
								  ,unsigned long* pulFrameRate);	
	bool ResetCurFrameRateByVideoMode();													// 根据相机的当前视频模式调整当前帧率
	bool ResetCurShutterByFrameRate();														// 根据相机的当前帧率调整曝光时间



private:
//	CameraBase			*m_pCamera;															// 相机采集基类
	GigECamera			*m_pCamera;
	PTCameraType         m_cameraType;														// 相机种类，1394或GigE
	bool				 m_bOnline;															// 相机是否连接
	PTGrabImageMode      m_grabImageMode;													// 采集图像方式
	bool	             m_bIsGrabbing;														// 是否正在连续采集图像
	bool                 m_bIsSnap;															// 是否正在单帧采集图像
	PTGrabThreadStatus	 m_threadStatus;													// 采集线程所处的工作模式
	HANDLE				 m_hEventSnapped;	
	bool                 m_bAcquireSuceess;                                     
	bool				 m_bFreezeStop;														// 是否为FreezeStop
	Image                m_ImageRaw;														// 相机采集原图像
	unsigned char*       m_pGrabImage;														// 采集到的图像数据
	long	             m_lWidth;															// 图像的宽
	long	             m_lHeight;															// 图像的高
	PTPixelFormat		 m_pixelFormat;														// 图像的像素格式				
	void*	             m_pMainFrm;														// 回调函数主窗口指针
	PTGrabbedCallback	 m_funcCallback;													// 回调函数	
	bool				 m_bSendCallbackEnable;												// 是否允许回调	
	PTTriggerMode		 m_nTriggerMode;													// 触发模式
	CRITICAL_SECTION	 m_criticalSection;													// 临界区，控制图像数据的读写	
	unsigned int		*m_pSerialNum;														// 相机序列号
	///////////////////////////线程方式采集////////////////////////////////////////
	CWinThread*	         m_pWinThread;														// 连续采集线程
	HANDLE				 m_hEventToRun;														// 采集线程处于暂停时，发出消息开始采集事件句柄
	HANDLE				 m_hEventExit;			  											// 子线程通知主线程已结束事件句柄
	bool				 m_bTerminate;														// 退出实时显示标志
	bool				 m_bHaveStopped;													// 是否已经退出实时采集显示
	///////////////////////////监控线程相关////////////////////////////////////////
	CWinThread			*m_pMonitorThread;													// 监控线程
	long                 m_lMonitorTimeout;													// 监控超时
	bool				 m_bMonitorExit;													// 退出监控线程	
	HANDLE               m_hEventMonitorBegin;												// 监控开始事件
	HANDLE				 m_hEventMonitorExit;												// 监控退出事件
	bool				 m_bMonitorEnd;
public:
	static std::auto_ptr<PTBusMangerInfo>	m_AutoBusManager;								// 总线管理类
};

#pragma pack(pop)

#endif // !defined(AFX_scPTGrab_H__A02A8D75_C6EE_4704_A17F_9B0A27B3447D__INCLUDED_)

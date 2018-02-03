// scPTGrab2.cpp: implementation of the scPTGrab2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "svPTGrab2.h"
//#include "svTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static VideoMode g_fvmFormatVideoMode[8][8] = 
{
	{
		VIDEOMODE_160x120YUV444,
			VIDEOMODE_320x240YUV422 ,
			VIDEOMODE_640x480YUV411,
			VIDEOMODE_640x480YUV422,
			VIDEOMODE_640x480RGB,
			VIDEOMODE_640x480Y8,
			VIDEOMODE_640x480Y16,
	},
	{
			VIDEOMODE_800x600YUV422 ,
				VIDEOMODE_800x600RGB,
				VIDEOMODE_800x600Y8,		
				VIDEOMODE_1024x768YUV422,
				VIDEOMODE_1024x768RGB,
				VIDEOMODE_1024x768Y8,
				VIDEOMODE_800x600Y16,
				VIDEOMODE_1024x768Y16
		},
		{
				VIDEOMODE_1280x960YUV422 ,
					VIDEOMODE_1280x960RGB,
					VIDEOMODE_1280x960Y8,		
					VIDEOMODE_1600x1200YUV422,
					VIDEOMODE_1600x1200RGB,
					VIDEOMODE_1600x1200Y8,
					VIDEOMODE_1280x960Y16,
					VIDEOMODE_1600x1200Y16
			},
			{
					VIDEOMODE_FORCE_32BITS
				},
				{
						VIDEOMODE_FORCE_32BITS,
					},
					{
							VIDEOMODE_FORCE_32BITS,
						},
						{
								VIDEOMODE_FORCE_32BITS,
							},
							{
									VIDEOMODE_FORMAT7,
								},
								
};

static FrameRate g_frFrameRate[] = 
{
		FRAMERATE_1_875,				// 1.875 fps. (Frames per second)
		FRAMERATE_3_75,					// 3.75 fps.
		FRAMERATE_7_5,					// 7.5 fps.
		FRAMERATE_15,					// 15 fps.
		FRAMERATE_30,					// 30 fps.
		FRAMERATE_60,					// 60 fps.
		FRAMERATE_120,					// 120 fps.
		FRAMERATE_240,					// 240 fps.
		FRAMERATE_FORMAT7               // Format7
};

static float g_fFrameRate[] = 
{
		1.875,
		3.75,
		7.5,
		15.0,
		30.0,
		60.0,
		120.0,
		240.0,
		-1
};

// 静态成员初始化
std::auto_ptr<PTBusMangerInfo> scPTGrab2::m_AutoBusManager = std::auto_ptr<PTBusMangerInfo>(NULL);

scPTGrab2::scPTGrab2(PTGrabImageMode grabImageMode)
{
	m_grabImageMode				= grabImageMode;  
	m_pCamera					= NULL;	
	m_bOnline					= false;
	m_pMainFrm					= NULL;
	m_funcCallback				= NULL;
	m_bSendCallbackEnable		= true;	
	m_pGrabImage				= NULL;
	m_lWidth					= 0;
	m_lHeight					= 0;
	m_bIsGrabbing				= false;
	m_bIsSnap					= false;
	m_threadStatus				= ePTIdle;
	m_bAcquireSuceess			= false;
	m_bFreezeStop				= false;
	m_hEventSnapped				= NULL;
	m_nTriggerMode				= ePTNotTrigger;

	// 线程采集图像相关
	m_pWinThread				= NULL;													
	m_hEventToRun				= NULL;											
	m_hEventExit				= NULL;			  										
	m_bTerminate				= true;												
	m_bHaveStopped				= true;	
	
	m_cameraType				= eInterfaceUnknow;
	m_pSerialNum				= NULL;

	// 监控线程相关
	m_pMonitorThread			= NULL;
	m_hEventMonitorBegin		= NULL;
	m_hEventMonitorExit			= NULL;
	m_lMonitorTimeout			= 4000;	
	m_bMonitorExit				= false;
	m_bMonitorEnd				= false;

	InitializeCriticalSection(&m_criticalSection); 	// 临界区
}

scPTGrab2::~scPTGrab2()
{
	Destroy();

	DeleteCriticalSection(&m_criticalSection); 
}

// 断开相机连接
bool scPTGrab2::Destroy()
{
	if (eWaitEvent == m_grabImageMode) // 事件采集方式
	{
		Freeze();
		CloseHandle(m_hEventSnapped);
		m_hEventSnapped = NULL;
	}
	else // 线程采集方式
	{
		TerminateGrabThread();
	}
	
	// 关闭触发
	CloseTriggerMode();
	
	// 断开连接
	if (m_pCamera)
	{
		m_pCamera->SetCallback(NULL, NULL);
		m_pCamera->StopCapture();
		m_pCamera->Disconnect();	
		delete m_pCamera;
		m_pCamera = NULL;
	}
	
	if (m_pSerialNum)
	{
		delete m_pSerialNum;
		m_pSerialNum = NULL;
	}
	
	// 清空
	if (m_pGrabImage != NULL)
	{
		delete [] m_pGrabImage;
		m_pGrabImage = NULL;
		
		m_lWidth = 0;
		m_lHeight = 0;
	}

	m_bOnline = false;

	m_cameraType = eInterfaceUnknow;

	return true;
}

// 获取相机的相关信息
bool scPTGrab2::GetCameraInfo(CameraInfo* pCamInfo)
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	error = m_pCamera->GetCameraInfo(pCamInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}

// 获取相机的序列号
bool scPTGrab2::GetCameraSerialNumber(unsigned int* pSerialNumber)
{
	if (m_pSerialNum)
	{
		*pSerialNumber = *m_pSerialNum;

		return true;
	}
	else
	{
		return false;
	}
}

// 获取相机的索引
bool scPTGrab2::GetCameraIndex(unsigned int* pCamIndex)
{	
	if (!IsOnline())
	{
		return false;
	}

	unsigned int uiCnt;
	unsigned int uiSerialNum;

	if (!GetCameraSerialNumber(&uiSerialNum))
	{
		return false;
	}

	if (!GetBusCameraCount(&uiCnt))
	{
		return false;
	}

	PTCameraInfo *pCamInfo = new PTCameraInfo[uiCnt];
	if (!EnumerateBusCameras(pCamInfo, &uiCnt))
	{
		delete []pCamInfo;
	
		return false;
	}

	for (int j=0; j<uiCnt; j++)
	{
		if (pCamInfo[j].m_uiSerialNum == uiSerialNum)
		{
			*pCamIndex = pCamInfo[j].m_uiIndex;

			delete []pCamInfo;

			return true;
		}
	}

	delete []pCamInfo;

	return false;
}

// 设置回调函数参数
void scPTGrab2::SetOwner(void* pOwner, PTGrabbedCallback funcCallback)
{
	m_pMainFrm = pOwner;
	m_funcCallback = funcCallback;
}

// 设置是否调用回调函数
void scPTGrab2::SetSendCallbackEnable(bool bEnable)
{
	m_bSendCallbackEnable = bEnable;
}

// 获取是否调用回调函数
bool scPTGrab2::GetSendCallbackEnable()
{
	return m_bSendCallbackEnable;
}

// 采集完成调用回调函数
void scPTGrab2::SendCallback(PTStatus status)
{
	// 调用回调函数
	if( (m_funcCallback != NULL) && (m_pGrabImage != NULL) )
	{
		if (ePTGrabOK == status)
		{
			m_funcCallback(m_pMainFrm, status, m_pGrabImage, m_lWidth, m_lHeight, m_pixelFormat);		
		}
		else
		{
			m_funcCallback(m_pMainFrm, status, NULL, NULL, NULL, ePTUnknown);		
		}
	}
}

// eWaitEvent采集模式下的内部采集触发回调函数
void scPTGrab2::PTImageEventCallback(Image *pImage, const void *pCallbackData)
{
	scPTGrab2 *pThis = (scPTGrab2*)pCallbackData;
	
	pThis->m_bAcquireSuceess = false;

	if (!pImage || !pImage->GetData())
	{
		Sleep(2);

		return;
	}
	
	if (ePTGrab == pThis->m_threadStatus) // 连续采集
	{	
		if (pThis->m_bIsGrabbing)
		{
			PTStatus status = ePTGrabOK;

			pThis->m_ImageRaw.DeepCopy(pImage);
				
			if (!pThis->TransferData())
			{
				status = ePTGrabFailed;
			}
			else
			{
				pThis->m_bAcquireSuceess = true;
			}
			
			if (pThis->m_bSendCallbackEnable)
			{
				pThis->SendCallback(status);
			}
		}

	}
	else if (ePTSnap == pThis->m_threadStatus) // 单帧采集
	{
		if (pThis->m_bIsSnap)
		{
			PTStatus status = ePTGrabOK;
			
			pThis->m_ImageRaw.DeepCopy(pImage);
			
			if (!pThis->TransferData())
			{
				status = ePTGrabFailed;
			}
			else
			{
				pThis->m_bAcquireSuceess = true;
			}
			
			if (pThis->m_bSendCallbackEnable)
			{
				pThis->SendCallback(status);
			}
			
			SetEvent(pThis->m_hEventSnapped);

			pThis->m_bIsSnap = false;
		}
	}

	Sleep(2);

	return;
} 

// 根据相机GUID初始化相机
bool scPTGrab2::InitFromGuid(PGRGuid *pGuid)
{
	Error error;
	
	// 释放资源
	Destroy();

	// 获取相机类型
	m_cameraType = GetCameraTypeFromGuid(pGuid);

	if (/*eGigE*/eUSB2 == m_cameraType) // GigE Camera
	{
		m_pCamera = new /*GigE*/Camera;
	}
 	else if (eIEEE1394 == m_cameraType || eUSB2 == m_cameraType || eUSB3 == m_cameraType) // 1394/USB Camera
 	{
 		m_pCamera = new Camera;
 	}
	else // Unknown Camera
	{
		return false;
	}
	
	// 连接
	error = m_pCamera->Connect(pGuid);
	if (error != PGRERROR_OK)
	{
		delete m_pCamera;
		m_pCamera = NULL;
		return false;
	}
	else
	{
		CameraInfo camInfo;
		error = m_pCamera->GetCameraInfo(&camInfo);
		if (error != PGRERROR_OK)
		{
			delete m_pCamera;
			m_pCamera = NULL;
			return false;
		}

		m_pSerialNum = new unsigned int;
		*m_pSerialNum = camInfo.serialNumber;

		// 关闭触发
		CloseTriggerMode();
		
		// 试采集图像一幅图像确定图像尺寸，并分配内存
		EnterCriticalSection(&m_criticalSection);
		// 开始采集
		Error error;
		error = m_pCamera->StartCapture(NULL,NULL);
		m_pCamera->WriteRegister(0x954,1);

 		if (error != PGRERROR_OK)
 		{
 			LeaveCriticalSection(&m_criticalSection);
 		
 			delete m_pCamera;
 			m_pCamera = NULL;
 
 			return false;
 		}
		// 获取图像
		if (ePTGrabOK != AcquireImage())
		{
			LeaveCriticalSection(&m_criticalSection);
			
			delete m_pCamera;
			m_pCamera = NULL;

			return false;
		}
		// 停止采集
		m_pCamera->StopCapture();
		
		LeaveCriticalSection(&m_criticalSection);
		
		// 采集初始化
		if (eWaitEvent == m_grabImageMode) // 事件采集方式
		{
			m_hEventSnapped = CreateEvent(NULL,true,false,_T(""));
		}
		else // 线程采集方式
		{
			if (!CreateGrabThread())
			{
				delete m_pCamera;
				m_pCamera = NULL;
				return false;
			}
		}
		
		m_bOnline = true;

		return true;
	}
}
												
// 根据相机序列号初始化相机
bool scPTGrab2::InitFromSerial(unsigned int uiSerialNum) 
{
	PGRGuid guid;

	GetCameraGuidFromSerialNum(uiSerialNum, &guid);

	return (InitFromGuid(&guid));
}

// 根据索引号初始化相机
bool scPTGrab2::InitFromIndex(unsigned int nCamIndex) 
{
	PGRGuid guid;

	GetCameraGuidFromIndex(nCamIndex, &guid);
	
	return (InitFromGuid(&guid));
}

// 开始连续采集（支持FreeRun、触发两种模式）
// 调用该连续采集函数，没有等待立即返回
bool scPTGrab2::Grab()
{
	if (!IsInit())
	{
		return false;
	}

	if (eWaitEvent == m_grabImageMode) // 事件采集方式
	{
		Stop();
		
		m_threadStatus = ePTGrab;
		m_bIsGrabbing = true;
		m_bIsSnap = false;
		
		Start();
	}
	else // 线程采集方式
	{
		Stop();

		Start();
	
		if (!m_bIsGrabbing)
		{
			m_bAcquireSuceess = false;
			m_threadStatus = ePTGrab;
			m_bIsGrabbing = true;
			SetEvent(m_hEventToRun);
		}
	}

	return IsOnline();
}	

// 结束连续采集	
bool scPTGrab2::Freeze() 
{
	if (!IsInit())
	{
		return false;
	}

	if (eWaitEvent == m_grabImageMode) // 事件采集方式
	{
		m_bFreezeStop	= true;
		Stop();
		m_threadStatus	= ePTIdle;
		m_bIsGrabbing	= false;
		m_bIsSnap		= true;
		m_bFreezeStop   = false;
	}
	else // 线程采集方式
	{
		m_bFreezeStop = true;
		
		Stop();
	
		if (m_bIsGrabbing || !m_bHaveStopped)
		{
			m_threadStatus = ePTIdle;
			m_bIsGrabbing = false;
			SetEvent(m_hEventToRun);
			int i = 0;
			while (!m_bHaveStopped && i < 500)
			{
				Sleep(2);
				i++;	
			}
		}

		m_bFreezeStop = false;
	}

	return IsOnline();
}

// 获取是否处于连续采集状态
bool scPTGrab2::IsGrabbing()
{
	if (!IsOnline())
	{
		return false;
	}

	return m_bIsGrabbing;
}

// 采集单帧图像（只支持FreeRun模式）。
// 如果bWaitSnapped=true, 等待直到采集完成才返回；
// 如果bWaitSnapped=false, 调用后不等待采集完直接返回；
bool scPTGrab2::Snap(bool bWaitSnapped)
{
	if (!IsInit())
	{
		return false;
	}

	if (eWaitEvent == m_grabImageMode) // 事件采集方式
	{
		Stop();
		
		if (bWaitSnapped)
		{
			ResetEvent(m_hEventSnapped); // 重置单帧采集完成事件
		}
		
		m_threadStatus = ePTSnap;
		m_bIsGrabbing = false;
		m_bIsSnap = true;
		
		Start();
		if (bWaitSnapped)
		{
			// 等待单帧采集完成事件，1秒钟之内采集不完退出等待，返回false
			if (WAIT_TIMEOUT == WaitForSingleObject(m_hEventSnapped, 1000))
			{
				return false;
			}	

			return IsAcquireSuccess();
		}	

	}
	else
	{
		// 如果采集线程处于连续采集状态，则返回false
		if (m_bIsGrabbing || !m_bHaveStopped)
		{
			return false;
		}
		
		//add by he 
		//	Stop();	
		
		if (!Start())
		{
			Stop();
			Start();
		}
			
		m_bAcquireSuceess = false;
		m_threadStatus = ePTSnap;
		if (bWaitSnapped)
		{
			ResetEvent(m_hEventSnapped);		// 重置单帧采集完成事件
		}
		
		SetEvent(m_hEventToRun);				// 发送运行采集事件
		
		if (bWaitSnapped)
		{
			// 等待单帧采集完成事件，1秒钟之内采集不完退出等待，返回false
			if (WAIT_TIMEOUT == WaitForSingleObject(m_hEventSnapped, 1000))
			{
				return false;
			}
		
			return IsAcquireSuccess();
		}

	}

	return IsOnline();
}

// 开始
bool scPTGrab2::Start()
{
	if (!IsOnline())
	{
		return false;
	}

	if (eWaitEvent == m_grabImageMode) // 事件采集方式
	{
		Error error;
		error = m_pCamera->StartCapture(PTImageEventCallback, this);
		if (error != PGRERROR_OK)
		{
			return false;
		}
	}
	else // 线程采集方式
	{
		Error error;
		// 需先将回调函数设为空
		m_pCamera->SetCallback(NULL, NULL);
		error = m_pCamera->StartCapture(NULL, NULL);

// 		while(error != PGRERROR_OK)
// 		{
// 			m_pCamera->StopCapture();
// 			Sleep(1);
// 			m_pCamera->StartCapture(NULL, NULL);
// 		}

		if (error != PGRERROR_OK)
		{
			return false;
		}
	}

	return true;
}

// 停止												
bool scPTGrab2::Stop()
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	error = m_pCamera->StopCapture();
// 	while(error != PGRERROR_OK)
// 	{
// 		m_pCamera->StopCapture();
// 		Sleep(1);
// 	}

// 	if (error != PGRERROR_OK)
// 	{
// 		return false;
// 	}
	
	return true;
}

// 图像数据格式转换：
// 首先分配图像内存，
// 需要考虑图像的宽高、格式。
// 目前支持三种PGR的像素格式：FLYCAPTURE_MONO8、FLYCAPTURE_RAW8和FLYCAPTURE_RGB8
// eGrey8：对应于PGR的FLYCAPTURE_MONO8、FLYCAPTURE_RAW8；
// eRGB24：对应于FLYCAPTURE_RGB8，但需要将R-G-B排列的数据转换为B-G-R排列的数据
bool scPTGrab2::TransferData()
{
	if (NULL == m_ImageRaw.GetData())
	{
		return false;
	}
	
	// 获取图像像素格式
	PTPixelFormat pixelFormat;	// 像素格式
	int nBytes;					// 每个像素占用的字节数
	switch(m_ImageRaw.GetPixelFormat())
	{
	case PIXEL_FORMAT_MONO8:
		{
			pixelFormat = ePTGrey8;
			nBytes = 1;
		}
		break;
	case PIXEL_FORMAT_RGB8:
	case PIXEL_FORMAT_RAW8:
		{
			pixelFormat = ePTRGB24;
			nBytes = 3;
		}
		break;
	default:
		return false;
	}
	
	// 获取图像的宽高
	LONG lWidth = m_ImageRaw.GetCols();
	LONG lHeight = m_ImageRaw.GetRows();
	
	// 分配图像数据内存
	if (NULL == m_pGrabImage)
	{
		m_lWidth = lWidth;
		m_lHeight = lHeight;
		m_pixelFormat = pixelFormat;
		m_pGrabImage = new unsigned char[m_lWidth * m_lHeight * nBytes];
	}
	else
	{
		if (m_lWidth != lWidth || m_lHeight != lHeight || m_pixelFormat != pixelFormat)
		{
			delete[] m_pGrabImage;
			m_pGrabImage = NULL;
			
			m_lWidth = lWidth;
			m_lHeight = lHeight;
			m_pixelFormat = pixelFormat;
			m_pGrabImage = new unsigned char[m_lWidth * m_lHeight * nBytes];
		}
	}
	
	// 判断内存分配是否成功
	if (NULL == m_pGrabImage)
	{
		return false;
	}
	
	switch(m_ImageRaw.GetPixelFormat())
	{
	case PIXEL_FORMAT_MONO8:
	case PIXEL_FORMAT_RGB8:
		{
			// 拷贝图像内存
			memcpy(m_pGrabImage, m_ImageRaw.GetData(), m_lWidth * m_lHeight * nBytes);
		}
		break;
	case PIXEL_FORMAT_RAW8:
		{
			// 拷贝图像内存
			Image convertedImage;
			
			// Convert the raw image
			m_ImageRaw.Convert( PIXEL_FORMAT_BGR, &convertedImage );
			int a  = convertedImage.GetBitsPerPixel();
			memcpy(m_pGrabImage, convertedImage.GetData(), m_lWidth * m_lHeight * nBytes);
		}
		break;
	default:
		return false;
	}
	
	// 如果PGR像素格式是FLYCAPTURE_RGB8，则需要将R-G-B排列的数据转换为B-G-R排列的数据
	if (PIXEL_FORMAT_RGB8 == m_ImageRaw.GetPixelFormat()) // RGB转为BGR
	{
		for (int j=0; j<lHeight; j++)
			for (int i=0; i<lWidth*3; i+=3)
			{
				BYTE byTemp;
				byTemp = m_pGrabImage[j*lWidth*3+i];
				m_pGrabImage[j*lWidth*3+i] = m_pGrabImage[j*lWidth*3+i+2];
				m_pGrabImage[j*lWidth*3+i+2] = byTemp;
			}
	}
	
	return true;
}

// 判断相机是否支持指定视频格式
// GigE相机只支持Format7模式
bool scPTGrab2::IsVideoModeSupported(VideoMode vmVideoMode)
{
	if (!IsOnline())
	{
		return false;
	}
	
	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
	{
		return false;
	}
	
	if (eGigE == m_cameraType) // GigE Camera
	{
		if (VIDEOMODE_FORMAT7 == vmVideoMode)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Camera *pCamera = (Camera*)m_pCamera;

	if ( vmVideoMode == VIDEOMODE_640x480RGB
		|| vmVideoMode == VIDEOMODE_640x480Y8
		|| vmVideoMode == VIDEOMODE_800x600RGB
		|| vmVideoMode == VIDEOMODE_800x600Y8
		|| vmVideoMode == VIDEOMODE_1024x768RGB
		|| vmVideoMode == VIDEOMODE_1024x768Y8
		|| vmVideoMode == VIDEOMODE_1280x960RGB
		|| vmVideoMode == VIDEOMODE_1280x960Y8
		|| vmVideoMode == VIDEOMODE_1600x1200Y8
		|| vmVideoMode == VIDEOMODE_1600x1200Y8)
	{
		Error error;
		bool bSupported = false;
		for (int j = 0; j < sizeof(g_frFrameRate)/sizeof(g_frFrameRate[0])-1; j++)
		{
			error = pCamera->GetVideoModeAndFrameRateInfo(vmVideoMode, g_frFrameRate[j], &bSupported);
			if ((error == PGRERROR_OK) && bSupported)
			{
				break;
			}
		}
		
		return bSupported;
	}
	else if (VIDEOMODE_FORMAT7 == vmVideoMode) //	默认支持Fmt7
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 判断相机是否支持指定帧率模式
// GigE相机只支持Format7模式
bool scPTGrab2::IsFrameRateSupported(FrameRate frFrameRate)						
{
	if (!IsOnline())
	{
		return false;
	}
	
	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
	{
		return false;
	}	
	
	// query current video mode
	VideoMode vmVideoMode;
	if (!GetCurVideoMode(&vmVideoMode))
	{
		return false;
	}

	if (VIDEOMODE_FORMAT7 == vmVideoMode|| eGigE == m_cameraType) 
	{
		if (FRAMERATE_FORMAT7 == frFrameRate)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	Camera *pCamera = (Camera*)m_pCamera;
//	Camera *pCamera = (Camera*)m_pCamera;
		
	// is supported ?
	Error error;
	bool bSupported = false;
	error = pCamera->GetVideoModeAndFrameRateInfo(vmVideoMode, frFrameRate, &bSupported);
	if (error != PGRERROR_OK) return false;
	
	return bSupported;
}

// 设置视频模式
// 设置除Format7之外的其它视频模式
bool scPTGrab2::SetCurVideoMode(VideoMode vmVideoMode)
{
	if (!IsOnline())
	{
		return false;
	}

	if (!IsVideoModeSupported(vmVideoMode))
	{
		return false;
	}

	if (eGigE == m_cameraType) // GigE Camera
	{
		if (VIDEOMODE_FORMAT7 == vmVideoMode)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool bIsGrabbing = IsGrabbing();
	if (bIsGrabbing)
	{
		Freeze();
	}
			
	//设置寄存器
	unsigned long ulFormat, ulMode;
	if (!ConvertVideoModeToULongs(vmVideoMode, &ulFormat, &ulMode))
	{
		return false;
	}
	ulFormat = ulFormat<<29;
	ulMode = ulMode<<29;
	
	Error error;
	error = m_pCamera->WriteRegister(0X608, ulFormat);
	if (error != PGRERROR_OK) return false;
	
	error = m_pCamera->WriteRegister(0X604, ulMode);
	if (error != PGRERROR_OK) return false;			
	
	if (bIsGrabbing)
	{
		Grab();
	}

	return true;
}

// 获取当前视频模式
bool scPTGrab2::GetCurVideoMode(VideoMode* pvmVideoMode)
{
	if (!IsOnline())
	{
		return false;
	}

	if (NULL == pvmVideoMode)
	{
		return false;
	}	
	
	if (eGigE == m_cameraType) // GigE Camera
	{
		*pvmVideoMode = VIDEOMODE_FORMAT7;

		return true;
	}
	
	Camera *pCamera = (Camera*)m_pCamera;

	Error error;
	VideoMode vmTemp;
	FrameRate frTemp;
	error = pCamera->GetVideoModeAndFrameRate(&vmTemp, &frTemp);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	else
	{
		*pvmVideoMode = vmTemp;
		
		return true;
	}
}

// 设置帧率模式
// 设置除Format7之外的其它帧率模式
bool scPTGrab2::SetCurFrameRate(FrameRate frFrameRate)						
{
	if (!IsOnline())
	{
		return false;
	}

	if (!IsFrameRateSupported(frFrameRate))
	{
		return false;
	}

	VideoMode vmMode;
	if (!GetCurVideoMode(&vmMode))
	{
		return false;
	}
	
	if (VIDEOMODE_FORMAT7 == vmMode || eGigE == m_cameraType)
	{
		if (FRAMERATE_FORMAT7 == frFrameRate)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	unsigned long ulFrameRate;
	if (!ConvertFrameRateToULong(frFrameRate, &ulFrameRate))
	{
		return false;
	}
	
	ulFrameRate = ulFrameRate<<29;
	
	Error error = m_pCamera->WriteRegister(0X600, ulFrameRate);
	if (error != PGRERROR_OK) return false;
	
	return true;
}

// 获取帧率模式
bool scPTGrab2::GetCurFrameRate(FrameRate* pfrFrameRate)							
{
	if (!IsOnline())
	{
		return false;
	}

	if (NULL == pfrFrameRate)
	{
		return false;
	}	
	
	VideoMode vmMode;
	if (!GetCurVideoMode(&vmMode))
	{
		return false;
	}

	if (VIDEOMODE_FORMAT7 == vmMode)
	{
		*pfrFrameRate = FRAMERATE_FORMAT7;
		
		return true;
	}
	
	Error error;
	unsigned int ulValue = 0;	
	error = m_pCamera->ReadRegister(0x600, &ulValue);
	if (error != PGRERROR_OK) return false;
	
	ulValue = ulValue>>29;	
	*pfrFrameRate = g_frFrameRate[ulValue];
	
	return true;
}

// 获取在当前视频模式下，支持的最大帧率
bool scPTGrab2::GetMaxFrameRate(FrameRate* pfrFrameRate)						
{
	if (!IsOnline())
	{
		return false;
	}

	if (NULL == pfrFrameRate)
	{
		return false;
	}	
	
	VideoMode vmCurVideoMode;
	if (!GetCurVideoMode(&vmCurVideoMode))
	{
		return false;
	}

	if (VIDEOMODE_FORMAT7 == vmCurVideoMode|| eGigE == m_cameraType) 
	{
		*pfrFrameRate = FRAMERATE_FORMAT7;
		
		return true;
	}	
	
	Camera *pCamera = (Camera*)m_pCamera;
		
	// 查询可用的最大帧率，如果有则返回true，否则返回false
	Error error;
	bool bSupported;
	int nIndex = 0;
	int nFrameRateCount = sizeof(g_frFrameRate)/sizeof(g_frFrameRate[0]);
	for (nIndex = nFrameRateCount - 1; nIndex >= 0; nIndex--)
	{
		error = pCamera->GetVideoModeAndFrameRateInfo(vmCurVideoMode, g_frFrameRate[nIndex], &bSupported);
		if (error != PGRERROR_OK) return false;
		if (bSupported)
		{
			*pfrFrameRate = g_frFrameRate[nIndex];
			return true;
		}
	}
	
	return false;
}

// 设置帧率
bool scPTGrab2::SetCurFrameRate(float fFrameRate)					              
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = FRAME_RATE;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	
	// 设置
	cameraProperty.absValue = fFrameRate;
	cameraProperty.absControl = true;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	
	return true;
}

// 获取当前帧率
bool scPTGrab2::GetCurFrameRate(float *pfFrameRate)					           
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = FRAME_RATE;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfFrameRate = cameraProperty.absValue;
	
	return true;
}

// 获取可用的帧率范围
bool scPTGrab2::GetFrameRateRange(float *fMinFrameRate, float *fMaxFrameRate)	                      
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = FRAME_RATE;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*fMinFrameRate = cameraPropertyInfo.absMin;
	*fMaxFrameRate = cameraPropertyInfo.absMax;
	
	return true;
}

// 设置曝光时间，单位ms
bool scPTGrab2::SetCurShutter(float fShutter)									
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = SHUTTER;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	// 设置
	cameraProperty.absValue = fShutter;
	cameraProperty.absControl = true;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	

	return true;
}

// 设置曝光时间，单位ms
bool scPTGrab2::GetCurShutter(float* pfShutter)										
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	Property cameraProperty;
	cameraProperty.type = SHUTTER;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	*pfShutter = cameraProperty.absValue;
	
	return true;
}

// 获取可用的曝光时间范围，单位ms
bool scPTGrab2::GetShutterRange(float *pfMinShutter, float *pfMaxShutter)
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = SHUTTER;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfMinShutter = cameraPropertyInfo.absMin;
	*pfMaxShutter = cameraPropertyInfo.absMax;

	return true;
}

// 设置增益，单位db
bool scPTGrab2::SetCurGain(float fGain)												
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = GAIN;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	// 设置
	cameraProperty.absValue = fGain;
	cameraProperty.absControl = true;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	
	return true;
}

// 获取增益，单位db
bool scPTGrab2::GetCurGain(float* pfGain)											
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	Property cameraProperty;
	cameraProperty.type = GAIN;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfGain = cameraProperty.absValue;

	return true;
}

// 获取可用的增益范围，单位db
bool scPTGrab2::GetGainRange(float *pfMinGain, float *pfMaxGain)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = GAIN;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	*pfMinGain = cameraPropertyInfo.absMin;

	*pfMaxGain = cameraPropertyInfo.absMax;
	
	return true;
}

// 设置Sharpness
bool scPTGrab2::SetCurSharpness(unsigned int uiSharpness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = SHARPNESS;
	cameraProperty.absControl = false;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	// 设置
	cameraProperty.valueA = uiSharpness;
	cameraProperty.type = SHARPNESS;
	cameraProperty.absControl = false;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	
	return true;
}	

// 获取当前Sharpness												
bool scPTGrab2::GetCurSharpness(unsigned int* piSharpness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = SHARPNESS;
	cameraProperty.absControl = false; //Sharpness是absolute模式
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*piSharpness = cameraProperty.valueA;
	
	return true;
}
	
// 获取Sharpness范围										
bool scPTGrab2::GetSharpnessRange(unsigned int *piMinSharpness, unsigned int *piMaxSharpness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = SHARPNESS;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*piMinSharpness = cameraPropertyInfo.min;
	
	*piMaxSharpness = cameraPropertyInfo.max;
	
	return true;
}	
			
// 设置Gamma
bool scPTGrab2::SetCurGamma(float fGamma)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = GAMMA;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	// 设置
	cameraProperty.absValue = fGamma;
	cameraProperty.absControl = true;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	
	return true;
}	

// 获取当前Gamma													
bool scPTGrab2::GetCurGamma(float* pfGamma)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = GAMMA;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfGamma = cameraProperty.absValue;
	
	return true;
}	

// 获取Gamma范围													
bool scPTGrab2::GetGammaRange(float *pfMinGamma, float *pfMaxGamma)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = GAMMA;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfMinGamma = cameraPropertyInfo.absMin;
	
	*pfMaxGamma = cameraPropertyInfo.absMax;
	
	return true;
}	
							
// 设置Brightness
bool scPTGrab2::SetCurBrightness(float fBrightness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = BRIGHTNESS;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	// 设置
	cameraProperty.absValue = fBrightness;
	cameraProperty.absControl = true;
	cameraProperty.onOff = true;
	cameraProperty.autoManualMode = false; // 手动设置模式
	error = m_pCamera->SetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	
	return true;
}	

// 获取当前Brightness											
bool scPTGrab2::GetCurBrightness(float* pfBrightness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	Property cameraProperty;
	cameraProperty.type = BRIGHTNESS;
	cameraProperty.absControl = true;
	error = m_pCamera->GetProperty(&cameraProperty);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfBrightness = cameraProperty.absValue;
	
	return true;
}	

// 获取Brightness范围
bool scPTGrab2::GetBrightnessRange(float *pfMinBrightness, float *pfMaxBrightness)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	PropertyInfo cameraPropertyInfo;
	cameraPropertyInfo.type = BRIGHTNESS;
	error = m_pCamera->GetPropertyInfo(&cameraPropertyInfo);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pfMinBrightness = cameraPropertyInfo.absMin;
	
	*pfMaxBrightness = cameraPropertyInfo.absMax;
	
	return true;
}

// Poll the camera to make sure the camera is actually in trigger mode
// before we start it (avoids timeouts due to the trigger not being armed)
bool scPTGrab2::CheckTriggerReady()
{
	if (!IsOnline())
	{
		return false;
	}

	const unsigned int k_softwareTrigger = 0x62C;
    Error error;
    unsigned int regVal = 0;
	
    do 
    {
        error = m_pCamera->ReadRegister( k_softwareTrigger, &regVal );
        if (error != PGRERROR_OK)
        {
			return false;
        }
		
    } while ( (regVal >> 31) != 0 );
	
	return true;
}                           

// Check that the camera actually supports the PGR SOFT_ASYNC_TRIGGER or SOFTWARE_TRIGGER
// method of software triggering
bool scPTGrab2::CheckSoftwareTriggerPresence()
{
	if (!IsOnline())
	{
		return false;
	}

	const unsigned int k_triggerInq = 0x530;
	
	Error error;
	unsigned int regVal = 0;
	
	error = m_pCamera->ReadRegister( k_triggerInq, &regVal );
	
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	if( ( regVal & 0x10000 ) != 0x10000 )
	{
		return false;
	}
	
	return true;
}

// 设置相机的触发模式(软触发、硬触发、或是非触发)。
bool scPTGrab2::SetTriggerMode(PTTriggerMode mode, unsigned long ulTimeout )    
{
	if (!IsOnline())
	{
		return false;
	}

	if (ePTNotTrigger == mode)
	{
		return CloseTriggerMode();
	}
	
	Error   error;	
	TriggerMode triggerMode;
    error = m_pCamera->GetTriggerMode( &triggerMode );
    if (error != PGRERROR_OK)
    {
		CloseTriggerMode();

        return false;
    }
	triggerMode.onOff = true;
	triggerMode.mode = 0;
	triggerMode.parameter = 0;
	if (ePTSoftwareTrigger == mode) // 软触发
	{
		triggerMode.source = 7;
	}
	else
	{
		triggerMode.source = 0;
	}
	error = m_pCamera->SetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		CloseTriggerMode();

		return false;
	}
	m_nTriggerMode = mode;
	
	bool retVal = CheckTriggerReady();
	if( !retVal )
	{
		CloseTriggerMode();

		return false;
	}

	FC2Config config;
    error = m_pCamera->GetConfiguration( &config );
    if (error != PGRERROR_OK)
    {
		CloseTriggerMode();

        return false;
    } 

	// 设置采集延时
    config.grabTimeout = ulTimeout;
    error = m_pCamera->SetConfiguration( &config );
    if (error != PGRERROR_OK)
    {
		CloseTriggerMode();

        return false;
    } 
	
//	Start();

	return true;
}   

// 获取当前的触发模式           
PTTriggerMode scPTGrab2::GetTriggerMode()											
{
	if (!IsOnline())
	{
		return ePTNotTrigger;
	}

	TriggerMode triggerMode;
	Error error;
    error = m_pCamera->GetTriggerMode( &triggerMode );
    if (error != PGRERROR_OK)
    {
        return ePTNotTrigger;
    }

	if (triggerMode.onOff)
	{
		if (7 == triggerMode.source)
		{
			return ePTSoftwareTrigger;
		}
		else
		{
			return ePTHardwareTrigger;
		}
	}

	return ePTNotTrigger;
}

// 关闭触发模式
bool scPTGrab2::CloseTriggerMode()											
{
	if (!m_pCamera)
	{
		return false;
	}

	Error error;
	TriggerMode triggerMode;
	error = m_pCamera->GetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	triggerMode.onOff = false;    
    error = m_pCamera->SetTriggerMode( &triggerMode );
    if (error != PGRERROR_OK)
    {
        return false;
    }

	m_nTriggerMode = ePTNotTrigger;

//	m_pCamera->StopCapture();

	return true;
}

// 是否以触发方式（软触发或硬触发）采集图像
bool scPTGrab2::IsTrigger()														
{
	if (!IsOnline())
	{
		return false;
	}

	if ((ePTSoftwareTrigger == m_nTriggerMode) || (ePTHardwareTrigger == m_nTriggerMode))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 用触发方式采集图像：软触发
bool scPTGrab2::SendSoftTrigger()
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;	
	if (m_nTriggerMode == ePTSoftwareTrigger)
	{
		if(CheckSoftwareTriggerPresence())
		{
			if(CheckTriggerReady())
			{
				error = m_pCamera->WriteRegister(0x62C, 0x80000000);
				if (error != PGRERROR_OK)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}	

// 获取图像的像素格式
PTPixelFormat scPTGrab2::GetImagePixelFormat()									
{
	if (!IsOnline())
	{
		return ePTUnknown;
	}

	return m_pixelFormat;
}

// 获取图像的宽					
long scPTGrab2::GetImageWidth()												
{
	if (!IsOnline())
	{
		return 0;
	}

	return 	m_lWidth;	
}

// 获取图像的高									
long scPTGrab2::GetImageHeight()												
{
	if (!IsOnline())
	{
		return 0;
	}

	return m_lHeight;	
}

// 获取存储图像所需的内存大小（字节）
long scPTGrab2::GetImageDataSize()											
{
	if (!IsOnline())
	{
		return 0;
	}

	int nBytes = 1;
	
	switch(m_pixelFormat)
	{
	case ePTGrey8 :
		nBytes = 1;
		break;
	case ePTRGB24 :
		nBytes = 3;
		break;
	case ePTRGBA32 :
		nBytes = 4;
		break;
	default :
		nBytes = 1;
	}
	
	return m_lWidth * m_lHeight * nBytes;	
}

// 获取图像数据，要求外部已分配好内存
// 内存的大小由GetImageDataSize()获得；
bool scPTGrab2::GetImageData(unsigned char* pImage)						
{
	if (!IsOnline())
	{
		return false;
	}

	if (NULL == pImage)
	{
		return false;
	}
	
	EnterCriticalSection(&m_criticalSection);
	if (!m_bAcquireSuceess||
		 m_pGrabImage == NULL || m_lWidth <= 0 || m_lHeight <= 0)
	{
		LeaveCriticalSection(&m_criticalSection);
	
		return false;
	}
	
	memcpy(pImage, m_pGrabImage, GetImageDataSize());	
	
	LeaveCriticalSection(&m_criticalSection);

	return true;	
}
	
// 将指定视频模式转为在相机寄存器中相应的索引值
bool  scPTGrab2::ConvertVideoModeToULongs(VideoMode vmVideoMode, unsigned long* pulFormat, unsigned long* pulMode)
{	
	if (NULL == pulFormat || NULL == pulMode)
	{
		return false;
	}
	
	unsigned long ulFormat = 0;
	unsigned long ulMode =0;
	for (ulFormat = 0; ulFormat < 8; ulFormat++)
	{
		for (ulMode = 0; ulMode < 8; ulMode++)
		{
			if (vmVideoMode == g_fvmFormatVideoMode[ulFormat][ulMode])
			{
				*pulFormat = ulFormat;
				*pulMode = ulMode;
			
				return true;
			}
		}
	}
	
	return false;
}

// 将指定帧率模式转为在相机寄存器中相应的索引值
bool scPTGrab2::ConvertFrameRateToULong(FrameRate frFrameRate, unsigned long* pulFrameRate)
{
	if (NULL == pulFrameRate)
	{
		return false;
	}
	
	switch(frFrameRate)
	{
		// 1.875 fps. (Frames per second)
	   case FRAMERATE_1_875:
		   *pulFrameRate = 0;
		   break;
		   // 3.75 fps.
	   case FRAMERATE_3_75:
		   *pulFrameRate = 1;
		   break;
		   // 7.5 fps.
	   case FRAMERATE_7_5:
		   *pulFrameRate = 2;
		   break;
		   // 15 fps.
	   case FRAMERATE_15:
		   *pulFrameRate = 3;
		   break;
		   // 30 fps.
	   case FRAMERATE_30:
		   *pulFrameRate = 4;
		   break;
		   // 60 fps.
	   case FRAMERATE_60:
		   *pulFrameRate = 5;
		   break;
		   // 120 fps.
	   case FRAMERATE_120:
		   *pulFrameRate = 6;
		   break;
		   // 240 fps.
	   case FRAMERATE_240:
		   *pulFrameRate = 7;
		   break;
	   case FRAMERATE_FORMAT7:
		   *pulFrameRate = 8;
		   break;
	   default:
		   return false;
	}
	
	return true;
}

// 根据当前视频模式充值当前帧率模式
bool scPTGrab2::ResetCurFrameRateByVideoMode()
{
	if (!IsOnline())
	{
		return false;
	}

	FrameRate frFrameRate;
	if (!GetCurFrameRate(&frFrameRate))
	{
		return false;
	}
	if (IsFrameRateSupported(frFrameRate))
	{
		return true;
	}
	
	if (!GetMaxFrameRate(&frFrameRate))
	{
		return false;
	}
	
	return SetCurFrameRate(frFrameRate);
}

// 根据当前视频模式重置曝光时间参数
bool scPTGrab2::ResetCurShutterByFrameRate()
{
	if (!IsOnline())
	{
		return false;
	}

	float fMaxShutter;
	float fCurShutter;
	float fMinShutter;
	if (!GetShutterRange(&fMinShutter, &fMaxShutter))
	{
		return false;
	}
	
	if (!GetCurShutter(&fCurShutter))
	{
		return false;
	}
	
	if (fCurShutter > fMaxShutter)
	{
		SetCurShutter(fMaxShutter);
	}
	
	return true;
}

// 获取当前图像（灰度）
// bool scPTGrab2::GetImage(scImageGray& image, long alignModulus /* = 1 */)
// {
// 	if (!IsOnline())
// 	{
// 		return false;
// 	}
// 
// 	EnterCriticalSection(&m_criticalSection);
// 	
// 	if (m_pixelFormat != ePTGrey8
// 		|| !m_bAcquireSuceess 
// 		|| m_pGrabImage == NULL || m_lWidth <= 0 || m_lHeight <= 0)
// 	{
// 		LeaveCriticalSection(&m_criticalSection);
// 	
// 		return false;
// 	}
// 	image.CreateImageBuffer(m_lWidth, m_lHeight, alignModulus);
// 	
// 	unsigned char* pDst;
// 	unsigned char* pSrc = m_pGrabImage;
// 	int nBytes = sizeof(unsigned char);
// 	int i = 0;
// 	for (i = 0; i < m_lHeight; i++)
// 	{
// 		pDst = image.PointToRow(i);
// 		memcpy(pDst, pSrc, m_lWidth * nBytes);
// 		
// 		pSrc += m_lWidth * nBytes;
// 	}
// 	
// 	LeaveCriticalSection(&m_criticalSection);
// 	
// 	return true;
// }
// 
// // 获取当前图像（彩色）
// bool scPTGrab2::GetImage(scImageRGB& image, long alignModulus /* = 1 */)
// {
// 	if (!IsOnline())
// 	{
// 		return false;
// 	}
// 
// 	EnterCriticalSection(&m_criticalSection);
// 	
// 	if (m_pixelFormat != ePTRGB24
// 		|| !m_bAcquireSuceess 
// 		|| m_pGrabImage == NULL || m_lWidth <= 0 || m_lHeight <= 0)
// 	{
// 		LeaveCriticalSection(&m_criticalSection);
// 	
// 		return false;
// 	}
// 	image.CreateImageBuffer(m_lWidth, m_lHeight, alignModulus);
// 	
// 	unsigned char* pDst;
// 	unsigned char* pSrc = m_pGrabImage;
// 	int nBytes = 3 * sizeof(unsigned char);
// 	int i = 0;
// 	for (i = 0; i < m_lHeight; i++)
// 	{
// 		pDst = image.PointToRow(i);
// 		memcpy(pDst, pSrc, m_lWidth * nBytes);
// 		
// 		pSrc += m_lWidth * nBytes;
// 	}
// 	
// 	LeaveCriticalSection(&m_criticalSection);
// 	
// 	return true;
// }

// 设置采集延时，单位：ms
bool scPTGrab2::SetGrabTimeout(unsigned long ulTimeout)
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	FC2Config config;
	error = m_pCamera->GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	config.grabTimeout = ulTimeout;
	
	error = m_pCamera->SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}	

// 设置监控采集延时，线程采集方式
bool scPTGrab2::SetMonitorTimeout(unsigned long ulTimeout)
{
	if (!IsOnline())
	{
		return false;
	}

	m_lMonitorTimeout = ulTimeout;

	return true;
}									

//  将当前参数保存到相机中
//  lChannel:通道号
bool scPTGrab2::SaveToCamera(long lChannel)
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	error = m_pCamera->SaveToMemoryChannel(lChannel);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}

// 设置异步和同步总线速度
bool scPTGrab2::SetBusSpeed(BusSpeed  asyncBusSpeed, BusSpeed  isochBusSpeed)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	FC2Config config;
	error = m_pCamera->GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	config.asyncBusSpeed = asyncBusSpeed;
	config.isochBusSpeed = isochBusSpeed;
	
	error = m_pCamera->SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	return true;
}

// 获取异步和同步总线速度
bool scPTGrab2::GetBusSpeed( BusSpeed* pAsyncBusSpeed, BusSpeed* pIsochBusSpeed)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	FC2Config config;
	error = m_pCamera->GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	*pAsyncBusSpeed = config.asyncBusSpeed;
	*pIsochBusSpeed = config.isochBusSpeed;	
	
	return true;
}

// 获取相机类型
PTCameraType scPTGrab2::GetCameraType()
{
	if (!IsOnline())
	{
		return eInterfaceUnknow;
	}

	return m_cameraType;
}

// 写寄存器
// uiAdd:地址；uiValue：写入值
bool scPTGrab2::WriteRegisiter(unsigned int uiAdd, unsigned int uiValue)
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	error = m_pCamera->WriteRegister(uiAdd, uiValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}

// 读寄存器
// uiAdd:地址；uiValue：读入值
bool scPTGrab2::ReadRegisiter(unsigned int uiAdd, unsigned int *uiValue)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error error;
	error = m_pCamera->ReadRegister(uiAdd, uiValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	return true;
}

// 相机是否连接
bool scPTGrab2::IsOnline()
{
	if (!m_bOnline || !m_pSerialNum)
	{
		return false;
	}

	if (!GetBusManager())
	{
		return true;
	}

	std::map<unsigned int, bool>::iterator iter;
	iter = m_AutoBusManager.get()->m_BusCamaraOnlineInfo.find(*m_pSerialNum);
	if (iter != m_AutoBusManager.get()->m_BusCamaraOnlineInfo.end())
	{
		return iter->second;
	}

	return true;
}

// 初始化是否成功
bool scPTGrab2::IsInit()
{
	if (!m_bOnline || !m_pSerialNum)
	{
		return false;
	}

	return true;
}																		

// 采集线程函数
// 只有当采集模式为eGrabThread时，才会创建采集线程
UINT scPTGrab2::GrabThread(LPVOID lpParam)
{ 
	scPTGrab2 *pOwner=(scPTGrab2*)lpParam;
	
	if (NULL == pOwner)
	{
		return 0;
	}
	
	for( ; ; )
	{
		WaitForSingleObject(pOwner->m_hEventToRun, INFINITE);	// 当收到了运行采集循环的事件，包括单帧采集和连续采集
		if (pOwner->m_bTerminate)								// 如果结束线程标志为真，则退出总体循环
		{
			break;
		}
		
		switch(pOwner->m_threadStatus)
		{
		case ePTSnap:
			{
				PTStatus status = pOwner->AcquireImage();		// 采集获取一幅图像
				if (pOwner->m_bSendCallbackEnable)
				{
					pOwner->SendCallback(status);				// 调用SendCallback(true)函数
				}
				
				ResetEvent(pOwner->m_hEventToRun);				// 允许继续进行采集
				SetEvent(pOwner->m_hEventSnapped);				// 通知调用线程停止等待
			}
			break;
		case ePTGrab:
			{
				pOwner->m_bHaveStopped = !pOwner->m_bIsGrabbing;// 标记连续采集开始
				
				while (pOwner->m_bIsGrabbing)					// 是否允许连续采集
				{
					PTStatus status = pOwner->AcquireImage();	// 采集获取一幅图像
					if (pOwner->m_bSendCallbackEnable)
					{
						pOwner->SendCallback(status);			// 调用SendCallback(status)函数
					}
					Sleep(2);
				}
				
				pOwner->m_bHaveStopped = true;					// 标记连续采集已经停止了
				ResetEvent(pOwner->m_hEventToRun);				// 允许继续进行采集
				
			}
			break;
		}	
	}
	
	//发送"采集显示线程结束消息"
	SetEvent(pOwner->m_hEventExit);	
	
	return 0;
}

// 	监控采集回调函数
unsigned int scPTGrab2::MonitorAcquireImageThread(LPVOID lpParam)
{
	scPTGrab2 *pOwner = (scPTGrab2*)lpParam;
	if (!pOwner)
	{
		return 0;
	}

//	scTimer cTimer;
	double dTime = 0.0;
	bool bTimeOut = false;
	while(1)
	{
		WaitForSingleObject(pOwner->m_hEventMonitorBegin, INFINITE);
		if (pOwner->m_bMonitorExit)
		{
			break;
		}


		// 等待计时
//		cTimer.Reset();
		dTime = 0.0;
		bTimeOut = false;
		while (1)
		{
//			dTime = cTimer.GetTimeMilli(false);
			if (pOwner->m_bMonitorExit || pOwner->m_bMonitorEnd)
			{
				break;
			}

			if (dTime >= pOwner->m_lMonitorTimeout)
			{
				bTimeOut = true;
				break;
			}

			Sleep(3);
		}

		if (bTimeOut)
		{
			if (pOwner->IsGrabbing())
			{
				pOwner->Stop();
				pOwner->Start();
			}
		}

		Sleep(1);
	}

	SetEvent(pOwner->m_hEventMonitorExit);

	return 0;
}						

// 创建采集线程
// 只有当采集模式为eGrabThread时，才会创建采集线程
bool scPTGrab2::CreateGrabThread()
{	
	if (!m_bTerminate)
	{
		return false;
	}
	
	//创建采集线程
	m_bTerminate = false;
	m_hEventSnapped = CreateEvent(NULL,true,false,_T(""));
	m_hEventToRun = CreateEvent(NULL,true,false,_T(""));
	m_hEventExit = CreateEvent(NULL,true,false,_T(""));	
   	m_pWinThread = AfxBeginThread(GrabThread,this,0,0,0,NULL); 	
    if (NULL == m_pWinThread)
	{
		return false;
	}
	
	// 创建监控线程
	m_bMonitorExit = false;
	m_bMonitorEnd  = false;
	m_hEventMonitorBegin = CreateEvent(NULL, false, false, _T(""));
	m_hEventMonitorExit = CreateEvent(NULL, false, false, _T(""));
//	m_pMonitorThread = AfxBeginThread(MonitorAcquireImageThread, this, 0, 0, 0, NULL);
// 	if (NULL == m_pMonitorThread)
// 	{
// 		return false;
// 	}

	return true;
}

// 终止采集线程
// 只有当采集模式为eGrabThread时，才会终止采集线程
bool scPTGrab2::TerminateGrabThread()
{	
	if (m_bTerminate)
	{
		return true;
	}
	
	if (m_bIsGrabbing)
	{
		Freeze();
	}
	
	// 关闭监控线程
	m_bMonitorExit = true;
	m_bMonitorEnd = true;
	SetEvent(m_hEventMonitorBegin);
	if (WAIT_TIMEOUT == WaitForSingleObject(m_hEventMonitorExit, 1000))
	{
		Sleep(500);
	}

	if (m_pMonitorThread != NULL)
	{
		m_pMonitorThread = NULL;
	}

	CloseHandle(m_hEventMonitorBegin);
	m_hEventMonitorBegin = NULL;

	CloseHandle(m_hEventMonitorExit);
	m_hEventMonitorExit = NULL;

	//关闭采集线程
	m_bTerminate = true;
	SetEvent(m_hEventToRun);
	
	if (WAIT_TIMEOUT == WaitForSingleObject(m_hEventExit, 1000))
	{
		Sleep(500);
	}	
	
	if (m_pWinThread != NULL)
	{
		m_pWinThread = NULL;
	}	
	
	CloseHandle(m_hEventExit);
	m_hEventExit = NULL;

	CloseHandle(m_hEventSnapped);
	m_hEventSnapped = NULL;

	CloseHandle(m_hEventToRun);
	m_hEventToRun = NULL;
	
	return true;
}

// 从相机采集一幅图像，并进行相应的转换
// 采集图像返回两种标志结果：相机采集是否成功，图像采集是否成功
// 当从相机采集一幅图像成功时，并成功进行相应格式转换后，则返回ePTGrabOK，图像采集成功
// 当从相机采集一幅图像成功时，但未成功进行相应格式转换后，则返回ePTGrabFailed，图像采集失败
// 当从相机采集一幅图像超时时，不进行格式转换，则返回ePTGrabTimeout，图像采集失败
// 当从相机采集一幅图像出错时，如果是调用了Freeze,则根据原来图像采集状态返回ePTGrabOK或ePTGrabFailed；否则返回ePTGrabFailed，图像采集失败
PTStatus scPTGrab2::AcquireImage()
{
	EnterCriticalSection(&m_criticalSection);
	bool bAquireSuceess = m_bAcquireSuceess;
	m_bAcquireSuceess = false;
	
	Error error;
	
	m_bMonitorEnd = false;
	SetEvent(m_hEventMonitorBegin);
	// 从相机采集图像
	error = m_pCamera->RetrieveBuffer(&m_ImageRaw);
	if (error == PGRERROR_OK)
	{
		m_bMonitorEnd = true;
	
		m_bAcquireSuceess = TransferData();
		LeaveCriticalSection(&m_criticalSection);
		

		// 相机采集成功；图像获取是否成功由m_bAcquireSuceess决定
		return m_bAcquireSuceess ? ePTGrabOK : ePTGrabFailed;
	}
	else if(error == PGRERROR_TIMEOUT)
	{
		m_bMonitorEnd = true;

		m_bAcquireSuceess = false;
		LeaveCriticalSection(&m_criticalSection);
		
		// 相机采集超时也算成功；图像获取失败
		return ePTGrabTimeout;
	}
	else
	{
		if (m_bFreezeStop)
		{
			m_bAcquireSuceess = bAquireSuceess;
		}
		else
		{
			m_bAcquireSuceess = false;
		}
		
		m_bMonitorEnd = true;

		LeaveCriticalSection(&m_criticalSection);
			
		return m_bAcquireSuceess ? ePTGrabOK : ePTGrabFailed;
	}
}

// 重新扫描总线，当前相机连接会被中断
bool scPTGrab2::RescanBus()
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	Error error;
	error = pBusManager->RescanBus();
	if (error != PGRERROR_OK)
	{
		return false;
	}

	Sleep(100);

	return true;
}																

// 获取总线上相机的数量
bool scPTGrab2::GetBusCameraCount(unsigned int* pCamCount)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	Error error;
	error = pBusManager->GetNumOfCameras(pCamCount);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	else
	{
		return true;
	}
}	

// 获取总线上各相机的GUID值
// pGuid：必须分配内存；pSize:返回相机个数
bool scPTGrab2::EnumerateBusCameras(PGRGuid* pGuid, unsigned int* pSize)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	unsigned int uCameraNum;
	if (!GetBusCameraCount(&uCameraNum) || uCameraNum <= 0)
	{
		return false;
	}

	if (!pGuid || !pSize)
	{
		return false;
	}

	*pSize = uCameraNum;

	Error error;
	for (unsigned int i=0; i<uCameraNum; i++)
	{
		PGRGuid guid;
		error = pBusManager->GetCameraFromIndex(i, &guid);
		if (error != PGRERROR_OK)
		{
			pGuid[i] = guid;
		}
		else
		{
			pGuid[i] = guid;
		}
	}

	return true;
}	

// 获取总线上各相机的序列号
// pSerialInfo：必须分配内存；pSize:返回相机个数
bool scPTGrab2::EnumerateBusCameras(unsigned int* pSerialInfo, unsigned int* pSize)
{	
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}
	
	unsigned int uCameraNum;
	if (!GetBusCameraCount(&uCameraNum) || uCameraNum <= 0)
	{
		return false;
	}
	
	if (!pSerialInfo || !pSize)
	{
		return false;
	}
	
	*pSize = uCameraNum;

	Error error;
	for (unsigned int i=0; i<uCameraNum; i++)
	{
		unsigned int uSerialNum;
		PGRGuid guid;
		error = pBusManager->GetCameraSerialNumberFromIndex(i, &uSerialNum);
		if (error == PGRERROR_OK)
		{
			pSerialInfo[i] = uSerialNum;
		}
		else
		{
			pSerialInfo[i] = 0;
		}
	}
	
	return true;
}

// 获取总线上各相机的信息(序列号，相机类型等)
// pCameraInfo：必须分配内存；pSize:返回相机个数
bool scPTGrab2::EnumerateBusCameras(PTCameraInfo* pCameraInfo, unsigned int* pSize)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	unsigned int uCameraNum;
	PGRGuid guid;
	/*GigE*/
	Camera Camera;
	//Camera camera;

	if (!GetBusCameraCount(&uCameraNum) || uCameraNum <= 0)
	{
		return false;
	}
	
	if (!pCameraInfo || !pSize)
	{
		return false;
	}
	
	*pSize = uCameraNum;

	Error error;
	for (unsigned int i=0; i<uCameraNum; i++)
	{
		pCameraInfo[i].m_uiIndex = i;
		unsigned int uiSerialNum;
		error = pBusManager->GetCameraSerialNumberFromIndex(i, &uiSerialNum);
		if (error == PGRERROR_OK)
		{
			pBusManager->GetCameraFromSerialNumber(uiSerialNum, &guid);

			pCameraInfo[i].m_uiSerialNum = uiSerialNum;
			pCameraInfo[i].m_CameraType = GetCameraTypeFromSerialNum(uiSerialNum);
			
			// GigE Info
			if (eGigE == pCameraInfo[i].m_CameraType) // GigE Camera
			{
				if (Camera.Connect(&guid) == PGRERROR_OK)
				{
					CameraInfo tmpCamInfo;
					Camera.GetCameraInfo(&tmpCamInfo);
					strcpy(pCameraInfo[i].m_cModelName, tmpCamInfo.modelName);
					strcpy(pCameraInfo[i].m_cResolution, tmpCamInfo.sensorResolution);
				}
			//	GetCameraIPAddressByIndex(i, pCameraInfo[i].m_ipAddress, pCameraInfo[i].m_subnetMask, pCameraInfo[i].m_defaultGateway);
			}
			else
			{	
				if (Camera.Connect(&guid) == PGRERROR_OK)
				{
					CameraInfo tmpCamInfo;
					Camera.GetCameraInfo(&tmpCamInfo);
					strcpy(pCameraInfo[i].m_cModelName, tmpCamInfo.modelName);
					strcpy(pCameraInfo[i].m_cResolution,  tmpCamInfo.sensorResolution);
				}
			}
		}
		else
		{
			pCameraInfo[i].m_uiSerialNum = 0;
			pCameraInfo[i].m_CameraType = eInterfaceUnknow;
		}
		
	}
	
	return true;
}

// 根据索引获取相机的GUID
bool scPTGrab2::GetCameraGuidFromIndex(int iIndex, PGRGuid *pGuid)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	unsigned int uCameraNum;
	if (!GetBusCameraCount(&uCameraNum) || uCameraNum <= 0)
	{
		return false;
	}

	Error error;
	error = pBusManager->GetCameraFromIndex(iIndex, pGuid);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	else
	{
		return true;
	}

}

// 根据序列号获取相机的GUID
bool scPTGrab2::GetCameraGuidFromSerialNum(unsigned int uiSerialNum, PGRGuid *pGuid)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return false;
	}

	unsigned int uCameraNum;
	if (!GetBusCameraCount(&uCameraNum) || uCameraNum <= 0)
	{
		return false;
	}
	
	Error error;
	error = pBusManager->GetCameraFromSerialNumber(uiSerialNum, pGuid);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	else
	{
		return true;
	}

}

// 根据GUID获取相机的类型
PTCameraType scPTGrab2::GetCameraTypeFromGuid(PGRGuid *pGuid)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return eInterfaceUnknow;
	}

	PTCameraType cameraType(eInterfaceUnknow);
	Error error;
	InterfaceType interfaceType;
	error = pBusManager->GetInterfaceTypeFromGuid(pGuid, &interfaceType);
	if (error != PGRERROR_OK)
	{
		return eInterfaceUnknow;
	}
	
	if (INTERFACE_GIGE == interfaceType) // GigE Camera
	{
		cameraType = eGigE;
	}
	else if (INTERFACE_IEEE1394 == interfaceType) // 1394 Camera
	{
		cameraType = eIEEE1394;
	}
	else if (INTERFACE_USB2 == interfaceType) // USB2.0 Camera
	{
		cameraType = eUSB2;
	}
	else if (INTERFACE_USB3 == interfaceType) // USB3.0 Camera
	{
		cameraType = eUSB3;
	}

	return cameraType;
}

// 根据索引获取相机的类型
PTCameraType scPTGrab2::GetCameraTypeFromIndex(unsigned int uiIndex)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return eInterfaceUnknow;
	}

	PGRGuid guid;
	if (!GetCameraGuidFromIndex(uiIndex, &guid))
	{
		return eInterfaceUnknow;
	}
	
	PTCameraType cameraType(eInterfaceUnknow);
	Error error;	
	InterfaceType interfaceType;
	error = pBusManager->GetInterfaceTypeFromGuid(&guid, &interfaceType);
	if (INTERFACE_GIGE == interfaceType) // GigE Camera
	{
		cameraType = eGigE;
	}
	else if (INTERFACE_IEEE1394 == interfaceType) // 1394 Camera
	{
		cameraType = eIEEE1394;
	}
	else if (INTERFACE_USB2 == interfaceType) // USB2.0 Camera
	{
		cameraType = eUSB2;
	}
	else if (INTERFACE_USB3 == interfaceType) // USB3.0 Camera
	{
		cameraType = eUSB3;
	}
	
	return cameraType;
}

// 根据序列号获取相机的类型
PTCameraType scPTGrab2::GetCameraTypeFromSerialNum(unsigned int uiSerialNum)
{
	BusManager *pBusManager = GetBusManager();
	if (!pBusManager)
	{
		return eInterfaceUnknow;
	}

	PGRGuid guid;
	if (!GetCameraGuidFromSerialNum(uiSerialNum, &guid))
	{
		return eInterfaceUnknow;
	}
	
	PTCameraType cameraType(eInterfaceUnknow);
	Error error;	
	InterfaceType interfaceType;
	error = pBusManager->GetInterfaceTypeFromGuid(&guid, &interfaceType);
	if (INTERFACE_GIGE == interfaceType) // GigE Camera
	{
		cameraType = eGigE;
	}
	else if (INTERFACE_IEEE1394 == interfaceType) // 1394 Camera
	{
		cameraType = eIEEE1394;
	}
	else if (INTERFACE_USB2 == interfaceType) // USB2.0 Camera
	{
		cameraType = eUSB2;
	}
	else if (INTERFACE_USB3 == interfaceType) // USB3.0 Camera
	{
		cameraType = eUSB3;
	}

	return cameraType;
}

// 根据GUID设置GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::SetCameraIPAddressByGuid(PGRGuid *pGuid, IPAddress ipAddress, 
//								   IPAddress subnetMask, IPAddress defaultGateway)
//{
//	CameraInfo camInfo[128];
//	memset(camInfo, 0, 128*sizeof(CameraInfo));
//	unsigned int uiCameraCnt;
//	Error error;
//	uiCameraCnt = 128;
//	error = BusManager::DiscoverCameras(camInfo, &uiCameraCnt);
//	if (error != PGRERROR_OK || uiCameraCnt <= 0)
//	{
//		return false;
//	}
//
//	for (int j=0; j<uiCameraCnt; j++)
//	{
//		PGRGuid tmpGuid;
//		if (GetCameraGuidFromSerialNum(camInfo[j].serialNumber, &tmpGuid))
//		{
//			if (tmpGuid == *pGuid)
//			{
//				MACAddress macAddress;
//				macAddress = camInfo[j].macAddress;
//				error = BusManager::ForceIPAddressToCamera(macAddress, ipAddress, subnetMask, defaultGateway);
//				if (error == PGRERROR_OK)
//				{
//					return true;
//				}
//				else
//				{
//					return false;
//				}
//			}
//		}
//	}
//
//	return false;
//}

// 根据序列号设置GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::SetCameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress ipAddress, 
//											  IPAddress subnetMask, IPAddress defaultGateway)
//{
//	PGRGuid guid;
//	if (!GetCameraGuidFromSerialNum(uiSerialNum, &guid))
//	{
//		return false;
//	}
//
//	return SetCameraIPAddressByGuid(&guid, ipAddress, subnetMask, defaultGateway);
//}

// 根据索引设置GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::SetCameraIPAddressByIndex(unsigned int uiIndex, IPAddress ipAddress, 
//										  IPAddress subnetMask, IPAddress defaultGateway)
//{
//	PGRGuid guid;
//	if (!GetCameraGuidFromIndex(uiIndex, &guid))
//	{
//		return false;
//	}
//	
//	return SetCameraIPAddressByGuid(&guid, ipAddress, subnetMask, defaultGateway);	
//}

// 根据GUID获取GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::GetCameraIPAddressByGuid(PGRGuid *pGuid, IPAddress &ipAddress, 
//										 IPAddress &subnetMask, IPAddress &defaultGateway)
//{
//	CameraInfo camInfo[128];
//	memset(camInfo, 0, 128*sizeof(CameraInfo));
//	unsigned int uiCameraCnt;
//	Error error;
//	uiCameraCnt = 128;
//	error = BusManager::DiscoverCameras(camInfo, &uiCameraCnt);
//	if (error != PGRERROR_OK || uiCameraCnt <= 0)
//	{
//		return false;
//	}
//	
//	for (int j=0; j<uiCameraCnt; j++)
//	{
//		PGRGuid tmpGuid;
//		if (GetCameraGuidFromSerialNum(camInfo[j].serialNumber, &tmpGuid))
//		{
//			if (tmpGuid == *pGuid)
//			{
//				ipAddress = camInfo[j].ipAddress;
//				subnetMask = camInfo[j].subnetMask;
//				defaultGateway = camInfo[j].defaultGateway;
//
//				return true;
//			}
//		}
//		
//	}
//
//	return false;
//}

// 根据索引获取GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::GetCameraIPAddressByIndex(unsigned int uiIndex, IPAddress &ipAddress, 
//										  IPAddress &subnetMask, IPAddress &defaultGateway)
//{	
//	PGRGuid guid;
//	if (!GetCameraGuidFromIndex(uiIndex, &guid))
//	{
//		return false;
//	}
//	
//	return GetCameraIPAddressByGuid(&guid, ipAddress, subnetMask, defaultGateway);		
//}
//
//// 根据序列号获取GigE相机的IP、子网掩码和默认网关
//bool scPTGrab2::GetCameraIPAddressBySerialNum(unsigned int uiSerialNum, IPAddress &ipAddress, 
//											  IPAddress &subnetMask, IPAddress &defaultGateway)
//{
//	PGRGuid guid;
//	if (!GetCameraGuidFromSerialNum(uiSerialNum, &guid))
//	{
//		return false;
//	}
//	
//	return GetCameraIPAddressByGuid(&guid, ipAddress, subnetMask, defaultGateway);			
//}


// 重新设置总线上Camera相机IP
//bool scPTGrab2::AutoForceAllCamerasIP()
//{
//	BusManager *pBusManager = GetBusManager();
//	if (!pBusManager)
//	{
//		return false;
//	}
//
//	Error error;
//
//	// 强制设置IP
//	error = pBusManager->ForceAllIPAddressesAutomatically();
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	// 设置完成后，等待5s
//	Sleep(5000);
//
//	// 重新扫描总线
//	error = pBusManager->RescanBus();
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	return true;
//}
//
//// 查询Format7模式信息
//bool scPTGrab2::QueryFormat7Info(PTFormat7QueryInfo &format7QueryInfo)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
//	{
//		return false;
//	}	
//	
//	if (/*eGigE*/ eUSB2== m_cameraType) // GigE Camera
//	{
//		Camera *pCamera = (Camera*)m_pCamera;
//		
//		Mode mode;
//		Error error;
//		error = pCamera->GetGigEImagingMode(&mode);
//		if (error != PGRERROR_OK)
//		{
//			return false;
//		}
//
//		if (format7QueryInfo.m_mode != mode)
//		{
//			return false;
//		}
//
//		GigEImageSettingsInfo imageSettingsInfo;
//		error = pCamera->GetGigEImageSettingsInfo(&imageSettingsInfo);
//		if (error != PGRERROR_OK)
//		{
//			return false;
//		}
//		
//		format7QueryInfo.m_uiUnitStartX = imageSettingsInfo.offsetHStepSize;
//		format7QueryInfo.m_uiUnitStartY = imageSettingsInfo.offsetVStepSize;
//		format7QueryInfo.m_uiUnitWidth = imageSettingsInfo.imageHStepSize;
//		format7QueryInfo.m_uiUnitHeight = imageSettingsInfo.imageVStepSize;
//		format7QueryInfo.m_uiMaxWidth = imageSettingsInfo.maxWidth;
//		format7QueryInfo.m_uiMaxHeight = imageSettingsInfo.maxHeight;
//		format7QueryInfo.m_uiPixelFormatSupported = imageSettingsInfo.pixelFormatBitField;	
//	}
//	else // IEEE1394或USB
//	{
//		Camera *pCamera = (Camera*)m_pCamera;	
//		
//		Error error;
//		Format7Info format7Info;
//		format7Info.mode = format7QueryInfo.m_mode;
//		bool bSupported = false;
//		error = pCamera->GetFormat7Info(&format7Info, &bSupported);
//		if (error != PGRERROR_OK || !bSupported)
//		{
//			return false;
//		}
//		
//		format7QueryInfo.m_uiUnitStartX = format7Info.offsetHStepSize;
//		format7QueryInfo.m_uiUnitStartY = format7Info.offsetVStepSize;
//		format7QueryInfo.m_uiUnitWidth = format7Info.imageHStepSize;
//		format7QueryInfo.m_uiUnitHeight = format7Info.imageVStepSize;
//		format7QueryInfo.m_uiMaxWidth = format7Info.maxWidth;
//		format7QueryInfo.m_uiMaxHeight = format7Info.maxHeight;
//		format7QueryInfo.m_uiPixelFormatSupported = format7Info.pixelFormatBitField;	
//	}
//	
//	return true;
//}								
//	
// 获取Format7参数
bool scPTGrab2::GetFormat7Config(PTFormat7Info &format7Info)
{
	if (!IsOnline())
	{
		return false;
	}
	
	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
	{
		return false;
	}	
	
	//if (eGigE == m_cameraType) // GigE Camera
	//{
	//	Camera *pCamera = (Camera*)m_pCamera;
	//	
	//	Mode mode;
	//	Error error;
	//	error = pCamera->GetGigEImagingMode(&mode);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}
	//	format7Info.m_mode = mode;

	//	GigEImageSettings imageSettings;
	//	error = pCamera->GetGigEImageSettings( &imageSettings);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}
	//	
	//	format7Info.m_uiStartX = imageSettings.offsetX;
	//	format7Info.m_uiStartY = imageSettings.offsetY;
	//	format7Info.m_uiWidth = imageSettings.width;
	//	format7Info.m_uiHeight = imageSettings.height;
	//	format7Info.m_pixelFormat = imageSettings.pixelFormat;	
	//}
	else // IEEE1394或USB
	{
		Camera *pCamera = (Camera*)m_pCamera;	
		
		Error error;
		Format7ImageSettings formatSettings;
		unsigned int uiPacketSize;
		float fBusSpeed;
		error = pCamera->GetFormat7Configuration(&formatSettings, &uiPacketSize, &fBusSpeed);
		if (error != PGRERROR_OK)
		{
			return false;
		}

		format7Info.m_fBusSpeed = fBusSpeed;
		format7Info.m_uiStartX = formatSettings.offsetX;
		format7Info.m_uiStartY = formatSettings.offsetY;
		format7Info.m_uiWidth = formatSettings.width;
		format7Info.m_uiHeight = formatSettings.height;
		format7Info.m_mode = formatSettings.mode;
		format7Info.m_pixelFormat = formatSettings.pixelFormat;
	}
	
	return true;
}

// 设置Format7参数
bool scPTGrab2::SetFormat7Config(PTFormat7Info format7Info)
{
	if (!IsOnline())
	{
		return false;
	}
	
	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
	{
		return false;
	}	
	
	//if (eGigE == m_cameraType) // GigE Camera
	//{
	//	Camera *pCamera = (Camera*)m_pCamera;
	//	
	//	Error error;
	//	Mode mode = format7Info.m_mode;
	//	bool bSupported;
	//	error = pCamera->QueryGigEImagingMode(mode, &bSupported);
	//	if (error != PGRERROR_OK || !bSupported)
	//	{
	//		return false;
	//	}

	//	error = pCamera->SetGigEImagingMode(mode);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}

	//	GigEImageSettings imageSettings;
	//	imageSettings.offsetX = format7Info.m_uiStartX;
	//	imageSettings.offsetY = format7Info.m_uiStartY;
	//	imageSettings.width = format7Info.m_uiWidth;
	//	imageSettings.height = format7Info.m_uiHeight;
	//	imageSettings.pixelFormat = format7Info.m_pixelFormat;
	//	
	//	error = pCamera->SetGigEImageSettings( &imageSettings );
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}
	//}
	else // 1394/USB
	{
		Camera *pCamera = (Camera*)m_pCamera;	
		
		Error error;	
		Format7ImageSettings formatSettings;	
		formatSettings.offsetX = format7Info.m_uiStartX;
		formatSettings.offsetY = format7Info.m_uiStartY;
		formatSettings.width = format7Info.m_uiWidth;
		formatSettings.height = format7Info.m_uiHeight;
		formatSettings.pixelFormat = format7Info.m_pixelFormat;
		formatSettings.mode = format7Info.m_mode;
		
		error = pCamera->SetFormat7Configuration(&formatSettings, format7Info.m_fBusSpeed);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
	}
	
	return true;
}

// Format7模式信息是否合理
bool scPTGrab2::IsFormat7InfoSupported(PTFormat7Info &format7Info, PTFormat7InfoParaFilter filter)
{
	if (!IsOnline())
	{
		return false;
	}
	
	if (eInterfaceUnknow == m_cameraType) // Unknown Camera
	{
		return false;
	}	
	
	// Format7ImageSettings
	//if (eGigE == m_cameraType) // GigE Camera
	//{
	//	Camera *pCamera = (Camera*)m_pCamera;
	//	
	//	Error error;
	//	GigEImageSettingsInfo imageSettingsInfo;
	//	GigEImageSettings imageSettings;
	//	Mode curMode;

	//	// 获取查询信息
	//	error = pCamera->GetGigEImageSettingsInfo(&imageSettingsInfo);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}
	//
	//	// 获取当前信息
	//	error = pCamera->GetGigEImageSettings(&imageSettings);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}
	//	error = pCamera->GetGigEImagingMode(&curMode);
	//	if (error != PGRERROR_OK)
	//	{
	//		return false;
	//	}

	//	// 开始判断
	//	if (filter == eFmt7ParaAll)
	//	{
	//		if (format7Info.m_mode != curMode)
	//		{
	//			return false;
	//		}

	//		if (format7Info.m_uiStartX < 0 || format7Info.m_uiStartY < 0
	//			|| format7Info.m_uiWidth <= 0 || format7Info.m_uiHeight <= 0
	//			|| (format7Info.m_uiStartX+format7Info.m_uiWidth) > imageSettingsInfo.maxWidth
	//			|| (format7Info.m_uiStartY+format7Info.m_uiHeight) > imageSettingsInfo.maxHeight
	//			|| (format7Info.m_uiStartX%imageSettingsInfo.offsetHStepSize) != 0  
	//			|| (format7Info.m_uiStartY%imageSettingsInfo.offsetVStepSize) != 0
	//			|| (format7Info.m_uiWidth%imageSettingsInfo.imageHStepSize) != 0
	//			|| (format7Info.m_uiHeight%imageSettingsInfo.imageVStepSize) != 0
	//			|| (format7Info.m_fBusSpeed < 0) || (format7Info.m_fBusSpeed > 1))
	//		{
	//			return false;
	//		}

	//		if ((format7Info.m_pixelFormat&imageSettingsInfo.pixelFormatBitField) == 0)
	//		{
	//			return false;
	//		}

	//		return true;
	//	}
	//	if (filter == eFmt7Mode)
	//	{
	//		if (format7Info.m_mode != curMode)
	//		{
	//			return false;
	//		}
	//		
	//		return true;
	//	}
	//	if (filter == eFmt7StartX)
	//	{
	//		if (format7Info.m_uiStartX < 0 || (format7Info.m_uiStartX%imageSettingsInfo.offsetHStepSize) != 0
	//			|| (format7Info.m_uiStartX+imageSettings.width)>imageSettingsInfo.maxWidth)
	//		{
	//			return false;
	//		}
	//		
	//		return true;
	//	}
	//	if (filter == eFmt7StartY)
	//	{
	//		if (format7Info.m_uiStartY < 0 || (format7Info.m_uiStartY%imageSettingsInfo.offsetVStepSize) != 0
	//			|| (format7Info.m_uiStartY+imageSettings.height)>imageSettingsInfo.maxHeight)
	//		{
	//			return false;
	//		}
	//		
	//		return true;
	//	}
	//	if (filter == eFmt7Width)
	//	{
	//		if (format7Info.m_uiWidth <= 0 || (format7Info.m_uiWidth%imageSettingsInfo.imageHStepSize) != 0
	//			|| (imageSettings.offsetX+format7Info.m_uiWidth)>imageSettingsInfo.maxWidth)
	//		{
	//			return false;
	//		}
	//			
	//		return true;
	//	}
	//	if (filter == eFmt7Height)
	//	{
	//		if (format7Info.m_uiHeight <= 0 || (format7Info.m_uiHeight%imageSettingsInfo.imageVStepSize) != 0
	//			|| (imageSettings.offsetY+format7Info.m_uiHeight)>imageSettingsInfo.maxHeight)
	//		{
	//			return false;
	//		}
	//		
	//		return true;
	//	}
	//	if (filter == eFmt7PixelFormat)
	//	{
	//		if ((format7Info.m_pixelFormat&imageSettingsInfo.pixelFormatBitField) == 0)
	//		{
	//			return false;
	//		}

	//		return true;
	//	}
	//	if (filter == eFmt7BusSpeed)
	//	{
	//		if (format7Info.m_fBusSpeed < 0 || format7Info.m_fBusSpeed > 1)
	//		{
	//			return false;
	//		}
	//		
	//		return true;
	//	}
	//}
	else // 1394/USB
	{
		Camera *pCamera = (Camera*)m_pCamera;	

		Error error;
		Format7ImageSettings formatSettings;
		unsigned int uiPacketSize;
		float fBusSpeed;
		Format7Info format7QueryInfo;
		bool bSupported = false;
		bool bSettingsValid = false;
		Format7PacketInfo packetInfo;
		Format7ImageSettings testformat7Settings;

		// 获取当前Format7属性
		error = pCamera->GetFormat7Configuration(&formatSettings, &uiPacketSize, &fBusSpeed);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		testformat7Settings = formatSettings;

		// 获取查询信息
		format7QueryInfo.mode = formatSettings.mode;
		error = pCamera->GetFormat7Info(&format7QueryInfo, &bSupported);
		if (error != PGRERROR_OK || !bSupported)
		{
			return false;
		}

		// 开始判断
		if (filter == eFmt7ParaAll)
		{
			testformat7Settings.mode = format7Info.m_mode;
			testformat7Settings.offsetX = format7Info.m_uiStartX;
			testformat7Settings.offsetY = format7Info.m_uiStartY;
			testformat7Settings.width = format7Info.m_uiWidth;
			testformat7Settings.height = format7Info.m_uiHeight;
			testformat7Settings.pixelFormat = format7Info.m_pixelFormat;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}

			if (format7Info.m_fBusSpeed < 0 || format7Info.m_fBusSpeed > 1)
			{
				return false;
			}

			return true;
		}
		if (filter == eFmt7Mode)
		{
			testformat7Settings.mode = format7Info.m_mode;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}
			
			return true;
		}
		if (filter == eFmt7StartX)
		{
			testformat7Settings.offsetX = format7Info.m_uiStartX;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}

			return true;
		}
		if (filter == eFmt7StartY)
		{
			testformat7Settings.offsetY = format7Info.m_uiStartY;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}
			
			return true;
		}
		if (filter == eFmt7Width)
		{
			testformat7Settings.width = format7Info.m_uiWidth;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}
			
			return true;
		}
		if (filter == eFmt7Height)
		{
			testformat7Settings.height = format7Info.m_uiHeight;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}
			
			return true;
		}
		if (filter == eFmt7PixelFormat)
		{
			testformat7Settings.pixelFormat = format7Info.m_pixelFormat;
			error = pCamera->ValidateFormat7Settings(&testformat7Settings ,&bSettingsValid, &packetInfo);
			if (error != PGRERROR_OK || !bSettingsValid)
			{
				return false;
			}
			
			return true;
		}
		if (filter == eFmt7BusSpeed)
		{
			if (format7Info.m_fBusSpeed < 0 || format7Info.m_fBusSpeed > 1)
			{
				return false;
			}

			return true;
		}
	}

	return false;
}							


// 采集单帧图像；
// 该采集函数不能与他采集函数同时调用；
// 该采集函数直到采集完成后退出；
bool scPTGrab2::SnapImage()
{
	if (!IsOnline())
	{
		return false;
	}

	m_pCamera->SetCallback(NULL, NULL);	 // 需先将回调函数设为空

	m_pCamera->StartCapture(NULL, NULL);
	
	PTStatus status = AcquireImage();
	
	if (m_bSendCallbackEnable)
	{
		SendCallback(status);
	}
	
	m_pCamera->StopCapture();
	
	return IsAcquireSuccess();
}

// 利用软触发方式，采集单帧图像
// 发送完软触发信号后，直到采集单帧图像完成后，该函数才返回
// 调用该函数，需要将相机设置为软触发模式
// 该函数不依赖于采集线程
bool scPTGrab2::TriggerSnap()
{
	if (!IsOnline())
	{
		return false;
	}
	
	m_pCamera->StopCapture();

	m_pCamera->SetCallback(NULL, NULL); // 需先将函数设为空

	m_pCamera->StartCapture(NULL, NULL);
	
	if (!SendSoftTrigger())
	{
		return false;
	}
	
	PTStatus status = AcquireImage();
	
	if (m_bSendCallbackEnable)
	{
		SendCallback(status);
	}
	
	m_pCamera->StopCapture();
	
	return IsAcquireSuccess();
}

// 图像是否采集成功
bool scPTGrab2::IsAcquireSuccess()
{
	return m_bAcquireSuceess;
}

//设置状态，设置brightness、exposure、shutter、gain、framerate为手动状态
bool scPTGrab2::SetStatusManual()
{
	if (!IsOnline())
	{
		return false;
	}

	Error error;
	
	unsigned int uiValue = 0XC2000000;	
	
	//设置Brightness为Abs_Value手动方式
	error = m_pCamera->WriteRegister(0X800, uiValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	//设置exposure为Abs_Value手动方式
	error = m_pCamera->WriteRegister(0X804,uiValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	//设置shutter时间为Abs_Value手动方式	
	error = m_pCamera->WriteRegister(0X81C,uiValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	//设置gain为Abs_Value手动方式
	error = m_pCamera->WriteRegister(0X820,uiValue);	
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}                      

// 设置GigE相机PersistentIP地址
//bool scPTGrab2::SetGigEPersistentIPAddress(IPAddress ipAddress, IPAddress subnetMask, 
//								IPAddress defaultGateway)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Camera *pCamera = (Camera*)m_pCamera;
//	Error error;
//
//	unsigned int uiIPAddress = 0;
//	unsigned int uiSubnetMask = 0;
//	unsigned int uiDefaultGateway = 0;
//	
//	// 设置IP地址
//	uiIPAddress =  (ipAddress.octets[0]<<24)+ (ipAddress.octets[1]<<16) + (ipAddress.octets[2]<<8) + (ipAddress.octets[3]);
//	error = pCamera->WriteGVCPRegister(0x64c, uiIPAddress);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	// 设置子网掩码
//	uiSubnetMask = (subnetMask.octets[0]<<24)+ (subnetMask.octets[1]<<16) + (subnetMask.octets[2]<<8) + (subnetMask.octets[3]);
//	error = pCamera->WriteGVCPRegister(0x65c, uiSubnetMask);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//
//	// 设置默认网关
//	uiDefaultGateway = (defaultGateway.octets[0]<<24)+ (defaultGateway.octets[1]<<16) + (defaultGateway.octets[2]<<8) + (defaultGateway.octets[3]);
//	error = pCamera->WriteGVCPRegister(0x66c, uiDefaultGateway);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//
//// 获取GigE相机PersistentIP地址
//bool scPTGrab2::GetGigEPersistentIPAddress(IPAddress &ipAddress, IPAddress &subnetMask, 
//								IPAddress &defaultGateway)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Camera *pCamera = (Camera*)m_pCamera;
//	Error error;
//	
//	unsigned int uiIPAddress = 0;
//	unsigned int uiSubnetMask = 0;
//	unsigned int uiDefaultGateway = 0;
//
//	// 获取IP地址
//	error = pCamera->ReadGVCPRegister(0x64c, &uiIPAddress);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	ipAddress = IPAddress(uiIPAddress);
//
//	// 获取子网掩码
//	error = pCamera->ReadGVCPRegister(0x65c, &uiSubnetMask);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	subnetMask = IPAddress(uiSubnetMask);
//
//	// 获取默认网关
//	error = pCamera->ReadGVCPRegister(0x66c, &uiDefaultGateway);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	defaultGateway = IPAddress(uiDefaultGateway);
//
//	return true;
//}
//
//// 设置PacketSize
//bool scPTGrab2::SetCurGigEPacketSize(unsigned int uiPacketSize)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	CameraProperty.propType = PACKET_SIZE;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	CameraProperty.value = uiPacketSize;
//	error = pCamera->SetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	return true;
//}
//	
//// 获取当前PacketSize								
//bool scPTGrab2::GetCurGigEPacketSize(unsigned int* piPacketSize)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	CameraProperty.propType = PACKET_SIZE;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	*piPacketSize = CameraProperty.value;
//
//	return true;
//}	
//
//// 获取PacketSize范围								
//bool scPTGrab2::GetGigEPacketSizeRange(unsigned int *piMinPacketSize, unsigned int *piMaxPacketSize)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	unsigned int uiRealMaxSize = 0;	 // 实际Size
//
//	CameraProperty.propType = PACKET_SIZE;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	error = pCamera->DiscoverGigEPacketSize(&uiRealMaxSize);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//
//	*piMinPacketSize = CameraProperty.min;
//
//	*piMaxPacketSize = min(CameraProperty.max, uiRealMaxSize);
//
//	return true;
//}
//
//// 设置PacketDelay
//bool scPTGrab2::SetCurGigEPacketDelay(unsigned int uiPacketDelay)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	
//	CameraProperty.propType = PACKET_DELAY;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	CameraProperty.value = uiPacketDelay;
//	error = pCamera->SetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//// 获取当前PacketDelay								
//bool scPTGrab2::GetCurGigEPacketDelay(unsigned int* piPacketDelay)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	
//	CameraProperty.propType = PACKET_DELAY;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	*piPacketDelay = CameraProperty.value;
//	
//	return true;
//}
//
//// 获取PacketDelay范围								
//bool scPTGrab2::GetGigEPacketDelayRange(unsigned int *piMinPacketDelay, unsigned int *piMaxPacketDelay)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	
//	CameraProperty.propType = PACKET_DELAY;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	
//	*piMinPacketDelay = CameraProperty.min;
//	
//	*piMaxPacketDelay = CameraProperty.max;
//
//	return true;
//}
//
//// 设置HeartBeatEnable
//bool scPTGrab2::SetCurGigEHeartBeatEnable(unsigned int uiHeartBeatEnable)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	
//	CameraProperty.propType = HEARTBEAT;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	CameraProperty.value = uiHeartBeatEnable;
//	error = pCamera->SetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	return true;
//}
//
//// 获取当前HeartBeatEnable						
//bool scPTGrab2::GetCurGigEHeartBeatEnable(unsigned int* piHeartBeatEnable)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//	
//	CameraProperty.propType = HEARTBEAT;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	*piHeartBeatEnable = CameraProperty.value;
//	
//	return true;
//}
//
//						
//// 设置HeartBeatTimeOut
//bool scPTGrab2::SetCurGigEHeartBeatTimeOut(unsigned int uiHeartBeatTimeOut)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	CameraProperty.propType = HEARTBEAT_TIMEOUT;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	CameraProperty.value = uiHeartBeatTimeOut;
//	error = pCamera->SetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	return true;
//}	
//
//// 获取当前HeartBeatTimeOut					
//bool scPTGrab2::GetCurGigEHeartBeatTimeOut(unsigned int* piHeartBeatTimeOut)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	CameraProperty.propType = HEARTBEAT_TIMEOUT;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	*piHeartBeatTimeOut = CameraProperty.value;
//	
//	return true;
//}	
//
//// 获取HeartBeatTimeOut范围	
//bool scPTGrab2::GetGigEHeartBeatTimeOutRange(unsigned int *piMinHeartBeatTimeOut, 
//								  unsigned int *piMaxHeartBeatTimeOut)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	GigEProperty CameraProperty;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	CameraProperty.propType = HEARTBEAT_TIMEOUT;
//	error = pCamera->GetGigEProperty(&CameraProperty);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	*piMinHeartBeatTimeOut = CameraProperty.min;
//	
//	*piMaxHeartBeatTimeOut = CameraProperty.max;
//
//	return true;
//}
//
//// 设置GigEIP配置方式
//bool scPTGrab2::SetGigEIPConfigurationType(BOOL bPersistent, BOOL bDHCP)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	unsigned int uiValue = 4;
//	if (bPersistent)
//	{
//		uiValue += 1;
//	}
//	if (bDHCP)
//	{
//		uiValue += 2;
//	}
//
//	error = pCamera->WriteGVCPRegister(0x014, uiValue);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//
//	return true;
//}	
//
//// 获取GigEIP配置方式
//bool scPTGrab2::GetGigEIPConfigurationType(BOOL &bPersistent, BOOL &bDHCP)
//{
//	if (!IsOnline())
//	{
//		return false;
//	}
//	
//	if (m_cameraType != eGigE)
//	{
//		return false;
//	}
//
//	Error error;
//	Camera *pCamera = (Camera*)m_pCamera;
//
//	unsigned int uiValue = 0;
//	error = pCamera->ReadGVCPRegister(0x014, &uiValue);
//	if (error != PGRERROR_OK)
//	{
//		return false;
//	}
//	
//	bPersistent = uiValue & 0x01;
//	bDHCP = (uiValue>>1) & 0x01;
//
//	return true;
//}							
// 获取BusManager对象
BusManager* scPTGrab2::GetBusManager()
{
	if (m_AutoBusManager.get() == NULL)
	{
		m_AutoBusManager = std::auto_ptr<PTBusMangerInfo>(new PTBusMangerInfo);
	}

	return  m_AutoBusManager.get()->m_pBusManger;	
}	

bool scPTGrab2::Restart()
{
	if (!m_bOnline || !m_pCamera || !m_pSerialNum)
	{
		return false;
	}
	
	Error error;

	PGRGuid guid;
	if (!GetCameraGuidFromSerialNum(*m_pSerialNum, &guid))
	{
		return false;
	}

	// 连接
	error = m_pCamera->Connect(&guid);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	else
	{
		bool bIsGrabbing = m_bIsGrabbing;
		if (bIsGrabbing)
		{
			Freeze();
		}

		// 关闭触发
		CloseTriggerMode();
		
		// 试采集图像一幅图像确定图像尺寸，并分配内存
		EnterCriticalSection(&m_criticalSection);
		// 开始采集
		Error error;
		error = m_pCamera->StartCapture(NULL, NULL);
		if (error != PGRERROR_OK)
		{
			LeaveCriticalSection(&m_criticalSection);
						
			return false;
		}
		// 获取图像
		if (ePTGrabOK != AcquireImage())
		{
			LeaveCriticalSection(&m_criticalSection);
			
			return false;
		}
		// 停止采集
		m_pCamera->StopCapture();
		
		LeaveCriticalSection(&m_criticalSection);

		if (bIsGrabbing)
		{
			Grab();
		}
	}

	return true;
}
										
// 总线相机在线回调
void scPTGrab2::InternalBusCameraArrivalCallback(void* pParameter, unsigned int serialNumber)
{
	PTBusMangerInfo *pBusMangerInfo = (PTBusMangerInfo*)pParameter;

	EnterCriticalSection(&pBusMangerInfo->m_csBusCallback);

	std::map<unsigned int, bool>::iterator iter;
	iter = pBusMangerInfo->m_BusCamaraOnlineInfo.find(serialNumber);
	if (iter != pBusMangerInfo->m_BusCamaraOnlineInfo.end())
	{
		iter->second = true;
	}
	else
	{
		pBusMangerInfo->m_BusCamaraOnlineInfo.insert(std::pair<unsigned int, bool>(serialNumber, true));
	}

	// 调用回调函数
	if (pBusMangerInfo->m_pUserCameraArrivalCallback)
	{
		pBusMangerInfo->m_pUserCameraArrivalCallback(pBusMangerInfo->m_pUserCameraArrivalCallbackPara, serialNumber);
	}

	LeaveCriticalSection(&pBusMangerInfo->m_csBusCallback);
}	

// 总线相机掉线回调
void scPTGrab2::InternalBusCameraRemovalCallback(void* pParameter, unsigned int serialNumber)
{
	PTBusMangerInfo *pBusMangerInfo = (PTBusMangerInfo*)pParameter;

	EnterCriticalSection(&pBusMangerInfo->m_csBusCallback);
	std::map<unsigned int, bool>::iterator iter;
	iter = pBusMangerInfo->m_BusCamaraOnlineInfo.find(serialNumber);
	if (iter != pBusMangerInfo->m_BusCamaraOnlineInfo.end())
	{
		iter->second = false;
	}
	else
	{
		pBusMangerInfo->m_BusCamaraOnlineInfo.insert(std::pair<unsigned int, bool>(serialNumber, false));
	}

	// 调用回调函数
	if (pBusMangerInfo->m_pUserCameraRemovalCallback)
	{
		pBusMangerInfo->m_pUserCameraRemovalCallback(pBusMangerInfo->m_pUserCameraRemovalCallbackPara, serialNumber);
	}

	LeaveCriticalSection(&pBusMangerInfo->m_csBusCallback);
}	

// 显式删除BusManager对象，在DLL中使用
bool scPTGrab2::DeleteBusManager()
{
	if (m_AutoBusManager.get() != NULL)
	{
		delete m_AutoBusManager.release();
	}

	return true;
}														
	
// 设置总线相机在线回调
bool scPTGrab2::SetUserBusCameraArrivalCallback(void* pParameter, CameraStatusCallback pCallback) 
{
	if (!GetBusManager())
	{
		return false;
	}

	m_AutoBusManager.get()->SetCameraArrivalCallback(pParameter, pCallback);

	return true;
}	

// 设置总线相机掉线回调
bool scPTGrab2::SetUserBusCameraRemovalCallback(void* pParameter, CameraStatusCallback pCallback)
{
	if (!GetBusManager())
	{
		return false;
	}
	
	m_AutoBusManager.get()->SetCameraRemovalCallback(pParameter, pCallback);
	
	return true;
}
	
tagPTBusMangerInfo::tagPTBusMangerInfo()
{
	m_pBusManger = new BusManager;
	InitializeCriticalSection(&m_csBusCallback);
	m_pArrivalHandle  = NULL;
	m_pRemovalHandle  = NULL;
	m_pUserCameraArrivalCallback = NULL;
	m_pUserCameraRemovalCallback = NULL;
	m_pUserCameraArrivalCallbackPara = NULL;
	m_pUserCameraRemovalCallbackPara = NULL;
	m_pBusManger->RegisterCallback(scPTGrab2::InternalBusCameraArrivalCallback, ARRIVAL, this, &m_pArrivalHandle);
	m_pBusManger->RegisterCallback(scPTGrab2::InternalBusCameraRemovalCallback, REMOVAL, this, &m_pRemovalHandle);
}

tagPTBusMangerInfo::~tagPTBusMangerInfo()
{
	if (m_pArrivalHandle)
	{
		m_pBusManger->UnregisterCallback(m_pArrivalHandle);
		m_pArrivalHandle = NULL;
	}
	
	if (m_pRemovalHandle)
	{
		m_pBusManger->UnregisterCallback(m_pRemovalHandle);
		m_pRemovalHandle = NULL;
	}
	DeleteCriticalSection(&m_csBusCallback);
	delete m_pBusManger;
	m_pBusManger = NULL;
	m_BusCamaraOnlineInfo.clear();
	m_pUserCameraArrivalCallback = NULL;
	m_pUserCameraRemovalCallback = NULL;
	m_pUserCameraArrivalCallbackPara = NULL;
	m_pUserCameraRemovalCallbackPara = NULL;
}

void tagPTBusMangerInfo::SetCameraArrivalCallback(void* pPara, CameraStatusCallback pCallback)
{
	if (!pCallback)
	{
		return; 
	}
	
	EnterCriticalSection(&m_csBusCallback);
	m_pUserCameraArrivalCallback = pCallback;
	m_pUserCameraArrivalCallbackPara = pPara;
	LeaveCriticalSection(&m_csBusCallback);
}

void tagPTBusMangerInfo::SetCameraRemovalCallback(void* pPara, CameraStatusCallback pCallback)
{
	if (!pCallback)
	{
		return; 
	}
	
	EnterCriticalSection(&m_csBusCallback);
	m_pUserCameraRemovalCallback = pCallback;
	m_pUserCameraRemovalCallbackPara = pPara;
	LeaveCriticalSection(&m_csBusCallback);	
}
	
// 设置GPIO模式,iPin:0,1,2,3
bool scPTGrab2::SetGPIOMode(int iPin, PTGMMode mode)
{
	if (!IsOnline())
	{
		return false;
	}

	Error   error;
	unsigned int ulValue = 0;
	
	if (iPin < 0 || iPin >=4)
	{
		return false;
	}
	
	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	if (!(ulValue & 0x80000000)) // 不支持
	{
		return false; 
	}

	int iModeValue = mode;
	ulValue = 0x80000000 | (iModeValue<<16);
	error = m_pCamera->WriteRegister(0x1110 + iPin*0x10, ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	return true;
}

// 获取GPIO模式,iPin:0,1,2,3
PTGMMode scPTGrab2::GetGPIOMode(int iPin)
{
	if (!IsOnline())
	{
		return ePTGMUnknown;
	}

	Error   error;
	unsigned int ulValue = 0;
	int iMode = -1;

	if (iPin < 0 || iPin >=4)
	{
		return ePTGMUnknown;
	}

	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return ePTGMUnknown;
	}
	
	if (!(ulValue & 0x80000000)) // 不支持
	{
		return ePTGMUnknown; 
	}
	
	iMode = (ulValue & 0x000F0000) >> 16;
	switch(iMode)
	{
	case 0:
		return ePTGMInput;
	case 1:
		return ePTGMOutput;
	case 2:
		return ePTGMTrigger;
	case 3:
		return ePTGMStrobe;
	case 4:
		return ePTGMPWM;
	case 8:
		return ePTGMDCAM;
	default:
		return ePTGMUnknown;
	}

	return ePTGMUnknown;
}        

// input模式下，获取输入值
bool scPTGrab2::GetGPIOInput(int iPin, int &iValue)
{
	if (!IsOnline())
	{
		return false;
	}

	Error   error;
	unsigned int ulValue = 0;

	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMInput)
	{
		return false;
	}

	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	iValue = ulValue & 0x00000001;

	return true;
}

// output模式下，设置输出值
bool scPTGrab2::SetGPIOOutput(int iPin, int iValue)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMOutput && mode != ePTGMDCAM)
	{
		return false;
	}

	if (mode == ePTGMOutput)
	{
		error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
		if (iValue == 0)
		{
			ulValue &= 0xFFFFFFFE;
		}
		else
		{
			ulValue |= 0x00000001;
		}
		error = m_pCamera->WriteRegister(0x1110 + iPin*0x10, ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
	}
	else // ePTGMDCAM
	{
		error = m_pCamera->ReadRegister(0x11F0, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}

		if (iValue == 0)
		{
			ulValue &= ~(1<<(31-iPin));
		}
		else
		{
			ulValue |= 1<<(31-iPin);
		}

		error = m_pCamera->WriteRegister(0x11F0, ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
	}

	return true;
}

// Trigger模式下，设置触发极性，bPolarity:0,下降沿;1:上升沿
bool scPTGrab2::SetGPIOTriggerPolarity(int iPin, bool bPolarity)
{
	iPin = 0;
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMTrigger)
	{
		return false;
	}

	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	ulValue = 0x80020000;
	if (bPolarity)
	{
		ulValue |= 0x00000001;
	}
	error = m_pCamera->WriteRegister(0x1110 + iPin*0x10, ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}

// Trigger模式下，获取触发极性，bPolarity:0,下降沿;1:上升沿
bool scPTGrab2::GetGPIOTriggerPolarity(int iPin, bool &bPolarity)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMTrigger)
	{
		return false;
	}
	
	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	
	bPolarity = ulValue & 0x0000FFFF;

	return true;
} 

// Strobe模式下，bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms
bool scPTGrab2::SetGPIOStrobeParam(int iPin, bool bPolarity, float fDelay, float fDuration)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMStrobe && mode != ePTGMDCAM)
	{
		return false;
	}

	if (mode == ePTGMStrobe)
	{
		// 设置Delay和Duration
		WORD wDelay = fDelay * 0.001 * PTCLOCK_FREQ;
		WORD wDuration =  fDuration * 0.001 * PTCLOCK_FREQ;
		ulValue = (wDelay<<16) | wDuration; 
		error = m_pCamera->WriteRegister(0x1110 + iPin*0x10+0x4, ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
		// 设置极性
		error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		ulValue &= 0x80030000;
		if (bPolarity)
		{
			ulValue |= 0x00000001;
		}
		error = m_pCamera->WriteRegister(0x1110 + iPin*0x10, ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
	}
	else // ePTGMDCAM
	{
		error = m_pCamera->ReadRegister(0x1300, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}

		bool bPresent = ulValue & (1<<(31-iPin));
		if (!bPresent) // 不存在
		{
			return false;
		}
	
		error = m_pCamera->ReadRegister(0x1400+0x4*iPin, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}

		bPresent = (ulValue & 0x80000000)>>31;
		bool bRead_out = (ulValue & 0x08000000)>>27;
		bool bOn_Off = (ulValue & 0x04000000)>>26;
		bool PolarityChange = (ulValue & 0x02000000)>>25;
		int iMinTime = (ulValue>>12) & 0x00000FFF;
		int iMaxTime = ulValue &0x00000FFF;
		if (!bPresent || !bRead_out || !bOn_Off)
		{
			return false;
		}

		DWORD wDelay = fDelay * 0.001 * PTCLOCK_FREQ;
		DWORD wDuration =  fDuration * 0.001 * PTCLOCK_FREQ;
		if (wDelay > iMaxTime)
		{
			wDelay = iMaxTime;
		}
		if (wDelay < iMinTime)
		{
			wDelay = iMinTime;
		}
		if (wDuration > iMaxTime)
		{
			wDuration = iMaxTime;
		}
		if (wDuration < iMinTime)
		{
			wDuration = iMinTime;
		}
		wDelay &= 0x0FFF;
		wDuration &= 0x0FFF;

		BYTE byStrobHead;
		if (bPolarity)
		{
			byStrobHead = 0x83;
		}
		else
		{
			byStrobHead = 0x82;
		}

		ulValue = byStrobHead<<24;
		ulValue |= (wDelay<<12);
		ulValue |= wDuration;
		error = m_pCamera->WriteRegister(0x1500+0x4*iPin, ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}	
	}

	return true;
}

// Strobe模式下，获取参数bPolarity:0,High active,1:low active; fDelay,ms;fDuration,ms
bool scPTGrab2::GetGPIOStrobeParam(int iPin, bool &bPolarity, float &fDelay, float &fDuration)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMStrobe && mode != ePTGMDCAM)
	{
		return false;
	}
	
	if (mode == ePTGMStrobe)
	{
		// 获取Delay和Duration
		error = m_pCamera->ReadRegister(0x1110 + iPin*0x10+0x4, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		fDelay  = (ulValue>>16) * (1.0/PTCLOCK_FREQ) * 1000.0;
		fDuration  = (ulValue&0x0000FFFF)  * (1.0/PTCLOCK_FREQ) * 1000.0;
		
		// 获取极性
		error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		bPolarity = ulValue & 0x0000FFFF;
	}
	else //ePTGMDCAM
	{
		error = m_pCamera->ReadRegister(0x1300, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
		bool bPresent = ulValue & (1<<(31-iPin));
		if (!bPresent) // 不存在
		{
			return false;
		}
		
		error = m_pCamera->ReadRegister(0x1400+0x4*iPin, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
		bPresent = (ulValue & 0x80000000)>>31;
		bool bRead_out = (ulValue & 0x08000000)>>27;
		bool bOn_Off = (ulValue & 0x04000000)>>26;
		bool PolarityChange = (ulValue & 0x02000000)>>25;
		int iMinTime = (ulValue>>12) & 0x00000FFF;
		int iMaxTime = ulValue &0x00000FFF;
		if (!bPresent || !bRead_out || !bOn_Off)
		{
			return false;
		}
		
		error = m_pCamera->ReadRegister(0x1500+0x4*iPin, &ulValue);
		if (error != PGRERROR_OK)
		{
			return false;
		}
		
		BYTE byStrobHead = (ulValue & 0xFF000000)>>24;
		bPresent = (byStrobHead & 0x80)>>7;
		if (!bPresent)
		{
			return false;
		}
		bPolarity = byStrobHead & 0x01;
		fDelay = ((ulValue & 0x00FFF000) >> 12) * (1.0/PTCLOCK_FREQ) * 1000.0;
		fDuration = (ulValue & 0x00000FFF) * (1.0/PTCLOCK_FREQ) * 1000.0;
	}
	
	return true;
}

// PWM模式下，设置极性，脉冲个数，持续时间(低和高),bEnableEnPin:是否采用EnPin;iEnablePinNum:EnPin序号
bool scPTGrab2::SetGPIOPWMParam(int iPin, bool bPolarity, int iPWMCnt, float fLowDuration, float fHighDuration, 
					 bool bEnableEnPin, int iEnablePinNum, bool bEnablePinPolarity)
{

	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMPWM)
	{
		return false;
	}
	
	// 设置	fLowDuration 和 fHightDuration
	WORD wLowPeriod = fLowDuration * 0.001 * PTCLOCK_FREQ;
	WORD wHighPeriod = fHighDuration * 0.001 * PTCLOCK_FREQ;
	ulValue = (wLowPeriod<<16) | wHighPeriod; 
	error = m_pCamera->WriteRegister(0x1110 + iPin*0x10+0x4, ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}


	// 设置PWM参数
	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	BYTE wPwmCnt  = iPWMCnt;
	BYTE wEnPinNum = iEnablePinNum & 0x07;
	BYTE wControl = (bEnablePinPolarity<<2) | (bEnableEnPin<<1) | (bPolarity);
	ulValue = 0x80040000 | (wPwmCnt<<8) | (wEnPinNum<<4) | wControl;
	error = m_pCamera->WriteRegister(0x1110 + iPin*0x10, ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}

	return true;
}

// PWM模式下，获取参数，极性，脉冲个数，持续时间ms(低和高),bEnableEnPin:是否采用EnPin;iEnablePinNum:EnPin序号
bool scPTGrab2::GetGPIOPWMParam(int iPin, bool &bPolarity, int &iPWMCnt, float &fLowDuration, float &fHighDuration, 
					 bool &bEnableEnPin, int &iEnablePinNum, bool &bEnablePinPolarity)
{
	if (!IsOnline())
	{
		return false;
	}
	
	Error   error;
	unsigned int ulValue = 0;
	
	PTGMMode mode;
	mode = GetGPIOMode(iPin);
	if (mode != ePTGMPWM)
	{
		return false;
	}
	
	// 获取	fLowDuration 和 fHightDuration
	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10+0x4, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}
	fLowDuration  = (ulValue>>16) * (1.0/PTCLOCK_FREQ) * 1000.0;
	fHighDuration = (ulValue&0x0000FFFF)  * (1.0/PTCLOCK_FREQ) * 1000.0;

	
	// 获取PWM参数
	error = m_pCamera->ReadRegister(0x1110 + iPin*0x10, &ulValue);
	if (error != PGRERROR_OK)
	{
		return false;
	}	
	iPWMCnt  = (ulValue>>8)&0x000000FF;
	BYTE wControl = (ulValue&0x0000000F);
	bEnablePinPolarity = (wControl>2) & 0x01;
	bEnableEnPin = (wControl>>1) & 0x01;
	bPolarity = wControl & 0x01;
	iEnablePinNum = (ulValue>>4)&0x00000007;

	return true;
}

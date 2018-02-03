
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS
#define WMU_UpdateStats (WM_USER+1)
#define WMU_SetRealToMapRatioAndVel (WM_USER+2)//主窗口中设置图片到实际的比例尺,以及速度的消息
#define WMU_SetGunParm (WM_USER+3)			//主窗口设置喷枪相关参数的消息
#define WMU_SetWorkRegParm (WM_USER+4)		//设置工作区的相关参数的消息

#define WMU_BeginWashGun (WM_USER+5)        //向主窗口发送开始洗涤喷枪，主窗口接着往下位机传指令
#define WMU_CloseAllGun  (WM_USER+6)        //关闭所有正在洗涤的喷枪

#define WMU_CameraCorrect (WM_USER+7)       //相机得到的相对于基准的偏差
#define WMU_SAVEPMCDIRECTORY (WM_USER+8)     //在IDD_PAINTDLG中当按下生成程序时，向上一对话框发送一个信号
#define WMU_PUMPPOWEROFF (WM_USER+9)
#define WMU_MOTOROFF (WM_USER+10)
#define WMU_PUMPPOWERON (WM_USER+11)
#define WMU_MOTORON (WM_USER+12)
#define WMU_RETURNZEROPOINT (WM_USER+13)
#define WMU_STOPALLFUNCTION (WM_USER+14)
#define WMU_DOWNLOADFILE (WM_USER+15)
#define WMU_RUN (WM_USER+16)
#define WMU_TEMPORARYSTOP (WM_USER+17)
#define WMU_STOPNOW (WM_USER+18)
#define WMU_AGAINRUN (WM_USER+19)
#define WMU_BEGINWASHGUN (WM_USER+20)
#define WMU_STOPWASHGUN (WM_USER+21)
//下面两个消息表示在对齐喷枪起点时，按下对齐+X等的响应消息和放开消息
#define WMU_ALIGNMENTCLICKED (WM_USER+22)
#define WMU_ALIGNMENTLOOSEN (WM_USER+23)
#define WMU_GUN0_DELAY (WM_USER+24)//喷枪0延迟开关数据传输消息
#define WMU_GUN1_DELAY (WM_USER+25)//喷枪1延迟开关数据传输消息
#define WMU_GUN2_DELAY (WM_USER+26)//喷枪2延迟开关数据传输消息
#define WMU_GUN3_DELAY (WM_USER+27)//喷枪3延迟开关数据传输消息
#define WMU_GUN4_DELAY (WM_USER+28)//喷枪4延迟开关数据传输消息
#define WMU_GUN5_DELAY (WM_USER+29)//喷枪5延迟开关数据传输消息
#define WMU_GUN0_REV_DELAY (WM_USER+30)//喷枪0逆序延迟开关数据传输消息
#define WMU_GUN1_REV_DELAY (WM_USER+31)//喷枪1逆序延迟开关数据传输消息
#define WMU_GUN2_REV_DELAY (WM_USER+32)//喷枪2逆序延迟开关数据传输消息
#define WMU_GUN3_REV_DELAY (WM_USER+33)//喷枪3逆序延迟开关数据传输消息
#define WMU_GUN4_REV_DELAY (WM_USER+34)//喷枪4逆序延迟开关数据传输消息
#define WMU_GUN5_REV_DELAY (WM_USER+35)//喷枪5逆序延迟开关数据传输消息
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



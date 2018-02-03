#pragma once
/*!
 * \class CPTCameraUsing
 *
 * \brief 在scPTGrab2类基础上增加我们要的初始化设置相机
 *
 * \author admin
 * \date 八月 2015
 */
#include "svptgrab2.h"
class CPTCameraUsing :
	public scPTGrab2
{
public:
	CPTCameraUsing(void);
	~CPTCameraUsing(void);
public:

	bool  InitCameraSetting(void);///<设置摄像机像素格式为BGR
};


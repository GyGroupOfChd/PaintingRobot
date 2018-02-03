#pragma once
/*!
 * \class  CRoadRecOnHalconWithPtgrey
 *
 * \brief 本类用于利用Halcon库，实现pointgrey相机采集图片，同时利用CRoadLineDetect类进行车道线识别，多封装了一些功能
 *
 * \author admin
 * \date 七月 2015
 */
#include "StdAfx.h"
#include <windows.h>
#include "RoadLineDetect.h"
//using namespace std;
class CRoadRecOnHalconWithPtgrey
{
public:
	CRoadRecOnHalconWithPtgrey(void);
	~CRoadRecOnHalconWithPtgrey(void);
	void Init(Hlong pParent,CRect RecOfWindowDispLay);      //初始化第一个对话框
	void InitTwo(Hlong pParent,CRect RecOfWindowDispLay);
	bool CreateGrabThread();///<创建采集线程，未启用
	UINT GrabThread(LPVOID lpParam);///<采集线程响应程序，未启用
	bool RecOneGrab()///<识别一次，未启用
	{
		return false;
	}
	LocationCorrect RecOneGrab(unsigned char* pImgData, long lWidth, long lHeight)   ///<识别一次
	{
		if ( pImgData!=NULL)
		{
			return m_HalconRoadRec.RoadRec(pImgData, lWidth, lHeight);
			
		}
	}
	LocationCorrect RecOneGrabTwo(unsigned char* pImgData, long lWidth, long lHeight)  //第二个相机图像处理程序
	{
		if ( pImgData!=NULL)
		{
			return m_HalconRoadRec.RoadRecTwo(pImgData, lWidth, lHeight);

		}
	}
	HalconCpp::HTuple * QueryGreyThreshold()
	{
		return m_HalconRoadRec.QueryGreyThreshold();
	}
	bool ChangeGreyThresholdValue(unsigned MinThres,unsigned MaxThres)///<改变阈值的接口函数，值需在0~255
	{
		return m_HalconRoadRec.ChangeGreyThresholdValue(MinThres,MaxThres);
	}
	bool ChangeGreyThresholdMaxValue(unsigned MaxThres)///<改变阈值的接口函数，值需在0~255
	{
		return m_HalconRoadRec.ChangeGreyThresholdMaxValue(MaxThres);
	}
	bool ChangeGreyThresholdMinValue(unsigned MinThres)///<改变阈值的接口函数，值需在0~255
	{
		return m_HalconRoadRec.ChangeGreyThresholdMinValue(MinThres);
	}


	CRoadLineDetect* QueryRoadLineRecSolid(){
		return &m_HalconRoadRec;
	}
	void ChangeThresholdSlider(ThresholdSlide m_ThresholsSlider)        //改变阈值
	{
		 m_HalconRoadRec.SetThresholdSlider(m_ThresholsSlider);
	}
	void ChangeThresholdSliderTwo(ThresholdSlide m_ThresholsSlider)        //改变阈值
	{
		m_HalconRoadRec.SetThresholdSliderTwo(m_ThresholsSlider);
	}
protected:
	CRoadLineDetect m_HalconRoadRec;


	
};


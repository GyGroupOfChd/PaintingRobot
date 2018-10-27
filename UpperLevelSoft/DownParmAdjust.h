#pragma once
#include "DownParmValueHead.h"
/*!
 * \class CDownParmAdjust
 *
 * \ingroup GroupName
 *
 * \brief 本类用于给形成的下传参数进行调整，以便后续处理实现下载的内容，此类转供DownParmValue类调用或继承
 * 颜色状态存在于m_CheckPointColorListWithDelay，突变点状态存在于m_CheckPointListWithDelay
 ShrinkCheckPointVec
 *
 * TODO: long description
 *
 * \note 
 *
 * \author gy
 *
 * \version 1.0
 *
 * \date 十二月 2017
 *
 * Contact: user@company.com
 *
 */
//template<class CDownParmType>
class CDownParmAdjust
{
public:
	CDownParmAdjust(void)
	{

	}
	~CDownParmAdjust(void)
	{

	}
protected:
/*!
 *@brief 对给定点施加平移操作，平移值在乘员变量m_TransitValue中，会判断平移后是否坐标超限0~m_MaxLength
 *
 *@parm
 *@parm
 *
 *@return
 *@todo
 */
	void AddTransitValue(CornerPoint& Point)
	{
		Point.YPoint+=m_TransitValue;
		Point.YPoint=Point.YPoint>m_MaxLength?m_MaxLength:Point.YPoint;
		Point.YPoint=Point.YPoint<0?0:Point.YPoint;
	}
/*!
 *@brief 对一列中的所有内容进行平移，TransitTheRows调用
 *
 *@parm
 *@parm
 *
 *@return
 *@todo
 */
	void TransitTheRealWorkPartsOftheRows(VectorTypePoint& DownParm,int TransitValue)
	{
		m_TransitValue=TransitValue;//bUnEvenRow?TransitValue:-TransitValue;//奇数列+，偶数列减
		VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		//it_vCheckPoint++;//第一个是起始位置//第一个起点不动，该点实际是新加进去的每列强制要求关闭的起始点（取消）
		for (it_vCheckPoint;it_vCheckPoint!=DownParm.end();it_vCheckPoint++)
		{
			AddTransitValue(*it_vCheckPoint);
		}
	}
public:
/*!
 *@brief 平移各列，考虑各列不超过最大范围为0-MaxLength，取消（并保留平移前的起点位置，并设改点位置颜色位0）
 *所以改动后，真正的喷涂区域整体平移了。平移后的结果仍然保存在输入的 DownParm，DownParmColor参数中
 *
 *@parm DownParm 列各突变点的坐标
 *@parm DownParmColor 列各突变点对应的颜色
 *@parm TransitValue 全部区域整体平移的数值，实际上对偶数列还有一个额外的平移，该平移数值由宏EXTRA_TRANSIT_FOR_EVEN_ROW确定
 *@parm MaxLength 最大的运动范围，超过这个范围会被截断，以防运动超限
 *@parm bUnEvenRow 是否当前为偶数列
 *
 *@return
 *@todo
 */
	void TransitTheRows(VectorTypePoint& DownParm,VectorType& DownParmColor,int TransitValue,float MaxLength,bool bUnEvenRow)
	{

		
		m_MaxLength=MaxLength;
		//VectorType::iterator it_vCheckPointColor=DownParm.begin();
				
		/////////////////////平移之前在列头上把起点复制一份/////////////////////////////////////////////////////
		//CornerPoint HeadPoint=*(DownParm.begin());//第一个点复制
		//DownParm.insert(DownParm.begin(),HeadPoint);
		//DownParmColor.insert(DownParmColor.begin(),0);//插入新起始点对应的颜色0,新起点全关
		
		//////////////////////////////////////////////////////////////////////////
		TransitTheRealWorkPartsOftheRows(DownParm,TransitValue);//整体平移


		/*		int EXTRA_TRANSIT_FOR_EVEN_ROW=-40;初始文件为-40，可以根据修改文件修改参数*/
		int EXTRA_TRANSIT_FOR_EVEN_ROW;
		FILE *fp;
		char buffer[256];
		if ( fp=fopen("E:\\偶数列移动设置.txt","r"));
		fgets(buffer,255,fp); 
		EXTRA_TRANSIT_FOR_EVEN_ROW=atoi(buffer);
		fclose(fp);

		if (!bUnEvenRow)//偶数列再移动若干mm
		{
			TransitTheRealWorkPartsOftheRows(DownParm,EXTRA_TRANSIT_FOR_EVEN_ROW);//整体平移
		}

		FILE *m_p=fopen("E:\\偶数列移动设置.txt","w");
		CString str;
		str.Format(_T("%d"),EXTRA_TRANSIT_FOR_EVEN_ROW);
		fwrite(str,1,str.GetLength(),m_p);
		str.ReleaseBuffer();
		fclose(m_p);

		//VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		//it_vCheckPoint++;//第一个是起始位置
		//for (it_vCheckPoint;it_vCheckPoint!=DownParm.end();it_vCheckPoint++)
		//{
		//	AddTransitValue(*it_vCheckPoint);
		//}
	//	std::for_each(it_vCheckPoint,DownParm.end(),AddTransitValue);	
		
		
	}
/*!
 *@brief 避免一列结束后移动到下一列时产生斜向运动，在下一列运动前先增加一个新的起点，该起点Y坐标是上一列运动的终点
 *
 *@parm DownParm 当前列所有的运动点
 *@parm DownParmColor 列各突变点对应的颜色
 *
 *@return
 *@todo
 */
	bool AddRowStartPointForOneRowMotion(VectorTypePoint& DownParm,VectorType& DownParmColor)
	{
		if (PreRowPoint.size()==0)//第一列
		{
			PreRowPoint=DownParm;
			return false;
		}
		VectorTypePoint::iterator it_vPreCheckPoint=PreRowPoint.end();
		it_vPreCheckPoint--;
		CornerPoint NewStartPoint=*it_vPreCheckPoint;//上一列最后一个点
		VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		NewStartPoint.XPoint=it_vCheckPoint->XPoint;
		DownParm.insert(it_vCheckPoint,NewStartPoint);
		DownParmColor.insert(DownParmColor.begin(),0);
		PreRowPoint=DownParm;
		return true;
	}
protected:
	VectorTypePoint PreRowPoint;///<上一列所有的点
	int m_TransitValue;
	float m_MaxLength;
};


#pragma once
//#include "Parm.h"
#include "atltypes.h"
#include <vector>
#include <algorithm>
/********************************************************************
	created:	2011/09/02
	created:	2:9:2011   23:33
	filename: 	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft\PaintCell.h
	file path:	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft
	file base:	PaintCell
	file ext:	h
	author:		YangGAO
	
	purpose:	从矩形中继承而来，表示喷涂的最小单位——目前默认是矩形，内部单位均为像素
*********************************************************************/
/**
* @class 
* @brief 整个图片上操作区间的相关属性，它实际上是包络所有需要喷涂区域的最小矩形
*
* 用于定义操作区间的长宽等,单位为像素，实际距离的话单位为mm
*/
struct PaintAreaTemplate 
{
	long StartPointX;///<起始点X坐标
	long StartPointY;///<起始点Y坐标

	long EndPointX;///<终点X坐标
	long EndPointY;///<终点Y坐标

	long WidthInPixel;///<宽度，单位像素
	long HeightInPixel;///<高度，单位像素

	double WidthInMM;///<宽度，单位MM
	double HeightInMM;///<高度，单位MM
	int Width()
	{
		return EndPointX-StartPointX;
	}
	int Height()
	{
		return EndPointY-StartPointY;
	}
	
};
/**
*@brief 专供待会 pushBackColor函数中的count if使用，只返回真
*
*
*@parm
*
*@return
*@todo
*/
static bool CountFun (COLORREF& CurColor) { return true; }
typedef std::vector<COLORREF> ColorVec;
class CPaintCell :
	public CRect
{
public:
	CPaintCell(void);
	~CPaintCell(void);
	unsigned m_uColorIndex;///<本单元内的颜色所对应的序号

/**
*@brief 将当前喷涂区域的颜色信息存起来
*
*检测当前颜色是否已经存入过了，没有的话，将新遇到的颜色填入 m_vColorToBePainted，并把相应下标赋给被喷涂区域的m_uColorIndex，否则把已有
*颜色下标赋给被喷涂区域的m_uColorIndex
*@parm CurColor当前喷涂单元的颜色的
*
*@return
*@todo
*/
	void pushBackColor(COLORREF CurColor)
	{
		
		if (m_vColorToBePainted.empty())
		{
			m_vColorToBePainted.push_back(CurColor);
			this->m_uColorIndex=0;
				return;
		}
		ColorVec::iterator iterEnd=m_vColorToBePainted.end();
		ColorVec::iterator iterBeg=m_vColorToBePainted.begin();
		ColorVec::iterator it;
		it = std::find(iterBeg, iterEnd, CurColor);
		if (it!=iterEnd)//是已有颜色
		{
			this->m_uColorIndex=count_if(iterBeg,it,CountFun);//个数代表着下标，下标从0开始，count if 恰好满足，不需-1。
			//this->m_uColorIndex=it-iterBeg;//不支持iterator之间的加减
		}
		else//新的颜色，未见过的
		{
			m_vColorToBePainted.push_back(CurColor);
			this->m_uColorIndex=m_vColorToBePainted.size()-1;
		}
		//ASSERT(this->m_uColorIndex!=6);
	}
	void InitAll()
	{
		m_vColorToBePainted.clear();
	}
	void operator=(CRect TarRect)
	{
		top=TarRect.top;
		bottom=TarRect.bottom;
		left=TarRect.left;
		right=TarRect.right;
	}
	void operator=(CPaintCell& TarRect)
	{
		top=TarRect.top;
		bottom=TarRect.bottom;
		left=TarRect.left;
		right=TarRect.right;
		m_uColorIndex=TarRect.m_uColorIndex;

	}
	COLORREF& QueryColorByIndex(unsigned ColorIndex)
	{return m_vColorToBePainted[ColorIndex];}
	unsigned QueryNumOfColorToBePainted()
	{return m_vColorToBePainted.size();	}
protected:
	static ColorVec m_vColorToBePainted;///<所有识别出来的颜色，都将存入本向量，其下标，即是该颜色对应的序号;
};
/**
* @class 
* @brief 喷涂区域中的列类
*
* 将喷涂区域逐列进行划分，得到列。默认列宽为喷涂单元的宽度
*/
class CRow:public CRect
{
public:
	CRect * CellInRow(unsigned Line)
	{
		return &( m_vPaintCellVec[Line]);
	}
	CPaintCell * FindCellInRow(unsigned Line)
	{
		return &( m_vPaintCellVec[Line]);
	}
	long ValidTopOfThisRow()
	{
		return m_vPaintCellVec[0].top;
	}
	long ValidBottomOfThisRow()
	{
		if (m_bNeedPaint)
		{
			std::vector<CPaintCell>::iterator itOfCellVec;
			itOfCellVec=m_vPaintCellVec.end()-1;
			return itOfCellVec->bottom;
		}
		return 0;
	}
public:
	void InitAll()
	{
		m_vPaintCellVec.clear();
	}
	bool m_bNeedPaint;///<本列包含需要喷涂的单元
	std::vector<CPaintCell> m_vPaintCellVec;///<本列中包含的所有喷涂单元
	
	unsigned NumOfCellInRow()
	{
		return  m_vPaintCellVec.size();
	}
};
/**
* @class 
* @brief 整体一次移动的工作区域，矩形
*
* 
*/
class CRect_Work_Region: public CRect
{
public:
	CRect_Work_Region(void);
	~CRect_Work_Region(void);
public:
	std::vector<CRow> m_vRowsVec;///<本工作区中包含的所有需要喷涂的列
public:
	
	unsigned NumOfRow;///<本工作区中包含的有需要喷涂列数
	unsigned m_uNumOfPaintCell;///<本工作区中包含的喷涂单元个数
	bool m_bNeedPaint;///<本区包含需要喷涂的单元
	void InitAll()
	{
		m_bNeedPaint=false;
		 NumOfRow=0;
		 m_uNumOfPaintCell=0;
		 m_vRowsVec.clear();
		WidthOfWorkRegion=LineNumOfWorkRegion=RowNumOfWorkRegion=NumOfWorkRegion=0;
		 InRowIndex=HeightofWorkRegion=0;
	}
/**
*@brief 检查自身是否位于图像的边界上
*
*
*@parm BorderPixelVal 边界对应的坐标值，像素
*@parm BorderPixelVal 当前要检查的是否X轴上的边界?是，为真
*
*@return 是则为真
*@todo
*/
	bool IsBorder(unsigned BorderPixelVal,bool bCheckingBorderOfX)
	{
		if (bCheckingBorderOfX)
		{return BorderPixelVal==right;}
		return BorderPixelVal==bottom;
	}
	CRect * RecedCell(unsigned Line,unsigned Row)
	{
		return m_vRowsVec[Row].CellInRow(Line);
	}
	CPaintCell * FindRecedCell(unsigned Line,unsigned Row)
	{
		return m_vRowsVec[Row].FindCellInRow(Line);
	}
	int NumOfCellInGivenRow(unsigned Row)
	{
		return m_vRowsVec[Row].NumOfCellInRow();
	}
	long NumOfCell()
	{
		return m_uNumOfPaintCell;
	}
	bool GivenRowNeedPaint(unsigned Row)
	{
		return m_vRowsVec[Row].m_bNeedPaint;
	}
/**
*@brief 第一个有效喷涂颜色的起始坐标
*
*
*@parm
*
*@return
*@todo
*/
	long ValidTopOfGivenRow(unsigned Row)
	{
		return m_vRowsVec[Row].ValidTopOfThisRow();
	}
	long ValidBottomOfGivenRow(unsigned Row)
	{
		return m_vRowsVec[Row].ValidBottomOfThisRow();
	}
public:
	static unsigned NumOfWorkRegion;///<全图中划分的工作区个数
	static unsigned RowNumOfWorkRegion;///<全图中划分的工作区列的数目
	static unsigned LineNumOfWorkRegion;///<全图中划分的工作区行的数目
	static unsigned WidthOfWorkRegion;///<一个工作区域标准的宽度，特别注意与CRect函数集成来的Width函数不同。这个是共有的性质，后者是某一个区域的实际宽度。
	static unsigned HeightofWorkRegion;///<一个工作区域标准的高度，特别注意与CRect函数集成来的Height函数不同。这个是共有的性质，后者是某一个区域的实际高度。
	unsigned InRowIndex;///<本工作区所在的列序号

};

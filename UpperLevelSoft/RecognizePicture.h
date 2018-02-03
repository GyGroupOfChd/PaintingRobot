#pragma once
#include "Parm.h"
#include <atltypes.h>
//#include "PaintCell.h"
#include <math.h>
#include <vector>
#include <map>
#include "gyMath.h"
/********************************************************************
	created:	2011/08/31
	created:	31:8:2011   11:14
	filename: 	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft\RecognizePicture.h
	file path:	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft
	file base:	RecognizePicture
	file ext:	h
	author:		YangGao
	
	purpose:	对读入的图片进行图像处理，暂时主要针对规则的图形，固定工作区域为矩形，喷涂区域也为矩形。无偏斜

*********************************************************************/
//#define AVERAGE_SIZE_OF_CELL ///<对单元的参数预识别时，进行复杂的平均计算，是最
/** @file 
*@brief  对读入的图片进行图像处理
* @author YangGao
* @date 2011/08/31
* @version 1.0
*
* 对读入的图片进行图像处理，暂时主要针对规则的图形，固定工作区域为矩形，喷涂区域也为矩形。无偏斜。先将整个图片划分为若干工作区，再将工作区划分为每一个喷涂单元加以识别
*/
/**
* @class 
* @brief 本类专门用于将图片划分成一个个的喷涂工作区域。
*
* 
*/
class CPartionPicture
{
public:
	CPartionPicture(void);
	~CPartionPicture(void);
	/**
	*@brief 将整个图片分区
	*
	*按照沿X,Y轴方向第一次发现需喷涂的方块位置将整个图片分区
	*@parm pPicDC 地图DC
	*@parm Width 地图宽度
	*@parm Height 地图高度
	*
	*@return 整幅图片发现了需要喷涂的地方则返回真
	*@todo
	*/
	bool PartionThePic(CDC*pPicDC,long Width,long Height);
/**
*@brief 智能化的划分图片
*
*与PartionThePic相比，增加了对每个划分的工作区间的所有边界的智能化处理，避免边界将最小喷涂单元列1分为2
*@parm
*
*@return
*@todo
*/
	bool PationPicIntell(CDC*pPicDC,long Width,long Height);
	/**
	*@brief 重新微调工作区的边界，使之不分割喷涂单元
	*
	*在进行了一次划分后，重新微调工作区的边界，使之不分割喷涂单元
	*@parm
	*
	*@return
	*@todo
	*/
	int RefineTheBorder(CDC*pPicDC);
protected:
	/**
	*@brief 将整个图片分区
	*
	*可以按照预定义的列宽度、图片尺寸划分。这里假设列宽即为每个PaintCell得宽度。直接依整体图片的最小包络矩形
	*大小，对该包络矩形进行分割工作区。实现对整个图片的分区.按照先列后行的顺序将分区结果存储在m_vWorkRegionPationed
	*@parm
	*
	*@return 没找到合适的边界，或者没找到喷涂区域则返回假否则返回真
	*@todo
	*/
	bool PartionThePicRegulaly(CDC*pPicDC,long Width,long Height);
/**
*@brief 划分工作区后进一步将工作区划分为一个个列
*
*划分工作区后,对每一个工作区按照列宽，简单的，而不对每一个工作区中是否划出的列都有喷涂单元进行检测。
*进一步将工作区划分为一个个列,相当于画格子
*@parm
*
*@return
*@todo
*/
	void FurtherPartionTheWorkRegIntoRow(CRect_Work_Region& Work_Region);
	/**
	*@brief 按给定区域宽度将整个图片分区
	*
	*可以按照预定义的列宽度、图片尺寸划分。
	*@parm
	*
	*@return
	*@todo
	*/
	bool PartionThePicAccordGivenWidth(CDC*pPicDC,long Width,long Height);
/**
*@brief 用于在划分区域时，检查边界并特殊该边界上的区域属性进行处理
*
*专供 划分函数调用，当划分到边界时，可能一个工作区域并不完整，需要特殊处理
*@parm CurRowOrLine      当前需要判断的行、列算子
*@parm BorderVal         边界处算子值
*@parm AttributeL        需要赋值的属性L
*@parm BorderAttributeR  边界处需要赋给的特殊R属性值
*@parm AttributeLVal     非边界处需要赋给的L属性值
*
*@return 的确是特殊处理的边界则返回值真
*@todo   本函数只考虑了边界处连个变量的异常。另外也只考虑了划分时最后一行与最后一列的异常处理，如果是第一行或第一列的出现异常的话，未处理
*/
	bool PartionBorderCheckAndHandle(unsigned CurRowOrLine,unsigned BorderVal,long& AttributeL,long&AttributeR,
									long BorderAttributeLVal,long BorderAttributeRVal,
									long AttributeLVal,long AttributeRVal)
	{
		if (CurRowOrLine==BorderVal)
		{
			AttributeL=BorderAttributeLVal;
			AttributeR=BorderAttributeRVal;
			return true;
		}
		else
		{
			AttributeL=AttributeLVal;
			AttributeR=AttributeRVal;
		}
		return false;
	}
	public:
/**
*@brief 搜索整个图片的有效边界
*
*当不是简单的按照工作区宽度对整个图片进行分割时，必须考虑到图片的四周有可能有空白区域无用。因此将四条边向内压缩直到最小包络图片内部图形
*其结果存储在g_sPaintAreaParm中
*@parm 
*
*@return 若最小包络矩形是存在的，图片内部是有喷涂图像的，则返回真
*@todo
*/
	bool SearchValidatePicBorder(CDC*pPicDC,long Width,long Height);
/**
*@brief 在搜索图片的最小包络矩形时将边,沿X轴向内缩
*
*在搜索图片的最小包络矩形时将边,沿X轴向内缩，以碰到的第一个需喷涂区域的X坐标，作为本地图新的X坐标边界。该函数依靠 StartX与其边界 BorderVal之间的大小判断是沿X增加方向
*搜索，还是沿-方向搜索
*@parm pPicDC  地图DC
*@parm BorderVal X坐标对应的搜索界限，
*@parm StartX  沿X坐标搜索的起始点坐标
*@parm StartY  整个有效地图的Y坐标最小值
*@parm BiggerBorderOfYaxs  整个有效地图的Y坐标最大值
*
*@return 找到需喷涂的区域则返回真
*@todo
*/
	bool ShrinkXBorder(CDC*pPicDC,long BorderVal,long StartX,long StartY,long BiggerBorderOfYaxs);
	/**
	*@brief 在搜索图片的最小包络矩形时将边,沿Y轴向内缩
	*
	*在搜索图片的最小包络矩形时将边,沿X轴向内缩，以碰到的第一个需喷涂区域的X坐标，作为本地图新的X坐标边界。该函数依靠 StartY与其边界 BorderVal之间的大小判断是沿Y增加方向
	*搜索，还是沿-方向搜索
	*@parm pPicDC  地图DC
	*@parm BorderVal Y坐标对应的搜索界限，
	*@parm StartX  整个有效地图的X坐标最小值
	*@parm StartY  沿Y坐标搜索的起始点坐标
	*@parm BiggerBorderOfYaxs  整个有效地图的X坐标最大值
	*
	*@return 找到需喷涂的区域则返回真
	*@todo
	*/
	bool ShrinkYBorder(CDC*pPicDC,long BorderVal,long StartX,long StartY,long BiggerBorderOfXaxs);
	/**
	*@brief 重置，或者说初始化函数
	*
	*重置，或者说初始化函数,将所有成员变量赋给零值，清空所有容器，防止出错
	*@parm
	*
	*@return
	*@todo
	*/
	void InitAll()
	{
		m_vWorkRegionPationed.clear();
	}
public:
	std::vector<CRect_Work_Region>m_vWorkRegionPationed;///<划分后得到的工作区序列，存储顺序为按照先列后行
	//m_vWorkRegionPationed.size_type
	/**
	*@brief 按先逐行的方式将整个图片分区
	*
	*按先逐列的方式将整个图片分区,外部需指定好分区的参数，比如尺寸。分区依据为尺寸。结果存放在m_vWorkRegionPationed
	*@parm
	*
	*@return
	*@todo
	*/
	bool Partion(void);
	
};
class CMapSearch
{
protected:
	/**
	*@brief 搜索非空颜色区块的长度
	*
	*从给定点开始，同时沿x,y方向搜索非空颜色区块，将其信息结果放在m_SearchResRect
	*@parm
	*
	*@return
	*@todo
	*/
	bool SearchLengthOfNonNuLLColor(CDC* pDCOfMapToRec,long XBorder,long YBorder,long StartX,long StartY);
	CRect m_SearchResRect;///<SearchLengthOfNonNuLLColor函数搜索出的非空颜色区块
	/**
	*@brief 沿X轴搜索本喷涂区域的边界
	*
	*在一列的搜索中，已经找到了喷涂区域，找到了该喷涂区域的起始点(左上)沿X轴方向搜索其占的大小
	*
	*@parm CurColor 当前区域包含的颜色
	*@parm pPicDC 带地图的DC用于提取地图
	*@parm Border 该方向的边界，即X方向大小
	*@parm StartX 本喷涂单元的起始点的x坐标
	*@parm StartY 本喷涂单元的起始点的Y坐标
	*
	*@return 该喷涂区域大于等于1个像素的宽度则返回真
	*@todo
	*/
	int SearchAlongX(COLORREF& CurColor,CDC*pPicDC,long Border,long StartX,long StartY);
	/**
	*@brief 沿Y轴搜索本喷涂区域的边界
	*
	*在一列的搜索中，已经找到了喷涂区域，找到了该喷涂区域的起始点(左上)沿X轴方向搜索其占的大小
	*
	*@parm CurColor 当前区域包含的颜色
	*@parm pPicDC 带地图的DC用于提取地图
	*@parm Border 该方向的边界，即Y方向大小
	*@parm StartX 本喷涂单元的起始点的x坐标
	*@parm StartY 本喷涂单元的起始点的Y坐标
	*
	*@return 该喷涂区域大于等于1个像素的宽度则返回真
	*@todo
	*/
	int SearchAlongY(COLORREF& CurColor,CDC*pPicDC,long Border,long StartX,long StartY);
};
typedef std::map<unsigned,unsigned>::value_type MValType;
typedef std::map<unsigned,unsigned> ResMapType;
/**
* @class 
* @brief 对图片进行预识别,识别出图片内单元的平均尺寸参数
*
* 
*/
class CAverageCellSizeRec:public CMapSearch
{
protected:
#define BIGEST_SIZE_RATIO_OF_ONE_CELL 1.5//因图片变形，允许单个单元尺寸变化的系数
	/**
	*@brief 沿1行搜索喷涂单元的宽度参数,把所有可能结果放在m_vPossibleSizeOfCell
	*
	*沿x，沿1行搜索喷涂单元的宽度参数,把所有可能结果放在m_vPossibleSizeOfCell,要求给定一个可能的最小宽度值，这个值可能在内部发生改变，如果寻找到更小的话
	*@parm 
	*
	*@return 返回找到的最小宽度值，若没找到则返回的是图片的宽度。
	*@todo
	*/
	unsigned Search1LineForAllPossibleParmOfPaintCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight,long StartX,long StartY,unsigned& MiniSizeOfParm);

	std::map<unsigned,unsigned> m_mPossibleWidthOfCell;//所有可能是一个单元的尺寸值,主索引为值，值为个数
	/**
	*@brief 沿1行搜索喷涂单元的最小宽度参数
	*
	*沿x，结果宽度返回
	*@parm 
	*
	*@return 返回找到的最小宽度值，若没找到则返回的是已经找到的最小宽度。
	*@todo
	*/
	unsigned Search1LineForMiniParmOfPaintCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight,long StartX,long StartY,unsigned& MiniSizeOfParm);
	/**
	*@brief 逐行搜索喷涂单元的最小宽度参数
	*
	*沿x，先粗略搜索出最小宽度值，再将所有可能的宽度值存起来，求平均作为结果宽度返回
	*@parm 
	*
	*@return 返回找到的单个单元合理宽度值，若没找到则返回的0。
	*@todo
	*/
	unsigned SearchWidthOfOneParm(CDC* pDCOfMapToRec,long MapWidth,long MapHeight);
protected:
	/**
	*@brief 沿1列搜索喷涂单元的最小高度参数
	*
	*沿x，结果宽度返回
	*@parm 
	*
	*@return 返回找到的最小宽度值，若没找到则返回的是已经找到的最小高度。
	*@todo
	*/
	unsigned Search1RowForMiniParmOfPaintCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight,long StartX,long StartY,unsigned& MiniSizeOfParm);
	/**
	*@brief 沿1列搜索喷涂单元的宽度参数,把所有可能结果放在m_vPossibleSizeOfCell
	*
	*沿x，沿1列搜索喷涂单元的宽度参数,把所有可能结果放在m_vPossibleSizeOfCell,要求给定一个可能的最小宽度值，这个值可能在内部发生改变，如果寻找到更小的话
	*@parm 
	*
	*@return 返回找到的最小高度值，若没找到则返回的是图片的高度。
	*@todo
	*/
	unsigned Search1RowForAllPossibleParmOfPaintCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight,long StartX,long StartY,unsigned& MiniSizeOfParm);
	/**
	*@brief 逐行搜索喷涂单元的最小宽度参数
	*
	*沿x，先粗略搜索出最小宽度值，再将所有可能的宽度值存起来，求平均作为结果宽度返回
	*@parm 
	*
	*@return 返回找到的单个单元合理宽度值，若没找到则返回的0。
	*@todo
	*/
	unsigned SearchHeightOfOneParm(CDC* pDCOfMapToRec,long MapWidth,long MapHeight);
	ResMapType m_mPossibleHeightOfCell;
};
/**
* @class CPreRecgnize
* @brief 对图片进行预处理。识别图片内喷涂单元的尺寸，会识别单元之间的间隔
*
* 
*/
class CPreRecgnize:public CAverageCellSizeRec
{
public:
	/**
 *@brief 类的接口函数，负责识别图片数据
 *
 *
 *@parm pDCOfMapToRec需要识别的图片所在的DC
 *@parm MapWidth 图片的宽度，单位像素
 *@parm MapHeight 图片的高度，单位像素
 *
 *@return 成功则返回真
 *@todo
 */
	bool PreReggnizeForParm(CDC* pDCOfMapToRec,long MapWidth,long MapHeight);
protected:

	/*unsigned m_uHeightResult;
	unsigned m_uWidthResult;*/
	/**
	*@brief 搜索喷涂单元的长、宽参数
	*
	*同时沿x,y方向搜索，结果放在m_RecgnizedWidthParmOfPaintCell，m_RecgnizedHeightParmOfPaintCell
	*@parm 
	*
	*@return
	*@todo
	*/
	bool LookForParmOfPaintCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight,long StartX,long StartY)
	{
		if (gCellSizePreRecFlag==AVERAGE_SIZE_OF_CELL)
		{
			m_RecgnizedWidthParmOfPaintCell=SearchWidthOfOneParm(pDCOfMapToRec,MapWidth,MapHeight);
			m_RecgnizedHeightParmOfPaintCell=SearchHeightOfOneParm(pDCOfMapToRec,MapWidth,MapHeight);
			if (m_RecgnizedWidthParmOfPaintCell!=0&&m_RecgnizedHeightParmOfPaintCell!=0)
			{return true;}
			return false;			
		}		
		if (SearchLengthOfNonNuLLColor(pDCOfMapToRec,MapWidth,MapHeight, StartX, StartY))
		{
			m_RecgnizedWidthParmOfPaintCell=m_SearchResRect.Width();
			m_RecgnizedHeightParmOfPaintCell=m_SearchResRect.Height();
			return true;
		}		
		return false;
	}
	void SetParmAccordingly()
	{
		gRowWidth=m_RecgnizedWidthParmOfPaintCell+1;
		gHeightOfEveryPaintCell= m_RecgnizedHeightParmOfPaintCell+1;//算法中由于用到Width函数等，决定必须+1
	}
/**
*@brief 自动识别单元之间空隙的宽度，默认其宽度与高度是一样的
*
*自动识别单元之间空隙的宽度。这里对空隙是有假设的，空隙宽度至少为 MINI_CELL_WIDTH_THREAD第一个发现的喷涂单元右下角处必然有空隙,否则就没空隙
*关于单元尺寸的识别，在这里如果搜索空隙时，碰到了也会进行处理。
*@parm
*
*@return 若第一个发现的有色区块是喷涂单元，且后来发现了间隙则返回2，若第一个发现的是喷涂单元，后来没发现空隙则返回3，若第一个发现的是个空隙，则返回1，否则返回0
*
*@todo
*/
	int LookForParmOfGapBetweenCell(CDC* pDCOfMapToRec,long MapWidth,long MapHeight);
	
	unsigned m_RecgnizedWidthParmOfPaintCell;
	unsigned m_RecgnizedHeightParmOfPaintCell;
	
	int SearchBorderOfCurPaintCell(COLORREF& CurColor,CDC*pPicDC,long Border,long StartX,long StartY,bool bSearchAlongX);

//Average ( ) 



};
typedef std::vector<CRect_Work_Region>::iterator  ItOfRegVec;
class CRecognizePicture:public CPartionPicture,public CPreRecgnize
{
public:
	CRecognizePicture(void);
	~CRecognizePicture(void);
/**
*@brief 类的接口函数，负责识别图片数据
*
*
*@parm pDCOfMapToRec需要识别的图片所在的DC
*@parm MapWidth 图片的宽度，单位像素
*@parm MapHeight 图片的高度，单位像素
*
*@return 成功则返回真
*@todo
*/
	bool RecgnizePic(CDC* pDCOfMapToRec,long MapWidth,long MapHeight);
/**
*@brief 返回划分了多少个区域，不代表区域内有无色块
*
*
*@parm
*
*@return
*@todo
*/
	unsigned NumOfRecedPic()
	{
		return m_vWorkRegionPationed.size();
	}
	long NumOfRecedCell()
	{
		long NumOfCell=0;
		for (std::vector<CRect_Work_Region>::iterator itReg=m_vWorkRegionPationed.begin();
			itReg!=m_vWorkRegionPationed.end();itReg++)
		{
			if (itReg->m_bNeedPaint)
			{
				NumOfCell+=itReg->NumOfCell();
			}			
		}
		return NumOfCell;
	}
	CPoint TopLeftOfWorkRegion(unsigned indexOfReg)
	{
		return m_vWorkRegionPationed[indexOfReg].TopLeft();
	}
	CPoint BottomRighttOfWorkRegion(unsigned indexOfReg)
	{
		return m_vWorkRegionPationed[indexOfReg].BottomRight();
	}
	unsigned TopOfReg(unsigned indexOfReg)
	{
		return m_vWorkRegionPationed[indexOfReg].top;
	}
	unsigned BottomOfReg(unsigned indexOfReg)
	{
		return m_vWorkRegionPationed[indexOfReg].bottom;
	}
	unsigned LeftOfReg(unsigned indexOfReg)
	{
		return  m_vWorkRegionPationed[indexOfReg].left;
	}
	unsigned RightOfReg(unsigned indexOfReg)
	{
		return m_vWorkRegionPationed[indexOfReg].right;
	}
	CRect* RectOfReg(unsigned indexOfReg) 
	{
		return &(m_vWorkRegionPationed[indexOfReg]);
	}
	CRect* RectOfCell(unsigned Line,unsigned Row,unsigned indexOfReg)
	{
		return (m_vWorkRegionPationed[indexOfReg].RecedCell(Line,Row));
	}
/**
*@brief 查询指定的喷涂单元并返回指针
*
*
*@parm
*
*@return
*@todo
*/
	CPaintCell* QueryPaintCell(unsigned Line,unsigned Row,unsigned indexOfReg)
	{
		return (m_vWorkRegionPationed[indexOfReg].FindRecedCell(Line,Row));
	}
	/**
	*@brief 查询指定列的起始终止位置,查询的是识别结果，仍未考虑喷枪安装导致的余量问题
	*
	*有需要喷涂则返回真，并且通过输入参数StartY EndY返回结果，单位为mm，未考虑喷枪安装导致的余量
	*@parm StartY 输入
	*
	*@return
	*@todo
	*/
	//Row代表列数编号,indexOfReg代表区域编号
	bool QueryStartEndParmOfRow(unsigned Row,unsigned indexOfReg,unsigned& StartY,unsigned& EndY,float RatioOfPiexel,float RatioOfYPiexel)
	{
		if (m_vWorkRegionPationed[indexOfReg].GivenRowNeedPaint(Row))//只有当前列有需要喷涂的单元才会为TRUE
		{
			StartY=(m_vWorkRegionPationed[indexOfReg].ValidTopOfGivenRow(Row))-m_vWorkRegionPationed[indexOfReg].top;//运动参数相对于工作区的上边沿，与其坐标不同
			StartY*=RatioOfYPiexel;
			EndY=m_vWorkRegionPationed[indexOfReg].ValidBottomOfGivenRow(Row)-m_vWorkRegionPationed[indexOfReg].top;
			EndY*=/*RatioOfPiexel*/RatioOfYPiexel;
			return true;
		}		
		return false;
	}
/**
*@brief 查询工作区的矩形参数
*
*
*@parm
*
*@return
*@todo
*/
	CRect* QueryRegRectParm(unsigned indexOfReg)
	{
		return &m_vWorkRegionPationed[indexOfReg];
	}
	unsigned NumOfRowInGivenWorkReg(unsigned WorkReg)
	{
		return m_vWorkRegionPationed[WorkReg].NumOfRow;        //返回当前区域的列数
	}
	unsigned NumOfCellInGivenRowOfGivenReg(unsigned WorkReg,unsigned Row)
	{
		return m_vWorkRegionPationed[WorkReg].NumOfCellInGivenRow(Row);
	}
//bool CheckLocationInReg()
	/**
	*@brief 对给定点，查出在整个工作区序列中它所在区的编号
	*
	*
	*@parm  point 给定点坐标
	*
	*@return 给定点它所在区的编号
	*@todo
	*/
	unsigned LocGivenPointInRegs(CPoint & point)
	{
		unsigned PixelDisBetweenWorkReg=gbOnePixelBetweenWorkReg?1:0;
		unsigned LineNum=point.y/(CRect_Work_Region::HeightofWorkRegion+PixelDisBetweenWorkReg);//若-1是指与前面Partion函数中一致
		unsigned RowNum=point.x/(CRect_Work_Region::WidthOfWorkRegion+PixelDisBetweenWorkReg);
		return RowNum+LineNum*(CRect_Work_Region::RowNumOfWorkRegion);
		//ItOfRegVec ItofRegs=m_vWorkRegionPationed.begin();
		//while (ItofRegs!=m_vWorkRegionPationed.end())
		//{
		//	ItofRegs++;
		//}
	}
	

protected:
/**
*@brief 在地图的指定工作区中找到合法的方块区域，并将他们的信息记录
*
*在地图中找到合法的方块区域型的paintCell，并将他们的信息记录,外部调用本函数。每列中的一个方形只调用一次。
*要求保证首次调用时，当前点坐标为左上角（假设坐标系原点在整个矩形的左上角）
*@parm CurColor 当前点的颜色
*@parm x  当前点的x
*@parm y  当前点的y坐标
*@parm pPicDC 图片
*@parm CurReg 初步划分了列的工作区
*
*@return
*@todo
*/
	bool ValidateRegionInWorkRegion(COLORREF& CurColor,long x,long y,CDC*pPicDC,ItOfRegVec CurReg);
	/**
	*@brief 在地图的指定工作区中找到合法的方块区域，并将他们的信息记录,特别针对整齐固定所有尺寸的喷涂栅格，grid栅格
	*
	*在地图中找到合法的固定尺寸的方块区域型的paintCell，并将他们的信息记录。
	*要求保证首次调用时，当前点坐标为左上角（假设坐标系原点在整个矩形的左上角）
	*@parm CurColor 当前点的颜色
	*@parm x  当前点的x
	*@parm y  当前点的y坐标
	*@parm pPicDC 图片
	*@parm CurReg 初步划分了列的工作区
	*
	*@return
	*@todo
	*/
	bool ValidateGridRegionInWorkRegion(COLORREF& CurColor,long x,long y,CDC*pPicDC,ItOfRegVec CurReg);
public:
	/**
	*@brief 重置，或者说初始化函数
	*
	*重置，或者说初始化函数,将所有成员变量赋给零值，清空所有容器，防止出错
	*@parm
	*
	*@return
	*@todo
	*/
	void InitAll()
	{
		CPartionPicture::InitAll();
		m_vRowOfPaintCell.clear();
		m_CurCell.InitAll();
	}
protected:
	
	std::vector<CPaintCell> m_vRowOfPaintCell;///<喷涂单元列，记录一列之中所有的喷涂单元
	CPaintCell m_CurCell;

public:
	COLORREF& QueryColorByIndex(unsigned ColorIndex)
	{return m_CurCell.QueryColorByIndex(ColorIndex);}
	unsigned QueryNumOfColorToBePainted()
	{return m_CurCell.QueryNumOfColorToBePainted();	}
	bool FoundCellToBePaint()
	{
		ItOfRegVec ItofRegs=m_vWorkRegionPationed.begin();
		while (ItofRegs!=m_vWorkRegionPationed.end())
		{
			if (ItofRegs->m_bNeedPaint)
			{
				return true;
			}
			ItofRegs++;
		}
		return false;
	}
/**
*@brief 从给定坐标出发，设该坐标点为列的左上角，在一列中搜索矩形喷涂区域
*
*从给定坐标出发，设该坐标点为列的左上角，在一列中搜索矩形喷涂区域，将该矩形信息记录，并将整个一列中的所有喷涂信息记录到m_vRowOfPaintCell
*本函数中，并未考虑列宽度可能的不同，而是直接认为就是gRowWidth,且搜索沿列的起始列边进行，这就要求保证列宽度
*@parm pPicDC 带地图的DC用于提取地图
*@parm RowStartX 本列的起始点(左上)X坐标
*@parm RowStartY 本列的起始点(左上)Y坐标
*@parm CurReg    本列所在的工作区域的指针Iterator类型
*
*@return  找到合法的喷涂区域则返回真，否则返回假
*@todo
*/
	//bool SearchValidateRegionInRow(CDC*pPicDC,long RowStartX,long RowStartY,ItOfRegVec CurReg);
	/**
	*@brief 在一列中搜索矩形喷涂区域
	*
	*对指定列，在一列中搜索矩形喷涂区域，将该矩形信息记录，并将整个一列中的所有喷涂信息记录到m_vRowOfPaintCell
	*本函数中，列信息利用RowIndex，指定，且搜索沿列的竖直中线进行
	*@parm pPicDC 带地图的DC用于提取地图
	*@parm RowIndex 本列在工作区块中的索引
	*@parm CurReg    本列所在的工作区域的指针Iterator类型
	*
	*@return  找到合法的喷涂区域则返回真，否则返回假
	*@todo
	*/
bool SearchValidateRegionInRow(CDC*pPicDC,ItOfRegVec CurReg,unsigned RowIndex);
/**
*@brief 在一个栅格化的列中搜索固定大小的矩形喷涂区域
*
*对指定列，在一列中搜索矩形喷涂区域，将该矩形信息记录，并将整个一列中的所有喷涂信息记录到m_vRowOfPaintCell
*本函数中，列信息利用RowIndex，指定，且搜索沿列的竖直中线进行，与SearchValidateRegionInRow不同处在于本列中所有的栅格都是固定大小及单元的
*@parm pPicDC 带地图的DC用于提取地图
*@parm RowIndex 本列在工作区块中的索引
*@parm CurReg    本列所在的工作区域的指针Iterator类型
*
*@return  找到合法的喷涂区域则返回真，否则返回假
*@todo
*/

bool SearchValidateRegionInGridRow(CDC*pPicDC,ItOfRegVec CurReg,unsigned RowIndex);

/**
*@brief 找到喷涂区后，记录
*
*在一列的搜索中，已经找到了喷涂区域，找到了该喷涂区域的起始点(左上)沿Y轴方向搜索其占的大小，X方向默认为列宽，将结果记录到成员变量m_CurCell，最后再将它压入m_vRowOfPaintCell
*
*@parm CurColor 当前区域包含的颜色
*@parm pPicDC 带地图的DC用于提取地图
*@parm CurCellRect 本单元的信息
*@parm StartX 本喷涂单元的起始点的x坐标
*@parm StartY 本喷涂单元的起始点的Y坐标
*
*@return 该喷涂区域大于等于1个像素的宽度则返回真
*@todo
*/
	bool RecordPaintCell(COLORREF& CurColor,CDC*pPicDC,CRect& CurCellRect,long SearchStartX,long SearchStartY);
	/**
	*@brief 找到喷涂区后，记录
	*
	*在一列的搜索中，已经找到了喷涂区域，找到了该喷涂区域的起始点(左上)
	*喷涂区域实际上是固定的整齐的栅格，因此，无需再搜索其Y方向上的大小，其值为固定值
	*
	*@parm CurColor 当前区域包含的颜色
	*@parm pPicDC 带地图的DC用于提取地图
	*@parm CurCellRect 本单元的信息
	*@parm StartX 本喷涂单元的起始点的x坐标
	*@parm StartY 本喷涂单元的起始点的Y坐标
	*
	*@return 该喷涂区域大于等于1个像素的宽度则返回真
	*@todo
	*/
	bool RecordGridPaintCell(COLORREF& CurColor,CDC*pPicDC,CRect& CurCellRect,long SearchStartX,long SearchStartY);
};
/**
* @class 
* @brief 本类用于对图片进行预识别，识别出必须参数，比如喷涂单元的尺寸
*
* 
*/

#pragma once
/** @file 
*@brief 本文件用于存放各种处理识别结果的适配类，大部分与枪、人为设置等参数有关的内容在本文件。识别部分其实与大部分人工参数无关
* @author Yang GAO
* @date 
* @version 1.0
*
* 
*/
/**
* @class 
* @brief 将识别出来的随机的颜色顺序影射为机器需要的顺序
*
* 由人指定的序号，要与识别出的颜色一一匹配起来，并提供这种匹配的借口。
*/
#include <vector>
#include <map>
#include <algorithm>
#include "gyMath.h"
#include <math.h>
#define MAX_COLOR_MACHINE_INDEX 6 //最大可喷涂颜色数为MAX_COLOR_MACHINE_INDEX，但颜色编号范围为：0~MAX_COLOR_MACHINE_INDEX-1，MAX_COLOR_MACHINE_INDEX本身为无色
/**
*@brief 专门用于比较m_ColorIndexRecedMap中的第2参数的类似函数的一个东西,仿函数类.用于findif函数中，来找到指定的对象。这么用是Stl中findif要求的。也是种技巧
*
*
*@parm
*
*@return
*@todo
*/
template<class T>
class map_finder
{
public:
	map_finder(unsigned &cmp):Compaer(cmp){};
		const unsigned Compaer;
	bool operator ()(const T &pair)
	{   return pair.second == Compaer;} 

};
/**
* @class 
* @brief 本类用于将识别的颜色存储并映射到颜色编号,注意:本类中的机器编号，实际上也是后面多喷枪模式中的虚拟编号
*
* 
*/
class CColorResAdapter
{
public:
	typedef std::map<unsigned,unsigned> ColorResMapType;	
	typedef std::map<unsigned,unsigned>::value_type MValType;

	typedef std::map<unsigned,int> ColorResHexMapType;///<机器编号到PLC中的2进制映射图类型
	typedef std::map<unsigned,int>::value_type HexMValType;///<ColorResHexMapType中内容对应的类型
	CColorResAdapter( );
	~CColorResAdapter();
public:
/**
*@brief 颜色映射队列末尾添加一组映射
*
*颜色映射队列末尾添加一组映射，默认映射前后的值是一样的，没有映射后的值考虑重复的问题这里要注意
*
*@parm
*
*@return
*@todo
*/
	bool pushback(unsigned Index)
	{
		std::pair< ColorResMapType::iterator,bool > InserReslt;//map插入操作的结果
		InserReslt=m_ColorIndexRecedMap.insert(MValType(Index,Index));
		return InserReslt.second;
	}
	unsigned size()
	{
		return m_ColorIndexMachineHexMap.size();
	}
	/**
	*@brief 颜色到PLC2进制编码映射队列末尾添加一组映射
	*
	*颜色映射队列末尾添加一组映射，默认映射前后的值是一样的，没有映射后的值考虑重复的问题这里要注意
	*0号喷枪对应值0x01,1号对应0x02,....
	*@parm
	*
	*@return
	*@todo
	*/
	bool pushback_HexMap(unsigned Index)
	{
		std::pair<ColorResHexMapType::iterator,bool > InserReslt;//map插入操作的结果
		int Val=1;		
		if (Index==MAX_COLOR_MACHINE_INDEX)
		{Val=0;	}
		else
		{Val<<=Index;}
		InserReslt=m_ColorIndexMachineHexMap.insert(HexMValType(Index,Val));
		return InserReslt.second;
	}
	unsigned Compaer;

	
	
	
	
/**
*@brief 对给定的识别色，改变其对应的实际颜色编号(增加1方式)，
*
*这里的+1考虑了这样一个规则：输入的编号为图像识别中的编号，它所对应的机器颜色映射值+1，但是，
*前提是，更优先的图像编号所占用的映射值必须跳过。所以本函数可能还会改变比MapRecedIndex优先度更低的
*颜色所对应的机器颜色值。这些编号的优先度是按照更小的编号更优先的方式。
*@parm  MapRecedIndex 图像识别中赋予的颜色索引编号
*
*@return 成功的话返回真
*@todo
*/
	bool AddMachineIndexOnRule(unsigned MapRecedIndex)
	{
		ColorResMapType::iterator iter;
		iter = m_ColorIndexRecedMap.find(MapRecedIndex);
		if (iter==m_ColorIndexRecedMap.end())
		{	return false;	}//输入的有误，没有找到该编号
		unsigned AddedNum=1;
		ColorResMapType::iterator it;
		do 
		{
			unsigned AddedRes=RoundAddColorIndex(iter->second,AddedNum);
			iter->second=AddedRes;
			it = std::find_if(m_ColorIndexRecedMap.begin(),iter,map_finder<MValType>(AddedRes));
		} while (it!=iter);
		InfluenceLessImportanters(iter,m_ColorIndexRecedMap.end());
		
		return true;
	}
/**
*@brief 根据识别出来的颜色序号，查询该颜色对应的枪号，也就是机器编号
*
*
*@parm  MColorIndex 识别出来的颜色序号
*
*@return
*@todo
*/
	unsigned QueryMachineColorIndexByRecedIndex(unsigned MColorIndex)
	{
		if (MColorIndex>6)
		{
			MColorIndex=6;
		}
		ColorResMapType::iterator iter=m_ColorIndexRecedMap.find(MColorIndex);     //索引值最大为6
		//std::find_if(m_ColorIndexRecedMap.begin(),m_ColorIndexRecedMap.end(),map_finder<MValType>(AddedRes));
		//ColorResHexMapType::iterator iterHex=m_ColorIndexMachineHexMap.find(iter->second);
		return iter->second;               //返回存储对象
	}
	/**
	*@brief 根据枪号，也就是机器编号，查询该枪在PLC中的对应控制二进制编码
	*
	*
	*@parm  MColorIndex 机器编号，即面板上为每个颜色设置的数字
	*
	*@return
	*@todo
	*/
	unsigned QueryMachineHexMapByMachineColorIndex(unsigned MColorIndex)
	{
			ColorResHexMapType::iterator iterHex=m_ColorIndexMachineHexMap.find(MColorIndex);
			return iterHex->second;
	}
	unsigned QueryMachineHexMapByRecedIndex(unsigned MColorIndex)
	{
		unsigned MechineColor=QueryMachineColorIndexByRecedIndex( MColorIndex);
		return QueryMachineHexMapByMachineColorIndex( MechineColor);
	}
	/**
	*@brief 根据PLC中的对应控制二进制编码，反过来查询机器编号，即面板上为每个颜色设置的数字,注意:本类中的机器编号，实际上也是后面多喷枪模式中的虚拟编号
	*
	*
	*@parm  MhexValue 该颜色的二进制编码
	*
	*@return 找到返回该颜色的机器编码，及人工设置的数字。否则返回-1
	*@todo
	*/
	unsigned QueryMachineColorIndexByMachineHexValue(unsigned MhexValue)
	{
		ColorResHexMapType::iterator iterFound = std::find_if(m_ColorIndexMachineHexMap.begin(),m_ColorIndexMachineHexMap.end(),map_finder<MValType>(MhexValue));//找出该二进制值对应的机器编号
		return iterFound==m_ColorIndexMachineHexMap.end()?-1:iterFound->first;
	}
protected:
	/**
	*@brief 颜色 ComparedIt比CurIt的优先度更高吗？
	*
	*
	*@parm
	*
	*@return
	*@todo
	*/
	bool LessImportant(ColorResMapType::iterator CurIt,ColorResMapType::iterator ComparedIt)
	{return CurIt->first<ComparedIt->first;}
	
/**
*@brief 更改收到影响的低重要度值，存在递归调用
*
*
*@parm
*
*@return
*@todo
*/
	void InfluenceLessImportanters(ColorResMapType::iterator ChangedIter,ColorResMapType::iterator IterEnd)
	{
		if (ChangedIter==IterEnd)
		{return;}
		ColorResMapType::iterator CurIter=ChangedIter;
		CurIter++;
		while ( CurIter!=IterEnd)
		{
			CurIter= std::find_if(CurIter,IterEnd,map_finder<MValType>(ChangedIter->second));
			if (CurIter!=IterEnd)
			{
				AddMachineIndexOnRule(CurIter->first);///<递归调用
			}		
		}
	}
/**
*@brief 循环增加颜色编号
*
*该编号加了后大于 m_MaxColorNum的话，返回的是它除以m_MaxColorNum后的余数
*@parm
*
*@return
*@todo
*/
	unsigned RoundAddColorIndex(unsigned MapRecedIndex,unsigned AddNum=1)
	{
		unsigned res=MapRecedIndex+AddNum;
		return res>m_MaxColorNum?((res%m_MaxColorNum)-1):res;/*-*/;
	}
protected:
	ColorResMapType m_ColorIndexRecedMap;///<识别出的索引与机器所接的颜色顺序之间的影射，第一个元素为识别出的顺序，第2个元素为机器所需的顺序，存进去后会自动按关键字从小到大排序的
	unsigned m_MaxColorNum;					///<最大的颜色编号
	ColorResHexMapType m_ColorIndexMachineHexMap;///<由机器所接的颜色顺序编号，到PLC中2进制格式喷枪编号的映射。这才是PLC中真正的机器颜色编号，第一个元素为机器所需的顺序，第2个元素为PLC中颜色对应的2进制值
private:
};
typedef std::vector<float>::iterator MotionParm_It_TYPE;
/**
* @class 
* @brief 本类专用于由识别结果形成运动控制所需参数
*
* 
*/
template<class RecResType>
class MotionParmAdapter
{
public:
	MotionParmAdapter()
	{
		m_uGunNum=2;
		m_fGunDis=2000;
	};
	~MotionParmAdapter(){};
	typedef MotionParm_It_TYPE Parm_It_TYPE;
/**
*@brief 设置图片到实际的比例，
*
*
*@parm Ratio 为1个像素对应多少mm
*
*@return
*@todo
*/
	//void SetRatioOfPiexl(float Ratio)
	//{
	//	m_fRatioOfPixel=Ratio;
	//}
	/**
	 *@brief 将输入的参数格式化为传输数据中长度数据所需的格式
	 *
	 *将输入的参数格式化为传输数据中长度数据所需的格式,比如如需传输的所有长度参数单位为(个脉冲)时这里需要更改
	 *@parm
	 *
	 *@return
	 *@todo
	 */
	float FormatLengthParm(float parm,bool JudgeXOrY)             //2016年12月10日改
	{
		if (JudgeXOrY==true)         //X轴
		{
			return parm*m_fRatioOfPixel;
		}
		if (JudgeXOrY==false)      //Y轴
		{
			return parm*m_fRatioOfYPixel;
		}
		
	}
	/**
	*@brief 实际速度，
	*
	*
	*@parm Vel 实际运行速度 转/s
	*
	*@return
	*@todo
	*/
	void SetRealVel(float Vel)
	{
		m_sHeaderInfo.Vel=Vel;
	}
	void SetRealRatio(float RealRatio,float RealYRatio)
	{		
		m_fRatioOfPixel=RealRatio;	
		m_fRatioOfYPixel=RealYRatio;
	}
	void SetCellSizeReal()            //形成单元宽高实际值
	{
		m_sHeaderInfo.PaintWidth=FormatLengthParm(gRowWidth,true);
		m_sHeaderInfo.PaintHeigth=FormatLengthParm(gHeightOfEveryPaintCell,false);
	}
	/**
	*@brief 对外接口函数，导出参数序列
	*
	*
	*@parm WorkRegNum 需要到处哪个工作区中的参数>=0,当该值小于0时导出全部工作区的参数。
	*
	*@return 成功则为真
	*@todo
	*/
	Parm_It_TYPE ExportMotionParmVec(int WorkRegNum)
	{
		
		Formheader(pRecRes->NumOfRowInGivenWorkReg(WorkRegNum),m_sHeaderInfo.PaintHeigth,m_sHeaderInfo.PaintWidth,m_sHeaderInfo.Vel);
		if (WorkRegNum>=0)	{
			return QueryParmVec(WorkRegNum,m_fRatioOfPixel,m_fRatioOfYPixel);
		}
		else	{
			return  QueryAllRegParmVec(m_fRatioOfPixel);
		}
	
		//Formheader()
	}
	Parm_It_TYPE ExportMotionParmVecEnd(int WorkRegNum)
	{	 return QueryParmVecEnd(WorkRegNum);	}
/**
*@brief 为参数增加文件头，其中也是参数，不过是全局的一些参数
*
*
*@parm
*
*@return
*@todo
*/
	Parm_It_TYPE  InsertParmHeader()
	{
		m_vParmVec.insert(m_vParmVec.begin(),m_vParmHeader.begin(),m_vParmHeader.end());
		return m_vParmVec.begin();
	}
protected:
	/**
	*@brief 设置喷枪数及喷枪间距
	*
	*
	*@parm GunNum 喷枪数 <100
	*@parm GunDis 喷枪间距 单位毫米,<2000
	*
	*@return 设置正确则返回真
	*@todo
	*/
	bool SetPaintGunParm(unsigned GunNum,float GunDis)
	{
		if (GunNum>100||GunDis>2000)
		{return false;	}
		m_uGunNum=GunNum;
		m_fGunDis=GunDis;
		return true;
	}
	/**
	*@brief 导入识别结果
	*
	*
	*@parm
	*
	*@return 成功则为真
	*@todo
	*/
	bool ImportRecRes(RecResType* RecRes)
	{
		if (RecRes){	
			pRecRes=RecRes;            //pRecRes将拥有形参的一切
			return true;
		}		
		return false;
	}
	/**
	*@brief 对得到的运动范围进行扩充
	*
	*对得到的运动范围进行扩充，以补偿多喷枪安装,导致的部分喷枪无法走完全部喷涂范围的问题
	*这里不考虑运动范围扩充后是否超出机构的最大行程问题,这需要外部预先考虑到
	*@parm BorderTobeExtend 需要扩充的运动范围边界,mm
	*@parm ExtendDirect 扩充的方向，+方向为1，-方向为-1
	*
	*@return 扩充后的运动范围边界,mm
	*@todo
	*/
	float ExtendMotionRange(float BorderTobeExtend,int ExtendDirect)
	{
		return BorderTobeExtend+ExtendDirect*(m_uGunNum-1)*m_fGunDis;
	}
	/**
	*@brief 对指定的工作区形成其参数留用于输出，并返回指针
	*
	*先查询获得识别结果中每列的上下沿，然后考虑喷枪安装模式的不同进行扩充
	*
	*@parm BorderTobeExtend 需要扩充的运动范围边界,mm
	*@parm ExtendDirect 扩充的方向，+方向为1，-方向为-1
	*
	*@return 扩充后的运动范围边界,mm
	*@todo
	*/
	Parm_It_TYPE QueryParmVec(int WorkRegNum,float RatioOfPixel=m_fRatioOfPixel,float RatioOfYPixel=m_fRatioOfYPixel,bool bNewQuery=true)
	{
		unsigned StartY;
		unsigned EndY;
		if (bNewQuery)
		{	m_vParmVec.clear();	}
		
		bool bReserve=false;
		int Low=pRecRes->NumOfRowInGivenWorkReg(WorkRegNum);         //当前区域的列数
		for (unsigned i=0;i<pRecRes->NumOfRowInGivenWorkReg(WorkRegNum);i++)
		{
			if(pRecRes->QueryStartEndParmOfRow(i,WorkRegNum,StartY,EndY,RatioOfPixel,RatioOfYPixel))  //i代表列，WorkRegNum为在整个区域中当前在喷第几个区
			{
				m_vParmVec.push_back(i);//只有当前列有需要喷涂的小单元才记录
				if (bReserve) {
					EndY=ExtendMotionRange(EndY,1);//本应该扩充StartY的，但StartY=0时无法扩充，只好EndY，恰好结果一样
					m_vParmVec.push_back(EndY);
					m_vParmVec.push_back(StartY);				
				}
				else{		
					EndY=ExtendMotionRange(EndY,1);
					m_vParmVec.push_back(StartY);
					m_vParmVec.push_back(EndY);
				}
			}
			bReserve=!bReserve;
		}
		return m_vParmVec.begin();
	}
	Parm_It_TYPE QueryParmVecEnd(int WorkRegNum)
	{
		return m_vParmVec.end();
	}
/**
*@brief 形成公共部分参数，存入m_vParmHeader返回该部分的指针。
*
*
*@parm 
*
*@return
*@todo
*/
	Parm_It_TYPE Formheader(unsigned NumOfRowToBePaint,float PaintHeight,float PaintWidth,float Vel)
	{
		m_vParmHeader.clear();
		m_vParmHeader.push_back(NumOfRowToBePaint);
		m_vParmHeader.push_back(Vel);
		m_vParmHeader.push_back(PaintHeight);
		m_vParmHeader.push_back(PaintWidth);
		return m_vParmHeader.begin();
	}
	Parm_It_TYPE QueryHeader()
	{
		m_vParmHeader.begin();
	}
	Parm_It_TYPE QueryAllRegParmVec(float RatioOfPixel)
	{
		Parm_It_TYPE it;
		for (int i=0;i<pRecRes->NumOfRecedPic();i++)
		{	it=QueryParmVec(i,RatioOfPixel,false);	}
		return it;
	}
	RecResType* pRecRes;
protected:
public:
	std::vector<float>m_vParmVec;
	std::vector<float>m_vParmHeader;
	unsigned m_uGunNum;///<枪数
	float m_fGunDis;///<枪距离mm
	struct HeaderInfo
	{
		float Vel;///<移动实际速度
		float PaintWidth;///<喷涂单元实际宽度
		float PaintHeigth;///<喷涂单元实际高度
	}m_sHeaderInfo;///<存储参数头中的信息，与m_vParmHeader中的信息应完全一致
	float m_fRatioOfPixel;///<像素到实际mm的比例尺
	float m_fRatioOfYPixel;
};


#define PARM_USE_COLOR_INDEX_ONLY //控制宏，定义本宏则参数格式中仅适用颜色编号，而没有单元格编号。无色的补上无色单元
/**
* @class 
* @brief 将识别结果转化为PLC需要的格式，以方便通讯
*
* 
*/
template<class RecResType,class PaintCellType>
//定义模板就是如果一个类中数据成员的数据类型不能确定，或者是某个成员函数的参数或返回值的类型不能确定，
//就必须将此类声明为模板，它的存在不是代表一个具体的、实际的类，而是代表着一类类。
class CPLC_Parm_Adapter:public VirtualGunMapperForMultiColoGun// public CColorResAdapter
{

	
public:
	typedef std::vector<int>::iterator PLC_Parm_It_TYPE;
	typedef std::vector<int> PLC_Parm_TYPE;
	/**
	*@brief 导出参数序列
	*
	*
	*@parm WorkRegNum 需要到处哪个工作区中的参数>=0,当该值小于0时导出全部工作区的参数。
	*
	*@return 成功则为真
	*@todo
	*/
	 PLC_Parm_It_TYPE ExportPLC_ParmVec(int WorkRegNum)
	{
		Formheader(FormatLengthParm(gHeightOfEveryPaintCell));
		if (WorkRegNum>0)	{
			return QueryParmVec(WorkRegNum);
		}
		else	{
			return  QueryAllRegParmVec();
		}		
	}
	 PLC_Parm_It_TYPE ExportPLC_ParmVecEnd(int WorkRegNum)
	{	 return QueryParmVecEnd(WorkRegNum);	}
	 PLC_Parm_It_TYPE  InsertParmHeader()
	 {
		 m_vParmVec.insert(m_vParmVec.begin(),m_vParmHeader.begin(),m_vParmHeader.end());
		 return m_vParmVec.begin();
	 }
	 /**
	 *@brief 导入识别结果
	 *
	 *
	 *@parm pRec 识别结果类
	 *
	 *@return 成功则为真
	 *@todo
	 */
	 bool ImportRecRes(RecResType* pRec)                //形参不确定类型
	 {	if (pRec){
			 pRecRes=pRec;
			 return true;
		}
		return false;
	 }
	 RecResType * QueryMapRecResPointer(void)
	 {
		 return pRecRes;
	 }
	 void SetRealRatio(float Ratio,float YRatio)
	 {
		 m_fRealRatio=Ratio;
		 m_fRealYRatio=YRatio;
	 }
protected:
	

	
	PLC_Parm_It_TYPE QueryAllRegParmVec()
	{
		PLC_Parm_It_TYPE it;
		for (int i=0;i<pRecRes->NumOfRecedPic();i++)
		{	it=QueryParmVec(i);	}
		return it;
	}
	/**
	*@brief 生成参数头
	*
	*
	*@parm PaintCellHeight 单元的高度 单位毫米，并取整了
	*
	*@return
	*@todo
	*/
	PLC_Parm_It_TYPE Formheader(unsigned PaintCellHeight)
	{
		//m_vParmHeader.push_back();
		m_vParmHeader.clear();
		m_vParmHeader.push_back(PaintCellHeight);
		return m_vParmHeader.begin();
	}
	PLC_Parm_TYPE m_vParmHeader;///<公共参数组成的参数流头，依次包括单元格高度、
/**
*@brief 依据标准值，判断给定的方块是否是个正常方块，
*
*若方块高度超出给定值一定范围，则为假，否则为真
*@parm CurCellHeight方块的实际高度
*@parm StandardCellHeight 方块高度的比较基准
*
*@return 若是为真
*@todo
*/
	bool IsRegularCell(float CurCellHeight, float StandardCellHeight)
	{
		static float Torerant=(BIGEST_SIZE_RATIO_OF_ONE_CELL-1)*StandardCellHeight;
		return fabs(CurCellHeight-StandardCellHeight)<Torerant;
	}
/**
*@brief 记录指定的单元
*
*由于识别出的结果中只记录了需要喷涂的单元格，因此若单元格列中出现空白则须特殊处理
*按照单元格相对本列起点的位置确定方块编号，这样若列内突然出现空白的方格位置，即可以判断出来，并在参数序列中插入空白格
*对本工作区第一列的单元格，其前面全部填空白格。
*@parm WorkRegNum 当前单元所在工作区号
*@parm i 当前单元格的行号
*@parm j 当前单元格的列号
*@parm bfirstCellInRow 当前单元格在本列中是第一个
*@parm bfirstCellInRow 当前单元格在本列中是第一个
*@parm PreIndexOfGrid 前一单元格在本列中的编号
*@parm bFirstRow 当前单元格所属列是工作区内的第一列
*
*@return
*@todo
*/
	void RecordExactCellToParmVec(int WorkRegNum,unsigned i,unsigned j,bool& bfirstCellInRow,long& PreIndexOfGrid,bool & bFirstRow)
	{
		unsigned StartY;
		unsigned EndY;
		CRect* pRegRectParm;
		PaintCellType* pCell=pRecRes->QueryPaintCell(i,j,WorkRegNum);  //返回当前小单元格的位置
		pRegRectParm=pRecRes->QueryRegRectParm(WorkRegNum);
		StartY=(pCell->top)-(pRegRectParm->top);     //每一列的喷涂初始位置
		long IndexOfGrid=Floor4_Ceil5((float)StartY/gHeightOfEveryPaintCell);//列内编号
		if (bfirstCellInRow)		//每列的第一个元素要考虑初始化PreIndexOfGrid
		{	
			PreIndexOfGrid=IndexOfGrid;
			bfirstCellInRow=false;
			if (bFirstRow)//喷涂开始第一列，它的第一个元素之前全部填空白格
			{
				for (int i=0;i<IndexOfGrid;i++)	{
					//m_vParmVec.push_back(QueryMachineHexMapByRecedIndex(MAX_COLOR_MACHINE_INDEX));//填入空白色，关键只在第一列插入0
					;
				}
				bFirstRow=false;
			}
		}
#ifdef PARM_USE_COLOR_INDEX_ONLY
		long IndexDif=abs(IndexOfGrid-PreIndexOfGrid);//编号差
		if (IndexDif>1)	{
			for (int i=0;i<IndexDif-1;i++)	{
				m_vParmVec.push_back(QueryMachineHexMapByRecedIndex(MAX_COLOR_MACHINE_INDEX));//填入空白色
			}
		}
		PreIndexOfGrid=IndexOfGrid;
#else
		if (!IsRegularCell(pCell->Height(), gHeightOfEveryPaintCell))//超尺寸的方格，边界处可能出现
		{
			EndY=(pCell->bottom)-(pRegRectParm->top);
			m_vParmVec.push_back(-1);//列内编号
			m_vParmVec.push_back(FormatLengthParm(StartY));
			m_vParmVec.push_back(FormatLengthParm(EndY));
		}
		else
		{m_vParmVec.push_back(IndexOfGrid);}//列内编号}		
#endif	
	m_vParmVec.push_back(QueryMachineHexMapByRecedIndex(pCell->m_uColorIndex));//颜色编号
	}
/**
*@brief 查询需要传输的参数，仅考虑色块，未考虑枪数等内容
*
*参数的顺序如下：参数头，列内容。对每列比对了前后列的内容，并对需要的进行了插补。
*列内容依次包括：本列内需喷涂的单元个数，由上至下逐个单元信息。注意，每喷涂区域的第一列，把空白空格都添上了
*单元信息存在两种格式，PARM_USE_COLOR_INDEX_ONLY宏控制
*1则为只有颜色编号，目前默认为本格式
*2则为：《单元编号（非常规的话，单元编号为-1，并且紧跟该单元的起止位置）+颜色编号》
*例如参数列：4，0，0,1,1,2,1,2
*           它表示有两列：
*                  第一列有4个单元格，前两个颜色为0，后两个颜色为1.其中第一列把空白的都补成了颜色0的色块。
*                  第二列有2个单元格，颜色分别为1，2
*这里假定如出现超长喷涂单元，则该单元必为该列最后一个
*@parm
*
*@return
*@todo
*/
	PLC_Parm_It_TYPE QueryParmVec(int WorkRegNum)
	{
		bool bReserveOrder=false;///<是否逆过来记录顺序
		m_vParmVec.clear();
		long PreIndexOfGrid;///<本列前一个单元格在列中的编号
		bool bFirstRow=true;//是否从起点到该处的第一列
		m_vInsertedNullCellInRows.clear();
		std::vector<int>NumOfInsertedNullTmp;
		for (unsigned j=0;j<pRecRes->NumOfRowInGivenWorkReg(WorkRegNum);j++)//注意可能存在空白列,j为喷涂的列数
		{
			NumOfInsertedNullTmp.clear();
			NumOfInsertedNullTmp.push_back(j);//先加入列号，后面如果没有的话可能会删掉			
			//unsigned SizeOfInsertedNullCellVec=NumOfInsertedNullTmp.size();
		/*	pRecRes->QueryStartEndParmOfRow(i,WorkRegNum,StartY,EndY,RatioOfPixel);*/
			unsigned LinMax=pRecRes->NumOfCellInGivenRowOfGivenReg(WorkRegNum,j);//可能保存当前列中需要喷涂的单元	
			//////////////////////////////////////////////////////////////////////////
			int InsertedNum=0;
			//if (j!=pRecRes->NumOfRowInGivenWorkReg(WorkRegNum)-1){
			//	InsertedNum=CheckCurRowIfInsertNullColor(bReserveOrder,WorkRegNum,j,false,true);//检测队尾是否需要插补，但不插入
			//}
			//if (j!=0){
			//	InsertedNum+=CheckCurRowIfInsertNullColor(bReserveOrder,WorkRegNum,j,false,false);//检测队首是否需要插补，真实插入
			//}
			if (LinMax<=0)//空白行的话跳出
			{continue;}
			//////////////////////////////////////////////////////////////////////////
#ifdef PARM_USE_COLOR_INDEX_ONLY
			unsigned CurStartY;               //当前列的喷涂起始位置
			unsigned CurEndY;                 //当前列的喷涂终止位置
			pRecRes->QueryStartEndParmOfRow(j,WorkRegNum,CurStartY,CurEndY,1,1);
			unsigned NumOfCell=Floor4_Ceil5(float(CurEndY-CurStartY)/gHeightOfEveryPaintCell);     //当前列要喷涂的单元
			m_vParmVec.push_back(NumOfCell);		//压入列内需喷涂单元个数,该数值后面会更新
			unsigned IndexOfTotal=m_vParmVec.size();		// 本列单元总数的位置
#else			
			m_vParmVec.push_back(LinMax);			//压入列内需喷涂单元个数,该数值后面会更新
			unsigned IndexOfTotal=m_vParmVec.size();//本列单元总数的位置
#endif			
			//if (j!=0&&(((j%2)==1)?true:false))//后向监测一遍，即与前一列去比较
			if (j!=0)//后向监测一遍，即与前一列去比较，修改自上一行
			{    
				//InsertedNum+=CheckCurRowIfInsertNullColor_FixBlankRow(bReserveOrder,WorkRegNum,j,true,false);//非队首列，与后一列相比较的检测是否需要插补，真实插入
				InsertedNum=CheckCurRowIfInsertNullColor_FixBlankRow(bReserveOrder,WorkRegNum,j,true,false);//非队首列，与后一列相比较的检测是否需要插补，真实插入
				if (InsertedNum!=0)
				{
					NumOfInsertedNullTmp.push_back(InsertedNum);
				}				
				//SaveInsertedNullCell(InsertedNum);
			}
			bool bFirstCellInRow=true;			
			if (bReserveOrder)	{        //在列中逆着记录数据												
				if (LinMax>0){					
					for (int i=LinMax-1;i>=0;i--)//逐行记录,即逐单元格
					{						
						RecordExactCellToParmVec(WorkRegNum,i,j,bFirstCellInRow,PreIndexOfGrid,bFirstRow);
					}
				}				
			}
			else           //在列中顺着记录数据	
			{				
				for (unsigned i=0;i<LinMax;i++)//在列中逐行记录
				{
					RecordExactCellToParmVec(WorkRegNum,i,j,bFirstCellInRow,PreIndexOfGrid,bFirstRow);
				}
			}
			InsertedNum=0;
			if (j!=pRecRes->NumOfRowInGivenWorkReg(WorkRegNum)-1)	//前向监测一遍
			{
				InsertedNum=CheckCurRowIfInsertNullColor_FixBlankRow(bReserveOrder,WorkRegNum,j,true,true);////非队尾列，与后一列相比较的前向检测是否需要插补，真实插入
				//SaveInsertedNullCell(InsertedNum);
				if (InsertedNum!=0)
				{
					NumOfInsertedNullTmp.push_back(InsertedNum);
				}
			}
			if (NumOfInsertedNullTmp.size()>1)//加入了新的空白格
			{
				SaveInsertedNullCell(NumOfInsertedNullTmp);
			}
			unsigned RealSize=m_vParmVec.size();
			m_vParmVec[IndexOfTotal-1]=RealSize-IndexOfTotal;//更新总数
			bReserveOrder=!bReserveOrder;//有效列			
			bFirstRow=false;
		}
		return m_vParmVec.begin();
	}
/**
*@brief 检查本列是否需要插补空白格，注意，要求当前列的比较列不能是空白列，即列的前后不能有空白列，本版本函数即将废除
*
*检查本列是否需要插补空白格，检查后若是需要，则返回需要插补的空白格数，具体是否真正插补由 bInsert位控制
*如果需要将当前列于前一列比较，此时需要添加的空白格位置，一定是当前列数据队列的队首。
*如果需要与下一列比较，此时需要添加的空白格位置，一定是当前列的队尾。
*@parm bLookForward 是与下一排比较还是与上一排比较
*
*@return 本列中插入的单元格个数
*@todo 若列与列之间存在空白列，这时候在比较就会出问题，比如死循环。因为与空白列比较时，空白列就没有读出起始与结束值
*/
	unsigned CheckCurRowIfInsertNullColor(bool bReserveOrder,unsigned WorkRegNum,unsigned CurRowNum,bool bInsert,bool bLookForward)
	{
		unsigned StartY;
		unsigned EndY;
		unsigned StartY0;
		unsigned EndY0;
		pRecRes->QueryStartEndParmOfRow(CurRowNum,WorkRegNum,StartY,EndY,1);
		unsigned InsertedNum=0;
		int AddNum=bLookForward?1:-1;
		if (bLookForward){	//是与下一列比较，此时要添加的位置均为当前队列的末尾
			if (bReserveOrder)		{
				pRecRes->QueryStartEndParmOfRow(CurRowNum+AddNum,WorkRegNum,StartY0,EndY0,1);
				if (StartY<StartY0)	{//两列间存在差距，但当前列比下一列长，应该加到下一列中
					return 0;
				}
				InsertedNum=InsertNULLColor(StartY,StartY0,bInsert);				
			}
			else {			
				pRecRes->QueryStartEndParmOfRow(CurRowNum+AddNum,WorkRegNum,StartY0,EndY0,1);
				if (EndY>EndY0)	{//两列间存在差距，但当前列比下一列长，应该加到下一列中
					return 0;
				}
				InsertedNum=InsertNULLColor(EndY,EndY0,bInsert);	
			}
		}
		else{//是与上一列比较，此时要添加的位置均为当前队列的开头处
			if (bReserveOrder)		{
				pRecRes->QueryStartEndParmOfRow(CurRowNum+AddNum,WorkRegNum,StartY0,EndY0,1);
				if (EndY>EndY0)	{//两列间存在差距，但当前列比上一列长，应该加到上一列中
					return 0;
				}
				InsertedNum=InsertNULLColor(EndY,EndY0,bInsert);//EndY这才是数据队列首				
			}
			else {
				pRecRes->QueryStartEndParmOfRow(CurRowNum+AddNum,WorkRegNum,StartY0,EndY0,1);
				if (StartY<StartY0)	{//两列间存在差距，但当前列比上一列长，应该加到上一列中
					return 0;
				}				
				InsertedNum=InsertNULLColor(StartY,StartY0,bInsert);	
			}
		}
		return InsertedNum;
	}
	void SaveInsertedNullCell(std::vector<int>& itNum)
	{
		if (itNum.size()>0)
		{
			m_vInsertedNullCellInRows.push_back(itNum);
		}
	}
	bool IsNullCellInserted()
	{
		if (m_vInsertedNullCellInRows.size()>0)
		{
			return true;
		}
		return false;
	}
	std::vector<int>::iterator ReturnInsertedNullCell()
	{
		return m_vInsertedNullCellInRows.begin();
	}
	/**
	*@brief 检查本列是否需要插补空白格，第2种方式，解决列的前后不能有空白列的问题
	*
	*检查本列是否需要插补空白格，检查后若是需要，则返回需要插补的空白格数，具体是否真正插补由 bInsert位控制
	*如果需要将当前列于前一列比较，此时需要添加的空白格位置，一定是当前列数据队列的队首。
	*如果需要与下一列比较，此时需要添加的空白格位置，一定是当前列的队尾。
	*本函数改进如下，
	*@parm bLookForward 是与下一排比较还是与上一排比较
	*
	*@return 本列中插入的单元格个数,队列的开头处为负，队列的末尾为正
	*@todo
	*/
	int CheckCurRowIfInsertNullColor_FixBlankRow(bool bReserveOrder,unsigned WorkRegNum,unsigned CurRowNum,bool bInsert,bool bLookForward)
	{
		/*unsigned StartY;
		unsigned EndY;
		unsigned StartY0;
		unsigned EndY0;*/
		//pRecRes->QueryStartEndParmOfRow(CurRowNum,WorkRegNum,StartY,EndY,1);
		unsigned TotalRowNum=pRecRes->NumOfRowInGivenWorkReg(WorkRegNum);//总列数
		unsigned CompRow;//要与之比较的列
		int NumofInserted=0;
		//unsigned InsertedNum=0;
		int AddNum=bLookForward?1:-1;
		if (bLookForward){	//是与下一列比较，此时要添加的位置均为当前队列的末尾
			if (bReserveOrder)		{
				NumofInserted=CheckAndInsertNullCell(WorkRegNum,TotalRowNum, CurRowNum,AddNum,bInsert,true);				
			}
			else {	
				NumofInserted=CheckAndInsertNullCell(WorkRegNum,TotalRowNum, CurRowNum,AddNum,bInsert,false);	
			}
			return NumofInserted;
		}
		else{//是与上一列比较，此时要添加的位置均为当前队列的开头处
			if (bReserveOrder)		{
				NumofInserted=CheckAndInsertNullCell(WorkRegNum,TotalRowNum, CurRowNum,AddNum,bInsert,false);			
			}
			else {
				NumofInserted=CheckAndInsertNullCell(WorkRegNum,TotalRowNum, CurRowNum,AddNum,bInsert,true);	
			}			
			return -NumofInserted;
		}
		return NumofInserted;
	}
	/**
	*@brief 按给定的方向检查是否需要插入空白格
	*
	*检查本列是否需要插补空白格，检查后若是需要，则返回需要插补的空白格数，具体是否真正插补由 bInsert位控制
	*将当前列与目标列比较，目标列是：在有效的列数范围内，由当前列按照指定的方向搜索所得到的最接近的有效列
	*搜索方向是向前还是向后，由 AddNum的正负来控制，
	*当前列与目标列之间，是比较起始坐标，还是终点坐标，由bCompareStart控制，它为真则比较的是起始点坐标
	*
	*
	*@parm TotalRowNum 本工作单元的总列数
	*
	*@return 本列中插入的单元格个数
	*@todo
	*/
	unsigned CheckAndInsertNullCell(unsigned WorkRegNum,unsigned TotalRowNum,unsigned CurRowNum,int AddNum,bool bInsert,bool bCompareStart)
	{
		unsigned CompRow;//要与之比较的列
		unsigned StartY0;
		unsigned EndY0;
		unsigned StartY;
		unsigned EndY;
		unsigned InsertedNum=0;
		pRecRes->QueryStartEndParmOfRow(CurRowNum,WorkRegNum,StartY,EndY,1,1);
		CompRow=CurRowNum+AddNum;
		if (bCompareStart)	{
			for (CompRow;CompRow<TotalRowNum&&CompRow>=0;CompRow+=AddNum){
			if (pRecRes->QueryStartEndParmOfRow(CompRow,WorkRegNum,StartY0,EndY0,1,1)){
					if (StartY<StartY0)	{//两列间存在差距，但当前列比下一列长，应该加到下一列中
						return 0;
					}
					InsertedNum=InsertNULLColor(StartY,StartY0,bInsert);
					return InsertedNum;
				}//找到合适的比较对象则比较，否则跳出
			}	
		}
		else{
			for (CompRow;CompRow<TotalRowNum&&CompRow>=0;CompRow+=AddNum){
				if (pRecRes->QueryStartEndParmOfRow(CompRow,WorkRegNum,StartY0,EndY0,1,1)){
					if (EndY>EndY0)	{//两列间存在差距，但当前列比下一列长，应该加到下一列中
						return 0;
					}
					InsertedNum=InsertNULLColor(EndY,EndY0,bInsert);
					return InsertedNum;
				}//找到合适的比较对象则比较，否则跳出
			}	
		}
		return InsertedNum;
	}
/**
*@brief 检测是否需要插补空白单元，并返回需要插补的数目。bInsert控制是否真的插补，是的话，在m_vParmVec中的当前位置插入，否则不实际插补
*
*当前列的终止位置与下一列的起始位置若不在同一水平线上，意味着喷枪需要走过一段空行程，该段空行程还必须排除在
*控制喷枪开关的计数范围外。因此需要插入一个空白格，其颜色编号取6（正常颜色编号为0~5）意味着无色
*
*@parm CurY  当前列的Y坐标位置(像素)
*@parm CompY  与当前Y比较的Y坐标位置(像素)
*@parm bInsert 是否真正插入空格
*
*@return 是的话，在当前位置插入并返回真，否则返回假
*@todo
*/
	unsigned InsertNULLColor(unsigned CurY,unsigned CompY,bool bInsert=true)
	{
		const static float ErrorTorence=(unsigned)(gHeightOfEveryPaintCell*0.1);//容差，当误差在此范围内时认为无需插补
		//int Dis=(int)CurEndY-(int)NexStartY;
		float Dis=(float)CurY-(float)CompY;
		float DisOfY=fabs(Dis);
		if (DisOfY<ErrorTorence)
		{return 0;	}
		unsigned NumOfCellToInsert=Floor4_Ceil5((float)DisOfY/gHeightOfEveryPaintCell);
		if (!bInsert)
		{return NumOfCellToInsert;}
		for (unsigned i=0;i<NumOfCellToInsert;i++)//插入空白格
		{
#ifdef PARM_USE_COLOR_INDEX_ONLY
			m_vParmVec.push_back(QueryMachineHexMapByRecedIndex(MAX_COLOR_MACHINE_INDEX));
#else
			m_vParmVec.push_back(0);
			m_vParmVec.push_back(QueryMachineColorIndexByRecedIndex(MAX_COLOR_MACHINE_INDEX));
#endif
		}
		return NumOfCellToInsert;
	}
	 /**
	 *@brief 将输入的参数格式化为传输数据中长度数据所需的格式
	 *
	 *将输入的参数格式化为传输数据中长度数据所需的格式,默认目标格式为mm
	 *比如如需传输的所有长度参数单位为(个脉冲)时这里需要更改
	 *@parm
	 *
	 *@return
	 *@todo
	 */
	 float FormatLengthParm(float parm)         //单元高度
	 {
		 return parm*/*m_fRealRatio*/m_fRealYRatio;
	 }
	PLC_Parm_It_TYPE QueryParmVecEnd(int WorkRegNum)
	{
		return m_vParmVec.end();
	}
	
	
protected:
	PLC_Parm_TYPE m_vParmVec;///<参数留结果
	std::vector<std::vector<int>>m_vInsertedNullCellInRows;///<为避免斜向运动插入的空白格，格式为，列数，已插入格数。其中格数为正表示插在列尾，格数为负表示插在列头，且如有同时队尾和队头插补的话，必然是先队头、后队尾。这里列头指的是本列运动起始的地方
	RecResType * pRecRes;///<识别结果对象的指针
	float m_fRealRatio;///<每个像素对应毫米的比例值
	float m_fRealYRatio; //每个像素对应毫米的比例值
};
/**
* @class 
* @brief 本类专用于在喷枪的实际编号与颜色识别编号之间添加一道虚拟喷枪，这样来实现多色喷枪。
*CColorResAdapter类中提供了依据喷枪机器编号查询PLC所需的参数的功能，然而该类是为单色喷枪设计，即每一喷枪对应一色。
*由于需考虑多把多色喷枪的情况，在此加入本类，引入了虚拟单色喷枪号的概念，即喷枪实际编号(0开始)对应多个虚拟喷枪号(0开始)，再用虚拟喷枪号则可以利用CColorResAdapter类去
*查询PLC所需参数
*
* 
*/
class VirtualGunMapperForMultiColoGun: public CColorResAdapter
{
public:
	VirtualGunMapperForMultiColoGun()
	{
		m_uColoPerGun=m_uSeledGunMode=1;
	}
	/**
	*@brief 为满足多色喷枪的应用，进行了映射，本函数基于颜色号，查询出真实的喷枪号
	*
	*实际喷枪可能一枪多色，虚拟单喷枪是一枪一色，这两者间存在映射关系,
	*多色时，0号喷枪对应前m_uColoPerGun个虚拟喷枪，1号对应随后的m_uColoPerGun个，以此类推...
	*
	*@parm Color 颜色编号，0号颜色对应0号虚拟喷枪，1号对应1号。。。这里注意，颜色是2进制表示，即0号颜色编号是2的0次即1,1号颜色是2的1次即2
	*@return 返回-1的话表示没有枪能对应上，否则返回的是对应的喷枪号
	*@todo
	*/
	unsigned QueryRealGunNumOnColor(unsigned Color)
	{
		int virtualGunNum;
		switch(m_uSeledGunMode)
		{
		case 2://一把多色喷枪
			if (Color<m_uColoPerGun){
				return 0;
			}
		case 3://多把多色喷枪
			virtualGunNum=QueryMachineColorIndexByMachineHexValue(Color);
			return virtualGunNum==-1?-1:floor((float)virtualGunNum/m_uColoPerGun);
		case 1:// 多把单色
			return Color;
		}
		return -1;
	}
	/**
	*@brief 检测指定的颜色是否属于指定的喷枪
	*
	*实际喷枪可能一枪多色，虚拟单喷枪是一枪一色，这两者间存在映射关系,
	*多色时，0号喷枪对应前m_uColoPerGun个虚拟喷枪，1号对应随后的m_uColoPerGun个，以此类推...
	*
	*@parm Color 指定颜色的颜色编号，这里注意，颜色是2进制表示，即0号颜色编号是2的0次即1,1号颜色是2的1次即2
	*@parm RealGunNum 指定的真实喷枪编号
	*@return 是的话返回真，否则返回假
	*@todo
	*/
	bool IsCurColorBelongToGivenGun(unsigned TheColor, unsigned RealGunNum)
	{
		if (m_uSeledGunMode==1)// 多把单色,直接依据枪好查色，对比颜色是否相同
		{
			return TheColor==QueryMachineHexMapByMachineColorIndex(RealGunNum);
		}
		return RealGunNum==QueryRealGunNumOnColor(TheColor)?true:false;//否则依据颜色倒查抢号是否相同
	}
	unsigned HowManyVirtualGunUsed()
	{

	}
protected:
	unsigned m_uSeledGunMode;///<枪的模式，多把单色1，一把多色2，还是多把多色3
	unsigned m_uColoPerGun;///<每把枪的颜色数
};

/**
* @class 
* @brief 将识别结果转化为PLC需要的格式，以方便通讯,本类中把输出数据的格式更加复杂
*输出所有需要改变状态的位置及当时所有喷枪的状态
*
* 
*/
template<class RecResType,class PaintCellType>
class CPLC_Parm_ComplexType_Adapter:public CPLC_Parm_Adapter<RecResType,PaintCellType>
{
public:
	CPLC_Parm_ComplexType_Adapter()
	{
		m_uGunNum=1;
		m_fGunDis=200;
		

	}
	/**
	*@brief 导出参数序列，准备好参数头m_vComplexParmHeaderVec，但并不把参数头与参数序列组合起来。
	*
	*
	*@parm WorkRegNum 需要到处哪个工作区中的参数>=0,当该值小于0时导出全部工作区的参数。
	*
	*@return 成功则为真
	*@todo
	*/
	PLC_Parm_It_TYPE ExportComplexPLC_ParmVec(int WorkRegNum)
	{
		FormComplexheader(FormatLengthParm(gHeightOfEveryPaintCell));
		if (WorkRegNum>=0)	{
			return QueryComplexParmVec(WorkRegNum);
		}
		else	{
			return  QueryAllRegComplexParmVec();
		}		
	}
	PLC_Parm_It_TYPE ExportComplexPLC_ParmVecEnd(int WorkRegNum)
	{
		return m_vComplexParmVec.end();
	}
	/**
	*@brief 当已经形成参数流之后，在头部插入已经准备好的参数头。m_vComplexParmHeaderVec
	*
	*
	*@todo
	*/
	PLC_Parm_It_TYPE  InsertComplexParmHeader()
	{
		m_vComplexParmVec.insert(m_vComplexParmVec.begin(),m_vComplexParmHeaderVec.begin(),m_vComplexParmHeaderVec.end());
		return m_vComplexParmVec.begin();
	}
	
	/**
	*@brief 设置喷枪数及喷枪间距
	*
	*
	*@parm GunNum 喷枪数 <100
	*@parm GunDis 喷枪间距 单位毫米,<2000
	*
	*@return 设置正确则返回真
	*@todo
	*/
	bool SetPaintGunParm(unsigned GunNum,float GunDis,int GunMod=1,unsigned ColoNumPerGun=1)
	{
		if (GunNum>100||GunDis>2000)
		{return false;	}
		m_uGunNum=GunNum;
		m_fGunDis=GunDis;
		m_uColoPerGun=ColoNumPerGun;
		m_uSeledGunMode=GunMod;
		return true;
	}
	/**
	*@brief 计算出所有可能需要改变喷枪状态的位置
	*
	*计算出所有可能需要改变喷枪状态的位置，这些位置均为整除一个单元格尺寸后的余数。这样才符合PLC的计数方式
	*这里是以第一把喷枪作为基准的。第一把喷枪即从上往下运动为正向运动的话首先进入的喷枪。
	*@parm
	*
	*@return 存在这种位置则返回真，否则返回假
	*@todo
	*/
	bool CalculateAllPossibleStateChangeCoordinate()
	{
		m_vChangeStateCoordVec.clear();
		m_vChangeStateRoundCoordVec.clear();
		m_fCellHeightInMM=FormatLengthParm(gHeightOfEveryPaintCell);  //单元格高
		if (m_uGunNum>0)
		{
			m_vChangeStateCoordVec.push_back(0);//0为单元开始处
			m_vChangeStateRoundCoordVec.push_back(0);
			for (unsigned i=1;i<m_uGunNum;i++)
			{
				float CoodVal=(i*m_fGunDis)/m_fCellHeightInMM;
				m_vChangeStateCoordVec.push_back(CoodVal);
				CoodVal= CoodVal-floor(CoodVal);//	fmod((i*m_fGunDis),m_fCellHeightInMM);
				std::vector<float>::iterator itFindExist=find(m_vChangeStateRoundCoordVec.begin(),m_vChangeStateRoundCoordVec.end(),CoodVal);
				if (itFindExist==m_vChangeStateRoundCoordVec.end())//没重复的
				{
					m_vChangeStateRoundCoordVec.push_back(CoodVal);
				}				
			}
			//std::sort(m_vChangeStateCoordVec.begin(),m_vChangeStateCoordVec.end());
			std::sort(m_vChangeStateRoundCoordVec.begin(),m_vChangeStateRoundCoordVec.end());
		}
		return false;
	}
	/**
	*@brief 查询需要传输的参数
	*
	*参数的顺序如下：参数头，列内容。首先通过调用 QueryParmVec函数形成了简单化的本列内容列表，然后依据该函数结果
	*计算出每把喷枪对应的状态，从而形成PLC需要的参数。
	*列内容依次包括：本列内需喷涂的单元个数，由上至下逐个单元信息。
	*其中单元信息包括：《单元编号（非常规的话，单元编号为-1，并且紧跟该单元的起止位置）+颜色编号》
	*这里假定如出现超长喷涂单元，则该单元必为该列最后一个
	*@parm
	*
	*@return
	*@todo
	*/
	PLC_Parm_It_TYPE QueryComplexParmVec(int WorkRegNum)
	{
#ifdef PARM_USE_COLOR_INDEX_ONLY//目前仅考虑了此种情况下的实现
		m_vComplexParmVec.clear();
		//unsigned CountOfCurCell=0;///<当前单元在本列中的第几个有效单元
		PLC_Parm_It_TYPE SimpleTypeParmVecItBeg= QueryParmVec(WorkRegNum);//先形成简单格式的列内容，后续将以本结果为依据
		PLC_Parm_It_TYPE SimpleTypeParmVecItEnd= QueryParmVecEnd(WorkRegNum);
		TRACE("初步形成PLC数据正常");
		unsigned NumOfCellInRow;
		bool bReverse=false;//逆向移动的列
		unsigned RowBeginIndex;//列起点序列号
		for (SimpleTypeParmVecItBeg;SimpleTypeParmVecItBeg!=SimpleTypeParmVecItEnd;SimpleTypeParmVecItBeg++)//逐个色块
		{		
			NumOfCellInRow=*SimpleTypeParmVecItBeg;//列内参数个数，当前列有多少个单元格		
			m_vComplexParmVec.push_back(NumOfCellInRow); 
			RowBeginIndex=m_vComplexParmVec.size();//列参数中，本喷涂列所有参数的个数（这一项）对应在本列位置序号+1，后面需要依据本序号来更新该项的数值。
			//SimpleTypeParmVecItBeg++;
			//SimpleTypeParmVecItBeg=CheckStatusInRowOfParmVec(SimpleTypeParmVecItBeg,NumOfCellInRow,bReverse);
			SimpleTypeParmVecItBeg=CheckStatusInRowOfParmVec_Fix(SimpleTypeParmVecItBeg,NumOfCellInRow,bReverse);
			m_vComplexParmVec.push_back(0);//每列处理完成后人为+个0
			NumOfCellInRow=m_vComplexParmVec.size()-RowBeginIndex;//重新更新本列中参数个数，例如本位值为4，表示后面跟着的4个参数均为本列的参数。
			m_vComplexParmVec[RowBeginIndex-1]=NumOfCellInRow;
			bReverse=!bReverse;
			//SimpleTypeParmVecItBeg++;
		}
#else//这种情况暂不考虑，直接返回简单版本的结果
		return QueryParmVec(WorkRegNum);
#endif
		
		return m_vComplexParmVec.begin();
	}
/**
*@brief 在已经准备好的简单格式数据列中，划分出一列，并逐个单元格检查记录喷枪状态，然后指针停在列末尾的单元处
*
*
*@parm CurIt 简单色块参数序列的指针，是简单化QueryParmVec函数的结果。
*@parm NumOfCellInRow 当前列中的色块个数
*@parm bReverse 喷枪在本列是逆向移动？？是的为真。
*
*@return
*@todo
*/
	//PLC_Parm_It_TYPE CheckStatusInRowOfParmVec(PLC_Parm_It_TYPE CurIt,unsigned NumOfCellInRow,bool bReverse)
	//{
	//	//unsigned MaxIndexOf

	//	std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();
	//	for (unsigned i=0;i<NumOfCellInRow;i++)	{//注意i为基准点所走过的单元格数目0开始算起，0表示基准点还在编号为0的单元范围内
	//		CurIt++;               //单元格各喷枪状态
	//		CheckGunStatus(CurIt,i,bReverse,itBeg);			
	//	}
	//	if (m_uGunNum<2)
	//	{return CurIt;}
	//	//对于多喷枪来说下面的程序有问题
	//	/////////////////////////结尾部分，喷枪列有部分超出范围，需单独处理/////////////////////////////////////////////////
	//	unsigned NumOfCellOfNewBaseGun=ceil(m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0]);//新基准喷枪在老基准喷枪(老基准点为m_vChangeStateCoordVec[0])所在方格后的第几个方格内,注意要-1
	//	PLC_Parm_It_TYPE CurCellOfNewBaseGun=CurIt-NumOfCellOfNewBaseGun+1;  //得到第二把喷枪所在喷涂单元颜色
	//	unsigned NumOfCellToEnd= NumOfCellOfNewBaseGun;                      //新基准若不是恰好在一个方格的起点的话找出新基准对应的突变点
	//	float GunDis=m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0];
	//	float BaseGunStartPoint= GunDis-floor( GunDis);//起点对应突变点,2017年2月18日改
	//	std::vector<float>::iterator StartPointIt=m_vChangeStateRoundCoordVec.begin();
	//	float SecondGunPosition=(*(m_vChangeStateRoundCoordVec.end()-1))-GunDis+floor(m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0]);//第二把喷枪的位置
	//	//得到第二把喷枪的位置，有可能在倒数第二个喷涂单元里,还必须得到当第一把喷枪到达边界点时，第二把喷枪的状态。
	//	float SecondGunPositionWhenFirstGunArrive=SecondGunPosition+1-(*(m_vChangeStateRoundCoordVec.end()-1));
	//	std::vector<float>::iterator itBegAgain=m_vChangeStateRoundCoordVec.begin();
	//	std::vector<float>::iterator itBegAgainll;
	//	for (itBegAgain;itBegAgain!=m_vChangeStateRoundCoordVec.end();itBegAgain++)
	//	{
	//		if (*itBegAgain-SecondGunPositionWhenFirstGunArrive>0)
	//		{
	//			break;            //得到当第一把喷枪到达极限位置时，下一时刻第二把喷枪应从那个突变点开始计算
	//		}
	//	}
	//	for (int m_GunIndex=1;m_GunIndex<m_uGunNum;m_GunIndex++)         //每把喷枪都要经历
	//	{
	//		for (int i=0;i<=NumOfCellOfNewBaseGun-1;i++)       //判断每把喷枪还要经历几个单元格
	//		{
	//			PLC_Parm_It_TYPE m_CurrentSecondGunColor=CurCellOfNewBaseGun+i;     //当前单元颜色
	//			//CheckGunStatus中第一个参数代表当前喷涂单元的颜色，第二个参数代表当前喷涂单元的列号，第三个参数代表
	//			//方向，第四个参数代表从第几个突变点查起，第五个参数代表从第几把枪开始检查其状态
	//			CheckGunStatus(m_CurrentSecondGunColor,NumOfCellInRow-(NumOfCellOfNewBaseGun-1)+i,bReverse,itBegAgain-1,m_GunIndex);
	//		}
	//	}
	//	
	//	return CurIt;
	//}
	/**
	*@brief 在已经准备好的简单格式数据列中，划分出一列，并逐个单元格检查记录喷枪状态，然后指针停在列末尾的单元处
	*
	* CheckStatusInRowOfParmVec函数的改进版本，改用补虚拟色块的方法计算颜色
	*@parm CurIt 简单色块参数序列的指针，是简单化QueryParmVec函数的结果。
	*@parm NumOfCellInRow 当前列中的色块个数
	*@parm bReverse 喷枪在本列是逆向移动？？是的为真。
	*
	*@return
	*@todo
	*/
	PLC_Parm_It_TYPE CheckStatusInRowOfParmVec_Fix(PLC_Parm_It_TYPE CurIt,unsigned NumOfCellInRow,bool bReverse)
	{
		PLC_Parm_TYPE ExpandedCellVector;//用来扩充的色块序列
		//unsigned MaxIndexOf
		std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();//一个色块内的第0个突变点的指针
		for (unsigned i=0;i<NumOfCellInRow;i++)	{//注意i为基准点所走过的单元格数目0开始算起，0表示基准点还在编号为0的单元范围内
			CurIt++;
				CheckGunStatus(CurIt,i,bReverse,itBeg);	
				ExpandedCellVector.push_back(*CurIt);//拷贝
		}
		if (m_uGunNum<2)
		{return CurIt;}
		
		/////////////////////////结尾部分，喷枪列有部分超出范围，新建一个色块序列，插入虚拟的无色序列/////////////////////////////////////////////////
		//unsigned NumOfCellOfNewBaseGun=ceil(m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0]);//第二把喷枪做新基准喷枪在老基准喷枪(老基准点为m_vChangeStateCoordVec[0])所在方格后的第几个方格内,注意要-1，从1开始
		//PLC_Parm_It_TYPE CurCellOfNewBaseGun=CurIt-NumOfCellOfNewBaseGun+1;//万一减完超限怎么办????有问题
		//unsigned NumOfCellToEnd= NumOfCellOfNewBaseGun;//距跑出喷涂区还有多少个格，从0开始算起
		//////新基准若不是恰好在一个方格的起点的话找出新基准对应的突变点
		//float GunDis=m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0];
		unsigned VirtualExpandedCellNum=ceil(m_fGunDis*(m_uGunNum-1)/m_fCellHeightInMM);//从1开始
		
		for (int i=0;i<VirtualExpandedCellNum;i++)
		{
			ExpandedCellVector.push_back(0);//压入无色色块
		}
		PLC_Parm_It_TYPE itVirtualBorder=ExpandedCellVector.begin()+NumOfCellInRow;//扩充的起始处
		float AllGunDis=m_vChangeStateCoordVec[m_uGunNum-1]-m_vChangeStateCoordVec[0];
		float RoundCoord=fmod(AllGunDis,1);
		std::vector<float>::iterator	itFound;
			itFound=std::find(m_vChangeStateRoundCoordVec.begin(),m_vChangeStateRoundCoordVec.end(),RoundCoord);
		int Num=itFound-m_vChangeStateRoundCoordVec.begin();//距离，相差几个元素
		unsigned MaxSizeOfComplexParmVecInOneColummn=m_vComplexParmVec.size()+(VirtualExpandedCellNum-1)*m_uGunNum+Num+1;
		for (int i=0;i<VirtualExpandedCellNum;i++)
		{
			CheckGunStatus(itVirtualBorder,NumOfCellInRow+i,bReverse,itBeg,0,false,MaxSizeOfComplexParmVecInOneColummn);	//逐个虚拟色块判断
			itVirtualBorder++;
		}
		
		//float BaseGunStartPoint= GunDis-floor( GunDis);//第二把喷枪作为起点对应的突变点
		//PLC_Parm_It_TYPE itEnd=CurIt+1;//色块的结尾
		/////////////////////////////需要考虑的喷枪少了一把，形成新的突变点序列/////////////////////////////////////////////
		
		return CurIt;
	}
	/**
	*@brief 在已经准备好的简单格式数据列中，划分出一列，并逐个单元格检查记录喷枪状态，然后指针停在列末尾的单元处
	*
	*结尾部分，喷枪列有部分超出范围，需单独处理，此部分有bug，本函数废弃!!!!!!!!!!!!!!!!!!!!!
	*@parm CurIt 简单色块参数序列的指针，是简单化QueryParmVec函数的结果。
	*@parm NumOfCellInRow 当前列中的色块个数
	*@parm bReverse 喷枪在本列是逆向移动？？是的为真。
	*
	*@return
	*@todo
	*/
	PLC_Parm_It_TYPE CheckStatusInRowOfParmVec(PLC_Parm_It_TYPE CurIt,unsigned NumOfCellInRow,bool bReverse)
	{
		//unsigned MaxIndexOf
		std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();//一个色块内的第0个突变点的指针
		for (unsigned i=0;i<NumOfCellInRow;i++)	{//注意i为基准点所走过的单元格数目0开始算起，0表示基准点还在编号为0的单元范围内
			CurIt++;
	/*		if (bReverse)
			{CheckGunStatus(CurIt,i,bReverse,itBeg,m_uGunNum-1);	
			}
			else*/
				CheckGunStatus(CurIt,i,bReverse,itBeg);			
		}
		if (m_uGunNum<2)
		{return CurIt;}
		/////////////////////////结尾部分，喷枪列有部分超出范围，需单独处理/////////////////////////////////////////////////
		unsigned NumOfCellOfNewBaseGun=ceil(m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0]);//第二把喷枪做新基准喷枪在老基准喷枪(老基准点为m_vChangeStateCoordVec[0])所在方格后的第几个方格内,注意要-1，从1开始
		PLC_Parm_It_TYPE CurCellOfNewBaseGun=CurIt-NumOfCellOfNewBaseGun+1;//万一减完超限怎么办????有问题
		unsigned NumOfCellToEnd= NumOfCellOfNewBaseGun;//距跑出喷涂区还有多少个格，从0开始算起
		////新基准若不是恰好在一个方格的起点的话找出新基准对应的突变点
		float GunDis=m_vChangeStateCoordVec[1]-m_vChangeStateCoordVec[0];
		float BaseGunStartPoint= GunDis-floor( GunDis);//第二把喷枪作为起点对应的突变点
		PLC_Parm_It_TYPE itEnd=CurIt+1;//色块的结尾
		/////////////////////////////需要考虑的喷枪少了一把，形成新的突变点序列/////////////////////////////////////////////
		std::vector<float>::iterator StartPointIt=m_vChangeStateCoordVec.begin()+1;//第二把喷枪的坐标17.8.15修改
		//////////////////////////////////////////////////////////////////////////
		//std::vector<float>::iterator StartPointIt=find(m_vChangeStateRoundCoordVec.begin(),m_vChangeStateRoundCoordVec.end(),BaseGunStartPoint);
		
		for (unsigned i=1;i<m_vChangeStateCoordVec.size();i++)//注意i为超出喷涂范围的枪数目，还在喷涂范围内的枪挨个作为新的基准喷枪，当所有枪都出去了，则遍历完毕
		{
			NumOfCellToEnd=NumOfCellOfNewBaseGun;
			for (PLC_Parm_It_TYPE itNew=CurCellOfNewBaseGun;itNew!=itEnd;itNew++)//当前基准喷枪所在的色块为当前色块，从当前的色块开始往后遍历(这里不用考虑运动方向)
			{//注意，由于喷枪间距固定，不管是哪把喷枪作为新基准，它做新基准时都是从CurCellOfNewBaseGun这个色块，这个突变点开始的。
				//unsigned CurCellIndex=NumOfCellInRow-1-NumOfCellToEnd;//从运动开始方向算起，当前这个色块是第几个色块，从0开始标号
				
				unsigned CurCellIndex=NumOfCellInRow-NumOfCellToEnd;//从运动开始方向算起，当前这个色块是第几个色块，从0开始标号，17.8.15修改
				if (itNew==CurCellOfNewBaseGun)	{//第一个方块可能不完整，从中间某个突变点开始，会导致问题
					//int FunctionGunNum=m_vChangeStateCoordVec.size()-i;//还剩下的喷枪数
					//vector<float>NewRoundPointCoord;

					CheckGunStatus(itNew,CurCellIndex,bReverse,StartPointIt,i,true);
				}
				else{
					CheckGunStatus(itNew,CurCellIndex,bReverse,itBeg,i);//对当前色块内的突变点，判断状态
				}				
				NumOfCellToEnd--;
			}			
		}
		return CurIt;
	}
	std::vector<float>::iterator ReturnChangedColorPointBegin()
	{
		return m_vChangeStateRoundCoordVec.begin();
	}
	std::vector<float>::iterator ReturnChangedColorPointEnd()
	{
		return m_vChangeStateRoundCoordVec.end();
	}
	/**
	*@brief 形成参数头，实际上参数头即为每个喷涂单元的高度，单位mm
	*
	*
	*@parm PaintCellHeight 每个喷涂单元的高度，单位mm
	*
	*@return
	*@todo
	*/
	PLC_Parm_It_TYPE FormComplexheader(unsigned PaintCellHeight)
	{
		m_vComplexParmHeaderVec.clear();
		m_vComplexParmHeaderVec.push_back(PaintCellHeight);
		return m_vComplexParmHeaderVec.begin();
	}
	PLC_Parm_It_TYPE QueryAllRegComplexParmVec()
	{
		PLC_Parm_It_TYPE it;
		RecResType *pRecRes2=QueryMapRecResPointer(); 
		for (int i=0;i<pRecRes2->NumOfRecedPic();i++)
		{	it=QueryComplexParmVec(i);	}
		return it;
	}
protected:
	
/**
*@brief 对给定的喷涂单元，确定其内部所有状态变化点处的喷枪状态值并记录进参数队列
*
*@parm SimpleTypeParmVecItBeg 给定的喷涂单元(即假定基准喷枪在该喷涂单元内，逐个检测出所有可能状态变化点处的喷枪状态)
*@parm IndexOfCurCell 该喷涂单元是目前为止基准点所在的位置，从运动开始算起的第几个喷涂单元？)
*@parm itBeg  从这个色块内的哪个突变点开始判断？大部分时候应从第0个，当起始位置在色块内某个位置时则不是,这时应输入的突变点是没有圆整的突变点集中的点
*@parm bReverse 该列是逆向移动？是则为真
*@parm StartOfGunIndex 从第几把枪开始检查其状态，当所有喷枪均在喷涂范围内时为0，有几把喷枪不在则为几，与运动方向无关,枪号从0开始
*@return
*@todo
*/
	void CheckGunStatus(PLC_Parm_It_TYPE SimpleTypeParmVecItBeg,unsigned IndexOfCurCell,bool bReverse,std::vector<float>::iterator itBeg,unsigned StartOfGunIndex=0,const bool bStartFromMiddle=false,const unsigned MaxCheckPointNum=99999)
	{
		//SimpleTypeParmVecItBeg当前单元格中单元格颜色对应的喷枪
		//std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();
		int NumOfValidCell=0;//还在颜色范围内的喷涂单元个数
		unsigned GunNum=m_vChangeStateCoordVec.size();
		unsigned CurGunIndex;
		int GunAddStep=1;
		//if (bReverse)
		//{GunAddStep=-1;	}//17.8.15修改
		std::vector<float>::iterator itBegCopy=itBeg;
		std::vector<float>::iterator itChangPointVecEnd;
		if (bStartFromMiddle)//从中间某个突变点开始
		{
			itChangPointVecEnd=m_vChangeStateCoordVec.end();
		}
		else
		{
			itChangPointVecEnd=m_vChangeStateRoundCoordVec.end();
		}
		for (itBegCopy;itBegCopy!=itChangPointVecEnd;itBegCopy++)                    //从起始状态变化点开始遍历所有状态变化点位置
		{
			unsigned GunStateResult=0;
			for (unsigned indexOfGun=StartOfGunIndex;indexOfGun<GunNum;indexOfGun=indexOfGun+GunAddStep)    //从基准喷枪开始，逐个喷枪的检查
			{//当从中间某个突变点开始时，会出错，例如如果突变点是0，0.2，0.6，如果从0.6开始是喷枪2，0.2实际上是喷枪3，对应的实际突变点是1.2，这时就会导致遍历会跳过喷枪3

				//IndexOfGun为喷枪的编号,StartOfGunIndex为0，itBeg颜色突变点
				////////////////////////////////NumOfValidCell存在bug，可能出现负数//////////////////////////////////////////
				//NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfGun]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
				//NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfGun]-m_vChangeStateCoordVec[StartOfGunIndex]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
				float StartRoundCheckPoint;//起始喷枪在色块内的圆整突变点坐标
				if (bStartFromMiddle)
				{
					StartRoundCheckPoint=1-((*itBegCopy)-floor(m_vChangeStateCoordVec[StartOfGunIndex]));//当前的起点过了上一个色块多少
				}
				else
					StartRoundCheckPoint=(*itBegCopy);
				int NumOfCellOfCurGUN=ceil(m_vChangeStateCoordVec[indexOfGun]-m_vChangeStateCoordVec[StartOfGunIndex]-StartRoundCheckPoint);//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个色块内，它也代表距离基准喷枪的色块数
				//int NumOfCellOfCurGUN=floor(m_vChangeStateCoordVec[indexOfGun]-m_vChangeStateCoordVec[StartOfGunIndex]+StartRoundCheckPoint);//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个色块内，它也代表距离基准喷枪的色块数,从0开始，0代表同在一个色快
				if (NumOfCellOfCurGUN<=IndexOfCurCell)       //这个喷枪在有颜色的有效范围内,以防止这把喷枪还远着，没进入喷涂区则不需要判断，与运动方向无关
				{//IndexOfCurCells是从运动方向数过来当前色块的序号，代表了从基准喷枪倒推回去有多少个色块是有颜色的，
					//当前喷枪距离基准喷枪的色块数就必须小于IndexOfCurCell，否则这把喷枪还没进入喷涂区
					//GunStateResult=0;SimpleTypeParmVecItBeg为颜色
					PLC_Parm_It_TYPE GunInCellColor=SimpleTypeParmVecItBeg-NumOfCellOfCurGUN;   //当前查看喷枪所在色块(对应的PLC编号)
					if (bReverse){
						CurGunIndex=GunNum-indexOfGun-1;//当前枪的实际号码
					}
					else
					{
						 CurGunIndex=indexOfGun;//当前枪的实际号码
					}									
					//if (*GunInCel lColor==QueryMachineHexMapByMachineColorIndex(CurGunIndex))//专用于多把单色喷枪
					//if()//兼容多色喷枪的方案
					//if (IsCurColorBelongToGivenGun(*GunInCellColor,CurGunIndex)) //改过,2017年2月18日
					if(GunNum==1)    //单色喷枪绝对对
					{
						GunStateResult=GunStateResult|(*GunInCellColor);//将该枪置位
					}
					else if(*GunInCellColor==QueryMachineHexMapByMachineColorIndex(CurGunIndex)) //喷枪编号与颜色
					{
						GunStateResult=GunStateResult|(*GunInCellColor);//将该枪置位
					}
				}
			}
			m_vComplexParmVec.push_back(GunStateResult);
			if (m_vComplexParmVec.size()>=MaxCheckPointNum)//数目够了
				return;
			
		}		
	}
	/**
	*@brief 对给定的喷涂单元，确定其内部所有状态变化点处的喷枪状态值并记录进参数队列，专为从中间的某个突变点开始检查设计，改自CheckGunStatus
	*
	*@parm SimpleTypeParmVecItBeg 给定的喷涂单元(即假定基准喷枪在该喷涂单元内，逐个检测出所有可能状态变化点处的喷枪状态)
	*@parm IndexOfCurCell 该喷涂单元是目前为止基准点所在的位置，从运动开始算起的第几个喷涂单元？)
	*@parm itBeg  从这个色块内的哪个突变点开始判断？这里的突变点指没有圆整的突变点集中的点
	*@parm bReverse 该列是逆向移动？是则为真
	*@parm StartOfGunIndex 从第几把枪开始检查其状态，当所有喷枪均在喷涂范围内时为0，有几把喷枪不在则为几，与运动方向无关,枪号从0开始
	*@return
	*@todo
	*/
	//void CheckGunStatusForStartGivenPoint(PLC_Parm_It_TYPE SimpleTypeParmVecItBeg,unsigned IndexOfCurCell,bool bReverse,std::vector<float>::iterator itBeg,unsigned StartOfGunIndex=0)
	//{
	//	//SimpleTypeParmVecItBeg当前单元格中单元格颜色对应的喷枪
	//	//std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();
	//	int NumOfValidCell=0;//还在颜色范围内的喷涂单元个数
	//	unsigned GunNum=m_vChangeStateCoordVec.size();
	//	unsigned CurGunIndex;
	//	int GunAddStep=1;
	//	if (bReverse)
	//	{GunAddStep=-1;	}//17.8.15修改
	//	for (itBeg;itBeg!=m_vChangeStateRoundCoordVec.end();itBeg++)                    //从起始状态变化点开始遍历所有状态变化点位置
	//	{
	//		unsigned GunStateResult=0;
	//		for (unsigned indexOfGun=StartOfGunIndex;indexOfGun<GunNum;indexOfGun=indexOfGun+GunAddStep)    //从基准喷枪开始，逐个喷枪的检查
	//		{//当从中间某个突变点开始时，会出错，例如如果突变点是0，0.2，0.6，如果从0.6开始是喷枪2，0.2实际上是喷枪3，对应的实际突变点是1.2，这时就会导致遍历会跳过喷枪3

	//			//IndexOfGun为喷枪的编号,StartOfGunIndex为0，itBeg颜色突变点
	//			////////////////////////////////NumOfValidCell存在bug，可能出现负数//////////////////////////////////////////
	//			//NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfGun]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
	//			//NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfGun]-m_vChangeStateCoordVec[StartOfGunIndex]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
	//			int NumOfCellOfCurGUN=ceil(m_vChangeStateCoordVec[indexOfGun]-m_vChangeStateCoordVec[StartOfGunIndex]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个色块内，它也代表距离基准喷枪的色块数
	//			if (NumOfCellOfCurGUN<=IndexOfCurCell)       //这个喷枪在有颜色的有效范围内,以防止这把喷枪还远着，没进入喷涂区则不需要判断，与运动方向无关
	//			{//IndexOfCurCell代表从运动方向数过来当前色块的序号，当前喷枪距离基准喷枪的色块数就必须小于IndexOfCurCell，否则这把喷枪还没进入喷涂区
	//				//如果NumOfValidCell<=IndexOfCurCell说明当前查找的喷枪还没进入喷涂单元格内
	//				//GunStateResult=0;IndexOfCurCell为喷涂单元的列号,SimpleTypeParmVecItBeg为颜色
	//				PLC_Parm_It_TYPE GunInCellColor=SimpleTypeParmVecItBeg-NumOfCellOfCurGUN;   //当前查看喷枪所在色块(对应的PLC编号)
	//				if (bReverse){
	//					CurGunIndex=GunNum-indexOfGun-1;//当前枪的实际号码
	//				}
	//				else
	//				{
	//					CurGunIndex=indexOfGun;//当前枪的实际号码
	//				}									
	//				//if (*GunInCel lColor==QueryMachineHexMapByMachineColorIndex(CurGunIndex))//专用于多把单色喷枪
	//				//if()//兼容多色喷枪的方案
	//				//if (IsCurColorBelongToGivenGun(*GunInCellColor,CurGunIndex)) //改过,2017年2月18日
	//				if(GunNum==1)    //单色喷枪绝对对
	//				{
	//					GunStateResult=GunStateResult|(*GunInCellColor);//将该枪置位
	//				}
	//				else if(*GunInCellColor==QueryMachineHexMapByMachineColorIndex(CurGunIndex)) //喷枪编号与颜色
	//				{
	//					GunStateResult=GunStateResult|(*GunInCellColor);//将该枪置位
	//				}
	//			}
	//		}
	//		m_vComplexParmVec.push_back(GunStateResult);
	//	}		
	//}
	/**
	*@brief 对给定的喷涂单元，确定其内部所有状态变化点处的喷枪状态值并记录进参数队列,新版本，专为兼容多色喷枪准备
	*
	*@parm SimpleTypeParmVecItBeg 给定的喷涂单元(即假定基准喷枪在该喷涂单元内，逐个检测出所有可能状态变化点处的喷枪状态)
	*@parm IndexOfCurCell 该喷涂单元是目前为止基准点所在的位置(从运动开始算起的第几个喷涂单元？)
	*@parm bReverse 该列是逆向移动？是则为真
	*@parm StartOfGunIndex 从第几把枪开始检查其状态，当所有喷枪均在喷涂范围内时为0，有几把喷枪不在则为几，与运动方向无关,枪号从0开始
	*@return
	*@todo
	*/
	void CheckMultiColoGunStatus(PLC_Parm_It_TYPE SimpleTypeParmVecItBeg,unsigned IndexOfCurCell,bool bReverse,std::vector<float>::iterator itBeg,unsigned StartOfGunIndex=0)
	{
		//std::vector<float>::iterator itBeg=m_vChangeStateRoundCoordVec.begin();
		int NumOfValidCell=0;//还在颜色范围内的喷涂单元个数
		unsigned GunNum=m_vChangeStateCoordVec.size();
		unsigned CurGunIndex;
		unsigned RealGunNum;
		for (itBeg;itBeg!=m_vChangeStateRoundCoordVec.end();itBeg++)//假设基准喷枪在逐个状态变化点位置
		{
			unsigned GunStateResult=0;
			for (unsigned indexOfVirtualGun=StartOfGunIndex;indexOfVirtualGun<this->size();indexOfVirtualGun++)//逐个喷枪的检查
			{
				//RealGunNum=
				////////////////////////////////NumOfValidCell存在bug，可能出现负数//////////////////////////////////////////
				//NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfGun]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
				NumOfValidCell=ceil(m_vChangeStateCoordVec[indexOfVirtualGun]-m_vChangeStateCoordVec[StartOfGunIndex]-(*itBeg));//喷枪在基准喷枪(基准点为m_vChangeStateCoordVec[StartOfGunIndex])所在方格后的第几个方格内
				if (NumOfValidCell<=IndexOfCurCell)//这个喷枪在有颜色的有效范围内
				{
					PLC_Parm_It_TYPE GunInCellColor=SimpleTypeParmVecItBeg-NumOfValidCell;//该喷枪所在色块(对应的PLC编号)
					if (bReverse){
						CurGunIndex=GunNum-indexOfGun-1;//当前枪的实际号码
					}else{
						CurGunIndex=indexOfGun;//当前枪的实际号码
					}									
					//if (*GunInCellColor==QueryMachineHexMapByMachineColorIndex(CurGunIndex))//专用于多把单色喷枪
					//if()//兼容多色喷枪的方案
					if (IsCurColorBelongToGivenGun(*GunInCellColor,CurGunIndex))
					{
						GunStateResult=GunStateResult|(*GunInCellColor);//将该枪置位
					}					
				}
			}
			m_vComplexParmVec.push_back(GunStateResult);
		}		
	}
	std::vector<float>m_vChangeStateCoordVec;///<有可能需要改变喷枪状态的坐标值，到此位置则要检验各喷枪状态，这些位置均为除以一个单元格尺寸后的浮点倍数，突变点坐标
	std::vector<float>m_vChangeStateRoundCoordVec;///<有可能需要改变喷枪状态的坐标值，到此位置则要检验各喷枪状态，这些位置均为除以一个单元格尺寸后的余数相对单元格尺寸的倍速(),突变点坐标的整数部分
	PLC_Parm_TYPE m_vComplexParmVec;///<复杂格式的参数流结果
	PLC_Parm_TYPE m_vComplexParmHeaderVec;///<参数头序列
public:
	unsigned m_uGunNum;///<枪数目，不管是单色还是多色的。

	float m_fGunDis;///<枪距
	float m_fCellHeightInMM;///<单元高度，单位mm
};
#define USE_COMPLEX_PLC_PARM_TYPE
/**
* @class 
* @brief 将识别结果转化为下位机需要的格式
*
* 
*/
template<class RecResType,class PaintCellType>
class CResultAdapter:public  MotionParmAdapter<RecResType>,public CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>
{
public:
	CResultAdapter(void)
	{	//ImportRecRes(RecRes);	
		m_fRatioOfPixel=1;
	}
	~CResultAdapter(void)
	{	}
	Parm_It_TYPE InsertHeaderOfMotionParm()
	{
		return InsertParmHeader();
	}
	Parm_It_TYPE ExportMotionParmVec(int WorkRegNum)//注意，该函数必须在ExporPLCParmVec后调用，否则就不能考虑到插入的空白点带来的运动的影响
	{
		
		MotionParmAdapter<RecResType>::ExportMotionParmVec(WorkRegNum);		
		return MotionParmAdapter<RecResType>::InsertParmHeader();
	}
	/*!
 *@brief 因为准备PLC数据时可能为了不走斜线在列头或尾部插补了方块，因此需要相应的扩充运动范围信息,这里由Direction控制是沿正向运动列还是负向运动列
 *
 *@parm RowNum 列号
 *@parm itInserted 所有插入的栅格信息，格式为列号，起点插入栅格数，终点插入栅格数。注意这里itInserted里面起点的扩充都是负数，终点的扩充都是输入的正数以示区别
 *@parm itParm 已经准备好的运动参数 
 *@parm Direction 沿Y增加方向运动时为1，否则为-1
 *
 *@return
 *@todo
 */
	void ExtendOneRowMotionWithDirection(std::vector<std::vector<int>>::iterator itInserted,MotionParm_It_TYPE itParm,int Direction)
	{
		if ((*itInserted).size()>2)//多于两个参数，先队头，后队尾
		{
			itParm++;
			(*itParm)+=Direction*(*itInserted)[1]*m_sHeaderInfo.PaintHeigth;//终点扩充
			itParm++;//队尾
			(*itParm)+=Direction*(*itInserted)[2]*m_sHeaderInfo.PaintHeigth;//起点扩充,
		}
		else//只有一个扩充
		{
			if ((*itInserted)[1]<0)//单独队头
			{
				itParm++;				
			}
			else//队尾
			{
				itParm=itParm+2;
			}
			(*itParm)+=Direction*(*itInserted)[1]*m_sHeaderInfo.PaintHeigth;//终点扩充
		}
	}
/*!
 *@brief 因为准备PLC数据时可能为了不走斜线在列头或尾部插补了方块，因此需要相应的扩充运动范围信息,必须在形成了其余运动参数后最后调用
 *
 *@parm RowNum 列号
 *@parm itInserted 所有插入的栅格信息，格式为列号，起点插入栅格数，终点插入栅格数。注意这里itInserted里面起点的扩充都是负数，终点的扩充都是输入的正数以示区别
 *@parm itParm 已经准备好的运动参数 
 *
 *@return
 *@todo
 */
	void ExtendOneRowMotion(std::vector<std::vector<int>>::iterator itInserted,MotionParm_It_TYPE itParm,unsigned DisToFirstRow)
	{
		if (DisToFirstRow%2==0)//0开始，距离为偶数的列是向下移动，它的起点扩充是减正数栅格，终点扩充是+正栅格
		{			
			ExtendOneRowMotionWithDirection(itInserted,itParm,1);			
		}
		else//偶数列，它的起点扩充是+正数栅格，终点扩充是-正栅格
		{
			ExtendOneRowMotionWithDirection(itInserted,itParm,-1);
		}
	}
/*!
 *@brief 因为准备PLC数据时可能为了不走斜线在列头或尾部插补了方块，因此需要相应的扩充运动范围信息,必须在形成了其余运动参数后最后调用
 *
 *
 *@return
 *@todo
 */
	void ExtendMotionRangeForInsertedNullCell()
	{
		//MotionParmAdapter<RecResType>::m_vParmVec;
		
		if (CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::IsNullCellInserted())
		{
			unsigned HeadSize=MotionParmAdapter<RecResType>::m_vParmHeader.size();
			std::vector<std::vector<int>>::iterator itInserted=CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::m_vInsertedNullCellInRows.begin();
			for (itInserted;itInserted!=CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::m_vInsertedNullCellInRows.end();itInserted++)
			{
				int RowNum=(*itInserted)[0];
				MotionParm_It_TYPE itParm=find(MotionParmAdapter<RecResType>::m_vParmVec.begin(),MotionParmAdapter<RecResType>::m_vParmVec.end(),RowNum);//这里在已形成的参数序列里找列号，这就要求列号不能恰好和某个起终点坐标相同
				/*MotionParm_It_TYPE itEnd=MotionParmAdapter<RecResType>::m_vParmVec.end()--;*/
				//如果重了，比如第0列，起点是0，终点也是，第一列终点也是0，则找的是第一个符合的位置，是正确的
				int Distance=(itParm-MotionParmAdapter<RecResType>::m_vParmVec.begin()-HeadSize);
				long nDis=Floor4_Ceil5( Distance%3);
				while(nDis!=0&&itParm!=MotionParmAdapter<RecResType>::m_vParmVec.end())//在有效范围内，如果是列号，则一定是在参数序列中位于3的整数倍位置。
				{
					itParm++;
					itParm=find(itParm,MotionParmAdapter<RecResType>::m_vParmVec.end(),RowNum);//这里在已形成的参数序列里找列号
					Distance=(itParm-MotionParmAdapter<RecResType>::m_vParmVec.begin()-HeadSize)%3;
					nDis=Floor4_Ceil5( Distance);
				}
				if (itParm==MotionParmAdapter<RecResType>::m_vParmVec.end())//没找到这个列
				{
					return;
				}
				int RowDisToFirstRow=Floor4_Ceil5(Distance/3);//走过的第几列了
				ExtendOneRowMotion(itInserted,itParm,RowDisToFirstRow);				
			}			
		}
	}
	/*!
	 *@brief 生成并输出PLC所需要的数据，注意必须
	 *
	 *@parm
	 *@parm
	 *
	 *@return
	 *@todo
	 */
	 PLC_Parm_It_TYPE ExporPLCParmVec(int WorkRegNum)
	 {
		 CPLC_Parm_Adapter<RecResType,PaintCellType>::ExportPLC_ParmVec(WorkRegNum);
		 //ExtendMotionRangeForInsertedNullCell();
		 return CPLC_Parm_Adapter<RecResType,PaintCellType>::InsertParmHeader();
	 }
/**
*@brief 把图像识别的结果导入进来用于识别运动参数，PLC控制参数
*
*@return
*@todo
*/
	 void ImportRecedRes(RecResType* RecRes)
	{
		MotionParmAdapter<RecResType>::ImportRecRes(RecRes);
		CPLC_Parm_Adapter<RecResType,PaintCellType>::ImportRecRes(RecRes);
#ifdef USE_COMPLEX_PLC_PARM_TYPE
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::ImportRecRes(RecRes);
#endif
	}
public:
//#ifdef USE_COMPLEX_PLC_PARM_TYPE
	void SetParmOfGunForParm(unsigned GunNum=2,float GunDis=200)
	{
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::SetPaintGunParm(GunNum,GunDis);
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::CalculateAllPossibleStateChangeCoordinate();
		MotionParmAdapter<RecResType>::SetPaintGunParm(GunNum,GunDis);
	}
	void SetParmOfGunForParm(unsigned GunNum=2,float GunDis=200,int GunMod=1,unsigned ColoNumPerGun=1)
	{
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::SetPaintGunParm(GunNum,GunDis,GunMod, ColoNumPerGun);
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::CalculateAllPossibleStateChangeCoordinate();
		MotionParmAdapter<RecResType>::SetPaintGunParm(GunNum,GunDis);
	}
	//CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType> m_cComplexTypePlcParmAdapter;///<复杂格式的PLC数据转换器
/**
*@brief 准备好指定工作区内的参数流，并将其头部迭代器暴露出去。
*
*
*@parm WorkRegNum 工作区编号
*
*@return 参数流的头部迭代器
*@todo
*/
	PLC_Parm_It_TYPE ExporComplexPLCParmVec(int WorkRegNum)
	{
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::ExportComplexPLC_ParmVec(WorkRegNum);

		return 	CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::InsertComplexParmHeader();
	}
	PLC_Parm_It_TYPE ExportComplexPLC_ParmVecEnd(int WorkRegNum)
	{
		return CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::ExportComplexPLC_ParmVecEnd(WorkRegNum);
	}
//#endif
	/**
	*@brief 设置素到实际mm的比例尺系数
	*
	*
	*@parm
	*
	*@return 设置成功返回真
	*@todo
	*/
	bool SetRatioOfPixel(float RatioOfPixel,float RatioOfYPixel)
	{
		if (RatioOfPixel<=0||RatioOfYPixel<=0)
		{ 
			return false;		
		}
		m_fRatioOfPixel=RatioOfPixel;
		m_fRatioOfYPixel=RatioOfYPixel;
		MotionParmAdapter<RecResType>::SetRealRatio(m_fRatioOfPixel,m_fRatioOfYPixel);
		MotionParmAdapter<RecResType>::SetCellSizeReal();//必须设置好实际尺寸
		CPLC_Parm_Adapter<RecResType,PaintCellType>::SetRealRatio(m_fRatioOfPixel,m_fRatioOfYPixel);
#ifdef USE_COMPLEX_PLC_PARM_TYPE
		CPLC_Parm_ComplexType_Adapter<RecResType,PaintCellType>::SetRealRatio(m_fRatioOfPixel,m_fRatioOfYPixel);
#endif
		return true;
	}
	/**
	*@brief 实际速度，
	*
	*
	*@parm Vel 实际运行速度 转/s
	*
	*@return
	*@todo
	*/
	void SetRealVel(float Vel)
	{
		//m_sHeaderInfo.Vel=Vel;
		MotionParmAdapter<RecResType>::SetRealVel(Vel);
	}
protected:
	float m_fRatioOfPixel;///<每个像素对应毫米的比例值
	float m_fRatioOfYPixel; ///在Y轴上每个像素对应毫米的比例值
	float m_fRealVel;///<实际运行速度单位转/s
};

#pragma once
#include "iostream"
#include <vector>
#include <utility>
#include "ResultAdapter.h"
#include "afx.h"
#include ".\pmac\runtime.h"
#include "gyMath.h"
#include "Parm.h"
#define LINESPEED 24000        //当喷枪直线运行时的速度
#define SPEEDTRANSLATE 600
#define MAXPOINT 1400
typedef std::vector<int> VectorType;

struct GunModelAndDistance
{
	int GunNember;
	float BetweenGunDistance;
};
struct GunCondition
{
	bool OneCondition;
	bool TwoCondition;
	bool ThreeCondition;
	bool FourCondition;
	bool FiveCondition;
	bool SixCondition;
};
struct CameraParm
{
	float Angle_Correct;
	float X_Correct;
	float Y_Correct;
};
struct PerGunCondition
{
	int Index;
	GunCondition EveryPerGunCondition;
};
struct GunPerBeginAndEnd
{
	int Index;
	int Begin;
	int End;
};
struct CornerPoint          //记录每一次转角的XY坐标
{
	int XPoint;
	float YPoint;
};
struct CoordinateFrame
{
	bool IsRequireCanGet;  //能不能喷涂
	float BeginXPoint;
	float BeginYPoint;
	float FrameRange;
};
typedef std::vector<std::pair<int,float>> CheckPointPairVecType;
class CheckPointMap_finder
{
	public:
		CheckPointMap_finder(const float &cmp_string):m_s_cmp(cmp_string){}
		bool operator ()(const std::map<float, int>::value_type &pair)
		{
		      return pair.first == m_s_cmp;
		}
		private:
		const float &m_s_cmp;                    
};
class DownValue
{
public:
	DownValue();
	~DownValue();
	void GetDownValueMotion(MotionParm_It_TYPE MotionBegin,MotionParm_It_TYPE MotionEnd);
	void GetDownValuePlc(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis);
	
	void GetDownValuePlc2(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis);

	void GetDownCameraValue(CString CameraParm);
	/*void GetDownMotionSpeed(int MotionSpeed);*/
	CString CompileGunCondition(int Parm);
	CString GetStringPaser_star(CString gps_str,int num);
	void StartOpen(CString Directory);
	void StartOpenPlc(CString Directory,CoordinateFrame m_CoordinateFrame,float GunDelay[][2]);
	void CheckHeight(float Height);
	void GetGunModelAndGunDistance(int GunNember,float BetweenGunDistance);//得到喷枪模式和喷枪之间的距离
	void TranslateEveryChangeColor(int BeginPoint,int EndPoint);  //查找颜色突变点的坐标
	void TranslateEveryChangePoint(unsigned m_uGunNum,float m_fGunDis);                             //查找每一个位置突变点
	//////////////////////////////////////////////////////////////////////////
	void TranslateOneColumm(unsigned CurColumIndex,float MoveRangeEnd,float MoveRangeBegin,int CellToBeRun,bool bisForward);
	void TranslateEveryChangePoint2(unsigned m_uGunNum,float m_fGunDis);                             //查找每一个位置突变点

private:
	GunCondition m_Gundition;
	int PerLaw;      //代表当前工作区域有几列
	std::vector<PerGunCondition> m_EveryPerCondition;
	std::vector<GunPerBeginAndEnd> m_GunPerBeginAndEnd;
	std::map<int,VectorType> GunColorAll;        //用map来表示喷枪状态的二维数组，行代表列，列代表各列的喷枪状态
	typedef std::vector<CornerPoint> VectorTypePoint;
	VectorTypePoint m_VectorTypePoint;
	std::map<int,VectorTypePoint> AllChangeColorPosition;
	VectorType m_EveryColumnGunColor;               //每列喷枪的状态
	CornerPoint m_CornerPoint;                   //记录每一次转角坐标
	int m_CornerPointColor;                      //记录每一次转角的喷枪状态
	bool JudgeIsFirstCorner;
	float Speed;
	int PerWidth;
	int PerHeight;
	int m_MotionSpeed;
	bool JudgeReturn;
	bool JudgeChangeDirection;
	int HeightPer;       //每列的高度
	int LastRemberY;
	bool JudgeFirstLow;
	CameraParm m_CameraParm;
	int m_JudgeOneChoice;
	GunModelAndDistance m_GunModelAndDistance;
	std::vector<int> m_ColumnXYPointEverySholdJudgeColor; //在每一列中存储每一个需要检测喷枪颜色突变点的坐标
	int GunMenber; //当选择多喷枪模式时代表喷枪数
	std::vector<float>::iterator m_ChangedColorPointHead;
	std::vector<float>::iterator m_ChangedColorPointHeadCopy;
	std::vector<float>::iterator m_ChangedColorPointHeadCopyl;
	std::vector<float>::iterator m_ChangedColorPointEnd;
	int OldColor;      //记录上一颜色突变点的颜色值
	bool JudgeIsOrFirstPer;
protected:
	bool WriteDelayParm(float OpenDelay, float CloseDelay,FILE* fp);
	bool WriteAllDelayParm(float GunDelay[][2],FILE*fp,unsigned GunNum=4)
	{
		
		for (unsigned i=0;i<GunNum;i++)
		{
			//float fopen=GunDelay[i][0];

			WriteDelayParm( GunDelay[i][0], GunDelay[i][1],fp);
		}
		return true;
	}
/*!
 *@brief 给文本文件写入所有突变点及信息
 *
 *@parm
 *@parm
 *
 *@return
 *@todo
 */
	void WriteCheckPointsAndStatus(unsigned GunNum,int CheckPointX,float CheckPintY,float GunDelay[][2],FILE*fp,VectorType::iterator m_ColumnUnitColor);
	void CompareString(unsigned GunNum,int CheckPintY, float GunDelay[][2]);
/*!
 *@brief 逐一对原始突变点进行扩充、填写新的颜色状态
 *
 *@parm CheckPoint 所有的原始突变点集合
 *@parm CheckPointColorList 所有原始突变点对应的颜色状态
 *@parm GunDelay 所有的延迟
 *
 *@return
 *@todo
 */	
	bool CheckEveryGunDelayAndAddCheckPoint(VectorTypePoint CheckPoint,VectorType CheckPointColorList,float GunDelay[][2],unsigned GunNum,bool bReverse);
/*!
 *@brief 对给定原始突变点进行扩充、填写新的颜色状态
 *
 *@parm itCurColor 原始突变点对应颜色
 *@parm itCheckPoint 原始突变点
 *@parm PreColor 前一原始突变点的颜色
 *@parm GunDelay 所有的延迟
 *
 *@return
 *@todo 倒着走时未处理，需要排序后再倒一次，一个突变点对同一把喷枪状态要么由开到关要么反之，要么维持原状，只有四种。不管扩充成几个不会矛盾
 */		
	bool CheckOneCheckPoint(VectorType::iterator itCurColor,VectorTypePoint::iterator itCheckPoint,int PreColor,float GunDelay[][2],unsigned GunNum,bool bReverse);
/*!
 *@brief 对给定扩充后的突变点进行扩充、填写新的颜色状态，按照给定的突变点及引起突变的枪号，在原来颜色PreColor基础上设置相应枪位的颜色状态，
 *
 *@parm itCurColor 原始突变点对应颜色
 *@parm itSortedOpenCheckSets 排序了的开启突变点集迭代器
 *@parm itSortedCloseCheckSets 排序了的关闭突变点集迭代器
  *@parm itAllDelayedCheckSets 未排序所有突变点击的迭代器
  *@parm PreColor 前一原始突变点的颜色
 *@parm 
 *
 *@return
 *@todo 倒着走时未处理，需要排序后再倒一次，一个突变点对同一把喷枪状态要么由开到关要么反之，要么维持原状，只有四种。不管扩充成几个不会矛盾
 */		
	bool CheckOneExpandedCheckPoint(VectorType::iterator itCurColor,std::map< float,int>::iterator itSortedOpenCheckSets,
		std::map< float,int>::iterator itSortedCloseCheckSets,std::map< float,int>::iterator itAllDelayedCheckSets,
		std::map< float,int>& DelayMap,//延迟之后的突变点,导致开启延迟的枪号,
		std::map< float,int>& CloseDelayMap,//延迟之后的突变点,导致关闭延迟的枪号,
		CheckPointPairVecType RepeatedOpenCheckPoints, CheckPointPairVecType RepeatedCloseCheckPoints,
		int& PreColor,bool bReverse);
	VectorTypePoint m_CheckPointListWithDelay;//考虑延迟后的新的突变点
	VectorType m_CheckPointColorListWithDelay;//考虑延迟后的新的突变点处的颜色
	VectorTypePoint OneCheckPointExpandTmp;//单个检查点扩充后的检查点集,延迟完成了扩充成GunNum个突变点
	VectorType ColorOfExpandedCheckPointsTmp;//扩充后的检查点集对应的颜色集合
	std::vector<int>GunNumValue;//各把枪枪号对应的十六进制数
	//初始化所有枪号对应的二进制数
	void InitGunNumValue(int GunNum)
	{		
		GunNumValue.clear();
		for (int i=0;i<GunNum;i++)
		{
			GunNumValue.push_back(pow((float)2,i));//各喷枪对应的号码值,目前仅限4把喷枪
		}
	}
	/*!
	 *@brief 检查重复的突变点，并把所有重复的点单独拿出来，按照突变点位置从小到大排列
	 *
	 *@parm Delay 突变点的坐标
	 *@parm
	 *
	 *@return
	 *@todo
	 */
	CheckPointPairVecType CheckGetRepeatedDelay(std::vector<float> Delay);
	/*!
	 *@brief 对每一个扩充后的突变点，对给定的枪，最终形成该点该有的颜色状态即所有枪的状态，这里会针对是延迟开启还是延迟关闭突变点做分别处理,会对与他重复的突变点处理
	 *
	 *@parm itCurColor 当前原始突变点处的颜色状太
	 *@parm GunIndex 需要判断的枪号，即这个扩充的突变点是由几号枪的延迟照成的
	 *@parm PreColor 到达此突变点前的颜色状态
	 *@parm RepeatedCheckPoints 与本突变点重合的其他突变点，注意开启和关闭引致的扩充后产生的重合点要分别给
	 *@parm NeedOpen 这是个延迟开的突变点么？
	 *
	 *@return
	 *@todo
	 */
	bool SetExpandedCheckPointsForEachGun(VectorType::iterator itCurColor,int GunIndex,
		float CheckPointPose,// CornerPoint& GunCheckPointTmp, 
		int& PreColor,CheckPointPairVecType& RepeatedCheckPoints,
		bool NeedOpen);
	/*!
	 *@brief 因为要把所有扩充的延迟点统一考虑，却又必须依据它是要延迟开启还是延迟关闭分别处理，所以要判断他是开还是关的延迟点
	 *
	 *@parm 
	 *@parm
	 *
	 *@return 返回该突变点是否为输入的类型，是为真
	 *@todo
	 */
	bool JudgeExpandedCheckPointType(std::map< float,int>::iterator itAllDelayedCheckSets,
		std::map< float,int>::iterator itSorteTypeCheckSets,std::map< float,int>::iterator itSorteTypeCheckSetsEnd)
	{
		for (itSorteTypeCheckSets;itSorteTypeCheckSets!=itSorteTypeCheckSetsEnd;itSorteTypeCheckSets++)	{	//是提前开启判断的话 
			//float a=itAllDelayedCheckSets->first;
			//float b=itSorteTypeCheckSets->first;
			if (itAllDelayedCheckSets->first==itSorteTypeCheckSets->first){
				return true;
			}
		}
		return false;
	}
/*!
 *@brief 当后一个原始突变点扩充后跑到前面某个原始突变点扩充后的位置之前了，产生冲突，需要额外处理
 *内部的其他函数会调用m_vStackOfAllDelayPointMapInRow，m_vStackOfAllRepeatedOpenCheckPoints等内部的突变点集合，因此
 *迭代器it_CurCheckPoint，itRepeatedCheckPoints，itRepeatedCloseCheckPoints必须与这些变量严格一一对应
 *增加延迟突变点间相互冲突，即后一个原始突变点产生的延迟突变点，跑到前某个原始突变点延迟扩充后的范围内的情况。处理思路是，找到这个产生冲突的突变点，把这个突变点是由几号喷枪的延时造成的，把这几号喷枪对应的颜色位的值取出来，
 *再把最新的这几位的值覆盖回去所有与之冲突的突变点对应的颜色位
 *结果会改变m_CheckPointColorListWithDelay中存的颜色
 *@parm fCurCheckPoint 当前要检查是否发生冲突的突变点Y坐标
 *@parm StackOfAllDelayPointMapInRow 本列所有扩充后的突变点序列，该突变点序列必须没有经过最后的排序，还是按照原始突变点的顺序排列
 *
 *@return 
 *@todo
 */
	bool CheckConflictFromDifferentOriginCheckPoint(std::vector<std::pair<float,int>>::iterator it_CurCheckPoint,std::vector<std::pair<float,int>>& StackOfAllDelayPointMapInRow
		,CheckPointPairVecType::iterator& itRepeatedCheckPoints,CheckPointPairVecType::iterator& itRepeatedCloseCheckPoints
		,bool bReverse);
	void ClearAllCheckPointsBackUpVecInRow()
	{
		m_CheckPointListWithDelay.clear();
		m_CheckPointColorListWithDelay.clear();
		m_vStackOfAllDelayPointMapInRow.clear();//每列清零一次

		m_vStackOfAllRepeatedCloseCheckPoints.clear();
		m_vStackOfAllRepeatedOpenCheckPoints.clear();
	}
	float m_fMaxRange;//本次喷涂的最大运动范围值
	void LimitCheckPoint(float& OpenCheckPointAfterDelay,float& CloseCheckPointAfterDelay,float MaxRange)
	{
		OpenCheckPointAfterDelay=OpenCheckPointAfterDelay<0?0:OpenCheckPointAfterDelay;
		CloseCheckPointAfterDelay=CloseCheckPointAfterDelay<0?0:CloseCheckPointAfterDelay;
		OpenCheckPointAfterDelay=OpenCheckPointAfterDelay>MaxRange?MaxRange:OpenCheckPointAfterDelay;
		CloseCheckPointAfterDelay=CloseCheckPointAfterDelay>MaxRange?MaxRange:CloseCheckPointAfterDelay;

		//if (OpenCheckPointAfterDelay<0)		{
		//	OpenCheckPointAfterDelay=0;
		//}
		//if (CloseCheckPointAfterDelay<0)	{
		//	CloseCheckPointAfterDelay=0;
		//}
		//if (OpenCheckPointAfterDelay>MaxRange)		{
		//	OpenCheckPointAfterDelay=MaxRange;
		//}
		//if (CloseCheckPointAfterDelay)		{
		//	CloseCheckPointAfterDelay=MaxRange;
		//}
	}
	std::vector<std::pair<float,int>> m_vStackOfAllDelayPointMapInRow;//所有延迟之后的突变点,导致延迟的枪号,用于处理最后的突变点冲突调节
	
	CheckPointPairVecType m_vStackOfAllRepeatedOpenCheckPoints;//所有延迟之后重复的开启突变点,导致延迟的枪号,用于处理最后的突变点冲突调节
	CheckPointPairVecType m_vStackOfAllRepeatedCloseCheckPoints;//所有延迟之后重复的关闭突变点,导致延迟的枪号,用于处理最后的突变点冲突调节
 /*!
 *@brief 进行给定点颜色的位操作。用于把当前这个颜色中产生冲突的枪号对应的颜色位覆盖回去产生冲突点的颜色位
 *
 *@parm BackPosIndex 要被覆盖的冲突点的下标
 *@parm GunIndex 给定枪号
 *
 *@return
 *@todo
 */
	void CoverGivenBitColorForGivenPoint(int BackCheckPoint,int CurCheckPoint,int GunIndex);
 /*!
 *@brief 回溯要覆盖的冲突点的颜色，把当前这个颜色中产生冲突的枪号对应的位置覆盖回去产生冲突点的颜色位
 *
 *@parm BackPosIndex 要被覆盖的冲突点的下标
 *@parm CurIndex 当前这个点的下标
 *@parm  itRepeatedCheckPoints 在开启突变点集中的基准位置，函数内可能移动它并传出来
 *
 *@return
 *@todo
 */
	void BackSetConflictPointColor(std::vector<std::pair<float,int>>::iterator itvPreCheckPoint,
		std::vector<std::pair<float,int>>::iterator itvCurCheckPoint,	
		CheckPointPairVecType::iterator& itRepeatedCheckPoints, CheckPointPairVecType::iterator& itRepeatedCloseCheckPoints);
 /*!
 *@brief 判断当前给定的位置是否与其他突变点发生了重复，发生重复的位置由itRepeatedCheckPoints返回
 *
 *@parm CheckPointPose 要判断的突变点位置
 *@parm itRepeatedCheckPoints 当前要判断的位置
 *
 *@return
 *@todo
 */
	bool JudgeRepeatedCheckPoint(float CheckPointPose,CheckPointPairVecType::iterator& itRepeatedCheckPoints,CheckPointPairVecType::iterator itRepeatedCheckPointsEnd)
	{
		for (itRepeatedCheckPoints;itRepeatedCheckPoints!=itRepeatedCheckPointsEnd;itRepeatedCheckPoints++)
		{
			if (CheckPointPose==itRepeatedCheckPoints->second)//确实重复了
			{
				return true;
			}
		}
		return false;
	}
};

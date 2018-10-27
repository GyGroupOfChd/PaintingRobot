#include "DownParmValue.h"
#include "math.h"
DownValue::DownValue()
{
	GunColorAll.clear();
	m_EveryColumnGunColor.clear();
	AllChangeColorPosition.clear();
	PerLaw=0;
	JudgeReturn=false;
	JudgeChangeDirection=true;
	LastRemberY=0;
	JudgeFirstLow=true;
	m_JudgeOneChoice=0;
	m_CornerPoint.XPoint=0;
	m_CornerPoint.YPoint=0;
	JudgeIsFirstCorner=false;
	JudgeIsOrFirstPer=true;
	InitGunNumValue(6);//初始化喷枪号 
}
DownValue::~DownValue()
{
	GunMenber=1;
}
void DownValue::GetGunModelAndGunDistance(int GunNember,float BetweenGunDistance)
{
	m_GunModelAndDistance.GunNember=GunNember;//喷枪数
	m_GunModelAndDistance.BetweenGunDistance=BetweenGunDistance;//喷枪间距
}
void DownValue::GetDownValueMotion(MotionParm_It_TYPE MotionBegin,MotionParm_It_TYPE MotionEnd)            //不可变更
{
	m_GunPerBeginAndEnd.clear();
	PerLaw=(int)*MotionBegin;              //代表当前工作区有几列
	MotionBegin++;
	Speed=*MotionBegin;
	MotionBegin++;
	PerWidth=*MotionBegin;
	MotionBegin++;
	PerHeight=*MotionBegin;
	int RemberNumber=0;
	MotionBegin++;
	if (m_GunModelAndDistance.GunNember!=1)
	{
		bool IsClockWise=true;                                 //顺时针
		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //保存每一列的起始位置和终止位置，并保存列号
		{	

			GunPerBeginAndEnd m_GunPerBAE;
			if (RemberNumber==0)          //得到每一列的列号
			{
				m_GunPerBAE.Index=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==1)        //得到每一列的起点坐标
			{
				m_GunPerBAE.Begin=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==2)     //得到每一列的终点坐标
			{
				m_GunPerBAE.End=*MotionBegin;
				m_GunPerBeginAndEnd.push_back(m_GunPerBAE);
				RemberNumber=0;
			}
			IsClockWise=!IsClockWise;
		}
	}
	else
	{
		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //保存每一列的起始位置和终止位置，并保存列号
		{	
			GunPerBeginAndEnd m_GunPerBAE;
			if (RemberNumber==0)          //得到每一列的列号
			{
				m_GunPerBAE.Index=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==1)        //得到每一列的起点坐标
			{
				int Begin=*MotionBegin;
				int BeginIndex=Begin/PerHeight;
				int m_Shang=Begin%PerHeight;
				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
				m_GunPerBAE.Begin=(BeginIndex+Result)*PerHeight;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==2)     //得到每一列的终点坐标
			{
				int End=*MotionBegin;
				int EndIndex=End/PerHeight;
				int m_Shang=End%PerHeight;
				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
				m_GunPerBAE.End=(EndIndex+Result)*PerHeight;
				m_GunPerBeginAndEnd.push_back(m_GunPerBAE);
				RemberNumber=0;
			}
		}
	}

}
void DownValue::TranslateOneColumm(unsigned CurColumIndex,float MoveRangeEnd,float MoveRangeBegin,int CellToBeRun,bool bisForward)
{
	CornerPoint m_XY;
	m_XY.XPoint=CurColumIndex*PerHeight;//假定列宽与高一样是PerHeight了
	std::vector<float>::iterator itChangePointHead=m_ChangedColorPointHead;
	for (int i=0;i<CellToBeRun;i++ )//每各色块，含因为枪距而补充的虚拟色块
	{
		if (bisForward)//正向
		{
			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//每个色块内的突变点
			{
				float Y_coord=MoveRangeBegin+(i+(*itChangePointHead))*PerHeight;
				if (Y_coord<=MoveRangeEnd+1.1)//在行程限制内，1.1是为了防止有时会有1的微小误差
				{					
					m_XY.YPoint=Y_coord;
					m_VectorTypePoint.push_back(m_XY);	
				}
				else
					break;
			}
		}
		else
		{
			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//每个色块内的突变点
			{
				float Y_coord=MoveRangeBegin-(i+(*itChangePointHead))*PerHeight;
				if (Y_coord>=MoveRangeEnd-1.1)//在行程限制内 1.1是为了防止又是会有1的微小误差
				{					
					m_XY.YPoint=Y_coord;
					m_VectorTypePoint.push_back(m_XY);	
				}
				else
					break;
			}
		}
		//m_ChangedPoint.push_back(i*PerHeight);
		
	}
}
//void DownValue::GetDownValueMotion2(MotionParm_It_TYPE MotionBegin,MotionParm_It_TYPE MotionEnd)            //不可变更
//{
//	m_GunPerBeginAndEnd.clear();
//	PerLaw=(int)*MotionBegin;              //代表当前工作区有几列
//	MotionBegin++;
//	Speed=*MotionBegin;
//	MotionBegin++;
//	PerWidth=*MotionBegin;
//	MotionBegin++;
//	PerHeight=*MotionBegin;
//	int RemberNumber=0;
//	MotionBegin++;
//	if (m_GunModelAndDistance.GunNember!=1)
//	{
//		bool IsClockWise=true;                                 //顺时针
//		
//		for (MotionBegin;MotionBegin!=MotionEnd;MotionBegin++)  //保存每一列的起始位置和终止位置，并保存列号
//		{	
//
//			GunPerBeginAndEnd m_GunPerBAE;
//			if (RemberNumber==0)          //得到每一列的列号
//			{
//				m_GunPerBAE.Index=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==1)        //得到每一列的起点坐标
//			{
//				m_GunPerBAE.Begin=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==2)     //得到每一列的终点坐标
//			{
//				m_GunPerBAE.End=*MotionBegin;
//				m_GunPerBeginAndEnd.push_back(m_GunPerBAE);
//				RemberNumber=0;
//				IsClockWise=!IsClockWise;
//			}			
//		}
//	}
//	else//暂不做修改
//	{
//		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //保存每一列的起始位置和终止位置，并保存列号
//		{	
//			GunPerBeginAndEnd m_GunPerBAE;
//			if (RemberNumber==0)          //得到每一列的列号
//			{
//				m_GunPerBAE.Index=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==1)        //得到每一列的起点坐标
//			{
//				int Begin=*MotionBegin;
//				int BeginIndex=Begin/PerHeight;
//				int m_Shang=Begin%PerHeight;
//				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
//				m_GunPerBAE.Begin=(BeginIndex+Result)*PerHeight;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==2)     //得到每一列的终点坐标
//			{
//				int End=*MotionBegin;
//				int EndIndex=End/PerHeight;
//				int m_Shang=End%PerHeight;
//				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
//				m_GunPerBAE.End=(EndIndex+Result)*PerHeight;
//				m_GunPerBeginAndEnd.push_back(m_GunPerBAE);
//				RemberNumber=0;
//			}
//		}
//	}
//
//}
//void DownValue::TranslateOneColumm(unsigned CurColumIndex,float MoveRangeEnd,float MoveRangeBegin,int CellToBeRun,bool bisForward)
//{
//	CornerPoint m_XY;
//	m_XY.XPoint=CurColumIndex*PerHeight;//假定列宽与高一样是PerHeight了
//	std::vector<float>::iterator itChangePointHead=m_ChangedColorPointHead;
//	for (int i=0;i<CellToBeRun;i++ )//每各色块，含因为枪距而补充的虚拟色块
//	{
//		if (bisForward)//正向
//		{
//			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//每个色块内的突变点
//			{
//				float Y_coord=MoveRangeBegin+(i+(*itChangePointHead))*PerHeight;
//				if (Y_coord<=MoveRangeEnd+1.1)//在行程限制内，1.1是为了防止有时会有1的微小误差
//				{					
//					m_XY.YPoint=Y_coord;
//					m_VectorTypePoint.push_back(m_XY);	
//				}
//				else
//					break;
//			}
//		}
//		else
//		{
//			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//每个色块内的突变点
//			{
//				float Y_coord=MoveRangeBegin-(i+(*itChangePointHead))*PerHeight;
//				if (Y_coord>=MoveRangeEnd-1.1)//在行程限制内 1.1是为了防止又是会有1的微小误差
//				{					
//					m_XY.YPoint=Y_coord;
//					m_VectorTypePoint.push_back(m_XY);	
//				}
//				else
//					break;
//			}
//		}
//		//m_ChangedPoint.push_back(i*PerHeight);
//
//	}
//}
void DownValue::TranslateEveryChangePoint2(unsigned m_uGunNum,float m_fGunDis)               //得到所有突变点的坐标
{
	int FirstAndLastGunDistance=(m_uGunNum-1)*m_fGunDis;               //第一把喷枪到最后一把喷枪之间的间隔
	std::vector<GunPerBeginAndEnd>::iterator m_GunPerBeginAndEndIndex;
	m_GunPerBeginAndEndIndex=m_GunPerBeginAndEnd.begin();
	m_VectorTypePoint.clear();
	float columnHeight;
	columnHeight=(*m_GunPerBeginAndEndIndex).End-(*m_GunPerBeginAndEndIndex).Begin-FirstAndLastGunDistance;     //喷涂区域的高
	int CellToBeRun;//要跑的方块数	
	int m_Column=m_GunPerBeginAndEnd.size();             //当前喷涂单元有多少列
	bool JudgeOriention=true;                            //判断是逆时针还是顺时针
	std::vector<float> m_ChangedPoint;                     //得到突变点
	std::vector<float>::iterator m_ChangedPointIndex;
	std::vector<float>::iterator m_ChangedPointIndexCopy;
	m_ChangedPoint.clear();
	
	
	for (int i=0;i<m_Column;i++)
	{
		m_VectorTypePoint.clear();
		CellToBeRun=ceil(abs(float((*m_GunPerBeginAndEndIndex).End-(*m_GunPerBeginAndEndIndex).Begin)/PerHeight));
		TranslateOneColumm((*m_GunPerBeginAndEndIndex).Index,(*m_GunPerBeginAndEndIndex).End,(*m_GunPerBeginAndEndIndex).Begin,CellToBeRun,JudgeOriention);
		
		AllChangeColorPosition.insert(std::map<int,VectorTypePoint>::value_type((*m_GunPerBeginAndEndIndex).Index,m_VectorTypePoint)); 
		m_GunPerBeginAndEndIndex++;
		JudgeOriention=!JudgeOriention;
	}
	
}
void DownValue::TranslateEveryChangePoint(unsigned m_uGunNum,float m_fGunDis)               //得到所有突变点的坐标
{

	int FirstAndLastGunDistance=(m_uGunNum-1)*m_fGunDis;               //第一把喷枪到最后一把喷枪之间的间隔
	std::vector<GunPerBeginAndEnd>::iterator m_GunPerBeginAndEndIndex;
	m_GunPerBeginAndEndIndex=m_GunPerBeginAndEnd.begin();

	int m_Column=m_GunPerBeginAndEnd.size();             //当前喷涂单元有多少列
	bool JudgeOriention=true;                            //判断是逆时针还是顺时针
	std::vector<float> m_ChangedPoint;                     //得到突变点
	std::vector<float>::iterator m_ChangedPointIndex;
	std::vector<float>::iterator m_ChangedPointIndexCopy;
	m_ChangedPoint.clear();
	for (m_ChangedColorPointHeadCopy;m_ChangedColorPointHeadCopy<m_ChangedColorPointEnd;m_ChangedColorPointHeadCopy++)           //求突变点
	{
		m_ChangedPoint.push_back((*m_ChangedColorPointHeadCopy));
	}
	float GetSecondGunPosition=(float)(PerHeight-m_fGunDis)/PerHeight;            //得到第二把喷枪的位置
	m_ChangedPointIndex=m_ChangedPoint.begin();
	for (m_ChangedPointIndex;m_ChangedPointIndex!=m_ChangedPoint.end();m_ChangedPointIndex++)
	{
		if (m_ChangedPointIndex==(m_ChangedPoint.end()-1))
		{
			m_ChangedPointIndexCopy=m_ChangedPointIndex=m_ChangedPoint.end();
			break;
		}
		if ((*m_ChangedPointIndex)-GetSecondGunPosition>0)
		{
			m_ChangedPointIndexCopy=m_ChangedPointIndex;
			break;
		}
	}
	AllChangeColorPosition.clear();
	for (m_GunPerBeginAndEndIndex;m_GunPerBeginAndEndIndex!=m_GunPerBeginAndEnd.end();m_GunPerBeginAndEndIndex++)
	{

		m_VectorTypePoint.clear();
		int columnHeight;
		int EveryColumnPerNumber;
		if (JudgeOriention==true)
		{
			columnHeight=(*m_GunPerBeginAndEndIndex).End-(*m_GunPerBeginAndEndIndex).Begin-FirstAndLastGunDistance;     //喷涂区域的高
			EveryColumnPerNumber=(float)columnHeight/PerHeight+0.5;       //得到每一列的单元格数
		}
		else
		{
			columnHeight=(*m_GunPerBeginAndEndIndex).Begin-(*m_GunPerBeginAndEndIndex).End-FirstAndLastGunDistance;     //喷涂区域的高
			EveryColumnPerNumber=(float)columnHeight/PerHeight+0.5;       //得到每一列的单元格数
		}
		for (int Index=0;Index<EveryColumnPerNumber;Index++)
		{

			for (m_ChangedColorPointHead;m_ChangedColorPointHead<m_ChangedColorPointEnd;m_ChangedColorPointHead++)
			{

				CornerPoint m_XY;
				m_XY.XPoint=(*m_GunPerBeginAndEndIndex).Index*PerHeight;
				if (JudgeOriention==true)
				{
					m_XY.YPoint=(*m_GunPerBeginAndEndIndex).Begin+ Index*PerHeight+(*m_ChangedColorPointHead)*PerHeight;
				}
				else
				{
					m_XY.YPoint=(*m_GunPerBeginAndEndIndex).Begin- Index*PerHeight-(*m_ChangedColorPointHead)*PerHeight;
				}

				m_VectorTypePoint.push_back(m_XY);
			}
			m_ChangedColorPointHead=m_ChangedColorPointHeadCopyl;
		}
		for (int GunValue=1;GunValue<m_uGunNum;GunValue++)
		{
			CornerPoint m_XYl;
			m_XYl.XPoint=(*m_GunPerBeginAndEndIndex).Index*PerHeight;
			if (JudgeOriention==true)
			{
				m_XYl.YPoint=(*m_GunPerBeginAndEndIndex).End-FirstAndLastGunDistance+(GunValue-1)*m_fGunDis;//各枪过最后的色块末边算得
			}
			else
			{
				m_XYl.YPoint=(*m_GunPerBeginAndEndIndex).End+FirstAndLastGunDistance-(GunValue-1)*m_fGunDis;
			}
			m_VectorTypePoint.push_back(m_XYl);

			for (m_ChangedPointIndex;m_ChangedPointIndex!=m_ChangedPoint.end();m_ChangedPointIndex++)
			{
				if (JudgeOriention==true)
				{
					m_XYl.YPoint=(*m_GunPerBeginAndEndIndex).End-FirstAndLastGunDistance+GunValue*m_fGunDis+(*m_ChangedPointIndex-1)*PerHeight;
				}
				else
				{
					m_XYl.YPoint=(*m_GunPerBeginAndEndIndex).End+FirstAndLastGunDistance-GunValue*m_fGunDis-(*m_ChangedPointIndex-1)*PerHeight;
				}
				m_VectorTypePoint.push_back(m_XYl);

			}
			m_ChangedPointIndex=m_ChangedPointIndexCopy;
		}
		CornerPoint m_XYlEnd;
		m_XYlEnd.XPoint=(*m_GunPerBeginAndEndIndex).Index*PerHeight;
		m_XYlEnd.YPoint=(*m_GunPerBeginAndEndIndex).End;
		m_VectorTypePoint.push_back(m_XYlEnd);
		JudgeOriention=!JudgeOriention;
		AllChangeColorPosition.insert(std::map<int,VectorTypePoint>::value_type((*m_GunPerBeginAndEndIndex).Index,m_VectorTypePoint)); 
	}

	//得到每一列的喷涂高度
}
void DownValue::TranslateEveryChangeColor(int BeginPoint,int EndPoint)              //简析颜色,没用到
{

	GunPerBeginAndEnd m_EveryChangedColorPoint;
	int m_PerHeight=PerHeight;        //每个小单元的高度
	if (BeginPoint>EndPoint)          //考虑到喷枪往返不一致
	{
		m_PerHeight=-m_PerHeight;
	}
	int EveryColumnUnitMenber=(EndPoint-BeginPoint)/m_PerHeight; //得到每一列有多少个喷涂单元
	for (int i=1;i<=EveryColumnUnitMenber;i++)
	{
		for (int j=1;j<=GunMenber;j++)
		{
			if (BeginPoint<EndPoint)
			{
				if ((j-1)*m_GunModelAndDistance.BetweenGunDistance>PerHeight)
				{
					int m_merchant=((j-1)*m_GunModelAndDistance.BetweenGunDistance)/m_PerHeight;
					int m_Remainder=((j-1)*(int)m_GunModelAndDistance.BetweenGunDistance)%m_PerHeight;
					int Point=BeginPoint+(i-1+m_merchant)*PerHeight+m_Remainder;
					m_ColumnXYPointEverySholdJudgeColor.push_back(Point);
				}
				else
				{
					int Point=BeginPoint+(i-1)*m_PerHeight+(j-1)*m_GunModelAndDistance.BetweenGunDistance;
					m_ColumnXYPointEverySholdJudgeColor.push_back(Point);
				}
			}
			else
			{
				if ((j-1)*m_GunModelAndDistance.BetweenGunDistance>PerHeight)
				{
					int m_merchant=((j-1)*m_GunModelAndDistance.BetweenGunDistance)/PerHeight;
					int m_Remainder=((j-1)*(int)m_GunModelAndDistance.BetweenGunDistance)%PerHeight;
					int Point=BeginPoint-((i-1+m_merchant)*PerHeight+m_Remainder);
					m_ColumnXYPointEverySholdJudgeColor.push_back(Point);
				}
				else
				{
					int Point=BeginPoint-((i-1)*PerHeight+(j-1)*m_GunModelAndDistance.BetweenGunDistance);
					m_ColumnXYPointEverySholdJudgeColor.push_back(Point);
				}
			}

		}
	}
}
void DownValue::GetDownCameraValue(CString CameraParm)
{
	CString m_AngleCamera=GetStringPaser_star(CameraParm,1);
	if (m_AngleCamera!="")
	{
		m_CameraParm.Angle_Correct= atof(m_AngleCamera);
	}
	CString m_XCamera=GetStringPaser_star(CameraParm,2);
	if (m_XCamera!="")
	{
		m_CameraParm.X_Correct=atof(m_XCamera.GetBuffer());
	}
	CString m_YCamera=GetStringPaser_star(CameraParm,3);
	if (m_YCamera!="")
	{
		m_CameraParm.Y_Correct=atof(m_YCamera.GetBuffer());
	}
}
CString DownValue::GetStringPaser_star(CString gps_str,int num)    //截取逗号到星号间的字符串
{
	int start=0;
	for(int i=0;i<num;i++)
	{
		int find_ok=gps_str.Find(",",start);
		if(find_ok>=0)
			start=find_ok+1;
		else 
			return _T("");
	}
	int end=gps_str.Find(",",start);
	if(end>start)
		return gps_str.Mid(start,end-start);
	else 
		return _T("");
}
void DownValue::GetDownValuePlc(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis)       //存储喷枪状态
{
	std::vector<int>::iterator m_RemberStoragePlc;
	AllChangeColorPosition.clear();//

	m_RemberStoragePlc=StoragePlc.begin();
	int RemberNumber=(StoragePlc.size()-1)/PerLaw;
	m_RemberStoragePlc++;  //m_RemberStoragePlc首个保存的是单元高度
	GunColorAll.clear();         //每次都到清空
	m_EveryColumnGunColor.clear();
	int Index=0;
	for (m_RemberStoragePlc;m_RemberStoragePlc!=StoragePlc.end();m_RemberStoragePlc++)
	{
		int ColumnUintNumber=*m_RemberStoragePlc;    //当前列中喷枪状态数，含分割用的０
		for (int Number=0;Number<ColumnUintNumber-1;Number++)//ColumnUintNumber比实际突变点数大1，因为在列末尾人为加了一个0做列分割
		{
			m_RemberStoragePlc++;
			m_EveryColumnGunColor.push_back(*m_RemberStoragePlc);                                    //每一列的plc的值
		}
		GunColorAll.insert(std::map<int,VectorType>::value_type(Index,m_EveryColumnGunColor));       //得到一个二位数组
		m_EveryColumnGunColor.clear();
		m_RemberStoragePlc++;//绕过列末尾的分割０
		Index++;
	}
	m_ChangedColorPointHead=ChangedColorPointHead;
	m_ChangedColorPointHeadCopy=ChangedColorPointHead;
	m_ChangedColorPointHeadCopyl=ChangedColorPointHead;
	m_ChangedColorPointEnd=ChangedColorPointEnd;
	TranslateEveryChangePoint2(m_uGunNum,m_fGunDis);
	
}

//void DownValue::GetDownValuePlc2(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis)       //存储喷枪状态
//{
//	std::vector<int>::iterator m_RemberStoragePlc;
//	m_RemberStoragePlc=StoragePlc.begin();
//	int RemberNumber=(StoragePlc.size()-1)/PerLaw;
//	m_RemberStoragePlc++;  //m_RemberStoragePlc首个保存的是单元高度
//	GunColorAll.clear();         //每次都到清空
//	m_EveryColumnGunColor.clear();
//	int Index=0;
//	//////////////////////////////提取颜色状态////////////////////////////////////////////
//	for (m_RemberStoragePlc;m_RemberStoragePlc!=StoragePlc.end();m_RemberStoragePlc++)
//	{
//		int ColumnUintNumber=*m_RemberStoragePlc;    //当前列中喷枪状态数
//		for (int Number=0;Number<ColumnUintNumber;Number++)
//		{
//			m_RemberStoragePlc++;
//			m_EveryColumnGunColor.push_back(*m_RemberStoragePlc);                                    //每一列的plc的值
//		}
//		GunColorAll.insert(std::map<int,VectorType>::value_type(Index,m_EveryColumnGunColor));       //得到一个二位数组
//		m_EveryColumnGunColor.clear();
//		Index++;
//	}
//	m_ChangedColorPointHead=ChangedColorPointHead;
//	m_ChangedColorPointHeadCopy=ChangedColorPointHead;
//	m_ChangedColorPointHeadCopyl=ChangedColorPointHead;
//	m_ChangedColorPointEnd=ChangedColorPointEnd;
//	TranslateEveryChangePoint(m_uGunNum,m_fGunDis);//提取坐标点
//	
//}

CString DownValue::CompileGunCondition(int Parm)  //转变图块颜色到m
{
	GunCondition m_Storage;
	m_Storage.FiveCondition=false;
	m_Storage.FourCondition=false;
	m_Storage.SixCondition=false;
	m_Storage.ThreeCondition=false;
	m_Storage.TwoCondition=false;
	m_Storage.OneCondition=false;

	CString m_Answer="";

	int SixCondition=Parm/32;
	if (SixCondition==1)
	{
		m_Storage.SixCondition=true;
	}
	else
		m_Storage.SixCondition=false;
	int FiveConditionReserve=Parm%32;
	int FiveCondition=FiveConditionReserve/16;
	if (FiveCondition==1)
	{
		m_Storage.FiveCondition=true;
	}
	else
		m_Storage.FiveCondition=false;
	int FourConditionReserve=FiveConditionReserve%16;
	int FourCondition=FourConditionReserve/8;
	if (FourCondition==1)
	{

		{
			m_Storage.FourCondition=true;
			m_Answer=m_Answer+"m4012=1";
		}

	}
	else
	{
		m_Storage.FiveCondition=false;
		m_Answer=m_Answer+"m4012=0";
	}
	int ThreeConditionReserve=FourConditionReserve%8;
	int ThreeCondition=ThreeConditionReserve/4;
	if (ThreeCondition==1)
	{
		m_Storage.ThreeCondition=true;
		m_Answer=m_Answer+" m4011=1";
	}
	else
	{
		m_Storage.ThreeCondition=false;
		m_Answer=m_Answer+" m4011=0";
	}
	int TwoConditionReserve=ThreeConditionReserve%4;
	int TwoCondition=TwoConditionReserve/2;
	if (TwoCondition==1)
	{
		m_Storage.TwoCondition=true;
		m_Answer=m_Answer+" m4010=1";
	}
	else
	{
		m_Storage.TwoCondition=false;
		m_Answer=m_Answer+" m4010=0";
	}
	int OneConditionReserve=TwoConditionReserve%2;
	if (OneConditionReserve==1)
	{
		m_Storage.OneCondition=true;
		m_Answer=m_Answer+" m4009=1";
	}
	else
	{	
		m_Storage.OneCondition=false;
		m_Answer=m_Answer+" m4009=0";
	}
	m_Answer=m_Answer+'\n';
	return m_Answer;
}
void DownValue::StartOpen(CString Directory)           //当前不需要
{
	int v=(int)Speed;
	CString DirectoryFile="";
	DirectoryFile=Directory+"\\DownMove.pmc";
	//char ch[1024];
	//memset(ch,0,strlen(ch));
	//strncpy(ch,(LPCTSTR)DirectoryFile,sizeof(ch));
	//ch= DirectoryFile.GetBuffer(DirectoryFile.GetLength() + 1);
	//DirectoryFile.ReleaseBuffer();
	//CString tt="D:\\MAIn.pmc";

	FILE  *fp=fopen((LPCTSTR)DirectoryFile,"w");
	//fwrite("OPEN PROG255 CLEAR\n",1,strlen("OPEN PROG255 CLEAR\n"),fp);
	fprintf(fp,"&1\n");
	fprintf(fp,"OPEN PROG255 CLEAR\n");
	fprintf(fp,"abs \n");
	fprintf(fp,"lin \n");
	fprintf(fp,"F(%d) \n",v);      //单位m/s
	fprintf(fp,"TA500 \n");        //加速度
	fprintf(fp,"TS0 \n");           //S曲线加速度
	int c=1;
	int k=0;
	char buf[1024];
	memset(buf,0,sizeof(buf));
	int x,y;
	for (int j=0;j<PerLaw;j++)               //n代表列
	{
		int x= PerWidth*j;
		if (JudgeReturn==false)  //y轴为最大
		{
			if (JudgeFirstLow==false)           //做第一行判断
			{
				sprintf(buf, "X%d Y%d M1000==(%d)\n",x,LastRemberY,c);
				fwrite(buf,strlen(buf),1,fp);
			}
			JudgeFirstLow=false;
			int y=HeightPer;
			sprintf(buf, "X%d Y%d M1000==(%d)\n",x,y,c);
			fwrite(buf,strlen(buf),1,fp);
			LastRemberY=y;
			JudgeReturn=true;
		}
		else                       //Y轴为最小
		{
			sprintf(buf, "X%d Y%d M1000==(%d)\n",x,LastRemberY,c);
			fwrite(buf,strlen(buf),1,fp);
			int y=0;
			sprintf(buf, "X%d Y%d M1000==(%d)\n",x,y,c);
			fwrite(buf,strlen(buf),1,fp);
			LastRemberY=y;
			JudgeReturn=false;
		}
	}
	sprintf(buf, "CLOSE");
	fwrite(buf,strlen(buf),1,fp);
	fclose(fp);
}
//void DownValue::WriteCheckPointsAndStatus(VectorTypePoint::iterator itPoints,double GunDelay[][2],FILE*fp,VectorType::iterator m_ColumnUnitColor)
//{
//
//}

void DownValue::WriteCheckPointsAndStatus(unsigned GunNum,int CheckPointX,float CheckPintY,double GunDelay[][2],FILE*fp,VectorType::iterator m_ColumnUnitColor)
{
	/*for (unsigned i=0;i<GunNum;i++)
	{*/
		CString Storage="";
		Storage.Format("x %d y %d\n",CheckPointX,Floor4_Ceil5(CheckPintY));
		fwrite(Storage,strlen(Storage),1,fp);
		CString EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
		fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);

		//Storage="";
		//Storage.Format("x %d y %f\n",CheckPointX,(CheckPintY));
		//fwrite(Storage,strlen(Storage),1,fp);
		//EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
		//fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
	//}
}
void DownValue::WriteCheckPointsAndStatus2(unsigned GunNum,int CheckPointX,float CheckPintY,double GunDelay[][2],FILE*fp,VectorType::iterator m_ColumnUnitColor,VectorType::iterator m_PreColumnUnitColor)
{
	/*for (unsigned i=0;i<GunNum;i++)
	{*/
		CString Storage="";
		Storage.Format("x %d y %d\n",CheckPointX,Floor4_Ceil5(CheckPintY));
		fwrite(Storage,strlen(Storage),1,fp);
		if(BitCheckFor1Change(m_PreColumnUnitColor,m_ColumnUnitColor))
		{
			fwrite(Storage,strlen(Storage),1,fp);
		}
		CString EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
		fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);

		//Storage="";
		//Storage.Format("x %d y %f\n",CheckPointX,(CheckPintY));
		//fwrite(Storage,strlen(Storage),1,fp);
		//EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
		//fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
	//}
}
bool DownValue::BitCheckFor1Change(VectorType::iterator m_PreColumnUnitColor,VectorType::iterator m_ColumnUnitColor,int GunNum)
{
	for (int i=0;i<GunNum;i++)
	{
		int CurColorBitStatus=((*m_ColumnUnitColor)&GunNumValue[i]);
		int PreColorBitStatus=((*m_PreColumnUnitColor)&GunNumValue[i]);
		if (CurColorBitStatus>PreColorBitStatus)
		{
			return true;
		}
	}
	return false;
}
void DownValue::CompareString(unsigned GunNum,int CheckPintY, double GunDelay[][2])
{
	//	char F1[1000]=int(CheckPintY+GunDelay[0][0]);
	//	char F2[1000]=int(CheckPintY+GunDelay[0][1]);
	//	for(unsigned i=0;i<GunNum;i++)
	//	{
	//		strcat(F1,int(CheckPintY+GunDelay[i][0]));
	//		strcat(F2,int(CheckPintY+GunDelay[i][1]));
	//	}
	//	strcat(F1,F2);
	//	std::vector<int>myvector (F1,F1+1000);
	//	std::sort(myvector.begin(),myvector.end(),myobject);
	//}
}
bool cmp_by_value(const std::pair<int,int>& lhs, const std::pair<int,int>& rhs) {  
	return lhs.second < rhs.second;  
}  

CheckPointPairVecType DownValue::CheckGetRepeatedDelay(std::vector<float> Delay)
{
	CheckPointPairVecType DelayMap;//不能直接用map，

	for (int i=0;i< Delay.size();i++)//建立下标与内容的映射
	{
		DelayMap.push_back(std::make_pair(i,Delay[i]));
	}
	sort(DelayMap.begin(),DelayMap.end(),cmp_by_value);//按值从小到大排序
	bool bRecentlySame=false;
	CheckPointPairVecType repeatedCheckPoints;//所有重复的突变点，第一个是枪号，后一个是它的重复坐标
	for (int i=0;i<DelayMap.size()-1;i++)
	{
		
			if ((DelayMap[i].second)==(DelayMap[i+1].second))
			{
				bRecentlySame=true;
				repeatedCheckPoints.push_back(DelayMap[i]);//重复的拿出来
			}
			else if (bRecentlySame){
				repeatedCheckPoints.push_back(DelayMap[i]);//重复的拿出来
				bRecentlySame=false;
			}
			else{
				bRecentlySame=false;
			}	
	}
	if (bRecentlySame)
	{
		repeatedCheckPoints.push_back(DelayMap[DelayMap.size()-1]);//重复的拿出来
	}
	return repeatedCheckPoints;
}

CheckPointPairVecType DownValue::CheckGetRepeatedDelay2(std::vector<float> Delay,int GunNum)
{
	CheckPointPairVecType DelayMap;//不能直接用map，
	int indGun=0;
	int Step=0;
	for (int i=0;i< Delay.size();i++)//建立下标与内容的映射
	{
		Step=std::floor((double)i/GunNum);
		DelayMap.push_back(std::make_pair(i-Step*GunNum,Delay[i]));
	}
	sort(DelayMap.begin(),DelayMap.end(),cmp_by_value);//按值从小到大排序
	bool bRecentlySame=false;
	CheckPointPairVecType repeatedCheckPoints;//所有重复的突变点，第一个是枪号，后一个是它的重复坐标
	for (int i=0;i<DelayMap.size()-1;i++)
	{

		if ((DelayMap[i].second)==(DelayMap[i+1].second))
		{
			bRecentlySame=true;
			repeatedCheckPoints.push_back(DelayMap[i]);//重复的拿出来
		}
		else if (bRecentlySame){
			repeatedCheckPoints.push_back(DelayMap[i]);//重复的拿出来
			bRecentlySame=false;
		}
		else{
			bRecentlySame=false;
		}	
	}
	if (bRecentlySame)
	{
		repeatedCheckPoints.push_back(DelayMap[DelayMap.size()-1]);//重复的拿出来
	}
	return repeatedCheckPoints;
}
bool CompPoint(const CornerPoint &a,const CornerPoint &b)
{
	return a.YPoint<b.YPoint;
}

//template bool seekSameCheckPoints(const pair<float, int> &t)
//{
//	return t.second == 1 ? 1 : 0;
//}
#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0
bool DownValue::CheckAndSetRemainColor(VectorTypePoint::iterator itCurOriginCheckPoint,VectorTypePoint::iterator itOriginCheckPointVecEnd,//CornerPoint& GunCheckPointTmp, 
	VectorType::iterator itCurOriginCheckPointColor,int ToBeCheckedGunIndex,
	float CurExpandedCheckPointPose, double GunDelay[][2],
	int& ColorStatus,//本喷枪位对应的前一状态，也就是应该保持的状态。
	bool bReverse)
{
	VectorTypePoint::iterator itExpandedCheckPointPos;
	int CurColorBitStatus;//本喷枪位对应的状态，也就是应该保持的状态。
	itCurOriginCheckPoint++;//指向下一个原始突变点
	if (itCurOriginCheckPoint==itOriginCheckPointVecEnd)//最后一个
	{
		return false;
	}
	itCurOriginCheckPointColor++;//下一个原始突变点的颜色
	bool NowBeforeTheOpenPoint=false;
	bool NowBeforeClosePoint=false;
	int AddStep=bReverse?-1:1;	
	int MovedStep=1;//扩充突变点在当前原始突变点后的第几个突变点前
	for (itCurOriginCheckPoint;itCurOriginCheckPoint!=itOriginCheckPointVecEnd;itCurOriginCheckPoint++)
	{
		float OpenPointTobeComp=itCurOriginCheckPoint->YPoint+AddStep*GunDelay[ToBeCheckedGunIndex][0];
		float ClosePointTobeComp=itCurOriginCheckPoint->YPoint+AddStep*GunDelay[ToBeCheckedGunIndex][1];
		if (bReverse)
		{
			if ( CurExpandedCheckPointPose>=OpenPointTobeComp)//扩充后就在这个原始突变点扩充之后，当前突变点的其他位应该保留这个原始突变点的前一个这些位的状态
			{
				//itExpandedCheckPointPos=itCurOriginCheckPoint--;
				NowBeforeTheOpenPoint=true;
			}
			if (CurExpandedCheckPointPose>=ClosePointTobeComp)
			{
				NowBeforeClosePoint=true;
			}
		}
		else
		{
			if ( CurExpandedCheckPointPose<=OpenPointTobeComp)//扩充后就在这个原始突变点扩充之后，当前突变点的其他位应该保留这个原始突变点的前一个这些位的状态
			{
				//itExpandedCheckPointPos=itCurOriginCheckPoint--;
				NowBeforeTheOpenPoint=true;
			}
			if (CurExpandedCheckPointPose<=ClosePointTobeComp)
			{
				NowBeforeClosePoint=true;
			}
		}
		
		if ( NowBeforeClosePoint|| NowBeforeTheOpenPoint)//扩充后的突变点在这,要么在开启延时前，要么在关闭延时前
		{
			//itExpandedCheckPointPos=itCurOriginCheckPoint--;
			itCurOriginCheckPoint--;
			CurColorBitStatus=((*itCurOriginCheckPointColor)&GunNumValue[(ToBeCheckedGunIndex)]);//对应喷枪位的状态取出来
			itCurOriginCheckPointColor--;
			int PreColorBitStatus;
			if (MovedStep>1)//用原始突变点状态作为前一状态
			{
				PreColorBitStatus=((*itCurOriginCheckPointColor)&GunNumValue[(ToBeCheckedGunIndex)]);//对应喷枪位的状态取出来
			}
			else//前一突变点如果是当前自己的突变点的话，状态正在改写，不能用原始突变点状态作为前一状态
				PreColorBitStatus=((ColorStatus)&GunNumValue[(ToBeCheckedGunIndex)]);//对应喷枪位的状态取出来
				//int ColorIndex=itExpandedCheckPointPos-itOriginCheckPointVecHead;
			
			///////////////将PreColorBitStatus覆盖回去ColorStatus///////////////////////////////////////////////////////////
			if (PreColorBitStatus)			{
				setbit(ColorStatus,ToBeCheckedGunIndex);
			}
			else
				clrbit(ColorStatus,ToBeCheckedGunIndex);//该关闭这位
			
			if ((!NowBeforeTheOpenPoint)&&CurColorBitStatus>0)//扩充点在延迟开启点之后，CurColorBitStatus该开
			{
				setbit(ColorStatus,ToBeCheckedGunIndex);

			}
			if ( (!NowBeforeClosePoint)&&CurColorBitStatus==0)//扩充点在延迟关闭点之后，CurColorBitStatus该关
			{
				clrbit(ColorStatus,ToBeCheckedGunIndex);//该关闭这位
			}
			break;
		}
		itCurOriginCheckPointColor++;//下一个原始突变点的颜色
		MovedStep++;
	}


	return NowBeforeClosePoint| NowBeforeTheOpenPoint;

}
bool DownValue::SetExpandedCheckPointsForEachGun(VectorType::iterator itCurColor,int GunIndex,
	float CheckPointPose, //CornerPoint& GunCheckPointTmp, 
	int& PreColor,
	CheckPointPairVecType& RepeatedCheckPoints,
	bool NeedOpen)
{
	bool bFunctioned=false;//是否在这个函数里做了改变
	int GunStatuTmp=((*itCurColor)&GunNumValue[(GunIndex)]);//这把枪该开还是关，看该位是否为1
	KeepRemainedBitNumVec.clear();
	for (int i=0;i<4;i++)
	{
		KeepRemainedBitNumVec.push_back(i);
	}
	if (NeedOpen)//提前开启点
	{
		if (GunStatuTmp>0)//该开
		{setbit(PreColor,GunIndex);	
		bFunctioned=true;
		}//实现是当前喷枪需要提前开就把PreColor的相应位置1
	}
	else//提前关闭点
	{
		if (GunStatuTmp==0)//该关
		{
			clrbit(PreColor,GunIndex);//实现是当前喷枪需要提前关就把PreColor的相应位置0
			bFunctioned=true;
		}
	}

	KeepRemainedBitNumVec.erase(remove(KeepRemainedBitNumVec.begin(), KeepRemainedBitNumVec.end(), GunIndex),KeepRemainedBitNumVec.end() );//删除掉处理过的枪号，剩下维持原状的枪号位
	//GunCheckPointTmp.YPoint=CheckPointPose;
	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//存储考虑延迟后的突变点位置
	CheckPointPairVecType::iterator itRepeatedCheckPoints=RepeatedCheckPoints.begin();
	/////////////////////////////////处理重复突变点/////////////////////////////////////////
	for (itRepeatedCheckPoints;itRepeatedCheckPoints!=RepeatedCheckPoints.end();itRepeatedCheckPoints++)
	{
		if (CheckPointPose==itRepeatedCheckPoints->second)//确实重复了
		{		
			int GunStatuTmp=((*itCurColor)&GunNumValue[(itRepeatedCheckPoints->first)]);//这把枪该开还是关，看该位是否为1
			if (NeedOpen)
			{
				if (GunStatuTmp>0)//该开
				{setbit(PreColor,(itRepeatedCheckPoints->first));	
				}//实现是当前喷枪需要提前开就把PreColor的相应位置1,可能和前面重复，不过没关系
			}
			else
			{
				if (GunStatuTmp==0)//该关
				{
					clrbit(PreColor,(itRepeatedCheckPoints->first));//实现是当前喷枪需要提前关就把PreColor的相应位置0
				}

			}
			KeepRemainedBitNumVec.erase(remove(KeepRemainedBitNumVec.begin(), KeepRemainedBitNumVec.end(), itRepeatedCheckPoints->first),KeepRemainedBitNumVec.end() );//删除掉处理过的枪号，剩下维持原状的枪号位
			//std::vector<int>::iterator HandledIndex=std::find(KeepRemainedBitNumVec.begin(),KeepRemainedBitNumVec.end(),itRepeatedCheckPoints->first);//删除掉处理过的枪号，剩下维持原状的枪号位
			//if (HandledIndex!=KeepRemainedBitNumVec.end())
			//{
			//	KeepRemainedBitNumVec.erase(std::find(KeepRemainedBitNumVec.begin(),KeepRemainedBitNumVec.end(),HandledIndex));
			//}
		}
	}
	///////////////////////////把没处理的位处理下///////////////////////////////////////////////

	return true;
}
bool DownValue::CheckOneExpandedCheckPoint(VectorType::iterator itCurColor,std::map< float,int>::iterator itSortedOpenCheckSets,
	std::map< float,int>::iterator itSortedCloseCheckSets,std::map< float,int>::iterator itAllDelayedCheckSets,
	std::map< float,int>& DelayMap,//延迟之后的突变点,导致开启延迟的枪号,
    std::map< float,int>& CloseDelayMap,//延迟之后的突变点,导致关闭延迟的枪号,
	CheckPointPairVecType RepeatedOpenCheckPoints, CheckPointPairVecType RepeatedCloseCheckPoints,
	int& PreColor,bool bReverse=false)
{
	bool bOpenCheck=false;//要检查开启延时点
	bool bCloseCheck=false;//要检查关闭延时点
	bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap.end());
	bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap.end());
	/////////////////////////////////如果既是延迟开的，又是延迟关的，则执行两次，不会矛盾/////////////////////////////////////////

	if (bOpenCheck)//是延迟开的突变点
	{
		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			PreColor,RepeatedOpenCheckPoints,true);
	}
	if (bCloseCheck)//是延迟关的突变点
	{
		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			PreColor,RepeatedCloseCheckPoints,false);
	}		

	//GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;//移到函数外
	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//存储考虑延迟后的突变点位置
	
	return true;
}
//bool DownValue::CheckOneExpandedCheckPoint(VectorType::iterator itCurColor,std::map< float,int>::iterator itSortedOpenCheckSets,
//	std::map< float,int>::iterator itSortedCloseCheckSets,std::vector<std::pair<float,int>>::iterator itAllDelayedCheckSets,
//	std::map< float,int>& DelayMap,//延迟之后的突变点,导致开启延迟的枪号,
//	std::map< float,int>& CloseDelayMap,//延迟之后的突变点,导致关闭延迟的枪号,
//	CheckPointPairVecType RepeatedOpenCheckPoints, CheckPointPairVecType RepeatedCloseCheckPoints,
//	int& PreColor,bool bReverse=false)
//{
//	bool bOpenCheck=false;//要检查开启延时点
//	bool bCloseCheck=false;//要检查关闭延时点
//	bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap.end());
//	bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap.end());
//	/////////////////////////////////如果既是延迟开的，又是延迟关的，则执行两次，不会矛盾/////////////////////////////////////////
//
//	if (bOpenCheck)//是延迟开的突变点
//	{
//		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
//			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
//			PreColor,RepeatedOpenCheckPoints,true);
//	}
//	if (bCloseCheck)//是延迟关的突变点
//	{
//		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
//			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
//			PreColor,RepeatedCloseCheckPoints,false);
//	}		
//
//	//GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;//移到函数外
//	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//存储考虑延迟后的突变点位置
//
//	return true;
//}

//倒着走时未处理，需要排序后再倒一次，一个突变点对同一把喷枪状态要么由开到关要么反之，要么维持原状，只有四种。不管扩充成几个不会矛盾
bool DownValue::CheckOneCheckPoint(const VectorType::iterator itCurColor,const VectorTypePoint::iterator itCheckPoint,int PreColor,double GunDelay[][2],unsigned GunNum=4,bool bReverse=false)
{
	//VectorTypePoint::iterator itCheckPoint=CheckPoint.begin();
	//VectorType::iterator itCurColor=CheckPointColorList.begin();
	int SavedPreColor= PreColor;
	std::map< float,int> DelayMap;//延迟之后的突变点,导致开启延迟的枪号,
	std::map< float,int> CloseDelayMap;//延迟之后的突变点,导致关闭延迟的枪号,<
	std::map<float,int>AllDelayPointMap;//所有延迟之后的突变点,导致延迟的枪号,<
	std::vector<float> OpenDelayPoints;
	std::vector<float> CloseDelayPoints;
	int AddStep=bReverse?-1:1;
	for (int i=0;i<GunNum;i++)
	{
		double OpenCheckPointAfterDelay=AddStep*GunDelay[i][0]+(double)itCheckPoint->YPoint;
		double CloseCheckPointAfterDelay=AddStep*GunDelay[i][1]+(double)itCheckPoint->YPoint;
		LimitCheckPoint(OpenCheckPointAfterDelay,CloseCheckPointAfterDelay,m_fMaxRange);
		OpenDelayPoints.push_back(OpenCheckPointAfterDelay);
		CloseDelayPoints.push_back(CloseCheckPointAfterDelay);
		DelayMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,i));
		CloseDelayMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,i));
		AllDelayPointMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,i));
		AllDelayPointMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,i));
	}
	if (bReverse)
	{
		std::map<float,int>::reverse_iterator rItAllDelayPointMap=AllDelayPointMap.rbegin();
		std::map<float,int>::reverse_iterator rItAllDelayPointMapEnd=AllDelayPointMap.rend();
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),rItAllDelayPointMap,rItAllDelayPointMapEnd);//留存最后的列冲突调整用
	
	}
	else
	{
		
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),AllDelayPointMap.begin(),AllDelayPointMap.end());//留存最后的列冲突调整用
	}
	////处理提前开启问题//////////////////////////////////////////////////////////////////////////
	std::map< float,int>::iterator itSortedOpenCheckSets= DelayMap.begin();//map它会自动按key值也就是坐标值从小到大排序,还会删除key值重复项。。。
	std::map< float,int>::iterator itSortedCloseCheckSets= CloseDelayMap.begin();//map它会自动按key值也就是坐标值从小到大排序
	std::map< float,int>::iterator itAllDelayedCheckSets;//=AllDelayPointMap.begin()
	//itSortedCheckPoint=DelayMap.begin();
	
	//int GunNumValue[4]={0x01,0x02,0x04,0x08};//各喷枪对应的号码值,仅限4把喷枪
	CornerPoint GunCheckPointTmp;

	GunCheckPointTmp=*itCheckPoint;//初始化为原来的单个检查点

	OneCheckPointExpandTmp.clear();//最后这一突变点延迟完成了扩充成GunNum个突变点
	ColorOfExpandedCheckPointsTmp.clear();
	
	bool bOpenCheck=false;//要检查开启延时点
	bool bCloseCheck=false;//要检查关闭延时点
	CheckPointPairVecType RepeatedOpenCheckPoints=CheckGetRepeatedDelay(OpenDelayPoints);
	CheckPointPairVecType RepeatedCloseCheckPoints=CheckGetRepeatedDelay(CloseDelayPoints);
	m_vStackOfAllRepeatedOpenCheckPoints.insert(m_vStackOfAllRepeatedOpenCheckPoints.end(),RepeatedOpenCheckPoints.begin(),RepeatedOpenCheckPoints.end());//留存最后的列冲突调整用
	m_vStackOfAllRepeatedCloseCheckPoints.insert(m_vStackOfAllRepeatedCloseCheckPoints.end(),RepeatedCloseCheckPoints.begin(),RepeatedCloseCheckPoints.end());//留存最后的列冲突调整用
	if (bReverse)
	{
		itAllDelayedCheckSets=AllDelayPointMap.end();//最大的
		itAllDelayedCheckSets--;
		for ( int i=0; i<AllDelayPointMap.size();i++)//由大到小，对逐个延迟后的突变点进行判断是否需要提前开关
		{			
			PreColor=findPreColorAfterPreExpand(itAllDelayedCheckSets->first,PreColor,true,bReverse);
			CheckOneExpandedCheckPoint(itCurColor,itSortedOpenCheckSets,itSortedCloseCheckSets,itAllDelayedCheckSets,
				DelayMap,CloseDelayMap,RepeatedOpenCheckPoints,RepeatedCloseCheckPoints,
				PreColor,bReverse);
			///////////////前面没处理的位再处理一下///////////////////////////////////////////////////////////
			if (itAllDelayedCheckSets->first<itCheckPoint->YPoint)//往前扩充的不需要处理
			{
				for (std::vector<int>::iterator itUnHandledBit=KeepRemainedBitNumVec.begin(); itUnHandledBit!=KeepRemainedBitNumVec.end();itUnHandledBit++)
				{
					CheckAndSetRemainColor(itCheckPoint,m_itChecPointVecEnd,//CornerPoint& GunCheckPointTmp, 
						itCurColor,*itUnHandledBit,	itAllDelayedCheckSets->first, GunDelay,PreColor,//本喷枪位对应的前一状态，也就是应该保持的状态。
						bReverse);
				}
			}
			
			GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;
			OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//存储考虑延迟后的突变点位置
			ColorOfExpandedCheckPointsTmp.push_back(PreColor);//存储考虑延迟后的突变点处的颜色，与点一一对应
			itAllDelayedCheckSets--;

		}//到最后一个点的PreColor应与当前颜色一样，
	}
	else
	{
		itAllDelayedCheckSets=AllDelayPointMap.begin();
		for ( itAllDelayedCheckSets; itAllDelayedCheckSets!=AllDelayPointMap.end();itAllDelayedCheckSets++)//由小到大，对逐个延迟后的突变点进行判断是否需要提前开关
		{

			//bOpenCheck=false;
			//bCloseCheck=false;

			//bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap);
			//bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap);
			///////////////////////////////////如果既是延迟开的，又是延迟关的，则执行两次，不会矛盾/////////////////////////////////////////
			//if (bOpenCheck)//是延迟开的突变点
			//{
			//	SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			//		itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			//		PreColor,RepeatedOpenCheckPoints,true);
			//}
			//if (bCloseCheck)//是延迟关的突变点
			//{
			//	SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			//		itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			//		PreColor,RepeatedCloseCheckPoints,false);
			//}		
			//ColorOfExpandedCheckPointsTmp.push_back(PreColor);//存储考虑延迟后的突变点处的颜色，与点一一对应
			PreColor=findPreColorAfterPreExpand(itAllDelayedCheckSets->first,PreColor,true,bReverse);
			CheckOneExpandedCheckPoint(itCurColor,itSortedOpenCheckSets,itSortedCloseCheckSets,itAllDelayedCheckSets,
				DelayMap,CloseDelayMap,RepeatedOpenCheckPoints,RepeatedCloseCheckPoints,
				PreColor,bReverse);
			///////////////前面没处理的位再处理一下///////////////////////////////////////////////////////////
			if (itAllDelayedCheckSets->first>itCheckPoint->YPoint)//往前扩充的不需要处理
			{
				for (std::vector<int>::iterator itUnHandledBit=KeepRemainedBitNumVec.begin(); itUnHandledBit!=KeepRemainedBitNumVec.end();itUnHandledBit++)
				{
					CheckAndSetRemainColor(itCheckPoint,m_itChecPointVecEnd,//CornerPoint& GunCheckPointTmp, 
						itCurColor,*itUnHandledBit,	itAllDelayedCheckSets->first, GunDelay,PreColor,//本喷枪位对应的前一状态，也就是应该保持的状态。
						bReverse);
				}
			}
			GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;
			OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//存储考虑延迟后的突变点位置
			ColorOfExpandedCheckPointsTmp.push_back(PreColor);//存储考虑延迟后的突变点处的颜色，与点一一对应
			
			
		}//到最后一个点的PreColor应与当前颜色一样，
	}
	
	

	return true;

}
void DownValue::CoverGivenBitColorForGivenPoint(int BackCheckPoint,int CurCheckPoint,int GunIndex)
{
	int TheNeedCoverColorBit=m_CheckPointColorListWithDelay[CurCheckPoint]&GunNumValue[GunIndex];//需要把哪些喷枪对应的位的颜色提取出来，覆盖回去冲突点的相应位
	if (TheNeedCoverColorBit>0)	{
		setbit(m_CheckPointColorListWithDelay[BackCheckPoint],GunIndex);		
	}
	else	{
		clrbit(m_CheckPointColorListWithDelay[BackCheckPoint],GunIndex);
	}
}
void DownValue::BackSetConflictPointColor(std::vector<std::pair<float,int>>::iterator itvPreCheckPoint,
	std::vector<std::pair<float,int>>::iterator itvCurCheckPoint,
	CheckPointPairVecType::iterator& itRepeatedCheckPoints,CheckPointPairVecType::iterator& itRepeatedCloseCheckPoints)
{
	int GunIndex=itvCurCheckPoint->second;//要回溯的枪号
	static unsigned MaxCheckPointNum=m_GunModelAndDistance.GunNember*2;
	float BackCheckPoint=itvPreCheckPoint->first;//要回溯的突变点
	unsigned BackCheckPointIndex=itvPreCheckPoint-m_vStackOfAllDelayPointMapInRow.begin();////要回溯的突变点的下标
	if (BackCheckPointIndex==6)
	{
		BackCheckPointIndex=6;
	}
	unsigned CurCheckPointIndex=itvCurCheckPoint-m_vStackOfAllDelayPointMapInRow.begin();
	CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
	///////////////这个原始突变点有重复的话,处理重复的开启点///////////////////////////////////////////////////////////
	unsigned FoundRepeated=0;//找到的重复点个数一轮重复开启点是集中的几个连续的点,同常个数不超过喷枪数乘2，以此来减少搜索重复点的范围
	CheckPointPairVecType::iterator itRepeatedCheckPointsMoved=itRepeatedCheckPoints;//避免每次从头开始找

	
	while (JudgeRepeatedCheckPoint(itvCurCheckPoint->first,itRepeatedCheckPointsMoved,m_vStackOfAllRepeatedOpenCheckPoints.end()))//itRepeatedCheckPoints会在函数里被移动，注意
	{//从itRepeatedCheckPointsMoved开始往后查找， 
		/*itRepeatedCheckPointsMoved=itRepeatedCheckPoints;*/
		int GunIndex=itRepeatedCheckPointsMoved->first;//导致重复的枪号
		CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
		itRepeatedCheckPointsMoved++;
		itRepeatedCheckPoints=itRepeatedCheckPointsMoved;//移动基准，传出去
		FoundRepeated++;
		if (itRepeatedCheckPointsMoved==m_vStackOfAllRepeatedOpenCheckPoints.end()||FoundRepeated>MaxCheckPointNum)
		{break;
		}
	}
	///////////////这个原始突变点有重复的话,处理重复的关闭启点///////////////////////////////////////////////////////////
	itRepeatedCheckPointsMoved=itRepeatedCloseCheckPoints;//避免每次从头开始找
	FoundRepeated=0;
	while (JudgeRepeatedCheckPoint(itvCurCheckPoint->first,itRepeatedCheckPointsMoved,m_vStackOfAllRepeatedCloseCheckPoints.end()))//itRepeatedCheckPoints会在函数里被移动，注意
	{
		int GunIndex=itRepeatedCheckPointsMoved->first;//导致重复的枪号
		CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
		itRepeatedCheckPointsMoved++;
		itRepeatedCloseCheckPoints=itRepeatedCheckPointsMoved;
		FoundRepeated++;
		if (itRepeatedCheckPointsMoved==m_vStackOfAllRepeatedCloseCheckPoints.end()||FoundRepeated>MaxCheckPointNum)
		{break;
		}

	}
	
	

	
}
//正向移动的话后一个的突变点比前面某个的还要小的话,相应喷枪的位应以超越到前面的那个为准
bool DownValue::CheckConflictFromDifferentOriginCheckPoint(std::vector<std::pair<float,int>>::iterator it_CurCheckPoint,std::vector<std::pair<float,int>>& StackOfAllDelayPointMapInRow
	,CheckPointPairVecType::iterator& itRepeatedCheckPoints,CheckPointPairVecType::iterator& itRepeatedCloseCheckPoints
	,bool bReverse)
{
	std::vector<std::pair<float,int>>::iterator itvAllUnSortCheckPoint=it_CurCheckPoint;
	//itvAllUnSortCheckPoint=find_if(StackOfAllDelayPointMapInRow.begin(),StackOfAllDelayPointMapInRow.end(),CheckPointMap_finder(fCurCheckPoint));//找到这个突变点在集合中的位置
	std::vector<std::pair<float,int>>::iterator itPre;
	if (itvAllUnSortCheckPoint==StackOfAllDelayPointMapInRow.begin())
	{
		itPre=StackOfAllDelayPointMapInRow.begin();//前一个
	}
	else
		itPre=itvAllUnSortCheckPoint-1;
		
	bool bFoundConflict=false;
	//itvAllUnSortCheckPoint++;
	int CurIndex=0;
	int BackPosIndex=0;
	int CountTmp=0;
	//CheckPointPairVecType::iterator itRepeatedCheckPoints=m_vStackOfAllRepeatedOpenCheckPoints.begin();
	//CheckPointPairVecType::iterator itRepeatedCloseCheckPoints=m_vStackOfAllRepeatedCloseCheckPoints.begin();
	
	//for (itvAllUnSortCheckPoint;itvAllUnSortCheckPoint!=StackOfAllDelayPointMapInRow.end();itvAllUnSortCheckPoint++)
	//{
	//	CountTmp++;
		if (bReverse)//逆向喷涂运动
		{
			while (itPre!=StackOfAllDelayPointMapInRow.begin()){
				if (it_CurCheckPoint->first >itPre->first )//出现新的比老的大，冲突 
				{
					BackSetConflictPointColor(itPre,itvAllUnSortCheckPoint,itRepeatedCheckPoints,itRepeatedCloseCheckPoints);
				}							
				itPre--;//出现冲突就去回溯，找到新的冲突点按照顺序应该排到谁的后面去
				//bFoundConflict=true;
				/*BackPosIndex--;*/

			}
		}
		else
		{
			
		/*	if (it_CurCheckPoint->first==120)
			{
				BackPosIndex=CurIndex++;
			}*/
			while (itPre!=StackOfAllDelayPointMapInRow.begin()){
				if (it_CurCheckPoint->first <itPre->first )//出现新的比老的小，冲突 
				{
					BackSetConflictPointColor(itPre,itvAllUnSortCheckPoint,itRepeatedCheckPoints,itRepeatedCloseCheckPoints);
				}							
				itPre--;//出现冲突就去回溯，找到新的冲突点按照顺序应该排到谁的后面去
				//bFoundConflict=true;
				//BackPosIndex--;

			}
			//bFoundConflict=false;

		}
		//itPre=itvAllUnSortCheckPoint;
	/*}*/
	return false;
}
bool lessmark(const std::vector<std::pair<float,int>>::iterator& stItem1, const std::vector<std::pair<float,int>>::iterator& stItem2)
{
	return stItem1->first < stItem2->first;
}
int DownValue::findPreColorAfterPreExpand(float CurCheckPointPos,int PreColor,bool bSearchNewPreColor=true,bool bReverse=false)
{
	if (!bSearchNewPreColor)
	{return PreColor;
	}
	std::vector<std::pair<float,int>>::iterator itm_vStackOfAllDelayPointMapInRow=m_vStackOfAllDelayPointMapInRow.begin();
	//std::map<float,int> NearestPrePoints;//在它前面的所有点集
	std::vector<std::pair<float,int>>::iterator NearestPrePoint=m_vStackOfAllDelayPointMapInRow.begin();////在它前面的最接近自己的点，初值为最远离自己的那个起始点
	//bool bFounded=false;//发现了在某个突变点前面
	if (!bReverse)
	{
		for (itm_vStackOfAllDelayPointMapInRow;itm_vStackOfAllDelayPointMapInRow!=m_vStackOfAllDelayPointMapInRow.end();itm_vStackOfAllDelayPointMapInRow++)//把所有比他小的都存起来
		{
			if (itm_vStackOfAllDelayPointMapInRow->first<CurCheckPointPos-TOLERANCE_OF_SAME_POINT)//多减TOLERANCE_OF_SAME_POINT是为了避免有时一个是119.999，一个是120.001，两者其实是一个点，应特别注意
			{
				//bFounded=true;
				
				if (NearestPrePoint->first<itm_vStackOfAllDelayPointMapInRow->first)
				{
					NearestPrePoint=itm_vStackOfAllDelayPointMapInRow;
				}
				//NearestPrePoint.push_back((itm_vStackOfAllDelayPointMapInRow));
			}
		}
	}
	else
	{
		for (itm_vStackOfAllDelayPointMapInRow;itm_vStackOfAllDelayPointMapInRow!=m_vStackOfAllDelayPointMapInRow.end();itm_vStackOfAllDelayPointMapInRow++)//把所有比他小的都存起来
		{
			if (itm_vStackOfAllDelayPointMapInRow->first>CurCheckPointPos+TOLERANCE_OF_SAME_POINT)//多减TOLERANCE_OF_SAME_POINT是为了避免有时一个是119.999，一个是120.001，两者其实是一个点，应特别注意
			{
				//bFounded=true;
				//NearestPrePoint.push_back((itm_vStackOfAllDelayPointMapInRow));
				if (NearestPrePoint->first>itm_vStackOfAllDelayPointMapInRow->first)
				{
					NearestPrePoint=itm_vStackOfAllDelayPointMapInRow;
				}
			}
		}
	}
	
	
	if (NearestPrePoint!=m_vStackOfAllDelayPointMapInRow.begin())//找到了此前的状态级
	//if (bFounded)
	{
		int ColorIndex=NearestPrePoint-m_vStackOfAllDelayPointMapInRow.begin();
		if (bReverse)
		{
			//std::sort(NearestPrePoint.begin(),NearestPrePoint.end(),lessmark);
			
			return ColorIndex>=m_CheckPointColorListWithDelay.size()?PreColor:m_CheckPointColorListWithDelay[ColorIndex];

		}
		else{
			//std::sort(NearestPrePoint.begin(),NearestPrePoint.end(),lessmark);
			//int ColorIndex=NearestPrePoint-m_vStackOfAllDelayPointMapInRow.begin();
			return ColorIndex>=m_CheckPointColorListWithDelay.size()?PreColor:m_CheckPointColorListWithDelay[ColorIndex];
		}
			
	}
	else
		return 0;//不在任何突变点之前，只能是第一个，则前色必须为0
}
bool DownValue::CheckEveryGunDelayAndAddCheckPoint(VectorTypePoint CheckPoint,VectorType CheckPointColorList,double GunDelay[][2],unsigned GunNum=4,bool bReverse=false)
{
	if (CheckPoint.size()!=CheckPointColorList.size())
	{return false;
	}

	VectorTypePoint::iterator itCheckPoint=CheckPoint.begin();
	VectorType::iterator itCurColor=CheckPointColorList.begin();
	m_itChecPointVecEnd=CheckPoint.end();
	ClearAllCheckPointsBackUpVecInRow();
	//////////////////////////////第一个特殊处理////////////////////////////////////////////
	CheckOneCheckPoint(CheckPointColorList.begin(),CheckPoint.begin(),0,GunDelay, GunNum,bReverse);
	int PreColor=*itCurColor;
	itCheckPoint++;
	itCurColor++;
	m_CheckPointListWithDelay.insert(m_CheckPointListWithDelay.end(),OneCheckPointExpandTmp.begin(),OneCheckPointExpandTmp.end());
	m_CheckPointColorListWithDelay.insert(m_CheckPointColorListWithDelay.end(),ColorOfExpandedCheckPointsTmp.begin(),ColorOfExpandedCheckPointsTmp.end());//存入
	//////////////////////////////////////////////////////////////////////////
	for(itCheckPoint;itCheckPoint!=CheckPoint.end();itCheckPoint++)//逐个原始突变点
	{
		CheckOneCheckPoint(itCurColor,itCheckPoint,PreColor,GunDelay, GunNum,bReverse);
		PreColor=*itCurColor;
		itCurColor++;
		m_CheckPointListWithDelay.insert(m_CheckPointListWithDelay.end(),OneCheckPointExpandTmp.begin(),OneCheckPointExpandTmp.end());
		m_CheckPointColorListWithDelay.insert(m_CheckPointColorListWithDelay.end(),ColorOfExpandedCheckPointsTmp.begin(),ColorOfExpandedCheckPointsTmp.end());
	}
	
	/////////////////////////前面只考虑了单个突变点扩充后的先后顺序问题，但不同突变点扩充后，可能导致需要再次排序,这里只简单排序，未考虑排序后对颜色值的影响，存在潜在bug/////////////////////////////////////////////////
	std::map<float,int> PoseAndIndex;//各突变点处的坐标与对应的编号，0开始的先后顺序编号
	std::map<float,int>::iterator itPoseAndInd;
	int i=0;
	if (m_CheckPointColorListWithDelay.size()!=m_CheckPointListWithDelay.size()||m_CheckPointColorListWithDelay.size()!=m_vStackOfAllDelayPointMapInRow.size())//出错
	{
		return false;//
	}
	//VectorTypePoint::iterator itPrePoint=m_CheckPointListWithDelay.begin();//
	
	std::vector<std::pair<float,int>>::iterator it_CurCheckPoint=m_vStackOfAllDelayPointMapInRow.begin();
	CheckPointPairVecType::iterator itRepeatedCheckPoints=m_vStackOfAllRepeatedOpenCheckPoints.begin();
	CheckPointPairVecType::iterator itRepeatedCloseCheckPoints=m_vStackOfAllRepeatedCloseCheckPoints.begin();
	//std::map<float,int>::reverse_iterator itReverse_PoseAndInd;
	std::map<float,int>::iterator itCompPrePoseAndInd;
	for (VectorTypePoint::iterator itAllPoint=m_CheckPointListWithDelay.begin();itAllPoint!=m_CheckPointListWithDelay.end();itAllPoint++)//排序
	{
		//itPoseAndInd=PoseAndIndex.end();			
		//if (itPoseAndInd!=PoseAndIndex.begin())//防止在PoseAndIndex第一个元素时出错
		//{
		//	itPoseAndInd--;	//PoseAndIndex中的最后一个元素
		bool bFoundSamePoint=false;
			for (itCompPrePoseAndInd=PoseAndIndex.begin();itCompPrePoseAndInd!=PoseAndIndex.end();itCompPrePoseAndInd++)//在此前所有排过序的点中找是否有重合点
			{
				if (fabs(itAllPoint->YPoint-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//新的待插入的与老的PoseAndIndex中的重合点
				{
					//PoseAndIndex[itAllPoint->YPoint]=i;//新的覆盖老的
					itCompPrePoseAndInd->second=i;//新的覆盖老的
					bFoundSamePoint=true;
				}
			}
			
			//////////////////////////在最终排序前，把出现冲突的原素的颜色重设一下////////////////////////////////////////////////
			//正向移动的话后一个的突变点比前面某个的还要小的话,相应喷枪的位，
		/*	CheckConflictFromDifferentOriginCheckPoint(it_CurCheckPoint, m_vStackOfAllDelayPointMapInRow,itRepeatedCheckPoints,itRepeatedCloseCheckPoints,bReverse);
			it_CurCheckPoint++;
			i++;
			continue;*/

		//}		
		//////////////////////////在最终排序前，把出现冲突的原素的颜色重设一下////////////////////////////////////////////////
		//正向移动的话后一个的突变点比前面某个的还要小的话,相应喷枪的位，
		CheckConflictFromDifferentOriginCheckPoint(it_CurCheckPoint, m_vStackOfAllDelayPointMapInRow,itRepeatedCheckPoints,itRepeatedCloseCheckPoints,bReverse);
		it_CurCheckPoint++;
		if (!bFoundSamePoint)
		{
			PoseAndIndex.insert(std::map<float,int>::value_type(itAllPoint->YPoint,i));
		}
		//////////////////////////////////////////////////////////////////////////		
		//itPrePoint++;
		i++;
	}//先排序得出索引
	VectorTypePoint SortedPointsTmp;
	VectorType SortedPointsColorTmp;
	if (bReverse)//逆向运动
	{
		std::map<float,int>::reverse_iterator itSortedIndex=PoseAndIndex.rbegin();//逆序迭代器
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.rend();itSortedIndex++)//按索引形成新的序列
		{
			SortedPointsTmp.push_back(m_CheckPointListWithDelay[itSortedIndex->second]);
			SortedPointsColorTmp.push_back(m_CheckPointColorListWithDelay[itSortedIndex->second]);
			//vStackOfAllDelayPointMapInRowTmp.push_back(m_vStackOfAllDelayPointMapInRow[itSortedIndex->second]);//排序零时存在这里
			//itSortedIndex++;
		}
	}
	else
	{
		std::map<float,int>::iterator itSortedIndex=PoseAndIndex.begin();
		//std::vector<std::pair<float,int>> vStackOfAllDelayPointMapInRowTmp;//m_vStackOfAllDelayPointMapInRow中的内容排序后零时存在这里
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.end();itSortedIndex++)//按索引形成新的序列
		{
			SortedPointsTmp.push_back(m_CheckPointListWithDelay[itSortedIndex->second]);
			SortedPointsColorTmp.push_back(m_CheckPointColorListWithDelay[itSortedIndex->second]);
			//vStackOfAllDelayPointMapInRowTmp.push_back(m_vStackOfAllDelayPointMapInRow[itSortedIndex->second]);//排序零时存在这里
			//itSortedIndex++;
		}
	}	
	
	m_CheckPointListWithDelay=SortedPointsTmp;
	m_CheckPointColorListWithDelay=SortedPointsColorTmp;
	//m_vStackOfAllDelayPointMapInRow=vStackOfAllDelayPointMapInRowTmp;


	/*for (it_CurCheckPoint;it_CurCheckPoint<m_vStackOfAllDelayPointMapInRow.end();it_CurCheckPoint++)
	{
	}*/
	

	

	return true;

}

bool DownValue::StartOpenPlc(CString Directory,CoordinateFrame m_CoordinateFrame,double GunDelay[][2],double GunDelayInReverse[][2])
{
	CString PointXY;             //喷涂起点坐标
	float ChangedFrameX=m_CoordinateFrame.BeginXPoint*cos(m_CoordinateFrame.FrameRange)+m_CoordinateFrame.BeginYPoint*sin(m_CoordinateFrame.FrameRange);  //得到在新坐标系下起点坐标
	float ChangedFrameY=(m_CoordinateFrame.BeginYPoint+2)*cos(m_CoordinateFrame.FrameRange)-m_CoordinateFrame.BeginXPoint*sin(m_CoordinateFrame.FrameRange);
	PointXY.Format("x %f y %f\n",ChangedFrameX,ChangedFrameY);
	CString DirectoryFilePlc="";
	DirectoryFilePlc=Directory+"\\DownPlcAndMotion.pmc";
	FILE  *fp=fopen((LPSTR)(LPCSTR)DirectoryFilePlc,"w");
	if (!fp)
	{
		return false;
	}
	std::vector<PerGunCondition>::iterator m_PerGun;
	m_PerGun=m_EveryPerCondition.begin();
	m_JudgeOneChoice=1;
	fprintf(fp,"&1\n");
	CString FrameRotateXCstring="";
	float FrameRotateX=250*cos(m_CoordinateFrame.FrameRange);               //以下为旋转轴,用于补偿
	float FrameRotateY=250*sin(m_CoordinateFrame.FrameRange);
	CString FrameX="";
	FrameX.Format("%f",FrameRotateX);
	CString FrameY;
	FrameY.Format("%f",FrameRotateY);
	CString FrameYL="";
	if (FrameRotateY<0)
	{
		FrameYL.Format("%f",-FrameRotateY);
		FrameRotateXCstring="#3->"+FrameX+"X+"+FrameYL+"Y #2->"+FrameY+"X+"+FrameX+"Y";
	}
	else
		FrameRotateXCstring="#3->"+FrameX+"X-"+FrameY+"Y #2->"+FrameY+"X+"+FrameX+"Y";
	//FrameRotateXCstring.Format("#1->%fX-%Y #2->%fX+%fY",FrameRotateX,FrameRotateY,FrameRotateY,FrameRotateX);
	fprintf(fp,FrameRotateXCstring);
	fprintf(fp,"\n");
	fprintf(fp,"close\n");
	fprintf(fp,"open prog 30\n");
	fprintf(fp,"clear\n");
	fprintf(fp,"linear\n");
	fprintf(fp,"abs\n");
	fprintf(fp,"ta 10\n");
	fprintf(fp,"ts 0\n");
	CString CornerSpeed="";
	CornerSpeed.Format("F %d\n",(int)Speed*SPEEDTRANSLATE);
	fwrite(CornerSpeed,strlen(CornerSpeed),1,fp);     //直线速度
	fprintf(fp,"dwell 500\n");
	fprintf(fp,PointXY);
	fprintf(fp,"HOMEZ 2,3\n");
	JudgeIsFirstCorner=false;
	std::vector<GunPerBeginAndEnd>::iterator m_PointIndex;
	std::map<int,VectorType>::iterator m_GunColor;                     //单元颜色迭代器
	std::map<int,VectorTypePoint>::iterator m_ChangedColorPosition;
	m_GunColor=GunColorAll.begin();
	m_ChangedColorPosition=AllChangeColorPosition.begin();
	m_PointIndex=m_GunPerBeginAndEnd.begin();                            //坐标迭代器
	//首先解决从喷涂区域顶点到第一个需要喷涂点

	int IndexBeginColown=0;  //找出应该喷涂喷涂的第一列在整个区域的列号
	int ColownValue=m_GunPerBeginAndEnd.size();                       //当前区域需要喷涂的列数
	bool clockwise=true;                                              //喷枪是顺时针还是逆时针运动
	WriteAllDelayParm( GunDelay,fp);
	bool ReverseMove=false;
	//unsigned GunNum=m_GunModelAndDistance.GunNember;
	if (m_GunModelAndDistance.GunNember!=1)                           //多喷枪生成代码
	{
		for (int Index=0;Index<m_GunPerBeginAndEnd.size();Index++,m_GunColor++,m_ChangedColorPosition++)//每列
		{
			JudgeIsOrFirstPer=true;
			VectorType m_EveryVectorl=m_GunColor->second;    //每一列的单元颜色
			VectorTypePoint m_VectorTypePosition=m_ChangedColorPosition->second;
			m_fMaxRange=m_GunPerBeginAndEnd[Index].End>m_GunPerBeginAndEnd[Index].Begin?m_GunPerBeginAndEnd[Index].End:m_GunPerBeginAndEnd[Index].Begin;
			//if (!CheckEveryGunDelayAndAddCheckPoint(m_VectorTypePosition,m_EveryVectorl,GunDelay,m_GunModelAndDistance.GunNember,ReverseMove))//增加延时信息
			if (!CalculateAllDelayedCheckPointInRow(m_VectorTypePosition,m_EveryVectorl,ReverseMove?GunDelayInReverse:GunDelay,m_GunModelAndDistance.GunNember,ReverseMove))//将延迟考虑进去
			{
				return false;//出错
			}
			
			VectorType::iterator m_ColumnUnitColor=m_CheckPointColorListWithDelay.begin();
			VectorType::iterator ItPreUnitColor;
			VectorTypePoint::iterator itPoints=m_CheckPointListWithDelay.begin();
			WriteCheckPointsAndStatus(m_GunModelAndDistance.GunNember,(*itPoints).XPoint,(*itPoints).YPoint, GunDelay,fp,m_ColumnUnitColor);
			itPoints++;
			ItPreUnitColor= m_ColumnUnitColor;
			m_ColumnUnitColor++;
			//VectorTypePoint::iterator m_ColumnnUnitPosition=m_CheckPointListWithDelay.begin();
			for (itPoints;itPoints!=m_CheckPointListWithDelay.end();itPoints++)
			{
				WriteCheckPointsAndStatus2(m_GunModelAndDistance.GunNember,(*itPoints).XPoint,(*itPoints).YPoint, GunDelay,fp,m_ColumnUnitColor,ItPreUnitColor);
				ItPreUnitColor= m_ColumnUnitColor;
				m_ColumnUnitColor++;
			}
			ReverseMove=!ReverseMove;
			//OldColor=*m_ColumnUnitColor;
		}

	}
	else///////多色喷枪未修改考虑延迟
	{
		for (m_PointIndex,m_GunColor;m_PointIndex!=m_GunPerBeginAndEnd.end();m_PointIndex++,m_GunColor++)
		{
			int X=(*m_PointIndex).Index*PerWidth;            //X坐标
			int BeginY=(*m_PointIndex).Begin;                //每一列的起始坐标
			int EndY=(*m_PointIndex).End;                    //每一列的终止坐标
			CString Storage="";
			Storage.Format("x %d y %d\n",X,BeginY);
			fwrite(Storage,strlen(Storage),1,fp);
			CString EveryCellColor=CompileGunCondition(0);
			fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
			VectorType m_EveryVectorl=m_GunColor->second;    //每一列的单元颜色
			VectorType::iterator m_ColumnUnitColor=m_EveryVectorl.begin();
			for (int i=0;i<(m_EveryVectorl.size()-1);i++,m_ColumnUnitColor++)
			{
				if (clockwise==true)                        //喷枪顺势针     
				{
					int Y=BeginY+(i+1)*PerHeight;
					CString Storage="";
					Storage.Format("x %d y %d\n",X,Y);
					fwrite(Storage,strlen(Storage),1,fp);
				}
				else                                       //喷枪逆时针
				{
					int Y=EndY+(m_EveryVectorl.size()-1)*PerHeight-(i+1)*PerHeight;
					CString Storage="";
					Storage.Format("x %d y %d\n",X,Y);
					fwrite(Storage,strlen(Storage),1,fp);
				}
				CString EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
				fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
			}
			clockwise=!clockwise;              //顺时针与逆时针交替进行
		}
	}

	/*CString Storage="";
	Storage.Format("x %d y %d\n",0,0);
	fwrite(Storage,strlen(Storage),1,fp);
	CString EveryCellColor=CompileGunCondition(0);
	fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);*/
	fprintf(fp,"close\n");
	fclose(fp);
	//for (m_PointIndex,m_GunColor=GunColorAll.begin();m_PointIndex<m_GunPerBeginAndEnd.end();m_PointIndex++,m_GunColor++)
	//{
	//	int X=(*m_PointIndex).Index*PerHeight;            //X坐标
	//	if (JudgeIsFirstCorner==true)          //当每一次转角时，要有移动坐标
	//	{
	//		CString CornerSpeed="";
	//		CornerSpeed.Format("F %d\n",(int)Speed*SPEEDTRANSLATE);
	//		fwrite(CornerSpeed,strlen(CornerSpeed),1,fp);          //直线速度
	//		CString StoragePoint="";
	//		StoragePoint.Format("x %d y %d\n",X,m_CornerPoint.YPoint);
	//		fwrite(StoragePoint,strlen(StoragePoint),1,fp);
	//		CString EveryCellColorCorner=CompileGunCondition(m_CornerPointColor);
	//		fwrite(EveryCellColorCorner,strlen(EveryCellColorCorner),1,fp);
	//	}
	//	CString LineSpeed="";
	//	LineSpeed.Format("F %d\n",LINESPEED);
	//	fwrite(LineSpeed,strlen(LineSpeed),1,fp);          //直线速度
	//	JudgeIsFirstCorner=true;
	//	int tt=m_GunColor->first;
	//	VectorType m_EveryVectorl=m_GunColor->second;
	//	VectorType::iterator m_ColumnColorIndex;
	//	m_ColumnColorIndex=m_EveryVectorl.begin();
	//	int Size=m_EveryVectorl.size()-1;    //单元比喷色少1
	//	for (int Index=1;Index<=Size;Index++,m_ColumnColorIndex++)
	//	{
	//		int YChangePoint=PerHeight;
	//		if ((*m_PointIndex).End>(*m_PointIndex).Begin)
	//		{
	//			YChangePoint=YChangePoint;
	//		}
	//		else
	//			YChangePoint=-YChangePoint;              //当一列终点坐标比起点坐标小时，Y轴坐标应变小
	//		int Y=(*m_PointIndex).Begin+Index*YChangePoint;     //Y坐标
	//		if (Y==0)
	//		{
	//			Y=5;
	//		}
	//		CString Storage="";
	//		Storage.Format("x %d y %d\n",X,Y);
	//		fwrite(Storage,strlen(Storage),1,fp);
	//		CString EveryCellColor=CompileGunCondition((*m_ColumnColorIndex));
	//		fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
	//		if (Index==Size)
	//		{
	//			m_CornerPoint.XPoint=X;
	//			m_CornerPoint.YPoint=Y;
	//		}
	//	}
	//	m_CornerPointColor=(*m_ColumnColorIndex);   //因为喷枪状态
	//}
	//CString StoragePointEnd="";
	//StoragePointEnd.Format("x %d y %d\n",m_CornerPoint.XPoint,m_CornerPoint.YPoint);
	//fwrite(StoragePointEnd,strlen(StoragePointEnd),1,fp);
	//CString EndCellColorCorner=CompileGunCondition(0);            //最终全部关闭
	//fwrite(EndCellColorCorner,strlen(EndCellColorCorner),1,fp);
	//fprintf(fp,"close\n");
	//fclose(fp);
	return true;
}
void DownValue::CheckHeight(float Height)
{
	HeightPer=(int)Height;
}
// 写入延迟参数

bool DownValue::WriteDelayParm(float OpenDelay, float CloseDelay,FILE* fp)
{
/*	CString Storage="";
	Storage.Format("Op %f Cl %f\n",OpenDelay,CloseDelay);
	fwrite(Storage,strlen(Storage),1,fp);*/	

	return false;
}
////////////////////方案2//////////////////////////////////////////////////////
void DownValue::ExpandOneOriginCheckPoint(
	std::map< float,int>& DelayMap,//延迟之后的突变点,导致开启延迟的枪号,
	std::map< float,int>& CloseDelayMap,//延迟之后的突变点,导致关闭延迟的枪号,<
	std::map<float,int>&AllDelayPointMap,//所有延迟之后的突变点,导致延迟的枪号,<
	std::vector<float>& OpenDelayPoints,
	std::vector<float>& CloseDelayPoints,
	const VectorTypePoint::iterator itCheckPoint,const int OriginPointIndex,
	const double GunDelay[][2],const unsigned GunNum=4,const bool bReverse=false	
	
	)
{
	int AddStep=bReverse?-1:1;
	for (int i=0;i<GunNum;i++)
	{
		double OpenCheckPointAfterDelay=AddStep*GunDelay[i][0]+(double)itCheckPoint->YPoint;
		double CloseCheckPointAfterDelay=AddStep*GunDelay[i][1]+(double)itCheckPoint->YPoint;
		LimitCheckPoint(OpenCheckPointAfterDelay,CloseCheckPointAfterDelay,m_fMaxRange);
		/*OpenDelayPoints.push_back(OpenCheckPointAfterDelay);
		CloseDelayPoints.push_back(CloseCheckPointAfterDelay);
		DelayMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,i));
		CloseDelayMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,i));*/
		AllDelayPointMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,i));
		AllDelayPointMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,i));
		m_StackOfAllOpenDelayMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,i));
		m_StackOfAllCloseDelayMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,i));
	/*	m_mExpandedPointAndOriginCheckPointIndexMap.insert(std::map<float,int>::value_type(CloseCheckPointAfterDelay,OriginPointIndex));
		m_mExpandedPointAndOriginCheckPointIndexMap.insert(std::map<float,int>::value_type(OpenCheckPointAfterDelay,OriginPointIndex));*/

	}
	if (bReverse)
	{
		std::map<float,int>::reverse_iterator rItAllDelayPointMap=AllDelayPointMap.rbegin();
		std::map<float,int>::reverse_iterator rItAllDelayPointMapEnd=AllDelayPointMap.rend();
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),rItAllDelayPointMap,rItAllDelayPointMapEnd);//留存最后的列冲突调整用

	}
	else
	{

		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),AllDelayPointMap.begin(),AllDelayPointMap.end());//留存最后的列冲突调整用
	}
	//CheckPointPairVecType RepeatedOpenCheckPoints=CheckGetRepeatedDelay2(OpenDelayPoints,GunNum);
	//CheckPointPairVecType RepeatedCloseCheckPoints=CheckGetRepeatedDelay2(CloseDelayPoints,GunNum);
	//m_vStackOfAllRepeatedOpenCheckPoints.insert(m_vStackOfAllRepeatedOpenCheckPoints.end(),RepeatedOpenCheckPoints.begin(),RepeatedOpenCheckPoints.end());//留存最后的列冲突调整用
	//m_vStackOfAllRepeatedCloseCheckPoints.insert(m_vStackOfAllRepeatedCloseCheckPoints.end(),RepeatedCloseCheckPoints.begin(),RepeatedCloseCheckPoints.end());//留存最后的列冲突调整用
	
}
void DownValue::SortAllExpandedCheckPoint(bool bReverse,int XCoord)
{
	
	std::map<float,int>::iterator itPoseAndInd;
	int i=0;
	
	//VectorTypePoint::iterator itPrePoint=m_CheckPointListWithDelay.begin();//

	std::vector<std::pair<float,int>>::iterator it_CurCheckPoint=m_vStackOfAllDelayPointMapInRow.begin();
	/*CheckPointPairVecType::iterator itRepeatedCheckPoints=m_vStackOfAllRepeatedOpenCheckPoints.begin();
	CheckPointPairVecType::iterator itRepeatedCloseCheckPoints=m_vStackOfAllRepeatedCloseCheckPoints.begin();*/
	std::map<float,int>::iterator itCompPrePoseAndInd;
	for (it_CurCheckPoint;it_CurCheckPoint!=m_vStackOfAllDelayPointMapInRow.end();it_CurCheckPoint++)//排序
	{

		bool bFoundSamePoint=false;
		for (itCompPrePoseAndInd=PoseAndIndex.begin();itCompPrePoseAndInd!=PoseAndIndex.end();itCompPrePoseAndInd++)//在此前所有排过序的点中找是否有重合点
		{
			if (fabs(it_CurCheckPoint->first-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//新的待插入的与老的PoseAndIndex中的重合点
			{
				//PoseAndIndex[itAllPoint->YPoint]=i;//新的覆盖老的
				itCompPrePoseAndInd->second=i;//新的覆盖老的
				bFoundSamePoint=true;
			}
		}
		if (!bFoundSamePoint)
		{
			PoseAndIndex.insert(std::map<float,int>::value_type(it_CurCheckPoint->first,i));
		}
		//////////////////////////////////////////////////////////////////////////		
		//itPrePoint++;
		i++;
	}//先排序得出索引
	VectorTypePoint SortedPointsTmp;
	//VectorType SortedPointsColorTmp;
	if (bReverse)//逆向运动
	{
		CornerPoint Tmp;
		std::map<float,int>::reverse_iterator itSortedIndex=PoseAndIndex.rbegin();//逆序迭代器
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.rend();itSortedIndex++)//按索引形成新的序列
		{
			Tmp(XCoord,itSortedIndex->first);
			SortedPointsTmp.push_back(Tmp);
		
		}
	}
	else
	{
		std::map<float,int>::iterator itSortedIndex=PoseAndIndex.begin();
		CornerPoint Tmp;
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.end();itSortedIndex++)//按索引形成新的序列
		{
			Tmp(XCoord,itSortedIndex->first);
			SortedPointsTmp.push_back(Tmp);
		}
	}	

	m_CheckPointListWithDelay=SortedPointsTmp;
	//m_CheckPointColorListWithDelay=SortedPointsColorTmp;
}
void DownValue::CalCulateOneExpandedCheckPointStatues(const VectorType::iterator itCurOriginColor,std::map< float,int>::iterator itCurDelayedCheckPoint,int& PreColor,bool bReverse=false)
{
	std::map< float,int>::iterator itSortedOpenCheckSets=m_StackOfAllOpenDelayMap.begin();//map它会自动按key值也就是坐标值从小到大排序,还会删除key值重复项。。。
	std::map< float,int>::iterator itSortedCloseCheckSets=m_StackOfAllCloseDelayMap.begin();//map它会自动按key值也就是坐标值从小到大排序
	//std::map< float,int>::iterator itCurDelayedCheckPoint=AllDelayPointMap.begin();//=AllDelayPointMap.begin()

	CheckOneExpandedCheckPoint(itCurOriginColor,itSortedOpenCheckSets,itSortedCloseCheckSets,itCurDelayedCheckPoint,
		m_StackOfAllOpenDelayMap,m_StackOfAllCloseDelayMap,m_vStackOfAllRepeatedOpenCheckPoints
		,m_vStackOfAllRepeatedCloseCheckPoints,	PreColor,bReverse);
}
bool DownValue::RecheckEveryOriginCheckPointToSetExpandCheckPointForOneGun(int GunIndex,VectorTypePoint& CheckPoint,VectorType& CheckPointColorList,double GunDelay[][2],bool bReverse=false)
{
	VectorTypePoint::iterator itCheckPoint=CheckPoint.begin();
	VectorType::iterator itCurColor=CheckPointColorList.begin();
	bool bGunOpened=false;
	//bool bColorStatusChanged=false;
	std::vector<float> vExpandedCheckPointForThisGun;
	VectorType vColorsForEveryExpandedCheckPoint;
	int AddStep=bReverse?-1:1;
	//CornerPoint CheckPointTmp=(*itCheckPoint);
	double OpenCheckPointAfterDelay=0;
	double CloseCheckPointAfterDelay=0;
	for (itCurColor;itCurColor!=CheckPointColorList.end();itCurColor++)
	{
		int GunStatuTmp=((*itCurColor)&GunNumValue[(GunIndex)]);//这把枪该开还是关，看该位是否为1
		if (!bGunOpened&&GunStatuTmp>0)//没开，却要开了
		{
			OpenCheckPointAfterDelay=AddStep*GunDelay[GunIndex][0]+(double)itCheckPoint->YPoint;
			LimitCheckPoint(OpenCheckPointAfterDelay,CloseCheckPointAfterDelay,m_fMaxRange);
			//CheckPointTmp.YPoint=OpenCheckPointAfterDelay;
			vExpandedCheckPointForThisGun.push_back(OpenCheckPointAfterDelay);
			vColorsForEveryExpandedCheckPoint.push_back(GunStatuTmp);
			bGunOpened=true;
		}
		else if(bGunOpened&&GunStatuTmp==0)//开了，却要关了
		{
			CloseCheckPointAfterDelay=AddStep*GunDelay[GunIndex][1]+(double)itCheckPoint->YPoint;
			//CheckPointTmp.YPoint=CloseCheckPointAfterDelay;
			LimitCheckPoint(OpenCheckPointAfterDelay,CloseCheckPointAfterDelay,m_fMaxRange);
			vExpandedCheckPointForThisGun.push_back(CloseCheckPointAfterDelay);
			vColorsForEveryExpandedCheckPoint.push_back(GunStatuTmp);
			bGunOpened=false;
		}
		itCheckPoint++;
	}
	m_vStackOfAllDelayPointIndexByGun.push_back(vExpandedCheckPointForThisGun);
	m_vStackOfAllColorsIndexByGun.push_back(vColorsForEveryExpandedCheckPoint);
	return m_vStackOfAllColorsIndexByGun.size()>0&&m_vStackOfAllDelayPointIndexByGun.size()>0;
}
void DownValue::MergeAllCheckPointAndColor(int GunNum,bool bReverse=false)
{
	//
	std::map<float,int> PoseAndColor;//位置、颜色
	std::map<float,int>::iterator itCompPrePoseAndInd;
	bool bFoundSamePoint=false;
	for(int i=0;i<GunNum;i++)
	{
		VectorType::iterator itCurColor=m_vStackOfAllColorsIndexByGun[i].begin();
		std::vector<float>::iterator itCheckPoint=m_vStackOfAllDelayPointIndexByGun[i].begin();
		for (itCheckPoint;itCheckPoint!=m_vStackOfAllDelayPointIndexByGun[i].end();itCheckPoint++)//加入第i把枪的所有点与颜色
		{
			for (itCompPrePoseAndInd=PoseAndColor.begin();itCompPrePoseAndInd!=PoseAndColor.end();itCompPrePoseAndInd++)//在此前所有排过序的点中找是否有重合点
			{
				if (fabs(*itCheckPoint-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//重合点
				{
					itCompPrePoseAndInd->second|=*itCurColor;
				}
				if (*itCheckPoint>itCompPrePoseAndInd->first+TOLERANCE_OF_SAME_POINT)//新的待插入的位置在哪
				{
					break;
				//	bFoundSamePoint=true;
				}
			}
			for (itCompPrePoseAndInd;itCompPrePoseAndInd!=PoseAndColor.end();itCompPrePoseAndInd++)//改写它所在位置之后的所有点的状态
			{
				itCompPrePoseAndInd->second|=*itCurColor;
			}
			PoseAndColor.insert(std::map<float,int>::value_type(*itCheckPoint,*itCurColor));//添加新的
			itCurColor++;
		}
		
	}

}
int DownValue::FindPoseInSortedVec(float Pose,VectorTypePoint::iterator& BeginIter,VectorTypePoint::iterator EndIter)
{
	//VectorTypePoint::iterator itSortedVec=m_CheckPointListWithDelay.begin();
	for (BeginIter;BeginIter!=EndIter;BeginIter++)
	{
		if (fabs(BeginIter->YPoint-Pose)<TOLERANCE_OF_SAME_POINT)
		{
			return BeginIter-m_CheckPointListWithDelay.begin();
		}
	}
	return BeginIter-m_CheckPointListWithDelay.begin();
}

void DownValue::MergeAllSortedCheckPointAndColor(int GunNum)
{
	for(int i=0;i<GunNum;i++)
	{
		VectorType::iterator itCurColor=m_vStackOfAllColorsIndexByGun[i].begin();
		std::vector<float>::iterator itCheckPoint=m_vStackOfAllDelayPointIndexByGun[i].begin();
		VectorTypePoint::iterator itStartCheckPointInSortedVec=m_CheckPointListWithDelay.begin();
		//VectorTypePoint::iterator itEndheckPointInSortedVec=m_CheckPointListWithDelay.begin();
		int StartIndex=0;
		int EndIndex=0;
		for (itCheckPoint;itCheckPoint!=m_vStackOfAllDelayPointIndexByGun[i].end();itCheckPoint)//加入第i把枪的所有点与颜色
		{
			StartIndex=FindPoseInSortedVec(*itCheckPoint,itStartCheckPointInSortedVec,m_CheckPointListWithDelay.end());//找到状态开始点的位置
			itCheckPoint++;
			if (itCheckPoint==m_vStackOfAllDelayPointIndexByGun[i].end())
			{
				AddColor( StartIndex ,*itCurColor);
				break;
			}
			EndIndex=FindPoseInSortedVec(*itCheckPoint,itStartCheckPointInSortedVec,m_CheckPointListWithDelay.end());//找到状态结束点的位置
			for (StartIndex;StartIndex<EndIndex;StartIndex++)//把状态写入
			{
				AddColor( StartIndex ,*itCurColor);
			}
			itCurColor++;			
		}

	}
	//m_CheckPointListWithDelay
}
bool DownValue::ShrinkCheckPointVec(unsigned NumOfOriginCheckPoint)
{
	if (m_CheckPointColorListWithDelay.size()==NumOfOriginCheckPoint)//没延迟，不用缩
	{return false;
	}
	VectorType::iterator it_vCheckPointColor=m_CheckPointColorListWithDelay.begin();
	VectorTypePoint::iterator it_vCheckPoint=m_CheckPointListWithDelay.begin();
	VectorType::iterator it_vCheckPointColorPre=it_vCheckPointColor;
	it_vCheckPointColor++;
	it_vCheckPoint++;
	for (it_vCheckPointColor;it_vCheckPointColor!=m_CheckPointColorListWithDelay.end();it_vCheckPointColor)
	{
		if (*it_vCheckPointColor==*it_vCheckPointColorPre)//重复状态
		{
			 it_vCheckPointColor = m_CheckPointColorListWithDelay.erase(it_vCheckPointColor);
			 it_vCheckPoint=m_CheckPointListWithDelay.erase(it_vCheckPoint);
		}
		else
		{
			it_vCheckPointColorPre=it_vCheckPointColor;
			it_vCheckPoint++;
			it_vCheckPointColor++;
		}
		
	}
	return true;
}
bool DownValue::CalculateAllDelayedCheckPointInRow(VectorTypePoint CheckPoint,VectorType CheckPointColorList,double GunDelay[][2],unsigned GunNum=4,bool bReverse=false)
{
	if (CheckPoint.size()!=CheckPointColorList.size())
	{return false;
	}

	//VectorTypePoint::iterator itCheckPoint=CheckPoint.begin();
	//VectorType::iterator itCurColor=CheckPointColorList.begin();
	m_itChecPointVecEnd=CheckPoint.end();
	ClearAllCheckPointsBackUpVecInRow();
	VectorTypePoint::iterator itCurCheckPoint=CheckPoint.begin();
	ExpandAllOriginCheckPointInRow(CheckPoint, GunDelay,GunNum, bReverse);
	SortAllExpandedCheckPoint(bReverse,itCurCheckPoint->XPoint);
	for (int i=0;i<GunNum;i++)
	{
		RecheckEveryOriginCheckPointToSetExpandCheckPointForOneGun(i,CheckPoint,CheckPointColorList, GunDelay,bReverse);
	}
	m_CheckPointColorListWithDelay.resize(m_CheckPointListWithDelay.size(),0);
	MergeAllSortedCheckPointAndColor(GunNum);
	ShrinkCheckPointVec( CheckPoint.size());
	//std::vector<GunPerBeginAndEnd>::iterator itRowBeginAndEndIndex=m_GunPerBeginAndEnd.begin();
	//int FirstAndLastGunDistance=(GunNum-1)*m_fGunDis;               //第一把喷枪到最后一把喷枪之间的间隔
	//float columnHeight=(*itRowBeginAndEndIndex).End-(*itRowBeginAndEndIndex).Begin-FirstAndLastGunDistance;
	m_ParmAdjust.TransitTheRows(m_CheckPointListWithDelay,m_CheckPointColorListWithDelay,DEFAULT_TRANSIT_VALUE,MAXPAINTRANGEY,!bReverse);//下移
	m_ParmAdjust.AddRowStartPointForOneRowMotion(m_CheckPointListWithDelay,m_CheckPointColorListWithDelay);
	//VectorType::iterator itCurOriginColor;
	////std::map<float,int>::iterator itCurDelayedCheckPoint=AllDelayPointMap.begin();
	//int PreColor=0;

	//for (std::map<float,int>::iterator itCurDelayedCheckPoint=AllDelayPointMap.begin();itCurDelayedCheckPoint!=AllDelayPointMap.end();itCurDelayedCheckPoint++)
	//{
	//	itCurOriginColor=CheckPointColorList.begin()+m_mExpandedPointAndOriginCheckPointIndexMap[itCurDelayedCheckPoint->first];		
	//	CalCulateOneExpandedCheckPointStatues(itCurOriginColor,itCurDelayedCheckPoint,PreColor,bReverse);
	//	ColorOfExpandedCheckPointsTmp.push_back(PreColor);//存储考虑延迟后的突变点处的颜色，与点一一对应	
	//
	//}
	//m_CheckPointColorListWithDelay=ColorOfExpandedCheckPointsTmp;
	//m_CheckPointListWithDelay=OneCheckPointExpandTmp;


	return true;

}


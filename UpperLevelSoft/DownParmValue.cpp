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
	InitGunNumValue(6);//��ʼ����ǹ�� 
}
DownValue::~DownValue()
{
	GunMenber=1;
}
void DownValue::GetGunModelAndGunDistance(int GunNember,float BetweenGunDistance)
{
	m_GunModelAndDistance.GunNember=GunNember;//��ǹ��
	m_GunModelAndDistance.BetweenGunDistance=BetweenGunDistance;//��ǹ���
}
void DownValue::GetDownValueMotion(MotionParm_It_TYPE MotionBegin,MotionParm_It_TYPE MotionEnd)            //���ɱ��
{
	m_GunPerBeginAndEnd.clear();
	PerLaw=(int)*MotionBegin;              //����ǰ�������м���
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
		bool IsClockWise=true;                                 //˳ʱ��
		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //����ÿһ�е���ʼλ�ú���ֹλ�ã��������к�
		{	

			GunPerBeginAndEnd m_GunPerBAE;
			if (RemberNumber==0)          //�õ�ÿһ�е��к�
			{
				m_GunPerBAE.Index=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==1)        //�õ�ÿһ�е��������
			{
				m_GunPerBAE.Begin=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==2)     //�õ�ÿһ�е��յ�����
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
		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //����ÿһ�е���ʼλ�ú���ֹλ�ã��������к�
		{	
			GunPerBeginAndEnd m_GunPerBAE;
			if (RemberNumber==0)          //�õ�ÿһ�е��к�
			{
				m_GunPerBAE.Index=*MotionBegin;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==1)        //�õ�ÿһ�е��������
			{
				int Begin=*MotionBegin;
				int BeginIndex=Begin/PerHeight;
				int m_Shang=Begin%PerHeight;
				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
				m_GunPerBAE.Begin=(BeginIndex+Result)*PerHeight;
				RemberNumber=RemberNumber+1;
			}
			else if (RemberNumber==2)     //�õ�ÿһ�е��յ�����
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
	m_XY.XPoint=CurColumIndex*PerHeight;//�ٶ��п����һ����PerHeight��
	std::vector<float>::iterator itChangePointHead=m_ChangedColorPointHead;
	for (int i=0;i<CellToBeRun;i++ )//ÿ��ɫ�飬����Ϊǹ������������ɫ��
	{
		if (bisForward)//����
		{
			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//ÿ��ɫ���ڵ�ͻ���
			{
				float Y_coord=MoveRangeBegin+(i+(*itChangePointHead))*PerHeight;
				if (Y_coord<=MoveRangeEnd+1.1)//���г������ڣ�1.1��Ϊ�˷�ֹ��ʱ����1��΢С���
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
			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//ÿ��ɫ���ڵ�ͻ���
			{
				float Y_coord=MoveRangeBegin-(i+(*itChangePointHead))*PerHeight;
				if (Y_coord>=MoveRangeEnd-1.1)//���г������� 1.1��Ϊ�˷�ֹ���ǻ���1��΢С���
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
//void DownValue::GetDownValueMotion2(MotionParm_It_TYPE MotionBegin,MotionParm_It_TYPE MotionEnd)            //���ɱ��
//{
//	m_GunPerBeginAndEnd.clear();
//	PerLaw=(int)*MotionBegin;              //����ǰ�������м���
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
//		bool IsClockWise=true;                                 //˳ʱ��
//		
//		for (MotionBegin;MotionBegin!=MotionEnd;MotionBegin++)  //����ÿһ�е���ʼλ�ú���ֹλ�ã��������к�
//		{	
//
//			GunPerBeginAndEnd m_GunPerBAE;
//			if (RemberNumber==0)          //�õ�ÿһ�е��к�
//			{
//				m_GunPerBAE.Index=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==1)        //�õ�ÿһ�е��������
//			{
//				m_GunPerBAE.Begin=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==2)     //�õ�ÿһ�е��յ�����
//			{
//				m_GunPerBAE.End=*MotionBegin;
//				m_GunPerBeginAndEnd.push_back(m_GunPerBAE);
//				RemberNumber=0;
//				IsClockWise=!IsClockWise;
//			}			
//		}
//	}
//	else//�ݲ����޸�
//	{
//		for (MotionBegin;MotionBegin<MotionEnd;MotionBegin++)  //����ÿһ�е���ʼλ�ú���ֹλ�ã��������к�
//		{	
//			GunPerBeginAndEnd m_GunPerBAE;
//			if (RemberNumber==0)          //�õ�ÿһ�е��к�
//			{
//				m_GunPerBAE.Index=*MotionBegin;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==1)        //�õ�ÿһ�е��������
//			{
//				int Begin=*MotionBegin;
//				int BeginIndex=Begin/PerHeight;
//				int m_Shang=Begin%PerHeight;
//				int Result=Floor4_Ceil5Float(m_Shang,PerHeight);
//				m_GunPerBAE.Begin=(BeginIndex+Result)*PerHeight;
//				RemberNumber=RemberNumber+1;
//			}
//			else if (RemberNumber==2)     //�õ�ÿһ�е��յ�����
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
//	m_XY.XPoint=CurColumIndex*PerHeight;//�ٶ��п����һ����PerHeight��
//	std::vector<float>::iterator itChangePointHead=m_ChangedColorPointHead;
//	for (int i=0;i<CellToBeRun;i++ )//ÿ��ɫ�飬����Ϊǹ������������ɫ��
//	{
//		if (bisForward)//����
//		{
//			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//ÿ��ɫ���ڵ�ͻ���
//			{
//				float Y_coord=MoveRangeBegin+(i+(*itChangePointHead))*PerHeight;
//				if (Y_coord<=MoveRangeEnd+1.1)//���г������ڣ�1.1��Ϊ�˷�ֹ��ʱ����1��΢С���
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
//			for (itChangePointHead=m_ChangedColorPointHead;itChangePointHead!=m_ChangedColorPointEnd;itChangePointHead++)//ÿ��ɫ���ڵ�ͻ���
//			{
//				float Y_coord=MoveRangeBegin-(i+(*itChangePointHead))*PerHeight;
//				if (Y_coord>=MoveRangeEnd-1.1)//���г������� 1.1��Ϊ�˷�ֹ���ǻ���1��΢С���
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
void DownValue::TranslateEveryChangePoint2(unsigned m_uGunNum,float m_fGunDis)               //�õ�����ͻ��������
{
	int FirstAndLastGunDistance=(m_uGunNum-1)*m_fGunDis;               //��һ����ǹ�����һ����ǹ֮��ļ��
	std::vector<GunPerBeginAndEnd>::iterator m_GunPerBeginAndEndIndex;
	m_GunPerBeginAndEndIndex=m_GunPerBeginAndEnd.begin();
	m_VectorTypePoint.clear();
	float columnHeight;
	columnHeight=(*m_GunPerBeginAndEndIndex).End-(*m_GunPerBeginAndEndIndex).Begin-FirstAndLastGunDistance;     //��Ϳ����ĸ�
	int CellToBeRun;//Ҫ�ܵķ�����	
	int m_Column=m_GunPerBeginAndEnd.size();             //��ǰ��Ϳ��Ԫ�ж�����
	bool JudgeOriention=true;                            //�ж�����ʱ�뻹��˳ʱ��
	std::vector<float> m_ChangedPoint;                     //�õ�ͻ���
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
void DownValue::TranslateEveryChangePoint(unsigned m_uGunNum,float m_fGunDis)               //�õ�����ͻ��������
{

	int FirstAndLastGunDistance=(m_uGunNum-1)*m_fGunDis;               //��һ����ǹ�����һ����ǹ֮��ļ��
	std::vector<GunPerBeginAndEnd>::iterator m_GunPerBeginAndEndIndex;
	m_GunPerBeginAndEndIndex=m_GunPerBeginAndEnd.begin();

	int m_Column=m_GunPerBeginAndEnd.size();             //��ǰ��Ϳ��Ԫ�ж�����
	bool JudgeOriention=true;                            //�ж�����ʱ�뻹��˳ʱ��
	std::vector<float> m_ChangedPoint;                     //�õ�ͻ���
	std::vector<float>::iterator m_ChangedPointIndex;
	std::vector<float>::iterator m_ChangedPointIndexCopy;
	m_ChangedPoint.clear();
	for (m_ChangedColorPointHeadCopy;m_ChangedColorPointHeadCopy<m_ChangedColorPointEnd;m_ChangedColorPointHeadCopy++)           //��ͻ���
	{
		m_ChangedPoint.push_back((*m_ChangedColorPointHeadCopy));
	}
	float GetSecondGunPosition=(float)(PerHeight-m_fGunDis)/PerHeight;            //�õ��ڶ�����ǹ��λ��
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
			columnHeight=(*m_GunPerBeginAndEndIndex).End-(*m_GunPerBeginAndEndIndex).Begin-FirstAndLastGunDistance;     //��Ϳ����ĸ�
			EveryColumnPerNumber=(float)columnHeight/PerHeight+0.5;       //�õ�ÿһ�еĵ�Ԫ����
		}
		else
		{
			columnHeight=(*m_GunPerBeginAndEndIndex).Begin-(*m_GunPerBeginAndEndIndex).End-FirstAndLastGunDistance;     //��Ϳ����ĸ�
			EveryColumnPerNumber=(float)columnHeight/PerHeight+0.5;       //�õ�ÿһ�еĵ�Ԫ����
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
				m_XYl.YPoint=(*m_GunPerBeginAndEndIndex).End-FirstAndLastGunDistance+(GunValue-1)*m_fGunDis;//��ǹ������ɫ��ĩ�����
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

	//�õ�ÿһ�е���Ϳ�߶�
}
void DownValue::TranslateEveryChangeColor(int BeginPoint,int EndPoint)              //������ɫ,û�õ�
{

	GunPerBeginAndEnd m_EveryChangedColorPoint;
	int m_PerHeight=PerHeight;        //ÿ��С��Ԫ�ĸ߶�
	if (BeginPoint>EndPoint)          //���ǵ���ǹ������һ��
	{
		m_PerHeight=-m_PerHeight;
	}
	int EveryColumnUnitMenber=(EndPoint-BeginPoint)/m_PerHeight; //�õ�ÿһ���ж��ٸ���Ϳ��Ԫ
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
CString DownValue::GetStringPaser_star(CString gps_str,int num)    //��ȡ���ŵ��Ǻż���ַ���
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
void DownValue::GetDownValuePlc(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis)       //�洢��ǹ״̬
{
	std::vector<int>::iterator m_RemberStoragePlc;
	AllChangeColorPosition.clear();//

	m_RemberStoragePlc=StoragePlc.begin();
	int RemberNumber=(StoragePlc.size()-1)/PerLaw;
	m_RemberStoragePlc++;  //m_RemberStoragePlc�׸�������ǵ�Ԫ�߶�
	GunColorAll.clear();         //ÿ�ζ������
	m_EveryColumnGunColor.clear();
	int Index=0;
	for (m_RemberStoragePlc;m_RemberStoragePlc!=StoragePlc.end();m_RemberStoragePlc++)
	{
		int ColumnUintNumber=*m_RemberStoragePlc;    //��ǰ������ǹ״̬�������ָ��õģ�
		for (int Number=0;Number<ColumnUintNumber-1;Number++)//ColumnUintNumber��ʵ��ͻ�������1����Ϊ����ĩβ��Ϊ����һ��0���зָ�
		{
			m_RemberStoragePlc++;
			m_EveryColumnGunColor.push_back(*m_RemberStoragePlc);                                    //ÿһ�е�plc��ֵ
		}
		GunColorAll.insert(std::map<int,VectorType>::value_type(Index,m_EveryColumnGunColor));       //�õ�һ����λ����
		m_EveryColumnGunColor.clear();
		m_RemberStoragePlc++;//�ƹ���ĩβ�ķָ
		Index++;
	}
	m_ChangedColorPointHead=ChangedColorPointHead;
	m_ChangedColorPointHeadCopy=ChangedColorPointHead;
	m_ChangedColorPointHeadCopyl=ChangedColorPointHead;
	m_ChangedColorPointEnd=ChangedColorPointEnd;
	TranslateEveryChangePoint2(m_uGunNum,m_fGunDis);
	
}

//void DownValue::GetDownValuePlc2(std::vector<int> StoragePlc,std::vector<float>::iterator ChangedColorPointHead,std::vector<float>::iterator ChangedColorPointEnd,unsigned m_uGunNum,float m_fGunDis)       //�洢��ǹ״̬
//{
//	std::vector<int>::iterator m_RemberStoragePlc;
//	m_RemberStoragePlc=StoragePlc.begin();
//	int RemberNumber=(StoragePlc.size()-1)/PerLaw;
//	m_RemberStoragePlc++;  //m_RemberStoragePlc�׸�������ǵ�Ԫ�߶�
//	GunColorAll.clear();         //ÿ�ζ������
//	m_EveryColumnGunColor.clear();
//	int Index=0;
//	//////////////////////////////��ȡ��ɫ״̬////////////////////////////////////////////
//	for (m_RemberStoragePlc;m_RemberStoragePlc!=StoragePlc.end();m_RemberStoragePlc++)
//	{
//		int ColumnUintNumber=*m_RemberStoragePlc;    //��ǰ������ǹ״̬��
//		for (int Number=0;Number<ColumnUintNumber;Number++)
//		{
//			m_RemberStoragePlc++;
//			m_EveryColumnGunColor.push_back(*m_RemberStoragePlc);                                    //ÿһ�е�plc��ֵ
//		}
//		GunColorAll.insert(std::map<int,VectorType>::value_type(Index,m_EveryColumnGunColor));       //�õ�һ����λ����
//		m_EveryColumnGunColor.clear();
//		Index++;
//	}
//	m_ChangedColorPointHead=ChangedColorPointHead;
//	m_ChangedColorPointHeadCopy=ChangedColorPointHead;
//	m_ChangedColorPointHeadCopyl=ChangedColorPointHead;
//	m_ChangedColorPointEnd=ChangedColorPointEnd;
//	TranslateEveryChangePoint(m_uGunNum,m_fGunDis);//��ȡ�����
//	
//}

CString DownValue::CompileGunCondition(int Parm)  //ת��ͼ����ɫ��m
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
void DownValue::StartOpen(CString Directory)           //��ǰ����Ҫ
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
	fprintf(fp,"F(%d) \n",v);      //��λm/s
	fprintf(fp,"TA500 \n");        //���ٶ�
	fprintf(fp,"TS0 \n");           //S���߼��ٶ�
	int c=1;
	int k=0;
	char buf[1024];
	memset(buf,0,sizeof(buf));
	int x,y;
	for (int j=0;j<PerLaw;j++)               //n������
	{
		int x= PerWidth*j;
		if (JudgeReturn==false)  //y��Ϊ���
		{
			if (JudgeFirstLow==false)           //����һ���ж�
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
		else                       //Y��Ϊ��С
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
	CheckPointPairVecType DelayMap;//����ֱ����map��

	for (int i=0;i< Delay.size();i++)//�����±������ݵ�ӳ��
	{
		DelayMap.push_back(std::make_pair(i,Delay[i]));
	}
	sort(DelayMap.begin(),DelayMap.end(),cmp_by_value);//��ֵ��С��������
	bool bRecentlySame=false;
	CheckPointPairVecType repeatedCheckPoints;//�����ظ���ͻ��㣬��һ����ǹ�ţ���һ���������ظ�����
	for (int i=0;i<DelayMap.size()-1;i++)
	{
		
			if ((DelayMap[i].second)==(DelayMap[i+1].second))
			{
				bRecentlySame=true;
				repeatedCheckPoints.push_back(DelayMap[i]);//�ظ����ó���
			}
			else if (bRecentlySame){
				repeatedCheckPoints.push_back(DelayMap[i]);//�ظ����ó���
				bRecentlySame=false;
			}
			else{
				bRecentlySame=false;
			}	
	}
	if (bRecentlySame)
	{
		repeatedCheckPoints.push_back(DelayMap[DelayMap.size()-1]);//�ظ����ó���
	}
	return repeatedCheckPoints;
}

CheckPointPairVecType DownValue::CheckGetRepeatedDelay2(std::vector<float> Delay,int GunNum)
{
	CheckPointPairVecType DelayMap;//����ֱ����map��
	int indGun=0;
	int Step=0;
	for (int i=0;i< Delay.size();i++)//�����±������ݵ�ӳ��
	{
		Step=std::floor((double)i/GunNum);
		DelayMap.push_back(std::make_pair(i-Step*GunNum,Delay[i]));
	}
	sort(DelayMap.begin(),DelayMap.end(),cmp_by_value);//��ֵ��С��������
	bool bRecentlySame=false;
	CheckPointPairVecType repeatedCheckPoints;//�����ظ���ͻ��㣬��һ����ǹ�ţ���һ���������ظ�����
	for (int i=0;i<DelayMap.size()-1;i++)
	{

		if ((DelayMap[i].second)==(DelayMap[i+1].second))
		{
			bRecentlySame=true;
			repeatedCheckPoints.push_back(DelayMap[i]);//�ظ����ó���
		}
		else if (bRecentlySame){
			repeatedCheckPoints.push_back(DelayMap[i]);//�ظ����ó���
			bRecentlySame=false;
		}
		else{
			bRecentlySame=false;
		}	
	}
	if (bRecentlySame)
	{
		repeatedCheckPoints.push_back(DelayMap[DelayMap.size()-1]);//�ظ����ó���
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
#define setbit(x,y) x|=(1<<y) //��X�ĵ�Yλ��1
#define clrbit(x,y) x&=~(1<<y) //��X�ĵ�Yλ��0
bool DownValue::CheckAndSetRemainColor(VectorTypePoint::iterator itCurOriginCheckPoint,VectorTypePoint::iterator itOriginCheckPointVecEnd,//CornerPoint& GunCheckPointTmp, 
	VectorType::iterator itCurOriginCheckPointColor,int ToBeCheckedGunIndex,
	float CurExpandedCheckPointPose, double GunDelay[][2],
	int& ColorStatus,//����ǹλ��Ӧ��ǰһ״̬��Ҳ����Ӧ�ñ��ֵ�״̬��
	bool bReverse)
{
	VectorTypePoint::iterator itExpandedCheckPointPos;
	int CurColorBitStatus;//����ǹλ��Ӧ��״̬��Ҳ����Ӧ�ñ��ֵ�״̬��
	itCurOriginCheckPoint++;//ָ����һ��ԭʼͻ���
	if (itCurOriginCheckPoint==itOriginCheckPointVecEnd)//���һ��
	{
		return false;
	}
	itCurOriginCheckPointColor++;//��һ��ԭʼͻ������ɫ
	bool NowBeforeTheOpenPoint=false;
	bool NowBeforeClosePoint=false;
	int AddStep=bReverse?-1:1;	
	int MovedStep=1;//����ͻ����ڵ�ǰԭʼͻ����ĵڼ���ͻ���ǰ
	for (itCurOriginCheckPoint;itCurOriginCheckPoint!=itOriginCheckPointVecEnd;itCurOriginCheckPoint++)
	{
		float OpenPointTobeComp=itCurOriginCheckPoint->YPoint+AddStep*GunDelay[ToBeCheckedGunIndex][0];
		float ClosePointTobeComp=itCurOriginCheckPoint->YPoint+AddStep*GunDelay[ToBeCheckedGunIndex][1];
		if (bReverse)
		{
			if ( CurExpandedCheckPointPose>=OpenPointTobeComp)//�����������ԭʼͻ�������֮�󣬵�ǰͻ��������λӦ�ñ������ԭʼͻ����ǰһ����Щλ��״̬
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
			if ( CurExpandedCheckPointPose<=OpenPointTobeComp)//�����������ԭʼͻ�������֮�󣬵�ǰͻ��������λӦ�ñ������ԭʼͻ����ǰһ����Щλ��״̬
			{
				//itExpandedCheckPointPos=itCurOriginCheckPoint--;
				NowBeforeTheOpenPoint=true;
			}
			if (CurExpandedCheckPointPose<=ClosePointTobeComp)
			{
				NowBeforeClosePoint=true;
			}
		}
		
		if ( NowBeforeClosePoint|| NowBeforeTheOpenPoint)//������ͻ�������,Ҫô�ڿ�����ʱǰ��Ҫô�ڹر���ʱǰ
		{
			//itExpandedCheckPointPos=itCurOriginCheckPoint--;
			itCurOriginCheckPoint--;
			CurColorBitStatus=((*itCurOriginCheckPointColor)&GunNumValue[(ToBeCheckedGunIndex)]);//��Ӧ��ǹλ��״̬ȡ����
			itCurOriginCheckPointColor--;
			int PreColorBitStatus;
			if (MovedStep>1)//��ԭʼͻ���״̬��Ϊǰһ״̬
			{
				PreColorBitStatus=((*itCurOriginCheckPointColor)&GunNumValue[(ToBeCheckedGunIndex)]);//��Ӧ��ǹλ��״̬ȡ����
			}
			else//ǰһͻ�������ǵ�ǰ�Լ���ͻ���Ļ���״̬���ڸ�д��������ԭʼͻ���״̬��Ϊǰһ״̬
				PreColorBitStatus=((ColorStatus)&GunNumValue[(ToBeCheckedGunIndex)]);//��Ӧ��ǹλ��״̬ȡ����
				//int ColorIndex=itExpandedCheckPointPos-itOriginCheckPointVecHead;
			
			///////////////��PreColorBitStatus���ǻ�ȥColorStatus///////////////////////////////////////////////////////////
			if (PreColorBitStatus)			{
				setbit(ColorStatus,ToBeCheckedGunIndex);
			}
			else
				clrbit(ColorStatus,ToBeCheckedGunIndex);//�ùر���λ
			
			if ((!NowBeforeTheOpenPoint)&&CurColorBitStatus>0)//��������ӳٿ�����֮��CurColorBitStatus�ÿ�
			{
				setbit(ColorStatus,ToBeCheckedGunIndex);

			}
			if ( (!NowBeforeClosePoint)&&CurColorBitStatus==0)//��������ӳٹرյ�֮��CurColorBitStatus�ù�
			{
				clrbit(ColorStatus,ToBeCheckedGunIndex);//�ùر���λ
			}
			break;
		}
		itCurOriginCheckPointColor++;//��һ��ԭʼͻ������ɫ
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
	bool bFunctioned=false;//�Ƿ���������������˸ı�
	int GunStatuTmp=((*itCurColor)&GunNumValue[(GunIndex)]);//���ǹ�ÿ����ǹأ�����λ�Ƿ�Ϊ1
	KeepRemainedBitNumVec.clear();
	for (int i=0;i<4;i++)
	{
		KeepRemainedBitNumVec.push_back(i);
	}
	if (NeedOpen)//��ǰ������
	{
		if (GunStatuTmp>0)//�ÿ�
		{setbit(PreColor,GunIndex);	
		bFunctioned=true;
		}//ʵ���ǵ�ǰ��ǹ��Ҫ��ǰ���Ͱ�PreColor����Ӧλ��1
	}
	else//��ǰ�رյ�
	{
		if (GunStatuTmp==0)//�ù�
		{
			clrbit(PreColor,GunIndex);//ʵ���ǵ�ǰ��ǹ��Ҫ��ǰ�ؾͰ�PreColor����Ӧλ��0
			bFunctioned=true;
		}
	}

	KeepRemainedBitNumVec.erase(remove(KeepRemainedBitNumVec.begin(), KeepRemainedBitNumVec.end(), GunIndex),KeepRemainedBitNumVec.end() );//ɾ�����������ǹ�ţ�ʣ��ά��ԭ״��ǹ��λ
	//GunCheckPointTmp.YPoint=CheckPointPose;
	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//�洢�����ӳٺ��ͻ���λ��
	CheckPointPairVecType::iterator itRepeatedCheckPoints=RepeatedCheckPoints.begin();
	/////////////////////////////////�����ظ�ͻ���/////////////////////////////////////////
	for (itRepeatedCheckPoints;itRepeatedCheckPoints!=RepeatedCheckPoints.end();itRepeatedCheckPoints++)
	{
		if (CheckPointPose==itRepeatedCheckPoints->second)//ȷʵ�ظ���
		{		
			int GunStatuTmp=((*itCurColor)&GunNumValue[(itRepeatedCheckPoints->first)]);//���ǹ�ÿ����ǹأ�����λ�Ƿ�Ϊ1
			if (NeedOpen)
			{
				if (GunStatuTmp>0)//�ÿ�
				{setbit(PreColor,(itRepeatedCheckPoints->first));	
				}//ʵ���ǵ�ǰ��ǹ��Ҫ��ǰ���Ͱ�PreColor����Ӧλ��1,���ܺ�ǰ���ظ�������û��ϵ
			}
			else
			{
				if (GunStatuTmp==0)//�ù�
				{
					clrbit(PreColor,(itRepeatedCheckPoints->first));//ʵ���ǵ�ǰ��ǹ��Ҫ��ǰ�ؾͰ�PreColor����Ӧλ��0
				}

			}
			KeepRemainedBitNumVec.erase(remove(KeepRemainedBitNumVec.begin(), KeepRemainedBitNumVec.end(), itRepeatedCheckPoints->first),KeepRemainedBitNumVec.end() );//ɾ�����������ǹ�ţ�ʣ��ά��ԭ״��ǹ��λ
			//std::vector<int>::iterator HandledIndex=std::find(KeepRemainedBitNumVec.begin(),KeepRemainedBitNumVec.end(),itRepeatedCheckPoints->first);//ɾ�����������ǹ�ţ�ʣ��ά��ԭ״��ǹ��λ
			//if (HandledIndex!=KeepRemainedBitNumVec.end())
			//{
			//	KeepRemainedBitNumVec.erase(std::find(KeepRemainedBitNumVec.begin(),KeepRemainedBitNumVec.end(),HandledIndex));
			//}
		}
	}
	///////////////////////////��û�����λ������///////////////////////////////////////////////

	return true;
}
bool DownValue::CheckOneExpandedCheckPoint(VectorType::iterator itCurColor,std::map< float,int>::iterator itSortedOpenCheckSets,
	std::map< float,int>::iterator itSortedCloseCheckSets,std::map< float,int>::iterator itAllDelayedCheckSets,
	std::map< float,int>& DelayMap,//�ӳ�֮���ͻ���,���¿����ӳٵ�ǹ��,
    std::map< float,int>& CloseDelayMap,//�ӳ�֮���ͻ���,���¹ر��ӳٵ�ǹ��,
	CheckPointPairVecType RepeatedOpenCheckPoints, CheckPointPairVecType RepeatedCloseCheckPoints,
	int& PreColor,bool bReverse=false)
{
	bool bOpenCheck=false;//Ҫ��鿪����ʱ��
	bool bCloseCheck=false;//Ҫ���ر���ʱ��
	bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap.end());
	bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap.end());
	/////////////////////////////////��������ӳٿ��ģ������ӳٹصģ���ִ�����Σ�����ì��/////////////////////////////////////////

	if (bOpenCheck)//���ӳٿ���ͻ���
	{
		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			PreColor,RepeatedOpenCheckPoints,true);
	}
	if (bCloseCheck)//���ӳٹص�ͻ���
	{
		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			PreColor,RepeatedCloseCheckPoints,false);
	}		

	//GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;//�Ƶ�������
	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//�洢�����ӳٺ��ͻ���λ��
	
	return true;
}
//bool DownValue::CheckOneExpandedCheckPoint(VectorType::iterator itCurColor,std::map< float,int>::iterator itSortedOpenCheckSets,
//	std::map< float,int>::iterator itSortedCloseCheckSets,std::vector<std::pair<float,int>>::iterator itAllDelayedCheckSets,
//	std::map< float,int>& DelayMap,//�ӳ�֮���ͻ���,���¿����ӳٵ�ǹ��,
//	std::map< float,int>& CloseDelayMap,//�ӳ�֮���ͻ���,���¹ر��ӳٵ�ǹ��,
//	CheckPointPairVecType RepeatedOpenCheckPoints, CheckPointPairVecType RepeatedCloseCheckPoints,
//	int& PreColor,bool bReverse=false)
//{
//	bool bOpenCheck=false;//Ҫ��鿪����ʱ��
//	bool bCloseCheck=false;//Ҫ���ر���ʱ��
//	bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap.end());
//	bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap.end());
//	/////////////////////////////////��������ӳٿ��ģ������ӳٹصģ���ִ�����Σ�����ì��/////////////////////////////////////////
//
//	if (bOpenCheck)//���ӳٿ���ͻ���
//	{
//		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
//			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
//			PreColor,RepeatedOpenCheckPoints,true);
//	}
//	if (bCloseCheck)//���ӳٹص�ͻ���
//	{
//		SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
//			itAllDelayedCheckSets->first, //GunCheckPointTmp, 
//			PreColor,RepeatedCloseCheckPoints,false);
//	}		
//
//	//GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;//�Ƶ�������
//	//OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//�洢�����ӳٺ��ͻ���λ��
//
//	return true;
//}

//������ʱδ������Ҫ������ٵ�һ�Σ�һ��ͻ����ͬһ����ǹ״̬Ҫô�ɿ�����Ҫô��֮��Ҫôά��ԭ״��ֻ�����֡���������ɼ�������ì��
bool DownValue::CheckOneCheckPoint(const VectorType::iterator itCurColor,const VectorTypePoint::iterator itCheckPoint,int PreColor,double GunDelay[][2],unsigned GunNum=4,bool bReverse=false)
{
	//VectorTypePoint::iterator itCheckPoint=CheckPoint.begin();
	//VectorType::iterator itCurColor=CheckPointColorList.begin();
	int SavedPreColor= PreColor;
	std::map< float,int> DelayMap;//�ӳ�֮���ͻ���,���¿����ӳٵ�ǹ��,
	std::map< float,int> CloseDelayMap;//�ӳ�֮���ͻ���,���¹ر��ӳٵ�ǹ��,<
	std::map<float,int>AllDelayPointMap;//�����ӳ�֮���ͻ���,�����ӳٵ�ǹ��,<
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
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),rItAllDelayPointMap,rItAllDelayPointMapEnd);//���������г�ͻ������
	
	}
	else
	{
		
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),AllDelayPointMap.begin(),AllDelayPointMap.end());//���������г�ͻ������
	}
	////������ǰ��������//////////////////////////////////////////////////////////////////////////
	std::map< float,int>::iterator itSortedOpenCheckSets= DelayMap.begin();//map�����Զ���keyֵҲ��������ֵ��С��������,����ɾ��keyֵ�ظ������
	std::map< float,int>::iterator itSortedCloseCheckSets= CloseDelayMap.begin();//map�����Զ���keyֵҲ��������ֵ��С��������
	std::map< float,int>::iterator itAllDelayedCheckSets;//=AllDelayPointMap.begin()
	//itSortedCheckPoint=DelayMap.begin();
	
	//int GunNumValue[4]={0x01,0x02,0x04,0x08};//����ǹ��Ӧ�ĺ���ֵ,����4����ǹ
	CornerPoint GunCheckPointTmp;

	GunCheckPointTmp=*itCheckPoint;//��ʼ��Ϊԭ���ĵ�������

	OneCheckPointExpandTmp.clear();//�����һͻ����ӳ�����������GunNum��ͻ���
	ColorOfExpandedCheckPointsTmp.clear();
	
	bool bOpenCheck=false;//Ҫ��鿪����ʱ��
	bool bCloseCheck=false;//Ҫ���ر���ʱ��
	CheckPointPairVecType RepeatedOpenCheckPoints=CheckGetRepeatedDelay(OpenDelayPoints);
	CheckPointPairVecType RepeatedCloseCheckPoints=CheckGetRepeatedDelay(CloseDelayPoints);
	m_vStackOfAllRepeatedOpenCheckPoints.insert(m_vStackOfAllRepeatedOpenCheckPoints.end(),RepeatedOpenCheckPoints.begin(),RepeatedOpenCheckPoints.end());//���������г�ͻ������
	m_vStackOfAllRepeatedCloseCheckPoints.insert(m_vStackOfAllRepeatedCloseCheckPoints.end(),RepeatedCloseCheckPoints.begin(),RepeatedCloseCheckPoints.end());//���������г�ͻ������
	if (bReverse)
	{
		itAllDelayedCheckSets=AllDelayPointMap.end();//����
		itAllDelayedCheckSets--;
		for ( int i=0; i<AllDelayPointMap.size();i++)//�ɴ�С��������ӳٺ��ͻ�������ж��Ƿ���Ҫ��ǰ����
		{			
			PreColor=findPreColorAfterPreExpand(itAllDelayedCheckSets->first,PreColor,true,bReverse);
			CheckOneExpandedCheckPoint(itCurColor,itSortedOpenCheckSets,itSortedCloseCheckSets,itAllDelayedCheckSets,
				DelayMap,CloseDelayMap,RepeatedOpenCheckPoints,RepeatedCloseCheckPoints,
				PreColor,bReverse);
			///////////////ǰ��û�����λ�ٴ���һ��///////////////////////////////////////////////////////////
			if (itAllDelayedCheckSets->first<itCheckPoint->YPoint)//��ǰ����Ĳ���Ҫ����
			{
				for (std::vector<int>::iterator itUnHandledBit=KeepRemainedBitNumVec.begin(); itUnHandledBit!=KeepRemainedBitNumVec.end();itUnHandledBit++)
				{
					CheckAndSetRemainColor(itCheckPoint,m_itChecPointVecEnd,//CornerPoint& GunCheckPointTmp, 
						itCurColor,*itUnHandledBit,	itAllDelayedCheckSets->first, GunDelay,PreColor,//����ǹλ��Ӧ��ǰһ״̬��Ҳ����Ӧ�ñ��ֵ�״̬��
						bReverse);
				}
			}
			
			GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;
			OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//�洢�����ӳٺ��ͻ���λ��
			ColorOfExpandedCheckPointsTmp.push_back(PreColor);//�洢�����ӳٺ��ͻ��㴦����ɫ�����һһ��Ӧ
			itAllDelayedCheckSets--;

		}//�����һ�����PreColorӦ�뵱ǰ��ɫһ����
	}
	else
	{
		itAllDelayedCheckSets=AllDelayPointMap.begin();
		for ( itAllDelayedCheckSets; itAllDelayedCheckSets!=AllDelayPointMap.end();itAllDelayedCheckSets++)//��С���󣬶�����ӳٺ��ͻ�������ж��Ƿ���Ҫ��ǰ����
		{

			//bOpenCheck=false;
			//bCloseCheck=false;

			//bOpenCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedOpenCheckSets,DelayMap);
			//bCloseCheck=JudgeExpandedCheckPointType(itAllDelayedCheckSets,itSortedCloseCheckSets,CloseDelayMap);
			///////////////////////////////////��������ӳٿ��ģ������ӳٹصģ���ִ�����Σ�����ì��/////////////////////////////////////////
			//if (bOpenCheck)//���ӳٿ���ͻ���
			//{
			//	SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			//		itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			//		PreColor,RepeatedOpenCheckPoints,true);
			//}
			//if (bCloseCheck)//���ӳٹص�ͻ���
			//{
			//	SetExpandedCheckPointsForEachGun(itCurColor,itAllDelayedCheckSets->second,
			//		itAllDelayedCheckSets->first, //GunCheckPointTmp, 
			//		PreColor,RepeatedCloseCheckPoints,false);
			//}		
			//ColorOfExpandedCheckPointsTmp.push_back(PreColor);//�洢�����ӳٺ��ͻ��㴦����ɫ�����һһ��Ӧ
			PreColor=findPreColorAfterPreExpand(itAllDelayedCheckSets->first,PreColor,true,bReverse);
			CheckOneExpandedCheckPoint(itCurColor,itSortedOpenCheckSets,itSortedCloseCheckSets,itAllDelayedCheckSets,
				DelayMap,CloseDelayMap,RepeatedOpenCheckPoints,RepeatedCloseCheckPoints,
				PreColor,bReverse);
			///////////////ǰ��û�����λ�ٴ���һ��///////////////////////////////////////////////////////////
			if (itAllDelayedCheckSets->first>itCheckPoint->YPoint)//��ǰ����Ĳ���Ҫ����
			{
				for (std::vector<int>::iterator itUnHandledBit=KeepRemainedBitNumVec.begin(); itUnHandledBit!=KeepRemainedBitNumVec.end();itUnHandledBit++)
				{
					CheckAndSetRemainColor(itCheckPoint,m_itChecPointVecEnd,//CornerPoint& GunCheckPointTmp, 
						itCurColor,*itUnHandledBit,	itAllDelayedCheckSets->first, GunDelay,PreColor,//����ǹλ��Ӧ��ǰһ״̬��Ҳ����Ӧ�ñ��ֵ�״̬��
						bReverse);
				}
			}
			GunCheckPointTmp.YPoint=itAllDelayedCheckSets->first;
			OneCheckPointExpandTmp.push_back(GunCheckPointTmp);//�洢�����ӳٺ��ͻ���λ��
			ColorOfExpandedCheckPointsTmp.push_back(PreColor);//�洢�����ӳٺ��ͻ��㴦����ɫ�����һһ��Ӧ
			
			
		}//�����һ�����PreColorӦ�뵱ǰ��ɫһ����
	}
	
	

	return true;

}
void DownValue::CoverGivenBitColorForGivenPoint(int BackCheckPoint,int CurCheckPoint,int GunIndex)
{
	int TheNeedCoverColorBit=m_CheckPointColorListWithDelay[CurCheckPoint]&GunNumValue[GunIndex];//��Ҫ����Щ��ǹ��Ӧ��λ����ɫ��ȡ���������ǻ�ȥ��ͻ�����Ӧλ
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
	int GunIndex=itvCurCheckPoint->second;//Ҫ���ݵ�ǹ��
	static unsigned MaxCheckPointNum=m_GunModelAndDistance.GunNember*2;
	float BackCheckPoint=itvPreCheckPoint->first;//Ҫ���ݵ�ͻ���
	unsigned BackCheckPointIndex=itvPreCheckPoint-m_vStackOfAllDelayPointMapInRow.begin();////Ҫ���ݵ�ͻ�����±�
	if (BackCheckPointIndex==6)
	{
		BackCheckPointIndex=6;
	}
	unsigned CurCheckPointIndex=itvCurCheckPoint-m_vStackOfAllDelayPointMapInRow.begin();
	CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
	///////////////���ԭʼͻ������ظ��Ļ�,�����ظ��Ŀ�����///////////////////////////////////////////////////////////
	unsigned FoundRepeated=0;//�ҵ����ظ������һ���ظ��������Ǽ��еļ��������ĵ�,ͬ��������������ǹ����2���Դ������������ظ���ķ�Χ
	CheckPointPairVecType::iterator itRepeatedCheckPointsMoved=itRepeatedCheckPoints;//����ÿ�δ�ͷ��ʼ��

	
	while (JudgeRepeatedCheckPoint(itvCurCheckPoint->first,itRepeatedCheckPointsMoved,m_vStackOfAllRepeatedOpenCheckPoints.end()))//itRepeatedCheckPoints���ں����ﱻ�ƶ���ע��
	{//��itRepeatedCheckPointsMoved��ʼ������ң� 
		/*itRepeatedCheckPointsMoved=itRepeatedCheckPoints;*/
		int GunIndex=itRepeatedCheckPointsMoved->first;//�����ظ���ǹ��
		CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
		itRepeatedCheckPointsMoved++;
		itRepeatedCheckPoints=itRepeatedCheckPointsMoved;//�ƶ���׼������ȥ
		FoundRepeated++;
		if (itRepeatedCheckPointsMoved==m_vStackOfAllRepeatedOpenCheckPoints.end()||FoundRepeated>MaxCheckPointNum)
		{break;
		}
	}
	///////////////���ԭʼͻ������ظ��Ļ�,�����ظ��Ĺر�����///////////////////////////////////////////////////////////
	itRepeatedCheckPointsMoved=itRepeatedCloseCheckPoints;//����ÿ�δ�ͷ��ʼ��
	FoundRepeated=0;
	while (JudgeRepeatedCheckPoint(itvCurCheckPoint->first,itRepeatedCheckPointsMoved,m_vStackOfAllRepeatedCloseCheckPoints.end()))//itRepeatedCheckPoints���ں����ﱻ�ƶ���ע��
	{
		int GunIndex=itRepeatedCheckPointsMoved->first;//�����ظ���ǹ��
		CoverGivenBitColorForGivenPoint(BackCheckPointIndex, CurCheckPointIndex,GunIndex);
		itRepeatedCheckPointsMoved++;
		itRepeatedCloseCheckPoints=itRepeatedCheckPointsMoved;
		FoundRepeated++;
		if (itRepeatedCheckPointsMoved==m_vStackOfAllRepeatedCloseCheckPoints.end()||FoundRepeated>MaxCheckPointNum)
		{break;
		}

	}
	
	

	
}
//�����ƶ��Ļ���һ����ͻ����ǰ��ĳ���Ļ�ҪС�Ļ�,��Ӧ��ǹ��λӦ�Գ�Խ��ǰ����Ǹ�Ϊ׼
bool DownValue::CheckConflictFromDifferentOriginCheckPoint(std::vector<std::pair<float,int>>::iterator it_CurCheckPoint,std::vector<std::pair<float,int>>& StackOfAllDelayPointMapInRow
	,CheckPointPairVecType::iterator& itRepeatedCheckPoints,CheckPointPairVecType::iterator& itRepeatedCloseCheckPoints
	,bool bReverse)
{
	std::vector<std::pair<float,int>>::iterator itvAllUnSortCheckPoint=it_CurCheckPoint;
	//itvAllUnSortCheckPoint=find_if(StackOfAllDelayPointMapInRow.begin(),StackOfAllDelayPointMapInRow.end(),CheckPointMap_finder(fCurCheckPoint));//�ҵ����ͻ����ڼ����е�λ��
	std::vector<std::pair<float,int>>::iterator itPre;
	if (itvAllUnSortCheckPoint==StackOfAllDelayPointMapInRow.begin())
	{
		itPre=StackOfAllDelayPointMapInRow.begin();//ǰһ��
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
		if (bReverse)//������Ϳ�˶�
		{
			while (itPre!=StackOfAllDelayPointMapInRow.begin()){
				if (it_CurCheckPoint->first >itPre->first )//�����µı��ϵĴ󣬳�ͻ 
				{
					BackSetConflictPointColor(itPre,itvAllUnSortCheckPoint,itRepeatedCheckPoints,itRepeatedCloseCheckPoints);
				}							
				itPre--;//���ֳ�ͻ��ȥ���ݣ��ҵ��µĳ�ͻ�㰴��˳��Ӧ���ŵ�˭�ĺ���ȥ
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
				if (it_CurCheckPoint->first <itPre->first )//�����µı��ϵ�С����ͻ 
				{
					BackSetConflictPointColor(itPre,itvAllUnSortCheckPoint,itRepeatedCheckPoints,itRepeatedCloseCheckPoints);
				}							
				itPre--;//���ֳ�ͻ��ȥ���ݣ��ҵ��µĳ�ͻ�㰴��˳��Ӧ���ŵ�˭�ĺ���ȥ
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
	//std::map<float,int> NearestPrePoints;//����ǰ������е㼯
	std::vector<std::pair<float,int>>::iterator NearestPrePoint=m_vStackOfAllDelayPointMapInRow.begin();////����ǰ�����ӽ��Լ��ĵ㣬��ֵΪ��Զ���Լ����Ǹ���ʼ��
	//bool bFounded=false;//��������ĳ��ͻ���ǰ��
	if (!bReverse)
	{
		for (itm_vStackOfAllDelayPointMapInRow;itm_vStackOfAllDelayPointMapInRow!=m_vStackOfAllDelayPointMapInRow.end();itm_vStackOfAllDelayPointMapInRow++)//�����б���С�Ķ�������
		{
			if (itm_vStackOfAllDelayPointMapInRow->first<CurCheckPointPos-TOLERANCE_OF_SAME_POINT)//���TOLERANCE_OF_SAME_POINT��Ϊ�˱�����ʱһ����119.999��һ����120.001��������ʵ��һ���㣬Ӧ�ر�ע��
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
		for (itm_vStackOfAllDelayPointMapInRow;itm_vStackOfAllDelayPointMapInRow!=m_vStackOfAllDelayPointMapInRow.end();itm_vStackOfAllDelayPointMapInRow++)//�����б���С�Ķ�������
		{
			if (itm_vStackOfAllDelayPointMapInRow->first>CurCheckPointPos+TOLERANCE_OF_SAME_POINT)//���TOLERANCE_OF_SAME_POINT��Ϊ�˱�����ʱһ����119.999��һ����120.001��������ʵ��һ���㣬Ӧ�ر�ע��
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
	
	
	if (NearestPrePoint!=m_vStackOfAllDelayPointMapInRow.begin())//�ҵ��˴�ǰ��״̬��
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
		return 0;//�����κ�ͻ���֮ǰ��ֻ���ǵ�һ������ǰɫ����Ϊ0
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
	//////////////////////////////��һ�����⴦��////////////////////////////////////////////
	CheckOneCheckPoint(CheckPointColorList.begin(),CheckPoint.begin(),0,GunDelay, GunNum,bReverse);
	int PreColor=*itCurColor;
	itCheckPoint++;
	itCurColor++;
	m_CheckPointListWithDelay.insert(m_CheckPointListWithDelay.end(),OneCheckPointExpandTmp.begin(),OneCheckPointExpandTmp.end());
	m_CheckPointColorListWithDelay.insert(m_CheckPointColorListWithDelay.end(),ColorOfExpandedCheckPointsTmp.begin(),ColorOfExpandedCheckPointsTmp.end());//����
	//////////////////////////////////////////////////////////////////////////
	for(itCheckPoint;itCheckPoint!=CheckPoint.end();itCheckPoint++)//���ԭʼͻ���
	{
		CheckOneCheckPoint(itCurColor,itCheckPoint,PreColor,GunDelay, GunNum,bReverse);
		PreColor=*itCurColor;
		itCurColor++;
		m_CheckPointListWithDelay.insert(m_CheckPointListWithDelay.end(),OneCheckPointExpandTmp.begin(),OneCheckPointExpandTmp.end());
		m_CheckPointColorListWithDelay.insert(m_CheckPointColorListWithDelay.end(),ColorOfExpandedCheckPointsTmp.begin(),ColorOfExpandedCheckPointsTmp.end());
	}
	
	/////////////////////////ǰ��ֻ�����˵���ͻ����������Ⱥ�˳�����⣬����ͬͻ�������󣬿��ܵ�����Ҫ�ٴ�����,����ֻ������δ������������ɫֵ��Ӱ�죬����Ǳ��bug/////////////////////////////////////////////////
	std::map<float,int> PoseAndIndex;//��ͻ��㴦���������Ӧ�ı�ţ�0��ʼ���Ⱥ�˳����
	std::map<float,int>::iterator itPoseAndInd;
	int i=0;
	if (m_CheckPointColorListWithDelay.size()!=m_CheckPointListWithDelay.size()||m_CheckPointColorListWithDelay.size()!=m_vStackOfAllDelayPointMapInRow.size())//����
	{
		return false;//
	}
	//VectorTypePoint::iterator itPrePoint=m_CheckPointListWithDelay.begin();//
	
	std::vector<std::pair<float,int>>::iterator it_CurCheckPoint=m_vStackOfAllDelayPointMapInRow.begin();
	CheckPointPairVecType::iterator itRepeatedCheckPoints=m_vStackOfAllRepeatedOpenCheckPoints.begin();
	CheckPointPairVecType::iterator itRepeatedCloseCheckPoints=m_vStackOfAllRepeatedCloseCheckPoints.begin();
	//std::map<float,int>::reverse_iterator itReverse_PoseAndInd;
	std::map<float,int>::iterator itCompPrePoseAndInd;
	for (VectorTypePoint::iterator itAllPoint=m_CheckPointListWithDelay.begin();itAllPoint!=m_CheckPointListWithDelay.end();itAllPoint++)//����
	{
		//itPoseAndInd=PoseAndIndex.end();			
		//if (itPoseAndInd!=PoseAndIndex.begin())//��ֹ��PoseAndIndex��һ��Ԫ��ʱ����
		//{
		//	itPoseAndInd--;	//PoseAndIndex�е����һ��Ԫ��
		bool bFoundSamePoint=false;
			for (itCompPrePoseAndInd=PoseAndIndex.begin();itCompPrePoseAndInd!=PoseAndIndex.end();itCompPrePoseAndInd++)//�ڴ�ǰ�����Ź���ĵ������Ƿ����غϵ�
			{
				if (fabs(itAllPoint->YPoint-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//�µĴ���������ϵ�PoseAndIndex�е��غϵ�
				{
					//PoseAndIndex[itAllPoint->YPoint]=i;//�µĸ����ϵ�
					itCompPrePoseAndInd->second=i;//�µĸ����ϵ�
					bFoundSamePoint=true;
				}
			}
			
			//////////////////////////����������ǰ���ѳ��ֳ�ͻ��ԭ�ص���ɫ����һ��////////////////////////////////////////////////
			//�����ƶ��Ļ���һ����ͻ����ǰ��ĳ���Ļ�ҪС�Ļ�,��Ӧ��ǹ��λ��
		/*	CheckConflictFromDifferentOriginCheckPoint(it_CurCheckPoint, m_vStackOfAllDelayPointMapInRow,itRepeatedCheckPoints,itRepeatedCloseCheckPoints,bReverse);
			it_CurCheckPoint++;
			i++;
			continue;*/

		//}		
		//////////////////////////����������ǰ���ѳ��ֳ�ͻ��ԭ�ص���ɫ����һ��////////////////////////////////////////////////
		//�����ƶ��Ļ���һ����ͻ����ǰ��ĳ���Ļ�ҪС�Ļ�,��Ӧ��ǹ��λ��
		CheckConflictFromDifferentOriginCheckPoint(it_CurCheckPoint, m_vStackOfAllDelayPointMapInRow,itRepeatedCheckPoints,itRepeatedCloseCheckPoints,bReverse);
		it_CurCheckPoint++;
		if (!bFoundSamePoint)
		{
			PoseAndIndex.insert(std::map<float,int>::value_type(itAllPoint->YPoint,i));
		}
		//////////////////////////////////////////////////////////////////////////		
		//itPrePoint++;
		i++;
	}//������ó�����
	VectorTypePoint SortedPointsTmp;
	VectorType SortedPointsColorTmp;
	if (bReverse)//�����˶�
	{
		std::map<float,int>::reverse_iterator itSortedIndex=PoseAndIndex.rbegin();//���������
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.rend();itSortedIndex++)//�������γ��µ�����
		{
			SortedPointsTmp.push_back(m_CheckPointListWithDelay[itSortedIndex->second]);
			SortedPointsColorTmp.push_back(m_CheckPointColorListWithDelay[itSortedIndex->second]);
			//vStackOfAllDelayPointMapInRowTmp.push_back(m_vStackOfAllDelayPointMapInRow[itSortedIndex->second]);//������ʱ��������
			//itSortedIndex++;
		}
	}
	else
	{
		std::map<float,int>::iterator itSortedIndex=PoseAndIndex.begin();
		//std::vector<std::pair<float,int>> vStackOfAllDelayPointMapInRowTmp;//m_vStackOfAllDelayPointMapInRow�е������������ʱ��������
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.end();itSortedIndex++)//�������γ��µ�����
		{
			SortedPointsTmp.push_back(m_CheckPointListWithDelay[itSortedIndex->second]);
			SortedPointsColorTmp.push_back(m_CheckPointColorListWithDelay[itSortedIndex->second]);
			//vStackOfAllDelayPointMapInRowTmp.push_back(m_vStackOfAllDelayPointMapInRow[itSortedIndex->second]);//������ʱ��������
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
	CString PointXY;             //��Ϳ�������
	float ChangedFrameX=m_CoordinateFrame.BeginXPoint*cos(m_CoordinateFrame.FrameRange)+m_CoordinateFrame.BeginYPoint*sin(m_CoordinateFrame.FrameRange);  //�õ���������ϵ���������
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
	float FrameRotateX=250*cos(m_CoordinateFrame.FrameRange);               //����Ϊ��ת��,���ڲ���
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
	fwrite(CornerSpeed,strlen(CornerSpeed),1,fp);     //ֱ���ٶ�
	fprintf(fp,"dwell 500\n");
	fprintf(fp,PointXY);
	fprintf(fp,"HOMEZ 2,3\n");
	JudgeIsFirstCorner=false;
	std::vector<GunPerBeginAndEnd>::iterator m_PointIndex;
	std::map<int,VectorType>::iterator m_GunColor;                     //��Ԫ��ɫ������
	std::map<int,VectorTypePoint>::iterator m_ChangedColorPosition;
	m_GunColor=GunColorAll.begin();
	m_ChangedColorPosition=AllChangeColorPosition.begin();
	m_PointIndex=m_GunPerBeginAndEnd.begin();                            //���������
	//���Ƚ������Ϳ���򶥵㵽��һ����Ҫ��Ϳ��

	int IndexBeginColown=0;  //�ҳ�Ӧ����Ϳ��Ϳ�ĵ�һ��������������к�
	int ColownValue=m_GunPerBeginAndEnd.size();                       //��ǰ������Ҫ��Ϳ������
	bool clockwise=true;                                              //��ǹ��˳ʱ�뻹����ʱ���˶�
	WriteAllDelayParm( GunDelay,fp);
	bool ReverseMove=false;
	//unsigned GunNum=m_GunModelAndDistance.GunNember;
	if (m_GunModelAndDistance.GunNember!=1)                           //����ǹ���ɴ���
	{
		for (int Index=0;Index<m_GunPerBeginAndEnd.size();Index++,m_GunColor++,m_ChangedColorPosition++)//ÿ��
		{
			JudgeIsOrFirstPer=true;
			VectorType m_EveryVectorl=m_GunColor->second;    //ÿһ�еĵ�Ԫ��ɫ
			VectorTypePoint m_VectorTypePosition=m_ChangedColorPosition->second;
			m_fMaxRange=m_GunPerBeginAndEnd[Index].End>m_GunPerBeginAndEnd[Index].Begin?m_GunPerBeginAndEnd[Index].End:m_GunPerBeginAndEnd[Index].Begin;
			//if (!CheckEveryGunDelayAndAddCheckPoint(m_VectorTypePosition,m_EveryVectorl,GunDelay,m_GunModelAndDistance.GunNember,ReverseMove))//������ʱ��Ϣ
			if (!CalculateAllDelayedCheckPointInRow(m_VectorTypePosition,m_EveryVectorl,ReverseMove?GunDelayInReverse:GunDelay,m_GunModelAndDistance.GunNember,ReverseMove))//���ӳٿ��ǽ�ȥ
			{
				return false;//����
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
	else///////��ɫ��ǹδ�޸Ŀ����ӳ�
	{
		for (m_PointIndex,m_GunColor;m_PointIndex!=m_GunPerBeginAndEnd.end();m_PointIndex++,m_GunColor++)
		{
			int X=(*m_PointIndex).Index*PerWidth;            //X����
			int BeginY=(*m_PointIndex).Begin;                //ÿһ�е���ʼ����
			int EndY=(*m_PointIndex).End;                    //ÿһ�е���ֹ����
			CString Storage="";
			Storage.Format("x %d y %d\n",X,BeginY);
			fwrite(Storage,strlen(Storage),1,fp);
			CString EveryCellColor=CompileGunCondition(0);
			fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
			VectorType m_EveryVectorl=m_GunColor->second;    //ÿһ�еĵ�Ԫ��ɫ
			VectorType::iterator m_ColumnUnitColor=m_EveryVectorl.begin();
			for (int i=0;i<(m_EveryVectorl.size()-1);i++,m_ColumnUnitColor++)
			{
				if (clockwise==true)                        //��ǹ˳����     
				{
					int Y=BeginY+(i+1)*PerHeight;
					CString Storage="";
					Storage.Format("x %d y %d\n",X,Y);
					fwrite(Storage,strlen(Storage),1,fp);
				}
				else                                       //��ǹ��ʱ��
				{
					int Y=EndY+(m_EveryVectorl.size()-1)*PerHeight-(i+1)*PerHeight;
					CString Storage="";
					Storage.Format("x %d y %d\n",X,Y);
					fwrite(Storage,strlen(Storage),1,fp);
				}
				CString EveryCellColor=CompileGunCondition(*m_ColumnUnitColor);
				fwrite(EveryCellColor,strlen(EveryCellColor),1,fp);
			}
			clockwise=!clockwise;              //˳ʱ������ʱ�뽻�����
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
	//	int X=(*m_PointIndex).Index*PerHeight;            //X����
	//	if (JudgeIsFirstCorner==true)          //��ÿһ��ת��ʱ��Ҫ���ƶ�����
	//	{
	//		CString CornerSpeed="";
	//		CornerSpeed.Format("F %d\n",(int)Speed*SPEEDTRANSLATE);
	//		fwrite(CornerSpeed,strlen(CornerSpeed),1,fp);          //ֱ���ٶ�
	//		CString StoragePoint="";
	//		StoragePoint.Format("x %d y %d\n",X,m_CornerPoint.YPoint);
	//		fwrite(StoragePoint,strlen(StoragePoint),1,fp);
	//		CString EveryCellColorCorner=CompileGunCondition(m_CornerPointColor);
	//		fwrite(EveryCellColorCorner,strlen(EveryCellColorCorner),1,fp);
	//	}
	//	CString LineSpeed="";
	//	LineSpeed.Format("F %d\n",LINESPEED);
	//	fwrite(LineSpeed,strlen(LineSpeed),1,fp);          //ֱ���ٶ�
	//	JudgeIsFirstCorner=true;
	//	int tt=m_GunColor->first;
	//	VectorType m_EveryVectorl=m_GunColor->second;
	//	VectorType::iterator m_ColumnColorIndex;
	//	m_ColumnColorIndex=m_EveryVectorl.begin();
	//	int Size=m_EveryVectorl.size()-1;    //��Ԫ����ɫ��1
	//	for (int Index=1;Index<=Size;Index++,m_ColumnColorIndex++)
	//	{
	//		int YChangePoint=PerHeight;
	//		if ((*m_PointIndex).End>(*m_PointIndex).Begin)
	//		{
	//			YChangePoint=YChangePoint;
	//		}
	//		else
	//			YChangePoint=-YChangePoint;              //��һ���յ�������������Сʱ��Y������Ӧ��С
	//		int Y=(*m_PointIndex).Begin+Index*YChangePoint;     //Y����
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
	//	m_CornerPointColor=(*m_ColumnColorIndex);   //��Ϊ��ǹ״̬
	//}
	//CString StoragePointEnd="";
	//StoragePointEnd.Format("x %d y %d\n",m_CornerPoint.XPoint,m_CornerPoint.YPoint);
	//fwrite(StoragePointEnd,strlen(StoragePointEnd),1,fp);
	//CString EndCellColorCorner=CompileGunCondition(0);            //����ȫ���ر�
	//fwrite(EndCellColorCorner,strlen(EndCellColorCorner),1,fp);
	//fprintf(fp,"close\n");
	//fclose(fp);
	return true;
}
void DownValue::CheckHeight(float Height)
{
	HeightPer=(int)Height;
}
// д���ӳٲ���

bool DownValue::WriteDelayParm(float OpenDelay, float CloseDelay,FILE* fp)
{
/*	CString Storage="";
	Storage.Format("Op %f Cl %f\n",OpenDelay,CloseDelay);
	fwrite(Storage,strlen(Storage),1,fp);*/	

	return false;
}
////////////////////����2//////////////////////////////////////////////////////
void DownValue::ExpandOneOriginCheckPoint(
	std::map< float,int>& DelayMap,//�ӳ�֮���ͻ���,���¿����ӳٵ�ǹ��,
	std::map< float,int>& CloseDelayMap,//�ӳ�֮���ͻ���,���¹ر��ӳٵ�ǹ��,<
	std::map<float,int>&AllDelayPointMap,//�����ӳ�֮���ͻ���,�����ӳٵ�ǹ��,<
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
		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),rItAllDelayPointMap,rItAllDelayPointMapEnd);//���������г�ͻ������

	}
	else
	{

		m_vStackOfAllDelayPointMapInRow.insert(m_vStackOfAllDelayPointMapInRow.end(),AllDelayPointMap.begin(),AllDelayPointMap.end());//���������г�ͻ������
	}
	//CheckPointPairVecType RepeatedOpenCheckPoints=CheckGetRepeatedDelay2(OpenDelayPoints,GunNum);
	//CheckPointPairVecType RepeatedCloseCheckPoints=CheckGetRepeatedDelay2(CloseDelayPoints,GunNum);
	//m_vStackOfAllRepeatedOpenCheckPoints.insert(m_vStackOfAllRepeatedOpenCheckPoints.end(),RepeatedOpenCheckPoints.begin(),RepeatedOpenCheckPoints.end());//���������г�ͻ������
	//m_vStackOfAllRepeatedCloseCheckPoints.insert(m_vStackOfAllRepeatedCloseCheckPoints.end(),RepeatedCloseCheckPoints.begin(),RepeatedCloseCheckPoints.end());//���������г�ͻ������
	
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
	for (it_CurCheckPoint;it_CurCheckPoint!=m_vStackOfAllDelayPointMapInRow.end();it_CurCheckPoint++)//����
	{

		bool bFoundSamePoint=false;
		for (itCompPrePoseAndInd=PoseAndIndex.begin();itCompPrePoseAndInd!=PoseAndIndex.end();itCompPrePoseAndInd++)//�ڴ�ǰ�����Ź���ĵ������Ƿ����غϵ�
		{
			if (fabs(it_CurCheckPoint->first-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//�µĴ���������ϵ�PoseAndIndex�е��غϵ�
			{
				//PoseAndIndex[itAllPoint->YPoint]=i;//�µĸ����ϵ�
				itCompPrePoseAndInd->second=i;//�µĸ����ϵ�
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
	}//������ó�����
	VectorTypePoint SortedPointsTmp;
	//VectorType SortedPointsColorTmp;
	if (bReverse)//�����˶�
	{
		CornerPoint Tmp;
		std::map<float,int>::reverse_iterator itSortedIndex=PoseAndIndex.rbegin();//���������
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.rend();itSortedIndex++)//�������γ��µ�����
		{
			Tmp(XCoord,itSortedIndex->first);
			SortedPointsTmp.push_back(Tmp);
		
		}
	}
	else
	{
		std::map<float,int>::iterator itSortedIndex=PoseAndIndex.begin();
		CornerPoint Tmp;
		for (itSortedIndex;itSortedIndex!=PoseAndIndex.end();itSortedIndex++)//�������γ��µ�����
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
	std::map< float,int>::iterator itSortedOpenCheckSets=m_StackOfAllOpenDelayMap.begin();//map�����Զ���keyֵҲ��������ֵ��С��������,����ɾ��keyֵ�ظ������
	std::map< float,int>::iterator itSortedCloseCheckSets=m_StackOfAllCloseDelayMap.begin();//map�����Զ���keyֵҲ��������ֵ��С��������
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
		int GunStatuTmp=((*itCurColor)&GunNumValue[(GunIndex)]);//���ǹ�ÿ����ǹأ�����λ�Ƿ�Ϊ1
		if (!bGunOpened&&GunStatuTmp>0)//û����ȴҪ����
		{
			OpenCheckPointAfterDelay=AddStep*GunDelay[GunIndex][0]+(double)itCheckPoint->YPoint;
			LimitCheckPoint(OpenCheckPointAfterDelay,CloseCheckPointAfterDelay,m_fMaxRange);
			//CheckPointTmp.YPoint=OpenCheckPointAfterDelay;
			vExpandedCheckPointForThisGun.push_back(OpenCheckPointAfterDelay);
			vColorsForEveryExpandedCheckPoint.push_back(GunStatuTmp);
			bGunOpened=true;
		}
		else if(bGunOpened&&GunStatuTmp==0)//���ˣ�ȴҪ����
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
	std::map<float,int> PoseAndColor;//λ�á���ɫ
	std::map<float,int>::iterator itCompPrePoseAndInd;
	bool bFoundSamePoint=false;
	for(int i=0;i<GunNum;i++)
	{
		VectorType::iterator itCurColor=m_vStackOfAllColorsIndexByGun[i].begin();
		std::vector<float>::iterator itCheckPoint=m_vStackOfAllDelayPointIndexByGun[i].begin();
		for (itCheckPoint;itCheckPoint!=m_vStackOfAllDelayPointIndexByGun[i].end();itCheckPoint++)//�����i��ǹ�����е�����ɫ
		{
			for (itCompPrePoseAndInd=PoseAndColor.begin();itCompPrePoseAndInd!=PoseAndColor.end();itCompPrePoseAndInd++)//�ڴ�ǰ�����Ź���ĵ������Ƿ����غϵ�
			{
				if (fabs(*itCheckPoint-itCompPrePoseAndInd->first)<TOLERANCE_OF_SAME_POINT)//�غϵ�
				{
					itCompPrePoseAndInd->second|=*itCurColor;
				}
				if (*itCheckPoint>itCompPrePoseAndInd->first+TOLERANCE_OF_SAME_POINT)//�µĴ������λ������
				{
					break;
				//	bFoundSamePoint=true;
				}
			}
			for (itCompPrePoseAndInd;itCompPrePoseAndInd!=PoseAndColor.end();itCompPrePoseAndInd++)//��д������λ��֮������е��״̬
			{
				itCompPrePoseAndInd->second|=*itCurColor;
			}
			PoseAndColor.insert(std::map<float,int>::value_type(*itCheckPoint,*itCurColor));//����µ�
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
		for (itCheckPoint;itCheckPoint!=m_vStackOfAllDelayPointIndexByGun[i].end();itCheckPoint)//�����i��ǹ�����е�����ɫ
		{
			StartIndex=FindPoseInSortedVec(*itCheckPoint,itStartCheckPointInSortedVec,m_CheckPointListWithDelay.end());//�ҵ�״̬��ʼ���λ��
			itCheckPoint++;
			if (itCheckPoint==m_vStackOfAllDelayPointIndexByGun[i].end())
			{
				AddColor( StartIndex ,*itCurColor);
				break;
			}
			EndIndex=FindPoseInSortedVec(*itCheckPoint,itStartCheckPointInSortedVec,m_CheckPointListWithDelay.end());//�ҵ�״̬�������λ��
			for (StartIndex;StartIndex<EndIndex;StartIndex++)//��״̬д��
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
	if (m_CheckPointColorListWithDelay.size()==NumOfOriginCheckPoint)//û�ӳ٣�������
	{return false;
	}
	VectorType::iterator it_vCheckPointColor=m_CheckPointColorListWithDelay.begin();
	VectorTypePoint::iterator it_vCheckPoint=m_CheckPointListWithDelay.begin();
	VectorType::iterator it_vCheckPointColorPre=it_vCheckPointColor;
	it_vCheckPointColor++;
	it_vCheckPoint++;
	for (it_vCheckPointColor;it_vCheckPointColor!=m_CheckPointColorListWithDelay.end();it_vCheckPointColor)
	{
		if (*it_vCheckPointColor==*it_vCheckPointColorPre)//�ظ�״̬
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
	//int FirstAndLastGunDistance=(GunNum-1)*m_fGunDis;               //��һ����ǹ�����һ����ǹ֮��ļ��
	//float columnHeight=(*itRowBeginAndEndIndex).End-(*itRowBeginAndEndIndex).Begin-FirstAndLastGunDistance;
	m_ParmAdjust.TransitTheRows(m_CheckPointListWithDelay,m_CheckPointColorListWithDelay,DEFAULT_TRANSIT_VALUE,MAXPAINTRANGEY,!bReverse);//����
	m_ParmAdjust.AddRowStartPointForOneRowMotion(m_CheckPointListWithDelay,m_CheckPointColorListWithDelay);
	//VectorType::iterator itCurOriginColor;
	////std::map<float,int>::iterator itCurDelayedCheckPoint=AllDelayPointMap.begin();
	//int PreColor=0;

	//for (std::map<float,int>::iterator itCurDelayedCheckPoint=AllDelayPointMap.begin();itCurDelayedCheckPoint!=AllDelayPointMap.end();itCurDelayedCheckPoint++)
	//{
	//	itCurOriginColor=CheckPointColorList.begin()+m_mExpandedPointAndOriginCheckPointIndexMap[itCurDelayedCheckPoint->first];		
	//	CalCulateOneExpandedCheckPointStatues(itCurOriginColor,itCurDelayedCheckPoint,PreColor,bReverse);
	//	ColorOfExpandedCheckPointsTmp.push_back(PreColor);//�洢�����ӳٺ��ͻ��㴦����ɫ�����һһ��Ӧ	
	//
	//}
	//m_CheckPointColorListWithDelay=ColorOfExpandedCheckPointsTmp;
	//m_CheckPointListWithDelay=OneCheckPointExpandTmp;


	return true;

}


#pragma once
#include "DownParmValueHead.h"
/*!
 * \class CDownParmAdjust
 *
 * \ingroup GroupName
 *
 * \brief �������ڸ��γɵ��´��������е������Ա��������ʵ�����ص����ݣ�����ת��DownParmValue����û�̳�
 * ��ɫ״̬������m_CheckPointColorListWithDelay��ͻ���״̬������m_CheckPointListWithDelay
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
 * \date ʮ���� 2017
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
 *@brief �Ը�����ʩ��ƽ�Ʋ�����ƽ��ֵ�ڳ�Ա����m_TransitValue�У����ж�ƽ�ƺ��Ƿ����곬��0~m_MaxLength
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
 *@brief ��һ���е��������ݽ���ƽ�ƣ�TransitTheRows����
 *
 *@parm
 *@parm
 *
 *@return
 *@todo
 */
	void TransitTheRealWorkPartsOftheRows(VectorTypePoint& DownParm,int TransitValue)
	{
		m_TransitValue=TransitValue;//bUnEvenRow?TransitValue:-TransitValue;//������+��ż���м�
		VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		//it_vCheckPoint++;//��һ������ʼλ��//��һ����㲻�����õ�ʵ�����¼ӽ�ȥ��ÿ��ǿ��Ҫ��رյ���ʼ�㣨ȡ����
		for (it_vCheckPoint;it_vCheckPoint!=DownParm.end();it_vCheckPoint++)
		{
			AddTransitValue(*it_vCheckPoint);
		}
	}
public:
/*!
 *@brief ƽ�Ƹ��У����Ǹ��в��������ΧΪ0-MaxLength��ȡ����������ƽ��ǰ�����λ�ã�����ĵ�λ����ɫλ0��
 *���ԸĶ�����������Ϳ��������ƽ���ˡ�ƽ�ƺ�Ľ����Ȼ����������� DownParm��DownParmColor������
 *
 *@parm DownParm �и�ͻ��������
 *@parm DownParmColor �и�ͻ����Ӧ����ɫ
 *@parm TransitValue ȫ����������ƽ�Ƶ���ֵ��ʵ���϶�ż���л���һ�������ƽ�ƣ���ƽ����ֵ�ɺ�EXTRA_TRANSIT_FOR_EVEN_ROWȷ��
 *@parm MaxLength �����˶���Χ�����������Χ�ᱻ�ضϣ��Է��˶�����
 *@parm bUnEvenRow �Ƿ�ǰΪż����
 *
 *@return
 *@todo
 */
	void TransitTheRows(VectorTypePoint& DownParm,VectorType& DownParmColor,int TransitValue,float MaxLength,bool bUnEvenRow)
	{

		
		m_MaxLength=MaxLength;
		//VectorType::iterator it_vCheckPointColor=DownParm.begin();
				
		/////////////////////ƽ��֮ǰ����ͷ�ϰ���㸴��һ��/////////////////////////////////////////////////////
		//CornerPoint HeadPoint=*(DownParm.begin());//��һ���㸴��
		//DownParm.insert(DownParm.begin(),HeadPoint);
		//DownParmColor.insert(DownParmColor.begin(),0);//��������ʼ���Ӧ����ɫ0,�����ȫ��
		
		//////////////////////////////////////////////////////////////////////////
		TransitTheRealWorkPartsOftheRows(DownParm,TransitValue);//����ƽ��


		/*		int EXTRA_TRANSIT_FOR_EVEN_ROW=-40;��ʼ�ļ�Ϊ-40�����Ը����޸��ļ��޸Ĳ���*/
		int EXTRA_TRANSIT_FOR_EVEN_ROW;
		FILE *fp;
		char buffer[256];
		if ( fp=fopen("E:\\ż�����ƶ�����.txt","r"));
		fgets(buffer,255,fp); 
		EXTRA_TRANSIT_FOR_EVEN_ROW=atoi(buffer);
		fclose(fp);

		if (!bUnEvenRow)//ż�������ƶ�����mm
		{
			TransitTheRealWorkPartsOftheRows(DownParm,EXTRA_TRANSIT_FOR_EVEN_ROW);//����ƽ��
		}

		FILE *m_p=fopen("E:\\ż�����ƶ�����.txt","w");
		CString str;
		str.Format(_T("%d"),EXTRA_TRANSIT_FOR_EVEN_ROW);
		fwrite(str,1,str.GetLength(),m_p);
		str.ReleaseBuffer();
		fclose(m_p);

		//VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		//it_vCheckPoint++;//��һ������ʼλ��
		//for (it_vCheckPoint;it_vCheckPoint!=DownParm.end();it_vCheckPoint++)
		//{
		//	AddTransitValue(*it_vCheckPoint);
		//}
	//	std::for_each(it_vCheckPoint,DownParm.end(),AddTransitValue);	
		
		
	}
/*!
 *@brief ����һ�н������ƶ�����һ��ʱ����б���˶�������һ���˶�ǰ������һ���µ���㣬�����Y��������һ���˶����յ�
 *
 *@parm DownParm ��ǰ�����е��˶���
 *@parm DownParmColor �и�ͻ����Ӧ����ɫ
 *
 *@return
 *@todo
 */
	bool AddRowStartPointForOneRowMotion(VectorTypePoint& DownParm,VectorType& DownParmColor)
	{
		if (PreRowPoint.size()==0)//��һ��
		{
			PreRowPoint=DownParm;
			return false;
		}
		VectorTypePoint::iterator it_vPreCheckPoint=PreRowPoint.end();
		it_vPreCheckPoint--;
		CornerPoint NewStartPoint=*it_vPreCheckPoint;//��һ�����һ����
		VectorTypePoint::iterator it_vCheckPoint=DownParm.begin();
		NewStartPoint.XPoint=it_vCheckPoint->XPoint;
		DownParm.insert(it_vCheckPoint,NewStartPoint);
		DownParmColor.insert(DownParmColor.begin(),0);
		PreRowPoint=DownParm;
		return true;
	}
protected:
	VectorTypePoint PreRowPoint;///<��һ�����еĵ�
	int m_TransitValue;
	float m_MaxLength;
};


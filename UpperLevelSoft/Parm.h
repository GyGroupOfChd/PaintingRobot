/** @file 
*@brief ��������ļ�
* @author YangGAO
* @date 2011/09/02
* @version 1.0
*
* ���ļ����ڶ����õ���ȫ�ֱ�������������һ�����ض�Ӧ��ʵ�ʾ����Ƕ��٣��������ķֱ�����ʵ�ʾ���Ĺ�ϵ��
*/
#pragma once
#ifndef PARM_INCLUDED
#define PARM_INCLUDED
#include "PaintCell.h"
/********************************************************************
	created:	2011/09/02
	created:	2:9:2011   23:38
	filename: 	f:\�ҵ���Ŀ\SoftProj\UpperLevelSoft\UpperLevelSoft\Parm.h
	file path:	f:\�ҵ���Ŀ\SoftProj\UpperLevelSoft\UpperLevelSoft
	file base:	Parm
	file ext:	h
	author:		YangGAO
	
	purpose:	���ļ����ڶ����õ���ȫ�ֱ�������������һ�����ض�Ӧ��ʵ�ʾ����Ƕ��٣��������ķֱ�����ʵ�ʾ���Ĺ�ϵ��
*********************************************************************/
//////////////Ĭ��ֵ////////////////////////////////////////////////////////////
#define MM_PER_PIXEL 10///<ÿһ���ض�ӦΪ����mm��Ĭ��ֵ
#define Y_AXIS_MOVING_SPEED_IN_MM 40///<��Y�����е��ٶȣ���λmm��Ĭ��ֵ
#define X_AXIS_MOVING_SPEED_IN_MM 40///<��Y�����е��ٶȣ���λmm��Ĭ��ֵ
#define WIDTH_OF_REGION_IN_MM 240///<��������Ĭ�ϵĿ�ȣ���λmm
#define HEIGHT_OF_REGION_IN_MM 470///<��������Ĭ�ϵĸ߶ȣ���λmm
#define WIDTH_OF_ROW_IN_MM 100///<ÿ�������ڻ��ֵ��еĿ��
#define HEIGHT_OF_PAINT_CELL_IN_MM 100///<ÿ����Ϳ��Ԫ�ĸ߶ȣ���λmm��Ĭ��ֵ
#define MINI_CELL_WIDTH_THREAD 4///<һ����Ϳ��Ԫ��ȵ���Сֵ��С�������ȵĿ϶�������Ϳ��Ԫ
#define INDUCE_LENGTH 10///<˿�ܵ���mm

#define MAX_ORIENTION_CORRECT 0.2           ///���׼��ƫ�����Ƕ�
#define MAX_X_CORRECT 30                    ///���׼X��ƫ��������
#define MAX_Y_CORRECT 30                    ///���׼Y��ƫ��������
//////////////////////////////////////////////////////////////////////////
extern double gMMPerPixel;///<ÿһ���ض�ӦΪ����mm
extern bool gbOnePixelBetweenWorkReg;///<�ڹ�����֮�����һ�����ؿ�ȵļ��
extern enum AreaPartitionFlag{PARTION_BY_WIDTH,PARTION_BY_FIRST_FOUND,PARTION_BY_INTELLIGENCE,GRID_PARTION_BY_INTELLIGENCE}geAreaPartitionFlag;///<��־λ��������Ԥ�Ȱ��̶����������С�ָ�������ͼΪ���ɸ��������䣻�����Զ�Ѱ�ң��ӵ�һ������Ϳ�ķ���λ��ʼ���л��֣��������ܻ��Զ�����(Ŀǰδ��)
extern enum CellSizePreRecFlag{AVERAGE_SIZE_OF_CELL,MINI_SIZE_OF_CELL}gCellSizePreRecFlag;///<��־λ�����ƶԵ�Ԫ�Ĳ���Ԥʶ��ʱ�����и��ӵ�ƽ�����㣬���Ǽ򵥵�ֱ��ȡ��Сֵ��

extern PaintAreaTemplate g_sPaintAreaParm;///<ȫ�ֱ���������������������Ĳ���
//const 
extern CRect_Work_Region g_cWorkRegionParm;///<���幤������Ĳ���
//const 
extern COLORREF gNULLCOLOR;
extern long gRowWidth;///<ÿ����Ϳ�еĿ�ȣ���λ���ر��븳ֵ
extern long gHeightOfEveryPaintCell;///<ÿ����Ϳ��Ԫ�ĸ߶ȣ���λ���أ����븳ֵ����ʵ��ʶ�����ʱ��û�а�����ߣ����ǰ�ʵ�ʸ߶����ģ�ת����Ԥ����С��Ϳ����ĳߴ�ʹ��߽粻���ڴ�����Ϳ��Ԫ
static struct GapBetweenCellTemplate
{
	unsigned m_uWidth;///<��϶��ȣ���λ����
	//unsigned m_uHeight;
	COLORREF m_color;///<��϶����ɫ
	bool s_bGapExist;
}g_sGapBetweenCellParm;
//��ǹ��ģʽ�������
struct GunParm
{
	enum GunModel//��ǹģʽ
	{
		MultiGun=1,OneMultiColoGun=2,MultiMultiColoGun=3
	}CurgunMode;
	long GunNum;///<��ǹ��
	long GunDis;///<�����ǹ�Ļ���ǹ���
	long ColoPerGun;///<ÿ����ǹ����Ŀ
};
/*
���¶���ĺ꣬ȫ������Ϳ����Ӳ���й�
*/
//������Ƶ���Ϳ������������Ϳ��ΧΪ1250*1800
#define MAXPAINTRANGEY 1250     //��ǰ��Ƶ���Ϳ������Y�������Ϳ��ΧΪ1250mm
#define MAXPAINTRANGEX 1850    //��ǰ��Ƶ���Ϳ������X�������Ϳ��ΧΪ1250mm
#define DEFAULT_NUM_OF_GUN 4//Ĭ����ǹ��
#define DEFAULT_TRANSIT_VALUE 100//�������ƵĲ�������λmm
//#define EXTRA_TRANSIT_FOR_EVEN_ROW -60//ż���ж�������ƾ��룬��λmm,Y����Ϊ��
//���ڵ�һ�������˵��������ϵͳ��ռ�ݷǳ���Ҫ�����ã�������Ҫȷ���ڵ�ǰλ�ã���ǹ������λ�ã�
//һ������FIRSTCAMERAXORIGINPOINTΪ�ڵ�ǰ����ϵ�е�һ������İ�װλ�ã�X���꣬FIRSTCAMERAYORIGINPOINTΪY���꣬Ҫ��֤�����װ�����뵱ǰ����ϵƽ��
//����İ�װ����ȫ���������λ�����
#define FIRSTCAMERAXORIGINPOINT 340             //����Ϳ��������ϵ�µ�X��װλ�ã���λmm
#define FIRSTCAMERAYORIGINPOINT 1250            //����Ϳ��������ϵ�µ�Y��װλ�ã���λmm
//�ڶ�������ڵ�ǰ����ϵ�е�λ��
#define SECONDCAMERAXORIGINPOINT 1845           //����Ϳ��������ϵ�µ�X��װλ�ã���λmm
#define SECONDCAMERAYORIGINPOINT 1250           //����Ϳ��������ϵ�µ�Y��װλ�ã���λmm
//����Ϳ�����У�ʵ��Ҫ����ͷ���Ķ�׼����Ϳ�ĵ�һ����Ԫ��������λ��������ǹ���������в��

//�����������ԭ�㰲װλ���ڼ���λ��ԭ��������ϣ�һ��������Ϊ��������Ϊ��
#define FRAMEX 100        //��λ������ԭ����X����������ԭ������ľ���
#define FRAMEY 0         //��λ������ԭ����Y����������ԭ������ľ���
#define ANGLELIMIT 5*3.1415926/180 //����Ϳ��������ϵ����Ϳ��������ϵ�нǼ���
#define FRAMEXLIMIT 150  //����Ϳ�������½�ԭ������Ϳ�������½�ԭ����X���ϵļ���
#define FRAMEYLIMIT 150   //����Ϳ�������½�ԭ������Ϳ�������½�ԭ����Y���ϵļ���

//���ڶ���ǹ��˵����Ҫ��¼��һ����ǹ�����һ����ǹ�İ�װλ��
#define FIRSTGUNPOINTY 90
#define FINALLYGUNPOINTY -90
#endif
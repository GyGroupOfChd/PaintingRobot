/** @file 
*@brief 参数存放文件
* @author YangGAO
* @date 2011/09/02
* @version 1.0
*
* 本文件用于定义用到的全局变量参数，比如一个像素对应的实际距离是多少，编码器的分辨率与实际距离的关系等
*/
#pragma once
#ifndef PARM_INCLUDED
#define PARM_INCLUDED
#include "PaintCell.h"
/********************************************************************
	created:	2011/09/02
	created:	2:9:2011   23:38
	filename: 	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft\Parm.h
	file path:	f:\我的项目\SoftProj\UpperLevelSoft\UpperLevelSoft
	file base:	Parm
	file ext:	h
	author:		YangGAO
	
	purpose:	本文件用于定义用到的全局变量参数，比如一个像素对应的实际距离是多少，编码器的分辨率与实际距离的关系等
*********************************************************************/
//////////////默认值////////////////////////////////////////////////////////////
#define MM_PER_PIXEL 10///<每一像素对应为几个mm，默认值
#define Y_AXIS_MOVING_SPEED_IN_MM 40///<沿Y轴运行的速度，单位mm，默认值
#define X_AXIS_MOVING_SPEED_IN_MM 40///<沿Y轴运行的速度，单位mm，默认值
#define WIDTH_OF_REGION_IN_MM 240///<工作区域默认的宽度，单位mm
#define HEIGHT_OF_REGION_IN_MM 470///<工作区域默认的高度，单位mm
#define WIDTH_OF_ROW_IN_MM 100///<每个区域内划分的列的宽度
#define HEIGHT_OF_PAINT_CELL_IN_MM 100///<每个喷涂单元的高度，单位mm，默认值
#define MINI_CELL_WIDTH_THREAD 4///<一个喷涂单元宽度的最小值，小于这个宽度的肯定不是喷涂单元
#define INDUCE_LENGTH 10///<丝杠导程mm

#define MAX_ORIENTION_CORRECT 0.2           ///与基准的偏差最大角度
#define MAX_X_CORRECT 30                    ///与基准X轴偏差最大距离
#define MAX_Y_CORRECT 30                    ///与基准Y轴偏差最大距离
//////////////////////////////////////////////////////////////////////////
extern double gMMPerPixel;///<每一像素对应为几个mm
extern bool gbOnePixelBetweenWorkReg;///<在工作区之间存在一个像素宽度的间距
extern enum AreaPartitionFlag{PARTION_BY_WIDTH,PARTION_BY_FIRST_FOUND,PARTION_BY_INTELLIGENCE,GRID_PARTION_BY_INTELLIGENCE}geAreaPartitionFlag;///<标志位，控制是预先按固定工作区间大小分割整个地图为若干个工作区间；还是自动寻找，从第一个需喷涂的方块位开始逐行划分；还是智能化自动划分(目前未做)
extern enum CellSizePreRecFlag{AVERAGE_SIZE_OF_CELL,MINI_SIZE_OF_CELL}gCellSizePreRecFlag;///<标志位，控制对单元的参数预识别时，进行复杂的平均计算，还是简单的直接取最小值。

extern PaintAreaTemplate g_sPaintAreaParm;///<全局变量，定义整个工作区域的参数
//const 
extern CRect_Work_Region g_cWorkRegionParm;///<定义工作区域的参数
//const 
extern COLORREF gNULLCOLOR;
extern long gRowWidth;///<每个喷涂列的宽度，单位像素必须赋值
extern long gHeightOfEveryPaintCell;///<每个喷涂单元的高度，单位像素，必须赋值，在实际识别操作时，没有按这个走，而是按实际高度来的，转用于预先缩小喷涂区域的尺寸使其边界不至于穿过喷涂单元
static struct GapBetweenCellTemplate
{
	unsigned m_uWidth;///<间隙宽度，单位像素
	//unsigned m_uHeight;
	COLORREF m_color;///<间隙的颜色
	bool s_bGapExist;
}g_sGapBetweenCellParm;
//喷枪的模式，与参数
struct GunParm
{
	enum GunModel//喷枪模式
	{
		MultiGun=1,OneMultiColoGun=2,MultiMultiColoGun=3
	}CurgunMode;
	long GunNum;///<喷枪数
	long GunDis;///<多把喷枪的话喷枪间距
	long ColoPerGun;///<每把喷枪的数目
};
/*
以下定义的宏，全部跟喷涂机器硬件有关
*/
//对于设计的喷涂机器，最大的喷涂范围为1250*1800
#define MAXPAINTRANGEY 1250     //当前设计的喷涂机器，Y轴最大喷涂范围为1250mm
#define MAXPAINTRANGEX 1850    //当前设计的喷涂机器，X轴最大喷涂范围为1250mm
#define DEFAULT_NUM_OF_GUN 4//默认喷枪数
#define DEFAULT_TRANSIT_VALUE 100//整体下移的参数，单位mm
//#define EXTRA_TRANSIT_FOR_EVEN_ROW -60//偶数列额外的下移距离，单位mm,Y增加为正
//对于第一个相机来说，在整个系统中占据非常重要的作用，首先需要确定在当前位置，喷枪的启动位置；
//一下数据FIRSTCAMERAXORIGINPOINT为在当前坐标系中第一个相机的安装位置，X坐标，FIRSTCAMERAYORIGINPOINT为Y坐标，要保证相机安装精度与当前坐标系平行
//相机的安装坐标全部相对于限位来测的
#define FIRSTCAMERAXORIGINPOINT 340             //在喷涂机器坐标系下的X安装位置，单位mm
#define FIRSTCAMERAYORIGINPOINT 1250            //在喷涂机器坐标系下的Y安装位置，单位mm
//第二个相机在当前坐标系中的位置
#define SECONDCAMERAXORIGINPOINT 1845           //在喷涂机器坐标系下的X安装位置，单位mm
#define SECONDCAMERAYORIGINPOINT 1250           //在喷涂机器坐标系下的Y安装位置，单位mm
//在喷涂过程中，实际要将喷头中心对准带喷涂的第一个单元，所以限位坐标与喷枪坐标坐标有差别

//如果喷嘴坐标原点安装位置在极限位置原点的正轴上，一下两个宏为正，否者为负
#define FRAMEX 100        //限位坐标轴原点在X轴上与喷嘴原点坐标的距离
#define FRAMEY 0         //限位坐标轴原点在Y轴上与喷嘴原点坐标的距离
#define ANGLELIMIT 5*3.1415926/180 //带喷涂区域坐标系与喷涂机器坐标系夹角极限
#define FRAMEXLIMIT 150  //带喷涂区域左下角原点与喷涂机器左下角原点在X轴上的极限
#define FRAMEYLIMIT 150   //带喷涂区域左下角原点与喷涂机器左下角原点在Y轴上的极限

//对于多喷枪来说，需要记录第一把喷枪和最后一把喷枪的安装位置
#define FIRSTGUNPOINTY 90
#define FINALLYGUNPOINTY -90
#endif
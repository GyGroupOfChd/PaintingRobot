#include "StdAfx.h"
#include "Parm.h"

COLORREF gNULLCOLOR=RGB(255,255,255);///<无色对应的RGB，不需喷的颜色
long gRowWidth;///<每个喷涂列的宽度，单位像素必须赋值
long gHeightOfEveryPaintCell;///<每个喷涂单元的高度，单位像素，必须赋值，在实际识别操作时，没有按这个走，而是按实际高度来的，转用于预先缩小喷涂区域的尺寸使其边界不至于穿过喷涂单元
double gMMPerPixel=MM_PER_PIXEL;///<每一像素对应为几个mm
bool gbOnePixelBetweenWorkReg=false;///<在工作区之间存在一个像素宽度的间距
enum AreaPartitionFlag geAreaPartitionFlag=GRID_PARTION_BY_INTELLIGENCE;///<标志位，控制是预先按固定工作区间大小分割整个地图为若干个工作区间；还是自动寻找，从第一个需喷涂的方块位开始逐行划分；还是智能化自动划分(目前未做)
enum CellSizePreRecFlag gCellSizePreRecFlag;///<标志位，控制对单元的参数预识别时，进行复杂的平均计算，还是简单的直接取最小值。

PaintAreaTemplate g_sPaintAreaParm;///<全局变量，定义整个工作区域的参数
//const 
CRect_Work_Region g_cWorkRegionParm;///<定义工作区域的参数
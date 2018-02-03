#include "stdafx.h"
//喷枪最大喷涂范围为1250*1900
//每个相机相关系数
//注意，相机像素的坐标与其他坐标不一致
//第一个相机的坐标要求不一致
#define FIRSTCAMERAXAXISWIDTH 1288
#define FIRSTCAMERAYAXISHEIGHT 964
#define SECONDCAMERAXAXISWIDTH 1288
#define SECONDCAMERAYAXISHEIGHT 964
#define FIRSTEVERYPIXELTOEVERYCENTIMETREX 0.411490683   //第一个相机在X轴上一个像素等于多少个毫米
#define FIRSTEVERYPIXELTOEVERYCENTIMETREY 0.399377593   //第一个相机在Y轴上一个像素等于多少个毫米
#define SECONDEVERYPIXELTOEVERYCENTIMETREX 0.489130435   //第二个相机在X轴上一个像素等于多少个毫米
#define SECONDEVERYPIXELTOEVERYCENTIMETREY 0.487551867   //第二个相机在Y轴上一个像素等于多少个毫米
#define DISTANCEBETWEENCAMERA 170 //两个相机在X轴之间像素中心安装距离
//对于第一个相机来说，在整个系统中占据非常重要的作用，首先需要确定在当前位置，喷枪的启动位置；
//一下数据FIRSTCAMERAXORIGINPOINT为在当前坐标系中第一个相机的安装位置，X坐标，FIRSTCAMERAYORIGINPOINT为Y坐标，要保证相机安装精度与当前坐标系平行

#define FIRSTCAMERAXORIGINPOINT 340
#define FIRSTCAMERAYORIGINPOINT 1230            //单位毫米，坐标相对于限位
//第二个相机在当前坐标系中的位置
#define SECONDCAMERAXORIGINPOINT 1845               //单位毫米，坐标相对于限位
#define SECONDCAMERAYORIGINPOINT 1250
//在喷涂过程中，实际要将喷头中心对准带喷涂的第一个单元，所以限位坐标与喷枪坐标坐标有差别
#define FRAMEX 160
#define FRAMEY 10
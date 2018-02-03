#pragma once
/**
*部分额外的数学运算，目前包括四舍五入运算
*/
#define ONEHALF 0.5
static long Floor4_Ceil5(float NumTobeOperate)
{
	return long(NumTobeOperate+0.5);
}
static int Floor4_Ceil5Float(int Business,int Denominator)              //四舍五入，大于0.5返回1，小于0.5，返回0
{
	float Result=(float)Business/Denominator;
	if (Result>=ONEHALF)
	{
		return 1;
	}
	else
		return 0;
}
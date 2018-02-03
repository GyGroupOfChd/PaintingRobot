#include "StdAfx.h"
#include "ResultAdapter.h"
CColorResAdapter::CColorResAdapter( )
{
	m_MaxColorNum=MAX_COLOR_MACHINE_INDEX-1;//×î´ó±àºÅ

	for (unsigned i=0;i<=MAX_COLOR_MACHINE_INDEX;i++)
	{
		pushback(i);
		pushback_HexMap(i);		
	}
}
CColorResAdapter::~CColorResAdapter()
{

}


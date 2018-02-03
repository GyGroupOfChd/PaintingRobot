#include "stdafx.h"
#include "RoadRecOnHalconWithPtgrey.h"


CRoadRecOnHalconWithPtgrey::CRoadRecOnHalconWithPtgrey(void)
{	
}
//
//
CRoadRecOnHalconWithPtgrey::~CRoadRecOnHalconWithPtgrey(void)
{
}
void CRoadRecOnHalconWithPtgrey::Init(Hlong pParent,CRect RecOfWindowDispLay)
{
	m_HalconRoadRec.InitHalconDisp(pParent,RecOfWindowDispLay);
}
void CRoadRecOnHalconWithPtgrey::InitTwo(Hlong pParent,CRect RecOfWindowDispLay)
{
	m_HalconRoadRec.InitHalconDispTwo(pParent,RecOfWindowDispLay);
}
bool CRoadRecOnHalconWithPtgrey::CreateGrabThread()
{	
	return true;
}

UINT CRoadRecOnHalconWithPtgrey::GrabThread(LPVOID lpParam)
{ 
	

	return 0;
}
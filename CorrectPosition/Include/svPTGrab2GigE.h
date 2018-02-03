// svPTGrab2GigE.h: interface for the svPTGrab2GigE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVPTGRAB2GIGE_H__A1619F03_6369_4B98_8ABD_B5DC286C9928__INCLUDED_)
#define AFX_SVPTGRAB2GIGE_H__A1619F03_6369_4B98_8ABD_B5DC286C9928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "svPTGrab2.h"
#pragma pack(push,8)

// #ifdef SV_PTGRAB2_EXPORTS
// #define SV_PTGRAB2_API _declspec(dllexport)
// #else
// #define SV_PTGRAB2_API _declspec(dllimport)
// #endif

class /*SV_PTGRAB2_API*/ scPTGrab2GigE : public scPTGrab2 
{
public:
	scPTGrab2GigE();
	virtual ~scPTGrab2GigE();

};
#pragma pack(pop)

#endif // !defined(AFX_SVPTGRAB2GIGE_H__A1619F03_6369_4B98_8ABD_B5DC286C9928__INCLUDED_)

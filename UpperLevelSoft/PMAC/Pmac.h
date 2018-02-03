// Pmac.h: interface for the CPmac class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMAC_H__0AD7D9EB_90CD_4966_B3D0_CECDC0A1A7BA__INCLUDED_)
#define AFX_PMAC_H__0AD7D9EB_90CD_4966_B3D0_CECDC0A1A7BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPmac  
{
public:
	double pmacDPRPosition(int i,double units);
	double pmacDPRGetVel(int i,double units);
	SHORT pmacDPRAsciiStrToRotA(PCHAR inpstr, USHORT bufnum);
	void pmacDPRRotBufRemove(int bufnum);
	int pmacDPRBufLast();
	void pmacDPRRotBufChange(int new_size);
	DWORD pmacGetDWord(UINT offset);
	void pmacSetVariableDouble(char cVar,int nNum,double dValue);
	double pmacGetVariableDouble(char cVar,int nNum,double dDefault);
	void pmacSetVariable(char cVar,int nNum,int nValue);
	int pmacGetVariable(char cVar,int nNum,int nDefault);
	void pmacSendCommand(CString cmd);
	CString pmacGetResponse(CString cmd);
	CPmac();
	virtual ~CPmac();

};

#endif // !defined(AFX_PMAC_H__0AD7D9EB_90CD_4966_B3D0_CECDC0A1A7BA__INCLUDED_)

// Pmac.cpp: implementation of the CPmac class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "fenc.h"
#include "Pmac.h"
#include "Runtime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPmac::CPmac()
{

}

CPmac::~CPmac()
{

}
//-----------------pmac-true-----------------------------
/*
CString CPmac::pmacGetResponse(CString cmd)
{
	char buf[256];
	DeviceGetResponse(0,buf,255,(char*)(LPCTSTR)cmd);
	return buf;
}

void CPmac::pmacSendCommand(CString cmd)
{
	DeviceSendCommand(0,(char*)(LPCTSTR)cmd);
}

void CPmac::pmacSetVariable(char cVar, int nNum, int nValue)
{
	DeviceSetVariable(0,cVar,nNum,nValue);
}

int CPmac::pmacGetVariable(char cVar, int nNum, int nDefault)
{
	return DeviceGetVariable(0,cVar,nNum,nDefault);
}

void CPmac::pmacSetVariableDouble(char cVar, int nNum, double dValue)
{
	DeviceSetVariableDouble(0,cVar,nNum,dValue);
}

double CPmac::pmacGetVariableDouble(char cVar, int nNum, double dDefault)
{
	return DeviceGetVariableDouble(0,cVar,nNum,dDefault);
}

DWORD CPmac::pmacGetDWord(UINT offset)
{
	return DeviceDPRGetDWord(0,offset);
}

void CPmac::pmacDPRRotBufChange(int new_size)
{
	DeviceDPRRotBufChange(0,0,new_size);
}

int CPmac::pmacDPRBufLast()
{
	return DeviceDPRBufLast(0);
}

void CPmac::pmacDPRRotBufRemove(int bufnum)
{
	DeviceDPRRotBufRemove(0,bufnum);
}

SHORT CPmac::pmacDPRAsciiStrToRotA(PCHAR inpstr, USHORT bufnum)
{
	return DeviceDPRAsciiStrToRot(0, inpstr,bufnum);
}

double CPmac::pmacDPRPosition(int i, double units)
{
	return DeviceDPRPosition(0,i,units);
}

double CPmac::pmacDPRGetVel(int i, double units)
{
	return DeviceDPRGetVel(0,i,units);
}
*/
//--------------------------------------------------

//------------------pmac-false--------------------------

CString CPmac::pmacGetResponse(CString cmd)
{
	char buf[256];
	strcpy(buf,"error");
	return (CString)buf;
}

void CPmac::pmacSendCommand(CString cmd)
{
}

void CPmac::pmacSetVariable(char cVar, int nNum, int nValue)
{
}

int CPmac::pmacGetVariable(char cVar, int nNum, int nDefault)
{
	return 0;
}

void CPmac::pmacSetVariableDouble(char cVar, int nNum, double dValue)
{
}

double CPmac::pmacGetVariableDouble(char cVar, int nNum, double dDefault)
{
	return 0.0;
}

DWORD CPmac::pmacGetDWord(UINT offset)
{
	return 0;
}

void CPmac::pmacDPRRotBufChange(int new_size)
{
}

int CPmac::pmacDPRBufLast()
{
	return 0;
}

void CPmac::pmacDPRRotBufRemove(int bufnum)
{
}

SHORT CPmac::pmacDPRAsciiStrToRotA(PCHAR inpstr, USHORT bufnum)
{
	return 0;
}

double CPmac::pmacDPRPosition(int i, double units)
{
	return 0.0;
}

double CPmac::pmacDPRGetVel(int i, double units)
{
	return 0.0;
}

//---------------------------------------------------

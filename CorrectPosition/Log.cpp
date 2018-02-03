//*******************************************************************
//              
//  FILE:       Log.cpp
//              
//  AUTHOR:     
//              
//  PROJECT:    SysLog
//              
//  COMPONENT:  
//              
//  DATE:       26.02.2004
//              
//  COMMENTS:   -
//              
//              
//*******************************************************************

/*! \addtogroup Utility
 * @{
 */

/*! \file
 *  \brief 日志类的实现文件
 *
 */

/*! 
 * @}
 */

#include "stdafx.h"
#include "Log.h"
#include <string.h>
//在调用程序所在目录下生成Log文件夹，
CLogFile::CLogFile()
{

}
CLogFile::~CLogFile()
{

}
//在该文件夹下每天生成一个日志文件(如2004-07-03.log)
void CLogFile::WriteLogFile(CString &lpText)
{
	CStdioFile	fl;
	CString		strText;
	CString     strFilePath;
	CString		strFileName;
	CString     strDir;
	
	strText.Format(_T("%s"), lpText);
	if( strText.IsEmpty() )
	{
		return;
	}
	
	//日志存放路径
	strDir = GetDirOfExe();
	if( !IsDirExist(strDir + _T("Log")))
	{
		::CreateDirectory( strDir + "Log", NULL);
	}
	CTime t = CTime::GetCurrentTime();
	strFileName = t.Format("%Y-%m-%d.log");
	strFilePath = strDir + "Log\\" + strFileName;

	//如果文件大于1MB, 删!
	ClearLog(strFilePath);
	
	//写日志
	WriteLog(strText,strFilePath);	 
}

//获取主程序所在文件夹路径,以"\\"结尾
CString CLogFile::GetDirOfExe()
{	
	CString szDir;
	TCHAR	FilePath[MAX_PATH];
	TCHAR	*pdest = NULL;
	int		result;
	
	GetModuleFileName(NULL,FilePath,MAX_PATH);
	pdest = strrchr(FilePath,'\\');
	result = pdest-FilePath+1;	
	szDir.Format(_T("%s"),FilePath);
	szDir = szDir.Left(result);
    szDir = szDir + _T("WorkProject\\");
	return szDir;
}

//判断文件夹是否存在
BOOL CLogFile::IsDirExist(const CString &strDirPath)
{
	BOOL bRet;
	char szCurrentDiretory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szCurrentDiretory);	
	bRet = SetCurrentDirectory(strDirPath);
	SetCurrentDirectory(szCurrentDiretory);
	return bRet;
}

//如果文件大于1MB, 删!
void CLogFile::ClearLog(const CString& strFilePath)
{
	if( IsFileExist( strFilePath ) )
	{
		CFileStatus fs;
		CFile::GetStatus( strFilePath, fs);
		if( fs.m_size > 1048576 )
		{
			::DeleteFile( strFilePath );
		}
	}
}

//判断文件是否存在
BOOL CLogFile::IsFileExist(const CString &strFilePath)
{
	BOOL bFileFound = FALSE;
	WIN32_FIND_DATA FileInfo;
	HANDLE hFind;
	hFind = ::FindFirstFile(strFilePath, &FileInfo) ;
	if (hFind == INVALID_HANDLE_VALUE )
		bFileFound = FALSE;
	else
		bFileFound = TRUE;
	::FindClose (hFind);	
	return bFileFound;	
}

//写日志
void CLogFile::WriteLog(CString& strText,const CString& strFilePath)
{
	CStdioFile fl;
	if(!fl.Open(strFilePath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText))
	{
		return ;
	}
	try
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString t;
		t.Format(_T("[%d_%d_%d_%d_%d_%d]"),sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		//CTime t = CTime::GetCurrentTime();
		strText = t + strText;
		fl.SeekToEnd();
		fl.WriteString(strText);
		fl.WriteString(_T("\n"));
		fl.Close();
	}
	catch(CFileException *e)
	{
		e->Delete();
		return ;
	}
}


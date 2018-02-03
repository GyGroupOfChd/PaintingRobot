

#ifndef   _HHES_LOG_H_
#define   _HHES_LOG_H_

#include <afx.h>

/*! \brief 日志类
 *
 * 在调用程序所在目录下生成Log文件夹,在该文件夹下每天生成一个日志文件(如2004-07-03.log)
 * 如果日志文件已经大于1MB，则清空后再写入。
 */
class CLogFile
{
public:
	/*! \brief 构造函数
     *  
	 * 声明为protected，防止生成该类的实例
	 */
	CLogFile();

	/*! \brief 析构函数
     *  
	 */
	~CLogFile();

	/*! \brief 写日志文件
     *  
	 *  \param lpText 日志信息
	 */
	static void WriteLogFile(CString &lpText);	

protected:


	/*! \brief 获取主程序所在文件夹路径,以"\\"结尾
     *  
	 */
	static CString GetDirOfExe();

	/*! \brief 判断文件夹是否存在
     *  
	 */
	static BOOL IsDirExist(const CString &strDirPath);	

	/*! \brief 判断文件是否存在
     *  
	 */
	static BOOL IsFileExist(const CString &strFilePath);

	/*! \brief 如果文件大于1MB, 删!
     *  
	 */
	static void ClearLog(const CString& strFilePath);

	/*! \brief 写日志
     *  
	 */
	static void WriteLog(CString& strText, const CString& strFilePath);
	
};

#endif

/*! @} */
/************************************************************************/
/* 枚举文件                                                                              */
/************************************************************************/

#pragma once

#include <atlstr.h>

class CEnumFile
{
public:

	// 文件信息的封装类
	class FileInfo  
	{
	public:
		FileInfo(){}
		~FileInfo(){}

		//文件名
		CString m_strFileName;
		//文件大小
		DWORD	m_nSize;

		operator CString&()
		{
			return m_strFileName;
		}

		CString& GetStr()
		{
			return m_strFileName;
		}
	};

	// 通过给构造函数传参数，直接就开始枚举
	// 参数分别指定：要枚举的根目录；要枚举的文件类型
	CEnumFile(LPCTSTR lpDirName, LPCTSTR lpPost=_T("*.*"))
	{
		CString		strSearch;

		strSearch=lpDirName;

		// 路径后面加'\\'
		if (strSearch.Right(1)!=_T("\\"))
			strSearch.Append(_T("\\"));
		strSearch.Append(lpPost);
		m_strDir = lpDirName;

        WIN32_FIND_DATA	wfd = {0};
		HANDLE			hFind = ::FindFirstFile(strSearch,&wfd);

		if (hFind!=INVALID_HANDLE_VALUE)
		{
			do 
			{
				FileInfo	fi;
				CString		strFileName = wfd.cFileName;

				if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					// 处理目录
					// 如果是.和..，即返回上级目录，则不处理
					if (strFileName.CompareNoCase(_T("."))==0||
						strFileName.CompareNoCase(_T(".."))==0)
						continue;

					m_listDirs.Add(strFileName);
				}
				else
				{
					// 处理文件
                    fi.m_strFileName = strFileName;
                    fi.m_nSize		 = wfd.nFileSizeLow;	
					m_listFiles.Add(fi);
				}
				
			}while (::FindNextFile(hFind,&wfd));

            FindClose( hFind );
		} 
	}

	// 文件总数
	int GetFileCount()
	{
		return m_listFiles.GetSize();
	}

	// 目录总数
	int GetDirCount()
	{
		return m_listDirs.GetSize();
	}

	// 文件名
	CString& GetFileName(int i)
	{
		return m_listFiles[i].GetStr();
	}

	// 文件大小
	DWORD GetFilesize(int i)
	{
		return m_listFiles[i].m_nSize;
	}

	// 子目录路径
	CString& GetDirName(int i)
	{
		return m_listDirs[i];
	}

	// 文件路径名
	CString	GetFileFullPath(int i)
	{
		CString	strFull = m_strDir;
		strFull.Append(_T("\\"));
		strFull.Append(m_listFiles[i].GetStr());
		return strFull;
	}

	// 子目录完整路径
	CString GetDirFullPath(int i)
	{
		CString	strFull = m_strDir;
		strFull.Append(_T("\\"));
		strFull.Append(m_listDirs[i]);
		return strFull;
	}

protected:
	CSimpleArray<FileInfo>	m_listFiles;
	CSimpleArray<CString>	m_listDirs;
	CString					m_strDir;
};
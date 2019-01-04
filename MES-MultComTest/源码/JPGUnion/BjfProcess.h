// BjfProcess.h: interface for the CBjfProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BJFPROCESS_H__F82BF66E_9A88_489F_8981_2FF887C21F9C__INCLUDED_)
#define AFX_BJFPROCESS_H__F82BF66E_9A88_489F_8981_2FF887C21F9C__INCLUDED_
#define MAX_PATHC 255
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef struct tag_FILE_HEADER{
	char szType[42];
	char Redtime[8];
	int nFileCount;
}FILE_HEADER,*LPFILE_HEADER;
typedef struct tag_FILE_TAIL{
	char szType[42];
	char Redtime[40];
	int nFileCount;
}FILE_TAIL,*LPFILE_TAIL;

typedef struct tag_FILE_BLOOK{
	char szFileName[MAX_PATHC];
	DWORD dwFileLenth;
	FILE* pInStream;
}FILE_BLOOK,*LPFILE_BLOOK;
//--FILE_BLOOK的同地址内容，为容易定位而写
struct ByteLine
{
	BYTE strTem[260];
	DWORD dwFileLen;
	FILE* pInStream;
};
//--关联FILE_BLOOK与ByteLine的地址
typedef union tag_UNFILEBLOCK
{
	FILE_BLOOK FILE_BLOOKTem;

	ByteLine ByteLineTem;
}UNFILEBLOCK;
class CBjfVSJpg  
{
public:
	FILE_HEADER m_FILE_HEADERTem;
	FILE_TAIL m_FILE_TAILTem;
public:
	int GetClassFileCount(CString sSourceFileName);
	int GetClassFileCount(void);
	void SetClassFileCount(int nFileCount);
	CString GetClassRedtime(CString sSourceFileName);
	CString GetClassRedtime(void);
	CString GetClassIllegaltime(void);
	void SetClassRedtime(CString csRedtime);
	void SetClassIllegaltime(CString csRedtime);
	CString GetClassType();
	CString GetClassType(CString sSourceFileName);

	void SetClassType(CString );
	CString * GetEveryFile(CString sSourceFileName);
	CString * GetEveryFileGetPath(CString sSourceFileName);
	int GetJpgFileCount(CString sSourceFileName);
	CString GetOneFile(CString sSourceFileName, int nNum);
	CString GetOneFileForPath(CString sSourceFileName, int nNum);

	BOOL UnionAllFile(CString sDestFileName, CString * sSourceFileName);
	
	CBjfVSJpg();
	virtual ~CBjfVSJpg();

};

#endif // !defined(AFX_BJFPROCESS_H__F82BF66E_9A88_489F_8981_2FF887C21F9C__INCLUDED_)

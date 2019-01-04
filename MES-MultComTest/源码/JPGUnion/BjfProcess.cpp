// BjfProcess.cpp: implementation of the CBjfProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BjfProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
//#define MAX_PATHC 256
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBjfVSJpg::CBjfVSJpg()
{
	//memset(m_FILE_HEADERTem.Redtime,0,strlen(m_FILE_HEADERTem.Redtime));
	//memset(m_FILE_HEADERTem.szType,0,strlen(m_FILE_HEADERTem.szType));

	memset(m_FILE_HEADERTem.Redtime,0,8);
	memset(m_FILE_HEADERTem.szType,0,42);
}

CBjfVSJpg::~CBjfVSJpg()
{

}
//============================================================
//函数名称：UnionAllFile
//函数返回：BOOL
//函数参数：sDestFileName为要保存的bjf文件，
//          sSourceFileName为多个jpg的文件全称数组，
//          nFileCount为jpg文件的张数
//函数功能：合成多张JPEG图象，默认合成后的图片存放在D：\\临时图片
//备注    ：如果要合成的文件大小不一致，保存后的dwFileLenth
//          值不是实际文件的的大小。
//============================================================
BOOL CBjfVSJpg::UnionAllFile(CString sDestFileName, CString *sSourceFileName)
{

	UNFILEBLOCK UNFileBlockTem;
	int PhotoPoint = m_FILE_HEADERTem.nFileCount;
	char BjfName[256];
	char JpgName[256];

	void *FileBuffer;
	DWORD FileLength;
	strcpy(BjfName,sDestFileName);

	int i;
	i=0;

	CFile fp,fp1;
	//如果打开失败，返回0.
	if( !fp.Open(BjfName ,CFile::modeCreate | CFile::modeWrite,NULL) )
		return FALSE;

	FileLength = 0;
    fp.Write(&m_FILE_HEADERTem,sizeof(FILE_HEADER));	
	//bjf文件头的长度
	DWORD dBjfLength;
		
    dBjfLength=sizeof(FILE_HEADER);
	//循环打开指定个数的图片文件并读取内容，写入要合成的bjf文件。
	for(i=0;i<PhotoPoint;i++)
    {
		strcpy(JpgName,"");
		strcpy(JpgName,sSourceFileName[i]);
		if( !fp1.Open(JpgName,CFile::modeRead,NULL)  )
		{
			//continue;
			return FALSE;
		}
		FileLength = fp1.GetLength(); 
		FileBuffer = NULL;
		FileBuffer = malloc( FileLength );
		if( FileBuffer!=NULL)
		{
			FILE_BLOOK FILE_BLOOKTem;
			//表头初始化
			//JpgName文件名（不包括路径）要保存到结构里
			CString sTempFileName = "";
			int nSitu = 0;
			sSourceFileName[i].Format("%s",sSourceFileName[i]);	//需要先用format转一下，否则mid函数取的文件名丢内容
			nSitu = sSourceFileName[i].ReverseFind('\\');
			sTempFileName = sSourceFileName[i].Mid(nSitu+1);
			//--路径处理
			//sTempFileName="C:\\Program Files\\电子警察系统\\pic_zc\\"+sTempFileName;
			CString sFilePath="";
			sDestFileName.Format("%s",sDestFileName);
			nSitu = sDestFileName.ReverseFind('\\');
			sFilePath = sDestFileName.Mid(0,nSitu+1);
			sTempFileName=sFilePath+sTempFileName;

			strcpy(FILE_BLOOKTem.szFileName,sTempFileName);
            FILE_BLOOKTem.dwFileLenth=FileLength;
			//写入文件BLOOK块
			//fp.Write(&FILE_BLOOKTem,sizeof(FILE_BLOOK)); 
			UNFileBlockTem.FILE_BLOOKTem=FILE_BLOOKTem;
			UNFileBlockTem.ByteLineTem.dwFileLen=FileLength;
            //fp.Write(&UNFileBlockTem,sizeof(UNFILEBLOCK)); 
			fp.Write(&UNFileBlockTem,sizeof(FILE_BLOOK)); 
			//写文件内容
			fp.SeekToEnd(); 
			//读出文件内容后写入指定文件。
			fp1.Read(FileBuffer, FileLength); 
			fp.Write(FileBuffer,FileLength); 	
			free(FileBuffer);
		}
		fp1.Close(); 
	}
	fp.Write(&m_FILE_TAILTem,sizeof(FILE_TAIL));		
	fp.Close(); 

	return TRUE;
}

//============================================================
//函数名称：GetOneFile
//函数返回：CString，返回值为jpg文件全称
//函数参数：sSourceFileName为bjf文件名，
//          nNum为要取哪张jpg图片。
//函数功能：分解获取一张JPEG图象，默认分解后的图片存放在和Bjf同一目录下
//          如果要修改存放目录，则需要修改函数内容。
//备注    ：
//============================================================
CString CBjfVSJpg::GetOneFile(CString sSourceFileName, int nNum)
{
	int PhotoPoint = 0;

	//要取出的jpg是bjf里的第几张,0表示第1张,1表示第2张，2表示第3张，3表示第4张
	int n=nNum;

	char JpgName[256],BjfName[256];
	strcpy(BjfName,sSourceFileName);

	void *FileBuffer;
	DWORD FileLength,TmpLen;

	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//读FILE_HANDEAR
    ////FILE_HEADER FILE_HEADERTem;
	DWORD len=fp.GetLength()-sizeof(FILE_TAIL);
	fp.Seek(len,CFile::begin);
	fp.Read(&m_FILE_TAILTem, sizeof(FILE_TAIL));
	fp.SeekToBegin();
	fp.Read(&m_FILE_HEADERTem, sizeof(FILE_HEADER));

	//读FILE_BLOOK
	FILE_BLOOK FILE_BLOOKTem;
    //fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));

	//bjf文件头的长度------要读的内容到文件开始位置的长度
	DWORD dBjfLength;

    dBjfLength=sizeof(FILE_HEADER);

	/////////////
	FileLength = dBjfLength+sizeof(FILE_BLOOK);
	PhotoPoint=m_FILE_HEADERTem.nFileCount;
	if(n>PhotoPoint)
		return "";
	///////////

	for(int i0=0;i0<PhotoPoint;i0++)
	{
		BYTE *tem;
		tem=new BYTE[500];
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(tem, 500);
		unsigned long length;
		//for(int i1=0;i1<500;i1++)
		int i1=260;
		{
			//if(tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256>20*1024&&
			//   tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256<45*1024)
			{
				// ::AfxMessageBox("find lenth memory");
				length=tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256;
				//break;
			}
		}
		delete tem;
		//移动文件指针。
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));
        TmpLen=FILE_BLOOKTem.dwFileLenth;
		TmpLen=length;
		FILE_BLOOKTem.dwFileLenth=length;
		//判断是否是要打开的源文件。

		if(n==i0)
		{
			//是要打开的文件源文件
			fp.Seek(FileLength,CFile::begin);
		    FileBuffer = NULL;
			//分配文件BLOOK大小的内存。
		    FileBuffer = malloc( TmpLen );
			CString csFilename;
			csFilename=FILE_BLOOKTem.szFileName;
			int len=csFilename.ReverseFind('\\');
			csFilename=csFilename.Mid(len+1);
			strcpy(JpgName,csFilename);

			//文件名再加上和Bjf同目录的路径
			int nSitu = 0;
			CString sTempFilePath = "";
			CString sFileName = "";
			sSourceFileName.Format("%s",sSourceFileName);	//需要先用format转一下，否则mid函数取的文件名丢内容
			nSitu = sSourceFileName.ReverseFind('\\');
			sTempFilePath = sSourceFileName.Mid(0,nSitu);	//得到路径
			//--去掉文件名中的路径

			sFileName.Format("%s",JpgName);
			//--处理jpg文件名。
			CString szOrder;
			szOrder.Format("%d.jpg",nNum);
			//sFileName=sFileName+szOrder;
			//sFileName.Replace(".jpg",szOrder);
			sFileName = sTempFilePath + "\\" + sFileName;
			strcpy(JpgName,"");
			strcpy(JpgName,sFileName);

			if(FileBuffer!=NULL)
			{
				fp.Read(FileBuffer,TmpLen);
				//读取文件
				fp.Close();
				if( fp.Open(JpgName,CFile::modeCreate | CFile::modeWrite , NULL) )
				{
					//将读到的内容写入文件
					fp.Write(FileBuffer,TmpLen);
					fp.Close();
				}
				free(FileBuffer);
				CString filepath;
				filepath.Format(JpgName);
				//返回文件路径+文件名。
				return filepath;
			}
			else
			{
		    	fp.Close(); 
				return "";
			}
	    
		}
        //不是要打开的源文件，算出要移动文件指针的长度。
		FileLength+=FILE_BLOOKTem.dwFileLenth;
		FileLength+=sizeof(FILE_BLOOK);
	}
	return "";
}
//============================================================
//函数名称：GetOneFileForPath
//函数返回：CString，返回值为jpg文件全称
//函数参数：sSourceFileName为bjf文件名，
//          nNum为要取哪张jpg图片。
//函数功能：分解获取一张JPEG图象，默认分解后的图片存放在和Bjf同一目录下
//          如果要修改存放目录，则需要修改函数内容。
//备注    ：此函数为兼容九岁的图片浏览程序，一般不调用。
//============================================================
CString CBjfVSJpg::GetOneFileForPath(CString sSourceFileName, int nNum)
{
	int PhotoPoint = 0;

	//要取出的jpg是bjf里的第几张,0表示第1张,1表示第2张，2表示第3张，3表示第4张
	int n=nNum;

	char JpgName[256],BjfName[256];
	strcpy(BjfName,sSourceFileName);

	void *FileBuffer;
	DWORD FileLength,TmpLen;

	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//读FILE_HANDEAR
    ////FILE_HEADER FILE_HEADERTem;
	DWORD len=fp.GetLength()-sizeof(FILE_TAIL);
	fp.Seek(len,CFile::begin);
	fp.Read(&m_FILE_TAILTem, sizeof(FILE_TAIL));
	fp.SeekToBegin();
	fp.Read(&m_FILE_HEADERTem, sizeof(FILE_HEADER));

	//读FILE_BLOOK
	FILE_BLOOK FILE_BLOOKTem;
    //fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));

	//bjf文件头的长度------要读的内容到文件开始位置的长度
	DWORD dBjfLength;

    dBjfLength=sizeof(FILE_HEADER);

	/////////////
	FileLength = dBjfLength+sizeof(FILE_BLOOK);
	PhotoPoint=m_FILE_HEADERTem.nFileCount;
	if(n>PhotoPoint)
		return "";
	///////////

	for(int i0=0;i0<PhotoPoint;i0++)
	{
		BYTE *tem;
		tem=new BYTE[500];
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(tem, 500);
		unsigned long length;
		//for(int i1=0;i1<500;i1++)
		int i1=260;
		{
			//if(tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256>20*1024&&
			//   tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256<45*1024)
			{
				// ::AfxMessageBox("find lenth memory");
				length=tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256;
				//break;
			}
		}
		delete tem;
		//移动文件指针。
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));
        TmpLen=FILE_BLOOKTem.dwFileLenth;
		TmpLen=length;
		FILE_BLOOKTem.dwFileLenth=length;
		//判断是否是要打开的源文件。

		if(n==i0)
		{
			//是要打开的文件源文件
			fp.Seek(FileLength,CFile::begin);
		    FileBuffer = NULL;
			//分配文件BLOOK大小的内存。
		    FileBuffer = malloc( TmpLen );
			strcpy(JpgName,FILE_BLOOKTem.szFileName);
			//文件名再加上和Bjf同目录的路径
			int nSitu = 0;
			CString sTempFilePath = "";
			CString sFileName = "";
			sSourceFileName.Format("%s",sSourceFileName);	//需要先用format转一下，否则mid函数取的文件名丢内容
			nSitu = sSourceFileName.ReverseFind('\\');
			sTempFilePath = sSourceFileName.Mid(0,nSitu);	//得到路径
			sFileName.Format("%s",JpgName);
			//--处理jpg文件名。
			CString szOrder;
			szOrder.Format("%d.jpg",nNum);
			//sFileName=sFileName+szOrder;
			//sFileName.Replace(".jpg",szOrder);
			//sFileName = sTempFilePath + "\\" + sFileName;
			strcpy(JpgName,"");
			strcpy(JpgName,sFileName);

			if(FileBuffer!=NULL)
			{
				fp.Read(FileBuffer,TmpLen);
				//读取文件
				fp.Close();
				if( fp.Open(JpgName,CFile::modeCreate | CFile::modeWrite , NULL) )
				{
					//将读到的内容写入文件
					fp.Write(FileBuffer,TmpLen);
					fp.Close();
				}
				free(FileBuffer);
				CString filepath;
				filepath.Format(JpgName);
				//返回文件路径+文件名。
				return filepath;
			}
			else
			{
		    	fp.Close(); 
				return "";
			}
	    
		}
        //不是要打开的源文件，算出要移动文件指针的长度。
		FileLength+=FILE_BLOOKTem.dwFileLenth;
		FileLength+=sizeof(FILE_BLOOK);
	}
	return "";
}

//============================================================
//函数名称：GetJpgFileCount
//============================================================
int CBjfVSJpg::GetJpgFileCount(CString sSourceFileName)
{

	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return 0;


	//读FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
	fp.Close();
    return FILE_HEADERTem.nFileCount;
}
//============================================================
//函数名称：GetEveryFile
//函数返回：CString指针。
//函数参数：sSourceFileName为bjf文件名.
//         
//函数功能：根据bjf文件件中包含的jpg文件数量获取每个文件并保存到指定目录下
//          默认为d:\\临时图片。
//备注    ：
//============================================================
CString * CBjfVSJpg::GetEveryFile(CString sSourceFileName)
{
	
	int filecount;
	CString *csSeparate;
    csSeparate=new CString[4];
	filecount=GetJpgFileCount(sSourceFileName);
	for(int i=0;i<filecount;i++)
		csSeparate[i]=GetOneFile(sSourceFileName, i);

	return csSeparate;

}
//============================================================
//函数名称：GetEveryFile
//函数返回：CString指针。
//函数参数：sSourceFileName为bjf文件名.
//         
//函数功能：根据bjf文件件中包含的jpg文件数量获取每个文件并保存到指定目录下
//          默认为d:\\临时图片。
//备注    ：
//============================================================
CString * CBjfVSJpg::GetEveryFileGetPath(CString sSourceFileName)
{
	
	int filecount;
	CString *csSeparate;
    csSeparate=new CString[4];
	filecount=GetJpgFileCount(sSourceFileName);
	for(int i=0;i<filecount;i++)
		csSeparate[i]=GetOneFileForPath(sSourceFileName, i);

	return csSeparate;

}
//============================================================
//函数名称：ClassSet
//函数返回：无
//函数参数：FILE_HEADERTem类变量初始化.
//         
//函数功能：初始化文件头
//备注    ：
//============================================================
void CBjfVSJpg::SetClassType(CString csType)
{
	strcpy(m_FILE_HEADERTem.szType,csType);
}

CString CBjfVSJpg::GetClassType()
{
	CString csType;
	csType.Format(m_FILE_HEADERTem.szType);
	return csType;
}
CString CBjfVSJpg::GetClassType(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";


	//读FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileType;
    fileType.Format(FILE_HEADERTem.szType);
	fp.Close();
	return fileType;

}
//设置结构中的违法时间
void CBjfVSJpg::SetClassRedtime(CString csRedtime)
{
	CString szTem=csRedtime.Mid(0,8);
	strcpy(m_FILE_HEADERTem.Redtime,szTem);
	
}
void CBjfVSJpg::SetClassIllegaltime(CString csRedtime)
{
	CString szTem=csRedtime.Mid(0,40);
	strcpy(m_FILE_TAILTem.Redtime,szTem);
	
}
//得到结构中的违法时间
CString CBjfVSJpg::GetClassRedtime()
{
	CString csRedtime;
	csRedtime.Format(m_FILE_HEADERTem.Redtime);
	return csRedtime;
}
CString CBjfVSJpg::GetClassIllegaltime()
{
	CString csRedtime;
	csRedtime.Format(m_FILE_TAILTem.Redtime);
	return csRedtime;
}
//
CString CBjfVSJpg::GetClassRedtime(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//读FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileRedtime;
    fileRedtime.Format(FILE_HEADERTem.Redtime);
	fp.Close();
	return fileRedtime;
}

void CBjfVSJpg::SetClassFileCount(int nFileCount)
{
	m_FILE_HEADERTem.nFileCount=nFileCount;
}

int CBjfVSJpg::GetClassFileCount()
{
	return m_FILE_HEADERTem.nFileCount;
}

int CBjfVSJpg::GetClassFileCount(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return 0;

	//读FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileRedtime;
    fileRedtime.Format(FILE_HEADERTem.szType);
	fp.Close();
	return FILE_HEADERTem.nFileCount;
}

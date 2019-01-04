///////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////

//#include <Afx.h>
#include "stdafx.h"
#include <AfxColl.h>


#include "IniFile.H"

CIniFile::CIniFile(CString m_FName)
{
	m_Name = m_FName;
}

CString CIniFile::ReadString(CString m_Sec, CString m_Ident, CString m_Def)
{
	char Buffer[2048];

	GetPrivateProfileString(m_Sec, m_Ident,m_Def, Buffer, sizeof(Buffer), m_Name);
	return Buffer;
}

BOOL CIniFile::WriteString(CString m_Sec, CString m_Ident, CString m_Val)
{
	return WritePrivateProfileString(m_Sec, m_Ident, m_Val, m_Name);
}

BOOL CIniFile::ReadSections(CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(NULL, NULL, NULL, (char*) pvData, 16384, m_Name))
	{
        char *P = (char*) pvData;
        while (*P != 0)
		{
			m_Secs.Add(P);
			P += strlen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

BOOL CIniFile::ReadSection(CString m_Sec, CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(m_Sec, NULL, NULL, (char*) pvData, 16384, m_Name))
	{
        char *P = (char*) pvData;
        while (*P != 0)
		{
			m_Secs.Add(P);
			P += strlen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

CString CIniFile::GetParaName()
{
TCHAR       chSectionNames[2048]={0};       //所有节名组成的字符数组
char * pSectionName; //保存找到的某个节名字符串的首地址
int i;               //i指向数组chSectionNames的某个位置，从0开始，顺序后移
int j=0;             //j用来保存下一个节名字符串的首地址相对于当前i的位置偏移量
int count=0;         //统计节的个数

::GetPrivateProfileSectionNames(chSectionNames,2048,m_Name);  

for(i=0;i<2048;i++,j++)

{
      if(chSectionNames[0]=='\0')
       break;       //如果第一个字符就是0，则说明ini中一个节也没有
      if(chSectionNames[i]=='\0')
      {
       pSectionName=&chSectionNames[i-j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，
                                          //就是一个节名的首地址   
       j=-1;                              //找到一个节名后，j的值要还原，以统计下一个节名地址的偏移量

                                          //赋成-1是因为节名字符串的最后一个字符0是终止符，不能作为节名的一部分

										  //在获取节名的时候可以获取该节中键的值，前提是我们知道该节中有哪些键。
										  //AfxMessageBox(pSectionName);      //把找到的显示出来
       
       if(chSectionNames[i+1]==0)
       {
         break;      //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止
       }
      }  

}

return pSectionName;

}



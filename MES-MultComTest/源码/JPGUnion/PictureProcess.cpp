#include "StdAfx.h"
#include "PictureProcess.h"
#include "BasicUse.h"

CPictureProcess::CPictureProcess(void)
//: m_GdiplusToken(0)
{
	//GDI+图像库的初始化
	//GdiplusStartup(&m_GdiplusToken,&m_GdiplusStartupInput,NULL);
}

CPictureProcess::~CPictureProcess(void)
{
	//GdiplusShutdown(m_GdiplusToken);
}


//图片叠加文字
//Buf为传入的图象byte
void CPictureProcess::AddTextToBmp(CString sFile, CWnd* pWnd, BYTE *Buf, 
								   int Width, int Height,
								   CString sInfo)
{

	if(Buf==NULL)	//若未分配空间大小则返回
		return ;
	
	if(sInfo == "")		//若要叠加的串为""，则不叠加
		return ;

	//???
	int nWidthBytes = Width * 3;
	while( (nWidthBytes & 3)!=0)
		nWidthBytes ++;

	//保留原有画笔
	CClientDC client(pWnd);
	CPen Pen(PS_SOLID,1,RGB(0,255,255));
	CPen * pOldPen=(CPen *)client.SelectObject(&Pen);
	CBrush Brush(RGB(0,255,255));
	CBrush * pOldBrush=(CBrush *)client.SelectObject(&Brush);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&client);

	CBitmap MemB;
	short Col=16,Row=27;	//背景区域大小 一个字节的宽，高
	long length = sInfo.GetLength() ;
	long Len = Col*length;
	if(Len < 130)
		Len =130;
	MemB.CreateCompatibleBitmap(&client,Len,Row);	//创建一个背景区域，以便加字
	CBitmap * pOld=MemDC.SelectObject(&MemB);
	MemDC.PatBlt(0,0,Col*length,Row,WHITENESS);
	MemDC.SetTextColor(RGB(255,0,0));	//文字颜色,红色

	CFont OutFont;
	LOGFONT temlf;
	//temlf.lfHeight=-14; 
	temlf.lfHeight=16;	//字大小
	temlf.lfWidth=0; 
	temlf.lfEscapement=0; 
	temlf.lfOrientation=0; 
	temlf.lfWeight=400; 
	temlf.lfItalic=0; 
	temlf.lfUnderline=0; 
	temlf.lfStrikeOut=0; 
	temlf.lfCharSet=134; 
	temlf.lfOutPrecision=3; 
	temlf.lfClipPrecision=2; 
	temlf.lfQuality=1; 
	temlf.lfPitchAndFamily=2;  
	OutFont.CreateFontIndirect (&temlf);

	CFont* def_font = MemDC.SelectObject(&OutFont);
	MemDC.TextOut(0,0,sInfo);		//打上信息文字
	MemDC.SelectObject(def_font);

	COLORREF color;
	BYTE Red,Green,Blue;
	short i,j;
	//循环得到像素
	for(j=Col*length-1;j>=0;j--)
	{
		for(i=0;i<Row;i++)
		{
			color = MemDC.GetPixel(j,i);
			if(color == RGB(0,0,0))		//当白色时
			{
				i =-20;		//??
				break;
			}
		}
		if(i<0)
			break;
	}

	if(i<0)
	{
		if((j+2)<Col*length)
			Len = j+2;
		else
			Len = j;
	}

	long Offset1;
	int ii,jj,kk,ll;
	float Scale = 1.0;		//1.5;

	if(sInfo.GetLength()>58)
		Scale = (float)0.8;
	else
		Scale = (float)1.0;

	for(i=0;i<Row;i++)
	{
		for(j=0;j<Len;j++)
		{
			color = MemDC.GetPixel(j,i);	//像素
			Blue = GetBValue(color);
			Green = GetGValue(color);
			Red = GetRValue(color);

			//底色不迭加
			if(Blue == 255 && Green == 255 && Red ==255)
				continue;
							
			kk = (int)(Scale*(i+1) +0.5);

			for(ii = (int)(Scale*i); ii<kk ; ii++)
			{
				ll = (int)(Scale*(j+1)+0.5);
				for(jj = int(Scale*j) ; jj<ll; jj++)
				{
					Offset1 = (Height-10 -ii+1) * nWidthBytes + jj * 3;
					Buf[Offset1] = Blue;
					Buf[Offset1+1] = Green;
					Buf[Offset1+2] = Red;
				}
			}
		}
	}

	MemDC.SelectObject(pOld);

	def_font->DeleteObject();
	pOldPen->DeleteObject();
	pOldBrush->DeleteObject();

}

//显示图片到按钮上:文件形式bmp
void CPictureProcess::ShowPicture(CString strPicName,HWND hwndStill)
{
	////////如果不是bmp格式的不显示//////////////
	int nFind = -1;
	CString strBMPStr = strPicName;
	strBMPStr.MakeUpper();
	nFind = strBMPStr.Find("BMP");
	if(nFind <= 0)
		return;
	//////////////////////////////////////////

	CBasicUse bUse;
	if (bUse.FileExists(strPicName) == FALSE){
		return;
	}

	CString LastDisplayFile = strPicName;

	BITMAPINFO bitmapinfo = GetBmpInfo(LastDisplayFile);
	if(bitmapinfo.bmiHeader.biWidth <= 0)
		return;

	long Width,Height;
	Width = bitmapinfo.bmiHeader.biWidth;
	Height = bitmapinfo.bmiHeader.biHeight;
	BYTE *Buf;
	Buf = new  BYTE[(long)(Height*Width*3)]; 
	GetBmpBuffer(LastDisplayFile,Buf);

	ShowPictureByByte(Buf,hwndStill,Width,Height);

	if(Buf != NULL){
		delete Buf;
	}

}

//显示图片:流-stop
void CPictureProcess::ShowPicture(BYTE *bytePicName,HWND hwndStill){

	BYTE *Buf;

	Buf = bytePicName;

	int Width = 100;

	int Height = 100;
	
	ShowPictureByByte(Buf,hwndStill,Width,Height);

	if(Buf != NULL){
		delete Buf;
	}

}
//图片文件数据流显示到控件上（本程序用的是按钮控件）
void CPictureProcess::ShowPictureByByte(BYTE *Buf,HWND hwndStill,int Width,int Height)
{
	/////////显示图片:hwndStill为传进来的句柄////////////////////////
	RECT rc;
	::GetWindowRect( hwndStill, &rc );
	long lStillWidth = rc.right - rc.left;
	long lStillHeight = rc.bottom - rc.top;

 	//显示识别出来的图片
	BITMAPINFOHEADER bih;
	memset( &bih, 0, sizeof( bih ) );
	bih.biSize = sizeof( bih );
	bih.biWidth = Width;
	bih.biHeight = Height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;

	HDC hdcStill = ::GetDC( hwndStill );
	PAINTSTRUCT ps;
	::BeginPaint(hwndStill, &ps);
	int Prev = SetStretchBltMode(hdcStill, COLORONCOLOR);
	StretchDIBits( 
		hdcStill, 0, 0, 
		lStillWidth, lStillHeight, 
		0, 0, Width, Height, 
		Buf, 
		(BITMAPINFO*) &bih, 
		DIB_RGB_COLORS, 
		SRCCOPY );
	::EndPaint(hwndStill, &ps);
	::ReleaseDC( hwndStill, hdcStill ); 

	/*
	if(Buf != NULL){
		Buf = NULL;
		delete Buf;
	}
	*/

}

//jpg图片文件转换成BLOB数据流，以便数据库用
VARIANT CPictureProcess::ChangePicFileToBYTE(CString strPicName){

	CFile imagefile;
	VARIANT	varBLOB;
		
	try{
		//
		if(imagefile.Open(strPicName,CFile::modeRead)){

			int nSize = imagefile.GetLength();          //先得到jpg文件长度

			BYTE * pBuffer = new BYTE [nSize];  //按文件的大小在堆上申请一块内存
		 
			if (imagefile.Read(pBuffer, nSize) > 0 )    //把jpg文件读到pBuffer(堆上申请一块内存)
			{   // +----------------------------------------------
				BYTE *pBuf = pBuffer;     ///下面这一大段是把pBuffer里的jpg数据放到库中
				
				SAFEARRAY		*psa;
				SAFEARRAYBOUND	rgsabound[1];
		
				if(pBuf)
				{    
					rgsabound[0].lLbound = 0;
					rgsabound[0].cElements = nSize;
					psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
					for (long i = 0; i < (long)nSize; i++)
						SafeArrayPutElement (psa, &i, pBuf++);

					varBLOB.vt = VT_ARRAY | VT_UI1;
					varBLOB.parray = psa;
				}
							
				//释放
				if(pBuf != NULL)
					delete pBuf;
				
				SafeArrayDestroy(psa);	//释放

			}

			imagefile.Close();	

			if(pBuffer != NULL)
				delete pBuffer;
			
		}
		
		
	}catch(CException * e){
		CString error;
		error.Format("出错!\r\n错误信息：%s",e->GetErrorMessage("指针",255));
	}

	return varBLOB;
}

//数据库图象转换成bmp文件
BOOL CPictureProcess::DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile){

	BOOL ifSuccess = FALSE;

	try{
		
		if(BYTE *pBuffer = new BYTE [lngStreamLength+1])		///重新申请必要的存储空间
		{	
			char *pBuf = NULL;

			SafeArrayAccessData(vDBStream.parray,(void **)&pBuf);

			memcpy(pBuffer,pBuf,lngStreamLength);				///复制数据到缓冲区m_pBMPBuffer

			SafeArrayUnaccessData (vDBStream.parray);
	
			CFile outFile(strPicFile,CFile::modeCreate|CFile::modeWrite);

			LPCTSTR buffer = (LPCTSTR)GlobalLock((HGLOBAL)pBuffer);

			outFile.Write(buffer,lngStreamLength);

			GlobalUnlock((HGLOBAL)pBuf);

			outFile.Close();

			SafeArrayUnaccessData (vDBStream.parray);
	
			if(pBuffer != NULL)
				delete pBuffer;

			pBuf=0;

			//需要延时，防止文件生成速度慢


		}
		
		ifSuccess = TRUE;
	
	}catch(CException * e){
		CString error;
		error.Format("出错!\r\n错误信息：%s",e->GetErrorMessage("指针",255));
		return FALSE;
	}

	return ifSuccess;

}

//得到bmp图片的基本信息：宽，高，bits
//返回值为指针数组，包含信息为宽，高,bits及num ４位[4]
BITMAPINFO CPictureProcess::GetBmpInfo(LPCTSTR lpszBmpFile)
{
	BITMAPINFO BitmapInfo;
	
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return BitmapInfo;

	FILE *fp;

	//判断文件是否存在的另一种方式（看里面是否有内容）
	fp = fopen(lpszBmpFile,"r");
	if(fp==NULL)
	{
		return BitmapInfo;
	}
	
	long Offset,Offset1;
	Offset = sizeof(BITMAPFILEHEADER);

	Offset1 = sizeof(BITMAPINFO);
	Offset1 = 40;
	
	fseek(fp,Offset,SEEK_SET);
	fread(&BitmapInfo,Offset1,1,fp);
	
	fclose(fp);
		
	return BitmapInfo;
}

//得到bmp的流，前提是newbuffer被分配空间的大小就是lpszBmpFile读取内容的大小
void CPictureProcess::GetBmpBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer)
{
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return;

	FILE *fp;

	//判断文件是否存在的另一种方式（看里面是否有内容）
	fp = fopen(lpszBmpFile,"r");
	if(fp==NULL)
	{
		return;
	}
	
	long Offset,Offset1;
	Offset = sizeof(BITMAPFILEHEADER);

	Offset1 = sizeof(BITMAPINFO);
	Offset1 = 40;
	BITMAPINFO BitmapInfo;
	fseek(fp,Offset,SEEK_SET);
	fread(&BitmapInfo,Offset1,1,fp);

	int Height = BitmapInfo.bmiHeader.biHeight;
	int Width = BitmapInfo.bmiHeader.biWidth;
	int nBits = BitmapInfo.bmiHeader.biBitCount;
	
	Offset = Offset+Offset1;
	fseek(fp,Offset,SEEK_SET);
	fread(newbuffer,(long)(Height*Width*nBits/8),1,fp);
	fclose(fp);
	
}

//得到bmp图片翻转后的数据流，以便北京亚视车牌识别用...
//由于操作复杂，因此这里用的是动态库调用方式，简化程序 
void CPictureProcess::GetBmpVerBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer, CString dllName)
{
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return;
	
	///////////////得到数据流///////////////////////////////////
	BOOL bGet = FALSE;
	CBasicUse bUse;
	if (bUse.FileExists(dllName) == FALSE)
		return;
	
	//得到图片的翻转流，不然车牌识别无法进行。北京的车牌识别是从头开始的
	HINSTANCE HDLL_picProcess;	//动态连接库加载后的句柄jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);

	typedef BOOL(*pGetBmpVerBuffer)(LPCTSTR lpszBmpFile,LPBYTE newbuffer); 
	pGetBmpVerBuffer GetBmpVerBuffer;
	GetBmpVerBuffer=(pGetBmpVerBuffer)GetProcAddress(HDLL_picProcess,"GetBmpVerBuffer");
	if(GetBmpVerBuffer != NULL){
		bGet = GetBmpVerBuffer(sFilePath,newbuffer);
	}
	
	//卸载dll文件
	FreeLibrary(HDLL_picProcess);
	///////////////////////////////////////////////////////
	
}

//数据流存为bmp
//lpszFile 目标文件bmp; newbuffer为已有的数据流
BOOL CPictureProcess::SaveImage(LPCTSTR lpszFile, LPBYTE newbuffer, 
								long lWidth,long lHeight )
{

	if(newbuffer == NULL)
		return FALSE;

	//create file///////////
	HANDLE hf = CreateFile(
        lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, NULL, NULL );

    if( hf == INVALID_HANDLE_VALUE )
        return FALSE;

    // write out the file header
	long lBufferSize = lWidth * lHeight * 3;

    BITMAPFILEHEADER bfh;
    memset( &bfh, 0, sizeof( bfh ) );
    bfh.bfType = 'MB';
    bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
    bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );

    DWORD dwWritten = 0;
    WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );

    // and the bitmap format
    BITMAPINFOHEADER bih;
    memset( &bih, 0, sizeof( bih ) );
    bih.biSize = sizeof( bih );
    bih.biWidth = lWidth;
    bih.biHeight = lHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 24;	//图象为真彩色

    dwWritten = 0;
    WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );

    // and the bits themselves
    dwWritten = 0;
    WriteFile( hf, newbuffer, lBufferSize, &dwWritten, NULL );
    CloseHandle( hf );

	return TRUE;
}

/////bmp拉伸高度后保存图片，以便查看原始图片 288->576////////////////////
BOOL CPictureProcess::ZoomImageHeight(LPCTSTR lpszFile, int nHeightTimes)
{
	BOOL bSuccess = FALSE;

	CString strSourceFile = lpszFile;		//原始图

	//原始图大小信息
	long lWidthSource, lHeightSource, lSizeSource;
	//int nNum;

	BITMAPINFO bmpInfo = GetBmpInfo(strSourceFile);	//get bmp info
	if(bmpInfo.bmiHeader.biBitCount != 24)
		return FALSE;

	lWidthSource = bmpInfo.bmiHeader.biWidth;
	if (lWidthSource <= 0)
		return FALSE;
	lHeightSource = bmpInfo.bmiHeader.biHeight;
	lSizeSource = lWidthSource * lHeightSource * 3;
	
	LPBYTE lpImage;
	lpImage=new byte[lSizeSource];
	GetBmpBuffer(strSourceFile,lpImage);

	//////////////
	long lHeightDest = lHeightSource * nHeightTimes;	//高度
	long lWidthDest = lWidthSource;	//目标图片流的宽度和原的要保持一致
	long lSizeDest = lWidthDest * lHeightDest * 3;	//lHeightDest是入口参数 

	LPBYTE BufLast;
	BufLast=new byte[lSizeDest];		//分配目标数据流的空间
	ZoomImageHeight(lpImage,BufLast,lWidthSource,lHeightSource,nHeightTimes);
	
	::DeleteFile(lpszFile);		//删除原始图片
	SaveImage(lpszFile,BufLast,lWidthDest,lHeightDest);	//保存拉伸后的图片

	if(BufLast != NULL)
		delete BufLast;

	return TRUE;
}

//指定bmp数据流拉伸高度，lWidth和lHeight为原始流的宽高
//目标流的宽和原始的一样，高度是原始流的2倍
void CPictureProcess::ZoomImageHeight(LPBYTE sourcebuffer, LPBYTE destbuffer,
									  long lWidth,long lHeight,int nHeightTimes)
{

	int BmpHeight = 0;
	if(lHeight == 288 && lWidth >= 704)
	{
		//拉伸高度
		BmpHeight = lHeight;
		lHeight = lHeight * nHeightTimes;
	}else
		return;

	long len = lWidth * 3;
	LPBYTE newbuffer=new byte[lWidth*lHeight * 3];
	
	//目标图片高度是原图片高度2倍：拉伸压缩图片 288->576
	for(int i=0;i<BmpHeight;i++)
	{
		//移动2次成全图
		memcpy(destbuffer + i*len*nHeightTimes, sourcebuffer+i*len,len);
		memcpy(destbuffer + i*len*nHeightTimes + len, sourcebuffer+i*len,len);
	}

	destbuffer-=lWidth*lHeight*3;

	if(newbuffer != NULL)
		delete newbuffer;		//删除指针

	/*/////////////////////////////////////////////
	//高度为288时，要先进行拉伸
	//老方法，也好用 
	for (int i=0;i<288;i++)
	{
		 memcpy(newbuffer,lpImage,ptSize.x*3);
		 newbuffer+=ptSize.x*3;
		 memcpy(newbuffer,lpImage,ptSize.x*3);
		 newbuffer+=ptSize.x*3;
		 lpImage+=ptSize.x*3;
		 
	}
	destbuffer-=lWidth*lHeight*3;

	if(newbuffer != NULL)
		delete newbuffer;
	*//////////////////////////////////////////////

}

//jpg文件转换成bmp，并返回被转换的文件名
//临时用来显示的，显示之后要删除
CString CPictureProcess::ChangeJPGToBMP(CString strJPGFileName,CString dllName)
{

	CBasicUse bUse;

	FILE *fp;

	CString strTmp = strJPGFileName;
		
	CString LastDisplayFile = "";
	
	if(bUse.FileExists(strJPGFileName) == FALSE){
		return "";
	}

	//转换成名为.bmp的（截取路径）
	strTmp = bUse.JPGToBMPString(strTmp);
	
	strTmp = strTmp.Mid(strTmp.ReverseFind('\\') + 1);

	//放到临时目录下，以便显示图片用
	bUse.CreatePath(bUse.GetApp_Path() + "\\tempPic");
	LastDisplayFile = bUse.GetApp_Path() + "\\tempPic\\" + strTmp;

	//////////////测试时暂时屏蔽///////////
	
	//如果临时图片没内容，不显示
	if( strcmp(LastDisplayFile,"")!=0)
	{
		fp = fopen(LastDisplayFile,"r");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(LastDisplayFile);		//删除文件
		}
	}
	
	////把CString -> char，为了JpgToBmp11函数的参数
	char FileName_JPG[256];
	char FileName_BMP[256];

	strcpy(FileName_JPG,(LPCTSTR)strJPGFileName);

	LastDisplayFile.Format("%s",LastDisplayFile);

	strcpy(FileName_BMP,(LPCTSTR)LastDisplayFile);

	///将jpg格式文件转换成bmp格式文件///////////////////////////
	CString strCurAppPath = bUse.GetApp_Path();
	
	//调用动态连接库,带参数的方式
	HINSTANCE HDLL_picProcess;	//动态连接库加载后的句柄jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);
	typedef BOOL(*pJpgToBmp)(LPCTSTR lpszJpgFile,LPCTSTR lpszBmpFile); 
	pJpgToBmp JpgToBmp;
	JpgToBmp=(pJpgToBmp)GetProcAddress(HDLL_picProcess,"JpgToBmp");
	if(JpgToBmp != NULL){
		BOOL res=JpgToBmp(FileName_JPG,FileName_BMP);
	}
	//卸载dll文件
	FreeLibrary(HDLL_picProcess);

	LastDisplayFile.Format("%s",LastDisplayFile);
		
	return LastDisplayFile;

}


CString CPictureProcess::ChangeJPGToBMP2(CString strJPGFileName)
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCurPath;//CString strCurPath= m_bUse.GetApp_Path();
	char file_name[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, file_name, _MAX_PATH);
	CString strFileName = file_name;
	int num = strFileName.ReverseFind('\\');

	strCurPath = strFileName.Left(num+1);

	CBasicUse bUse;
	CString strTmp = bUse.JPGToBMPString(strJPGFileName);
	strTmp = strTmp.Mid(strTmp.ReverseFind('\\') + 1);

	bUse.CreatePath(strCurPath + "tempPic");
	CString JpgFile = strCurPath + "tempPic\\" + strTmp;

	//加载jpg或jpeg文件
	Bitmap *pBmp = Bitmap::FromFile(strJPGFileName.AllocSysString());
	if (pBmp)
	{
		CLSID clsid;
		//获取位图类的ID
		GetCodecClsid(L"image/bmp", &clsid);

		//保存为位图类型
		pBmp->Save(JpgFile.AllocSysString(),&clsid);
	}
	return JpgFile;
}
//获取各种类型图像的类ID
int CPictureProcess::GetCodecClsid(const WCHAR* format, CLSID* pClsid)//编码类标识符
{
	UINT  codenum = 0;   
	UINT  size = 0;   
	ImageCodecInfo* pImageCodecInfo = NULL;

	//获取图像的编码大小
	GetImageEncodersSize(&codenum, &size);
	if(size == 0)
		return -1 ;  

	//定义图像编码信息缓冲区
	pImageCodecInfo = new ImageCodecInfo[size];
	if(pImageCodecInfo == NULL)
		return -1; 

	//获取图像编码信息
	GetImageEncoders(codenum, size, pImageCodecInfo);
	for(UINT j = 0; j < codenum; ++j)
	{
		//比较图像编码，是否与参数相同
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			//获取图像类型的类ID
			*pClsid = pImageCodecInfo[j].Clsid;

			//释放图像编码信息缓冲区
			delete []pImageCodecInfo;
			return 0;  
		}    

	} 
	//释放图像编码信息缓冲区
	delete []pImageCodecInfo;
	return -1;  
}






//jpg文件转换成bmp，并返回被转换的文件名
//要放到和bmp同目录下
CString CPictureProcess::ChangeBMPToJPG(CString strBMPFileName,CString dllName)
{

	CBasicUse bUse;
	

	CString LastDisplayFile = "";
	CString strTmp = strBMPFileName;
			
	if(bUse.FileExists(strBMPFileName) == FALSE){
		return "";
	}

	//转换成名为.bmp的（截取路径）
	strTmp = bUse.BMPToJPGString(strTmp);
	LastDisplayFile = strTmp;	//得到jpg名

	FILE *fp;
	//如果临时图片没内容，不显示
	if( strcmp(LastDisplayFile,"")!=0)
	{
		fp = fopen(LastDisplayFile,"r");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(LastDisplayFile);		//删除文件
		}
	}
	
	////把CString -> char，为了JpgToBmp11函数的参数
	char FileName_JPG[256];
	char FileName_BMP[256];

	strcpy(FileName_BMP,(LPCTSTR)strBMPFileName);

	LastDisplayFile.Format("%s",LastDisplayFile);

	strcpy(FileName_JPG,(LPCTSTR)LastDisplayFile);

	///将jpg格式文件转换成bmp格式文件///////////////////////////
	CString strCurAppPath = bUse.GetApp_Path();
	
	//调用动态连接库,带参数的方式
	HINSTANCE HDLL_picProcess;	//动态连接库加载后的句柄jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);
	typedef BOOL(*pBmpToJpg)(LPCTSTR lpszBmpFile,LPCTSTR lpszJpgFile); 
	pBmpToJpg BmpToJpg;
	BmpToJpg=(pBmpToJpg)GetProcAddress(HDLL_picProcess,"BmpToJpg");
	if(BmpToJpg != NULL){
		BOOL res=BmpToJpg(FileName_BMP,FileName_JPG);
	}
	//卸载dll文件
	FreeLibrary(HDLL_picProcess);

	LastDisplayFile.Format("%s",LastDisplayFile);
		
	return LastDisplayFile;	//

}

CString CPictureProcess::ChangeBMPToJPG2(CString strBMPFileName)
{
	CBasicUse bUse;

	CString  JpgFile= bUse.BMPToJPGString(strBMPFileName);

	//加载位图文件
	Bitmap *pBmp = Bitmap::FromFile(strBMPFileName.AllocSysString());
	if (pBmp)
	{
		//定义类ID
		CLSID clsid;

		//设置压缩质量
		int nQuality = 95;

		//定义编码参数
		EncoderParameters Encoders;

		//设置参数数量
		Encoders.Count = 1;

		//设置参数标记
		Encoders.Parameter[0].Guid = EncoderQuality;

		//设置参数类型
		Encoders.Parameter[0].Type = EncoderParameterValueTypeLong;

		//设置参数值数量
		Encoders.Parameter[0].NumberOfValues = 1;

		//设置压缩质量
		Encoders.Parameter[0].Value = &nQuality;

		//获取JPEG类ID
		GetCodecClsid(L"image/jpeg", &clsid);

		//保存为JPEG文件
		pBmp->Save(JpgFile.AllocSysString(),&clsid,&Encoders);
	}
	return JpgFile;
}
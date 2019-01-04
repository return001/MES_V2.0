#include "stdafx.h"
#include "ZebraPrinter.h"

#include <fstream>
using namespace std;

//中文转16进制字符用于打印
#include "ConvertCHN.h"

CZebraPrinter::CZebraPrinter(void)
{
}

CZebraPrinter::~CZebraPrinter(void)
{
}

bool CZebraPrinter::OnPrintFunction(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font) 
{
	/*
	CString m_csIMEI;//输入的IMEI号
	//获取LABEL命令
	CString	csLABEL="";
	CString	csStr;
	//LABEL格式开始
	csLABEL="^XA\r\n";
	csLABEL+="^LH020,010\r\n";
	//入网证号码
	csLABEL+="^FO000,000^AFN,28,15^FDIMEI";

	csLABEL+=m_csIMEI;
	csLABEL+="^FS\r\n";
	//入网证条码	
	csLABEL+="^FO000,040^B3N,N,50,N,N^FD";
	csLABEL+=m_csIMEI;
	csLABEL+="^FS\r\n";
	//LABEL格式结束
	csLABEL+="^XZ\r\n";
	//获得打印标签数
	//m_ceLabels.GetWindowText(m_csLabels);
	int	labels=atoi("2");
	//保存LABEL格式命令于文件LABEL.ZPL
	ofstream ofsLabel("LABEL.ZPL",ios::binary);
	for(int i=0;i<labels;i++)
		ofsLabel.write(csLABEL,csLABEL.GetLength());
	ofsLabel.close();
	//传送命令文件到打印机	
	system("type LABEL.ZPL>LPT1");
	*/
//扫描下一个
	//...
	//PrintContent(high,width,x,y,SSID,PassWord,MSN,IMEI,Font,X_Pos,Y_Pos,CH_Font);
	if( hPLPort != INVALID_HANDLE_VALUE) 
	{
		CloseHandle( hPLPort);
		hPLPort = INVALID_HANDLE_VALUE;
	}

	hPLPort = CreateFile( "LPT1", GENERIC_READ| GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if( hPLPort == INVALID_HANDLE_VALUE) 
	{
		AfxMessageBox("错误: 并口打开失败,无法打印条码!");
		return false;
	}

	DWORD dwActWrite = 0;
	//int   MAX_BUFLEN = 40960;																				//NOT EXCEED THIS!不要超过40960
	//BOOL r0=WriteFile( hPLPort, csBuffer.GetBuffer(MAX_BUFLEN), csBuffer.GetLength(), &dwActWrite, NULL);	//通过并口给打印机发送数据
	CString csBuffer;
	csBuffer=PrintContent(high,width,x,y,SSID,PassWord,MSN,IMEI,Font,X_Pos,Y_Pos,CH_Font);					//Font:打印机内在有位图字体 A\B\D\E\F\G\H\GS\0  矢量字体P\Q\R\S\T\U\V
	BOOL Result=WriteFile( hPLPort, csBuffer, csBuffer.GetLength(), &dwActWrite, NULL);						//通过并口给打印机发送数据
	if((Result==FALSE)||(csBuffer.GetLength()!=dwActWrite))
	{
		AfxMessageBox("打印失败！");
		return false;
	}
	csBuffer.ReleaseBuffer();
	if( hPLPort != INVALID_HANDLE_VALUE) 
	{
		CloseHandle( hPLPort);
		hPLPort = INVALID_HANDLE_VALUE;
	}
	return true;
}

CString CZebraPrinter::PrintChinese(char *strSource,char* pCHN_ID,char* CH_Font,int high,int width,int x,int y) //CH_Font为字体 如“宋体”
{
	/*
	基本标签示例：  
	^XA								‘标签格式开始命令   
	^LH30,30						‘Label Home position设置标签起始参考点坐标为30,30，默认为 0,0    200dpi机器，8点=1mm  300dpi机器，12点=1mm  
	^FO20,10^AD^FDEXAMPLE^FS		‘^FO20,10-设置数据域起始坐标，距参 考点20,10   ^AD-设定字体为D（内部字体） ^FD-数据域开始命令   EXAMPLE实际打印内容  ^FS-数据域分隔命令   
	^FO20,60^B3^FDAAA001^FS			‘^B3-选择条码类型为39码   
	^XZ								‘标签格式结束命令
	*/


	/*
	^LTx 
	标签内容上下（Y轴）位置整体移动	  
	x：移动的数值，范围：-120 - +120点
	  
	^LSa 
	标签内容左右（X轴）位置整体移动  
	a：移动的数值，范围：-9999 - +9999点

	^Af,o,h,w 打印文字格式定义 
	f：字体，默认为0，选择范围：A-Z,0-9（均为打印机内部字体，非TrueType字体）  
	o：文字方向，N=普通，R=旋转90度，I=旋转180度，  B=旋转270度  
	h：字符高度，范围：10-1500点 

	^BYw,r,h 设定条码默认参数  
	w：条宽，缺省为2点，选择范围：1-10点  
	r：条空比率，缺省为3.0，选择范围：2.0-3.0
	h：条码高度 

	^B3o,e,h,f,g  打印39码 
	o：条码方向  
	e：Mod-43校验位，N=No（默认），Y=Yes 
	h：条码高度，范围：1-9999点  
	f：是否打印字符行，Y=Yes（默认），N=No 
	g：字符行位置，N：条码下方，Y：条码上方 

	^BCo,h,f,g,e,m  打印128码  
	e：UCC校验位，N=No（默认），Y=Yes  
	m：模式，一般设为A（自动选择）或忽略不计 

	^PRp 打印速率  
	p：速度——2-8（或A-E）=2英寸/秒-8英寸/秒 

	^PQq 打印数量  q：总数量，选择范围：1-99999999 

	^GBw,h,t  打印方框  w：方框宽度，h：方框宽度，t：线宽 


	XG 调用图象
	^XG指令格式
	^XG < Sve: > Objectname < .ext >, x, y
	^XG=调用图象

	Sve: = 贮存图象原装置
	附加，缺省是搜索最先的
	Objectname = 贮存图象名，1-8个字符 缺省用揢NKNOWN斆?/P>
	.ext = 扩展名，3个字符 固定的，始终是.GRF
	x = x方向放大因子。
	缺省值：1
	最小值：1，最大值：10

	y = y方向放大因子
	缺省值：1
	最小值：1，最大值10

	以下是用^XG指令调用图象从DRAM中SAMPLE.GRF然后打印5个不同地方和5不同尺寸在同一标签上的例子。
	移动图象
	^IM（移动图象）指令执行一个点阵图象直接从贮存原里移动。这指令同样调用图象指令而没尺寸参数。
	^IM指令格式是
	^IM < Svc: > Objectname < .ext >
	^IM = 移动点阵字体

	Svc: = 被移动图象的源装置
	附加的，缺省时搜索最行先的
	Objecname = 贮存图象名，1-8字符（缺省，用揢NKNOWN斆?/P>
	.ext = 扩展名，3字符 固定，始终是.GRF
	注1：用^FO指令，国家可以定位在标签任何位置。
	注2：在^IM和^XG指令间的区别是图象移动指令没有放大，所以所需格式时间较少。更进一步，这图象必须是在8，16或32


	*/
	CString DesData;																//DesData 是最后获取的   打印中文的ZPL指令
	CConvertCHN chn;
	//char *strSource = "测试";														//需要转换的汉字，如果汉字过多，需增加下面的数组长度
	char strEnd[2048]="\0";															//转换后的字符串，此字符串发送给打印机
	//memset(strEnd,0,2048);
	char *strError ="";

	if(!chn.autoFormatCHNCode(strSource,strEnd,pCHN_ID,CH_Font,strError,high,width))	//参数2为转换输出的数据   第三个参数为转换后保存的名称，打印的时候用到，可以任意8位字符 "OUTSTR01"
	{
		return "ERROR";
	}
	DesData.Format("%s",strEnd);
	DesData.Format(DesData+"^FO%d,%d^XG%s,1,1^FS",x,y-5,pCHN_ID);//-5上移一点
	return DesData;
	
	/*
	~DG 下载图象
　　~DG（下载图象）执行以下功能。
	1.	置打印机为图象模式。
	2.	命名图形。（这个名字将用来在标签中调用）
	3.	定义图象尺寸
	4.	下载十六进制字符串到打印机
	注意：
	1.	为使打印机明白，使用图形名时将以空格、句点、或扩展名结束
	2.	避免偶然被空格替换，不要在图象名中用空格。不同图象用不同名字。
	3.	如二个图象用了同一名送到打印机。第一个图象被删除而由第二个图象代替。
	～DG指令格式
　　	~DGd:o.x,t,w,DATA
	~DG	=	设置打印机为下载图形模式
	d	=	贮存图象的目标设备。
 	 		缺省值：R: (DRAM)
	其它值：B: (内存选件)
	o	=	图象名,1-8字符,
 	 		（缺省时，用UNKNOWN作字体名)
	x	=	扩展名，3字符 （ 固定的，始终是：GRF）
	t	=	图象总的字节数
	w	=	每行字节数
	DATA	=	ASCII十六进制串图象定义
　　	如目标名省略，就用UNKNOWN.GRF作为字图象名。数据串使用ASCII十六进制串图象定义，每个字符表示水平方向的四个点。
　　	以下是一个用～DG指令加载图象到DRAM的例子。贮存图象名叫SAMPLE.GRF。
	~DGR:SAMPLE.GRF,00080,010,
	FFFFFFFFFFFFFFFFFFFF
	8000FFFF0000FFFF0001
	8000FFFF0000FFFF0001
	8000FFFF0000FFFF0001
	FFFF0000FFFF0000FFFF
	FFFF0000FFFF0000FFFF
	FFFF0000FFFF0000FFFF
	FFFFFFFFFFFFFFFFFFFF
	参数t（图形总字节数）用以下公式计算：
　　	X (毫米）×打印机分辨率（点/毫米） ×  Y（毫米） ×打印机分辨（点/毫米）/8（点/字节）=   总字节
　　	X 是单位毫米的图象宽度。Y是单位毫米的图象高度。点/毫米打印机编程的打印分辨率。
　　	例如，确定图象8毫米宽，16毫米高，打印分辨率8点/毫米的正确t参数其公式是：
　　	8×8×16×8/8=1024字节
	　
	参数w（每行字节数）用以下公式计算：
　　	X (毫米）×打印机分辨率（点/毫米） /8（点/字节）=   每行字节数
　　	x是单位毫米的图象宽充，点/毫米是打印机偏移打印分辨率。
　　	例如，确定图象8毫米宽，打印分辨率8点/毫米的正确w参数，其公式是：
　　　　	8×8/8 = 8字节
	注意：
	1.	所有字节中一行的字节
	2.	w是t参数计算的第一个值
　　	参数<DATA>是一串十六进制数作为图象表示送打印机。每一十六进制字符代表水平方向四个点。如图象前四个点是白的，后四个点是黑的。二进制码的点00001111。十六进制表示二进制值将是OF。完整的图象码就是这样。完整图象被送打印机是一长连续十六制值。
　　	对～DG和～DB指令选择数据压缩方案。
　　	斑马打印机有一数魏数据压缩方案。在～DG和～DB指令中这方案减少下载图象和点阵字体实际数据字节数和总的时间。
　　	下面数1，2，3，4，5，……19代表在后面十六进制值（注：值从G开始O到O和A到F已经在十六进制值是使用）
	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y
	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19
　　	例如，送M6到打印机表示如下十六进制数据。
　　　　	6666666
　　	M的值是7，所以传送7个十六进制的6
	g	h	i	j	k	l	m	n	o	p	q	r	s	t	u	v	w	x	y	z
	20	40	60	80	100	120	140	160	180	200	220	240	260	280	300	320	340	360	380	400
　　	例如：送-hB到打印机表示送十六进制数据　　
	BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
	40
　　	h值是40，所以传送40 个十六进制的B。
	重复值
　　	几个重复值一起使用得到任何需要值，“vMB”和“MvB”一样，都是传送327个十六进制的B到打印机。
	把逗号（，）填进行，表示该行右边，都是0，
	感叹号（！）填进行，表示该行右边都是1，
	冒号 （：）表示重复上一行。
　　	解码和参数t字节数打印后，打印机返回原来的打印模式。
	减少下载图象时间
　　	当用～DG指令时，这方法减少实际送打印机的数据字节数，在下面图6.1和6.2可看到。
　　	在图6.2，如果十六进制串都是到零（0）结束。一个逗号（，）可以替代所有的零。如果十六进制串是奇数个0结束，需加一个零和逗号。感叹号（！）和冒号（：）在用法如前。
　　	警告：在你编辑器文本行将不同于ZPLII点的行。编辑器将字把点行包裹起来。ZPLII跳过文本行结束（如：回车和换行字符）。
　　	注：在图6.1和6.2回车已经被插入每个点行的结束。
	　
	 
	　
	~DN 取消下载图象
　　	^DG在解码和打印完参数t的字节后，打印机返回一般模式。用～DN（取消下载图象）指令。图象模式可以取消原来打印操作恢复。
	～DN指令格式
　　	～DN
	～DN：取消下载图象
	注意：任何^或～指令将结束下载。

	其实他就是将“strSource”字转换成图象再转换成十六进制编码最后进行ZPL的压缩，利用“~DG”命令将编码发送给打印机最后利用“^XG”命令进行图象打印。整个结果中~DGOUTSTR01这个命令是将图象编码发送给打印机并存贮在打印机存储卡上同时编码文件被命名为OUTSTR01。紧跟的两个参数是指打印图像的总字节和每行的字节数，最后的东西就是图象编码。
	*/

}

CString CZebraPrinter::PrintContent(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font)					//组合打印的ZPL2 命令
{
	CString tmp;

//[0]--网络名 值偏移量  [1]--"WIFI密码：" 字符偏移量  [2]--WIFI密码 值偏移量  [3]--MSN 值偏移量   [4]--IMEI  值偏移量  [5]--IMEI条码偏移量
//[0]--MSN Y轴偏移量   [1]--IMEI Y轴偏移量  [2]--IMEI条码 Y轴偏移量 

	tmp="^XA^IA6^XZ^FS^XA^FS^LH0,0^FS";
	tmp+="^FO";
	tmp.Format(tmp+"%d,%d^MD",x,y);																					//^MD（标签深度）指令调整与当前设定深度的关系，最小值 -30，最大30。（如参数错或不正确指令跳过）
	//tmp+=PrintChinese("网络名：","OUTSTR01",CH_Font,high,width,x,y);
	tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[0],y,Font,high,width,SSID);								//^FO%d,%d^-设置数据域起始坐标，距参 考点20,10   ^AD-设定字体为D（内部字体） ^FD-数据域开始命令   EXAMPLE实际打印内容  ^FS-数据域分隔命令   
	
	//tmp+=PrintChinese("WIFI密码：","OUTSTR01",CH_Font,high,width,x+X_Pos[1],y);
	tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x,y+Y_Pos[1],Font,high,width,PassWord);								   
	
	//tmp+=PrintChinese("S/N：","OUTSTR01",CH_Font,high,width,x,y+Y_Pos[0]);
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[3],y+Y_Pos[0],Font,high,width,MSN);								   
	
	//tmp+=PrintChinese("IMEI：","OUTSTR01",CH_Font,high,width,x,y+Y_Pos[1]);
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[4],y+Y_Pos[1],Font,high,width,IMEI);			
	//tmp+=PrintChinese("IMEI：","OUTSTR01",CH_Font,high,width,x+Y_Pos[1],y+Y_Pos[0]);								//X_Pos[1] 将SM于IMEI至于同行
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[4],y+Y_Pos[0],Font,high,width,IMEI);		
	/////////////////////////////////////////////////////////////////////////////////
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^BCN,65,N,N^FD%s^FS",x+X_Pos[5],y+Y_Pos[2],Font,high,width,IMEI);	

	tmp+="^PQ1^FS";
	tmp+="^PRA^FS^XZ^FS^XA^EG^XZ"; //打印结束  


	return tmp;
	/*
	CString csBuffer;
	
	csBuffer +=		"^XA";															//^XA标签格式开始命令   ^LH30,30   ‘Label Home position设置标签起始参考点坐标为30,30，默认为 0,0 
	csBuffer +=		"^MMT";															//^MM（打印模式）?设置打印机到四种基本模式之一，撕断，回线，剥离和切刀。  撕断
	csBuffer +=		"^LL0160";														//LL 标签长度
	csBuffer +=		"^LS0";															//左边偏移
	csBuffer +=		"^FT0,10^XGOUTSTR01,1,1^FS";									//FT相对于LH偏移；打印汉字，OUTSTR01即为转换后的字符串  
	csBuffer +=		"^FO20,80^B3N,N,100,Y^FD20120914^FS";							//打印条码
	csBuffer +=		"^FD20120914^FS";
	csBuffer +=		"^XZ";
		
	^MT指令格式
	^MT x
	^MT = 纸种类
	x = 厅用纸
	T = 热转印纸
	D = 热敏纸

	*/
	
	/*
	^PR（打印速度）指令决定标签打印速度和回转速度（送一空标签）
	^PRP,S
	^PR=打印速度
	P=打印速度
	缺省值：速度A
	许可值：A或2 50.8毫米/秒（2英寸/秒）
	B或3 76.2毫米/秒（2英寸/秒）
	C或4 101.6毫米/秒（2英寸/秒）
	5 127毫米/秒（2英寸/秒）
	D或6 152.4毫米/秒（2英寸/秒）
	E或8 203.2毫米/秒（2英寸/秒）

	S=回转速度
	缺省值：速度D
	许可值：A或2 50.8毫米/秒（2英寸/秒）
	B或3 76.2毫米/秒（2英寸/秒）
	C或4 101.6毫米/秒（2英寸/秒）
	5 127毫米/秒（2英寸/秒）
	D或6 152.4毫米/秒（2英寸/秒）
	E或8 203.2毫米/秒（2英寸/秒）
	打印机选择速度操作直到以后重输入格式或打印机关机。
	打印机速度应用举例。打印质量取决于标签、色带、打印速度和打印操作模式。运行测试你的应用是非常重要的。

	^EG或~EG（删除下载图象）指令是用于从DRAM中删除所有图象（标签格式图象和十六进制图象）
	～EG或^EG指令格式是
	～EG或^EG

	^MM（打印模式）指令是确定一张或一组标签已被打印后打印机的动作。有四种不同操作模式。
	Tear Off?打印后，标签空区进到撕板上，标签连着底纸，可以用于撕下。
	Rewind?  标签和底纸被回绕在一个（附件）外加回绕装置，下一张标签定位在打印头下（无回送动作）。
	Peel Off?打印后，部分标签从底纸上被分离。打印停止直到标签被取走。底纸被绕在只绕底纸的回绕轴上（注：选用回绕轴已装在打印机内）。
	Cutter?隔着空区打印的和下一标签被送进切纸器，标签切下。标签被控回进打印机继续打。
	^MM指令格式
	^MM=打印模式
	x = 要求模式
	T = 撕下
	R = 回绕
	P = 剥离
	C = 切刀
	A = 保留
	（如参数错或不正确指令跳过）

	*/
}
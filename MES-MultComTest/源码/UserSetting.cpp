// UserSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "UserSetting.h"


// CUserSetting 对话框

IMPLEMENT_DYNAMIC(CUserSetting, CDialog)

CUserSetting::CUserSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSetting::IDD, pParent)
	, UserName(_T(""))
	, Password(_T(""))
	, NewPassword(_T(""))
	, Limits(_T(""))
{

}

CUserSetting::~CUserSetting()
{
}

void CUserSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UserName);
	DDX_Text(pDX, IDC_EDIT2, Password);
	DDX_Text(pDX, IDC_EDIT3, NewPassword);
	DDX_Text(pDX, IDC_EDIT10, Limits);
	DDX_Control(pDX, IDC_COMBO3, LimitsControl);
	DDX_Control(pDX, IDC_COMBO2, AllUsersBox);
}


BEGIN_MESSAGE_MAP(CUserSetting, CDialog)

	ON_WM_TIMER()//定时器

	ON_BN_CLICKED(IDC_BUTTON1, &CUserSetting::OnBnClickedButtonstart1)
	ON_BN_CLICKED(IDC_BUTTON7, &CUserSetting::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CUserSetting::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &CUserSetting::OnBnClickedButtonstart10)
	ON_BN_CLICKED(IDC_BUTTON11, &CUserSetting::OnBnClickedButtonstart11)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CUserSetting::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CUserSetting 消息处理程序

BOOL CUserSetting::OnInitDialog()
{
	CDialog::OnInitDialog();	
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return FALSE;
	}
	GetUsersFromDB();
	for (int i = 0; i < UserArray.GetCount(); i++)
	{
		CString user = GetData((LPSTR)(LPCTSTR)UserArray.GetAt(i), "NULL", "@@", 1);	//订单
		AllUsersBox.AddString(user);
	}
	//AllUsersBox.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}
CString CUserSetting::GetData(char* Serial_Order_Return, CString Start, CString End, int Count, int HandleNum)
{
	CString DataGet;
	//CString DataGetTemp;
	int pos = -1;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s", Serial_Order_Return);
	for (int i = 0; i<Count; i++)
	{
		if (i == Count - 1)
		{
			if ((Start == "NULL") || (Start == ""))
			{
				pos = Serial_Order_Return_CS.Find(End);
				if (pos >= 0)
				{
					DataGet = Serial_Order_Return_CS.Left(pos);
				}
				else
				{
					return Serial_Order_Return_CS;
				}
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					pos = Serial_Order_Return_CS.Find(End);
					if (pos >= 0)
					{
						DataGet = Serial_Order_Return_CS.Left(pos);
					}
					else
					{
						return Serial_Order_Return_CS;//return "NULL";
					}
				}
				else
				{
					return "NULL";
				}
			}
		}
		else
		{
			if ((Start == "NULL") || (Start == ""))
			{
				continue;
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					continue;
				}
				else
				{
					return "NULL";
				}
			}
		}

		return DataGet;
	}
	return "NULL";
}

void CUserSetting::OnBnClickedButtonstart1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	UpdateUserToDb(UserName, Password, Limits);
}
BOOL CUserSetting::ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd)
{
	if (1)//数据库地址不为空，就上传数据
	{
		CString Conn = "";
		CString DB_FAIL = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", m_server, m_db, m_user, m_pwd);
		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1表示已经打开,0表示关闭，数据库意外断开，重连
		{
			if (MessageBox("数据库处于断开状态，重连？", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK)
				DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//数据库意外断开，重连
			else
				return FALSE;
		}

		if (DB_FAIL == "FAIL")
		{
			AfxMessageBox("连接数据库失败，请检查网络.......");
			return FALSE;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MD5 mb;
		//CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
		//mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
		//CString CpuOrderMD5 = (mb.toString()).c_str();

	}
	return TRUE;
}
BOOL CUserSetting::GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits)
{
	_variant_t var;
	CString FieldName, FieldValue;
	try
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		//var= myado.m_pRec->GetCollect(_variant_t((long)5));
		FieldName.Format("UserType");
		var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
		if (var.vt != VT_NULL)
		{
			FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
			*Limits = FieldValue;
		}
		else
		{
			*Limits = "";
		}

	}
	catch (_com_error e)
	{
		*Limits = "";
		myado.CloseSheet();
		return FALSE;
	}
	myado.CloseSheet();
	return TRUE;
}

BOOL CUserSetting::UpdateUserToDb(CString Name, CString Password, CString Limits, BOOL ChangePw, CString NewPassword)
{
	if (1)//数据库地址不为空，就上传数据
	{
		
		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		if(LimitsSys.Find("&10") != -1)
			myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");
		else
			myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if ((ChangePw==TRUE) && (NewPassword != "") && (Barcode_Check != TRUE))//修改密码但原始密码错误
		{
			AfxMessageBox("请先输入正确原始账户密码！");
			return FALSE;
		}
		if (Barcode_Check == TRUE)
		{
			if (Limits == "")
			{
				AfxMessageBox("权限配置为空！");
				return FALSE;
			}
			CString Field="UserType";//字段

			CString strSQL_Write_Barcode;
			_variant_t var;
			if(LimitsSys.Find("&10") != -1)//超级管理权限,无需密码可以更改权限密码
			{
				myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");

				if ((ChangePw == TRUE) && (NewPassword != ""))
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET " + Field + " = '" + Limits + "',Password = '" + NewPassword + "' WHERE Name='" + Name + "'";
				}
				else
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET " + Field + " = '" + Limits + "' WHERE Name='" + Name + "'";
				}
			}
			else//非超级管理人员,需密码仅可更新密码
			{
				myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");

				if ((ChangePw == TRUE) && (NewPassword != ""))
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET Password = '" + NewPassword + "' WHERE Name='" + Name + "' AND Password='" + Password + "'";
				}
			}
			//更新
			
			UP_Barcode &= myado.Execute(strSQL_Write_Barcode, &var);

			myado.CloseSheet();
			if ((UP_Barcode != TRUE) || (var.intVal != 1))
			{
				AfxMessageBox("更新失败！");
				return FALSE;
			}
			//更新
		}
		else
		{
			if(LimitsSys.Find("&10") == -1)//当前账号不存在,需超级管理账号
			{
				AfxMessageBox("该账号没有超级管理权限");
				return FALSE;
			}
			//直接上传
			CString strSQL1 = "Insert into dbo.LUserAccount(Name,Password,UserType)\
							  							  				values('" + Name + "','" + Password + "','" + Limits + "')";


			myado.OpenSheet("select * from LUserAccount");

			_variant_t var;
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			myado.CloseSheet();
			if ((SqlResult != TRUE) || (var.intVal != 1))
			{
				AfxMessageBox("插入失败！");
				return FALSE;
			}
			//直接上传

		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			AfxMessageBox("设置成功！！！");
		}
		else
		{
			AfxMessageBox("设置失败！！！");
		}

	}
	return TRUE;
}


void CUserSetting::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	UpdateUserToDb(UserName, Password, Limits, TRUE, NewPassword);
}


void CUserSetting::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	if(LimitsSys.Find("&10") == -1)
	{
		AfxMessageBox("该账号没有超级管理权限");
		return;
	}
	DeleteUserFromDb(UserName, Password,FALSE);
}


BOOL CUserSetting::DeleteUserFromDb(CString Name, CString Password,BOOL UsePW)
{
	if (UsePW==TRUE)//使用密码
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			_variant_t var;
			CString strSQL1 = "DELETE FROM LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'";
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			if (SqlResult != TRUE)
			{
				AfxMessageBox("删除用户失败！");
				return FALSE;
			}
			else
			{
				AfxMessageBox("删除用户成功！");
			}
		}
		else
		{
			AfxMessageBox("用户不存在！");
			return FALSE;
		}
	}
	else
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			_variant_t var;
			CString strSQL1 = "DELETE FROM LUserAccount WHERE Name='" + Name + "'";
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			if (SqlResult != TRUE)
			{
				AfxMessageBox("删除用户失败！");
				return FALSE;
			}
			else
			{
				AfxMessageBox("删除用户成功！");
			}
		}
		else
		{
			AfxMessageBox("用户不存在！");
			return FALSE;
		}
	}
	return TRUE;
}

void CUserSetting::OnBnClickedButtonstart10()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString LimitsCS;
	LimitsControl.GetWindowTextA(LimitsCS);
	if (LimitsCS != "")
	{
		if (Limits.Find(LimitsCS.Left(2)) == -1)
		{
			Limits += ("&" + LimitsCS.Left(2));
		}
	}
	UpdateData(FALSE);
}


BOOL CUserSetting::GetUsersFromDB()
{
	if (1)//数据库地址不为空，就上传数据
	{
		myado.OpenSheet("select * from LUserAccount order by Name");

		CString temp;
		/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////
		try
		{
			myado.m_pRec->MoveFirst(); //myado.m_pRec->MoveLast();
			do
			{
				try
				{
					temp = "";
					for (int aa = 0; aa < 3; aa++)
					{
						CString str1, str2;
						_variant_t varTemp;
						//判断数据库中的NULL值
						varTemp = myado.m_pRec->GetCollect(_variant_t((long)aa));
						if (varTemp.vt == VT_NULL)
							str1 = "";
						else
							str1.Format("%s", (LPCSTR)(_bstr_t)varTemp);

						temp += (str1 + "@@");

					}
					UserArray.Add(temp);
					if (myado.adoEOF() == 0)
					{
						myado.MoveNext();
						if (myado.adoEOF() == -1)
							break;
					}
					else
					{
						break;
					}
					/*if (myado.BOF() == 0)
					{
					myado.m_pRec->MovePrevious();
					if (myado.BOF() == -1)
					break;
					}
					else
					{
					break;
					}*/
				}
				catch (_com_error e)
				{
					CATCH_ERROR;
					myado.CloseSheet();
					return FALSE;
				}

			} while (myado.adoEOF() == 0);
		}
		catch (_com_error e)
		{
			//CATCH_ERROR;
			myado.CloseSheet();
			return FALSE;
		}
		myado.CloseSheet();
	}
	return TRUE;
}

void CUserSetting::OnBnClickedButtonstart11()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString LimitsCS;
	LimitsControl.GetWindowTextA(LimitsCS);
	if (LimitsCS != "")
	{
		if (Limits.Find(LimitsCS.Left(2)) != -1)
		{
			Limits.Replace("&" + LimitsCS.Left(2), "");
		}
	}
	UpdateData(FALSE);
}


void CUserSetting::OnCbnSelchangeCombo2()
{
	// TODO:  在此添加控件通知处理程序代码
	SetTimer(0, 100, NULL);
}
void CUserSetting::ChangeCombo()
{
	CString AllUsersBoxCS;
	AllUsersBox.GetWindowTextA(AllUsersBoxCS);
	for (int i = 0; i < UserArray.GetCount(); i++)
	{
		CString strValue = UserArray.GetAt(i);
		CString user = GetData((LPSTR)(LPCTSTR)strValue, "NULL", "@@", 1);	//用户
		CString password = GetData((LPSTR)(LPCTSTR)strValue, "@@", "@@", 1);
		CString limits = GetData((LPSTR)(LPCTSTR)strValue, "@@", "@@", 2);
		if (user == AllUsersBoxCS)
		{
			UserName = user;
			Limits = limits;
			break;
		}
		
	}
	Password="";
	NewPassword="";
	UpdateData(FALSE);
}

BOOL CUserSetting::CheckUser(CString Name, CString Password, CString* Limits)
{
	if (1)//数据库地址不为空，就上传数据
	{

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)//存在账号
		{
			if (GetUserType(myado, Name, Password, Limits) == TRUE)
			{

			}
			else
			{
				return FALSE;
			}
				
		}
	}
	return TRUE;
}
void CUserSetting::OnTimer(UINT nIDEvent)//定时器
{
	CString LineZhiDanCS;
	switch (nIDEvent)//定时器同时触发后，会先执行完一个，另外一个会重复几次,最好办法是合并定时器
	{
	case 0:
		KillTimer(0);
		ChangeCombo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
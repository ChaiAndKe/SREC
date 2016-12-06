// SettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "SettingDialog.h"
#include "afxdialogex.h"


// CSettingDialog 对话框

IMPLEMENT_DYNAMIC(CSettingDialog, CDialogEx)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDialog::IDD, pParent)
	, m_Data_StartAddr(_T(""))
	, m_Data_EndAddr(_T(""))
	, m_Code_StartAddr(_T(""))
	, m_Code_EndAddr(_T(""))
{

}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SETTING_DATA_STARTADDR, m_Data_StartAddr);
	DDX_Text(pDX, IDC_EDIT_SETTING_DATA_ENDADDR, m_Data_EndAddr);
	DDX_Text(pDX, IDC_EDIT_SETTING_CODE_STARTADDR, m_Code_StartAddr);
	DDX_Text(pDX, IDC_EDIT_SETTING_CODE_ENDADDR, m_Code_EndAddr);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CSettingDialog::OnBnClickedButtonReload)
	ON_BN_CLICKED(IDCANCEL, &CSettingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSettingDialog 消息处理程序


BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OnBnClickedButtonReload();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!CStringToUINT(m_Data_StartAddr,m_Data_StartUint,_T("数据段起始地址")))
	{
		return;
	}
	if (!CStringToUINT(m_Data_EndAddr,m_Data_EndUint,_T("数据段结束地址")))
	{
		return;
	}
	if (!CStringToUINT(m_Code_StartAddr,m_Code_StartUint,_T("代码段起始地址")))
	{
		return;
	}
	if (!CStringToUINT(m_Code_EndAddr,m_Code_EndUint,_T("代码段结束地址")))
	{
		return;
	}

	if (m_Data_StartUint>m_Data_EndUint)
	{
		MessageBox(_T("数据段地址设置错误，起始地址必须小于或等于结束地址"),_T("警告"),MB_OK|MB_ICONWARNING);
		return;
	}
	if (m_Code_StartUint>m_Code_EndUint)
	{
		MessageBox(_T("代码段地址设置错误，起始地址必须小于或等于结束地址"),_T("警告"),MB_OK|MB_ICONWARNING);
		return;
	}

	CString AppName,startAddr,endAddr;
	CString path = _T(".//bootcfg.ini");
	CFileFind finder;
	if (!finder.FindFile(path))
	{
		MessageBox(_T("未找到配置文件"),_T("警告"),MB_OK|MB_ICONWARNING);
		return;
	}
	AppName = _T("DataSegment");
	startAddr = _T("startAddr");
	endAddr = _T("endAddr");
	DWORD l_errorD;
	CString l_errorStr;
	if(::WritePrivateProfileString(AppName,startAddr,m_Data_StartAddr,path) == 0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("写入失败,错误代码%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	if(::WritePrivateProfileString(AppName,endAddr,m_Data_EndAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("写入失败,错误代码%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	AppName = _T("CodeSegment");
	if(::WritePrivateProfileString(AppName,startAddr,m_Code_StartAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("写入失败,错误代码%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	if(::WritePrivateProfileString(AppName,endAddr,m_Code_EndAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("写入失败,错误代码%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	AfxMessageBox(_T("写入成功"));

	OnBnClickedCancel();
}

BOOL CSettingDialog::CStringToUINT(const CString &str,UINT &d,CString mess)
{
	int strLength = str.GetLength();
	if (strLength!=10)
	{
		MessageBox(mess+_T("长度错误，请输入4字节地址，共计8位"),_T("警告"),MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	UCHAR *addr = new UCHAR[strLength];
	addr[0] = (UCHAR)str.GetAt(0);
	addr[1] = (UCHAR)str.GetAt(1);
	if (addr[0]!='0' || !(addr[1]=='x' || addr[1]=='X'))
	{
		MessageBox(mess+_T("格式错误，必须以0x或0X开头"),_T("警告"),MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	for (int i=2;i<strLength;i++)
	{
		addr[i] = (UCHAR)str.GetAt(i);

		if (addr[i]>='0' && addr[i]<='9')
		{
			addr[i] -= 0x30;
		}
		else if (addr[i]>='A' && addr[i]<='F')
		{
			addr[i] -= 0x37;
		}else if (addr[i]>='a' && addr[i]<='f')
		{
			addr[i] -= 0x57;
		}else{
			MessageBox(mess+_T("格式错误，只支持\"0-9\",\"a-f\"以及\"A-F\"之间的字符，以0x开头x或0X开头"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}	

	for (int i=0;i<4;i++)
	{
		addr[i]= addr[2+2*i]<<4|addr[3+2*i];
	}

	d=addr[0]<<24|addr[1]<<16|addr[2]<<8|addr[3];

	return TRUE;
}

void CSettingDialog::OnBnClickedButtonReload()
{
	// TODO: 在此添加控件通知处理程序代码
	CString AppName,startAddr,endAddr;
	CString path = _T(".//bootcfg.ini");
	CFileFind finder;
	if (!finder.FindFile(path))
	{
		MessageBox(_T("未找到配置文件"),_T("警告"),MB_OK|MB_ICONWARNING);
	}
	AppName = _T("DataSegment");
	startAddr = _T("startAddr");
	endAddr = _T("endAddr");
	::GetPrivateProfileString(
		AppName,
		startAddr,
		_T("123"),
		m_Data_StartAddr.GetBuffer(MAX_PATH),
		MAX_PATH,
		path);
	::GetPrivateProfileString(
		AppName,
		endAddr,
		_T("123"),
		m_Data_EndAddr.GetBuffer(MAX_PATH),
		MAX_PATH,
		path);

	AppName = _T("CodeSegment");

	::GetPrivateProfileString(
		AppName,
		startAddr,
		_T("123"),
		m_Code_StartAddr.GetBuffer(MAX_PATH),
		MAX_PATH,
		path);
	::GetPrivateProfileString(
		AppName,
		endAddr,
		_T("123"),
		m_Code_EndAddr.GetBuffer(MAX_PATH),
		MAX_PATH,
		path);

	UpdateData(FALSE);
}


void CSettingDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

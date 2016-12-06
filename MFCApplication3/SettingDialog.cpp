// SettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "SettingDialog.h"
#include "afxdialogex.h"


// CSettingDialog �Ի���

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


// CSettingDialog ��Ϣ�������


BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OnBnClickedButtonReload();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSettingDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (!CStringToUINT(m_Data_StartAddr,m_Data_StartUint,_T("���ݶ���ʼ��ַ")))
	{
		return;
	}
	if (!CStringToUINT(m_Data_EndAddr,m_Data_EndUint,_T("���ݶν�����ַ")))
	{
		return;
	}
	if (!CStringToUINT(m_Code_StartAddr,m_Code_StartUint,_T("�������ʼ��ַ")))
	{
		return;
	}
	if (!CStringToUINT(m_Code_EndAddr,m_Code_EndUint,_T("����ν�����ַ")))
	{
		return;
	}

	if (m_Data_StartUint>m_Data_EndUint)
	{
		MessageBox(_T("���ݶε�ַ���ô�����ʼ��ַ����С�ڻ���ڽ�����ַ"),_T("����"),MB_OK|MB_ICONWARNING);
		return;
	}
	if (m_Code_StartUint>m_Code_EndUint)
	{
		MessageBox(_T("����ε�ַ���ô�����ʼ��ַ����С�ڻ���ڽ�����ַ"),_T("����"),MB_OK|MB_ICONWARNING);
		return;
	}

	CString AppName,startAddr,endAddr;
	CString path = _T(".//bootcfg.ini");
	CFileFind finder;
	if (!finder.FindFile(path))
	{
		MessageBox(_T("δ�ҵ������ļ�"),_T("����"),MB_OK|MB_ICONWARNING);
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
		l_errorStr.Format(_T("д��ʧ��,�������%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	if(::WritePrivateProfileString(AppName,endAddr,m_Data_EndAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("д��ʧ��,�������%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	AppName = _T("CodeSegment");
	if(::WritePrivateProfileString(AppName,startAddr,m_Code_StartAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("д��ʧ��,�������%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	if(::WritePrivateProfileString(AppName,endAddr,m_Code_EndAddr,path)==0)
	{
		l_errorD = GetLastError();
		l_errorStr.Format(_T("д��ʧ��,�������%ld"),l_errorD);
		AfxMessageBox(l_errorStr);
		return;
	}
	AfxMessageBox(_T("д��ɹ�"));

	OnBnClickedCancel();
}

BOOL CSettingDialog::CStringToUINT(const CString &str,UINT &d,CString mess)
{
	int strLength = str.GetLength();
	if (strLength!=10)
	{
		MessageBox(mess+_T("���ȴ���������4�ֽڵ�ַ������8λ"),_T("����"),MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	UCHAR *addr = new UCHAR[strLength];
	addr[0] = (UCHAR)str.GetAt(0);
	addr[1] = (UCHAR)str.GetAt(1);
	if (addr[0]!='0' || !(addr[1]=='x' || addr[1]=='X'))
	{
		MessageBox(mess+_T("��ʽ���󣬱�����0x��0X��ͷ"),_T("����"),MB_OK|MB_ICONWARNING);
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
			MessageBox(mess+_T("��ʽ����ֻ֧��\"0-9\",\"a-f\"�Լ�\"A-F\"֮����ַ�����0x��ͷx��0X��ͷ"),_T("����"),MB_OK|MB_ICONWARNING);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString AppName,startAddr,endAddr;
	CString path = _T(".//bootcfg.ini");
	CFileFind finder;
	if (!finder.FindFile(path))
	{
		MessageBox(_T("δ�ҵ������ļ�"),_T("����"),MB_OK|MB_ICONWARNING);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

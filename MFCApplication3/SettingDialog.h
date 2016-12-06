#pragma once


// CSettingDialog �Ի���

class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	BOOL CStringToUINT(const CString &str,UINT &d,CString mess);
	CString m_Data_StartAddr;
	CString m_Data_EndAddr;
	CString m_Code_StartAddr;
	CString m_Code_EndAddr;
	UINT m_Data_StartUint;
	UINT m_Data_EndUint;
	UINT m_Code_StartUint;
	UINT m_Code_EndUint;
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedCancel();
};

#pragma once


// CSettingDialog 对话框

class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

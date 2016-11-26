
// MFCApplication3Dlg.h : 头文件
//

#pragma once
#include "header.h"
#include "testlistbox.h"
#include "AnalysisFile.h"

// CMFCApplication3Dlg 对话框
class CMFCApplication3Dlg : public CDialogEx
{
	// 构造
public:
	CMFCApplication3Dlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//待实现函数
	int ConnectCan(int canType,int channel,int baudRate);
	int DisConnectCan();
	void SendOrder(const BaseType *d);
	
public:
	//全局函数
	void ShowInfo(CString str, int index = -1);
	void ShowErrMessageBox(CString err);
	BOOL GenerateSendOrder(char order,UCHAR len,const UCHAR *d,UINT addr=0);

private:
	BOOL CStringToUINT(const CString &str,UINT& d,CString mess);//地址转换成UINT格式

public:
	//全局变量
	CString filePathName;//srec文件路径
	CAnalysisFile *fileToWrite;//srec文件对象
	UINT startAddress,stopAddress;//写数据模式下的起始和结束地址
	UINT passWord;

	BOOL m_Connect;//CAN口的连接状态
	DWORD m_devtype;//CAN设备类型 added by kexf
	DWORD m_devind;//CAN设备索引号
	DWORD m_cannum;//CAN通道 
	CMenu m_Menu;
	CTestListBox m_ListInfo;

	BaseType* receiceData;
	BaseType* sendData1;
	BaseType* sendData2;

	HANDLE receiveEvent;
	HANDLE exitEvent1;
	HANDLE exitEvent2;


	int m_startFromMain;//是否从main开始启动

public:
	//线程
	static UINT ReceiveThread(void *param);
	static UINT SendThreadErase(void *param);
	static UINT SendThreadProgram(void *param);

public:
	//status bar
	HWND hStatusWindow;

public:
	afx_msg void OnMenuExit();
	afx_msg void OnMenuLoadRbl();
	afx_msg void OnMenuSaveRbl();
	afx_msg void OnBnClickedButtonConnectcan();
	afx_msg void OnBnClickedButtonFilebrowse();
	afx_msg void OnBnClickedButtonStartbootloader();
	afx_msg void OnBnClickedRadioWritedata();
	afx_msg void OnBnClickedRadioEraseflash();
	afx_msg void OnBnClickedRadioEraseandprogram();
	afx_msg void OnCbnSelchangeComboEncryption();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnClose();
};



// MFCApplication3Dlg.h : ͷ�ļ�
//

#pragma once
#include "header.h"
#include "testlistbox.h"
#include "AnalysisFile.h"

// CMFCApplication3Dlg �Ի���
class CMFCApplication3Dlg : public CDialogEx
{
	// ����
public:
	CMFCApplication3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//��ʵ�ֺ���
	int ConnectCan(int canType,int channel,int baudRate);
	int DisConnectCan();
	void SendOrder(const BaseType *d);
	
public:
	//ȫ�ֺ���
	void ShowInfo(CString str, int index = -1);
	void ShowErrMessageBox(CString err);
	BOOL GenerateSendOrder(char order,UCHAR len,const UCHAR *d,UINT addr=0);

private:
	BOOL CStringToUINT(const CString &str,UINT& d,CString mess);//��ַת����UINT��ʽ

public:
	//ȫ�ֱ���
	CString filePathName;//srec�ļ�·��
	CAnalysisFile *fileToWrite;//srec�ļ�����
	UINT startAddress,stopAddress;//д����ģʽ�µ���ʼ�ͽ�����ַ
	UINT passWord;

	BOOL m_Connect;//CAN�ڵ�����״̬
	DWORD m_devtype;//CAN�豸���� added by kexf
	DWORD m_devind;//CAN�豸������
	DWORD m_cannum;//CANͨ�� 
	CMenu m_Menu;
	CTestListBox m_ListInfo;

	BaseType* receiceData;
	BaseType* sendData1;
	BaseType* sendData2;

	HANDLE receiveEvent;
	HANDLE exitEvent1;
	HANDLE exitEvent2;


	int m_startFromMain;//�Ƿ��main��ʼ����

public:
	//�߳�
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


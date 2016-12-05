
// MFCApplication3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"

//CAN 通信应包含的头文件
#include "ControlCAN.h"
#pragma comment(lib,"controlcan.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 对话框



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressState);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_Menu_Exit, &CMFCApplication3Dlg::OnMenuExit)
	ON_COMMAND(ID_Menu_Load_RBL, &CMFCApplication3Dlg::OnMenuLoadRbl)
	ON_COMMAND(ID_Menu_Save_RBL, &CMFCApplication3Dlg::OnMenuSaveRbl)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTCAN, &CMFCApplication3Dlg::OnBnClickedButtonConnectcan)
	ON_BN_CLICKED(IDC_BUTTON_FILEBROWSE, &CMFCApplication3Dlg::OnBnClickedButtonFilebrowse)
	ON_BN_CLICKED(IDC_BUTTON_STARTBOOTLOADER, &CMFCApplication3Dlg::OnBnClickedButtonStartbootloader)
	ON_BN_CLICKED(IDC_RADIO_WRITEDATA, &CMFCApplication3Dlg::OnBnClickedRadioWritedata)
	ON_BN_CLICKED(IDC_RADIO_ERASEFLASH, &CMFCApplication3Dlg::OnBnClickedRadioEraseflash)
	ON_BN_CLICKED(IDC_RADIO_ERASEANDPROGRAM, &CMFCApplication3Dlg::OnBnClickedRadioEraseandprogram)
	ON_CBN_SELCHANGE(IDC_COMBO_ENCRYPTION, &CMFCApplication3Dlg::OnCbnSelchangeComboEncryption)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_DEFAULTPASSWORD, &CMFCApplication3Dlg::OnBnClickedCheckDefaultpassword)
	ON_COMMAND(ID_ABOUT, &CMFCApplication3Dlg::OnAbout)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 消息处理程序

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

#ifdef _DEBUG
	SetWindowText(_T("BootLoader_DEBUG_MODE"));
	//((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T("00000000"));
#else
	SetWindowText(_T("BootLoader"));
	//((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T(""));
#endif
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T("00000000"));

	//加载菜单栏
	
	m_Menu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_Menu);
	DrawMenuBar();
	

	//设置状态栏


	UINT arr[3] = {13597461,13597462,13597463};
	m_StatusBar.Create(this);
	m_StatusBar.SetIndicators(arr,sizeof(arr)/sizeof(UINT));
	CRect rect;
	GetClientRect(rect);
	
	m_StatusBar.SetPaneInfo(0,arr[0],0,rect.Width()/2);
	m_StatusBar.SetPaneText(0,_T("CAN口未连接"));

	m_StatusBar.SetPaneInfo(1,arr[1],0,60);
	m_StatusBar.SetPaneText(1,_T("下载0%"));

	m_StatusBar.SetPaneInfo(2,arr[2],0,(rect.Width()/2)-150);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,
		AFX_IDW_CONTROLBAR_LAST,
		0);
	RECT l_rect;
	m_StatusBar.GetItemRect(2,&l_rect);
	m_ProgressState.SetParent(&m_StatusBar);
	m_ProgressState.MoveWindow(&l_rect);
	m_ProgressState.ShowWindow(SW_SHOW);
	m_ProgressState.SetRange(0,100);
	m_ProgressState.SetPos(0);


	//设置默认值
	((CComboBox*)GetDlgItem(IDC_COMBO_CANTYPE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->SetCurSel(1);
	((CComboBox*)GetDlgItem(IDC_COMBO_MCU))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_ENCRYPTION))->SetCurSel(0);
	//0x00FE0000
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->SetWindowTextW(_T("0x00FE0000"));
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->SetWindowTextW(_T("0x00FF0000"));
//	((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T("0x"));
	((CButton *)GetDlgItem(IDC_RADIO_ERASEANDPROGRAM))->SetCheck(TRUE);
	//禁用地址
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->EnableWindow(FALSE);

	m_Connect = FALSE;
	canFrameCount = 0;

	//接收和发送数据缓冲区
	receiceData = new BaseType(8);
	sendData1 = new BaseType(8);
	sendData2 = new BaseType(24);

	//线程事件
	//receiveEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	//exitEvent1 = CreateEvent(NULL,FALSE,FALSE,NULL);
	//exitEvent2 = CreateEvent(NULL,FALSE,FALSE,NULL);

	filePathName = _T("");
	fileToWrite = NULL;
	startAddress = 0;
	stopAddress  = 0;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication3Dlg::OnMenuExit()
{
	TRACE("菜单---退出");
	exit(-1);
}


void CMFCApplication3Dlg::OnMenuLoadRbl()
{
	TRACE("菜单---加载");
}


void CMFCApplication3Dlg::OnMenuSaveRbl()
{
	TRACE("菜单---保存");
}


void CMFCApplication3Dlg::OnBnClickedButtonConnectcan()
{
	TRACE("连接CAN");

	int a= ((CComboBox*)GetDlgItem(IDC_COMBO_CANTYPE))->GetCurSel();
	int b= ((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->GetCurSel();
	int c= ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->GetCurSel();
	int returnCode;
	
	if (m_Connect)
	{
		//已连接，断开连接
		returnCode = DisConnectCan();
		m_Connect = FALSE;
		((CButton*)GetDlgItem(IDC_BUTTON_STARTBOOTLOADER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CONNECTCAN))->SetWindowTextW(_T("连接CAN"));

		m_StatusBar.SetPaneText(0,_T("CAN已断开"));
		
	}
	else
	{
		//未连接，开始连接
		//三个参数：CAN类型，通道号，波特率
		int returnCode = ConnectCan(a,b,c);
		switch(returnCode)
		{
		case CAN_CONNECT_OK:			
			m_Connect = TRUE;
			((CButton*)GetDlgItem(IDC_BUTTON_STARTBOOTLOADER))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_CONNECTCAN))->SetWindowTextW(_T("断开CAN"));
			//set the status bar
			m_StatusBar.SetPaneText(0,_T("CAN已连接"));
			break;
		case CAN_OPENDEV_ERROR:
		case CAN_INITDEV_ERROR:
		case CAN_STARTDEV_ERROR:
		case CAN_SETFILTER_ERROR:
			//MessageBox(_T("CAN连接失败"), _T("警告"), MB_OK);
			MessageBox(_T("CAN连接失败!"), _T("警告"), MB_OK|MB_ICONWARNING);
			break;

		default:
			//未知错误
			m_StatusBar.SetPaneText(0,_T("CAN未知错误"));
			break;
		}
	}
	
}


void CMFCApplication3Dlg::OnBnClickedButtonFilebrowse()
{	
	TRACE("文件浏览");
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
			 NULL, 
			 NULL,
			 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			 (LPCTSTR)_TEXT("SREC Files (*.srec)|*.srec|"),
			 //"SREC Files (*.srec)|*.srec|All Files (*.*)|*.*||"
			 NULL);

	TCHAR buffer[256];
	GetCurrentDirectory(256,buffer);
	dlg.m_ofn.lpstrInitialDir = buffer;

	if(dlg.DoModal()==IDOK)
	{
		filePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
		((CEdit*)GetDlgItem(IDC_EDIT_FILEPASS))->SetWindowTextW(filePathName);
	}
	else
	{
		return;
	}
}


void CMFCApplication3Dlg::OnBnClickedButtonStartbootloader()
{
	TRACE("启动BootLoader");
	/*
	1.获取密码，
	2.判断是否选择了文件
	3.获取操作模式
	4.获取起始和结束地址
	5.判断是否从main启动
	*/
	//擦除			boot,key,erase,boot_end,get_version
	//只写数据		boot,key,program,boot_end,get_version
	//擦除并编程  boot,key,program,boot_end,get_version
	
	//清空ListInfo和进度条
	ShowProgress(0);
	m_ListInfo.ResetContent();
	//step1.获取密码
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->GetWindowText(str);
	
	if (!CStringToUINT(str,passWord,_T("密码")))
	{
		return;
	}
	//密码已经产生，在passWord

	//step2.判断是否选择了文件
	if(!((CButton *)GetDlgItem(IDC_RADIO_ERASEFLASH))->GetCheck())
	{
		if (filePathName.IsEmpty())
		{
			MessageBox(_T("请选择SREC文件"),_T("警告"),MB_OK|MB_ICONWARNING);
			return;
		}else
		{
			if (NULL!=fileToWrite)
			{
				fileToWrite->Close();
				delete fileToWrite;
				fileToWrite=NULL;
			}
			if (NULL==fileToWrite)
			{
				fileToWrite = new CAnalysisFile(filePathName);
			}
			if (NULL==fileToWrite)
			{
				//AfxMessageBox(_T("文件打开失败，请重试！"));
				ShowInfo(_T("文件打开失败，请重试！"));
				//ShowInfo(_T("退出BootLoader"));
				return;
			}
			/*
			else if (fileToWrite->CheckSrecFile()==FILE_ADDRESS_ERROR)
			{
				MessageBox(_T("文件格式错误，请检查！"),_T("警告"),MB_OK|MB_ICONWARNING);
				ShowInfo(_T("文件格式错误，请检查！"));
				ShowInfo(_T("退出BootLoader"));
				fileToWrite->Close();
				delete fileToWrite;
				fileToWrite = NULL;
				return;
			}*/
			fileToWrite->SeekToBegin();
			//ShowInfo(_T("文件打开成功！"));
		}
	}

	//step3.获取操作模式
	//读取INI配置文件
	CString AppName,startAddr,endAddr;
	CString path = _T(".//bootcfg.ini");
	CFileFind finder;

	int l_writeData = ((CButton *)GetDlgItem(IDC_RADIO_WRITEDATA))->GetCheck();
	int l_erase = ((CButton *)GetDlgItem(IDC_RADIO_ERASEFLASH))->GetCheck();
	int l_program = ((CButton *)GetDlgItem(IDC_RADIO_ERASEANDPROGRAM))->GetCheck();
	m_startFromMain = ((CButton *)GetDlgItem(IDC_CHECK_STARTFROMMAIN))->GetCheck();
	if (1==l_writeData || 1==l_program)
	{
		CString str;
		if (!finder.FindFile(path))
		{
			//MessageBox(_T("未找到配置文件"),_T("警告"),MB_OK|MB_ICONWARNING);
			ShowInfo(_T("未找到配置文件！"));
			return;
		}
		else
		{
			if(1==l_writeData)
				AppName = _T("DataSegment");
			else if(1==l_program)
				AppName = _T("CodeSegment");

			startAddr = _T("startAddr");
			endAddr = _T("endAddr");
			startAddress = ::GetPrivateProfileIntW(AppName,startAddr,1,path);
			stopAddress = ::GetPrivateProfileIntW(AppName,endAddr,1,path);
			if (stopAddress==1||startAddress==1)
			{
				//MessageBox(_T("地址读取错误，请检查"),_T("警告"),MB_OK|MB_ICONWARNING);
				ShowInfo(_T("地址读取错误，请检查！"));
				return;
			}
		}
		/*
		if (startAddress%8!=0)
		{
			//MessageBox(_T("起始地址设置错误，地址必须为8的整数倍！"),_T("警告"),MB_OK|MB_ICONWARNING);
			ShowInfo(_T("起始地址设置错误，地址必须为8的整数倍！"));
			ShowInfo(_T("退出BootLoader"));
			return;
		}*/
		//设置边界
		try{
			if (!fileToWrite->SetArrange(FALSE,startAddress,stopAddress))
			{
				//MessageBox(_T("地址设置错误，请重试！"),_T("警告"),MB_OK|MB_ICONWARNING);
				ShowInfo(_T("文件地址设置错误，请重新配置！"));
				return;
			}
		}catch(char *s)
		{
			CString tmp(s);
			//MessageBox(tmp,_T("警告"),MB_OK|MB_ICONWARNING);
			ShowInfo(tmp);
			return;
		}


	}
	//启动线程
	AfxBeginThread(SendThread,this);
	GetDlgItem(IDC_BUTTON_STARTBOOTLOADER)->EnableWindow(FALSE);
	/*
	GetDlgItem(IDC_BUTTON_CONNECTCAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STARTBOOTLOADER)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_WRITEDATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_ERASEFLASH)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_ERASEANDPROGRAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_STARTFROMMAIN)->EnableWindow(FALSE);*/
}

BOOL CMFCApplication3Dlg::CStringToUINT(const CString &str,UINT& d,CString mess)
{
	int strLength = str.GetLength();
	if (strLength!=8)
	{
		MessageBox(mess+_T("长度错误，请输入4字节地址，共计8位"),_T("警告"),MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	UCHAR *addr = new UCHAR[strLength];
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
			//AfxMessageBox(mess+_T("格式错误，只支持\"0-9\",\"a-f\"以及\"A-F\"之间的字符，以0x开头"));
			MessageBox(mess+_T("格式错误，只支持\"0-9\",\"a-f\"以及\"A-F\"之间的字符，以0x开头"),_T("警告"),MB_OK|MB_ICONWARNING);
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

void CMFCApplication3Dlg::OnBnClickedCheckDefaultpassword()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton *)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->GetCheck())
	{
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T("00000000"));
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T(""));
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(TRUE);
	}
}

void CMFCApplication3Dlg::OnBnClickedRadioWritedata()
{
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_CHECK_STARTFROMMAIN))->EnableWindow(TRUE);
}


void CMFCApplication3Dlg::OnBnClickedRadioEraseflash()
{
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_CHECK_STARTFROMMAIN))->EnableWindow(FALSE);
}


void CMFCApplication3Dlg::OnBnClickedRadioEraseandprogram()
{
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_CHECK_STARTFROMMAIN))->EnableWindow(TRUE);
}


void CMFCApplication3Dlg::OnCbnSelchangeComboEncryption()
{
	TRACE("密码启用禁用");
	switch(((CComboBox*)GetDlgItem(IDC_COMBO_ENCRYPTION))->GetCurSel())
	{
	case 0:
		if(!((CButton *)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->GetCheck())
			((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(TRUE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->EnableWindow(TRUE);
		
		break;
	case 1:
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(FALSE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->EnableWindow(FALSE);
		break;
	}
	
}

int CMFCApplication3Dlg::ConnectCan(int typeIndex,int channel,int baudRateIndex)
{
	const unsigned char baudRate[5][2]={
		{0x00,0x14},//1000Kbps
		{0x00,0x1C},//500Kbps
		{0x01,0x1C},//250Kbps
		{0x03,0x1C},//125Kbps
		{0x04,0x1C}//100Kbps
	};
	const DWORD baudRate_E[5] = {
		0x060003, //1000Kbps
		//0x060004,//800Kbps
		0x060007,//500Kbps
		0x1C0008,//250Kbps
		0x1C0011,//125Kbps
		0x160023 //100Kbps
		//0x1C002C, //20Kbps
		//0x1600B3, //20Kbps
		//0x1C00E0,//10Kbps
		//0x1C01C1//5Kbps
	};
	const unsigned int devtype[2]={
		VCI_USBCAN2,
		VCI_USBCAN_2E_U
	};
	DWORD devindex = 0;
	DWORD baud_E = 0;
	VCI_INIT_CONFIG init_config;
	VCI_FILTER_RECORD filterRecord;	//for USBCAN-2E-U

	if(0 == typeIndex)//for USBCAN　exclude USBCAN-2E-U series
	{
		init_config.AccCode = 0x00000000;//验收码
		init_config.AccMask = 0xFFFFFFFF;//屏蔽码
		init_config.Timing0 = baudRate[baudRateIndex][0];//设置波特率
		init_config.Timing1 = baudRate[baudRateIndex][1];
		init_config.Filter = 1;//单滤波
		init_config.Mode = 0;//正常模式
	}
	else if(1 == typeIndex)//for USBCAN USBCAN-2E-U series
	{
		init_config.Mode = 0;//正常模式
		baud_E = baudRate_E[baudRateIndex];
		filterRecord.Start = 0x00000000;
		filterRecord.End = 0x00000000;
	}

	m_devtype = devtype[typeIndex];
	m_devind = 0;//CAN设备索引号
	m_cannum = channel;//

	if(VCI_OpenDevice(m_devtype, m_devind, 0) != STATUS_OK)
	{
		//MessageBox(_T("打开设备失败!"),_T("警告"), MB_OK);
		return CAN_OPENDEV_ERROR;
	}
	if(1 == typeIndex)//for USBCAN-2E-U, VCI_SetReference should be called to init the baud 
	{
		if (VCI_SetReference(m_devtype,m_devind, m_cannum, 0, &baud_E) != STATUS_OK)
		{
			//MessageBox(_T("设置波特率错误!"), _T("警告"), MB_OK);
			VCI_CloseDevice(m_devtype,m_devind);
			return CAN_OPENDEV_ERROR;
		}
		VCI_SetReference(m_devtype, m_devind, m_cannum, 1, &filterRecord);//填充滤波表格
		if (VCI_SetReference(m_devtype, m_devind, m_cannum, 2, NULL) != STATUS_OK)//使滤波表格生效
		{
			//MessageBox(_T("设置滤波失败!"), _T("警告"), MB_OK | MB_ICONQUESTION);
			//MessageBox(_T("设置滤波失败!"), _T("警告"), MB_OK);
			VCI_CloseDevice(m_devtype,m_devind);
			return CAN_SETFILTER_ERROR;
		}
	}
	if(VCI_InitCAN(m_devtype, m_devind, m_cannum, &init_config) !=STATUS_OK)
	{
		//MessageBox(_T("初始化CAN失败!"),_T("警告"), MB_OK);
		VCI_CloseDevice(m_devtype, m_devind);
		return CAN_INITDEV_ERROR;
	}
	if(VCI_StartCAN(m_devtype,m_devind,m_cannum) != 1)
	{
		//MessageBox(_T("启动CAN失败!"),_T("警告"), MB_OK);
		return CAN_STARTDEV_ERROR;
	}
	/*
	if(1 == typeIndex)//for USBCAN-2E-U, VCI_SetReference should be called to set the filter
	{
		VCI_SetReference(m_devtype, m_devind, m_cannum, 1, &filterRecord);//填充滤波表格
		if (VCI_SetReference(m_devtype, m_devind, m_cannum, 2, NULL) != STATUS_OK)//使滤波表格生效
		{
			MessageBox(_T("设置滤波失败!"), _T("警告"), MB_OK | MB_ICONQUESTION);
			VCI_CloseDevice(m_devtype,m_devind);
			return CAN_SETFILTER_ERROR;
		}
	}*/
	
	return CAN_CONNECT_OK;
}
int CMFCApplication3Dlg::DisConnectCan()
{
	if(m_Connect)
	{
		Sleep(500);//断开连接之前先休眠
		VCI_CloseDevice(m_devtype,m_devind);
	}
	return CAN_DISCONNECT_OK;
}
void CMFCApplication3Dlg::ShowProgress(int percent)
{
	CString tmp;
	//tmp.Format(_T("下载 %d"),fileToWrite->GetSendedPercent());
	tmp.Format(_T("下载 %d"),percent);
	tmp+="%";
	::SendMessage(m_StatusBar.m_hWnd, SB_SETTEXT, 1, (LPARAM)tmp.GetBuffer());
	//m_StatusBar.SetPaneText(1,tmp);
	//m_ProgressState.SetPos(fileToWrite->GetSendedPercent());
	m_ProgressState.SetPos(percent);
}
void CMFCApplication3Dlg::ShowInfo(CString str, int index/*=-1*/)
{
	CString strInfo;
	/*
	SYSTEMTIME st;
	GetLocalTime(&st);
	strInfo.Format(_T("%d-%d-%d %2d:%2d:%2d "),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	strInfo += str;*/
	strInfo = str;
	if(index == -1)//在m_ListInfo末尾插入
	{
		m_ListInfo.InsertString(m_ListInfo.GetCount(),strInfo);
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
	else
	{
		m_ListInfo.DeleteString(index);
		m_ListInfo.InsertString(index, strInfo);
		m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
	}
}
void CMFCApplication3Dlg::ShowErrMessageBox(CString err)
{
	AfxMessageBox(err);
}

BOOL CMFCApplication3Dlg::GenerateSendOrder( char order,UCHAR len,const UCHAR *d,UINT addr/*=0*/ )
{
	UINT random;
	switch(order)
	{
	case ORDER_BOOT:
		sendData1->SetData(ORDER_BOOT,len,passWord);
		break;
	case ORDER_KEY:
		srand((UINT)time(NULL)); 
		random = (UINT)(rand());
		receiceData->random = random;
		sendData1->SetData(ORDER_KEY,len,random);
		break;
	case ORDER_ERASE:
		sendData2->SetData(ORDER_ERASE,len,0,NULL);
		break;
	case ORDER_BOOTEND:
		sendData2->SetData(ORDER_BOOTEND,len,0,NULL);
		break;
	case ORDER_GETVERSION:
		sendData2->SetData(ORDER_GETVERSION,len,0,NULL);
		break;
	case ORDER_MAINSTART:
		sendData2->SetData(ORDER_MAINSTART,len,addr,NULL);
		break;
	case ORDER_PROGRAM:
		sendData2->SetData(ORDER_PROGRAM,len,addr,d);
		break;
	case ORDER_PROGDATA:
		sendData2->SetData(ORDER_PROGDATA,len,addr,d);
		break;
	case ORDER_SPERASE:
		sendData2->SetData(ORDER_SPERASE,len,addr,d);
		break;
	}

	return TRUE;
}

BOOL CMFCApplication3Dlg::SendOrder(const BaseType *sendframe)
{
	VCI_CAN_OBJ canframe[3];

#ifdef _SIMULATOR
	canframe[0].SendType = 2;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
	canframe[1].SendType = 2;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
	canframe[2].SendType = 2;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
#else
	canframe[0].SendType = 0;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
	canframe[1].SendType = 0;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
	canframe[2].SendType = 0;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
#endif
	canframe[0].ExternFlag = 0;//0:标准帧,1:扩展帧
	canframe[0].RemoteFlag = 0;//禁用远程帧
	canframe[0].DataLen = 8;

	canframe[1].ExternFlag = 0;//0:标准帧,1:扩展帧
	canframe[1].RemoteFlag = 0;//禁用远程帧
	canframe[1].DataLen = 8;

	canframe[2].ExternFlag = 0;//0:标准帧,1:扩展帧
	canframe[2].RemoteFlag = 0;//禁用远程帧
	canframe[2].DataLen = 8;
	

	if((ORDER_BOOT == sendframe->command) || (ORDER_KEY == sendframe->command))//帧格式1
	{
		if(sendframe->totalLength != 8)
		{
			//TODO:数据长度异常
			ShowInfo(_T("CAN发送数据长度异常"));
			isTransmitOK = FALSE;
		}
		else
		{
			canframe[0].ID = MSGID_FARME1;//帧格式1 ID
			memcpy(canframe[0].Data, sendframe->allData, 8);

			isTransmitOK = FALSE;
			VCI_ClearBuffer(m_devtype, m_devind, m_cannum);//发送之前清除接收buffer

			if(1 == VCI_Transmit(m_devtype, m_devind, m_cannum, canframe, 1))//发送成功
			{
				isTransmitOK = TRUE;
				canFrameCount += 2;

#ifdef _MONITOR
				CString str,tmpstr;
	
				str = "";
				tmpstr.Format(_T("发送帧ID:%08x "),canframe[0].ID);
				str += tmpstr;
				tmpstr = " 数据：";
				str += tmpstr;
				for(int j = 0; j < canframe[0].DataLen; j++)
				{
					tmpstr.Format(_T("%02x "),canframe[0].Data[j]);
					str += tmpstr;
				}
				ShowInfo(str);
#endif
			}
			else
			{
				//TODO：发送失败
				ShowInfo(_T("CAN发送失败"));
				isTransmitOK = FALSE;
			}
		}
	}
	else//帧格式2
	{
		if(sendframe->totalLength != 24)
		{
			//TODO:数据长度异常
			ShowInfo(_T("CAN发送数据长度异常"));
			isTransmitOK = FALSE;
		}
		else
		{
			canframe[0].ID = MSGID_FARME2[0];
			canframe[1].ID = MSGID_FARME2[1];
			canframe[2].ID = MSGID_FARME2[2];
			memcpy(canframe[0].Data, &(sendframe->allData[0*8]), 8);
			memcpy(canframe[1].Data, &(sendframe->allData[1*8]), 8);
			memcpy(canframe[2].Data, &(sendframe->allData[2*8]), 8);
			
			isTransmitOK = FALSE;
			VCI_ClearBuffer(m_devtype, m_devind, m_cannum);//发送之前清除接收buffer

			if(3 == VCI_Transmit(m_devtype, m_devind, m_cannum, canframe, 3))//发送成功
			{
				isTransmitOK = TRUE;
				canFrameCount += 4;
#ifdef _MONITOR
				CString str,tmpstr;
				for(int i = 0; i < 3; i++)
				{
					str = "";
					tmpstr.Format(_T("发送帧ID:%08x "),canframe[i].ID);
					str += tmpstr;
					tmpstr = " 数据：";
					str += tmpstr;
					for(int j = 0; j < canframe[i].DataLen; j++)
					{
						tmpstr.Format(_T("%02x "),canframe[i].Data[j]);
						str += tmpstr;
					}
					ShowInfo(str);
				}
#endif
			}
			else
			{
				//TODO：一次发送失败，跳出 or 重试 is a question
				ShowInfo(_T("CAN发送失败"));
				isTransmitOK = FALSE;
			}
			
		}
	}
	return isTransmitOK;
}

BOOL CMFCApplication3Dlg::ReceiveOrderInMs(UINT timeOut)
{
	VCI_ERR_INFO errinfo;
	int len = 1;
	int receiveBufLen = 0;
	int i = 0;
	long startTick = 0;
	long endTick = 0;
	UINT waitTick = 0;

	while(isTransmitOK != TRUE);
	if(FALSE == m_Connect)
	{
		return FALSE;
	}
	startTick = GetTickCount();
	endTick = startTick;
	waitTick = endTick - startTick;
	
	while(waitTick < timeOut)//读CAN缓冲区超时判断
	{
		receiveBufLen = VCI_GetReceiveNum(m_devtype,m_devind,m_cannum);
#ifdef _SIMULATOR
		if(sendThreadState == ORDER_BOOT || sendThreadState == ORDER_KEY)
		{
			if(receiveBufLen > 0)
			{
				break;
			}
		}
		else
		{
			if(receiveBufLen == 3)
			{
				break;
			}
		}
#else
		if(receiveBufLen > 0) 
		{
			break;
		}
#endif

		endTick = GetTickCount();
		if(endTick < startTick)
		{
			waitTick = 0xFFFFFFFF - startTick + endTick;
		}
		else
		{
			waitTick = endTick - startTick;
		}
	}
	if(receiveBufLen <= 0)
	{
		//ShowInfo(_T("receiveBufLen <= 0"));
		return FALSE;
	}
	/*
	while(1)
	{
		receiveBufLen = VCI_GetReceiveNum(m_devtype,m_devind,m_cannum);
		if(receiveBufLen > 0) break;
	}*/

	PVCI_CAN_OBJ frameinfo = new VCI_CAN_OBJ[receiveBufLen];
	len = VCI_Receive(m_devtype, m_devind, m_cannum, frameinfo, receiveBufLen, 1);
	if(len <= 0)
	{
		//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
		//千万不能省略这一步（即使你可能不想知道错误码是什么）
		VCI_ReadErrInfo(m_devtype, m_devind, m_cannum, &errinfo);
		//ShowInfo(_T("len <= 0"));
		return FALSE;
	}
	else
	{
#ifndef _SIMULATOR
		for(i = 0; i < len; i++)
		{
			if(frameinfo[i].RemoteFlag != 0)
			{
				//TODO:出现了远程帧
				ShowInfo(_T("出现远程帧"));
			}
			else if((frameinfo[i].ID == MSGID_FRAMEREV) && (frameinfo[i].DataLen == 8))//接收正确的帧ID
			{
				receiceData->SetAllData((const char *)(frameinfo[i].Data), 8);
				if((receiceData->Calculate_Check() == frameinfo[i].Data[7]) && (receiceData->allData[0] == 0xA5))
				{
					UINT returnData = 0;
					receiceData->startSign = frameinfo[i].Data[0];
					receiceData->returnValue = frameinfo[i].Data[1];
					receiceData->dataLength = frameinfo[i].Data[2];
					//采用big-endian接收下位机返回的数据
					returnData = (frameinfo[i].Data[3]<<24)|frameinfo[i].Data[4]<<16|frameinfo[i].Data[5]<<8|frameinfo[i].Data[6];
					receiceData->returnData = returnData; //*((UINT *)(&(frameinfo[i].Data[3])));
					
					receiceData->m_check = frameinfo[i].Data[7];
					//SetEvent(receiveEvent);//设置接收事件
				}
				else
				{
					//接收数据帧错误
					memset(receiceData->allData, 0x00, receiceData->totalLength);
					receiceData->returnValue = 0x00;
				}		
			}
		}

		return TRUE;
#endif
	}
#if (defined _MONITOR) //&& (!(defined _SIMULATOR))
		{
			CString str,tmpstr;
			for(i = 0; i < len; i++)
			{
				str = "";
				tmpstr.Format(_T("接收帧ID:%08x "),frameinfo[i].ID);
				str += tmpstr;
				tmpstr = " 数据：";
				str += tmpstr;
				
				for(int j = 0; j < frameinfo[i].DataLen; j++)
				{
					tmpstr.Format(_T("%02x "),frameinfo[i].Data[j]);
					str += tmpstr;
				}
				//tmpstr.Format(_T("时间:%08x "),frameinfo[i].TimeStamp);
				//str += tmpstr;
				tmpstr.Format(_T("等待时间 %d ms"),waitTick);
				str += tmpstr;
				tmpstr.Format(_T("接收 %d帧数据"),len);
				str += tmpstr;
				ShowInfo(str);
			}
		}
#endif
#ifdef _SIMULATOR
		int FrameNum = 0;// for frame type 2
		BOOL FrameStart = FALSE;// for frame type 2
		UCHAR FrameOrder = 0;
		for(i = 0;i < len; i++)
		{
			if(FrameStart)// for frame type 2
			{
				FrameNum++;
				if(FrameNum == 3)
				{
					if(FrameOrder == ORDER_PROGRAM)
					{
						receiceData->returnValue = PROGRAM_OK;
						//Sleep(20);		
					}
					else if(FrameOrder == ORDER_ERASE)
					{
						receiceData->returnValue = ERASE_OK;
					}
					else if(FrameOrder == ORDER_GETVERSION)
					{
						receiceData->returnValue = GETVERSION_OK;
					}
					else if(FrameOrder == ORDER_MAINSTART)
					{
						receiceData->returnValue = MAINSTART_OK;
					}
					else if(FrameOrder == ORDER_BOOTEND)
					{
						receiceData->returnValue = BOOTEND_OK;
					}
					else if(FrameOrder == ORDER_PROGDATA)
					{
						receiceData->returnValue = PROGDATA_OK;
					}
					else if(FrameOrder == ORDER_SPERASE)
					{
						receiceData->returnValue = SPERASE_OK;
						FrameOrder = 0;
					}
					FrameOrder = 0;
					FrameNum = 0;
					FrameStart = FALSE;
					return TRUE;				
					//SetEvent(receiveEvent);//设置接收事件
				}
			}
			else if(FrameStart == FALSE && ('$' == frameinfo[i].Data[0]))
			{
				if(ORDER_BOOT == frameinfo[i].Data[1])
				{
					receiceData->returnValue = PASSWORD_OK;
					return TRUE;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_KEY == frameinfo[i].Data[1])
				{
					receiceData->returnValue = KEY_OK;
					return TRUE;
					//SetEvent(receiveEvent);//设置接收事件
				}
				else if(ORDER_ERASE == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_ERASE;
					//dlg->receiceData->returnValue = ERASE_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_PROGRAM == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_PROGRAM;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_GETVERSION == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_GETVERSION;
					//dlg->receiceData->returnValue = GETVERSION_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_MAINSTART == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_MAINSTART;
					//dlg->receiceData->returnValue = MAINSTART_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_BOOTEND == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_BOOTEND;
					//dlg->receiceData->returnValue = BOOTEND_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_PROGDATA == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_PROGDATA;
					//dlg->receiceData->returnValue = BOOTEND_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
				else if(ORDER_SPERASE == frameinfo[i].Data[1])
				{
					FrameStart = TRUE;
					FrameNum = 1;
					FrameOrder = ORDER_SPERASE;
					//dlg->receiceData->returnValue = BOOTEND_OK;
					//SetEvent(dlg->receiveEvent);//设置接收事件
				}
			}
		}
#endif	
}


BOOL CMFCApplication3Dlg::OrderBoot()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_BOOT,4,NULL))
		{
			return FALSE;
		}

		if(!SendOrder(sendData1))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case PASSWORD_OK:
					ShowInfo(_T("密码正确"));
					return TRUE;
				break;
				case PASSWORD_NOTOK:
					//ShowErrMessageBox(_T("密码错误,请重新输入密码"));
					MessageBox(_T("密码错误,请重新输入密码"),_T("警告"),MB_OK|MB_ICONWARNING);
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderKey()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_KEY,4,NULL))
		{
			return  FALSE;
		}
		if(!SendOrder(sendData1))
		{
			return FALSE;
		}
		
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE) 
		{
			UINT l_key = 0;
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case KEY_OK:
#ifndef _SIMULATOR
					//上位机判断校验是否匹配
					l_key = CalculateKey(receiceData->random);
					if (l_key != receiceData->returnData)
					{
						//KEY校验未通过，退出
						//ShowErrMessageBox(_T("KEY校验错误"));
						ShowInfo(_T("KEY校验错误"));
						return  FALSE;
					}
					else
#endif
					{	
						ShowInfo(_T("KEY校验通过"));
						return TRUE;
					}
				break;
				case KEY_NOTOK:
					//ShowErrMessageBox(_T("KEY校验错误"));
					ShowInfo(_T("KEY校验错误"));
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}//for switch
		}
		else
		{
			//超时，提示并退出
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while ((i < RETRY_TIMES) && (!exitSign));

	if ((i==RETRY_TIMES) && !exitSign)
	{
			//连接不稳定，退出
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderErase()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_ERASE,0,NULL))
		{
			return FALSE;
		}

		if(!SendOrder(sendData2))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case ERASE_OK:
					ShowInfo(_T("擦除完成"));
					return TRUE;
				break;
				case ERASE_NOTOK:
					//ShowErrMessageBox(_T("擦除flash失败"));
					MessageBox(_T("擦除flash失败"),_T("警告"),MB_OK|MB_ICONWARNING);
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		//ShowErrMessageBox(_T("连接不稳定"));
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderSPErase()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_SPERASE,0,NULL))
		{
			return FALSE;
		}

		if(!SendOrder(sendData2))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case SPERASE_OK:
					ShowInfo(_T("擦除完成"));
					return TRUE;
				break;
				case SPERASE_NOTOK:
					//ShowErrMessageBox(_T("擦除flash失败"));
					MessageBox(_T("擦除flash失败"),_T("警告"),MB_OK|MB_ICONWARNING);
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		//ShowErrMessageBox(_T("连接不稳定"));
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderProgram()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	int a = FILE_READ_NORMAL;

#ifndef _MONITOR
	DWORD frameNum = 0; 
	BOOL startProgram = FALSE;
	int listIndex = -1;
#endif

	while(a == FILE_READ_NORMAL)
	{
		a = fileToWrite->ReadNextLine();
		switch(a)
		{
			case FILE_READ_NORMAL:
			i = 0;
			do 
			{
				i++;
				if(!GenerateSendOrder(
								ORDER_PROGRAM,
								fileToWrite->GetDataSendLength(),
								fileToWrite->GetDataSend(),
								fileToWrite->GetDataSendAddr()))
				{
					return FALSE;
				}
				
				if(!SendOrder(sendData2))
				{
					//ShowInfo(_T("发送命令失败"));
					return FALSE;
				}

				if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
				{
					//收到数据，判断数据是否正确
					switch(receiceData->returnValue)
					{
						case PROGRAM_OK:
							exitSign = TRUE;
							ShowProgress(fileToWrite->GetSendedPercent());
#ifndef _MONITOR
							/*
							if(frameNum % 4 == 0) strListInfo = "正在烧写.";
							else if(frameNum % 4 == 1) strListInfo = "正在烧写..";
							else if(frameNum % 4 == 2) strListInfo = "正在烧写...";
							else if(frameNum % 4 == 3) strListInfo = "正在烧写....";
							if(!startProgram)
							{
								listIndex = m_ListInfo.GetCount();
								startProgram = TRUE;
							}
							ShowInfo(strListInfo, listIndex);
							frameNum++;
							*/
							
							if(!startProgram)
							{
								strListInfo = "正在烧写....";
								ShowInfo(strListInfo);
								startProgram = TRUE;
							}

#endif
						break;
						case PROGRAM_NOTOK:
						case DATA_ERR:
							strListInfo.Format(_T("本帧数据烧写失败，正在第%d次重试"), i);
							ShowInfo(strListInfo);
							exitSign = FALSE;
						break;
						case ADDRALIGN_ERR:
							ShowInfo(_T("文件地址对齐错误，无法写入"));
							return FALSE;
						break;
					}
				}
				else
				{
					//超时，提示并退出			
					//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
					MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
					return FALSE;
				}

			} while (i<RETRY_TIMES && !exitSign);

			if (i==RETRY_TIMES&&exitSign==FALSE)
			{
				//连接不稳定，退出
				ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
				return FALSE;
			}
			break;
			case FILE_READ_ERROR:
				//读取错误
				ShowInfo(_T("文件读取错误，停止烧写"));
				return FALSE;
			break;
			case FILE_READ_END:
					//文件结束
				ShowProgress(fileToWrite->GetSendedPercent());
				ShowInfo(_T("烧写完成"));//TODO:文件读取结束最后一帧发送完成才是烧写完成
				return TRUE;
			break;
		}//for switch(a)
	}//for while
}

BOOL CMFCApplication3Dlg::OrderProgData()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	int a = FILE_READ_NORMAL;

#ifndef _MONITOR
	static DWORD frameNum = 0; 
	static BOOL startProgram = FALSE;
	static int listIndex = -1;
#endif

	while(a == FILE_READ_NORMAL)
	{
		a = fileToWrite->ReadNextLine();
		switch(a)
		{
			case FILE_READ_NORMAL:
			i = 0;
			do 
			{
				i++;
				if(!GenerateSendOrder(
								ORDER_PROGDATA,
								fileToWrite->GetDataSendLength(),
								fileToWrite->GetDataSend(),
								fileToWrite->GetDataSendAddr()))
				{
					return FALSE;
				}
				
				if(!SendOrder(sendData2))
				{
					return FALSE;
				}
				if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
				{
					//收到数据，判断数据是否正确
					switch(receiceData->returnValue)
					{
						case PROGDATA_OK:
							exitSign = TRUE;
							ShowProgress(fileToWrite->GetSendedPercent());
#ifndef _MONITOR
							/*
							if(frameNum % 4 == 0) tmp = "正在烧写.";
							else if(frameNum % 4 == 1) tmp = "正在烧写..";
							else if(frameNum % 4 == 2) tmp = "正在烧写...";
							else if(frameNum % 4 == 3) tmp = "正在烧写....";*/
							strListInfo = "正在烧写....";
						
							if(!startProgram)
							{
								listIndex = m_ListInfo.GetCount();
								startProgram = TRUE;
								ShowInfo(strListInfo, listIndex);
							}
							//dlg->ShowInfo(tmp, listIndex);
							//frameNum++;
#endif
						break;
						case PROGDATA_NOTOK:
						case DATA_ERR:
							strListInfo.Format(_T("本帧数据烧写失败，正在第%d次重试"), i);
							ShowInfo(strListInfo);
							exitSign = FALSE;
						break;
						case ADDRALIGN_ERR:
							ShowInfo(_T("文件地址对齐错误，无法写入"));
							return FALSE;
						break;
					}
				}
				else
				{
					//超时，提示并退出			
					//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
					MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
					return FALSE;
				}

			} while (i<RETRY_TIMES && !exitSign);

			if (i==RETRY_TIMES&&exitSign==FALSE)
			{
				//连接不稳定，退出
				ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
				return FALSE;
			}
			break;
			case FILE_READ_ERROR:
				//读取错误
				ShowInfo(_T("读取错误，停止发送"));
				return FALSE;
			break;
			case FILE_READ_END:
				//文件结束
				ShowProgress(fileToWrite->GetSendedPercent());
				ShowInfo(_T("烧写完成"));//TODO:文件读取结束最后一帧发送完成才是烧写完成
				return TRUE;
			break;
		}//for switch(a)
	}//for while
}


BOOL CMFCApplication3Dlg::OrderBootEnd()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_BOOTEND,0,NULL))
		{
			return FALSE;
		}

		if(!SendOrder(sendData2))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case BOOTEND_OK:
					//在6中显示数据传输错误次数：x(data中第一字节)，flash写入失败次数，y(data中第二字节)
					strListInfo.Format(_T("数据传输错误次数:%d次\n flash写入失败的次数:%d次"),receiceData->allData[3],receiceData->allData[4]);
					
					//tmp1.Format(_T(" flash写入失败的次数:%d次"),receiceData->allData[4]);
					//tmp+= tmp1;
					ShowInfo(strListInfo);
					return TRUE;
				break;
				case BOOTEND_NOTOK:
					ShowInfo(_T("退出Boot失败，执行GetVersion命令"));
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		//ShowErrMessageBox(_T("连接不稳定"));
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderGetVersion()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(ORDER_GETVERSION,0,NULL))
		{
			return FALSE;
		}

		if(!SendOrder(sendData2))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case GETVERSION_OK:
					strListInfo.Format(_T("BootLoader版本为:%d.%d"),receiceData->allData[3],receiceData->allData[4]);
					ShowInfo(strListInfo);
					return TRUE;
				break;
				case GETVERSION_NOTOK:
					ShowInfo(_T("获取BootLoader版本失败"));
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		//ShowErrMessageBox(_T("连接不稳定"));
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}

BOOL CMFCApplication3Dlg::OrderMainStart()
{
	int i = 0;
	BOOL exitSign = FALSE;
	CString strListInfo;
	do 
	{
		i++;
		if(!GenerateSendOrder(
				ORDER_MAINSTART,
				0,
				NULL,
				fileToWrite->GetMainStartAddr()))
		{
			return FALSE;
		}

		if(!SendOrder(sendData2))
		{
			return FALSE;
		}
				
		if(ReceiveOrderInMs(ACK_TIMEOUT) == TRUE)
		{
			//收到数据，判断数据是否正确
			switch(receiceData->returnValue)
			{
				case MAINSTART_OK:
					ShowInfo(_T("主函数进入成功"));
					return TRUE;
				break;
				case MAINSTART_NOTOK:
					ShowInfo(_T("主函数进入不成功"));
					return FALSE;
				break;
				case DATA_ERR:
					strListInfo.Format(_T("数据传输校验错误,正在进行第%d次重试"),i);
					ShowInfo(strListInfo);
					exitSign = FALSE;
				break;
				default:
					ShowInfo(_T("未定义的返回值"));
					exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
			//ShowErrMessageBox(_T("下位机响应超时，停止烧写"));
			MessageBox(_T("下位机响应超时，请重试"),_T("警告"),MB_OK|MB_ICONWARNING);
			return FALSE;
		}

	} while (i < RETRY_TIMES && !exitSign );

	if ((i == RETRY_TIMES) && !exitSign)
	{
		//ShowErrMessageBox(_T("连接不稳定"));
		ShowInfo(_T("数据传输错误多次重试失败，请重新操作"));
		return FALSE;
	}
}


UINT CMFCApplication3Dlg::SendThread( void *param )
{
	CMFCApplication3Dlg* dlg = (CMFCApplication3Dlg*)param;

	int j;
	UCHAR state = 0;//记录当前thread运行的state
	UCHAR orderList[10];//
	//记录烧写时间
	CString str;
	long tickStart=0;
	long tickEnd=0;

	tickStart = GetTickCount();

	memset(orderList, 0, sizeof(orderList)*sizeof(UCHAR));

	if(((CButton *)dlg->GetDlgItem(IDC_RADIO_ERASEANDPROGRAM))->GetCheck())//擦除并编程
	{
		orderList[0] = ORDER_BOOT;
		orderList[1] = ORDER_KEY;
		orderList[2] = ORDER_ERASE;
		orderList[3] = ORDER_PROGRAM;
		orderList[4] = ORDER_BOOTEND;
		orderList[5] = ORDER_GETVERSION;
		if(((CButton *)dlg->GetDlgItem(IDC_CHECK_STARTFROMMAIN))->GetCheck())//从main开始执行
		{
			orderList[6] = ORDER_MAINSTART;
		}
	}
	else if(((CButton *)dlg->GetDlgItem(IDC_RADIO_WRITEDATA))->GetCheck())//只写数据
	{
		orderList[0] = ORDER_BOOT;
		orderList[1] = ORDER_KEY;
		orderList[2] = ORDER_SPERASE;
		orderList[3] = ORDER_PROGDATA;
		orderList[4] = ORDER_BOOTEND;
		orderList[5] = ORDER_GETVERSION;
		if(((CButton *)dlg->GetDlgItem(IDC_CHECK_STARTFROMMAIN))->GetCheck())//从main开始执行
		{
			orderList[6] = ORDER_MAINSTART;
		}
	}
	else if(((CButton *)dlg->GetDlgItem(IDC_RADIO_ERASEFLASH))->GetCheck())//擦除FLASH
	{
		orderList[0] = ORDER_BOOT;
		orderList[1] = ORDER_KEY;
		orderList[2] = ORDER_ERASE;
		orderList[3] = ORDER_BOOTEND;
		orderList[4] = ORDER_GETVERSION;
	}
	//启用密码从BOOT命令开始
	if( ((CComboBox*)dlg->GetDlgItem(IDC_COMBO_ENCRYPTION))->GetCurSel() == 0)
	{
		j = 0;
		state = orderList[j++];
	}
	else //禁用密码从KEY命令开始
	{
		j = 1;
		state = orderList[j++];
	}

	while(state != 0)
	{
		dlg->sendThreadState = state;
		//发送Boot
		if(state == ORDER_BOOT)
		{
			state = (dlg->OrderBoot())? orderList[j++]:0;
		}
		else if(state == ORDER_KEY)
		{
			state = (dlg->OrderKey())? orderList[j++]:0;
		}
		else if(state == ORDER_ERASE)
		{
			state = (dlg->OrderErase())? orderList[j++]:0;
		}
		else if(state == ORDER_SPERASE)
		{
			state = (dlg->OrderSPErase())? orderList[j++]:0;
		}
		else if(state == ORDER_PROGRAM)
		{
			state = (dlg->OrderProgram())? orderList[j++]:0;
		}
		else if(state == ORDER_PROGDATA)
		{
			state = (dlg->OrderProgData())? orderList[j++]:0;
		}
		else if(state == ORDER_GETVERSION)
		{
			state = (dlg->OrderGetVersion())? orderList[j++]:0;
		}
		else if(state == ORDER_BOOTEND)
		{
			state = (dlg->OrderBootEnd())? orderList[j++]:0;
		}
		else if(state == ORDER_MAINSTART)
		{
			state = (dlg->OrderMainStart())? orderList[j++]:0;
		}
	}		

	dlg->GetDlgItem(IDC_BUTTON_CONNECTCAN)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_BUTTON_STARTBOOTLOADER)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_RADIO_WRITEDATA)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_RADIO_ERASEFLASH)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_RADIO_ERASEANDPROGRAM)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_CHECK_STARTFROMMAIN)->EnableWindow(TRUE);

	state = orderList[j-2];
	if(ORDER_GETVERSION == state || ORDER_MAINSTART == state)
	{
		//记录烧写时间
		float timeUsed = 0;
		tickEnd = GetTickCount();
		if(tickEnd < tickStart)
		{
			timeUsed = (0xFFFFFFFF + tickEnd-tickStart)/1000.0;
		}
		else
		{
			timeUsed = (tickEnd-tickStart)/1000.0;
		}
		
		if(ORDER_ERASE == orderList[j-4])
		{
			str.Format(_T("擦除共用时间: %.3fs"),(tickEnd-tickStart)/1000.0);
		}
		else
		{
			str.Format(_T("烧写共用时间: %.3fs 数据传输速度: %dKbps"),timeUsed, (DWORD)((dlg->canFrameCount * 64.0)/timeUsed/1000.0));
		}
		dlg->ShowInfo(str);
	}
	//关闭打开的文件
	if (NULL!=dlg->fileToWrite)
	{
		dlg->fileToWrite->Close();
		delete dlg->fileToWrite;
		dlg->fileToWrite=NULL;
	}
	dlg->canFrameCount = 0;
	dlg->GetDlgItem(IDC_BUTTON_STARTBOOTLOADER)->EnableWindow(TRUE);
	return 0;
}


void CMFCApplication3Dlg::OnClose()
{
	// TODO:
	//MessageBox(_T("就要关闭我了，好怕怕"),MB_OK);
	DisConnectCan();
	CDialogEx::OnClose();
}



void CMFCApplication3Dlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}

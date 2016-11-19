
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
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMFCApplication3Dlg::OnBnClickedButtonTest)
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
	SetWindowText(_T("MPC5744P BootLoader通信_DEBUG_MODE"));
	((CButton*)GetDlgItem(IDC_BUTTON_TEST))->ShowWindow(SW_NORMAL);
#else
	SetWindowText(_T("MPC5744P BootLoader通信"));
#endif

	//加载菜单栏
	m_Menu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_Menu);
	DrawMenuBar();


	//设置状态栏
	HWND hDlg = GetSafeHwnd();
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_BORDER,
		TEXT("就绪"),	//显示在状态栏上的信息
		hDlg,			//父窗口句柄
		IDS_STATUS);	// 预定义的资源ID，相当于状态栏的ID号：GetDlgItem(IDS_STATUS)
	
	int pint[2] = { 350,-1 };
	//状态栏第一个方格右边界离窗口客户区左边界的距离为100
	//第二个方格右边界离窗口客户区左边界的距离为200
	//...以此类推
	//-1表示该方格的右边界为为窗口客户区的右边界
	::SendMessage(hStatusWindow, SB_SETPARTS, 2, (LPARAM)pint);
	::SendMessage(hStatusWindow, SB_SETTEXT, 0, (LPARAM)TEXT("CAN未连接"));
	::SendMessage(hStatusWindow, SB_SETTEXT, 1, (LPARAM)TEXT("下载 0%"));



	//设置默认值
	((CComboBox*)GetDlgItem(IDC_COMBO_CANTYPE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->SetCurSel(1);
	((CComboBox*)GetDlgItem(IDC_COMBO_MCU))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_ENCRYPTION))->SetCurSel(0);
	//0x00FE0000
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->SetWindowTextW(_T("0x00FE0000"));
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->SetWindowTextW(_T("0x"));
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->SetWindowTextW(_T("0x"));
	((CButton *)GetDlgItem(IDC_RADIO_ERASEANDPROGRAM))->SetCheck(TRUE);
	//禁用地址
	((CEdit*)GetDlgItem(IDC_EDIT_STARTADDRESS))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_ENDADDRESS))->EnableWindow(FALSE);

	m_Connect = FALSE;

	//接收和发送数据缓冲区
	receiceData = new BaseType(8);
	sendData1 = new BaseType(8);
	sendData2 = new BaseType(24);

	//线程事件
	receiveEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	exitEvent1 = CreateEvent(NULL,FALSE,FALSE,NULL);
	exitEvent2 = CreateEvent(NULL,FALSE,FALSE,NULL);

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



void CMFCApplication3Dlg::OnBnClickedButtonTest()
{
	// DEBUG模式下有效，否则隐藏

#if 1
	UCHAR a,b,c;
	a = 0;
	b=0;
	c=a+b;
	a=0;
	b=0;
#endif
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
		returnCode = DisConnectCan(a,b,c);
		m_Connect = FALSE;
		((CButton*)GetDlgItem(IDC_BUTTON_STARTBOOTLOADER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_CONNECTCAN))->SetWindowTextW(_T("连接CAN"));

		::SendMessage(hStatusWindow, SB_SETTEXT, 0, (LPARAM)TEXT("CAN已断开"));
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
			::SendMessage(hStatusWindow, SB_SETTEXT, 0, (LPARAM)TEXT("CAN已连接"));
			break;

		default:
			//未知错误
			::SendMessage(hStatusWindow, SB_SETTEXT, 0, (LPARAM)TEXT("CAN 未知错误"));
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

#if 0

	fileToWrite = new CAnalysisFile(filePathName);

	fileToWrite->CheckSrecFile();
	fileToWrite->SeekToBegin();


	if (!fileToWrite->SetArrange(TRUE,0x00fa0000,0x00fa0001))
	{
		return;
	}

	int a=FILE_READ_NORMAL;
	while(a==FILE_READ_NORMAL)
	{
		try{
			a=fileToWrite->ReadNextLine();
		}catch(char *s)
		{
			CString tmp(s);
			AfxMessageBox(tmp);
			return;
		}
		
		CString tmp;

		switch(a)
		{
		case FILE_READ_NORMAL:
			//正常发送

			if(!GenerateSendOrder(
				ORDER_PROGRAM,
				fileToWrite->GetDataSendLength(),
				fileToWrite->GetDataSend(),
				fileToWrite->GetDataSendAddr()))
					return ;
				SendOrder(sendData2);

				tmp.Format(_T("下载 %d"),fileToWrite->GetSendedPercent());
				tmp+="%";
				::SendMessage(hStatusWindow, SB_SETTEXT, 1, (LPARAM)tmp.GetBuffer());
			break;
		case FILE_READ_ERROR:
			//读取错误
			return ;
			break;
		case FILE_READ_END:
			//文件结束
			tmp.Format(_T("下载 %d"),fileToWrite->GetSendedPercent());
			tmp+="%";
			::SendMessage(hStatusWindow, SB_SETTEXT, 1, (LPARAM)tmp.GetBuffer());

			ShowInfo(_T("读取结束"),0);
			break;
		}

		Sleep(200);
	}

	if (fileToWrite!=NULL)
	{
		fileToWrite->Close();
		delete fileToWrite;
		fileToWrite = NULL;
	}
#endif

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
	
	//step1.获取密码
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->GetWindowText(str);
	if (!CStringToUINT(str,passWord,_T("密码")))
	{
		return;
	}
	//密码已经产生，在passWord

	//step2.判断是否选择了文件
	if (filePathName.IsEmpty())
	{
		AfxMessageBox(_T("请选择SREC文件"));
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
			AfxMessageBox(_T("文件打开失败，请重试！"));
			ShowInfo(_T("文件打开失败，请重试！"),0);
			ShowInfo(_T("退出BootLoader"),0);
			return;
		}else if (fileToWrite->CheckSrecFile()==FILE_ADDRESS_ERROR)
		{
			AfxMessageBox(_T("文件格式错误，请检查！"));
			ShowInfo(_T("文件格式错误，请检查！"),0);
			ShowInfo(_T("退出BootLoader"),0);
			fileToWrite->Close();
			delete fileToWrite;
			fileToWrite = NULL;
			return;
		}
		ShowInfo(_T("文件打开成功！"),0);
	}

	//step3.获取操作模式

	int l_writeData = ((CButton *)GetDlgItem(IDC_RADIO_WRITEDATA))->GetCheck();
	int l_erase = ((CButton *)GetDlgItem(IDC_RADIO_ERASEFLASH))->GetCheck();
	int l_program = ((CButton *)GetDlgItem(IDC_RADIO_ERASEANDPROGRAM))->GetCheck();
	m_startFromMain = ((CButton *)GetDlgItem(IDC_CHECK_STARTFROMMAIN))->GetCheck();
	if (1==l_writeData)
	{
		//只写数据
		CString str;
		((CEdit *)GetDlgItem(IDC_EDIT_STARTADDRESS))->GetWindowTextW(str);
		if(!CStringToUINT(str,startAddress,_T("地址")))
		{
			return;
		}

		if (startAddress%8!=0)
		{
			AfxMessageBox(_T("起始地址设置错误，地址必须为8的整数倍！"));
			ShowInfo(_T("起始地址设置错误，地址必须为8的整数倍！"),0);
			ShowInfo(_T("退出BootLoader"),0);
			return;
		}

		((CEdit *)GetDlgItem(IDC_EDIT_ENDADDRESS))->GetWindowTextW(str);
		if(!CStringToUINT(str,stopAddress,_T("地址")))
		{
			return;
		}

		if (!fileToWrite->SetArrange(FALSE,startAddress,stopAddress))
		{
			AfxMessageBox(_T("地址设置错误，请重试！"));
			return;
		}
		//启动线程
		AfxBeginThread(SendThreadProgram,this);
		return;
	}else if (1==l_erase)
	{
		//擦除
		//启动线程
		AfxBeginThread(SendThreadErase,this);
		return;
	}else if (1==l_program)
	{
		//编程
		if (!fileToWrite->SetArrange(TRUE))
		{
			AfxMessageBox(_T("地址设置错误，请重试！"));
			return;
		}
		AfxBeginThread(SendThreadProgram,this);
		return;
	}


}

BOOL CMFCApplication3Dlg::CStringToUINT(const CString &str,UINT& d,CString mess)
{
	int strLength = str.GetLength();
	if (strLength!=10)
	{
		AfxMessageBox(mess+_T("长度错误，请输入4字节地址，共计8位，以0x开头"));
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
			AfxMessageBox(mess+_T("格式错误，只支持\"0-9\",\"a-f\"以及\"A-F\"之间的字符，以0x开头"));
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
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(TRUE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->EnableWindow(TRUE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_SKIPPROMPT))->EnableWindow(TRUE);
		
		break;
	case 1:
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWROD))->EnableWindow(FALSE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_DEFAULTPASSWORD))->EnableWindow(FALSE);
		((CCheckListBox*)GetDlgItem(IDC_CHECK_SKIPPROMPT))->EnableWindow(FALSE);
		break;
	}
	
}

int CMFCApplication3Dlg::ConnectCan(int typeIndex,int channel,int baudRateIndex)
{
	const unsigned char baudRate[5][2]={
		{0x00,0x14},//1000Kbps
		{0x00,0x1C},//500Kbps
		{0x01,0x1C},//250Kbps
		{0x04,0x1C}//100Kbps
	};
	const unsigned int devtype[2]={
		VCI_USBCAN2,
		VCI_USBCAN_2E_U
	};
	DWORD devindex = 0;
	VCI_INIT_CONFIG init_config;
	init_config.AccCode = 0x00000000;//验收码
	init_config.AccMask = 0xFFFFFFFF;//屏蔽码
	init_config.Timing0 = baudRate[baudRateIndex][0];//设置波特率
	init_config.Timing1 = baudRate[baudRateIndex][1];
	init_config.Filter = 1;//单滤波
	init_config.Mode = 0;//正常模式

	m_devtype = devtype[typeIndex];
	m_devind = 0;//CAN设备索引号
	m_cannum = channel;//

	if(VCI_OpenDevice(m_devtype, m_devind, 0) != STATUS_OK)
	{
		MessageBox(_T("打开设备失败!"),_T("警告"), MB_OK|MB_ICONQUESTION);
		return CAN_OPENDEV_ERROR;
	}
	if(VCI_InitCAN(m_devtype, m_devind, m_cannum, &init_config) !=STATUS_OK)
	{
		MessageBox(_T("初始化CAN失败!"),_T("警告"), MB_OK|MB_ICONQUESTION);
		VCI_CloseDevice(m_devtype, m_devind);
		return CAN_INITDEV_ERROR;
	}

	return CAN_CONNECT_OK;
}
int CMFCApplication3Dlg::DisConnectCan(int canType,int channel,int baudRate)
{
	Sleep(500);//断开连接之前先休眠
	VCI_CloseDevice(m_devtype,m_devind);
	return CAN_DISCONNECT_OK;
}
void CMFCApplication3Dlg::ShowInfo(CString str, int code)
{
	m_ListInfo.InsertString(m_ListInfo.GetCount(),str);
	m_ListInfo.SetCurSel(m_ListInfo.GetCount()-1);
}
void CMFCApplication3Dlg::ShowErrMessageBox(CString err)
{
	AfxMessageBox(err);
}

BOOL CMFCApplication3Dlg::GenerateSendOrder( char order,UCHAR len,const UCHAR *d,UINT addr/*=0*/ )
{
	int random;
	switch(order)
	{
	case ORDER_BOOT:
		sendData1->SetData(ORDER_BOOT,len,passWord);
		break;
	case ORDER_KEY:
		srand(time(NULL)); 
		random = rand();
		receiceData->random =random;
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
	}

	return TRUE;
}

void CMFCApplication3Dlg::SendOrder(const BaseType *sendframe)
{
	VCI_CAN_OBJ canframe;
	canframe.SendType = 0;//0:正常发送，1：单次发送，2：自发自收，3：单次自发自收
	canframe.ExternFlag = 0;//0:标准帧,1:扩展帧
	canframe.RemoteFlag = 0;//禁用远程帧
	canframe.DataLen = 8;

	if((ORDER_BOOT == sendframe->command) || (ORDER_KEY == sendframe->command))//帧格式1
	{
		if(sendframe->dataLength != 8)
		{
			//TODO:数据长度异常
			ShowInfo(_T("CAN发送数据长度异常"),0);
		}
		else
		{
			canframe.ID = MSGID_FARME1;//帧格式1 ID
			memcpy(&canframe.Data, &sendframe->allData, 8);
			if(1 == VCI_Transmit(m_devtype, m_devind, m_cannum, &canframe, 1))//发送成功
			{
				//do nothing
			}
			else
			{
				//TODO：发送失败
				ShowInfo(_T("CAN发送失败"),0);
			}
		}
	}
	else//帧格式2
	{
		if(sendframe->dataLength != 24)
		{
			//TODO:数据长度异常
		}
		else
		{
			USHORT canframeNum = 0;
			ULONG transmitStatus = 0;
			for(canframeNum = 0; canframeNum < 3; canframeNum++)
			{
				canframe.ID = MSGID_FARME2[canframeNum];
				memcpy(&canframe.Data, &(sendframe->allData[canframeNum*8]), 8);
				if(1 == VCI_Transmit(m_devtype, m_devind, m_cannum, &canframe, 1))//发送成功
				{
					//do nothing
				}
				else
				{
					//TODO：一次发送失败，跳出 or 重试 is a question
					ShowInfo(_T("CAN发送失败"),canframeNum);
				}				
			}
		}
	}
}

UINT CMFCApplication3Dlg::ReceiveThread( void *param )
{
	CMFCApplication3Dlg *dlg = (CMFCApplication3Dlg *)param;
	VCI_CAN_OBJ frameinfo[50];
	VCI_ERR_INFO errinfo;
	int len = 1;
	int i = 0;
	while(1)
	{
		Sleep(1);
		if(FALSE == dlg->m_Connect)
			break;
		len=VCI_Receive(dlg->m_devtype, dlg->m_devind, dlg->m_cannum, frameinfo, 50, 200);
		if(len<=0)
		{
			//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
			//千万不能省略这一步（即使你可能不想知道错误码是什么）
			VCI_ReadErrInfo(dlg->m_devtype, dlg->m_devind, dlg->m_cannum, &errinfo);
		}
		else
		{
			/*
			for(i = 0; i < len; i++)
			{
				if(frameinfo[i].RemoteFlag != 0)//
				{
					//TODO:出现了远程帧
				}
				else
				{
					if(frameinfo[i].ID == MSGID_FRAMEREV)//接收正确的帧ID
					{
						dlg->receiceData->dataLength = frameinfo[i].DataLen;
					}
				}
			}*/
			if(frameinfo[len-1].RemoteFlag != 0)//每次都读最新的数据
			{
				//TODO:出现了远程帧
				dlg->ShowInfo(_T("出现远程帧"),0);
			}
			else
			{
				if((frameinfo[len-1].ID == MSGID_FRAMEREV) && (frameinfo[len-1].DataLen == 8))//接收正确的帧ID
				{
					dlg->receiceData->SetAllData((const char *)(&(frameinfo[len-1].Data[0])), 8);
					if((dlg->receiceData->GetCheck() == frameinfo[len-1].Data[7]) && (dlg->receiceData->allData[0] == 0xA5))
					{
						dlg->receiceData->startSign = frameinfo[len-1].Data[0];
						dlg->receiceData->returnValue = frameinfo[len-1].Data[1];
						dlg->receiceData->dataLength = frameinfo[len-1].Data[2];
						dlg->receiceData->m_check = frameinfo[len-1].Data[7];
					}
					else
					{
						//接收数据帧错误
						memset(dlg->receiceData->allData, 0x00, dlg->receiceData->totalLength);
						dlg->receiceData->returnValue = 0x00;
					}
					
				}
			}
		}
	}

	return 0;
}

UINT CMFCApplication3Dlg::SendThreadErase( void *param )
{
	CMFCApplication3Dlg* dlg = (CMFCApplication3Dlg*)param;


	//sendOrder(boot);
	//sendOrder(key);
	//sendOrder(program);
	//sendOrder(boot_end);
	//sendOrder(get_version);
	int i;
	BOOL exitSign;

	//1.发送Boot
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(boot);
		if(!dlg->GenerateSendOrder(ORDER_BOOT,4,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData1);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)//WAIT_TIMEOUT
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case PASSWORD_OK:
				dlg->ShowInfo(_T("密码正确"),0);
				exitSign = TRUE;
				break;
			case PASSRORD_NOTOK:
				dlg->ShowErrMessageBox(_T("密码错误"));
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return PASSRORD_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("BOOT命令校验错误，重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}
		
	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定，终止发送"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),0);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//2.发送key
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(key);
		if(!dlg->GenerateSendOrder(ORDER_KEY,4,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData1);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case KEY_OK:
				exitSign = TRUE;
				dlg->ShowInfo(_T("校验通过"),0);
				break;
			case KEY_NOTOK://修改后的应答表里无此项，可删除
				dlg->ShowErrMessageBox(_T("校验错误"));
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return KEY_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowErrMessageBox(_T("校验错误"));
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return DATA_ERR;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),i);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//3.发送erase
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(erase);
		if(!dlg->GenerateSendOrder(ORDER_ERASE,0,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case ERASE_OK:
				exitSign = TRUE;
				dlg->ShowInfo(_T("擦除完成"),0);
				break;
			case ERASE_NOTOK:
				dlg->ShowErrMessageBox(_T("擦除flash失败"));
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return ERASE_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("ERASE命令校验错误，重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定，擦除命令发送失败"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),i);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//4.发送boot_end
	i = 0;
	exitSign = FALSE;
	CString tmp=_T(""),tmp1=_T("");
	do 
	{
		i++;
		//sendOrder(boot_end);
		if(!dlg->GenerateSendOrder(ORDER_BOOTEND,0,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case BOOTEND_OK:
				exitSign = TRUE;
				//在6中显示数据传输错误次数：x(data中第一字节)，flash写入失败次数，y(data中第二字节)
				tmp1.Format(_T("数据传输错误次数:%d次, "),dlg->receiceData->allData[3]);
				tmp = tmp1;
				tmp1.Format(_T(" flash写入失败的次数:%d次"),dlg->receiceData->allData[4]);
				tmp+= tmp1;
				dlg->ShowInfo(tmp,0);
				break;
			case BOOTEND_NOTOK:
				dlg->ShowInfo(_T("退出Boot失败，执行GetVersion命令"),0);
				exitSign = TRUE;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("BOOT_END命令校验错误，重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),i);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//5.发送get_version
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(get_version);
		if(!dlg->GenerateSendOrder(ORDER_GETVERSION,0,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case GETVERSION_OK:
				tmp = "BootLoader版本为: ";
				tmp1.Format(_T("%d.%d"),
					dlg->receiceData->allData[3],
					dlg->receiceData->allData[4]);
				tmp+= tmp1;
				dlg->ShowInfo(tmp,0);
				exitSign = TRUE;
				break;
			case GETVERSION_NOTOK:
				/*SetEvent(dlg->exitEvent1);*/
				dlg->ShowInfo(_T("获取BootLoader版本失败"),0);
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return GETVERSION_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("GET_VERSION校验错误"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}
	dlg->ShowInfo(_T("BootLoader完成"),0);
	return 0;
}

UINT CMFCApplication3Dlg::SendThreadProgram( void *param )
{
	CMFCApplication3Dlg* dlg = (CMFCApplication3Dlg*)param;

	//sendOrder(boot);
	//sendOrder(key);
	//sendOrder(Erase);
	//sendOrder(boot_end);
	//sendOrder(get_version);
	int i;
	BOOL exitSign;

	//1.发送Boot
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(boot);
		if(!dlg->GenerateSendOrder(ORDER_BOOT,4,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData1);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)//WAIT_TIMEOUT
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case PASSWORD_OK:
				dlg->ShowInfo(_T("密码正确"),0);
				exitSign = TRUE;
				break;
			case PASSRORD_NOTOK:
				/*SetEvent(dlg->exitEvent1);*/
				dlg->ShowErrMessageBox(_T("密码错误"));
				dlg->ShowInfo(_T("密码错误"),0);
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return PASSRORD_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("密码校验错误,重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),0);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//2.发送key
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(key);
		if(!dlg->GenerateSendOrder(ORDER_KEY,4,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData1);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case KEY_OK:
				dlg->ShowInfo(_T("校验通过"),0);
				exitSign = TRUE;
				break;
			case KEY_NOTOK:
				/*SetEvent(dlg->exitEvent1);*/
				dlg->ShowErrMessageBox(_T("校验错误"));
				dlg->ShowInfo(_T("校验错误，发送终止"),0);
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return KEY_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("校验错误，发送终止"),0);
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return DATA_ERR;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		dlg->ShowInfo(_T("连接不稳定，终止发送"),0);
		dlg->ShowInfo(_T("退出BootLoader"),0);
		return DATA_ERR;
	}

	//3.发送PROGRAM
	CString tmp;
	int a=FILE_READ_NORMAL;
	while(a==FILE_READ_NORMAL)
	{
		a=dlg->fileToWrite->ReadNextLine();
		switch(a)
		{
		case FILE_READ_NORMAL:
			//正常发送
			i = 0;
			exitSign = FALSE;
			do 
			{
				i++;
				//sendOrder(erase);
				if(!dlg->GenerateSendOrder(
					ORDER_PROGRAM,
					dlg->fileToWrite->GetDataSendLength(),
					dlg->fileToWrite->GetDataSend(),
					dlg->fileToWrite->GetDataSendAddr()))
					return -1;
				dlg->SendOrder(dlg->sendData2);
				if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
				{
					//收到数据，判断数据是否正确
					switch(dlg->receiceData->returnValue)
					{
					case PROGRAM_OK:
						exitSign = TRUE;

						tmp.Format(_T("下载 %d"),dlg->fileToWrite->GetSendedPercent());
						tmp+="%";
						::SendMessage(dlg->hStatusWindow, SB_SETTEXT, 1, (LPARAM)tmp.GetBuffer());

						break;
					case PROGRAM_NOTOK:
					case DATA_ERR:
						exitSign = FALSE;
						break;
					}
				}
				else
				{
					//超时，提示并退出
#ifdef _TEST
					exitSign = TRUE;
#else
					dlg->ShowErrMessageBox(_T("连接失败"));
					return -1;
#endif
				}

			} while (i<10 && !exitSign);

			if (i==10&&exitSign==FALSE)
			{
				//连接不稳定，退出
				dlg->ShowErrMessageBox(_T("连接不稳定"));
				return DATA_ERR;
			}
			break;
		case FILE_READ_ERROR:
			//读取错误
			dlg->ShowInfo(_T("读取错误，停止发送"),0);
			return FILE_READ_ERROR;
			break;
		case FILE_READ_END:
			//文件结束
			tmp.Format(_T("下载 %d"),dlg->fileToWrite->GetSendedPercent());
			tmp+="%";
			::SendMessage(dlg->hStatusWindow, SB_SETTEXT, 1, (LPARAM)tmp.GetBuffer());

			dlg->ShowInfo(_T("读取结束"),0);
			break;
		}
	}

	CString tmp1;

	//4.发送boot_end
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(boot_end);
		if(!dlg->GenerateSendOrder(ORDER_BOOTEND,0,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case BOOTEND_OK:
				exitSign = TRUE;
				//在6中显示数据传输错误次数：x(data中第一字节)，flash写入失败次数，y(data中第二字节)
				tmp1.Format(_T("数据传输错误次数:%d次, "),dlg->receiceData->allData[3]);
				tmp = tmp1;
				tmp1.Format(_T(" flash写入失败的次数:%d次"),dlg->receiceData->allData[4]);
				tmp+= tmp1;
				dlg->ShowInfo(tmp,0);
				break;
			case BOOTEND_NOTOK:
				dlg->ShowInfo(_T("退出Boot失败，执行GetVersion命令"),0);
				exitSign = TRUE;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("BOOT_END命令校验错误，重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		return DATA_ERR;
	}

	//5.发送get_version
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(get_version);
		if(!dlg->GenerateSendOrder(ORDER_GETVERSION,0,NULL))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case GETVERSION_OK:
				tmp = "BootLoader版本为: ";
				tmp1.Format(_T("%d.%d"),
					dlg->receiceData->allData[3],
					dlg->receiceData->allData[4]);
				tmp+= tmp1;
				dlg->ShowInfo(tmp,0);
				exitSign = TRUE;
				break;
			case GETVERSION_NOTOK:
				dlg->ShowInfo(_T("获取BootLoader版本失败"),0);
				dlg->ShowInfo(_T("退出BootLoader"),0);
				return GETVERSION_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("GET_VERSION校验错误"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		return DATA_ERR;
	}

	//6.从main启动
	if (dlg->m_startFromMain!=1)
	{
		dlg->ShowInfo(_T("BootLoader完成"),0);
		return 0;
	}
	i = 0;
	exitSign = FALSE;
	do 
	{
		i++;
		//sendOrder(main_start);
		if(!dlg->GenerateSendOrder(
			ORDER_MAINSTART,
			4,
			NULL,
			dlg->fileToWrite->GetMainStartAddr()))
			return -1;
		dlg->SendOrder(dlg->sendData2);
		if (WaitForSingleObject(dlg->receiveEvent,1000)==WAIT_OBJECT_0)//WAIT_TIMEOUT
		{
			//收到数据，判断数据是否正确
			switch(dlg->receiceData->returnValue)
			{
			case MAINSTART_OK:
				dlg->ShowInfo(_T("主函数进入成功"),0);
				exitSign = TRUE;
				break;
			case MAINSTART_NOTOK:
				dlg->ShowInfo(_T("主函数进入不成功"),0);
				return MAINSTART_NOTOK;
				break;
			case DATA_ERR:
				dlg->ShowInfo(_T("MAINSTART命令校验错误，重新发送"),i);
				exitSign = FALSE;
				break;
			}
		}
		else
		{
			//超时，提示并退出			
#ifdef _TEST
			exitSign = TRUE;
#else
			dlg->ShowErrMessageBox(_T("连接失败"));
			return -1;
#endif
		}

	} while (i<10 && !exitSign);

	if (i==10&&exitSign==FALSE)
	{
		//连接不稳定，退出
		dlg->ShowErrMessageBox(_T("连接不稳定"));
		return DATA_ERR;
	}
	dlg->ShowInfo(_T("BootLoader完成"),0);
	return 0;
}

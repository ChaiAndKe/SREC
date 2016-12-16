
// MFCApplication3.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication3App

BEGIN_MESSAGE_MAP(CMFCApplication3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApplication3App ����

CMFCApplication3App::CMFCApplication3App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMFCApplication3App ����

CMFCApplication3App theApp;


// CMFCApplication3App ��ʼ��

BOOL CMFCApplication3App::InitInstance()
{

	hMutex = ::CreateMutex(NULL,FALSE,_T("Bootloader_DianQingKeJi"));
	if (hMutex!=NULL)
	{
		if (GetLastError()==ERROR_ALREADY_EXISTS)
		{
			return FALSE;
		}
	}

// 	HANDLE hSem = CreateSemaphore(NULL,1,1,m_pszExeName);
// 	if (GetLastError()==ERROR_ALREADY_EXISTS)
// 	{
// 		CloseHandle(hSem);
// 		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);
// 		while(::IsWindow(hWndPrevious))
// 		{
// 			if (::GetProp(hWndPrevious,m_pszExeName))
// 			{
// 				if (::IsIconic(hWndPrevious))
// 					::ShowWindow(hWndPrevious,SW_RESTORE);
// 				::SetForegroundWindow(hWndPrevious);
// 				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
// 
// 				return FALSE;
// 			}
// 			hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT);
// 		}
// 		return FALSE;
// 	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMFCApplication3Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CMFCApplication3App::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (hMutex!=NULL)
	{
		CloseHandle(hMutex);
	}
	return CWinApp::ExitInstance();
}

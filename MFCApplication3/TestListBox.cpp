#include "StdAfx.h"
#include "TestListBox.h"
#include "resource.h"

CTestListBox::CTestListBox(void)
{
}


CTestListBox::~CTestListBox(void)
{
}
BEGIN_MESSAGE_MAP(CTestListBox, CListBox)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_CLEAR, &CTestListBox::OnMenuClear)
END_MESSAGE_MAP()


void CTestListBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMenu menu;
	menu.LoadMenu(IDR_MENU2);
	
	CMenu *submenu;
	submenu=menu.GetSubMenu(0);
	
	CPoint tmppoint=point;
	ClientToScreen(&tmppoint);
	submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,tmppoint.x,tmppoint.y,
		this);

	CListBox::OnRButtonDown(nFlags, point);
}


void CTestListBox::OnMenuClear()
{
	// TODO: �ڴ���������������
	DWORD len=GetCount();
	for(DWORD i=0;i<len;i++)
		DeleteString(0);
}

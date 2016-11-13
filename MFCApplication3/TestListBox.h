#pragma once
#include "afxwin.h"
class CTestListBox :
	public CListBox
{
public:
	CTestListBox(void);
	~CTestListBox(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuClear();
};

